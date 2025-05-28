#!/bin/bash

# Exit on error
set -e

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Change to the project directory
cd "$SCRIPT_DIR"

# Check if .uproject file exists
if [ ! -f "MessagePack.uproject" ]; then
    echo "Error: MessagePack.uproject not found!"
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

# Use the correct script for generating project files
GENERATOR_SCRIPT="$UE_PATH/Engine/Build/BatchFiles/Mac/GenerateProjectFiles.sh"

if [ ! -f "$GENERATOR_SCRIPT" ]; then
    echo "Error: $GENERATOR_SCRIPT not found!"
    exit 1
fi

echo "Using Unreal Engine at: $UE_PATH"
echo "Using generator script: $GENERATOR_SCRIPT"

echo "Removing existing Xcode workspace..."
rm -rf "$SCRIPT_DIR/MessagePack.xcworkspace"

echo "Generating Xcode project files..."
"$GENERATOR_SCRIPT" -project="$SCRIPT_DIR/MessagePack.uproject" -game -engine

echo "Xcode project files generated successfully!"
echo "You can now open MessagePack.xcworkspace in Xcode" 