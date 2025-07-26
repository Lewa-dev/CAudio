# CAudio

A simple/basic audio library with a C interface based on OpenAL-Soft.<br>
Used in/originally intended for the game [Celaria](https://store.steampowered.com/app/1081650/Celaria/).<br>
Implements sound playback, positional audio and sound/music-streaming.<br>
Supports only .ogg audio files.

## Build
Works only on Windows (for now). Requires a CMake installation and Visual Studio 2022.<br>
After downloading the repo:
1. Execute "Initialise.bat" which prepares VCPKG and downloads/builds OpenAL-Soft.
2. Open the repo directory in Visual Studio as a CMake project. Visual studio will configure the project.
3. Build it. ("CTRL+SHIFT+B" or at the top menu "Build > Build All")
