#pragma once

#include "ntuples/ntuples.hh"


#define ax_maker(name_) []() constexpr {                                          \
  auto struct_maker_template_lambda = [](auto e) constexpr {                      \
    using ARG_T = decltype(e);                                                    \
    if constexpr (e.N_value == nt::ax_name_container_base_const::c_struct_maker)  \
    {                                                                             \
      if constexpr (!std::is_reference_v<typename ARG_T::type>)                   \
      {                                                                           \
        struct Zt##name_                                                          \
        {                                                                         \
          constexpr Zt##name_() {}                                                          \
          constexpr Zt##name_(const decltype(e.val) &e_) : name_(e_) {}                     \
          constexpr Zt##name_(decltype(e.val) &e_) : name_(e_) {}                           \
          constexpr Zt##name_(decltype(e.val) &&e_) : name_(std::move(e_)) {}               \
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