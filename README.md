![Oxygen running on the TI-84 Plus CE](https://i.imgur.com/yFg0fCF.gif)
# Oxygen

[![Latest release](https://img.shields.io/github/v/release/AlvajoyAsante/Oxygen?label=release)](https://github.com/AlvajoyAsante/Oxygen/releases)
[![Open issues](https://img.shields.io/github/issues/AlvajoyAsante/Oxygen)](https://github.com/AlvajoyAsante/Oxygen/issues)
[![License: GPL-3.0](https://img.shields.io/github/license/AlvajoyAsante/Oxygen)](LICENSE)



> *Breathe new life into your projects with Oxygen.*

Oxygen is a graphical UI and widget framework for C programs on the TI-84 Plus CE.
It provides reusable controls, cursor input, graphics helpers, windows, notifications,
and lightweight file and user systems for calculator applications built with the
[CE C Toolchain](https://github.com/CE-Programming/toolchain).

## Features

- **Widgets**: Build interfaces with buttons, labels, checkboxes, dropdowns, entries,
  menus, sliders, switches, scrollbars, sprites, and windows.
- **Cursor and mouse input**: Render a movable cursor, detect buttons, and define
  clickable regions.
- **Graphics**: Draw rounded shapes, work with sprites, convert colors, and create
  QR-code sprites.
- **GUI utilities**: Add a battery indicator, clock, text input, color picker, and
  on-screen keyboard.
- **Files and users**: Work with a virtual file system, save application data, and
  manage virtual users.
- **Notifications**: Display messages within and between Oxygen applications.

## Getting Started

### Prerequisites

Install the [CE C Toolchain](https://github.com/CE-Programming/toolchain) and verify
that you can build a standard toolchain project.

### Installation

1. Download the latest version from [GitHub Releases](https://github.com/AlvajoyAsante/Oxygen/releases),
   or clone this repository for the current development version.
2. Copy the [`oxygen`](oxygen) directory into your project's `src` directory.
3. Include the umbrella header in your source code:

   ```c
   #include "oxygen/oxygen.h"
   ```

4. Build your project with `make` as usual.

Your project should now contain the following structure:

```text
your-project/
├── Makefile
└── src/
    ├── main.c
    └── oxygen/
        ├── oxygen.h
        └── ...
```

## Documentation

See the [Oxygen documentation](https://alvajoy.slite.page/p/_fJsK_ZwIdXyCh/Oxygen)
for the framework reference, tutorials, examples, and troubleshooting guidance.

## Examples

The [`examples`](examples) directory contains ready-to-build projects demonstrating:

- Cursor detection and movement
- Clickable regions
- GUI utilities such as battery status, date and time, color selection, and text input
- Complete demos, including Flip Frog and Simon Says

![Cursor and interface example](https://i.imgur.com/sqW6fB6.gif)
![Window interface example](https://i.imgur.com/wXRc4y9.png)
![GUI example](https://i.imgur.com/P3u2YA0.png)
![Widget example](https://i.imgur.com/bLaxWpE.png)
![Application example](https://i.imgur.com/Mkf7S94.png)
![Color interface example](https://i.imgur.com/vF63krQ.png)

## Support and Development

- Join the [Oxygen Discord server](https://discord.gg/xyUZgnD4UJ)
- Visit the [Oxygen thread on Cemetech](https://www.cemetech.net/forum/viewtopic.php?t=15070)
- Follow development on the [Trello board](https://trello.com/b/eYALDr4Q/xenon-development-c)
- Report reproducible bugs through [GitHub Issues](https://github.com/AlvajoyAsante/Oxygen/issues)

## Credits

- **epsilonfive**: Optix widget system development
- **Captain-Calc**: Textioc development

## License

Oxygen is distributed under the [GNU General Public License v3.0](LICENSE).

Copyright (C) 2018-present Alvajoy Asante