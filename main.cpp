#include <iostream>
#include <cassert>
#include <vector>
#include "Tests.h"

#define START_TEST(a) try{            \
a();                                                                     \
std::cout << "Ok "<< #a << " is successful !" << std::endl;              \
}  catch (const std::exception& e) {                                            \
std::cout << "!  "<< #a<< " Exception caught: " << e.what() << std::endl; \
}

//Вопрос 1.
//
//На языке С/С++, написать алгоритм (функцию) определения четности целого числа, который будет аналогичен
//нижеприведенному по функциональности, но отличен по своей сути.
//
//Объяснить плюсы и минусы обеих реализаций.
//
//C/C++ example:
//
//bool isEven(int value){return value%2==0;}


// Реализация основанная на получении остатка от деления может занять больше тактов, чем использование операции побитового
// сдвига или побитовой маски (побитовое И), потому что, выполнение этой операции зависит от реализации её компилятором, и
// тех оптимизаций которые он приминяет. Побитовые операции же элементарны, и подразумевают именно, то что написанно,
// соответственно более предсказуемы и максимально быстры.
bool isEven(int value) { return value % 2 == 0; }

//Реализация основанная на побитовой маске (побитовое И)
bool isEvenMyImp(int value) { return !(value & 1); }

void TestIsEvenMyImp() {
    int nums[] = {INT32_MIN, -143, -16, -11, -3, -2, -1,
                  0, 1, 2, 3, 10, 15, 142, INT32_MAX};
    for (auto n : nums) {
        assert(isEven(n) == isEvenMyImp(n) && "TestIsEvenMyImp fail");
    }
}

//Вопрос 2.
//
//На языке С++, написать минимум по 2 класса реализовывающих циклический буфер.
//
//Объяснить плюсы и минусы каждой реализации.

// Задание 2 находится в файле CircularBuffer
// Изначально планировалось реализовать быстрый буфер хранящийся на стеке постоянного размера, и буфер лежащий в куче.
// Для этого, был создан класс CircularBufferArrayBase и два аллокатора, один для стековой памяти, второй для кучи.
// Алокаторы выбираются исходя из условия прописаного в шаблоне CircularBufferArray и его специализации.
// Для реализации быстрой итерации по непрерывной памяти, был создан шаблонный класс CyclicRangeValue, который
// обеспечивает возможность циклической итерации в заданном диапазоне.
// Для проверки корректности работы класса был написан ряд юнит тестов. И тестов скорости работы.
// После тестирования скорости, стало понятно, что за счет непрерывного доступа к ячейкам памяти в куче, версия с аллокатором
// в куче работает так же быстро как и версия на стеке, не зависимо от размеров данных и их колличества.
// Соответственно шаблонная версия с алокатором на стеке безсмысленна.
// Что можно улучшить. Можно удалить специализацию с алокацией на стеке, так в отсутствии необходимости знать размер
// стека на этапе компиляции можно удалить шаблонный параметр размера буфера. Нужно переписать алокатор на алокатор со
// стандартным интерфейсом. Доработать класс, до возможности использования стандартного алокатора.
// Добавить возможность увеличения/уменьшения размера буфера, путем релокации памяти.


//Вопрос 3.
//
//На языке С/С++, написать функцию, которая быстрее всего (по процессорным тикам) отсортирует данный ей массив чисел.
//
//Массив может быть любого размера со случайным порядком чисел (в том числе и отсортированным).
//
//Объяснить почему вы считаете, что функция соответствует заданным критериям.

// Данный способ сортировки, называется сортировкой подсчетом. Это самый быстрый способ сортировки чисел, так как
// выполняется за O(n+k). А именно, сортировка выполняется в два прохода: первый проход по массиву считывает числа и
// записывает их колличество в вектор, где индекс это число, а значение это колличество. Второй проход выписывает из
// этого вектора числа в исходный массив по порядку с учетом их колличества.

void CountingSort(std::vector<int>& nums) {
    int lo = INT_MAX, hi = INT_MIN;
    std::vector<int> catalog_plus(nums.size(), 0);
    std::vector<int> catalog_minus(nums.size(), 0);

    for (auto n : nums) {
        if (n < 0) {
            if (catalog_minus.size() <= std::abs(n)) {
                catalog_minus.resize(std::abs(n)*2, 0);
            }
            ++catalog_minus[std::abs(n)];
        } else {
            if (catalog_plus.size() <= n) {
                catalog_plus.resize(n * 2, 0);
            }
            ++catalog_plus[n];
        }
        lo = std::min(lo, n);
        hi = std::max(hi, n);
    }

    int j = 0;

    for (int i = lo; i <= hi; ++i) {
        if (i < 0) {
            for (int k = 0; k < catalog_minus[std::abs(i)]; ++k) {
                nums[j++] = i;
            }
        } else {
            for (int k = 0; k < catalog_plus[i]; ++k) {
                nums[j++] = i;
            }
        }
    }
}

void TestCountingSort() {
    std::vector<int> nums = {4, 2, 5, -4, -6, 7, 1, 2, 8, 9, -1, 8, 3, 2, 1};
    CountingSort(nums);
    std::vector<int> expected{-6, -4, -1, 1, 1, 2, 2, 2, 3, 4, 5, 7, 8, 8, 9};
    assert(nums == expected && "TestCountingSort fail");
}

int main() {
    START_TEST(TestIsEvenMyImp)

    START_TEST(CircBufferConstructBase)
    START_TEST(CircBufferConstructIterators)
    START_TEST(CircBufferConstructInitializerList)
    START_TEST(CircBufferConstructValue)
    START_TEST(CircBufferMethodEmplaceBack)
    START_TEST(CircBufferMethodPushBack)
    START_TEST(CircBufferMethodPopFront)
    START_TEST(CircBufferMethodPopBack)
    START_TEST(CircBufferMethodGetFront)
    START_TEST(CircBufferMethodGetBack)
    START_TEST(CircBufferMethodReleaseFront)
    START_TEST(CircBufferMethodReleaseBack)
    START_TEST(CircBufferMethodSize)
    START_TEST(CircBufferMethodCapacity)
    START_TEST(CircBufferMethodClear)
    START_TEST(CircBufferMethodEmpty)
    START_TEST(CircBufferMethodIsFull)
    START_TEST(CircBufferSquareOperator)
    START_TEST(CircBufferIteratorOperatorAssignment)
    START_TEST(CircBufferIteratorOperatorIncrement)
    START_TEST(CircBufferIteratorOperatorEquality)
    START_TEST(CircBufferIteration)
    START_TEST(CircBufferTimeTest)

    START_TEST(TestCountingSort)

    return 0;
}
