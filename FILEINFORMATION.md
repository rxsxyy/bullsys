# .bull format

a `.bull` file is simply an ini file.

- the **first section header** in the file names the project (e.g.
  `[bullsys]`). This is purely a label, used as the default output name
  and as a fallback project name; it takes no keys of its own.
- every other section is a **task** bullsys can run by name
  (`bullsys <name>`).
- a section named `profile` (or `profile:<name>` for named profiles, run
  as `bullsys profile:<name>`) may additionally describe how to compile
  the project.

recognized keys:

| key        | applies to      | meaning                                       |
|------------|-----------------|-----------------------------------------------|
| `command`  | any section     | shell command to run                          |
| `compiler` | profile section | compiler to invoke (default: `gcc`)           |
| `flags`    | profile section | raw compiler flags                            |
| `include`  | profile section | library name, linked as `-l<include>`         |
| `source`   | profile section | source file or directory (default: `.`)       |
| `output`   | profile section | output binary path (default: `./<project>`)   |

if a section sets any of `compiler`, `flags`, `include`, `source`, or
`output`, bullsys assembles a compile command from them. if it also sets
`command`, that command runs first, followed by `&&` and the compile
command.

if no `.bull` file is found, or the requested section isn't in it,
bullsys falls back to built-in defaults: compiler `gcc`, no flags or
include, source `.`, output `./<project>`, where `<project>` is taken
from the first section header, or the current directory's name if there
is no `.bull` file at all.
