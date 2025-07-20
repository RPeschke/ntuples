#pragma once

#include <cstddef>
#include <iostream>

namespace nt
{
    template <typename T>
    class span : public std::ranges::view_interface<span<T>>
    {
    public:
        using element_type = T;
        using value_type = std::remove_cv_t<T>;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using pointer = T *;
        using reference = T &;

        class iterator
        {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = std::ptrdiff_t;
            using pointer = T *;
            using reference = T &;

            iterator() : m_ptr(nullptr), m_stride(0) {}
            iterator(std::byte *ptr, size_type stride)
                : m_ptr(ptr), m_stride(stride) {}

            reference operator*() const { return *reinterpret_cast<pointer>(m_ptr); }
            pointer operator->() const { return reinterpret_cast<pointer>(m_ptr); }

            iterator &operator++()
            {
                m_ptr += m_stride;
                return *this;
            }

            iterator operator++(int)
            {
                iterator temp = *this;
                m_ptr += m_stride;
                return temp;
            }

            friend bool operator==(const iterator &a, const iterator &b)
            {
                return a.m_ptr == b.m_ptr;
            }

            friend bool operator!=(const iterator &a, const iterator &b)
            {
                return a.m_ptr != b.m_ptr;
            }

        private:
            std::byte *m_ptr;
            size_type m_stride;
        };

        span(std::byte *ptr, size_type count, size_type stride = sizeof(T))
            : m_ptr(ptr), m_count(count), m_stride(stride) {}

        span() =default;
        reference operator[](size_type idx) const
        {
            return *reinterpret_cast<pointer>(m_ptr + idx * m_stride);
        }

        pointer data() const noexcept
        {
            return reinterpret_cast<pointer>(m_ptr);
        }

        size_type size() const noexcept
        {
            return m_count;
        }

        size_type stride() const noexcept
        {
            return m_stride;
        }

        iterator begin() const noexcept
        {
            return iterator(m_ptr, m_stride);
        }

        iterator end() const noexcept
        {
            return iterator(m_ptr + m_count * m_stride, m_stride);
        }

    private:
        std::byte *m_ptr = nullptr;
        size_type m_count = 0;
        size_type m_stride = 0;
    };

}

#define nt_span(container, member) nt::span<decltype(container[0].member.v)>(                    \
    container.size() ? (std::byte *)&decltype(container[0].member)::get(container[0]) : nullptr, \
    container.size(), sizeof(decltype(container[0])))
