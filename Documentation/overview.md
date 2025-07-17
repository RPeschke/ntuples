
# Abstract

The NTuple library provides a type-safe way to associate values with names directly at the type level in C++. Rather than relying on strings or indices at runtime, each value is bound to a unique type representing its name, allowing strong typing, readable output, and compile-time validation. This makes NTuple particularly suited for applications where clarity, safety, and introspection are valuable.

Users can define field names either ahead of time or on the fly, and then combine them into NTuple objects. Each field behaves like the underlying value but retains its name, enabling readable output such as `arg1: 42`. Entire tuples can be streamed to `std::ostream`, showing a structured view of their contents. Tuples can also be subsetted or recombined, making them highly composable.

A standout use case is in constructing flexible, named function arguments. With the `bind_args` utility, developers can define default values for arguments and specify required fields using `std::optional`. At call time, arguments can be passed positionally or by name, in any order. The mechanism enforces correctness at compile time and enforces presence of required fields at runtime. This gives a clean alternative to traditional overloading or manual parsing of argument packs.

Together, these features make NTuple a lightweight but powerful abstraction for representing structured, named data in a strongly typed and composable way, well-suited for modern C++ applications and functional-style pipelines.



## Overview

The `NTuple` library introduces a type-safe, lightweight mechanism for associating **field names with values** at the type level. By encoding field names into the type system, this approach enables structured data manipulation without relying on string-based keys or runtime reflection. All field names are part of the type, enabling compile-time safety and introspectable semantics.

### Defining Field Names

To define new field names, use the `nt_new_field_name` macro:

```cpp
nt_new_field_name(arg1);
nt_new_field_name(arg2);
nt_new_field_name(arg3);
```

Each macro invocation introduces a new type representing the field name. Field names can be wrapped in a namespace for encapsulation and scoping:

```cpp
namespace fields {
  nt_new_field_name(id);
  nt_new_field_name(label);
}
```

### Creating Named Values

You can create a named value by assigning a value to the field name:

```cpp
auto instance = arg1 = 42;
```

This creates a container holding the value `42` tagged with the field name `arg1`. The result behaves much like an ordinary `int`, but retains its name for later introspection. Streaming it to `std::ostream` prints:

```
arg1: 42
```

### Creating an NTuple

Multiple field-name/value pairs can be combined into an NTuple using the `nt::ntuple` function:

```cpp
auto t = nt::ntuple(
  arg1 = 42,
  arg2 = 232.12,
  arg3 = std::string("hello world")
);
```

The resulting `t` is an NTuple containing three named fields. Each field can be accessed directly:

```cpp
t.arg1      // behaves like an int
t.arg2      // behaves like a double
t.arg3      // behaves like a std::string
```

Each member behaves like the underlying value but retains its field name. For example:

```cpp
std::cout << t.arg1 << "\n";
```

will produce:

```
arg1: 42
```

Printing the entire tuple:

```cpp
std::cout << t << "\n";
```

yields:

```
|arg1: 42 | arg2: 232.12 | arg3: "hello world"|
```

### Creating Sub-Tuples

You can also use existing field values to construct new NTuples:

```cpp
auto t2 = nt::ntuple(t.arg1);
```

This creates a new NTuple containing only the `arg1` field from `t`.

### Example Use Case: Ranges Integration

The `NTuple` design allows for seamless integration with modern C++ ranges, making it ideal for pipeline-based data processing:

```cpp
for (auto&& e : std::views::iota(1, 10)
              | std::views::transform([](int i) {
                  return nt::ntuple(index = i, index_squared = i * i);
                })
              | std::views::filter([](auto&& t) {
                  return t.index % 2 == 0;
                }))
{
  std::cout << e << "\n";
}
```

This will print tuples like:

```
|index: 2 | index_squared: 4|
|index: 4 | index_squared: 16|
|index: 6 | index_squared: 36|
|index: 8 | index_squared: 64|
```



### Defining Fields On the Fly

In addition to predefined field names via `nt_new_field_name`, it is also possible to define new fields **on the fly** using `nt_field(name)`. This is especially useful in functional or pipeline-based code where field names may be temporary or context-specific.

#### Example: Dynamic Field Creation in a Range Pipeline

```cpp
for (auto&& e : std::views::iota(1, 10)
              | std::views::transform([](int i) {
                  return nt::ntuple(
                    index = i,
                    index_squared = i * i,
                    nt_field(cubed) = i * i * i
                  );
                })
              | std::views::filter([](auto&& t) {
                  return t.cubed >= 216;
                }))
{
  std::cout << e << "\n";
}
```

**Output:**

```
|index: 6 | index_squared: 36 | cubed: 216|
|index: 7 | index_squared: 49 | cubed: 343|
|index: 8 | index_squared: 64 | cubed: 512|
|index: 9 | index_squared: 81 | cubed: 729|
```

This approach provides:

* On-the-fly creation of field names in local scope
* No global namespace pollution
* Same strong typing and introspection behavior as predefined fields





## Named Function Arguments

The NTuple library supports **named function arguments** by combining field-name/value syntax with the `bind_args` utility. This allows for expressive and flexible APIs with:

* Optional positional arguments
* Named arguments in any order
* Compile-time validation of accepted argument names
* Default values and required arguments using `std::optional`

### Defining a Function with Named Arguments

```cpp
template <typename... ARGS>
void my_function(ARGS&&... args)
{
  auto t0 = bind_args(
      arg1 = 0,                            // default value
      arg2 = 15,                           // default value
      arg3 = std::optional<int>()         // required argument
  )(args...);

  std::cout << t0 << std::endl;
}
```

Here, `bind_args(...)` returns a callable object that binds incoming arguments (both positional and named) to the fields defined. The call:

```cpp
my_function(4654, arg3 = 234234, arg2 = 150);
```

is interpreted as:

* `arg1 = 4654` (by position — overrides default `0`)
* `arg3 = 234234` (named argument)
* `arg2 = 150` (named argument, order doesn't matter)

### Behavior Summary

* **Positional arguments** fill in left-to-right based on fields not yet provided.
* **Named arguments** override values by field name and can appear in any order.
* **Missing arguments** are filled with defaults from `bind_args(...)`.
* **Fields with `std::optional` defaults** must be explicitly provided. If omitted, a **runtime error** will occur.
* **Unexpected or unknown field names** cause a **compile-time error**.

### Example: Valid and Invalid Calls

```cpp
// ✅ OK: all args provided
my_function(arg1 = 42, arg2 = 99, arg3 = 7);

// ✅ OK: using positional and named args
my_function(1, arg2 = 10, arg3 = 20);

// ✅ OK: only one positional arg and required arg provided
my_function(99, arg3 = 1000);

// ❌ ERROR: unknown argument name
my_function(foo = 12);  // Compile-time error: 'foo' not allowed

// ❌ ERROR: required argument missing (arg3)
my_function(arg1 = 1, arg2 = 2);  // Runtime error: 'arg3' required
```

This mechanism offers a simple, declarative alternative to traditional overloads or `std::tuple` packing, making your function interfaces:

* Self-documenting
* Safer
* Easier to extend

