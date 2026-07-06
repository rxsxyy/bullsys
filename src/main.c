
#include "global.h"
#include <stdio.h>

static void usage(const char *prog) {
    printf("Usage: %s <options> <profile> <action>\n", prog);
    printf("\nOptions:\n");
    printf("  -f FILE   Specify .bull file (default: build.bull)\n");
    printf("  -v        Verbose output\n");
    printf("  -n        Dry run (print commands without executing)\n");
    printf("  --help    Show this help\n");
    printf("  --version Shows executable version\n");
}

i32 main(i32 argc, char *argv[]) {

    usage(argv[0]);

    return 0;
}