
#include "ntuples/ntuples.hh"
#include "ntuples/dataframe.hh"
#include "ntuples/generic_algorithms.hh"
#include "ntuples/macro_ax_maker.hh"
#include "ntuples/macro_comperator.hh"
#include "ntuples/macro_groupby.hh"
#include "ntuples/macro_nt_new_axis.hh"
#include "ntuples/comparators.hh"



  #include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>
  

using namespace nt::algorithms;

  ax_new_axis(subdetector, int());
  nt_new_axis_c(test1 , 123);
  

  

//  nt_new_axis_t(sector , int() );
  //namespace __nt{       __nt_new_axis_core( sector); static const  auto s = nt::ax_name_container<ztsector>{} = int();       } 
  //using sector = decltype( __nt::s );
  nt_new_axis_t(sector , int{});
  nt_new_axis_c(sector1, std::string());

  using ex1 = decltype( sector1 );

  nt_new_name(axis);
  nt_new_name_t(plane);


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


int main(int argv, char** argc) {


  auto df11312 = fill_vector(10, [](int i){

    return nt::ntuple{
      subdetector = i/3 ,
      sector{i/2},
      ax_maker(ex1) = 546
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
      ax_maker(ex1) = 546
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
      ax_maker(ex1) = 546
    };
    }
    );

 
  std::cout << (test1 = 123211) << std::endl;
  std::cout << axis(klm_nameing[0]) << std::endl;

  klm_nameing.emplace_back(
      subdetector = 456 ,
      sector{123},
      sector1 = std::string("asd"),
      axis = "4655",
      plane() = 1231,
      ax_maker(ex1) = 546
  );
  auto x = ax_maker(ex1) = 456;
  const auto x1 = klm_nameing[0];

  std::cout << x1 << std::endl;
  constexpr auto g = get_fun([](auto&& e) -> decltype(auto) {
    return e.ex1;
  });
  
  g(klm_nameing[0]) =123;
  std::cout << x1 << std::endl;
  //klm_nameing[0].subdetector 



  auto x123123 = ax_maker(ex6) = std::string("asd");
  //x123123 = "asdaswqe";

  auto x123 = ax_maker(ex6) = 21321;


  auto df   = nt::dataframe<decltype(x123)>();
  df.ex6.push_back(123);

  auto x12 = x123.get(df);


  auto nt3 = nt::ntuple(x123,
    ax_maker(ex7) = 64
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
      ax_maker(ex1) = i,
      ax_maker(ex2) = i * i,
      ax_maker(ex3) = i * i * i
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
