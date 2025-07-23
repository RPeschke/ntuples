#pragma once
#include "ntuples/ntuples.hh"

namespace nt
{

    namespace __imple__     {

    template <typename FIELD_T, typename VECT_T, std::size_t Index>
    struct member_span
    {
        member_span()
        {
        }
        static constexpr std::size_t field_index = Index;
        using M_FIELD_T = FIELD_T;
        using M_VEC_T = VECT_T;

        nt::span<FIELD_T> get()
        {
            auto &m_vec = reinterpret_cast<VECT_T &>(*(this - field_index));

            return nt::span<FIELD_T>(
                m_vec.m_data.size() ? (std::byte *)&(FIELD_T::get(m_vec.m_data[0])) : nullptr,
                m_vec.m_data.size(), sizeof(decltype(m_vec.m_data[0])));
        }

        auto get_primitive()
        {
            auto &m_vec = reinterpret_cast<VECT_T &>(*(this - field_index));

            return nt::span<decltype(FIELD_T::get(m_vec.m_data[0]).v)>(
                m_vec.m_data.size() ? &(FIELD_T::get(m_vec.m_data[0]).v) : nullptr,
                m_vec.m_data.size(), sizeof(decltype(m_vec.m_data[0])));
        }

        auto operator()()
        {
            return get();
        }
    };

    template <typename IndexSeq, typename... Ts>
    struct vector_frame_impl;

    template <std::size_t... Is, typename... Ts>
    struct vector_frame_impl<std::index_sequence<Is...>, Ts...> :

        nt::base_maker_t<
            nt::_Remove_cvref_t<Ts>,
            nt::ax_type2<
                nt::__imple__::member_span<Ts, vector_frame_impl<std::index_sequence<Is...>, Ts...>, Is>,
                typename Ts::struct_maker>>...
    {
        std::vector<nt::ntuple<Ts...>> m_data;
        /* data */
        vector_frame_impl(const std::vector<
                          nt::ntuple<Ts...>> &vec) : m_data(vec) {}

        vector_frame_impl() = default;
        friend auto begin(vector_frame_impl const &v) { return v.m_data.begin(); }
        friend auto end(vector_frame_impl const &v) { return v.m_data.end(); }

        friend auto begin(vector_frame_impl &v) { return v.m_data.begin(); }
        friend auto end(vector_frame_impl &v) { return v.m_data.end(); }
        friend std::ostream &operator<<(std::ostream &os, const vector_frame_impl &v)
        {
            os << v.m_data;
            return os;
        }
        decltype(auto) operator[](std::size_t i)
        {
            return m_data[i];
        }
        decltype(auto) operator[](std::size_t i) const
        {
            return m_data[i];
        }
    };
    }
    template <typename... Ts>
    using vector_frame = nt::__imple__::vector_frame_impl<std::index_sequence_for<Ts...>, Ts...>;

    template <typename... T>
    auto to_vector_frame(const std::vector<nt::ntuple<T...>> &vec)
    {
        return vector_frame<T...>(vec);
    }

    namespace __imple__
    {
        template <typename... T>
        auto get_vector_frame_type(const nt::ntuple<T...> &vec)
        {
            return vector_frame<T...>(std::vector<nt::ntuple<T...>>{});
        }
    }

    template <typename FUNC_T>
    auto fill_vector_frame(size_t entries, FUNC_T &&func)
    {
        decltype(__imple__::get_vector_frame_type(func(size_t(0)))) ret;
        ret.m_data.reserve(entries);
        for (size_t i = 0; i < entries; ++i)
        {
            ret.m_data.push_back(func(i));
        }
        return ret;
    }

    template <typename RANGE_T>
    auto fill_vector_frame(RANGE_T &&range_)
    {

        decltype(__imple__::get_vector_frame_type(*range_.begin())) ret;
        // ret.m_data.reserve(entries);
        for (auto &&e : range_)
        {
            ret.m_data.push_back(e);
        }
        return ret;
    }
}