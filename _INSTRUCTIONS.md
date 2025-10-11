## Book
Sutter C++ Coding Standards. 2005

Explain rule 57~63 and show examples, pros & cons
Namespaces and Modules
- 57. Keep a type and its nonmember function interface in the same namespace.
- 58. Keep types and functions in separate namespaces unless they're specifically intended to work together.
- 59. Don't write namespace usings in a header file or before an #include.
- 60. Avoid allocating and deallocating memory in different modules.
- 61. Don't define entities with linkage in a header file.
- 62. Don't allow exceptions to propagate across module boundaries.
- 63. Use sufficiently portable types in a module's interface.

Use this structure to describe each rules
```
- Summary
- Reason
- Example (good)
- Example (bad)
- Decision (good for & not recommended for): Please also provide exampels for not recommended for
```

## OpenCL Examples

Show me those rules in OpenCL examples
- using a simple computer vision OpenCL algorithm as an example