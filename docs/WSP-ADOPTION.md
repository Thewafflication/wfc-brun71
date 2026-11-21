# WSP Adoption Record

**Content type:** Project adoption record

**Project:** wfc-brun71 (BASIC PDS 7.1 text-mode runtime)

**WSP release:** 1.4.0

**Submodule path:** `wsp/`

**Pinned commit:** `f009399dd1406219571ac978bfee85e85bbdfeac`

**Status:** Adopted from the start, with the same scripts and settings as the
other WFC libraries; the open items are listed below

The library is one of the WFC libraries and follows the WFC adoption record
(`planning/wsp-adoption-record.md` in
[WFC](https://github.com/Thewafflication/wfc)); this record covers what the
library repository itself carries.

## Selected profiles

| Profile | Selected | Scope or rationale |
| --- | --- | --- |
| Security/DFS | Yes | Hardening flags (WSP-SEC-0015) from `wsp/tools/cmake/WspBuild.cmake` in the standalone build |
| C source style | No | The sources are C++ |
| CMake style | Yes | `CMakeLists.txt` |
| Windows version resources | Not applicable | The library is static; no DLL yet |
| Windows code signing and Defender | Deferred | Until the first release |
| Documentation | Yes | `README.md`, `ROADMAP.md`, `COMPATIBILITY.md` |

## State (2026-11-21)

| Item | WSP source | Status | Notes |
| --- | --- | --- | --- |
| WSP pin | `wsp/architecture/adr-0001` | Done | 1.4.0, the release WFC pins |
| C++ and CMake formatters | `WSP-LANG-0002`, `WSP-LANG-0010` | Done | `.clang-format` (22.1.3), `.gersemirc` (0.29.2), copies of the WFC ones |
| Editor config, 80-column style | `WSP-STYLE-0001` to `0004` | Done | `.editorconfig`, `.gitattributes`; checked by `Test-SourceStyle.ps1` in the lint stage |
| Lint, build and test stages | `WSP-CHECK-0001` to `0004`, `0007`, `WSP-STYLE-0007` | Done | `scripts/Invoke-Lint.ps1`, `Invoke-Build.ps1`, `Invoke-Tests.ps1` (x64 Debug, zero tests is an error) |
| Commit gate | `WSP-CHECK-0001` | Done | `.pre-commit-config.yaml`; `scripts/Start-CommitGate.ps1` and `Show-TestProgress.ps1` are identical to the WFC copies |
| CI backstop | `WSP-CHECK-0008`, `WSP-TEST-0012`, `0013` | Partial | Lint and the x86/x64/ARM64 matrix in `.github/workflows/ci.yml`; first run pending |
| Requirements | `WSP-REQM-0001` to `0010` | Partial | `REQ-0001` to `REQ-0003`, status "Proposed"; no per-statement IDs yet |
| Test cases and traceability | `WSP-TEST-0001` to `0005`, `0011` | Done | `TC-0001` to `TC-0004`: specification `tests/tc-NNNN-*.tex`, CTest `TC-NNNN-<name>`, runner `tests/run-tc-NNNN.ps1`; `Test-Traceability.ps1` in the lint stage |
| Reference evidence | WFC practice | Open | No rule is checked against BRUN71 itself yet (`COMPATIBILITY.md`) |
| Commit-checks record, static analysis, source documentation, release records | `WSP-CHECK-0006`, `WSP-SAST`, `WSP-STYLE-0006`, `WSP-REL` | Open | As for the other libraries |
