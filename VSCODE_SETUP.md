# VS Code Setup for SwarmBot

## 🛠️ Fix "include errors detected" Message

This guide fixes the IntelliSense errors you're seeing in VS Code.

## Quick Fix

### Step 1: Install Required Extensions

In VS Code, install these extensions:
1. **C/C++** by Microsoft (ms-vscode.cpptools)
2. **CMake Tools** by Microsoft (ms-vscode.cmake-tools)

### Step 2: Run CMake Configure

```bash
# From the refactored_swarmbot directory
mkdir -p build
cd build
cmake ..
```

This creates `compile_commands.json` which tells VS Code where all the includes are.

### Step 3: Reload VS Code

1. Press `Ctrl+Shift+P` (or `Cmd+Shift+P` on Mac)
2. Type "Reload Window"
3. Press Enter

The include errors should now be gone!

## 📁 What Was Added

### .vscode/c_cpp_properties.json
Tells IntelliSense where to find header files:
- `${workspaceFolder}/includes`
- `${workspaceFolder}/src`
- System includes

### .vscode/settings.json
Configures CMake integration and file associations.

### Updated CMakeLists.txt
Now exports `compile_commands.json` with:
```cmake
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
```

## 🔧 Manual Fix (If Still Having Issues)

### Option 1: Check Configuration
1. Click the "Select a Kit" notification in VS Code
2. Choose your compiler (GCC or Clang)
3. Let CMake Tools configure

### Option 2: Manually Set Include Path

If includes still aren't working:

1. Press `Ctrl+Shift+P`
2. Type "C/C++: Edit Configurations (UI)"
3. Under "Include path", add:
   ```
   ${workspaceFolder}/**
   ${workspaceFolder}/includes
   ${workspaceFolder}/src
   ```

### Option 3: Check Compiler Path

In `.vscode/c_cpp_properties.json`, update `compilerPath`:

**Linux**:
```json
"compilerPath": "/usr/bin/g++"
```

**Mac**:
```json
"compilerPath": "/usr/bin/clang++"
```

**Windows**:
```json
"compilerPath": "C:/msys64/mingw64/bin/g++.exe"
```
(Adjust path based on your installation)

## ✅ Verification

After setup, you should see:
- ✅ No red squiggles under `#include` statements
- ✅ Auto-completion works for Othello classes
- ✅ Go to Definition (F12) works
- ✅ IntelliSense shows function signatures

## 🐛 Common Issues

### Issue 1: "Cannot find compile_commands.json"
**Solution**: 
```bash
cd build
cmake ..
```

### Issue 2: Includes still not found
**Solution**:
1. Check that `includes/` and `src/` folders exist
2. Verify files have correct paths
3. Reload window

### Issue 3: OpenMP errors
**Solution**: Install OpenMP:
```bash
# Ubuntu/Debian
sudo apt install libomp-dev

# Mac
brew install libomp

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-openmp
```

### Issue 4: Wrong configuration selected
**Solution**:
1. Click configuration name in bottom status bar
2. Select the right one:
   - "Linux" for Linux
   - "Mac" for macOS  
   - "Win32" for Windows

## 💡 Pro Tips

### Tip 1: Use CMake Tools Extension
- Click "Build" button in status bar
- Automatic configuration
- Integrated debugging

### Tip 2: Format on Save
Add to `.vscode/settings.json`:
```json
{
    "editor.formatOnSave": true,
    "C_Cpp.clang_format_style": "Google"
}
```

### Tip 3: Build from VS Code
Press `Ctrl+Shift+B` to build without leaving VS Code

### Tip 4: Debugging
1. Set breakpoints (click left of line numbers)
2. Press F5 to debug
3. CMake Tools sets this up automatically

## 📚 Recommended Settings

Add these to `.vscode/settings.json` for best experience:

```json
{
    "cmake.configureOnOpen": true,
    "C_Cpp.intelliSenseEngine": "default",
    "C_Cpp.errorSquiggles": "enabled",
    "C_Cpp.autocomplete": "default",
    "editor.suggestSelection": "first",
    "C_Cpp.default.cppStandard": "c++17"
}
```

## 🎯 Complete Workflow

### First Time Setup
```bash
# 1. Open project in VS Code
code refactored_swarmbot/

# 2. Install extensions (C/C++, CMake Tools)

# 3. Configure with CMake
mkdir build && cd build
cmake ..

# 4. Reload VS Code
# Ctrl+Shift+P > "Reload Window"
```

### Daily Workflow
```bash
# Build (Ctrl+Shift+B)
# Or from terminal:
cd build
cmake --build .

# Run
./swarmbot
```

## 🆘 Still Having Issues?

If nothing works:

1. **Delete build folder**:
   ```bash
   rm -rf build
   ```

2. **Reconfigure**:
   ```bash
   mkdir build && cd build
   cmake ..
   ```

3. **Reload VS Code**:
   - Close all files
   - Close VS Code
   - Reopen project

4. **Check Output**:
   - View > Output
   - Select "C/C++" from dropdown
   - Look for errors

## ✨ Success Indicators

You'll know it's working when:
- ✅ Includes are underlined in green (found)
- ✅ Hovering over functions shows documentation
- ✅ Ctrl+Click jumps to definition
- ✅ Auto-complete suggests Othello:: classes
- ✅ No "include errors detected" message

That's it! Your VS Code should now have full IntelliSense support for the SwarmBot project.