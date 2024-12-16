# Secure-Timeout-System-NXPS32

![polito](resources/logo_polito.jpg)

<!-- ## Table of Contents -->

<!-- TODO: make prettier -->

<!-- TODO: add the documentation section (with all the references) -->

<details closed>
<summary><b>Table of Contents</b></summary>
 
&nbsp;• [Project Overview](#project-overview) <br>
&nbsp;• [Features](#features) <br>
&nbsp;• [Getting Started](#getting-started) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Prerequisites](#prerequisites) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Installation](#installation) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Running the Emulator](#running-the-emulator) <br>
&nbsp;• [Project Structure](#project-structure) <br>
&nbsp;• [Board and FreeRTOS Integration](#board-and-freertos-integration) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Board Specifications](#board-specifications) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [FreeRTOS Application](#freertos-application) <br>
&nbsp;• [Usage](#usage) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [How to Run Applications](#how-to-run-applications) <br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;• [Available Configuration Options](#available-configuration-options) <br>
&nbsp;• [Team Collaboration](#team-collaboration) <br>
&nbsp;• [Contributing](#contributing) <br>
&nbsp;• [Authors](#authors) <br>
&nbsp;• [License](#license) <br>
&nbsp;• [Acknowledgments](#acknowledgments) <br>

<div align="right">
<i>Last updated: January 2025</i>
</div>

</details>

## Project Overview

This project implements a very simple secure timeout system application on the NXP S32K3X8EVB board using FreeRTOS, emulated with QEMU.

This project has been assigned for the [Computer Architectures and Operating Systems](#acknowledgments) course. The work has been carried out by group 2. For more information about the authors, please refer to the [Authors](#authors) section.

## Features

- Integration with FreeRTOS
- UART communication
- Hardware timer initialization and handling
- Secure timeout system with multiple tasks
- QEMU emulation support

## Getting Started

### Prerequisites

- ARM GCC Toolchain (`arm-none-eabi-gcc`)
- QEMU for ARM
- FreeRTOS source code (added as a submodule)
- Make

### Installation

1. Clone the repository:
    ```sh
    git clone --recurse-submodules https://baltig.polito.it/caos2024/group2.git 
    cd group2
    ```

2. Configure QEMU:
   To configure QEMU, navigate to the `qemu` directory and run the configuration script:
   ```bash
   cd qemu
   ./configure
   ninja -C build qemu-system-arm
   ```

## Project structure

- `App/`: Contains the main project files and source code.
    - `CMSIS/`: CMSIS headers and startup files.
    - `Peripherals/`: Contains peripheral driver files.
        - `IntTimer.c/.h`: Timer interrupt handling.
        - `uart.c/.h`: UART communication functions.
    - `SecureTimeoutSystem/`: Contains the secure timeout system implementation.
        - `globals.h`: Global variables for the secure timeout system.
        - `secure_timeout_systems.c/.h`: Secure timeout system functions.
    - `FreeRTOSConfig.h`: FreeRTOS configuration file.
    - `main.c`: Main application entry point.
    - `Makefile`: Build configuration and rules.
    - `s32_linker.ld`: Linker script for the project.
    - `s32_startup.c`: Startup code for the S32K3X8EVB board.
- `FreeRTOS`: FreeRTOS kernel and related files.
- `qemu`: QEMU emulator files.
- `resources/`: Additional resources such as images and documentation.
- `README.md`: Project documentation.
- `LICENSE-CC-BY-NC-4.0.md`: License file.

## Board and FreeRTOS Integration

### Board Specifications

- **Flash Memory**: 12MB
- **SRAM**: 2.25 MB
- **UART Base Address**: 0x4006A000
- **PIT Timer Base Address**: 0x40037000

### FreeRTOS Application

The FreeRTOS application includes tasks for monitoring user activity, handling alerts, and simulating events. The tasks are created and managed by FreeRTOS, and the system uses hardware timers for periodic operations.

### Running the App

1. Build the project:
    ```sh
    cd App
    make clean all
    ```

2. To run the emulator with the built project:
    ```sh
    make qemu_start
    ```

3. To run the emulator in debug mode:
    ```sh
    make qemu_debug
    ```

### Available Configuration Options

- `mainTASK_PRIORITY`: Priority for the main tasks.
- `MONITOR_TASK_PRIORITY`: Priority for the monitor task.
- `ALERT_TASK_PRIORITY`: Priority for the alert task.
- `EVENT_TASK_PRIORITY`: Priority for the event task.

## Team Collaboration

To ensure smooth collaboration and efficient Git workflow within the team, a dedicated [Git Guide](resources/GUIDE-GIT.md) has been created. This guide provides:

- Repository Setup: Step-by-step instructions to configure remote repositories.
- Synchronization: Scripts for pulling and pushing changes seamlessly.
- Usage Instructions: Best practices for avoiding conflicts and ensuring updates are integrated properly.
- Error Handling: Guidance on resolving conflicts and other synchronization issues.
- Submodules: Recommendations for managing dependencies like QEMU and FreeRTOS.

### Scripts for Git Operations

The project includes a synchronization script, `sync.sh`, to streamline repository management. This script accepts a single argument to specify the action:

- pull: Fetches and integrates the latest updates from the repository.
- push: Pushes your changes to both the primary and backup repositories, ensuring synchronization.

#### Usage 

Run the script with the desired action:

```bash
./sync.sh pull  # To pull updates  
./sync.sh push  # To push changes
```

Before using the script, ensure it is executable:

```bash
chmod +x sync.sh
```

For detailed instructions, refer to the [GUIDE-GIT.md](resources/GUIDE-GIT.md).

## Contributing

We welcome contributions to this project! Whether you are a member of the organization or an external contributor, follow the guidelines below to ensure a smooth collaboration process.

#### For Organization Members

If you're a member of the organization, you can commit directly to the repository for simple development tasks. For more complex contributions or when in doubt, please refer to the [CONTRIBUTING.md](resources/CONTRIBUTING.md) file for detailed guidelines.

#### For External Contributors

If you're an external contributor, please refer directly to the [CONTRIBUTING.md](resources/CONTRIBUTING.md) file for detailed instructions on how to get involved.

## Authors

| Name              | GitHub                                                                                                               | LinkedIn                                                                                                                                  | Email                                                                                                            |
| ----------------- | -------------------------------------------------------------------------------------------------------------------- | ----------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- |
| Andrea Botticella | [![GitHub](https://img.shields.io/badge/GitHub-Profile-informational?logo=github)](https://github.com/Botti01)       | [![LinkedIn](https://img.shields.io/badge/LinkedIn-Profile-blue?logo=linkedin)](https://www.linkedin.com/in/andrea-botticella-353169293/) | [![Email](https://img.shields.io/badge/Email-Send-blue?logo=gmail)](mailto:andrea.botticella@studenti.polito.it) |
| Fabrizio Emanuel  | [![GitHub](https://img.shields.io/badge/GitHub-Profile-informational?logo=github)](https://github.com/briss01)       | [![LinkedIn](https://img.shields.io/badge/LinkedIn-Profile-blue?logo=linkedin)](https://www.linkedin.com/in/fabrizio-emanuel-b57a28237/)  | [![Email](https://img.shields.io/badge/Email-Send-blue?logo=gmail)](mailto:fabrizio.emanuel@studenti.polito.it)  |
| Elia Innocenti    | [![GitHub](https://img.shields.io/badge/GitHub-Profile-informational?logo=github)](https://github.com/eliainnocenti) | [![LinkedIn](https://img.shields.io/badge/LinkedIn-Profile-blue?logo=linkedin)](https://www.linkedin.com/in/eliainnocenti/)               | [![Email](https://img.shields.io/badge/Email-Send-blue?logo=gmail)](mailto:elia.innocenti@studenti.polito.it)    |
| Renato Mignone    | [![GitHub](https://img.shields.io/badge/GitHub-Profile-informational?logo=github)](https://github.com/RenatoMignone) | [![LinkedIn](https://img.shields.io/badge/LinkedIn-Profile-blue?logo=linkedin)](https://www.linkedin.com/in/renato-mignone/)              | [![Email](https://img.shields.io/badge/Email-Send-blue?logo=gmail)](mailto:renato.mignone@studenti.polito.it)    |
| Simone Romano     | [![GitHub](https://img.shields.io/badge/GitHub-Profile-informational?logo=github)](https://github.com/sroman0)       | [![LinkedIn](https://img.shields.io/badge/LinkedIn-Profile-blue?logo=linkedin)](https://www.linkedin.com/in/simone-romano-383277307/)     | [![Email](https://img.shields.io/badge/Email-Send-blue?logo=gmail)](mailto:simone.romano@studenti.polito.it)     |

## License

This project is licensed under the [Creative Commons Attribution-NonCommercial 4.0 International (CC BY-NC 4.0)](https://creativecommons.org/licenses/by-nc/4.0/) license.

#### What This Means:

- **Attribution**: You are free to share (copy, redistribute) and adapt (remix, transform, build upon) the material as long as proper credit is given to the original author(s). 
- **Non-Commercial**: You may not use the material for commercial purposes.

#### Marking Your Work:

All documents and files within this repository are explicitly marked with the CC BY-NC 4.0 license, as outlined in the [Creative Commons marking guidelines](https://wiki.creativecommons.org/wiki/Marking_your_work_with_a_CC_license#Example:_Presentation).

For more details and templates, refer to:
- [GitHub License Templates](https://github.com/Gibberlings3/GitHub-Templates/tree/master/License-Templates)

By using this project, you agree to the terms of the [CC BY-NC 4.0 license](https://creativecommons.org/licenses/by-nc/4.0/).

## Acknowledgments

- **Stefano Di Carlo** ([stefano.dicarlo@polito.it](mailto:stefano.dicarlo@polito.it)): the professor who assigned us the project and is teaching the course **CAOS**.

<div align="right">
<a href="#top">Back to top</a>
</div>
