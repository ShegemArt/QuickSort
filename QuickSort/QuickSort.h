#pragma once

#include <cstddef>
#include <utility>

namespace algo {

    // Максимальная длина интервала, который досортировывается вставками.
    constexpr std::ptrdiff_t kInsertionSortThreshold = 42;

    namespace detail {
        // Обмен элементов через перемещение
        template<typename T>
        void move_swap(T* a, T* b) {
            if (a == b) {
                return;
            }
            T tmp = std::move(*a);
            *a = std::move(*b);
            *b = std::move(tmp);
        }

        // Выбирает опорный элемент
        template<typename T, typename Compare>
        T* find_support_element(T* first, T* last, Compare& comp) {
            T* mid = first + (last - first) / 2;
            T* back = last - 1;
            if (comp(*mid, *first)) {
                move_swap(mid, first);
            }
            if (comp(*back, *mid)) {
                move_swap(back, mid);
            }
            if (comp(*mid, *first)) {
                move_swap(mid, first);
            }

            T* pivot = first + 1;
            move_swap(mid, pivot);

            T* i = first + 2;
            T* j = last - 2;
            while (i <= j) {
                while (comp(*i, *pivot)) {
                    ++i;
                }
                while (comp(*pivot, *j)) {
                    --j;
                }
                if (i <= j) {
                    move_swap(i, j);
                    ++i;
                    --j;
                }
            }
            move_swap(pivot, j);
            return j;
        }
    }

    // Сортировка вставками
    template<typename T, typename Compare>
    void insertion_sort(T* first, T* last, Compare comp) {
        if (first == last) {
            return;
        }
        for (T* i = first + 1; i != last; ++i) {
            T value = std::move(*i);
            T* j = i;
            while (j != first && comp(value, *(j - 1))) {
                *j = std::move(*(j - 1));
                --j;
            }
            *j = std::move(value);
        }
    }

    // Быстрая сортировка
    template<typename T, typename Compare>
    void quick_sort(T* first, T* last, Compare comp, std::ptrdiff_t insertion_threshold) {
        if (first == last) {
            return;
        }
        if (insertion_threshold < 2) {
            insertion_threshold = 2;
        }
        while (last - first > insertion_threshold) {
            T* pos = detail::find_support_element(first, last, comp);
            if (pos - first < last - (pos + 1)) {
                quick_sort(first, pos, comp, insertion_threshold);
                first = pos + 1;
            }
            else {
                quick_sort(pos + 1, last, comp, insertion_threshold);
                last = pos;
            }
        }
        insertion_sort(first, last, comp);
    }

    // Гибридная сортировка
    template<typename T, typename Compare>
    void sort(T* first, T* last, Compare comp) {
        quick_sort(first, last, comp, kInsertionSortThreshold);
    }
}