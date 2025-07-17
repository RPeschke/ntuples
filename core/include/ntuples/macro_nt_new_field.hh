#pragma once

#include "ntuples/ntuples.hh"




#define __nt_new_field_core(field_name)                         \
  struct zt##field_name                                        \
  {                                                       \
    template <typename T>                                 \
    struct type_wrap                                      \
    {                                                     \
      constexpr type_wrap() {}                                      \
      template <typename T1>                              \
      constexpr type_wrap(T1 &&e_) : field_name(std::forward<T1>(e_)) {} \
      T field_name;                                            \
      decltype(auto) value() const                                           \
          {                                                                       \
            return field_name;                                                         \
          }                                               \
    };                                                    \
    template <typename Data_T>                            \
    using base_t = type_wrap<Data_T>;                     \
    static auto get_name()                                \
    {                                                     \
      return #field_name;                                 \
    }                                                     \
    template <typename T>                                 \
    static constexpr decltype(auto) get(T&& t) {          \
      return std::forward<T>(t).field_name;               \
    }                                                     \
    template <typename T=int>                                                     \
    static constexpr decltype(auto) static_assert_fail() {\
       static_assert(dependent_false<T>::value, "[NTUPLE ERROR] Field `" #field_name "` does not exist in this ntuple");                                                                     \
    }\
  }

#define __nt_new_field(qualifier, field_name, value) \
  namespace __nt                               \
  {                                            \
    __nt_new_field_core(field_name);                 \
  }                                            \
  qualifier field_name = (nt::ax_name_container<__nt::zt##field_name>{} = value)

#define nt_new_field(field_name, value) __nt_new_field(static constexpr inline auto, field_name, value)
#define nt_new_field_c(field_name, value) __nt_new_field(static const inline auto, field_name, value)

#define nt_new_field_t(field_name, value) \
  namespace __nt                    \
  {                                 \
    __nt_new_field_core(field_name);      \
  }                                 \
  using field_name = decltype(nt::ax_name_container<__nt::zt##field_name>{} = value)

#define nt_new_field_name(field_name)     \
  namespace __nt               \
  {                            \
    __nt_new_field_core(field_name); \
  }                            \
  static constexpr inline auto field_name = nt::ax_name_container<__nt::zt##field_name> {}

#define nt_new_field_name_t(field_name)   \
  namespace __nt               \
  {                            \
    __nt_new_field_core(field_name); \
  }                            \
  using field_name = nt::ax_name_container<__nt::zt##field_name>




