#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <functional>
#include <memory>
#include <numeric>
#include <random>
#include <string>
#include <utility>
#include <vector>

#include "Quicksort.h"

namespace {

    // Структура без операторов сравнения
    struct Person {
        std::string name;
        int age;

        Person(std::string name, int age)
            : name(std::move(name)), age(age) {
        }
    };

    // Тип, который только можно перемещать
    struct MoveOnly {
        int value;
        std::unique_ptr<int> data;

        explicit MoveOnly(int value)
            : value(value), data(new int(value * 10)) {
        }

        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;

        MoveOnly(MoveOnly&&) noexcept = default;
        MoveOnly& operator=(MoveOnly&&) noexcept = default;
    };

    // Проверка сортировки массива заданного размера
    void CheckReverseArraySort(std::size_t size) {
        std::vector<int> arr(size);

        for (std::size_t i = 0; i < size; ++i) {
            arr[i] = static_cast<int>(size - i);
        }

        std::vector<int> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(),
            arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    TEST(QuickSortTest, EmptyNullRange) {
        int* first = nullptr;

        EXPECT_NO_THROW(
            algo::sort(first, first,
                [](const int& a, const int& b) {
                    return a < b;
                })
        );
    }

    TEST(QuickSortTest, EmptyNonNullRange) {
        int arr[] = { 42 };

        algo::sort(arr, arr,
            [](const int& a, const int& b) {
                return a < b;
            });

        EXPECT_EQ(arr[0], 42);
    }

    TEST(QuickSortTest, EmptyVector) {
        std::vector<int> arr;

        int* first = arr.data();

        algo::sort(first, first,
            [](const int& a, const int& b) {
                return a < b;
            });

        EXPECT_TRUE(arr.empty());
    }

    TEST(QuickSortTest, SingleElement) {
        int arr[] = { 42 };

        algo::sort(arr, arr + 1,
            [](const int& a, const int& b) {
                return a < b;
            });

        EXPECT_EQ(arr[0], 42);
    }

    TEST(QuickSortTest, TwoElementsSorted) {
        int arr[] = { 1, 2 };

        algo::sort(arr, arr + 2,
            [](const int& a, const int& b) {
                return a < b;
            });

        EXPECT_EQ(arr[0], 1);
        EXPECT_EQ(arr[1], 2);
    }

    TEST(QuickSortTest, TwoElementsUnsorted) {
        int arr[] = { 2, 1 };

        algo::sort(arr, arr + 2,
            [](const int& a, const int& b) {
                return a < b;
            });

        EXPECT_EQ(arr[0], 1);
        EXPECT_EQ(arr[1], 2);
    }

    // ============================================================
    // 2. Отдельная проверка insertion_sort()
    // ============================================================

    TEST(InsertionSortTest, EmptyNullRange) {
        int* first = nullptr;

        EXPECT_NO_THROW(
            algo::insertion_sort(first, first,
                [](const int& a, const int& b) {
                    return a < b;
                })
        );
    }

    TEST(InsertionSortTest, SingleElement) {
        int arr[] = { 10 };

        algo::insertion_sort(
            arr, arr + 1,
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr[0], 10);
    }

    TEST(InsertionSortTest, ReverseSortedArray) {
        std::vector<int> arr = {
            10, 9, 8, 7, 6, 5, 4, 3, 2, 1
        };

        algo::insertion_sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
    }

    TEST(InsertionSortTest, ArrayWithDuplicates) {
        std::vector<int> arr = {
            5, 3, 5, 1, 3, 2, 2, 5, 1
        };

        algo::insertion_sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        std::vector<int> expected = {
            1, 1, 2, 2, 3, 3, 5, 5, 5
        };

        EXPECT_EQ(arr, expected);
    }

    TEST(InsertionSortTest, DescendingOrder) {
        std::vector<int> arr = {
            1, 5, 3, 8, 2, 9, 4
        };

        algo::insertion_sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a > b;
            }
        );

        EXPECT_TRUE(
            std::is_sorted(
                arr.begin(), arr.end(),
                std::greater<int>()
            )
        );
    }

    // ============================================================
    // 3. Проверка различных последовательностей QuickSort
    // ============================================================

    TEST(QuickSortTest, AlreadySortedArrayAboveThreshold) {
        std::vector<int> arr(200);

        std::iota(arr.begin(), arr.end(), 0);

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
    }

    TEST(QuickSortTest, ReverseSortedArrayAboveThreshold) {
        std::vector<int> arr(200);

        std::iota(arr.begin(), arr.end(), 0);
        std::reverse(arr.begin(), arr.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
    }

    TEST(QuickSortTest, AllElementsAreEqual) {
        std::vector<int> arr(257, 5);

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_TRUE(
            std::all_of(arr.begin(), arr.end(),
                [](int x) {
                    return x == 5;
                })
        );
    }

    TEST(QuickSortTest, ManyDuplicatesAboveThreshold) {
        std::vector<int> arr(257);

        for (std::size_t i = 0; i < arr.size(); ++i) {
            arr[i] = static_cast<int>((i * 7) % 11) - 5;
        }

        std::reverse(arr.begin(), arr.end());

        std::vector<int> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    TEST(QuickSortTest, OrganPipeSequence) {
        std::vector<int> arr;

        for (int i = 0; i <= 50; ++i) {
            arr.push_back(i);
        }

        for (int i = 49; i >= 0; --i) {
            arr.push_back(i);
        }

        std::vector<int> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    // ============================================================
    // 4. Проверка границы между insertion_sort и quick_sort
    // ============================================================

    TEST(QuickSortTest, ArrayBelowInsertionThreshold) {
        CheckReverseArraySort(41);
    }

    TEST(QuickSortTest, ArrayAtInsertionThreshold) {
        CheckReverseArraySort(42);
    }

    TEST(QuickSortTest, ArrayAboveInsertionThreshold) {
        CheckReverseArraySort(43);
    }

    // ============================================================
    // 5. Большие массивы
    // ============================================================

    TEST(QuickSortTest, LargeArray) {
        std::vector<int> arr(100000);

        std::mt19937 gen(12345);
        std::uniform_int_distribution<int> dist(
            -1000000, 1000000
        );

        for (int& x : arr) {
            x = dist(gen);
        }

        std::vector<int> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    // ============================================================
    // 6. Проверка различных компараторов
    // ============================================================

    TEST(QuickSortTest, DescendingOrderAboveThreshold) {
        std::vector<int> arr(200);

        std::iota(arr.begin(), arr.end(), 0);

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a > b;
            }
        );

        EXPECT_TRUE(
            std::is_sorted(
                arr.begin(), arr.end(),
                std::greater<int>()
            )
        );
    }

    TEST(QuickSortTest, StandardGreaterComparator) {
        std::vector<int> arr = {
            10, 3, 7, 2, 9, 1, 5, 8, 4, 6
        };

        arr.resize(100, 0);

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            std::greater<int>()
        );

        EXPECT_TRUE(
            std::is_sorted(
                arr.begin(), arr.end(),
                std::greater<int>()
            )
        );
    }

    TEST(QuickSortTest, AbsoluteValueAboveThreshold) {
        std::vector<int> arr(101);

        for (int i = 0; i < 101; ++i) {
            arr[i] = i - 50;
        }

        auto comp = [](const int& a, const int& b) {
            if (std::abs(a) != std::abs(b)) {
                return std::abs(a) < std::abs(b);
            }

            return a < b;
            };

        std::vector<int> expected = arr;
        std::sort(expected.begin(), expected.end(), comp);

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            comp
        );

        EXPECT_EQ(arr, expected);
    }

    // ============================================================
    // 7. Проверка примитивных типов данных
    // ============================================================

    TEST(QuickSortTest, DoubleArrayAboveThreshold) {
        std::vector<double> arr;

        for (int i = 100; i >= 0; --i) {
            arr.push_back(i * 0.5 - 25.0);
        }

        std::vector<double> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const double& a, const double& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    // ============================================================
    // 8. Проверка нетривиальных типов данных
    // ============================================================

    TEST(QuickSortTest, StringArrayAboveThreshold) {
        std::vector<std::string> arr;

        for (int i = 80; i >= 0; --i) {
            arr.push_back("string_" + std::to_string(i));
        }

        std::vector<std::string> expected = arr;
        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const std::string& a, const std::string& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);
    }

    TEST(QuickSortTest, PersonArrayAboveThreshold) {
        std::vector<Person> arr;

        for (int i = 0; i < 100; ++i) {
            arr.emplace_back(
                "Person_" + std::to_string(i),
                (i * 17) % 30
            );
        }

        auto comp = [](const Person& a, const Person& b) {
            return a.age < b.age;
            };

        std::vector<int> expected_ages;
        std::vector<std::string> expected_names;

        for (const Person& p : arr) {
            expected_ages.push_back(p.age);
            expected_names.push_back(p.name);
        }

        std::sort(expected_ages.begin(), expected_ages.end());
        std::sort(expected_names.begin(), expected_names.end());

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            comp
        );

        std::vector<int> actual_ages;
        std::vector<std::string> actual_names;

        for (const Person& p : arr) {
            actual_ages.push_back(p.age);
            actual_names.push_back(p.name);
        }

        EXPECT_TRUE(std::is_sorted(
            arr.begin(), arr.end(), comp
        ));

        std::sort(actual_names.begin(), actual_names.end());

        EXPECT_EQ(actual_ages, expected_ages);
        EXPECT_EQ(actual_names, expected_names);
    }

    // ============================================================
    // 9. Проверка move-семантики
    // ============================================================

    TEST(QuickSortTest, MoveOnlyObjects) {
        std::vector<MoveOnly> arr;

        // 100 элементов гарантированно задействуют QuickSort
        for (int i = 100; i >= 1; --i) {
            arr.emplace_back(i);
        }

        algo::sort(
            arr.data(), arr.data() + arr.size(),
            [](const MoveOnly& a, const MoveOnly& b) {
                return a.value < b.value;
            }
        );

        for (int i = 0; i < 100; ++i) {
            EXPECT_EQ(arr[i].value, i + 1);

            ASSERT_NE(arr[i].data, nullptr);

            EXPECT_EQ(*arr[i].data, (i + 1) * 10);
        }
    }

    // ============================================================
    // 10. Проверка сортировки части массива
    // ============================================================

    TEST(QuickSortTest, SortSubrange) {
        std::vector<int> arr;

        // За пределами сортируемого интервала
        arr.push_back(-1000000);

        // Сортируемый интервал
        for (int i = 100; i >= 1; --i) {
            arr.push_back(i);
        }

        // За пределами сортируемого интервала
        arr.push_back(1000000);

        std::vector<int> expected = arr;

        std::sort(
            expected.begin() + 1,
            expected.end() - 1
        );

        algo::sort(
            arr.data() + 1,
            arr.data() + arr.size() - 1,
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        EXPECT_EQ(arr, expected);

        // Проверяем, что крайние элементы не изменились
        EXPECT_EQ(arr.front(), -1000000);
        EXPECT_EQ(arr.back(), 1000000);
    }

    // ============================================================
    // 11. Проверка динамически выделенного массива
    // ============================================================

    TEST(QuickSortTest, DynamicArray) {
        const int n = 1000;

        std::unique_ptr<int[]> arr(new int[n]);

        std::mt19937 gen(54321);
        std::uniform_int_distribution<int> dist(-1000, 1000);

        std::vector<int> expected(n);

        for (int i = 0; i < n; ++i) {
            arr[i] = dist(gen);
            expected[i] = arr[i];
        }

        std::sort(expected.begin(), expected.end());

        algo::sort(
            arr.get(),
            arr.get() + n,
            [](const int& a, const int& b) {
                return a < b;
            }
        );

        for (int i = 0; i < n; ++i) {
            EXPECT_EQ(arr[i], expected[i]);
        }
    }

    // ============================================================
    // 12. Сравнение результата со std::sort
    // ============================================================

    TEST(QuickSortTest, RandomizedComparisonWithStdSort) {
        std::mt19937 gen(12345);
        std::uniform_int_distribution<int> dist(
            -10000, 10000
        );

        // Проверяем размеры от 0 до 500
        // По 5 разных массивов для каждого размера
        for (int n = 0; n <= 500; ++n) {
            for (int iteration = 0; iteration < 5; ++iteration) {
                std::vector<int> arr(n);

                for (int& x : arr) {
                    x = dist(gen);
                }

                std::vector<int> expected = arr;

                std::sort(
                    expected.begin(),
                    expected.end()
                );

                int* first = arr.data();
                int* last = first;

                if (!arr.empty()) {
                    last += arr.size();
                }

                algo::sort(
                    first, last,
                    [](const int& a, const int& b) {
                        return a < b;
                    }
                );

                EXPECT_EQ(arr, expected)
                    << "Array size: " << n
                    << ", iteration: " << iteration;
            }
        }
    }

    // ============================================================
    // 13. Проверка непосредственного разбиения
    // ============================================================

    TEST(QuickSortTest, MedianOfThreePartition) {
        int arr[] = {
            9, 2, 7, 4, 8, 1, 6
        };

        auto comp = [](const int& a, const int& b) {
            return a < b;
            };

        int* pivot = algo::detail::find_support_element(
            arr, arr + 7, comp
        );

        // median(9, 4, 6) = 6
        EXPECT_EQ(*pivot, 6);

        // Все элементы слева должны быть не больше pivot
        for (int* i = arr; i < pivot; ++i) {
            EXPECT_FALSE(comp(*pivot, *i));
        }

        // Все элементы справа должны быть не меньше pivot
        for (int* i = pivot + 1; i < arr + 7; ++i) {
            EXPECT_FALSE(comp(*i, *pivot));
        }

        // Проверяем, что все элементы сохранились
        std::vector<int> actual(arr, arr + 7);

        std::sort(actual.begin(), actual.end());

        const std::vector<int> expected = {
            1, 2, 4, 6, 7, 8, 9
        };

        EXPECT_EQ(actual, expected);
    }

    // ============================================================
    // 14. Проверка QuickSort с минимальным порогом
    // ============================================================

    TEST(QuickSortTest, QuickSortWithThresholdTwoOnThreeElements) {
        std::vector<int> arr = { 3, 1, 2 };

        algo::quick_sort(
            arr.data(),
            arr.data() + arr.size(),
            [](const int& a, const int& b) {
                return a < b;
            },
            2
        );

        EXPECT_EQ(arr, (std::vector<int>{1, 2, 3}));
    }

    TEST(QuickSortTest, DifferentInsertionThresholds) {
        std::vector<int> source(257);

        std::mt19937 gen(12345);
        std::uniform_int_distribution<int> dist(-1000, 1000);

        for (int& x : source) {
            x = dist(gen);
        }

        std::vector<int> expected = source;
        std::sort(expected.begin(), expected.end());

        const std::ptrdiff_t thresholds[] = {
            -10, 0, 1, 2, 8, 16, 32, 64, 256, 1000
        };

        for (std::ptrdiff_t threshold : thresholds) {
            std::vector<int> arr = source;

            algo::quick_sort(
                arr.data(),
                arr.data() + arr.size(),
                [](const int& a, const int& b) {
                    return a < b;
                },
                threshold
            );

            EXPECT_EQ(arr, expected)
                << "Threshold: " << threshold;
        }
    }

} // namespace
