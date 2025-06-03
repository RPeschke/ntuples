#pragma once

#include "ntuples/ntuples.hh"


// ax_new_axis(name_1231, 123);

#define __nt_new_axis_core(name_)                         \
  struct zt##name_                                        \
  {                                                       \
    template <typename T>                                 \
    struct type_wrap                                      \
    {                                                     \
      constexpr type_wrap() {}                                      \
      template <typename T1>                              \
      constexpr type_wrap(T1 &&e_) : name_(std::forward<T1>(e_)) {} \
      T name_;                                            \
    };                                                    \
    template <typename Data_T>                            \
    using base_t = type_wrap<Data_T>;                     \
    static auto get_name()                                \
    {                                                     \
      return #name_;                                      \
    }                                                     \
    template <typename T>                                 \
    static constexpr decltype(auto) get(T&& t) {          \
      return std::forward<T>(t).name_;                    \
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




