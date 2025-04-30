#!/bin/bash

VENV_DIR=".venv"

BUILD_TYPES=("Release" "Debug" "RelWithDebInfo")

check_conan() {
  if command -v conan &> /dev/null; then
    echo "Conan is installed."
    return 0
  else
    echo "Conan is not installed."
    return 1
  fi
}

setup_venv_and_conan() {
  if [ ! -d "$VENV_DIR" ]; then
    echo "Creating virtual environment in $VENV_DIR..."
    if command -v python3 &> /dev/null; then
      python3 -m venv "$VENV_DIR"
    elif command -v python &> /dev/null; then
      echo "Warning: 'python3' not found, using 'python'."
      python -m venv "$VENV_DIR"
    else
      echo "Error: Neither 'python3' nor 'python' found. Cannot create virtual environment."
      exit 1
    fi

    if [ $? -ne 0 ]; then
      echo "Error creating virtual environment."
      exit 1
    fi
    echo "Virtual environment created."
  else
    echo "Virtual environment $VENV_DIR already exists."
  fi

  if [ -f "$VENV_DIR/bin/activate" ]; then
    source "$VENV_DIR/bin/activate"
    echo "Virtual environment activated."
  else
     echo "Error: Activation script not found in $VENV_DIR/bin/activate."
     exit 1
  fi

  if ! command -v conan &> /dev/null; then
    echo "Installing Conan in the virtual environment..."
    pip install conan
    if [ $? -ne 0 ]; then
      echo "Error installing Conan in the virtual environment."
      exit 1
    fi
    echo "Conan installed in the virtual environment."
  else
      echo "Conan is already available in the activated environment."
  fi
}

if check_conan; then
  echo "Using existing Conan installation."
else
  setup_venv_and_conan
fi

echo "Starting Conan installations..."

for build_type in "${BUILD_TYPES[@]}"; do
  echo ""
  echo "--- Running conan install for build_type: $build_type ---"

  conan install . \
    --profile:host=c++26-rflection \
    --profile:build=c++26-rflection \
    --settings=build_type="$build_type" \
    --build=missing

  if [ $? -ne 0 ]; then
    echo "Error: conan install failed for build_type $build_type."
  fi
done

echo ""
echo "Conan installation script finished."