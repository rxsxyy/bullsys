# bullsys
minimalist build system using ini file

## build dependencies
- [iniParser](https://gitlab.com/iniparser/iniparser) (system-installed, linked via `-liniparser`; not vendored)

## building
`$ gcc -Wall -Wextra -std=c99 -pedantic -O2 -D_POSIX_C_SOURCE=200809L -liniparser src/*.c -o bullsys`

bullsys can also build itself

`$ bullsys`

## usage

```
bullsys <options> <action>
```

---
rxsxyy, 2026
