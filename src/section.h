
#ifndef SECTION_H
#define SECTION_H

#include "global.h"

typedef struct {
    char *project;
    char *name;
    char *command;
    bool has_build;
    char *compiler;
    char *flags;
    char *include;
    char *source;
    char *output;
} Section;

i32 section_load(const char *path, const char *section_name, Section *out);

char *section_build_command(const Section *s);

u0 section_free(Section *s);

#endif
