#!/bin/bash

# ---------------------------------------------------------
# This script automates the configuration and build of QEMU.
# It supports three build modes:
# 1. Normal QEMU build.
# 2. Verbose build with detailed output (-DVERBOSE).
# 3. Build specifically for ARM using --target-list=arm-softmmu.
#    This mode requires passing "arm" as an argument to the script.
#
# ---------------------------------------------------------

# Exit immediately if a command exits with a non-zero status
set -e

# Define paths (adjust if needed)
QEMU_DIR="../qemu"
QEMU_BUILD_DIR="$QEMU_DIR/build"

# Function to configure QEMU with the option to use arm-softmmu
configure_qemu() {
    echo "Configuring QEMU..."
    if [[ "$1" == "arm" ]]; then
        # Configure specifically for ARM target (arm-softmmu)
        echo "Using --target-list=arm-softmmu for ARM target"
        cd "$QEMU_DIR"
        ./configure --target-list=arm-softmmu
    else
        # Default configuration
        cd "$QEMU_DIR"
        ./configure
    fi
    echo "QEMU configuration completed."
}

# Function to build QEMU using Ninja with the -j option for parallel jobs
build_qemu() {
    echo "Building QEMU using Ninja..."
    ninja -C "$QEMU_BUILD_DIR" -j $(nproc) qemu-system-arm
    echo "QEMU build completed."
}

# Function to build QEMU with verbose output
build_qemu_verbose() {
    echo "Building QEMU with verbose output..."
    cd "$QEMU_DIR"
    ./configure --extra-cflags="-DVERBOSE"
    ninja -C "$QEMU_BUILD_DIR" -j $(nproc) qemu-system-arm
    echo "QEMU verbose build completed."
}

# Main function to handle the different build types
main() {
    if [[ "$1" == "verbose" ]]; then
        echo "Running verbose build..."
        build_qemu_verbose
    elif [[ "$1" == "arm" ]]; then
        echo "Running build with --target-list=arm-softmmu..."
        configure_qemu "arm"
        build_qemu
    else
        echo "Running normal build..."
        configure_qemu
        build_qemu
    fi
}

# Entry point
main "$1"

