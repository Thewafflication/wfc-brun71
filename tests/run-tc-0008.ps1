<#
.SYNOPSIS
Runs test case TC-0008.
.DESCRIPTION
Runs the CTest test TC-0008-* (see tests/tc-0001-*.tex for the
specification).
.PARAMETER BuildDirectory
CMake binary directory, relative to the repository root.
#>
[CmdletBinding()]
param([string]$BuildDirectory = 'out/build')

& (Join-Path $PSScriptRoot 'Invoke-TestCase.ps1') -Id 'TC-0008' `
    -BuildDirectory $BuildDirectory
