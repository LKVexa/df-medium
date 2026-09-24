# Security scope

Use source, binaries, state and output directories owned by a trusted local user.
Manifest verification rejects links, path escapes, duplicates and malformed evidence,
but assumes that the tree is not concurrently modified by an attacker.

Key generation refuses existing paths. An incomplete operation may leave a new
private key at the explicitly requested path; inspect the result before using it.
Never reuse development/qualification trust fixtures or commit generated keys.
Native tests and sanitizers are regression evidence, not formal isolation proofs.

Original release signatures cover original artifacts only. Current derived
inventories are not signed by the unavailable historical release key. Physical
trust provisioning and external qualification requirements remain BLOCKED.
Report reproducible issues to the GitHub owner without exposing private keys.
