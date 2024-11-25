#!/bin/bash
# sync_pull.sh
# This script detects the current shell and executes the appropriate commands
# to pull changes from both GitHub and GitLab repositories.
# It ensures operations are performed only on the main branch and handles
# repository synchronization issues.

# Detect current shell
current_shell="$([[ -n $ZSH_VERSION ]] && echo "zsh" || echo "bash")"

# Function to handle errors
handle_error() {
    echo "Error: $1"
    exit 1
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

# Check if we're on main branch
if [ "$current_branch" != "main" ]; then
    handle_error "Not on main branch. Please checkout main branch first:
    git checkout main"
fi

# Normal pull operation when everything is in sync
echo "Performing normal pull..."
git pull origin main || handle_error "Failed to pull from GitLab"

echo "Successfully completed synchronization"
