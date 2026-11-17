"""Summarize GCC gcov JSON for production C files, excluding test harnesses."""
import gzip
import json
from pathlib import Path

directory = Path('docs/evidence/coverage')
rows = []
for path in sorted(directory.glob('*.gcov.json.gz')):
    with gzip.open(path, 'rt') as stream:
        report = json.load(stream)
    for source in report['files']:
        if not source['file'].startswith('src/'):
            continue
        lines = source['lines']
        branches = [branch for line in lines for branch in line.get('branches', [])]
        rows.append((source['file'], sum(line['count'] > 0 for line in lines),
                     len(lines), sum(branch['count'] > 0 for branch in branches), len(branches)))
if len(rows) != 5:
    raise SystemExit(f'Expected five production source reports, found {len(rows)}')

def percentage(hit, total):
    return f'{100 * hit / total:.2f}%' if total else 'N/A'

for title, filename, hit_index, total_index in (
    ('Code Coverage Report', 'Code_Coverage_Report.md', 1, 2),
    ('Branch Coverage Report', 'Branch_Coverage_Report.md', 3, 4),
):
    hit = sum(row[hit_index] for row in rows)
    total = sum(row[total_index] for row in rows)
    text = [f'# {title}', '', '## Recorded measurement', '',
            'See [validation manifest](evidence/Validation_Manifest.md) for source SHA, environment, commands, and limitations.', '',
            '| Production source | Covered | Instrumented | Coverage |', '|---|---:|---:|---:|']
    for row in rows:
        text.append(f'| `{row[0]}` | {row[hit_index]} | {row[total_index]} | {percentage(row[hit_index], row[total_index])} |')
    text += [f'| **Total** | **{hit}** | **{total}** | **{percentage(hit, total)}** |', '',
             '## Method and limits', '',
             '`make coverage` rebuilds at `-O0 --coverage`, runs the complete Linux suite, and exports GCC gcov JSON with branch counts. '
             'Only `src/*.c` contributes to the denominator; test harnesses and external programs are excluded. '
             'Branch coverage means outgoing gcov branch edges taken at least once, not condition/MC/DC coverage. '
             'Forked children that exec or call `_exit` may not flush their counters; their behavior can be tested while gcov reports their code uncovered. '
             'Uncovered error paths include failures that require syscall/allocation fault injection. No 100% coverage claim is made.', '',
             'Raw evidence: [coverage run](evidence/coverage/test.log), [gcov output](evidence/coverage/gcov.log), and compressed JSON files in `docs/evidence/coverage/`.', '']
    Path('docs', filename).write_text('\n'.join(text), encoding='utf-8')
