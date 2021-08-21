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


## Pointer vs. Reference

Use pointers only if `nullptr` is a valid option (and is therefore explicitly checked for). Whenever you are expecting that a passed value is set,
pass that value by reference instead in order to make sure that the semantics forbid passing `nullptr` to the function.


## Qt vs. STL

Wherever feasible, you should prefer using types from the standard library (STL) instead of Qt-specific ones. Note that this only applies if you don't
have to pass that type (indirectly) into Qt functions that only accept Qt-specific types. In these cases, prefer Qt-types.

