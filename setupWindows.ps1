$also_compile = $false
$compile_config="Release"
$setup_all = $true

# PowerShell build script for Windows
$c78e_path = Split-Path -Parent $MyInvocation.MyCommand.Path

# Check Vulkan SDK
if (-not (Test-Path -Path "$env:VULKAN_SDK")) {
    $ign = Read-Host "VULKAN_SDK not found! Install VulkanSDK and set VULKAN_SDK system variable! Press Enter to exit."
    exit 1
}

# Ensure Git is installed
if (-not (Get-Command git -ErrorAction SilentlyContinue)) {
    $ign = Read-Host "Git not found! Install Git manually and retry. Press Enter to exit."
    exit 1
}

# Check if vendor/premake exists
if (-not (Test-Path -Path "$c78e_path\vendor\premake" -PathType Container)) {
    Write-Output "premake not found! updating submodules"
    git submodule update --init --recursive
}


# Check if export-compile-commands.lua exists
$premakeCompileCommandsPlugin = Join-Path $c78e_path "export-compile-commands.lua"
$premakeCompileCommandsOrigin = Join-Path $c78e_path "vendor\premake-export-compile-commands\export-compile-commands.lua"
# Check if $premakeCompileCommandsPlugin exists
if (-not (Test-Path -Path "$premakeCompileCommandsPlugin")) {
	# Check if $premakeCompileCommandsOrigin exists
	if (-not (Test-Path -Path "$premakeCompileCommandsOrigin")) {
		Write-Output "$premakeCompileCommandsOrigin not found! updating submodules"
		git submodule update --init --recursive
	}
	
	Copy-Item $premakeCompileCommandsOrigin $premakeCompileCommandsPlugin -Force
}

$premake_bin = "$c78e_path\vendor\premake\bin\release\premake5.exe"
# Check Premake
if (-not (Test-Path "$premake_bin")) {
	Write-Host "Compiling Premake..."
	Set-Location "$c78e_path\vendor\premake"
	.\Bootstrap.bat
	Set-Location "$c78e_path"
	#check again
	if (-not (Test-Path "$premake_bin")) {
		$ign = Read-Host "Beautifully failed to install premake. Well that shouldn't happen, your up! Press Enter to exit."
		exit 1
	}
}

#checking Visual Studio / MSBuild
$vswhere = "C:\Program Files (x86)\Microsoft Visual Studio\Installer\vswhere.exe"
if (Test-Path "$vswhere") {
	# Get the latest VS installation with MSBuild
	$vsInfoVerion = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationVersion
	$vsInfoPath = & $vswhere -latest -products * -requires Microsoft.Component.MSBuild -property installationPath

	# Parse the output (two lines: version + path)
	$vsVersion = ($vsInfoVerion | Select-String "^[0-9]") -split "\s+" | Select-Object -First 1
	$vsPath    = ($vsInfoPath | Select-String "^[A-Z]:\\")
	Write-Host "Found MSVS Version: $vsVersion, at: $vsPath" 
	
	# Map Visual Studio version → Premake tag
	switch -Regex ($vsVersion) {
		"^17\." { $premakeTag = "vs2022"; break }
		"^16\." { $premakeTag = "vs2019"; break }
		"^15\." { $premakeTag = "vs2017"; break }
		default { $premakeTag = "unknown" }
	}

	$runPremake = "$premake_bin $premakeTag"
	Write-Host "Premake MSVS Project Files ($runPremake)"
	Invoke-Expression $runPremake
	
	$sln_file = Get-ChildItem -Path $c78e_path -Filter "*.sln" | Select-Object -First 1
	if ($sln_file) {
		if($also_compile) {
			Write-Host "Successfully created Solution File..."
			Write-Host "==== Compiling (Windows Visual Studio / MSBuild, $compile_config) ===="
			
			# Build MSBuild path
			$msbuild64 = Join-Path $vsPath "MSBuild\Current\Bin\amd64\MSBuild.exe"
			$msbuild32 = Join-Path $vsPath "MSBuild\Current\Bin\MSBuild.exe"

			if (Test-Path $msbuild64) {
				$msbuildPath = $msbuild64
			} elseif (Test-Path $msbuild32) {
				$msbuildPath = $msbuild32
			}  else {
				throw "MSBuild.exe not found under $vsPath"
			}
			
			& "$msbuildPath" $sln_file /m /p:Configuration=$compile_config
			exit 0
		} else {
			Write-Host "Successfully created Solution File."
			if(-not $setup_all) {
				exit 0
			}
		}
	} else {
		Write-Host "Generating project files (Windows Visual Studio) failed!"
		exit 1
	}
	
} else {
	Write-Host "Visual Studio not found..."
}

#checking CMake
if (Get-Command cmake -ErrorAction SilentlyContinue) {
	$runPremake = "$premake_bin gmake2"
	Write-Host "==== Generating project files (Windows CMake) ===="
	Invoke-Expression $runPremake
	
	$mk_file = Get-ChildItem -Path $c78e_path -Filter "Makefile" | Select-Object -First 1
	if ($mk_file) {
		if($also_compile) {
			Write-Host "Successfully created Makefile..."
			Write-Host "==== Compiling (Windows CMake, $compile_config) ===="
			& cmake --build . --config $compile_config
			exit 0
		} else {
			Write-Host "Successfully created Makefile."
			if(-not $setup_all) {
				exit 0
			}
		}
	} else {
		Write-Host "Generating project files (Windows CMake) failed!"
		exit 1
	}
} else {
	Write-Host "CMake not found..."
}

Write-Host "No more options for Windows"
exit 0