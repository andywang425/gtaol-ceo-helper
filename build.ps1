$ErrorActionPreference = "Stop"

function Invoke-Step {
    param(
        [Parameter(Mandatory = $true)]
        [scriptblock]$Command
    )

    & $Command
    if ($LASTEXITCODE -ne 0) {
        exit $LASTEXITCODE
    }
}

$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
Set-Location $scriptDir

if (-not (Test-Path ".venv\Scripts\python.exe")) {
    Invoke-Step { uv venv }
}

Invoke-Step { uv sync --group dev --frozen }

Write-Host "Building gtaol-ceo-helper..."
Invoke-Step { uv run pyinstaller --noconfirm --clean --onefile --console --name "gtaol-ceo-helper" main.py }

Write-Host "Copying config.example.yaml to dist\config.yaml"
Copy-Item -Path "config.example.yaml" -Destination "dist\config.yaml" -Force

Write-Host "Building RegionLocator..."
Invoke-Step { gcc -Wall -Wextra -O2 "RegionLocator.c" -o "dist\RegionLocator.exe" }

Write-Host "Packaging gtaol-ceo-helper.exe and config.yaml into zip..."
$packageDir = "dist\gtaol-ceo-helper"
New-Item -Path $packageDir -ItemType Directory -Force | Out-Null
Copy-Item -Path "dist\gtaol-ceo-helper.exe" -Destination $packageDir -Force
Copy-Item -Path "dist\config.yaml" -Destination $packageDir -Force
Compress-Archive -Path $packageDir -DestinationPath "dist\gtaol-ceo-helper.zip" -Force
Remove-Item -Path $packageDir -Recurse -Force

Write-Host ""
Write-Host "Build finished"
