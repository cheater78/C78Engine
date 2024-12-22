#!/bin/bash

# WIP build script

parent_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

run_compiler=""
run_premake=""

# test Vulkan SDK
if [[ -z "$VULKAN_SDK" && -f "$VULKAN_SDK" ]]; then
    echo "VULKAN_SDK not found! The VulkanSDK has to be installed and the VULKAN_SDK system variable has to be set!"
    exit 1
fi

# platform detection
if [[ "$OSTYPE" == "linux-gnu"* ]]; then # linux
    # test git
    if ! [ -x "$(command -v git)" ]; then
        read -p "git not found! Do you want to install it? This requires the apt package manager!(Y / n)" confirm && [[ -z "$confirm" || "$confirm" == [yY] || "$confirm" == [yY][eE][sS] ]] || exit 1
        sudo apt install git -y
    fi

    # test make
    if ! [ -x "$(command -v make)" ]; then
        read -p "make not found! want to install it?(Y / n)" confirm && [[ -z "$confirm" || "$confirm" == [yY] || "$confirm" == [yY][eE][sS] ]] || exit 1
        sudo apt install make -y
    fi
    run_compiler="make -j $(( $(nproc) + 1 ))" # nproc works for gnu only!

    # test premake
    if [ -x "$(command -v premake)" ]; then
        run_premake="premake gmake2"
    else
		if ! [ -x "$parent_path/vendor/premake/bin/release/premake5" ]; then
            echo "premake not found! updating submodules"
            git submodule update --init --recursive
            echo "compiling premake"
            cd "$parent_path/vendor/premake/"
            make -f Bootstrap.mak linux      # Linux and similar Posix systems
            cd "$parent_path"
        fi
        run_premake="$parent_path/vendor/premake/bin/release/premake5 gmake2"
    fi
    
elif [[ "$OSTYPE" == "darwin"* ]]; then # Mac OSX
    echo "Mac OSX is currently not supported!"
    #make -f Bootstrap.mak osx        # for premake on Mac OS X
    exit 1
elif [[ "$OSTYPE" == "cygwin" ]]; then # POSIX compatibility layer and Linux environment emulation for Windows
    echo "WSL is currently not supported!"
    exit 1
elif [[ "$OSTYPE" == "msys" ]]; then # Lightweight shell and GNU utilities compiled for Windows (part of MinGW)
    echo "MinGW is currently not supported!"
    exit 1
elif [[ "$OSTYPE" == "win32" ]]; then # windows
    echo "Windows is currently not supported!"
	# test git
    if ! [ -x "$(command -v git)" ]; then
        read -p "git not found! Do you want to install it? This requires the apt package manager!(Y / n)" confirm && [[ -z "$confirm" || "$confirm" == [yY] || "$confirm" == [yY][eE][sS] ]] || exit 1
        echo "git not found"
		exit -1
    fi

    # test make
    if ! [ -x "$(command -v make)" ]; then
        read -p "make not found! want to install it?(Y / n)" confirm && [[ -z "$confirm" || "$confirm" == [yY] || "$confirm" == [yY][eE][sS] ]] || exit 1
        echo "make not found"
    fi
    run_compiler="make -j $(( $(nproc) + 1 ))" # nproc works for gnu only!

    # test premake
    if [ -x "$(command -v premake)" ]; then
        run_premake="premake gmake2"
    else
		if ! [ -x "$parent_path\vendor\premake\bin\release\premake5.exe" ]; then
            echo "premake not found! updating submodules"
            git submodule update --init --recursive
            echo "compiling premake"
            cd "$parent_path\vendor\premake\"
            nmake -f Bootstrap.mak windows   # for premake on Windows
            cd "$parent_path"
        fi
        run_premake="$parent_path\vendor\premake\bin\release\premake5.exe vs2022"
    fi
    #
    exit 1
elif [[ "$OSTYPE" == "freebsd"* ]]; then # FreeBSD
    echo "FreeBSD is currently not supported!"
    exit 1
else
    echo "Failed to detect your platform! ('$OSTYPE' is not supported!)"
    exit 1
fi

echo "==== Creating project files($OSTYPE, $run_compiler) ===="
$run_premake

echo "==== Compiling started($OSTYPE,$run_compiler) ===="
$run_compiler
