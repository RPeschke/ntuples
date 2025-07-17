#pragma once
#include "ntuples/ntuples.hh"
namespace nt
{
    namespace __imple__
    {

        template <typename T>
        struct is_ax_name_container
        {
        private:
            template <typename U>
            static std::true_type test(const nt::ax_name_container<U> *);

            static std::false_type test(...);

        public:
            static constexpr bool value = decltype(test(std::declval<std::remove_cvref_t<T> *>()))::value;
        };

        template <typename T>
        constexpr bool is_ax_name_container_v = is_ax_name_container<T>::value;

        template <typename T, typename K>
        constexpr bool is_assignable_via_brackets_v = requires(T t, K k) {
            { t[k] = k };
        };

        template <size_t N, typename NT_T, typename T1, typename... ARGS>
        auto bind_args1(NT_T &&t, T1 &&t1, ARGS &&...args)
        {
            if constexpr (::nt::__imple__::is_ax_name_container_v<T1>)
            {
                if constexpr (t1.index_of<NT_T>() < N)
                {
                    static_assert(dependent_false<NT_T>::value, "[NTUPLE ERROR] Named argument appears after positional arguments. "
                                                                "All named arguments must follow positional ones.");
                }
                else
                {
                    t[t1] = t1;
                }
            }

            if constexpr (sizeof...(args) == 0)
            {
                return t;
            }
            else
            {
                return ::nt::__imple__::bind_args1<N>(t, std::forward<ARGS>(args)...);
            }
        }

        template <size_t N, typename NT_T, typename T1, typename... ARGS>
        auto bind_args0(NT_T &&t, T1 &&t1, ARGS &&...args)
        {
            if constexpr (is_ax_name_container_v<T1>)
            {
                return ::nt::__imple__::bind_args1<N>(t, std::forward<T1>(t1), std::forward<ARGS>(args)...);
            }
            else
            {
                nt::get_nth<N>(t) = t1;
                return ::nt::__imple__::bind_args0<N + 1>(t, std::forward<ARGS>(args)...);
            }
        }
    }
    template <typename... NamedArgs>
    auto bind_args(NamedArgs &&...named_args)
    {
        // Return a lambda that captures named_args
        return [&](auto &&...args)
        {
            auto tup = nt::ntuple(std::forward<NamedArgs>(named_args)...);
            if constexpr (sizeof...(args) == 0)
            {
                return tup;
            }
            else
            {
                return ::nt::__imple__::bind_args0<0>(std::move(tup), std::forward<decltype(args)>(args)...);
            }
        };
    }
}