#!/bin/bash
# sync.sh
# This script detects the current shell and executes the appropriate commands
# to pull or push changes to/from both GitHub and GitLab repositories.
# It ensures operations are performed only on the main branch and handles
# repository synchronization issues.

# Color variables for readability
RED="\033[1;31m"
GREEN="\033[1;32m"
BLUE="\033[1;34m"
RESET="\033[0m"  # Reset to default color

# Detect current shell
current_shell="$([[ -n $ZSH_VERSION ]] && echo "zsh" || echo "bash")"

# Function to handle errors
handle_error() {
    echo -e "\n${RED}Error: $1${RESET}\n"
    exit 1
}

# Function for success messages (in green)
handle_success() {
    echo -e "\n${GREEN}$1${RESET}\n"
}

# Configure shell-specific settings
if [ "$current_shell" = "zsh" ]; then
    setopt err_exit
    setopt pipe_fail
    setopt unset
else
    set -euo pipefail
fi

# Store current branch name
current_branch=$(git rev-parse --abbrev-ref HEAD || handle_error "Failed to get current branch")

# Check if we're on the main branch
if [ "$current_branch" != "main" ]; then
    handle_error "Not on main branch. Please checkout the main branch first:
    git checkout main"
fi

# Check if action argument is provided
if [ $# -lt 1 ]; then
    handle_error "Usage: $0 <pull|push>"
fi

action="$1"

if [ "$action" = "pull" ]; then
    # Perform pull operation
    echo -e "\n${BLUE}Performing normal pull...${RESET}"
    git pull origin main || handle_error "Failed to pull from GitLab"
    handle_success "Successfully completed pull synchronization"

elif [ "$action" = "push" ]; then
    # Perform push operation
    echo -e "\n${BLUE}Checking if repositories are synchronized...${RESET}"
    git fetch origin main || handle_error "Failed to fetch updates from GitLab"
    git fetch github main || handle_error "Failed to fetch updates from GitHub"

    if ! git diff --quiet main origin/main || ! git diff --quiet main github/main; then
        handle_error "Local repository is not synchronized with GitLab or GitHub. Please pull updates before pushing."
    fi

    echo -e "\n${BLUE}Adding changes...${RESET}"
    git add . || handle_error "Failed to add changes"

    if git diff --cached --quiet; then
        handle_error "No changes to commit. Working directory is clean."
    fi

    echo -e "\n${BLUE}Enter commit message:${RESET}"
    read -r commit_message

    if [ -z "$commit_message" ]; then
        handle_error "Commit message cannot be empty"
    fi

    echo -e "\n${BLUE}Committing changes...${RESET}"
    git commit -m "$commit_message" || handle_error "Failed to commit changes"

    echo -e "\n${BLUE}Pushing to GitLab...${RESET}"
    git push origin main || handle_error "Failed to push to GitLab"

    echo -e "\n${BLUE}Pushing to GitHub...${RESET}"
    git push github main || handle_error "Failed to push to GitHub"

    handle_success "Successfully pushed to both repositories"

else
    handle_error "Invalid action: $action. Use 'pull' or 'push'."
fi
