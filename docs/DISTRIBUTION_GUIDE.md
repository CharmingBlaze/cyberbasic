# Distribution Guide

**How to distribute your CyberBasic games to players**

## Table of Contents
1. [Overview](#overview)
2. [What to Include](#what-to-include)
3. [Distribution Methods](#distribution-methods)
4. [Platform-Specific Instructions](#platform-specific-instructions)
5. [Packaging Scripts](#packaging-scripts)
6. [Best Practices](#best-practices)

---

## Overview

CyberBasic games are distributed as:
- The `cyberbasic.exe` (or `cyberbasic` on Linux/macOS) interpreter
- Your `.bas` game files
- Assets (images, sounds, music, data files)
- Any required DLLs (if not statically linked)

Players run your game by executing: `cyberbasic.exe game.bas`

---

## What to Include

### Required Files

1. **CyberBasic Executable**
   - `cyberbasic.exe` (Windows) or `cyberbasic` (Linux/macOS)
   - Located in your `build/` or `build-mingw/` directory

2. **Game Files**
   - All `.bas` files your game uses
   - Include any imported/included files

3. **Assets**
   - Images (`.png`, `.jpg`, etc.)
   - Sounds (`.wav`, `.ogg`, `.mp3`)
   - Music files
   - Data files (`.json`, `.txt`, etc.)

4. **Dependencies** (if not statically linked)
   - Windows: May need Visual C++ redistributables
   - Linux: May need system libraries
   - macOS: Usually bundled in `.app`

### Recommended Files

- `README.txt` - Instructions for players
- `LICENSE.txt` - License information
- Launcher script (`.bat` on Windows, `.sh` on Linux/macOS)

---

## Distribution Methods

### Method 1: Portable Zip Archive (Easiest)

**Best for:** Quick distribution, no installation needed

**Steps:**

1. **Create distribution folder:**
   ```
   MyGame_v1.0/
   ├── cyberbasic.exe
   ├── game.bas
   ├── assets/
   │   ├── sprites/
   │   ├── sounds/
   │   └── music/
   └── README.txt
   ```

2. **Create README.txt:**
   ```
   MyGame v1.0
   
   INSTRUCTIONS:
   1. Extract all files
   2. Run: cyberbasic.exe game.bas
   
   CONTROLS:
   - WASD: Move
   - ESC: Exit
   ```

3. **Zip the folder:**
   - Windows: Right-click → Send to → Compressed folder
   - Linux/macOS: `zip -r MyGame_v1.0.zip MyGame_v1.0/`

4. **Distribute the zip file**

### Method 2: Installer (Windows)

**Best for:** Professional distribution, easy installation

**Tools:**
- [Inno Setup](https://jrsoftware.org/isinfo.php) (Free, recommended)
- [NSIS](https://nsis.sourceforge.io/) (Free)
- [WiX Toolset](https://wixtoolset.org/) (Free, more complex)

**Inno Setup Example:**

```inno
[Setup]
AppName=MyGame
AppVersion=1.0
DefaultDirName={pf}\MyGame
DefaultGroupName=MyGame
OutputDir=dist

[Files]
Source: "cyberbasic.exe"; DestDir: "{app}"
Source: "game.bas"; DestDir: "{app}"
Source: "assets\*"; DestDir: "{app}\assets"; Flags: recursesubdirs

[Icons]
Name: "{group}\MyGame"; Filename: "{app}\cyberbasic.exe"; Parameters: "game.bas"
Name: "{commondesktop}\MyGame"; Filename: "{app}\cyberbasic.exe"; Parameters: "game.bas"
```

### Method 3: AppImage (Linux)

**Best for:** Linux distribution, single file

**Steps:**

1. Create AppDir structure:
   ```
   MyGame.AppDir/
   ├── cyberbasic
   ├── game.bas
   ├── assets/
   └── AppRun (executable script)
   ```

2. Create AppRun:
   ```bash
   #!/bin/sh
   cd "$(dirname "$0")"
   ./cyberbasic game.bas
   ```

3. Use [appimagetool](https://github.com/AppImage/AppImageKit) to create AppImage

### Method 4: DMG (macOS)

**Best for:** macOS distribution

**Steps:**

1. Create application bundle:
   ```
   MyGame.app/
   └── Contents/
       ├── MacOS/
       │   └── cyberbasic
       └── Resources/
           ├── game.bas
           └── assets/
   ```

2. Use Disk Utility or createDMG to create DMG file

---

## Platform-Specific Instructions

### Windows

#### Static Linking (Recommended)

Build with static linking to avoid DLL dependencies:

```bash
cmake .. -DBASIC_STATIC_LINK=ON -DCMAKE_BUILD_TYPE=Release
cmake --build .
```

This bundles all dependencies into `cyberbasic.exe`.

#### Distribution Structure

```
MyGame/
├── cyberbasic.exe
├── game.bas
├── run_game.bat
├── assets/
└── README.txt
```

**run_game.bat:**
```batch
@echo off
cd /d "%~dp0"
cyberbasic.exe game.bas
pause
```

#### Testing

Test on a clean Windows system without development tools:
1. Copy your distribution folder
2. Run `cyberbasic.exe game.bas`
3. Verify all assets load correctly

### Linux

#### Distribution Structure

```
MyGame/
├── cyberbasic
├── game.bas
├── run_game.sh
├── assets/
└── README.txt
```

**run_game.sh:**
```bash
#!/bin/bash
cd "$(dirname "$0")"
./cyberbasic game.bas
```

Make executable: `chmod +x run_game.sh`

#### Dependencies

May need to include or document:
- libc6 (usually pre-installed)
- libstdc++6
- libgcc-s1

#### AppImage or Flatpak

For broader compatibility, consider packaging as AppImage or Flatpak.

### macOS

#### Distribution Structure

```
MyGame.app/
└── Contents/
    ├── MacOS/
    │   └── cyberbasic
    └── Resources/
        ├── game.bas
        └── assets/
```

#### Code Signing

For distribution outside the App Store:
1. Get Apple Developer certificate
2. Sign the executable: `codesign --sign "Developer ID" cyberbasic`
3. Create DMG and sign it

---

## Packaging Scripts

### Windows Packaging Script

Create `package_game.ps1`:

```powershell
$GameName = "MyGame"
$Version = "1.0"
$BuildDir = "build-mingw"
$DistDir = "dist/${GameName}_${Version}"

# Create distribution directory
New-Item -ItemType Directory -Force -Path $DistDir
New-Item -ItemType Directory -Force -Path "$DistDir/assets"

# Copy files
Copy-Item "$BuildDir/cyberbasic.exe" $DistDir
Copy-Item "game.bas" $DistDir
Copy-Item -Recurse "assets/*" "$DistDir/assets/"

# Create launcher
@"
@echo off
cd /d "%~dp0"
cyberbasic.exe game.bas
pause
"@ | Out-File -FilePath "$DistDir/run_game.bat" -Encoding ASCII

# Create README
@"
$GameName v$Version

INSTRUCTIONS:
1. Extract all files
2. Double-click run_game.bat

CONTROLS:
- WASD: Move
- ESC: Exit
"@ | Out-File -FilePath "$DistDir/README.txt" -Encoding ASCII

# Create zip
Compress-Archive -Path $DistDir -DestinationPath "dist/${GameName}_${Version}.zip" -Force

Write-Host "Package created: dist/${GameName}_${Version}.zip"
```

### Linux/macOS Packaging Script

Create `package_game.sh`:

```bash
#!/bin/bash
GAME_NAME="MyGame"
VERSION="1.0"
BUILD_DIR="build"
DIST_DIR="dist/${GAME_NAME}_${VERSION}"

# Create distribution directory
mkdir -p "$DIST_DIR/assets"

# Copy files
cp "$BUILD_DIR/cyberbasic" "$DIST_DIR/"
cp "game.bas" "$DIST_DIR/"
cp -r "assets/"* "$DIST_DIR/assets/"

# Create launcher
cat > "$DIST_DIR/run_game.sh" << 'EOF'
#!/bin/bash
cd "$(dirname "$0")"
./cyberbasic game.bas
EOF
chmod +x "$DIST_DIR/run_game.sh"

# Create README
cat > "$DIST_DIR/README.txt" << EOF
${GAME_NAME} v${VERSION}

INSTRUCTIONS:
1. Extract all files
2. Run: ./run_game.sh

CONTROLS:
- WASD: Move
- ESC: Exit
EOF

# Create tar.gz
cd dist
tar -czf "${GAME_NAME}_${VERSION}.tar.gz" "${GAME_NAME}_${VERSION}"
cd ..

echo "Package created: dist/${GAME_NAME}_${VERSION}.tar.gz"
```

---

## Best Practices

### 1. Build for Release

Always build with Release configuration:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Release
```

### 2. Use Static Linking (Windows)

Avoid DLL dependencies:

```bash
cmake .. -DBASIC_STATIC_LINK=ON
```

### 3. Test on Clean Systems

Test your distribution on a system without development tools installed.

### 4. Include Clear Instructions

Create a README with:
- Installation instructions
- Controls
- System requirements
- Troubleshooting tips

### 5. Organize Assets

Keep assets organized:
```
assets/
├── sprites/
├── sounds/
├── music/
└── data/
```

### 6. Version Your Releases

Include version numbers in:
- Distribution folder name
- README file
- Game title (optional)

### 7. License Information

Include license information for:
- Your game
- CyberBasic
- Any third-party assets

### 8. Optimize File Sizes

- Compress images appropriately
- Use efficient audio formats
- Remove unused assets

---

## Distribution Checklist

Before distributing, verify:

- [ ] Game runs on clean system
- [ ] All assets are included
- [ ] README is clear and complete
- [ ] Launcher script works
- [ ] Version number is correct
- [ ] License information included
- [ ] Tested on target platform
- [ ] No debug files included
- [ ] File paths are relative
- [ ] All dependencies included (or documented)

---

## Example Distribution Structure

```
MyGame_v1.0/
├── cyberbasic.exe          (or cyberbasic on Linux/macOS)
├── game.bas
├── run_game.bat            (or run_game.sh)
├── README.txt
├── LICENSE.txt
└── assets/
    ├── sprites/
    │   ├── player.png
    │   └── enemy.png
    ├── sounds/
    │   ├── jump.wav
    │   └── collect.wav
    └── music/
        └── background.mp3
```

---

## Advanced: Creating Installers

### Inno Setup (Windows)

1. Download [Inno Setup](https://jrsoftware.org/isinfo.php)
2. Use the example script above
3. Compile to create installer

### AppImage (Linux)

1. Install [appimagetool](https://github.com/AppImage/AppImageKit)
2. Create AppDir structure
3. Run: `appimagetool MyGame.AppDir MyGame.AppImage`

### DMG (macOS)

1. Create application bundle
2. Use Disk Utility or createDMG
3. Sign with Developer ID (optional)

---

## Support

For distribution questions:
- Check this guide
- Review examples in the repository
- Open an issue on GitHub

**Happy distributing!** 🚀

