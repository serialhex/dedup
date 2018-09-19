REM Windows Build File

cl /I lib dedup.c

REM clang --target=x86_64-unknown-freebsd -o dedup dedup.c
