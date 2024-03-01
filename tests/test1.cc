
#include "ntuples/ntuples.hh"
  #include <algorithm>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <vector>
  

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



    template <typename FUNC_T>
    auto fill_vector(size_t entries, FUNC_T&& func)
    {
      std::vector< decltype(func(size_t(0))) > ret ;
      ret.reserve(entries);
      for (size_t i = 0; i < entries; ++i) {
        ret.push_back(func(i));
      }
      return ret;
    }

    struct identity {
      template<typename T>
      constexpr T&& operator()(T&& t) const noexcept
      {
        return (T&&)t;
      }
    };

    template <typename T1, typename T2, typename Prj_T = identity>
    struct __range__impl_prj  {
      __range__impl_prj(T1&& b, T2&& e, Prj_T&&  prj = identity{}) : m_begin(std::move(b), prj), m_end(std::move(e), prj) {}
      __range__impl_prj(const T1& b, const T2& e, Prj_T&&  prj = identity{}) : m_begin(b, prj), m_end(e, prj) {}
      auto begin() const
      {
        return m_begin;
      }
      auto back() const
      {
        return m_end.m_prj(*(m_end.get_base() - 1));
      }
      auto front() const
      {
        return *(m_begin);
      }
      auto end() const
      {
        return m_end;
      }
      auto operator[](size_t i) const
      {
        return m_begin.m_prj(*(m_begin.get_base() + i));
      }

      size_t size() const
      {
        return  m_end - m_begin;
      }
      struct begin_t : T1 {
        begin_t(T1&& t1, Prj_T& pro) : T1(std::move(t1)), m_prj(pro) {}
        begin_t(const T1& t1, Prj_T& pro) : T1(t1), m_prj(pro) {}
        Prj_T m_prj;
        auto operator*() const
        {
          return m_prj(**((const T1*)this));
        }
        T1 get_base() const
        {
          return *((const T1*)this);
        }
      } m_begin, m_end;


    };

    template <typename T1, typename T2>
    auto __range__(T1&& b, T2&& e)
    {
      return __range__impl_prj< nt:: _Remove_cvref_t<T1>, nt:: _Remove_cvref_t<T2> >(std::forward<T1>(b), std::forward<T2>(e), {});
    }
    template <typename T1, typename T2, typename PRJ>
    auto __range__(T1&& b, T2&& e, PRJ&& prj)
    {
      return __range__impl_prj< nt::_Remove_cvref_t<T1>, nt::_Remove_cvref_t<T2>, nt::_Remove_cvref_t<PRJ> >(std::forward<T1>(b), std::forward<T2>(e),
             std::forward<PRJ>(prj));
    }




    template <typename... T>
    struct __group {
      template <typename VEC_T, typename FUNC_T>
      static auto apply_append(const std::vector<VEC_T>& vec, FUNC_T&& fun){
        auto fun_c = [&](const auto& tail , const auto& head){
          auto rng = __range__(tail, head);
          return  nt::ntuple( T::get(*tail)...) | fun(rng);
        };
        return __apply__internal__( vec, fun_c );
      }

      template <typename VEC_T, typename FUNC_T>
      static auto apply(const std::vector<VEC_T>& vec, FUNC_T&& fun){
        auto fun_c = [&](const auto& tail , const auto& head){
          auto rng = __range__(tail, head);
          return  fun(rng);
        };
        return __apply__internal__( vec, fun_c );
      }

private:      
      template <typename VEC_T>
      static auto __assert_is_sorted__(const std::vector<VEC_T>& vec){
        constexpr auto lt =  nt::comparators::lessThan<T...>() ;
        auto is_sorted_ = std::is_sorted(std::begin(vec), std::end(vec), lt );

        if (!is_sorted_) {
          throw std::runtime_error("group::apply expects the input vector to be sorted according to the group parameters\nthe input vector was not sorted with respect to the group axis");
        }
      }

      template <typename VEC_T, typename FUNC_T>
      static auto create_empty_vector(){
       return std::vector <
        decltype(
          std::declval<FUNC_T>()(
            std::begin(  std::declval<const std::vector<VEC_T>&>() ),  
            std::end(    std::declval<const std::vector<VEC_T>&>() )
            )
        )
        > {};

         
      }


      template <typename VEC_T, typename FUNC_T>
      static auto __apply__internal__(const std::vector<VEC_T>& vec, FUNC_T&& fun_c)
      {
        __assert_is_sorted__(vec);
        auto ret = create_empty_vector<VEC_T,FUNC_T>();


        if (vec.empty()) {
          return ret;
        }
        
        auto tail = std::begin(vec);
        constexpr auto eq = nt::comparators::equal<T...>() ;
        for (auto head = std::begin(vec); head != std::end(vec); ++head) {
          if (! eq(*tail, *head )) {

     


            ret.push_back(
                      fun_c(tail ,head)
                    );
            tail = head;
          }
        }

        ret.push_back(
                    fun_c(tail , std::end(vec))
                    ) ;


        return ret;
      }

    };

template <typename ... ARGS>
auto group(ARGS...){
  return __group<nt::_Remove_cvref_t<ARGS>...>{};
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
