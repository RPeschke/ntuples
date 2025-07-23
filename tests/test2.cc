
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




using namespace nt::algorithms;

  nt_field_c(subdetector, int());
  nt_field_c(subdetector1, int());
  nt_new_field_c(test1 , int());
  

  

//  nt_new_field_t(sector , int() );
  //namespace __nt{       __nt_new_field_core( sector); static const  auto s = nt::ax_name_container<ztsector>{} = int();       } 
  //using sector = decltype( __nt::s );
  nt_new_field_t(sector , int{});
  nt_new_field_c(sector1, std::string());

  using ex1 = decltype( sector1 );

  nt_new_field_name(axis);
  nt_new_field_name_t(plane);


template <typename T>
struct get_fun{
  
  mutable T m_t;
  constexpr get_fun(T&& t) : m_t(std::move(t)){}

  template <typename ARGS>
  decltype(auto) operator()(ARGS&& args) const {

    return m_t(std::forward<ARGS>(args) );
  }
};


template <int N, typename... ARGS>
constexpr auto get_nth_type( const nt::ntuple<ARGS...>& )
    {

      return nt::_Remove_cvref_t<nt::NthTypeOf<N, nt::_Remove_cvref_t<ARGS>...>>();
    }


    template <typename T0, typename T1, typename T2, typename Comparision_T, typename projecttion_t >
    void vec_join_r(T0& ret, const T1& t1, const T2& t2, Comparision_T comp, projecttion_t project)
    {
      ret.clear();
      for (const auto& e1 : t1) {
        for (const auto& e2 : t2) {
          if (comp(e1, e2)) {
            ret.push_back(project(e1, e2));
          }
        }
      }

    }

    template <typename T1, typename T2, typename Comparision_T, typename projecttion_t >
    auto vec_join(const T1& t1, const T2& t2, Comparision_T comp, projecttion_t project)
    {
      std::vector<  decltype(project(t1[0], t2[0]))  > ret;
      vec_join_r(ret, t1, t2, comp, project);
      return ret;
    }


    template <typename... Tags, typename... Fields>
  auto keep(const nt::ntuple<Fields...>& t) {
      return nt::ntuple(Tags::get(t)...);
  }

  namespace detail {

// Try to call T0::get(T1) and detect if it's valid
template <typename T0, typename T1>
using try_get_t = decltype(T0::get(std::declval<T1>()));

template <typename T0, typename T1>
constexpr bool has_get_v = requires { typename std::type_identity_t<try_get_t<T0, T1>>; };

} // namespace detail

template <typename T0, typename T1, typename T2>
decltype(auto) get(T1&& t1, T2&& fallback) {
    if constexpr (detail::has_get_v<T0, T1>) {
        return T0::get(std::forward<T1>(t1));
    } else {
        return std::forward<T2>(fallback);
    }
}

template < typename T1, typename T2>
decltype(auto) get2(T1&& t1, T2&& fallback) {
    if constexpr (detail::has_get_v<T2, T1>) {
        return T2::get(std::forward<T1>(t1));
    } else {
        return std::forward<T2>(fallback);
    }
}

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
    if constexpr (is_assignable_via_brackets_v<NT_T, T1>) {
      t[t1] = t1;
    } else {
      enforce_assignable<NT_T, T1>();
    }
  }

  if constexpr (sizeof...(args) == 0){
    return t;
  } else {
    return update1<N+1>(t, std::forward<ARGS>(args)...);
  }
}

    auto l = [](auto& e) -> auto& {
        
      if constexpr (requires { e.subdetector; }) {
            return (e.subdetector); // keep parens to preserve ref category
        } else {
            static_assert(dependent_false<e>,
                "\n[NTUPLE ERROR] Cannot access field `subdetector` — it doesn't exist in this ntuple.\n"
                "→ Make sure you're using a valid tag for the ntuple structure.\n"
                "→ Check for typos or incorrect tag types.\n");
        }
    };

    template <typename... ARGS>
    void my_function(ARGS&&... args){
      auto t0 = update1<0>(
        nt::ntuple(
          subdetector = 0,
          sector(15)
        ),
        std::forward<ARGS>(args)...
      );
      std::cout << t0 << std::endl;
      l(t0);
      auto t1 = nt::ntuple(std::forward<ARGS>(args)...);
      std::cout << t1 << std::endl;
      auto def = nt::ntuple(sector(1), subdetector=2);
      auto k = get2(t1, subdetector = 123);
      std::cout << k << std::endl;
      //auto t2 = update(def, t1);


    }


int main(int argv, char** argc) {

 auto t = nt::ntuple();
 std::cout << t << std::endl;



  my_function(subdetector = 1  );


    auto result = 
          std::ranges::iota_view{0}
        | std::ranges::views::filter([](int n) { 
            return n % 2 == 0;
          })  // Filter even numbers
        | std::ranges::views::transform([](int n) { 
            return nt::ntuple{
                nt_field(member1) = n
              };

          })   
        | std::ranges::views::transform([](auto&& nt1 ) {
            return nt1 | nt::ntuple{              
                nt_field(n2) =  nt1.member1 * nt1.member1,
                nt_field(n3) =  nt1.member1 * nt1.member1 * nt1.member1,
                test1 = 1312
              };
        })
        | std::ranges::views::take(10);
        
    // Print the results
    for (auto&& n : result  ) {
        
        std::cout << nt::get_nth<1>(n) << std::endl;
        std::cout << n << std::endl;

        std::cout << nt::get_nth<1>(n).v << std::endl;
    }
    std::cout << std::endl;






























    


  auto df11312 = fill_vector(10, [](int i){

    return nt::ntuple{
      subdetector = i/3 ,
      sector{i/2},
      nt_field(ex1) = 546
    };

  });

  for (auto& e : df11312){
    std::cout << e << std::endl;
  }
  

  auto df2132321 = group(subdetector, sector{}).apply_append( df11312 , [] (const auto& x) {

    return nt::ntuple{
      
      plane() = 1231
    };

  });

  for (auto& e : df2132321){
    std::cout << e << std::endl;
  }
  
  auto n1 = nt::ntuple{
      sector{123},
      sector1 = std::string("asd")
    };
  auto n22 = nt::ntuple{
      sector{124},
      sector1 = std::string("asd")
    };

  std::cout << n1 <<std::endl;
  nt::comparators::on_common_args(n1, n1);
  auto x12312 = nt::get_nth<0>(n1) =3;
  std::cout << n1 <<std::endl;

  auto n2 = nt::ntuple{

      axis = "4655",
      plane() = 1231,
      nt_field(ex1) = 546
  };

  auto nt321424 = n1 | n2 ;
  std::cout << nt321424 << std::endl;
  auto klm_nameing  = nt::fill_dataframe(2 , [](int i) {

    return nt::ntuple{
      subdetector = i ,
      sector{123},
      sector1 = std::string("asd"),
      axis = "4655",
      plane() = 1231,
      nt_field(ex1) = 546
    };
    }
    );

 auto nt1 = nt::ntuple{
      subdetector = 1  ,
      sector{123},
      sector1 = std::string("asd"),
      axis = "4655",
      plane() = 1231,
      nt_field(ex1) = 546
    };
  //std::cout << nt::get<0>(nt1) << std::endl;

  /*
  auto [xx1 , xx2] = [&](){
    return nt::ntuple{
      subdetector = 1  ,
      sector{123}
    };
  }();
  std::cout<<xx1<< ",   " << xx2 <<'\n';
*/
auto temp = nt::ntuple{ subdetector = 1, sector{123} };
auto [xx1, xx2] = temp;
std::cout<<xx1<< ",   " << xx2 <<'\n';

  std::cout << (test1 = 123211) << std::endl;
  std::cout << axis(klm_nameing[0]) << std::endl;


  klm_nameing.emplace_back(
      subdetector = 456 ,
      sector{123},
      sector1 = std::string("asd"),
      axis = "4655",
      plane() = 1231,
      nt_field(ex1) = 546
  );
  auto x = nt_field(ex1) = 456;
  const auto x1 = klm_nameing[0];

  std::cout << x1 << std::endl;
  constexpr auto g = get_fun([](auto&& e) -> decltype(auto) {
    return e.ex1;
  });
  
  g(klm_nameing[0]) =123;
  std::cout << x1 << std::endl;
  //klm_nameing[0].subdetector 



  auto x123123 = nt_field(ex6) = std::string("asd");
  //x123123 = "asdaswqe";

  auto x123 = nt_field(ex6) = 21321;


  auto df   = nt::dataframe<decltype(x123)>();
  df.ex6.push_back(123);

  auto x12 = x123.get(df);


  auto nt3 = nt::ntuple(x123,
    nt_field(ex7) = 64
  );
  std::cout << nt3 << std::endl;
  std::cout << nt3[df.ex6] << std::endl;
  auto nt4 = nt::ntuple<decltype(nt3.ex7)&>(
    nt3.ex7
  );
  decltype(nt3.ex6)& ref = nt3.ex6;

  nt4.ex7 = 87;
  std::cout << nt4 << std::endl;
  std::cout << nt3[nt3.ex6] << std::endl;
  nt3[nt3.ex6] = 464;
  std::cout << nt3[nt3.ex6] << std::endl;
  [](const auto& e) {
    std::cout << e << std::endl;
  }(nt3);


  auto df1 = nt::fill_dataframe(100, [](int i) {
    return nt::ntuple(
      nt_field(ex1) = i,
      nt_field(ex2) = i * i,
      nt_field(ex3) = i * i * i
    );
    });




  std::cout << "\n\n" << std::endl;
  std::cout << df1 << std::endl;
  std::cout << "\n\n" << std::endl;
  std::cout << df1[5] << std::endl;
  df1[5].ex1 = 123213;
  df1.ex2[5] = 321;
  std::cout << df1[df1.ex2][5] << std::endl;
  

  std::cout << df1[4] << std::endl;
  df1.ex2[4] = 5464;
  std::cout << df1[4] << std::endl;
  df1[2].ex1 = 12321;
  std::cout << df1[2] << std::endl;

  return 0;
}
