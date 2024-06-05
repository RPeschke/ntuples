#pragma once

#include <type_traits>

#include <iostream>
#include <vector>

#include <iomanip>
#include <algorithm>

#include "ntuples/constexpr_for.hh"


namespace nt
{

  template <class _Ty>
  using _Remove_cvref_t = std::remove_cv_t<std::remove_reference_t<_Ty>>;

  template <int N, typename... Ts>
  using NthTypeOf =
      typename std::tuple_element<N, std::tuple<Ts...>>::type;



  template <typename T1>
  struct type_container
  {
    using type = T1;
  };

  template <typename T1, typename T2>
  struct ax_type : T2
  {
    T1 v = {};
    constexpr ax_type() {}
    constexpr ax_type(T1 t1) : v(std::move(t1)) {}
    constexpr ax_type operator=(T1 &&t1) const { return ax_type(std::move(t1)); }
    constexpr ax_type operator=(const T1 &t1) const { return ax_type(t1); }
    
    template <typename T_IN>
    constexpr ax_type operator()(T_IN &&t1) const { return ax_type(std::move( static_cast<T1>(t1) )); }
    
    template <typename T_IN>
    constexpr ax_type operator()(const T_IN& t1) const { return ax_type(static_cast<T1>(t1)); }
    constexpr ax_type operator=(T1 &&t1)
    {
      v = std::move(t1);
      return *this;
    }
    constexpr ax_type& operator=(const T1 &t1)
    {
      v = t1;
      return *this;
    }

    template <typename T>
    constexpr ax_type& operator=(const ax_type<T1, T>& t1) {
      v = t1.v;
      return *this;
    }

    using struct_maker = T2;
    using data_t = T1;

    operator T1()
    {
      return v;
    }

    operator const T1() const
    {
      return v;
    }

    template <typename T_RHS>
    friend constexpr bool operator<(const ax_type& lhs,  const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v < rhs.v;
    }

    template <typename T_RHS>
    friend constexpr bool operator<=(const ax_type& lhs,  const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v <= rhs.v;
    }

    template <typename T_RHS>
    friend constexpr bool operator==(const ax_type& lhs,  const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v == rhs.v;
    }

    template <typename T_RHS>
    friend constexpr bool operator!=(const ax_type& lhs, const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v != rhs.v;
    }

    template <typename T_RHS>
    friend constexpr bool operator>=(const ax_type& lhs, const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v >= rhs.v;
    }

    template < typename T_RHS>
    friend constexpr bool operator>(const ax_type& lhs, const ax_type<T1, T_RHS>& rhs)
    {
      return lhs.v > rhs.v;
    }




    friend constexpr bool operator<(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v < rhs;
    }
    friend constexpr bool operator<(const T1 lhs, const ax_type&  rhs)
    {
      return lhs < rhs.v;
    }


    friend constexpr bool operator<=(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v <= rhs;
    }

    friend constexpr bool operator<=(const T1& lhs, const  ax_type&   rhs)
    {
      return lhs <= rhs.v;
    }


    friend constexpr bool operator==(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v == rhs;
    }

    friend constexpr bool operator==(const T1& lhs, const  ax_type& rhs)
    {
      return lhs == rhs.v;
    }

    
    friend constexpr bool operator!=(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v != rhs;
    }

    friend constexpr bool operator!=(const T1& lhs, const  ax_type& rhs)
    {
      return lhs != rhs.v;
    }

    
    friend constexpr bool operator>=(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v >= rhs;
    }

    friend constexpr bool operator>=(const T1& lhs, const  ax_type& rhs)
    {
      return lhs >= rhs.v;
    }

    
    friend constexpr bool operator>(const ax_type& lhs, const T1& rhs)
    {
      return lhs.v > rhs;
    }

    friend constexpr bool operator>(const T1& lhs, const  ax_type& rhs)
    {
      return lhs > rhs.v;
    }


    friend std::ostream &operator<<(std::ostream &out, const ax_type &self)
    {
      out << self.get_name() << " : " << self.v;
      return out;
    }

  private:
    using T2::operator=;
  };

  namespace comparators
  {
    template <typename T1, typename... T_rest>
    struct _nt_compare
    {

      template <typename VECA_T, typename VECB_T>
      static auto __isLessthen(const VECA_T &vecA, const VECB_T &vecB)
      {
        if constexpr (sizeof...(T_rest) > 0)
        {
          if (T1::get(vecA) < T1::get(vecB))
          {
            return true;
          }
          else if (T1::get(vecA) > T1::get(vecB))
          {
            return false;
          }
          return _nt_compare<T_rest...>::template __isLessthen(vecA, vecB);
        }
        else
        {
          return T1::get(vecA) < T1::get(vecB);
        }
      }

      template <typename VECA_T, typename VECB_T>
      static auto __isEequal(const VECA_T &vecA, const VECB_T &vecB)
      {
        if constexpr (sizeof...(T_rest) > 0)
        {
          if (nt::_Remove_cvref_t<T1>::get(vecA) != nt::_Remove_cvref_t<T1>::get(vecB))
          {
            return false;
          }
          return _nt_compare<T_rest...>::template __isEequal(vecA, vecB);
        }
        else
        {
          return nt::_Remove_cvref_t<T1>::get(vecA) == nt::_Remove_cvref_t<T1>::get(vecB);
        }
      }
    };

    template <typename... T_rest>
    auto nt_compare(T_rest &&...)
    {
      return _nt_compare<nt::_Remove_cvref_t<T_rest>...>{};
    }

    template <typename... T_rest>
    constexpr auto lessThan(T_rest &&...)
    {
      return [less = _nt_compare<nt::_Remove_cvref_t<T_rest>...>{}](const auto &lhs, const auto &rhs)
      {
        return less.__isLessthen(lhs, rhs);
      };
    }

    template <typename... T_rest>
    constexpr auto lessThan()
    {
      return [less = _nt_compare<nt::_Remove_cvref_t<T_rest>...>{}](const auto &lhs, const auto &rhs)
      {
        return less.__isLessthen(lhs, rhs);
      };
    }

    template <typename... T_rest>
    constexpr auto equal(T_rest &&...)
    {
      return [less = _nt_compare<nt::_Remove_cvref_t<T_rest>...>{}](const auto &lhs, const auto &rhs)
      {
        return less.__isEequal(lhs, rhs);
      };
    }

    template <typename... T_rest>
    constexpr auto equal()
    {
      return [less = _nt_compare<nt::_Remove_cvref_t<T_rest>...>{}](const auto &lhs, const auto &rhs)
      {
        return less.__isEequal(lhs, rhs);
      };
    }
  }

  template <typename T1, typename T2>
  struct ax_type2 : T1, T2
  {

    constexpr ax_type2() = default;

    using struct_maker = T2;
    using data_t = T1;
  };

  struct ax_name_container_base_const
  {
    static constexpr int c_struct_maker = 0;
    static constexpr int c_getter1 = 1;
    static constexpr int c_get_name = 2;
  };

  template <typename T2>
  struct ax_name_container_base : ax_name_container_base_const
  {
    using type = T2;

    constexpr ax_name_container_base() = default;

    template <typename T, int N>
    struct type_wrap
    {
      static constexpr int N_value = N;
      T val;
      using type = T;
    };

    template <typename T>
    static constexpr auto struct_maker()
    {
      return decltype(std::declval<type>()(std::declval<type_wrap<T, c_struct_maker>>())){};
    }

    template <typename Data_T>
    using base_t = typename decltype(struct_maker<Data_T>())::type;

    template <typename T>
    static constexpr decltype(auto) get(T &t)
    {
      using getter1 = decltype(std::declval<type>()(std::declval<type_wrap<T &, c_getter1>>()));
      return getter1::get(t);
    }

    template <typename T>
    static constexpr decltype(auto) get(const T &t)
    {
      using getter1 = decltype(std::declval<type>()(std::declval<type_wrap<const T &, c_getter1>>()));
      return getter1::get(t);
    }
    template <typename T>
    static constexpr decltype(auto) get_value(const T &t)
    {
      return ax_name_container_base::get(t).v;
    }

    template <typename T>
    static constexpr decltype(auto) get_value(T &t)
    {
      return ax_name_container_base::get(t).v;
    }

    static constexpr auto get_name()
    {
      using name_getter = decltype(std::declval<type>()(std::declval<type_wrap<int, c_get_name>>()));
      return name_getter::get_name();
    }
  };

  template <typename TBase>
  struct ax_name_container : TBase
  {
    constexpr ax_name_container() = default;

    template <typename T>
    constexpr decltype(auto) operator()(T &&t) const
    {
      return TBase::get(t);
    }

    template <typename T>
    static constexpr decltype(auto) get_value(T &t)
    {
      return TBase::get(t).v;
    }

    template <typename T>
    static constexpr decltype(auto) get_value(const T &t)
    {
      return TBase::get(t).v;
    }

    template <typename T>
    constexpr auto operator=(T t) const
    {

      return ax_type<_Remove_cvref_t<T>, ax_name_container>{std::move(t)};
    }
  };

  template <typename... Ts>
  ax_name_container(Ts &&...ts) -> ax_name_container<_Remove_cvref_t<Ts>...>;

  template <typename T2>
  auto constexpr get_ax_name_container(const ax_name_container<T2> &t)
  {
    return ax_name_container<T2>{};
  }

  template <typename T, typename data_T>
  struct base_maker
  {
    using type = typename T::template base_t<data_T>;
  };
  template <typename T, typename data_T>
  using base_maker_t = typename base_maker<T, data_T>::type;

  template <typename... T>
  struct ntuple : base_maker_t<_Remove_cvref_t<T>, T>...
  {


    constexpr ntuple() : base_maker_t<_Remove_cvref_t<T>, T>(_Remove_cvref_t<T>{})... {}

    template <typename... Ts>
    constexpr ntuple(Ts &&...t1) : base_maker_t<_Remove_cvref_t<T>, T>(std::forward<Ts>(t1))... {}

    template <typename T2>
    decltype(auto) operator[](const ax_name_container<T2> &t)
    {
      return ax_name_container<T2>::get(*this);
    }

    friend std::ostream &operator<<(std::ostream &out, const ntuple &self)
    {
      out << "|";

      constexpr_for<0, sizeof...(T), 1>([&](auto i)
                                        {
        using  current_t = decltype(NthTypeOf<i, _Remove_cvref_t<T>...>{});
        out << " ";
        out << current_t::get(self);
        out << " |"; });

      return out;
    }

    template <typename... ARGS>
    auto operator|(const ntuple<ARGS...> &rhs) const
    {
      return ntuple<T..., ARGS...>(
          T::get(*this)...,
          ARGS::get(rhs)...);
    }


    friend constexpr bool operator<(const ntuple& lhs, const ntuple& rhs) {
      static constexpr auto lt =  comparators::lessThan<T...>();
      return lt(lhs, rhs);
    }
    inline static constexpr std::size_t __size__ = sizeof...(T);
  };

  template <typename... Ts>
  ntuple(Ts &&...ts) -> ntuple<_Remove_cvref_t<Ts>...>;

  template <int N, typename... ARGS>
  constexpr decltype(auto) get_nth(const nt::ntuple<ARGS...> &nt)
  {
    return nt::_Remove_cvref_t<nt::NthTypeOf<N, nt::_Remove_cvref_t<ARGS>...>>::get(nt);
  }

  template <int N, typename... ARGS>
  constexpr decltype(auto) get_nth(nt::ntuple<ARGS...> &nt)
  {
    return nt::_Remove_cvref_t<nt::NthTypeOf<N, nt::_Remove_cvref_t<ARGS>...>>::get(nt);
  }

  // Primary template for contains_type; defaults to false
  template <typename T, typename Ntuple>
  struct contains_type : std::false_type
  {
  };

  // Specialization for ntuple
  template <typename T, typename... ARGS>
  struct contains_type<T, ntuple<ARGS...>> : std::disjunction<std::is_same<T, ARGS>...>
  {
  };

  // Helper variable template
  template <typename T, typename Ntuple>
  constexpr bool contains_type_v = contains_type<T, Ntuple>::value;

}

