
#include "ntuples/ntuples.hh"
#include "ntuples/dataframe.hh"
#include "ntuples/generic_algorithms.hh"
#include "ntuples/macro_nt_field.hh"
#include "ntuples/macro_comperator.hh"
#include "ntuples/macro_groupby.hh"
#include "ntuples/macro_nt_new_field.hh"
#include "ntuples/comparators.hh"



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

#include "ntuples/std_adapter.hh"

#include "ntuples/bind_args.hh"


using namespace nt::algorithms;

  ax_field_c(subdetector, int());
  ax_field_c(subdetector1, int());
  nt_new_field_c(test1 , int());
    nt_new_field_t(sector , int{});
    nt_new_field_t(sector1 , int{});

template <typename T>
struct is_ax_name_container {
private:
  template <typename U>
  static std::true_type test(const nt::ax_name_container<U>*);

  static std::false_type test(...);

public:
  static constexpr bool value = decltype(test(std::declval<std::remove_cvref_t<T>*>()))::value;
};

template <typename T>
constexpr bool is_ax_name_container_v = is_ax_name_container<T>::value;

template <typename T, typename K>
constexpr bool is_assignable_via_brackets_v = requires(T t, K k) {
    { t[k] = k };
};

template <typename T, typename K>
constexpr void enforce_assignable() {
  static_assert(is_bracket_assignable<T, K>(),
    "\n\n[NTUPLE ERROR] Cannot assign using t[t1] = t1.\n"
    "Possible reasons:\n"
    "- The key type t1 is not valid for the ntuple\n"
    "- operator[] is not defined for the given key\n"
    "- The assignment type is invalid\n"
    "\nConsider checking your ntuple contents and ax_name_container bindings.\n"
  );
}

template <size_t N, typename NT_T, typename T1, typename... ARGS>
auto update1(NT_T&& t, T1&& t1, ARGS&&... args){
  if constexpr (is_ax_name_container_v<T1>){
    if constexpr ( t1.index_of<NT_T>() < N){
      static_assert(dependent_false<NT_T>::value,  "[NTUPLE ERROR] Named argument appears after positional arguments. "
                           "All named arguments must follow positional ones.");
    }else {
      t[t1] = t1;
    

    }
      

  }

  if constexpr (sizeof...(args) == 0){
    return t;
  } else {
    return update1<N>(t, std::forward<ARGS>(args)...);
  }
}

template <size_t N, typename NT_T, typename T1, typename... ARGS>
auto update0(NT_T&& t, T1&& t1, ARGS&&... args){
 if constexpr (is_ax_name_container_v<T1>){
  return update1<N>(t, std::forward<T1>(t1),  std::forward<ARGS>(args)...);
 }else{
  nt::get_nth<N>(t) = t1;
  return update0<N+1>(t,  std::forward<ARGS>(args)...);
 }
}


    auto l = [](auto& e) -> decltype(auto) {
        
      if constexpr (requires { e.subdetector; }) {
            return (e.subdetector); // keep parens to preserve ref category
        } else {
            static_assert(dependent_false<decltype(e)>::value,
                "\n[NTUPLE ERROR] Cannot access field `subdetector1` — it doesn't exist in this ntuple.\n"
        );
        }
    };


    template <typename... NamedArgs>
auto update00(NamedArgs&&... named_args) {
  // Return a lambda that captures named_args
  return [&](auto&&... args) {
    auto tup = nt::ntuple(std::forward<NamedArgs>(named_args)...);
    if constexpr(sizeof...(args) == 0){
      return tup;
    }else {
      return update0<0>(std::move(tup), std::forward<decltype(args)>(args)...);
    }
  };
}

    template<typename T>
struct has_field_t {
  static constexpr bool has_field1() {
    if constexpr (requires { typename T::subdetector; }) {
      return true;
    } else {
      return false;
    }
  }
};

template <typename... ARGS>
std::false_type has_field(ARGS&&...) {
  return std::false_type{};
}

template <typename T1 , typename T2>
auto  has_field(T1&& t1, T2&& t2) ->  decltype(t1[t2] ,   std::true_type{}) {
  return std::true_type{};
}

    template <typename... ARGS>
    void my_function(ARGS&&... args){
    //auto t0 = nt::ntuple( std::forward<ARGS>(args)...);
      auto t0 = bind_args(
   
          subdetector = 0,
          sector(15)
        )(
        args...
      );
     //constexpr  auto t11 =  has_field_t<decltype(t0)>::has_field1();
     std::cout<< t0<< std::endl;
      //constexpr  auto t1 = decltype(subdetector)::has_field(t0);

     // auto x= decltype(t0[subdetector1] ,   std::true_type{}){};
       constexpr  auto t1 = t0.contains_struct_maker_type< decltype(subdetector1)::struct_maker>() ;

      constexpr  auto t2 =  decltype(subdetector)::has_field<decltype(t0)>();
      constexpr  auto t3 =  decltype(subdetector1)::has_field<decltype(t0)>();

      sector::get(t0) = 213;
           std::cout<< t0<< std::endl;
      l(t0);



    }


int main(int argv, char** argc) {

 auto t = nt::ntuple();
 std::cout << t << std::endl;



  my_function(4654, sector(150)  );


  return 0;
}
