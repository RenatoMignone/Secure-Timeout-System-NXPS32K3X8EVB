
# GUIDE-GIT.md

## Overview
This guide provides essential instructions for setting up the Git repositories, configuring access, synchronizing changes, and collaborating with other members. Follow each section carefully to ensure consistency across the project and avoid conflicts.

## Sections
- [Repository Setup](#repository-setup)
- [Synchronization](#synchronization)
- [Usage Instructions](#usage-instructions)
- [Error Handling](#error-handling)
- [Working with Submodules](#working-with-submodules)

---

## Repository Setup

### Step 1: Clone the Repository
To start working on the repository, clone it from the **GitLab** source repository to your local machine.

```bash
git clone <gitlab_repository_link>
```

### Step 2: Set Up Remote Repository
After cloning from GitLab, add the remote GitHub repository link if it is not automatically added. This step allows you to push changes to GitHub, which serves as a backup repository, ensuring that your work is saved in multiple locations.

```bash
git remote add github <github_repository_link>
```

### Step 3: Verify the Remote Link
Ensure that the remote repository link has been set up correctly. Use the following command to check:

```bash
git remote -v
```

You should see output similar to this:

```
origin  <gitlab_repository_link> (fetch)
origin  <gitlab_repository_link> (push)
github  <github_repository_link> (fetch)
github  <github_repository_link> (push)
```

## Synchronization

### Making Scripts Executable
To use the provided synchronization scripts, which handle both pull and push operations, set the necessary permissions. These scripts are compatible with both Bash and Zsh shells.

```bash
chmod +x sync_pull.sh
chmod +x sync_push.sh
```

**Note:** Although these scripts don’t automatically handle synchronization errors, they will notify you if any errors occur. It is then the responsibility of team members to manually resolve conflicts as needed.

## Usage Instructions

1. **Avoid Editing the Same Files Concurrently:** Coordinate with other members to prevent editing the same file simultaneously. This is to avoid merge conflicts, which may require manual resolution. If each member is working on different files, pulling changes will automatically integrate them.
   
2. **Pull Updates Before Working:** Always run the `sync_pull.sh` script at the start of a work session to pull the latest updates from the repository.

    ```bash
    ./sync_pull.sh
    ```
    
3. **Pull After Modifications:** Once you've completed your modifications, run `sync_pull.sh` again to check if any other commits were added by teammates during your work session. This helps avoid conflicts during the push.

4. **Push Changes Using Sync Script:** When you’re ready to push your changes, use the `sync_push.sh` script. This will automatically push your changes to the primary GitHub repository and any configured backup repositories.

    ```bash
    ./sync_push.sh
    ```

**Note:** These scripts ensure that all changes are synchronized, reducing manual overhead.

## Error Handling

- **Sync Script Errors:** In case of synchronization errors, the scripts will output an error message. Currently, they do not perform error resolution automatically. You must manually resolve conflicts or issues before rerunning the scripts.
  
- **Conflict Resolution:** If a merge conflict occurs, use Git commands to resolve conflicts. Once resolved, re-run `sync_pull.sh` and then proceed with `sync_push.sh` to update the main repository.

## Working with Submodules [TO BE DECIDED]

If you don’t need to modify the internal files of certain dependencies (e.g., QEMU and FreeRTOS), it’s best to either:

1. Include them as **submodules** within this repository, or
2. Add their directories to `.gitignore` and let each member manage the cloning of these repositories independently.

However, if you need to directly modify files within these repositories, include them as fully versioned local repositories and manage them as regular files within this repository.

### Adding Submodules
To add a repository as a submodule, use the following command:

```bash
git submodule add <repository_link> <path>
```
