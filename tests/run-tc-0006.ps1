<#
.SYNOPSIS
Runs test case TC-0006.
.DESCRIPTION
Runs the CTest test TC-0006-* (see tests/tc-0001-*.tex for the
specification).
.PARAMETER BuildDirectory
CMake binary directory, relative to the repository root.
#>
[CmdletBinding()]
param([string]$BuildDirectory = 'out/build')

& (Join-Path $PSScriptRoot 'Invoke-TestCase.ps1') -Id 'TC-0006' `
    -BuildDirectory $BuildDirectory
