
#include "ntuples/ntuples.hh"
#include "ntuples/dataframe.hh"
#include "ntuples/generic_algorithms.hh"
#include "ntuples/macro_nt_field.hh"
#include "ntuples/macro_comperator.hh"
#include "ntuples/macro_groupby.hh"
#include "ntuples/macro_nt_new_field.hh"
#include "ntuples/comparators.hh"
#include "ntuples/span.hh"
#include "ntuples/nt_erased.hh"

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
#include "ntuples/nt_vector_erased.hh"

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
void fun(nt::span<const T> sp)
{
  for (auto &&e : sp)
  {
    std::cout << e << std::endl;
  }
}

void fun3(nt::span<const int> sp)
{
  for (auto &&e : sp)
  {
    std::cout << e << std::endl;
  }
}

void fun2(nt::nt_erased<int> te)
{

  for (int i = 0; i < te.size(); ++i)
  {
    std::cout << te.get(i) << std::endl;
  }
}

int main(int argv, char **argc)
{

  const int int1{};
  auto s = nt::span<const int>(&argv, 1, 1);

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

  auto df = nt::algorithms::fill_vector(10, [](int i)
                                        { return nt::ntuple(
                                              index = i,
                                              index_squared = i * i); });

  std::cout << df << std::endl;

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

  auto df3 = nt::fill_vector_frame(std::views::iota(1, 10) | std::views::transform([](auto i)
                                                                                   { return nt::ntuple(index = i, index_squared = i * i, nt_field(cubed) = i * i * i); }));

  std::cout << df3[0] << std::endl;
  for (auto &&e : df3)
  {
    e.cubed.v += 2;
    std::cout << e.cubed << std::endl;
  }

  auto t2 = nt::ntuple(index = 2, index_squared = 4, nt_field(cubed) = std::string(""));

  auto te = nt::nt_erased(t2);
  std::cout << te.get<int>(0) << std::endl;
  std::cout << te.get<int>(1) << std::endl;

  std::cout << te.get<int>("index") << std::endl;

  try
  {
    fun2(t2);
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  auto ve = [](const auto &e)
  {
    return nt::vector_erased(e);
  }(df);

  std::cout << ve.columns() << std::endl;

  try
  {
    ve.get<const double>(0);
    fun3(ve.get<int>(0));
  }
  catch (const std::runtime_error &e)
  {
    std::cerr << "Error: " << e.what() << std::endl;
  }

  for (auto &&e : ve.get<const int>(1))
  {
    std::cout << e << std::endl;
  }

  for (int i = 0; i < ve.columns(); ++i)
  {
    std::cout << ve.column_name(i) << std::endl;
  }
  return 0;
}
