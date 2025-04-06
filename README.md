<div align="center"><img src="resources/logo.webp" alt="AboutMe Logo"></div>

**AboutMe** is a lightweight, open-source command-line tool that quickly and securely displays detailed information about your hardware and software.
With no installation required and a tiny footprint, it provides a comprehensive system overview in just a few seconds.

Currently, it supports the following system details:

- Battery
- Bluetooth
- CPU
- Environment variables
- Features
- GPU
- Hard disks
- Installed applications
- Memory (RAM)
- Network
- Operating system
- Processes
- Restore points
- Scheduled tasks
- Services
- Shared resources
- Startup applications
- System clock
- Trash
- USB devices
- Wi-Fi

### Features

- Licensed under the [BSD 3-Clause License](https://opensource.org/licenses/BSD-3-Clause)
- No telemetry or data collection
- No installation — just run the binary
- No external dependencies
- Tiny size (< 500 KB)
- Modular design for easy customization

### Download

Binaries for Windows are available [here](https://github.com/mazoti/aboutme/tree/main/releases).
Some antivirus programs like BitDefender may flag these files as false positives, but this program 
does not modify any file on any hard disks or sends information over a network.

### Usage

Just run:

```
aboutme
```
If you don't want to press a key to exit, pass any argument:

```
aboutme 1
```
Problems with emojis? Try it with [Windows Terminal](https://github.com/microsoft/terminal).

### Source code

#### Directory structure:
<pre>
aboutme/
├── dockerfiles           # Folder with the dockerfiles for Linux 
├── makefiles             # Folder with the makefile scripts for each compiler and operating system
├── releases              # Folder with binary releases
├── resources             # Folder with resources like logos and icons
├── src                   # Source code folder
│    ├── common           # Folder with interfaces and common source code for all operating systems
│    ├── core             # System core modules for each operating system
│    │   ├── freebsd      # FreeBSD implementation of the core modules
│    │   ├── linux        # Linux implementation of the core modules
│    │   └── windows      # Windows implementation of the core modules
│    └── i18n             # Folder with all translations
│        ├── english      # English translation with error codes for all operating systems
│        └── portuguese   # Portuguese translation with error codes for all operating systems
└── tests                 # Folder with build tests
</pre>

### Build from source:

#### Requirements:
- Visual Studio Community Edition (open the "x64 Native Tools Command Prompt").

Edit the file makefiles/Makefile.windows.vs to enable or disable modules and open the command prompt:

```
cd aboutme
nmake -f makefiles\Makefile.windows.vs
```

### Translations
All strings are in the folder src/i18n, the current version supports two languages:

- English
- Portuguese

### Donations
You can become a sponsor [here](https://github.com/sponsors/mazoti)

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

**Thanks for your time and have fun!**
