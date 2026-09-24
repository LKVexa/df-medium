# DF Medium

**1.0.1** (`DF-PA21.2-1.0.1`) by **RUSSELL PHILIP SMITHSON**.

DF Medium provides the `N_MEDIUM` classical VM node for the PA-LCTL fabric.
It embeds the BOTTLE ROCKET 5.0.0 qualification candidate with its frozen
4.7.0 runtime contract: LCTLC/1.1, BRIR/1.1, ISA 4.1, ABI 1.0 and Device ABI 1.0.
This is the separate delivery of the supplied `DF_Medium` folder.

## Setup and validation

Use Python 3.10+ in a virtual environment:

```sh
python -m pip install --only-binary=:all: -r REQUIREMENTS.txt
python -B tools/validate_release.py
python -B adapter/dfabric/cli.py node-verify
```

The portable validator checks complete inventories, payload hashes, malformed
image parsing, output-path refusal and specification synchronization. CI runs
these checks on Windows and Linux with Python 3.10 and 3.14.

Native validation requires Linux, a C11 compiler, GNU make and OpenSSL/libcrypto
development files. `node-verify` builds and runs the native acceptance and adapter
battery in a disposable copy. Missing prerequisites are explicitly SKIPPED.
Run `python -B tools/validate_native.py` for the VM, wide-state and device
sanitizers plus regressions for all three host readers and administrative I/O.
CI also exercises these native checks on Linux with Python 3.12.

Build the node for local use with `python -B adapter/dfabric/cli.py node-build`.
Use trusted, dedicated work and state directories. The source distribution
excludes original machine-specific `.build` binaries and generated state.

## Security and qualification scope

Host readers enforce size bounds, reject nonregular files and symlinks, and
close handles on failure. Key generation uses exclusive creation, refuses
existing paths, and creates private keys with mode 0600. Choose new paths for
both outputs; a second-file failure can leave a newly created private key.
Existing key paths are neither overwritten nor deleted. Never commit generated
key material or use historical qualification fixtures as production trust.

The development host's buffered-write double close is fixed. APDU line input is
bounded, and adapter unit identifiers cannot escape their output directory.
The production host remains separate from the administrative signing tool.

The original strict qualification gate remains **BLOCKED**. A literal 72-hour
soak, independent operator/machine evidence, AArch64/native Windows qualification,
production trust provisioning and cross-host federation are not supplied by this
release. No physical quantum execution is claimed. Update external DF node
registry pins deliberately before binding this derived payload.

## Provenance and license

The root release version is 1.0.1; VM ISA/ABI labels retain the frozen contract.
Original signed manifests and historical evidence remain unchanged and describe
the original candidate, not these edits. `provenance/PAYLOAD_CHANGES.json` maps
original hashes to the current payload, and current inventories cover the full
source tree. The pinned PA-LCTL core is unchanged. Hashes detect drift; they do
not provide independent authentication.

See [AUDIT.md](AUDIT.md), [SECURITY.md](SECURITY.md), [LICENSE](LICENSE) and
[NOTICE](NOTICE). Copyright 2026 **RUSSELL PHILIP SMITHSON**, Apache License 2.0.
