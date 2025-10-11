# Rule 59: Don't write namespace using-directives or using-declarations before #include directives

## Explanation
Never place `using` directives or declarations before `#include` statements. This can cause the included headers to be interpreted in an unexpected namespace context, leading to subtle bugs and name conflicts.

## Pros
- **Predictable Behavior**: Headers are included in their expected namespace context
- **Avoids Hidden Bugs**: Prevents subtle name resolution issues
- **Better Hygiene**: Headers remain isolated from using-declarations
- **Standard Practice**: Follows expected C++ conventions

## Cons
- None - this is a best practice that should always be followed

## Examples
See the example files in this directory for demonstrations.
