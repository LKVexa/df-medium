# DF Medium 1.0.1 audit

Work was performed in a separate copy of DF_Medium; the original is unchanged.

## Findings and repairs

- Original shared adapter files matched DF Fabric. Reused tested manifest
  containment, strict records/hashes/counts and link/duplicate rejection.
- Administrative key generation previously truncated existing files and unlinked
  the private path on any failure. Exclusive creation now preserves existing
  files and refuses symbolic links. Private file mode is 0600.
- Administrative, production and independent-verifier readers previously sized
  allocations from unbounded file lengths. They now cap input at the image or
  image-plus-trust-envelope ceiling, initialize failure outputs, reject links,
  nonregular files and FIFO input, cleanse failed buffers and close streams.
- Fixed a double fclose in the development image writer when flush failed;
  repaired verifier read cleanup. Added /dev/full and descriptor regression cases.
- Bounded administrative APDU line input and capped signing-image growth.
- Adapter unit identifiers now reject path escapes, reserved device names,
  injected newlines and oversized names before lowering or creating outputs.
- Fixed UTF-8 specification test reads. Native tests reuse their generated smoke
  key pair so repeated sanitizer targets do not rely on overwriting private keys.
- Excluded original .build outputs. Retained original signed manifests and
  historical evidence; current digest records identify the hardened derivation.
  The native checksum gate now uses RELEASE_CONTENTS.sha256 for derived bytes,
  preserving the original SHA256SUMS and its historical signature context.
  Added Apache-2.0 LICENSE/NOTICE and README naming RUSSELL PHILIP SMITHSON.

## Validation

Local Python 3.12: 13 regression tests (12 passed, one Windows link-creation
privilege skip), including every truncation of the shipped BRIM image and
selected malformed-image fields. The portable release validator also checks
specification synchronization and the full root/payload inventories.

CI covers Windows/Linux Python 3.10/3.14 portable validation and Linux native
acceptance, adapter gates, source-size limits, VM/wide/device ASan/UBSan, and
all three host-reader regressions with leak detection. Review the actual Actions
result for native outcomes; this local Windows host has no C compiler.

The original qualification gate stays BLOCKED. Existing signed manifests are
historical and do not authenticate changed bytes. The original PA-LCTL core and
frozen runtime ABI remain unchanged. AArch64/native Windows qualification,
a literal 72-hour soak, independent operator/machine evidence, production trust
provisioning and cross-host federation remain outside this release's validation.
