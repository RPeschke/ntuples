#pragma once

#include <type_traits>

#include <iostream>
#include <vector>

#include <iomanip>
#include <algorithm>

namespace nt
{

  template <class _Ty>
  using _Remove_cvref_t = std::remove_cv_t<std::remove_reference_t<_Ty>>;

  template <int N, typename... Ts>
  using NthTypeOf =
      typename std::tuple_element<N, std::tuple<Ts...>>::type;

  template <auto Start, auto End, auto Inc, class F>
  constexpr void constexpr_for(F &&f)
  {
    if constexpr (Start < End)
    {
      f(std::integral_constant<decltype(Start), Start>());
      constexpr_for<Start + Inc, End, Inc>(f);
    }
  }

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
    constexpr ax_type operator=(const T1 &t1) const { return ax_type(std::move(t1)); }
    constexpr ax_type operator()(T1 &&t1) const { return ax_type(std::move(t1)); }
    constexpr ax_type operator()(const T1 &&t1) const { return ax_type(t1); }
    constexpr ax_type operator=(T1 &&t1)
    {
      v = std::move(t1);
      return *this;
    }
    constexpr ax_type operator=(const T1 &t1)
    {
      v = t1;
      return *this;
    }
    using struct_maker = T2;
    using data_t = T1;

    operator T1()
    {
      return v;
    }

    friend constexpr bool operator<(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v < rhs.v;
    }

    friend constexpr bool operator<=(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v <= rhs.v;
    }

    friend constexpr bool operator==(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v == rhs.v;
    }

    friend constexpr bool operator!=(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v != rhs.v;
    }

    friend constexpr bool operator>=(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v >= rhs.v;
    }

    friend constexpr bool operator>(const ax_type &lhs, const ax_type &rhs)
    {
      return lhs.v > rhs.v;
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

    template <typename... Ts>
    ntuple(Ts &&...t1) : base_maker_t<_Remove_cvref_t<T>, T>(std::forward<Ts>(t1))... {}

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

#define ax_maker(name_) []() constexpr {                                          \
  auto struct_maker_template_lambda = [](auto e) constexpr {                      \
    using ARG_T = decltype(e);                                                    \
    if constexpr (e.N_value == nt::ax_name_container_base_const::c_struct_maker)  \
    {                                                                             \
      if constexpr (!std::is_reference_v<typename ARG_T::type>)                   \
      {                                                                           \
        struct Zt##name_                                                          \
        {                                                                         \
          Zt##name_() {}                                                          \
          Zt##name_(const decltype(e.val) &e_) : name_(e_) {}                     \
          Zt##name_(decltype(e.val) &e_) : name_(e_) {}                           \
          Zt##name_(decltype(e.val) &&e_) : name_(std::move(e_)) {}               \
          decltype(e.val) name_;                                                  \
          decltype(e.val) value() const                                           \
          {                                                                       \
            return name_;                                                         \
          }                                                                       \
        };                                                                        \
        return nt::type_container<Zt##name_>{};                                   \
      }                                                                           \
      else                                                                        \
      {                                                                           \
        struct Zt##name_                                                          \
        {                                                                         \
          Zt##name_(decltype(e.val) e_) : name_(e_) {}                            \
          decltype(e.val) name_;                                                  \
          decltype(e.val) value() const                                           \
          {                                                                       \
            return name_;                                                         \
          }                                                                       \
        };                                                                        \
        return nt::type_container<Zt##name_>{};                                   \
      }                                                                           \
    }                                                                             \
    else if constexpr (e.N_value == nt::ax_name_container_base_const::c_getter1)  \
    {                                                                             \
      struct getter_t                                                             \
      {                                                                           \
        static constexpr auto &get(decltype(e.val) x)                             \
        {                                                                         \
          return x.name_;                                                         \
        }                                                                         \
      };                                                                          \
      return getter_t{};                                                          \
    }                                                                             \
    else if constexpr (e.N_value == nt::ax_name_container_base_const::c_get_name) \
    {                                                                             \
      struct name_getter_t                                                        \
      {                                                                           \
        static constexpr auto get_name()                                          \
        {                                                                         \
          return #name_;                                                          \
        }                                                                         \
      };                                                                          \
      return name_getter_t{};                                                     \
    }                                                                             \
  };                                                                              \
  return nt::ax_name_container<                                                   \
      nt::ax_name_container_base<decltype(struct_maker_template_lambda)>>{};      \
}()

#define ax_new_axis(name_, value) static constexpr inline auto name_ = (ax_maker(name_) = value)

#define ax_new_axis_t(name_, value)                                  \
  auto __internal__##name_ = [] { return ax_maker(name_) = value; }; \
  using name_ = decltype(__internal__##name_())

// ax_new_axis(name_1231, 123);

#define __nt_new_axis_core(name_)                         \
  struct zt##name_                                        \
  {                                                       \
    template <typename T>                                 \
    struct type_wrap                                      \
    {                                                     \
      type_wrap() {}                                      \
      template <typename T1>                              \
      type_wrap(T1 &&e_) : name_(std::forward<T1>(e_)) {} \
      T name_;                                            \
    };                                                    \
    template <typename Data_T>                            \
    using base_t = type_wrap<Data_T>;                     \
    static auto get_name()                                \
    {                                                     \
      return #name_;                                      \
    }                                                     \
    template <typename T>                                 \
    static constexpr auto &get(T &t)                      \
    {                                                     \
      return t.name_;                                     \
    }                                                     \
    template <typename T>                                 \
    static constexpr const auto &get(const T &t)          \
    {                                                     \
      return t.name_;                                     \
    }                                                     \
  }

#define __nt_new_axis(qualifier, name_, value) \
  namespace __nt                               \
  {                                            \
    __nt_new_axis_core(name_);                 \
  }                                            \
  qualifier name_ = (nt::ax_name_container<__nt::zt##name_>{} = value)

#define nt_new_axis(name_, value) __nt_new_axis(static constexpr inline auto, name_, value)
#define nt_new_axis_c(name_, value) __nt_new_axis(static const inline auto, name_, value)

#define nt_new_axis_t(name_, value) \
  namespace __nt                    \
  {                                 \
    __nt_new_axis_core(name_);      \
  }                                 \
  using name_ = decltype(nt::ax_name_container<__nt::zt##name_>{} = value)

#define nt_new_name(name_)     \
  namespace __nt               \
  {                            \
    __nt_new_axis_core(name_); \
  }                            \
  static constexpr inline auto name_ = nt::ax_name_container<__nt::zt##name_> {}

#define nt_new_name_t(name_)   \
  namespace __nt               \
  {                            \
    __nt_new_axis_core(name_); \
  }                            \
  using name_ = nt::ax_name_container<__nt::zt##name_>

namespace nt::algorithms
{


  template <typename T1, typename T2, typename Prj_T >
  struct __range__impl_prj
  {
    __range__impl_prj(T1 &&b, T2 &&e, Prj_T &&prj) : m_begin(std::move(b), prj), m_end(std::move(e), prj) {}
    __range__impl_prj(const T1 &b, const T2 &e, Prj_T &&prj ) : m_begin(b, prj), m_end(e, prj) {}
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
      return m_end - m_begin;
    }
    struct begin_t : T1
    {
      begin_t(T1 &&t1, Prj_T &pro) : T1(std::move(t1)), m_prj(pro) {}
      begin_t(const T1 &t1, Prj_T &pro) : T1(t1), m_prj(pro) {}
      Prj_T m_prj;
      auto operator*() const
      {
        return m_prj(**((const T1 *)this));
      }
      T1 get_base() const
      {
        return *((const T1 *)this);
      }
    } m_begin, m_end;
  };


  template <typename T1, typename T2, typename PRJ>
  auto __range__(T1 &&b, T2 &&e, PRJ &&prj)
  {
    return __range__impl_prj<nt::_Remove_cvref_t<T1>, nt::_Remove_cvref_t<T2>, nt::_Remove_cvref_t<PRJ>>(std::forward<T1>(b), std::forward<T2>(e),
                                                                                                         std::forward<PRJ>(prj));
  }

  template <typename T1, typename T2>
  auto __range__(T1 &&b, T2 &&e)
  {
    return __range__(
      std::forward<T1>(b), std::forward<T2>(e), 
      [](auto&& e ) -> decltype(auto) {  return e;}
    );
  }


  template <typename... T>
  struct __group
  {
    template <typename VEC_T, typename FUNC_T>
    static auto apply_append(const std::vector<VEC_T> &vec, FUNC_T &&fun)
    {
      auto fun_c = [&](const auto &tail, const auto &head)
      {
        auto rng = __range__(tail, head);
        return nt::ntuple(T::get(*tail)...) | fun(rng);
      };
      return __apply__internal__(vec, fun_c);
    }

    template <typename VEC_T, typename FUNC_T>
    static auto apply(const std::vector<VEC_T> &vec, FUNC_T &&fun)
    {
      auto fun_c = [&](const auto &tail, const auto &head)
      {
        auto rng = __range__(tail, head);
        return fun(rng);
      };
      return __apply__internal__(vec, fun_c);
    }

  private:
    template <typename VEC_T>
    static auto __assert_is_sorted__(const std::vector<VEC_T> &vec)
    {
      static constexpr auto lt = nt::comparators::lessThan<T...>();
      auto is_sorted_ = std::is_sorted(std::begin(vec), std::end(vec), lt);

      if (!is_sorted_)
      {
        throw std::runtime_error("group::apply expects the input vector to be sorted according to the group parameters\nthe input vector was not sorted with respect to the group axis");
      }
    }

    template <typename VEC_T, typename FUNC_T>
    static auto create_empty_vector()
    {
      return std::vector<
          decltype(std::declval<FUNC_T>()(
              std::begin(std::declval<const std::vector<VEC_T> &>()),
              std::end(std::declval<const std::vector<VEC_T> &>())))>{};
    }

    template <typename VEC_T, typename FUNC_T>
    static auto __apply__internal__(const std::vector<VEC_T> &vec, FUNC_T &&fun_c)
    {
      __assert_is_sorted__(vec);
      auto ret = create_empty_vector<VEC_T, FUNC_T>();

      if (vec.empty())
      {
        return ret;
      }

      auto tail = std::begin(vec);
      static constexpr auto eq = nt::comparators::equal<T...>();
      for (auto head = std::begin(vec); head != std::end(vec); ++head)
      {
        if (!eq(*tail, *head))
        {

          ret.push_back(
              fun_c(tail, head));
          tail = head;
        }
      }

      ret.push_back(
          fun_c(tail, std::end(vec)));

      return ret;
    }
  };

  template <typename... ARGS>
  auto group(ARGS...)
  {
    return __group<nt::_Remove_cvref_t<ARGS>...>{};
  }



}

namespace nt::comparators
{

  struct on_common_args_t
  {

    template <typename T1, typename T2>
    constexpr static bool __comp__(T1 &&t1, T2 &&t2)
    {
      bool ret = true;
      constexpr_for<0, _Remove_cvref_t<T1>::__size__, 1>(
          [&](const auto i)
          {
            using N_th_T = _Remove_cvref_t<decltype(nt::get_nth<i>(t1))>;
            if constexpr (contains_type_v<N_th_T, _Remove_cvref_t<T2>>)
            {
              ret &= (N_th_T::get(t1) == N_th_T::get(t2));
            }
          });
      return ret;
    }

    template <typename T1, typename T2>
    constexpr bool operator()(T1 &&t1, T2 &&t2) const
    {
      return __comp__(std::forward<T1>(t1), std::forward<T2>(t2));
    }
  };
  constexpr inline on_common_args_t on_common_args;
}
