
#include "global.h"
#include "section.h"

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// help message
static void usage(const char *prog) {
    printf("Usage: %s <options> <action>\n", prog);
    printf("\nOptions:\n");
    printf("  -f FILE   Specify .bull file (default: build.bull)\n");
    printf("  -v        Verbose output\n");
    printf("  -n        Dry run (print commands without executing)\n");
    printf("  --help    Show this help\n");
    printf("  --version Shows executable version\n");
}

i32 main(i32 argc, char *argv[]) {
    const char *bull_file = "build.bull";
    bool verbose = false;
    bool dry = false;

    static struct option long_opts[] = {
        {"help",    no_argument, 0, 'h'},
        {"version", no_argument, 0, 'V'},
        {0, 0, 0, 0}
    };

    i32 opt;
    while ((opt = getopt_long(argc, argv, "f:vn", long_opts, NULL)) != -1) {
        switch (opt) {
            case 'f':
                bull_file = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'n':
                dry = true;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            case 'V':
                printf("bullsys %s\n", BULLSYS_VERSION);
                return 0;
            default:
                usage(argv[0]);
                return 1;
        }
    }

    const char *section_name = (optind < argc) ? argv[optind++] : NULL;
    Section profile;
    if (section_load(bull_file, section_name, &profile) != 0) {
        fprintf(stderr, "error: failed to load profile\n");
        return 1;
    }

    char *cmd = section_build_command(&profile);
    if (!cmd) {
        fprintf(stderr, "error: failed to build command\n");
        section_free(&profile);
        return 1;
    }

    if (verbose || dry) {
        printf("%s\n", cmd);
    }

    i32 status = 0;
    if (!dry) {
        status = system(cmd);
        if (status != 0 && !verbose) {
            fprintf(stderr, "failed: %s\n", cmd);
        }
    }

    free(cmd);
    section_free(&profile);

    return status == 0 ? 0 : 1;
}
