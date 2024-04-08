# Mumble Commit Message Guidelines

A commit groups file changes with a title and description of the changes and additional metadata like change date, author, and committer.

These Commit Message Guidelines define and describe how changes shall be structured and described in commits.
Most notably, the text form in which to title the changes.

## General philosophy

- **Each change goes into its own commit**. If you want to summarize what you did with this commit (in the commit message)
and you start using the word "and", you probably want to split the commit up into 2 or more individual commits.

- **Each commit should compile** (try to make each commit self-contained so that it is possible to compile the code at
each commit).

- **Take your time** when composing the commit message. In order to have a good git-history the commit messages are essential.
Also if you put effort into the commit-message, you'll save work when creating a PullRequest as the description is already available.


## Commit message

Commit messages must follow this scheme:

```
TYPE(Scope): Summary

Message Body

Footer
```

The blank lines in between are mandatory. A commit **must** include a `TYPE` and a `Summary` and **may**
additionally contain any of the other components listed here.


### Subject line

The first line ("Subject line") should not exceed 50-70 characters. This is what gets displayed on GitHub at first glance, so it
should contain the most important information. In order for it to be as short and precise as possible, there is the `TYPE` and
optionally a `Scope`. With these it should already be clear what this commit is about in general. The short `Summary` should
then include further information that is important to understand the general idea of this commit at a glance.


#### TYPE

The `TYPE` is one of the following:

| **TYPE** | **Description** | **Example** |
| -------- | --------------- | ----------- |
| BREAK    | A breaking change - not backwards-compatible | A change in protocol (e.g. change UDP message format) |
| FEAT     | Introduction of a new feature (or extension of an existing one) | |
| FIX      | A bug fix | |
| FORMAT   | Change of formatting - does not influence how the code works | Change indentation of a line; Add braces around `if` body |
| DOCS     | Changes to the documentation (either in-source or out-of-source) | Add a Doxygen comment to a function |
| TEST     | Adds, changes or removes a test-case | |
| MAINT    | Maintenance - Change of non-code files | Make changes to `.plist` or `.rc` files |
| CI       | Changed something for the CI (continuous integration) | Update TravisCI to use newer ubuntu version |
| REFAC    | Code refactoring | Rename variable `x` to `y` |
| BUILD    | Changes related to the build process / buildsystem | Fix cmake script |
| TRANSLATION | Translation updates and changes | Update translation files |
| CHANGE   | Something was changed without falling into existing categories | Changed the default of a setting |
| REVERT   | A previous commit had to be reverted because e.g. it was buggy | - |

The `TYPE` has to be in **all-uppercase** in order for it to stand out.

If you feel like you need to use 2 or more types for a single commit but *can't split it* into multiple commits, you can
combine types with `/`: `FEAT/CI: <Summary>`


#### Scope

What area is the change about. For now we don't have fixed scope keywords. A scope could be something like `ui`, `client`,
`server`, `ice`, ...


#### Summary

The `Summary` is the heart of the subject line. It should contain a **very brief** summary of what you did in that commit.
In order to make this as short as possible, you may use grammatically incorrect sentences
("Add ability" instead of "add the ability").

In general the `Summary` should answer the question "Applying this commit will ..." where "..." is your `Summary`.

If your `Summary` contains "and", you should probably split your commit up.

Note: Issue references (such as #2305) **must not** be used in the `Summary`!


### Message Body

Here you give more details about the commit. Why is it necessary and what are the details of the change. You can use
multiple paragraphs for this and be as verbose as you want.

The `Message Body` should reference issues that are related to this change, but also provide a short summary of what that
issue is about (so that it can be understood without having to open that issue).

The `Message Body` should contain enough information for someone to be able to look at this commit at some point in the
future and know exactly what it does and why it was needed.


### Footer

The `Footer` contains a list of issue references prefixed by a keyword like `Closes`, `Fixes` or `Implements`.

Each reference should be on its own line:

```
Implements #1234
Closes #2215
```

Additionally the `Footer` may contain another paragraph indicating that a given commit was co-authored by other people.
Each co-author should be listed in a new line like so:

```
Co-Authored-By: Author name <author@example.com>
Co-Authored-By: Other author <other@example.com>
```


### References

We have used the term "reference" a few times in these guidelines. A *reference* has the form `#<ID>` where `<ID>` is the
ID GitHub assigns to the respective issue or PullRequest (it's the number that is displayed next to the title). You'll
notice that these references are actually turned into a link when viewing the commit message on GitHub. This is done
automatically by GitHub. You don't have to use a link in your commit message. In fact you *should not* use a link for
this purpose as this is just unnecessarily verbose and makes it hard to read the commit message outside of GitHub.


### Examples

```
FEAT(client): Add possibility to change username

Co-Authored-by: Gina Taylor <g.taylor@myspace.com>
```
```
FEAT(client): Add possibility to change username

As requested in #1234 this commit implements the ability to change your
username while being connected to a server.

For that X and Y had to be implemented.

The change now works by sending a XYZ message to the server with the "foo"
and "bar" fields set.

Implements #1234
```
```
FIX(client): Crash when loading settings
```
```
MAINT: Add XY to README
```

-----

This guide was inspired by https://github.com/bluejava/git-commit-guide

