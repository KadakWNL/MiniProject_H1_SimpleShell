# Resource audit for S2-06 / issue #15

The audit covers parser and executor memory, repeated pipeline descriptor
counts, shell exit with a background job, and SIGTERM while idle or waiting
for a foreground command. `make test` includes bounded SIGTERM regressions;
`make asan-test` repeats these under AddressSanitizer and UBSan.
`make valgrind` checks both unit harnesses and a shell pipeline/background/exit
scenario. SIGTERM scenarios run under sanitizers, rather than Valgrind.

Background stages and noninteractive foreground stages have process groups
so cleanup also signals descendants. Interactive foreground commands retain
the terminal's process group; this shell does not implement terminal job
control. Cleanup of those commands targets direct children. Descendants that
explicitly detach into a different process group are outside this audit.
SIGKILL cannot run cleanup handlers. This is bounded audit evidence, not a
guarantee against every possible abnormal exit or hostile child command.
