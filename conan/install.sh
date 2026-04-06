#!/bin/bash

if [ -z "$1" ]; then
    echo "Fatal Error: Missing preset name."
    echo "Usage: ././conan/install.sh <PresetName> | all"
    echo "Example 1: ././conan/install.sh Debug-gcc"
    echo "Example 2: ././conan/install.sh all"
    exit 1
fi

OS_NAME=$(uname -s)
IS_LINUX=false
IS_WINDOWS=false
SCRIPT_DIR=$(cd "$(dirname "${BASH_SOURCE[0]}")" &>/dev/null && pwd)

case "$OS_NAME" in
Linux*) IS_LINUX=true ;;
CYGWIN* | MINGW* | MSYS*) IS_WINDOWS=true ;;
esac

install_config() {
    echo "========================================="
    echo " Installing conan config                 "
    echo "========================================="
}

install_preset() {
    local PRESET=$1
    local PROFILE=""
    local BUILD_TYPE=""

    case "$PRESET" in
    "Debug-gcc")
        PROFILE="gcc-16"
        BUILD_TYPE="Debug"
        ;;
    "Release-gcc")
        PROFILE="gcc-16"
        BUILD_TYPE="Release"
        ;;
    "RelWithDebInfo-gcc")
        PROFILE="gcc-16"
        BUILD_TYPE="RelWithDebInfo"
        ;;
    "Sanitize-gcc")
        PROFILE="gcc-16"
        BUILD_TYPE="Debug"
        ;;
    "Coverage-gcc")
        PROFILE="gcc-16"
        BUILD_TYPE="Debug"
        ;;

    "Debug-clang")
        PROFILE="clang-21"
        BUILD_TYPE="Debug"
        ;;
    "Release-clang")
        PROFILE="clang-21"
        BUILD_TYPE="Release"
        ;;
    "RelWithDebInfo-clang")
        PROFILE="clang-21"
        BUILD_TYPE="RelWithDebInfo"
        ;;
    "Sanitize-clang")
        PROFILE="clang-21-sanitizer"
        BUILD_TYPE="Debug"
        ;;

    "Debug-msvc")
        PROFILE="msvc-194"
        BUILD_TYPE="Debug"
        ;;
    "Release-msvc")
        PROFILE="msvc-194"
        BUILD_TYPE="Release"
        ;;
    "RelWithDebInfo-msvc")
        PROFILE="msvc-194"
        BUILD_TYPE="RelWithDebInfo"
        ;;
    "Sanitize-msvc")
        PROFILE="msvc-194"
        BUILD_TYPE="Debug"
        ;;

    *)
        echo "Fatal Error: Unknown preset mapping for '$PRESET'"
        exit 1
        ;;
    esac

    echo "========================================="
    echo " Configuring Environment: $PRESET"
    echo " Conan Profile: $PROFILE"
    echo " Build Type:    $BUILD_TYPE"
    echo "========================================="

    conan install "$SCRIPT_DIR" \
        -pr:b "$PROFILE" \
        -pr:h "$PROFILE" \
        -s build_type="$BUILD_TYPE" \
        --build=missing \
        -c "tools.system.pipenv:python_interpreter=$PYTHON_EXE"

    if [ $? -eq 0 ]; then
        echo ">> Success: Toolchain generated for $PRESET."
        echo ""
    else
        echo ">> Error: Conan installation failed for $PRESET."
        exit 1
    fi
}

if [ "$1" == "all" ]; then
    echo "Executing batch installation for current OS..."
    if [ "$IS_LINUX" = true ]; then
        PRESETS=("Debug-gcc" "Release-gcc" "Debug-clang" "Release-clang")
        for p in "${PRESETS[@]}"; do
            install_preset "$p"
        done
    elif [ "$IS_WINDOWS" = true ]; then
        PRESETS=("Debug-msvc" "Release-msvc")
        for p in "${PRESETS[@]}"; do
            install_preset "$p"
        done
    else
        echo "Fatal Error: Unsupported OS for 'all' command."
        exit 1
    fi
else
    install_preset "$1"
fi
