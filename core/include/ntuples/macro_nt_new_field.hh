#pragma once

#include "ntuples/ntuples.hh"




#define __nt_new_field_core(filed_name)                         \
  struct zt##filed_name                                        \
  {                                                       \
    template <typename T>                                 \
    struct type_wrap                                      \
    {                                                     \
      constexpr type_wrap() {}                                      \
      template <typename T1>                              \
      constexpr type_wrap(T1 &&e_) : filed_name(std::forward<T1>(e_)) {} \
      T filed_name;                                            \
    };                                                    \
    template <typename Data_T>                            \
    using base_t = type_wrap<Data_T>;                     \
    static auto get_name()                                \
    {                                                     \
      return #filed_name;                                      \
    }                                                     \
    template <typename T>                                 \
    static constexpr decltype(auto) get(T&& t) {          \
      return std::forward<T>(t).filed_name;                    \
    }                                                     \
  }

#define __nt_new_field(qualifier, filed_name, value) \
  namespace __nt                               \
  {                                            \
    __nt_new_field_core(filed_name);                 \
  }                                            \
  qualifier filed_name = (nt::ax_name_container<__nt::zt##filed_name>{} = value)

#define nt_new_field(filed_name, value) __nt_new_field(static constexpr inline auto, filed_name, value)
#define nt_new_field_c(filed_name, value) __nt_new_field(static const inline auto, filed_name, value)

#define nt_new_field_t(filed_name, value) \
  namespace __nt                    \
  {                                 \
    __nt_new_field_core(filed_name);      \
  }                                 \
  using filed_name = decltype(nt::ax_name_container<__nt::zt##filed_name>{} = value)

#define nt_new_field_name(filed_name)     \
  namespace __nt               \
  {                            \
    __nt_new_field_core(filed_name); \
  }                            \
  static constexpr inline auto filed_name = nt::ax_name_container<__nt::zt##filed_name> {}

#define nt_new_field_name_t(filed_name)   \
  namespace __nt               \
  {                            \
    __nt_new_field_core(filed_name); \
  }                            \
  using filed_name = nt::ax_name_container<__nt::zt##filed_name>




