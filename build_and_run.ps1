# PowerShell build and run script
$ErrorActionPreference = 'Stop'

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $root

# Create build folder
$buildDir = Join-Path $root '.generated'
if (-not (Test-Path $buildDir)) { New-Item -ItemType Directory -Path $buildDir | Out-Null }

# Compile driver (detect compiler)
$driver = Join-Path $root 'main.cpp'
$outExe = Join-Path $buildDir 'banglish_driver.exe'

Write-Host 'Compiling driver...'
$clCmd = Get-Command cl -ErrorAction SilentlyContinue
if ($clCmd) {
    & cl /nologo /EHsc /std:c++17 `"$driver`" /Fe:`"$outExe`"
}
else {
    $gppCmd = Get-Command g++ -ErrorAction SilentlyContinue
    if (-not $gppCmd) { throw 'No C++ compiler found. Install Visual Studio Build Tools (cl) or MinGW (g++).' }
    & g++ -std=c++17 -O2 -o `"$outExe`" `"$driver`"
}

if ($LASTEXITCODE -ne 0) { throw 'Compilation failed' }

# Run driver
Write-Host 'Running driver...'
& "$outExe"
if ($LASTEXITCODE -ne 0) { throw 'Driver execution failed' }

Write-Host 'Done. See output files and .generated folder.'
