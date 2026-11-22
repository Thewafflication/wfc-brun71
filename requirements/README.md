# wfc-brun71 requirements

**Content type:** Project requirement index

Identifiers are `REQ-NNNN`, numbered from 0001 in this repository's own
namespace.

| Requirement | Subject | Verified by |
| --- | --- | --- |
| [REQ-0001](req-0001-text-screen.md) | Text screen of `SCREEN 0` | `TC-0001` |
| [REQ-0002](req-0002-keyboard.md) | Keyboard and `INKEY$` | `TC-0002` |
| [REQ-0003](req-0003-console-host.md) | Code page 437 and the Windows console host | `TC-0003`, `TC-0004` |
| [REQ-0004](req-0004-pds-programs.md) | PDS programs run in WFC | `TC-0005` |
| [REQ-0005](req-0005-input.md) | `INPUT`, `LINE INPUT` and `INPUT$` | `TC-0006`, `TC-0005` |
| [REQ-0006](req-0006-print-using.md) | `PRINT USING` | `TC-0007`, `TC-0005` |
| [REQ-0007](req-0007-sound.md) | `SOUND`, `PLAY`, `BEEP` and `SLEEP` | `TC-0008`, `TC-0005` |
| [REQ-0008](req-0008-soft-keys.md) | Soft keys: `KEY n`, `ON`, `OFF`, `LIST` | `TC-0009`, `TC-0005` |
| [REQ-0009](req-0009-key-traps.md) | Key trapping: `ON KEY(n) GOSUB`, `KEY(n)` | `TC-0010`, `TC-0005` |
| [REQ-0010](req-0010-errors-write-screen.md) | Error messages, `WRITE`, `SCREEN()` | `TC-0011`, `TC-0001`, `TC-0005` |

The test case specifications are `tests/tc-NNNN-<title>.tex`; the CTest names
are `TC-NNNN-<name>`; `tests/run-tc-NNNN.ps1` runs one test case (checked by
`wsp/tools/Test-Traceability.ps1` in the lint stage).
