
#include "ntuples/ntuples.hh"
#include "ntuples/dataframe.hh"
#include "ntuples/generic_algorithms.hh"
#include "ntuples/macro_nt_field.hh"
#include "ntuples/macro_comperator.hh"
#include "ntuples/macro_groupby.hh"
#include "ntuples/macro_nt_new_field.hh"
#include "ntuples/comparators.hh"
#include "ntuples/span.hh"

#include "ntuples/vector_frame.hh"

#include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>

#include <iostream>
#include <ranges>
#include <algorithm>
#include <numeric> // for std::iota
#include <optional>

#include "ntuples/std_adapter.hh"

#include "ntuples/bind_args.hh"

using namespace nt::algorithms;

nt_field_c(subdetector, int());
nt_field_c(subdetector1, int());
nt_new_field(test1, int());
nt_new_field_t(sector, int{});
nt_new_field_t(sector1, int{});

nt_new_field_name(sector2);
nt_new_field_name(index);
nt_new_field_name(index_squared);

std::ostream &operator<<(std::ostream &os, const std::optional<int> &opt)
{
  if (opt.has_value())
  {
    os << *opt;
  }
  else
  {
    os << "<nullopt>";
  }
  return os;
}

template <typename... ARGS>
void my_function(ARGS &&...args)
{
  auto t0 = bind_args(
      subdetector = 0,
      sector(15),
      sector2 = std::optional<int>())(
      args...);

  std::cout << t0 << std::endl;
}

template <typename T>
void fun(nt::span<T> sp)
{
  for (auto &&e : sp)
  {
    std::cout << e << std::endl;
  }
}

int main(int argv, char **argc)
{

  auto t = nt::ntuple();
  std::cout << t << std::endl;

  try
  {
    my_function();
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  my_function(4654, sector2 = 234234, sector(150));

  for (auto &&e : std::views::iota(1, 10) | std::views::transform([](auto i)
                                                                  { return nt::ntuple(index = i, index_squared = i * i, nt_field(cubed) = i * i * i); }) |
                      std::views::filter([](auto &&t)
                                         { return t.cubed >= 216; }))
  {
    std::cout << e << std::endl; // requires operator<< for nt::ntuple
  }

  auto df = nt::algorithms::fill_vector(10, [](auto i)
                                        { return nt::ntuple(
                                              index = i,
                                              index_squared = i * i); });

  std::cout << df << std::endl;

  auto sp = nt_span(df, index);

  std::cout << std::count_if(sp.begin(), sp.end(), [](auto &&e)
                             { return e >= 5; })
            << std::endl;

  for (auto &&e : sp | std::views::filter([](auto &&t)
                                          { return t >= 5; }))
  {
    std::cout << e << std::endl;
  }

  auto df1 = add_column(df, [](auto &&e)
                        { return nt::ntuple(nt_field(timestwo) = e.index * 2.12); });
  std::cout << df1;

  // auto v = vector_frame<decltype(index = 1), decltype(index_squared = 1)>{df};
  auto v = nt::to_vector_frame(df1);
  std::cout << (uint64_t)&v << std::endl;
  for (auto &&e : v.index.get_primitive())
  {

    std::cout << e << std::endl;
  }

  for (auto &&e : v.timestwo.get_primitive())
  {

    std::cout << e << std::endl;
  }

  using index_t = decltype(index = 3);

  // auto vec = vector_frame<decltype(df1[0].timestwo), typename decltype(df1[0].timestwo)::template  struct_maker<int> >{};
  auto df2 = nt::fill_vector_frame(10, [](auto i)
                                   { return nt::ntuple(
                                         index = i,
                                         index_squared = i * i); });

  std::cout << df2 << std::endl;

  auto df3 = nt::fill_vector_frame(std::views::iota(1, 10) 
                                      | std::views::transform([](auto i) { 
                                        return nt::ntuple(index = i, index_squared = i * i, nt_field(cubed) = i * i * i); 
                                      })
                                  );

  std::cout << df3[0] << std::endl;
  for (auto &&e : df3)
  {
    e.cubed.v += 2;
    std::cout << e.cubed << std::endl;
  }
  return 0;
}
