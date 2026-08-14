# Atlinx QMK Userspace ⌨️

Repository of my QMK keyboard layouts. My custom layouts are stored under `users/atlinx`, and are based on categories of keyboards (Planck, Preonic, etc.). `keyboards` contains the actual keymaps, which serves a glue to get the custom layouts working on a specific keyboard, disabling features that aren't supported on the board (ex. audio, rgb, etc.).

## Layouts

- [planck](./users/atlinx/planck/README.md)
    - [base](./users/atlinx/planck/base/README.md)
    - [glow](./users/atlinx/planck/glow/README.md)

## Developing

Development is preferably done on Linux or WSL.

1. Install VSCode
2. Install clang
3. Install the recommended VSCode extensions
