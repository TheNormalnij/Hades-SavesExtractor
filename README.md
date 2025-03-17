
# Hades save file extractor

Extracts Hades save files to a human redable Lua format. Imports Lua files back to the save file format.
The tool wasn't fully tested, so use it at your own risk. Always make a backup of your save files.

#### Supported games
+ Hades
+ Hades II

# Build

## Winows x64 with MSVC

```shell
cmake -A x64 . -B build_msvc
cmake --build build_msvc --config Release
```

# Usage

## Using simple scripts

1. Copy your save file (Profile<X>.sav where <X> is a save slot) from `Documents/Saved Games/Hades` to the HadesSavesExtractor.exe directory.
2. Rename the save file to `Profile1.sav`.
3. Run the script `extract.bat` to extract the save file to a Lua file.
4. Edit the `Profile1.lua` file.
5. Run the script `import.bat` to import the Lua file back to the save file format.
6. Copy the `Profile1.sav` file back to `Documents/Saved Games/Hades`.

## Using command line

```shell
HadesSavesExtractor.exe --extract <path to the .sav file> --out <path to the output .lua file>
```

```shell
HadesSavesExtractor.exe --import <path to the input .lua file> --out <path to the output .sav file>
```

### Example:

You have Profile1.sav in the same directory with HadesSavesExtractor.exe.

```shell
# Extract the save file
HadesSavesExtractor.exe --extract Profile1.sav --out Profile1.lua
# Import the save file
HadesSavesExtractor.exe --import Profile1.lua --out Profile1.sav
```
