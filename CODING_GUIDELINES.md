# Mumble coding guidelines

## Formatting

We use [clang-format](https://clang.llvm.org/docs/ClangFormat.html) to format our source code. The details of the formatting are fixed in the
`.clang-format` file at the root of this repository.

When making changes to the source code, please always reformat the changed files using this tool in order to ensure a consistent formatting across the
code base.


## Use of `auto`

Since we are using a more modern C++ standard, the usage of the `auto` keyword is in principle possible. However we have decided that for the sake of
readability of the code we want to restrict its usage to the following cases:
1. Usage of STL-iterators
2. If the expression of the assignment already contains the type explicitly (e.g. because of a cast or by usage of e.g. `make_unique`)

An example of the first scenario would be
```cpp
auto it = myVector.begin();
```
And examples for the second case are
```cpp
auto myObj   = new MyObject();
auto myOther = std::make_unique< MyOther >();
auto number  = static_cast< int >(1.5);
```


## Smart pointers

You should always prefer smart-pointers over raw pointers. The general rule is: Never use `new` and `delete` explicitly.

Also prefer `std::unique_ptr` over `std::shared_ptr`, unless you are really intending for the object to have shared ownership.

When it comes to passing pointers into functions, always pass as raw-pointer, unless you want to transfer ownership of the pointer to the function.

When using Qt-types, you may have to use the `qt_unique_ptr` provided in `QtUtils.h` which uses the `deleteLater` function rather than a plain call
to `delete`.


## Pointer vs. Reference

Use pointers only if `nullptr` is a valid option (and is therefore explicitly checked for). Whenever you are expecting that a passed value is set,
pass that value by reference instead in order to make sure that the semantics forbid passing `nullptr` to the function.


## Qt vs. STL

Wherever feasible, you should prefer using types from the standard library (STL) instead of Qt-specific ones. Note that this only applies if you don't
have to pass that type (indirectly) into Qt functions that only accept Qt-specific types. In these cases, prefer Qt-types.


## Variable Naming

Your variables should carry meaningful names that make it clear what the variable represents. Member variables should be prefixed with `m_`, static
variables with `s_` and other than these two, no prefixes should be used. In particular, do not use hungarian notation (variable name prefixes
encoding the variable's type). Variable names should use camelCase.


## Visibility

When adding member variables or functions, prefer restricting visibility as much as possible. That is, use `private` or `protected` instead of
`public` variables/functions for better encapsulation. Prefer public getter/setter functions for variables to public variables unless the latter
offers clear advantages for the specific use case.


## Self-documenting Types

Whenever possible, prefer the use of self-documenting types. An example of this is `std::chrono::seconds` that inherently encodes that it represents
seconds, which is way superior than e.g. having an `int` value that one has to know represents seconds.

Another advantage of using such types is that some mis-uses of the variable are prevented by means of compiler errors. Whenever possible, this should
be preferred. Having the compiler check invariants is much better than runtime checks, comments or implicit knowledge.


## Curly Braces Around Control Structures

All control structures (if, for, while, etc.) should always enclose their body in curly braces. Even in cases in which the C++ standard would allow to
omit them.
