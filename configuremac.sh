#!/bin/bash

# Auto-detect macOS architecture and configure VS Code
# Run this script to automatically set up your environment

echo "🍎 Detecting macOS Architecture..."

# Detect architecture
ARCH=$(uname -m)

if [ "$ARCH" = "arm64" ]; then
    echo "✅ Detected Apple Silicon (M1/M2/M3)"
    HOMEBREW_PREFIX="/opt/homebrew"
    CONFIG_NAME="Mac-ARM64"
    INTELLISENSE_MODE="macos-gcc-arm64"
elif [ "$ARCH" = "x86_64" ]; then
    echo "✅ Detected Intel Mac"
    HOMEBREW_PREFIX="/usr/local"
    CONFIG_NAME="Mac-Intel"
    INTELLISENSE_MODE="macos-gcc-x64"
else
    echo "❌ Unknown architecture: $ARCH"
    exit 1
fi

# Find GCC version
echo ""
echo "🔍 Finding GCC installation..."
GCC_VERSION=$(ls $HOMEBREW_PREFIX/bin/g++-* 2>/dev/null | grep -oE '[0-9]+$' | head -1)

if [ -z "$GCC_VERSION" ]; then
    echo "❌ GCC not found! Please install it:"
    echo "   brew install gcc"
    exit 1
fi

echo "✅ Found GCC version: $GCC_VERSION"
GCC_PATH="$HOMEBREW_PREFIX/bin/g++-$GCC_VERSION"

# Check if GCC exists
if [ ! -f "$GCC_PATH" ]; then
    echo "❌ GCC not found at $GCC_PATH"
    exit 1
fi

# Create .vscode directory if it doesn't exist
mkdir -p .vscode

# Update c_cpp_properties.json with detected values
echo ""
echo "📝 Updating VS Code configuration..."

cat > .vscode/c_cpp_properties.json << EOF
{
    "configurations": [
        {
            "name": "$CONFIG_NAME",
            "includePath": [
                "\${workspaceFolder}/**",
                "\${workspaceFolder}/includes",
                "\${workspaceFolder}/src",
                "$HOMEBREW_PREFIX/include",
                "$HOMEBREW_PREFIX/opt/gcc@$GCC_VERSION/include/c++/$GCC_VERSION",
                "$HOMEBREW_PREFIX/opt/libomp/include",
                "/Library/Developer/CommandLineTools/usr/include/c++/v1"
            ],
            "defines": [],
            "compilerPath": "$GCC_PATH",
            "cStandard": "c17",
            "cppStandard": "c++17",
            "intelliSenseMode": "$INTELLISENSE_MODE",
            "compileCommands": "\${workspaceFolder}/build/compile_commands.json",
            "configurationProvider": "ms-vscode.cmake-tools"
        }
    ],
    "version": 4
}
EOF

echo "✅ Created/Updated .vscode/c_cpp_properties.json"

# Update settings.json
cat > .vscode/settings.json << EOF
{
    "cmake.configureOnOpen": true,
    "cmake.buildDirectory": "\${workspaceFolder}/build",
    "cmake.exportCompileCommandsFile": true,
    "cmake.configureSettings": {
        "CMAKE_CXX_COMPILER": "$GCC_PATH",
        "CMAKE_C_COMPILER": "$HOMEBREW_PREFIX/bin/gcc-$GCC_VERSION"
    },
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "C_Cpp.default.compileCommands": "\${workspaceFolder}/build/compile_commands.json",
    "C_Cpp.default.cppStandard": "c++17",
    "C_Cpp.default.includePath": [
        "\${workspaceFolder}/**",
        "\${workspaceFolder}/includes",
        "\${workspaceFolder}/src"
    ],
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    }
}
EOF

echo "✅ Created/Updated .vscode/settings.json"

# Print summary
echo ""
echo "═══════════════════════════════════════"
echo "📋 Configuration Summary"
echo "═══════════════════════════════════════"
echo "Architecture:    $ARCH"
echo "Configuration:   $CONFIG_NAME"
echo "Homebrew:        $HOMEBREW_PREFIX"
echo "GCC Version:     $GCC_VERSION"
echo "GCC Path:        $GCC_PATH"
echo "IntelliSense:    $INTELLISENSE_MODE"
echo "═══════════════════════════════════════"
echo ""
echo "✅ VS Code configuration complete!"
echo ""
echo "Next steps:"
echo "1. Open VS Code: code ."
echo "2. Reload window: Cmd+Shift+P > 'Reload Window'"
echo "3. Build: Cmd+Shift+B"
echo ""
echo "Or build from terminal:"
echo "  mkdir -p build && cd build"
echo "  cmake .. -DCMAKE_CXX_COMPILER=$GCC_PATH"
echo "  cmake --build . -j8"
echo ""