#!/bin/bash

# Exit on error
set -e

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Change to the project directory
cd "$SCRIPT_DIR"

# Clean up Intermediate and Binaries folders
echo "Cleaning up Intermediate and Binaries folders..."
rm -rf "$SCRIPT_DIR/Intermediate"
rm -rf "$SCRIPT_DIR/Binaries"
rm -rf "$SCRIPT_DIR/Plugins/MessagePackPlugin/Intermediate"
rm -rf "$SCRIPT_DIR/Plugins/MessagePackPlugin/Binaries"

# Check if .uproject file exists
UPROJECT_FILE="MessagePack.uproject"
if [ ! -f "$UPROJECT_FILE" ]; then
    echo "Error: $UPROJECT_FILE not found in $SCRIPT_DIR!"
    exit 1
fi

# Get the Unreal Engine installation path
# You may need to modify this path based on your UE installation
UE_PATH="/Users/Shared/Epic Games/UE_5.5"

# Check if UE path exists
if [ ! -d "$UE_PATH" ]; then
    echo "Error: Unreal Engine not found at $UE_PATH"
    echo "Please modify the UE_PATH variable in this script to point to your Unreal Engine installation"
    exit 1
fi

# Check if Engine directory exists
if [ ! -d "$UE_PATH/Engine" ]; then
    echo "Error: Engine directory not found at $UE_PATH/Engine"
    exit 1
fi

# Path to the UE build script
UE_BUILD_SCRIPT="$UE_PATH/Engine/Build/BatchFiles/Mac/Build.sh"

if [ ! -f "$UE_BUILD_SCRIPT" ]; then
    echo "Error: UE Build Script not found at $UE_BUILD_SCRIPT!"
    exit 1
fi

echo "Using Unreal Engine at: $UE_PATH"

# Regenerate project files before building
PROJECT_FILES_GEN_SCRIPT="$UE_PATH/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh"
if [ ! -f "$PROJECT_FILES_GEN_SCRIPT" ]; then
    echo "Error: Project files generation script not found at $PROJECT_FILES_GEN_SCRIPT!"
    exit 1
fi
echo "Regenerating project files for: $UPROJECT_FILE"
"$PROJECT_FILES_GEN_SCRIPT" -project="$SCRIPT_DIR/$UPROJECT_FILE" -game

echo "Building project: $UPROJECT_FILE"

# Build the project (e.g., Game target, Mac platform, Development configuration)
# You can change "MessagePack" to "MessagePackEditor" to build the editor target
# You can change "Development" to "Shipping" or "DebugGame" for other configurations
TARGET_NAME="MessagePack"
PLATFORM="Mac"
CONFIGURATION="Development"

echo "Attempting to build target: $TARGET_NAME, Platform: $PLATFORM, Configuration: $CONFIGURATION"

"$UE_BUILD_SCRIPT" "$TARGET_NAME" "$PLATFORM" "$CONFIGURATION" -project="$SCRIPT_DIR/$UPROJECT_FILE" -waitmutex

echo "Build process finished."
echo "If successful, binaries can be found in the Binaries/$PLATFORM directory." 