
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

template <typename FIELD_T, typename VECT_T,  std::size_t Index>
struct member_span
{
  member_span()
  {
  }
  static constexpr std::size_t field_index = Index;
  using M_FIELD_T = FIELD_T;
  using M_VEC_T = VECT_T;

  nt::span<FIELD_T> get()
  {
    auto & m_vec =  reinterpret_cast<VECT_T&>(* (this  - field_index) );
      

      
    return nt::span<FIELD_T>(                   
    m_vec.m_data.size() ? (std::byte *)& (  FIELD_T::get(m_vec.m_data[0])) : nullptr, 
    m_vec.m_data.size(), sizeof(decltype(m_vec.m_data[0])));



  }

  
  auto get_primitive()
  {
    auto & m_vec =  reinterpret_cast<VECT_T&>(* (this  - field_index) );
      
      
      
    return nt::span<decltype(FIELD_T::get(m_vec.m_data[0]).v) >(                   
    m_vec.m_data.size() ? (std::byte *)& (  FIELD_T::get(m_vec.m_data[0])) : nullptr, 
    m_vec.m_data.size(), sizeof(decltype(m_vec.m_data[0])));



  }


  auto operator()(){
    return get();
  }

};

template <typename IndexSeq, typename... Ts>
struct vector_frame_impl;

template <std::size_t... Is, typename... Ts>
struct vector_frame_impl<std::index_sequence<Is...>, Ts...> :

          nt::base_maker_t<
                          nt::_Remove_cvref_t<Ts>, 
                          nt::ax_type2<
                                      member_span<Ts, vector_frame_impl<std::index_sequence<Is...>, Ts...>, Is>,
                                      typename Ts::struct_maker
                                      >
                            >...
{
   std::vector<nt::ntuple<Ts...>> m_data;
  /* data */
  vector_frame_impl(const std::vector<
          nt::ntuple<Ts...>>&  vec) : m_data(vec) {}

    vector_frame_impl() =default;
friend auto begin(vector_frame_impl const& v) { return v.m_data.begin(); }
friend auto end(vector_frame_impl const& v) { return v.m_data.end(); }

friend auto begin(vector_frame_impl & v) { return v.m_data.begin(); }
friend auto end(vector_frame_impl & v) { return v.m_data.end(); }
friend std::ostream &operator<<(std::ostream &os, const vector_frame_impl & v) {
    os << v.m_data;
    return os;
}
  decltype(auto) operator[](std::size_t i){
    return m_data[i];
  }
  decltype(auto) operator[](std::size_t i) const{
    return m_data[i];
  }
};


template <typename... Ts>
using vector_frame = vector_frame_impl<std::index_sequence_for<Ts...>, Ts...>;



template <typename... T> 
auto to_vector_frame(const  std::vector< nt::ntuple< T... > > & vec ){
  return vector_frame<T...>(vec);
}

template <typename... T> 
auto to_vector_frame1(const   nt::ntuple< T... >  & vec ){
  return vector_frame<T...>(std::vector<nt::ntuple< T... >> {});
}


  template <typename FUNC_T>
  auto fill_vector_frame(size_t entries, FUNC_T &&func)
  {
    decltype(to_vector_frame1(func(size_t(0)))) ret;
    ret.m_data.reserve(entries);
    for (size_t i = 0; i < entries; ++i)
    {
      ret.m_data.push_back(func(i));
    }
    return ret;
  }

   template <typename RANGE_T>
  auto fill_vector_frame(RANGE_T && range_)
  {

    decltype(to_vector_frame1( *range_.begin() )) ret;
    //ret.m_data.reserve(entries);
    for (auto&& e : range_)
    {
      ret.m_data.push_back(e);
    }
    return ret;
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



  //auto v = vector_frame<decltype(index = 1), decltype(index_squared = 1)>{df};
  auto v = nt::to_vector_frame(df1);
  std::cout << (uint64_t)&v << std::endl;
  for( auto &&e : v.index.get_primitive() ){

    std::cout << e << std::endl;
  }

  for( auto &&e : v.timestwo.get_primitive() ){

    std::cout << e << std::endl;
  }

  using index_t = decltype(index = 3);


  // auto vec = vector_frame<decltype(df1[0].timestwo), typename decltype(df1[0].timestwo)::template  struct_maker<int> >{};
  auto df2 = fill_vector_frame(10, [](auto i)
                                        { return nt::ntuple(
                                              index = i,
                                              index_squared = i * i); });

            std::cout << df2.m_data << std::endl;          
            
  auto df3 = nt::fill_vector_frame(std::views::iota(1, 10) | std::views::transform([](auto i)
                                                                  { return nt::ntuple(index = i, index_squared = i * i, nt_field(cubed) = i * i * i); }) );

  std::cout << df3[0] << std::endl;          
  for( auto &&e : df3 ){
    e.cubed.v += 2;
    std::cout << e.cubed  << std::endl;
  }
  return 0;
}
