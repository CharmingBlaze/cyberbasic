# Legacy Code Cleanup Policy

A consistent process to identify, deprecate, and remove legacy or unused code while keeping the repo healthy.

## Identification

- Search for unused symbols with ripgrep/IDE references and coverage reports.
- Flag features duplicating builtins vs generated raylib bindings.
- Look for "god file" sections that belong in modules per `PLAN.md`.

## Criteria for Deprecation

- Duplicates a capability owned elsewhere (per Ownership in `PLAN.md`).
- Unsafe or undefined behavior (e.g., returning pointers to temporaries).
- Unused and blocks modularization or clarity.

## Deprecation Process

1. Create an issue with scope and rationale.
2. Wrap blocks in `#ifdef CYBERBASIC_LEGACY` with a `// DEPRECATED: <reason> (since vX.Y)` comment.
3. Provide migration notes in `CHANGELOG.md` and, if applicable, code pointers.
4. Add tests to cover the supported path; avoid adding new tests for deprecated path.

## Removal

- After one release cycle (or immediately if unsafe), remove the guarded code.
- Keep history in Git; do not keep commented-out code.
- Update docs and examples accordingly.

## Tooling Support

- CI job greps for `CYBERBASIC_LEGACY` to list active deprecations.
- CI fails if deprecated blocks exceed approved count after a milestone.

## Examples

- Duplicate function in `src/rt_raylib.gen.cpp` conflicting with builtins → generator skip or `RL_` prefix, remove duplicate.
- Unsafe Color/Vector returns → replace with array marshaling; delete legacy pointer-return code.

## Checklist (Per Cleanup PR)

- [ ] Issue created with rationale and scope
- [ ] Deprecated blocks wrapped and documented
- [ ] Migration notes added to CHANGELOG
- [ ] Tests updated (no reliance on deprecated paths)
- [ ] Removal scheduled (milestone set)
