# 7 — Software Testing, Bug Fix and Retest

Contents: [Testing Tools](Testing_Tools.md), [Test Execution Report](Test_Execution_Report.md), and [AI-Assisted Bug Fix and Retest](AI_Assisted_Bug_Fix_and_Retest.md).

The canonical report is [`docs/Test_Report.md`](../docs/Test_Report.md). It records the historical
161-check Sprint 2 baseline, the 164-check `v1.0-final` release suite (`test_parser`, `test_executor`,
`test_shell.sh`, and `test_termination.py`) across `make test` and `make asan-test`, `make valgrind`
results, target environment, flaky-run record, and known limitations. The Part-3 expanded suite adds
lifecycle and resource-failure cases for 182 checks per build; its results are broken down in the
[Unit Test Report](../docs/Unit_Test_Report.md), [System Validation Report](../docs/System_Validation_Report.md),
[Code Coverage Report](../docs/Code_Coverage_Report.md), and
[Branch Coverage Report](../docs/Branch_Coverage_Report.md). The [resource
audit](../docs/Resource_Audit.md) records memory, descriptor, and termination findings.

The bug-fix record is populated only with a real defect and real retest evidence; no defect or
result is fabricated.
