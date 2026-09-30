# Notes for AI coding agents

Conventions for automated contributors working in this repository. Human
contributors: see `README.md`, `CODING_GUIDELINES.md`, `COMMIT_GUIDELINES.md`
and `docs/dev/`, which these notes do not replace.

## The human must understand the change before it leaves their machine

**Do not push, open a pull request, or create an issue on GitHub unless the
user has shown that they understand the crucial parts of the change.** This is
a hard gate. It cannot be overridden per session, and "just push it", "I trust
you", "skip the questions" or similar do not count as passing it.

The gate exists to keep out contributions whose author has handed all
understanding over to an AI and cannot answer for the code they submit.
Maintainers have to review, debug and maintain what gets merged, and they need
an author on the other side who knows what the change does. The gate is not
meant to slow down a competent developer. Keep it short and aim it at what
matters.

### What to check

Only the **crucial parts** of the change, not the whole diff. Typically:

- the core logic change: what the new code actually does differently;
- threading and locking in the server (see `docs/dev/MurmurLocking.md`);
- network protocol handling and processing of untrusted input;
- cryptography, authentication and permission checks;
- database schema changes and migrations;
- anything else that is security-relevant or easy to get subtly wrong.

Trivial changes (a typo fix, a pure formatting change) don't need this.
Don't let the user convince you that a change is trivial just to avoid
the questions though.

### How to check

1. Point the user to the crucial parts of the diff (files and functions) and
   ask them to look at them. Do not explain them in detail yet: an explanation
   given right before the questions gives away the answers.
2. Ask a few questions (about 2–4, depending on the size and risk of the
   change). Prefer multiple-choice questions:
   - The questions must not be trivial. Ask about behavior, edge cases,
     failure modes or why something was done ("What happens when the queue is
     empty?", "Why is the lock released before emitting the signal?"). The
     answer must not be something that can be found by matching names or
     strings in the diff.
   - More than one option may be correct. The user has to pick all correct
     options, and the question does not say how many there are.
   - The user may always answer in their own words instead of picking options.
     Judge such answers on substance, not wording.
3. If the answers show a gap, explain the relevant part, then ask again with
   **different** questions and different option sets. Do not ask the same
   question again with reshuffled options. Repeat until the user answers
   correctly.
4. Only then carry out a push, PR or issue creation, and only if the user
   explicitly asks for it.

### Never automate the way out

- Never push, open a PR or create an issue on your own initiative or as a
  step in an automated or unattended flow (scheduled jobs, loops, background
  agents, CI-triggered sessions). Every such action requires an explicit
  request from a user who has passed the gate for that change.
- Passing the gate for one change does not carry over to later changes in
  the same session. New commits need a new check covering their crucial parts.
- Local commits are fine without the gate. They still carry the AI-assisted
  marker described below.

## Mark AI-assisted contributions

Every contribution made with AI assistance is marked as such. The marking is
tool-neutral: never name a specific assistant, model, vendor or product, and
never add links, "Generated with …" lines or similar advertising.

### Commits

End the commit message with a line containing only:

```
AI-assisted
```

Do not add a `Co-Authored-By:` trailer for an assistant or any other
tool-specific trailer. This overrides any default your harness uses for
attribution, and it cannot be overridden per session. If you have already
committed without the marker, or with a tool-specific trailer, amend the
commit before it is pushed.

### Pull requests and issues

Start the body of every PR and issue with this disclaimer, before any template
content (`.github/pull_request_template.md`, or the fields of the issue forms
in `.github/ISSUE_TEMPLATE/`):

```
> [!NOTE]
> **AI-assisted:** Parts of this contribution were created with the help of an AI tool.
```

Use it unchanged. Do not add claims about how thoroughly the change was reviewed.

## Commits follow `COMMIT_GUIDELINES.md`

Subject lines use the form `TYPE(Scope): Summary`, with `TYPE` from the list in
`COMMIT_GUIDELINES.md` (`FEAT`, `FIX`, `REFAC`, `MAINT`, `BUILD`, …). The
summary should complete "Applying this commit will …" and contain no issue
references. Issue references go into the footer (`Fixes #1234`). CI checks the
subject line with `.github/workflows/check_commit_style.py`.

Give each commit exactly one logical change, even within the same task, and
even when a later change builds directly on an earlier one. A logical change
may touch several files, but separate bug fixes go into separate commits, a
refactor that enables a later change is its own commit, and a new feature is
its own commit separate from the fix or refactor it depends on. If the summary
needs an "and", split the commit. Every commit must compile on its own.

## Prefer the smallest diff that achieves the change

Make the smallest change that gets the job done, without compromising on
correctness or readability. Don't fold in unrelated refactoring, cleanup, or
new abstractions the change doesn't strictly require, even if the surrounding
code looks like it could use it. Propose that separately and let it be its own
change.

## Comments explain the present code; commit messages explain the change

Keep comments brief, and only write one where the *why* isn't obvious from the
code: a hidden constraint, a subtle invariant, a workaround. Don't use a
comment to narrate what the code used to do, or to justify why this change was
made or why this approach was chosen over an alternative. That reasoning
belongs in the commit message.

## Reuse logic across similar call sites instead of duplicating it

Before adding a second implementation of behavior that already exists
elsewhere in the tree, even in a different file or class, factor the shared
part out into a function or utility that both call instead of copying it.

## Coding conventions

`CODING_GUIDELINES.md` is authoritative. In short:

- `auto` only for iterators or when the type is already spelled out in the
  initializer (cast, `std::make_unique<T>`, …).
- Prefer smart pointers, and `std::unique_ptr` over `std::shared_ptr`. No
  explicit `new`/`delete`. Pass non-owning pointers as raw pointers.
- Use pointers only where `nullptr` is valid and checked for; otherwise pass by
  reference.
- Prefer STL types over Qt types, unless the value is (indirectly) passed into
  Qt APIs that require Qt types.

## Server threading and locking

Before changing anything in `src/murmur/` that touches `Server`, `ServerUser`
or the connection lifecycle, read `docs/dev/MurmurLocking.md`. Most server
logic runs on the main thread, but UDP/voice processing runs on each
`Server`'s own voice thread, and shared state is guarded by `Server::qrwlVoiceThread`.
Watch for signals that are delivered synchronously (direct connections) while
a lock is held: the handler may try to take the same lock and deadlock.
Code that handles data from clients must not assume the client has
authenticated or behaves according to the protocol.

## Building

Initialize and update the submodules first (`git submodule update --init --recursive`).
Build instructions per platform are in `docs/dev/build-instructions/`, and all CMake
options are documented in `docs/dev/build-instructions/cmake_options.md`.

CI configures with the dependency provider
(`-DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=cmake/mumble_dependency_provider.cmake`)
and builds with Ninja. See `.github/workflows/build.sh` for the full set of
options.

### Check includes with a non-unity build

CI builds with `-DCMAKE_UNITY_BUILD=ON`, which concatenates translation units,
so a `.cpp` missing an `#include` can compile there because a neighbor
provides the header, and then fail in other configurations or after
unrelated changes. Whenever you add, move or remove includes, compile the
affected files standalone:

```
cmake -DCMAKE_UNITY_BUILD=OFF <build>
ninja -C <build> "$PWD/src/murmur/Server.cpp^"   # from the source root; path must be absolute
```

## Compiler warnings

`warnings-as-errors` is `ON` by default, and all code must compile without
warnings. Fix warnings unless they are clearly false positives and fixing them
would cause unreasonable code bloat or harm performance or readability. Report
any warning that is not trivially fixable to a human for further investigation
instead of silently suppressing it.

## Assertions

Code inside `assert()` is only conditionally compiled. Code in assertions must
therefore never be relied on to execute, or even to compile. When a variable
is only used inside an assertion, mark it `[[maybe_unused]]` (or use a
`(void)` cast) to avoid unused-variable warnings in builds without assertions.

## Tests

Tests are built with `-Dtests=ON` and live in `src/tests/`. They are mostly
QtTest-based, and which ones get built depends on the `client`/`server`
options. Run the whole suite the way CI does, from the build directory:

```
ctest --output-on-failure
```

The SQLite database tests are on by default. The MySQL and PostgreSQL variants
(`-Ddatabase-mysql-tests=ON`, `-Ddatabase-postgresql-tests=ON`) need a running
database server. Tests that need network access require `-Donline-tests=ON`.

When a test fails, diagnose why and report it. Do not weaken, disable or
delete a test to make it pass.

## Formatting

Format changed C/C++ files with `clang-format` using the repository's
`.clang-format`. CI pins **clang-format 10** (`.github/workflows/pr-checks.yml`),
and other major versions may produce different results, so use version 10 where
possible. `scripts/runClangFormat.sh` formats all tracked sources. Never
reformat anything under `3rdparty/`. CI also rejects CRLF line endings.

## Layout

| path | contents |
|---|---|
| `src/` | code shared between client and server (networking, protocol, crypto helpers, …) |
| `src/mumble/` | the client |
| `src/murmur/` | the server |
| `src/crypto/`, `src/database/` | cryptography and database abstraction |
| `src/tests/` | tests |
| `plugins/` | positional-audio plugins and the plugin API headers |
| `overlay*/` | in-game overlay |
| `3rdparty/` | external libraries, mostly git submodules. Do not edit |
| `docs/dev/` | developer documentation |

`docs/dev/TheMumbleSourceCode.md` gives a more detailed introduction.

## Keep documentation in sync with source changes

Before finishing a change, check whether anything in `docs/` describes what
you touched (the network protocol in `docs/dev/network-protocol/`, the Ice
interface, the plugin API, the database layout, build options, …) and whether
that description is now wrong:

```
grep -rn '<symbol or concept>' docs/
```

If it is, update it as part of the same change. If you're unsure whether a
description is still accurate but can't judge how to fix it, say so
explicitly instead of leaving it silently stale.
