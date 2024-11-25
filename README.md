# Project name (group2)

![polito](resources/logo_polito.jpg)

<!-- ## Table of Contents -->

<!-- TODO: make prettier -->

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
<i>Last updated: November 2024</i>
</div>

</details>

## Project Overview



## Features



## Getting Started



### Prerequisites



### Installation



### Running the Emulator



## Project structure



## Board and FreeRTOS Integration



### Board Specifications



### FreeRTOS Application



## Usage



### How to Run Applications



### Available Configuration Options



## Team Collaboration

To ensure smooth collaboration and efficient Git workflow within the team, a dedicated [Git Guide](resources/GUIDE-GIT.md) has been created. This guide provides:

- Repository Setup: Step-by-step instructions to configure remote repositories.
- Synchronization: Scripts for pulling and pushing changes seamlessly.
- Usage Instructions: Best practices for avoiding conflicts and ensuring updates are integrated properly.
- Error Handling: Guidance on resolving conflicts and other synchronization issues.
- Submodules: Recommendations for managing dependencies like QEMU and FreeRTOS.

### Scripts for Git Operations

The project includes custom synchronization scripts (`sync_pull.sh` and `sync_push.sh`) to streamline the process:

- `sync_pull.sh`: Pulls the latest updates from the repository.
- `sync_push.sh`: Pushes your changes to both the primary and backup repositories.

Make sure the scripts are executable:

```bash
chmod +x sync_pull.sh
chmod +x sync_push.sh
```

For detailed instructions, refer to the [GUIDE-GIT.md](resources/GUIDE-GIT.md).

## Contributing

<!-- TODO: create a dedicated file ? -->

We welcome contributions to this project! Whether you are a member of the organization or an external contributor, follow the guidelines below to ensure a smooth collaboration process.

#### **For Organization Members**
If you're a member of the project team, follow these steps:

1. **Branch Naming**: 

   - Use descriptive branch names for your work (e.g., `feature/new-qemu-driver` or `fix/freertos-config`).
   
2. **Commit Messages**:

   - Follow a clear and concise commit message format:
   
     ```
     [Type] Short description (e.g., [Fix] Resolve timing bug in QEMU)
     ```
     
   - Types: `[Add]`, `[Fix]`, `[Update]`, `[Refactor]`, etc.
   
3. **Git Scripts**:

   - Use the provided [Git scripts](#team-collaboration) (`sync_pull.sh` and `sync_push.sh`) to ensure seamless synchronization with the team repository.
   
4. **Testing**:

   - Thoroughly test your changes locally using the provided test cases before pushing.
   
5. **Review Process**:

   - Open a merge request (MR) for your branch and tag a team member for review.
   - Include detailed information about the changes and any testing performed.

#### **For External Contributors**

We appreciate external contributions! Please follow these guidelines to contribute:

1. **Fork the Repository**:

   - Start by forking this repository and cloning it to your local machine.
   
   ```bash
   git clone <your-fork-link>
   ```

2. **Create a New Branch**:

   - Create a branch for your changes:
   
   ```bash
   git checkout -b feature/<short-description>
   ```

3. **Follow Coding Standards**:

   - Adhere to the project's coding standards. Check any existing guidelines in the repository.
   
4. **Testing**:

   - Test your changes thoroughly using the provided test cases or scripts.

5. **Submit a Pull Request (PR)**:

   - Push your changes to your fork and open a pull request to the main repository.
   - Clearly describe:
     - What changes you made.
     - Why they are necessary.
     - Any issues resolved or enhancements added.

6. **Engage in the Review**:

   - Be prepared to discuss and make updates based on feedback from the maintainers.
   
#### **Code of Conduct**

All contributors are expected to adhere to the project's [Code of Conduct](link-to-code-of-conduct-file). Be respectful and constructive when collaborating.

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

<!-- TODO: write -->
