#!/bin/bash
# sync_push.sh
# This script detects the current shell and executes the appropriate commands
# to push changes to both GitHub and GitLab repositories.
# It ensures operations are performed only on the main branch.

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

# Check if local repository is synchronized with GitLab and GitHub
echo "Checking if repositories are synchronized..."
git fetch origin main || handle_error "Failed to fetch updates from GitLab"
git fetch github main || handle_error "Failed to fetch updates from GitHub"

# Compare local and remote branches
if ! git diff --quiet main origin/main || ! git diff --quiet main github/main; then
    handle_error "Local repository is not synchronized with GitLab or GitHub. Please pull updates before pushing."
fi

# Add all changes
echo "Adding changes..."
git add . || handle_error "Failed to add changes"

# Check if there are changes to commit
if git diff --cached --quiet; then
    handle_error "No changes to commit. Working directory is clean."
fi

# Prompt for commit message
echo "Enter commit message:"
read -r commit_message

# Validate commit message is not empty
if [ -z "$commit_message" ]; then
    handle_error "Commit message cannot be empty"
fi

# Commit changes
echo "Committing changes..."
git commit -m "$commit_message" || handle_error "Failed to commit changes"

# Push to GitLab (origin)
echo "Pushing to GitLab..."
git push origin main || handle_error "Failed to push to GitLab"

# Push to GitHub (github)
echo "Pushing to GitHub..."
git push github main || handle_error "Failed to push to GitHub"

echo "Successfully pushed to both repositories"
