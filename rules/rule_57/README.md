# Rule 57: Keep a type and its nonmember function interface in the same namespace

## Explanation
Functions that are part of a type's interface but are not member functions should be placed in the same namespace as the type itself. This keeps related functionality together and enables argument-dependent lookup (ADL/Koenig lookup).

## Pros
- **ADL Support**: Enables argument-dependent lookup, making function calls more natural
- **Logical Organization**: Keeps related functionality together
- **Better Encapsulation**: Clear interface boundary
- **Easier Discovery**: Users can find all related functions in one namespace

## Cons
- **Namespace Pollution**: Can add many symbols to the namespace
- **Header Dependencies**: May require including more headers
- **Compilation Time**: Can increase compilation time with more symbols

## Examples
See the example files in this directory for demonstrations.
