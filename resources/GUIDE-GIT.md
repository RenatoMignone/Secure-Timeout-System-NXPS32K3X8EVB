
# GUIDE-GIT

## Overview

This guide provides clear instructions for setting up Git repositories, configuring remote access, synchronizing changes, and collaborating effectively. Follow each section carefully to ensure a smooth workflow and minimize conflicts.

## Sections
- [Repository Setup](#repository-setup)
- [Synchronization](#synchronization)
- [Usage Instructions](#usage-instructions)
- [Error Handling](#error-handling)
- [Working with Submodules](#working-with-submodules)

---

## Repository Setup

### Step 1: Clone the Repository

Clone the primary **GitLab** repository to your local machine to start working on the project.

```bash
git clone https://baltig.polito.it/caos2024/group2.git
```

### Step 2: Set Up Remote Repository

To enable backups and multi-location redundancy, add the **GitHub** repository as a remote:

```bash
git remote add github https://github.com/neo-CAOS/group2.git
```

### Step 3: Verify the Remote Link

Verify that both **GitLab** and **GitHub** repositories are configured correctly:

```bash
git remote -v
```

You should see output similar to this:

```
github	https://github.com/neo-CAOS/group2.git (fetch)
github	https://github.com/neo-CAOS/group2.git (push)
origin	https://baltig.polito.it/caos2024/group2.git (fetch)
origin	https://baltig.polito.it/caos2024/group2.git (push)
```

## Synchronization

### Synchronization Script

The project includes a single script, sync.sh, to handle both pull and push operations. This ensures consistency and minimizes errors.

#### Script setup

Before using the script, make it executable:

```bash
chmod +x sync.sh
```

#### Script Usage

- **Pull Updates:**

  Run the script with `pull` to fetch and integrate the latest changes:
  
  ```bash
  ./sync.sh pull
  ```

- **Push Changes:**

  Run the script with push to synchronize your local changes with both **GitLab** and **GitHub**:

  ```bash
  ./sync.sh push
  ```

> **Note**: The script checks for branch consistency, ensures local and remote synchronization, and prompts for a commit message during pushes.

## Usage Instructions

1. **Start by Pulling Updates:** Always pull the latest changes before making modifications:

   ```bash
   ./sync.sh pull
   ```

2. **Work on Modifications:** Make your changes locally, ensuring they are well-documented and isolated.

3. **Pull Again After Changes:** Before pushing, pull again to incorporate any updates made by collaborators:

   ```bash
   ./sync.sh pull
   ```

4. **Push Changes:** Once your modifications are complete and conflicts (if any) are resolved, push the changes:

   ```bash
   ./sync.sh push
   ```

5. **Coordinate with Team Members:** Avoid working on the same files simultaneously to minimize conflicts.

## Error Handling

#### Common Errors

<!-- TODO: REVIEW -->

- **Branch Mismatch:** If you are not on the `main` branch, the script will prompt you to switch:

  ```bash
  git checkout main
  ```

- **Synchronization Issues:** If local and remote branches are not synchronized, the script will notify you to pull updates before pushing.

#### Conflict Resolution

<!-- TODO: REVIEW -->

In case of conflicts:

1. Manually resolve conflicts in affected files.

2. Stage the resolved changes:
   
   ```bash
   git add <file>
   ```

3. Complete the merge process:

   ```bash
   git commit
   ```

4. Run the synchronization script again:

   ```bash
   ./sync.sh push
   ```

## Working with Submodules [TO BE DECIDED]

### Submodule Usage

For dependencies (e.g., QEMU or FreeRTOS), you can use submodules for better modularity.

#### Adding Submodules

To add a submodule:

```bash
git submodule add <repository_link> <path>
```

#### Updating Submodules

To initialize and update submodules in a cloned repository:

```bash
git submodule update --init --recursive
```

### Recommendations

- If you don’t need to modify the dependency files, use submodules to keep the repository lightweight.
- If modifications are required, include the dependency as part of the main repository and manage it directly.

For additional details, consult the official Git documentation on submodules.
