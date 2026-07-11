
#include "section.h"

#include <iniparser/iniparser.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static char *dup_or(const char *s, const char *fallback) {
    return strdup(s ? s : fallback);
}

static char *cwd_name(void) {
    char cwd[4096];
    if (!getcwd(cwd, sizeof(cwd))) {
        return strdup("a.out");
    }
    return strdup(basename(cwd));
}

i32 section_load(const char *path, const char *section_name, Section *out) {
    dictionary *ini = (access(path, F_OK) == 0) ? iniparser_load(path) : NULL;

    char *project = NULL;
    if (ini && iniparser_getnsec(ini) > 0) {
        const char *sec0 = iniparser_getsecname(ini, 0);
        if (sec0) {
            project = strdup(sec0);
        }
    }
    if (!project) {
        project = cwd_name();
    }

    const char *name = (section_name && section_name[0] != '\0') ? section_name : "profile";

    bool have_section = ini && iniparser_find_entry(ini, name);

    char default_output[512];
    snprintf(default_output, sizeof(default_output), "./%s", project);

    if (have_section) {
        char key[300];

        snprintf(key, sizeof(key), "%s:command", name);
        const char *cmd = iniparser_getstring(ini, key, NULL);
        out->command = cmd ? strdup(cmd) : NULL;

        char compiler_key[300], flags_key[300], include_key[300], source_key[300], output_key[300];
        snprintf(compiler_key, sizeof(compiler_key), "%s:compiler", name);
        snprintf(flags_key, sizeof(flags_key), "%s:flags", name);
        snprintf(include_key, sizeof(include_key), "%s:include", name);
        snprintf(source_key, sizeof(source_key), "%s:source", name);
        snprintf(output_key, sizeof(output_key), "%s:output", name);

        out->has_build = iniparser_find_entry(ini, compiler_key) || iniparser_find_entry(ini, flags_key) || iniparser_find_entry(ini, include_key) || iniparser_find_entry(ini, source_key) || iniparser_find_entry(ini, output_key);

        out->compiler = dup_or(iniparser_getstring(ini, compiler_key, "gcc"), "gcc");

        const char *flags = iniparser_getstring(ini, flags_key, NULL);
        out->flags = flags ? strdup(flags) : NULL;

        const char *inc = iniparser_getstring(ini, include_key, NULL);
        out->include = inc ? strdup(inc) : NULL;

        out->source = dup_or(iniparser_getstring(ini, source_key, "."), ".");
        out->output = dup_or(iniparser_getstring(ini, output_key, default_output), default_output);
    } else {
        out->command = NULL;
        out->has_build = true;
        out->compiler = strdup("gcc");
        out->flags = NULL;
        out->include = NULL;
        out->source = strdup(".");
        out->output = strdup(default_output);
    }

    out->project = project;
    out->name = strdup(name);

    if (ini) {
        iniparser_freedict(ini);
    }
    return 0;
}

static char *assemble_build_command(const Section *s) {
    isize cap = (isize)strlen(s->compiler) + (isize)strlen(s->source) + (isize)strlen(s->output) + 32;
    if (s->flags) {
        cap += (isize)strlen(s->flags) + 1;
    }
    if (s->include) {
        cap += (isize)strlen(s->include) + 4;
    }
    char *cmd = malloc((usize)cap);
    if (!cmd) {
        return NULL;
    }
    cmd[0] = '\0';

    strcat(cmd, s->compiler);

    if (s->flags) {
        strcat(cmd, " ");
        strcat(cmd, s->flags);
    }

    if (s->include) {
        strcat(cmd, " -l");
        strcat(cmd, s->include);
    }

    strcat(cmd, " ");
    strcat(cmd, s->source);

    usize slen = strlen(s->source);
    bool is_dir = slen == 0 || s->source[slen - 1] != 'c';
    if (is_dir) {
        if (slen > 0 && s->source[slen - 1] != '/') {
            strcat(cmd, "/");
        }
        strcat(cmd, "*.c");
    }

    strcat(cmd, " -o ");
    strcat(cmd, s->output);

    return cmd;
}

char *section_build_command(const Section *s) {
    char *build_cmd = s->has_build ? assemble_build_command(s) : NULL;

    if (s->command && build_cmd) {
        usize len = strlen(s->command) + strlen(" && ") + strlen(build_cmd) + 1;
        char *chained = malloc(len);
        if (!chained) {
            free(build_cmd);
            return NULL;
        }
        snprintf(chained, len, "%s && %s", s->command, build_cmd);
        free(build_cmd);
        return chained;
    }

    if (s->command) {
        return strdup(s->command);
    }
    if (build_cmd) {
        return build_cmd;
    }
    return strdup(":");
}

u0 section_free(Section *s) {
    free(s->project);
    free(s->name);
    free(s->command);
    free(s->compiler);
    free(s->flags);
    free(s->include);
    free(s->source);
    free(s->output);
}
