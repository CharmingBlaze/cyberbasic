# Contributing to CyberBasic

Thank you for contributing! This document defines how we collaborate to keep the codebase clean, modular, and friendly.

## Principles

- Prefer clarity over cleverness. Small, focused changes.
- No duplicate function registrations; respect ownership boundaries.
- Follow the architecture in `PLAN.md` and the tasks in `CHECKLIST.md`.
- Windows builds use MSYS2 MinGW-w64 + Ninja. Avoid Visual Studio generator.

## Workflow

1. Create an issue describing the change.
2. Branch naming: `feature/<brief>`, `fix/<brief>`, or `chore/<brief>`.
3. Keep PRs small and focused. One topic per PR.
4. Run: configure + build + tests locally (MSYS2 MinGW + Ninja). Attach logs if failing.
5. If your change affects bindings, re-run the generator and commit the diffs.
6. Add/adjust tests when fixing bugs or adding behavior.
7. Update `PLAN.md`/`CHECKLIST.md`/docs if you introduce or change a policy.

## Code Review

- At least one approval required.
- Reviewers check: duplication, ownership, type marshaling, lifecycle safety, style, tests.
- Avoid adding to "god files"; refactor to modules when touching large areas.

## Binding Generator Guidance

- Generated raylib layer must:
  - Use strict types: bool→`from_bool`, ints→`from_int`, floats→`from_number`.
  - Marshal structs (Color, Vector, Rectangle) as arrays per `PLAN.md`.
  - Avoid duplicates with builtins; skip or prefix conflicting names (e.g., `RL_`).
  - Stay modular: emit into `src/raylib/*.cpp` per category.

## Commit Style

- Conventional-ish: `feat:`, `fix:`, `refactor:`, `docs:`, `test:`, `chore:`.
- Reference issues like `Fixes #123` when applicable.

## CI Expectations

- Build with Ninja (release + debug).
- Run unit + integration tests.
- Run generator, ensure no unexpected diffs.
- Fail on duplicate registration unless allowed.

## Deprecations & Legacy Removal

- Follow `docs/LEGACY_CLEANUP.md`.
- Use `CYBERBASIC_LEGACY` guards and clear DEPRECATED comments.
- Provide a short deprecation window unless security/instability requires immediate removal.
