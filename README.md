<div align="center"><img src="resources/logo.webp"></div>

This program displays detailed information about the user's hardware and software in a very fast, easy, and safe way. It's open-source and works in command line. The current release displays:

- CPU
- GPU
- Memory (RAM)
- Hard disks
- USB devices
- System clock
- Battery
- WiFi
- Bluetooth
- Operating system
- Processes
- Services
- Startup applications

### Download

Binaries for Windows are available [here](https://github.com/mazoti/aboutme/tree/main/releases). Some antivirus programs like BitDefender may flag these files as false positives, but this program does not modify any file on any hard disks or sends information over a network (no telemetry).

### Usage

Just run: if you don't want to press a key to exit, pass any argument:

```
aboutme 1
```

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