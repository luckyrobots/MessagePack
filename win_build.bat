@echo off
setlocal

REM Setup MSVC environment
SET "VS_PATH=C:\Program Files\Microsoft Visual Studio\2022\Community"
SET "VCVARSALL_BAT=%VS_PATH%\VC\Auxiliary\Build\vcvarsall.bat"

IF NOT EXIST "%VCVARSALL_BAT%" (
    echo WARNING: Could not find vcvarsall.bat at %VCVARSALL_BAT%
    echo Please ensure MSVC Build Tools are installed and the path is correct.
    echo Continuing, but build may fail if environment is not set up.
) ELSE (
    echo Calling %VCVARSALL_BAT% x64
    call "%VCVARSALL_BAT%" x64
    IF ERRORLEVEL 1 (
        echo ERROR: Failed to initialize MSVC environment using vcvarsall.bat.
        exit /b 1
    )
)

REM Exit on error
REM Batch files don't have a direct equivalent of set -e. Errors need to be checked explicitly if necessary.

REM Get the directory where the script is located
SET "SCRIPT_DIR=%~dp0"
REM Remove trailing backslash if present
IF "%SCRIPT_DIR:~-1%"=="\" SET "SCRIPT_DIR=%SCRIPT_DIR:~0,-1%"

REM Change to the project directory
cd /D "%SCRIPT_DIR%"

REM Clean up Intermediate, Binaries, .vs, DerivedDataCache, and Saved folders
echo Cleaning up Intermediate, Binaries, .vs, DerivedDataCache, and Saved folders...
IF EXIST "%SCRIPT_DIR%\Intermediate" rd /s /q "%SCRIPT_DIR%\Intermediate"
IF EXIST "%SCRIPT_DIR%\Binaries" rd /s /q "%SCRIPT_DIR%\Binaries"
IF EXIST "%SCRIPT_DIR%\.vs" rd /s /q "%SCRIPT_DIR%\.vs"
IF EXIST "%SCRIPT_DIR%\DerivedDataCache" rd /s /q "%SCRIPT_DIR%\DerivedDataCache"
IF EXIST "%SCRIPT_DIR%\Saved" rd /s /q "%SCRIPT_DIR%\Saved"

IF EXIST "%SCRIPT_DIR%\Plugins\MessagePackPlugin\Intermediate" rd /s /q "%SCRIPT_DIR%\Plugins\MessagePackPlugin\Intermediate"
IF EXIST "%SCRIPT_DIR%\Plugins\MessagePackPlugin\Binaries" rd /s /q "%SCRIPT_DIR%\Plugins\MessagePackPlugin\Binaries"

REM Check if .uproject file exists
SET "UPROJECT_FILE=MessagePack.uproject"
IF NOT EXIST "%SCRIPT_DIR%\%UPROJECT_FILE%" (
    echo Error: %UPROJECT_FILE% not found in %SCRIPT_DIR%!
    exit /b 1
)

REM Get the Unreal Engine installation path
REM You may need to modify this path based on your UE installation
SET "UE_PATH=C:\Program Files\UE_5.5"

REM Check if UE path exists
IF NOT EXIST "%UE_PATH%" (
    echo Error: Unreal Engine not found at %UE_PATH%
    echo Please modify the UE_PATH variable in this script to point to your Unreal Engine installation
    exit /b 1
)

REM Check if Engine directory exists
IF NOT EXIST "%UE_PATH%\Engine" (
    echo Error: Engine directory not found at %UE_PATH%\Engine
    exit /b 1
)

REM Path to the UE build script
SET "UE_BUILD_SCRIPT=%UE_PATH%\Engine\Build\BatchFiles\Build.bat"

IF NOT EXIST "%UE_BUILD_SCRIPT%" (
    echo Error: UE Build Script not found at %UE_BUILD_SCRIPT%!
    exit /b 1
)

echo Using Unreal Engine at: %UE_PATH%

REM Regenerate project files before building
SET "PROJECT_FILES_GEN_SCRIPT=%UE_PATH%\Engine\Build\BatchFiles\Build.bat"
IF NOT EXIST "%PROJECT_FILES_GEN_SCRIPT%" (
    echo Error: Project files generation script not found at %PROJECT_FILES_GEN_SCRIPT%!
    exit /b 1
)
echo Regenerating project files for: %UPROJECT_FILE%
call "%PROJECT_FILES_GEN_SCRIPT%" -project="%SCRIPT_DIR%\%UPROJECT_FILE%" -game

echo Building project: %UPROJECT_FILE%

REM Build the project (e.g., Game target, Win64 platform, Development configuration)
REM You can change "MessagePack" to "MessagePackEditor" to build the editor target
REM You can change "Development" to "Shipping" or "DebugGame" for other configurations
SET "TARGET_NAME=MessagePack"
SET "PLATFORM=Win64"
SET "CONFIGURATION=Development"

echo Attempting to build target: %TARGET_NAME%, Platform: %PLATFORM%, Configuration: %CONFIGURATION%

call "%UE_BUILD_SCRIPT%" "%TARGET_NAME%" "%PLATFORM%" "%CONFIGURATION%" -project="%SCRIPT_DIR%\%UPROJECT_FILE%" -waitmutex

echo Build process finished.
echo If successful, binaries can be found in the Binaries\%PLATFORM% directory.

endlocal 