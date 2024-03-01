#pragma once
#include "ntuples/ntuples.hh"


namespace nt::algorithms {

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
}