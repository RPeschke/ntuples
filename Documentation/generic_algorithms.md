# `nt::algorithms` – Generic Algorithms for the NTuple Library

This header provides a collection of simple, reusable algorithms for working with vectors and `ntuple` containers in the `ntuple` library. These utilities are mainly proof-of-concept and prioritize clarity and functionality over performance.

## Overview

Namespace: `nt::algorithms`
Header: `#include <ntuples/generic_algorithms.hh>`

---
Perfect. Here’s a complete and clean documentation entry for `add_column`, including your example and the generated output.

---

## `add_column`

The add_column function extends each ntuple in a vector by applying a user-provided transformation and appending the result as additional fields. It enables functional-style augmentation of dataframes by chaining calculated columns without modifying the original data.


```cpp
template <typename VEC, typename FUNC_T>
auto add_column(const VEC &vec, FUNC_T &&func);
```

Adds an additional column to a vector of ntuples by applying a transformation to each element and concatenating the result via the `|` operator. The function should return a new ntuple that will be merged with each row.

### Parameters

* `vec`: A vector of ntuples.
* `func`: A function that takes one ntuple element from `vec` and returns a new ntuple (e.g., computed fields).

### Returns

A new `std::vector` containing ntuples with the additional field(s) appended.

---

### Example

```cpp
auto df = nt::algorithms::fill_vector(10, [](auto i) {
  return nt::ntuple(
      index = i,
      index_squared = i * i);
});

auto df1 = add_column(df, [](auto&& e) {
  return nt::ntuple(nt_field(timestwo) = e.index * 2);
});

std::cout << df1;
```

**Output:**

```
| index | index_squared | timestwo |
|----|----|----|
| 0 | 0 | 0 |
| 1 | 1 | 2 |
| 2 | 4 | 4 |
| 3 | 9 | 6 |
| 4 | 16 | 8 |
| 5 | 25 | 10 |
| 6 | 36 | 12 |
| 7 | 49 | 14 |
| 8 | 64 | 16 |
| 9 | 81 | 18 |
```

