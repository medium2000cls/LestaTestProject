//
// Created by Dmitry on 29.02.2024.
//

#include <initializer_list>
#include <vector>
#include <iostream>
#include <chrono>
#include <variant>
#include "CircularBuffer.h"
#include "Tests.h"

#define ASSERT_MESSAGE(condition, message)                                      \
    {                                                                           \
        if (!(condition)) {                                                     \
            std::cerr << "Assertion failed: " << #condition << " (" << message  \
            << ") in " << __FILE__ << " at line " << __LINE__ << std::endl;     \
        } else {                                                                \
            std::cout << "Assertion passed: " << #condition << std::endl;       \
        }                                                                       \
    }

#define ASSERT(condition)                                                               \
    {                                                                                   \
        if (!(condition)) {                                                             \
            std::cerr << "    Assertion failed: " << #condition << " in "               \
            << __FILE__ << " at line " << __LINE__ << std::endl;                        \
            throw std::runtime_error("");                                               \
        } else {                                                                        \
            /*std::cout << "    Assertion passed: " << #condition << std::endl;*/       \
        }                                                                               \
    }

#define TIME_DIF(func) { \
        auto start = std::chrono::high_resolution_clock::now(); \
        func(); \
        auto end = std::chrono::high_resolution_clock::now(); \
        std::chrono::duration<double> duration = end - start; \
        std::cout << "Time taken by " #func << ": " << duration.count() << " seconds" << std::endl; \
        }



void CircBufferConstructBase() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a;
        ASSERT(a.Capacity() == 5);
        ASSERT(a.Size() == 0);
        ASSERT(a.IsEmpty());
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 50000> a;
        ASSERT(a.Capacity() == 50000);
        ASSERT(a.Size() == 0);
        ASSERT(a.IsEmpty());
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a;
        ASSERT(a.Capacity() == 5);
        ASSERT(a.Size() == 0);
        ASSERT(a.IsEmpty());
    }
}

void CircBufferConstructIterators() {
    std::vector<int> vec = {1, 2, 3, 4, 5};

    //StackAllocator
    {
        CircularBufferArray<int, 20> a(vec.begin(), vec.end());
        ASSERT(a.Size() == 5);
        for (size_t i = 0; i < vec.size(); ++i) {
            ASSERT(vec[i] == a[i]);
        }
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000> a(vec.begin(), vec.end());
        auto it_begin = a.begin();
        auto it_end = a.end();
        ASSERT(a.Size() == 5);
        for (size_t i = 0; i < vec.size(); ++i) {
            ASSERT(vec[i] == a[i]);
        }
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 20, int> a(vec.begin(), vec.end());
        auto it_begin = a.begin();
        auto it_end = a.end();
        ASSERT(a.Size() == 5);
        for (size_t i = 0; i < vec.size(); ++i) {
            ASSERT(vec[i] == a[i]);
        }
    }

}
void CircBufferConstructInitializerList() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> b = {1, 2, 3, 4, 5};
        ASSERT(b[0] == 1);
        ASSERT(b[1] == 2);
        ASSERT(b[2] == 3);
        ASSERT(b[3] == 4);
        ASSERT(b[4] == 5);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> c = {1, 2, 3, 4, 5};
        ASSERT(c[0] == 1);
        ASSERT(c[1] == 2);
        ASSERT(c[2] == 3);
        ASSERT(c[3] == 4);
        ASSERT(c[4] == 5);
    }
}
void CircBufferConstructValue() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = 5;
        ASSERT(a[0] == 5);
        ASSERT(a[1] == 5);
        ASSERT(a[2] == 5);
        ASSERT(a[3] == 5);
        ASSERT(a[4] == 5);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> b = 5;
        for (int i = 0; i < 5000; ++i) {
            ASSERT(b[i] == 5);
        }
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> c = 5;
        ASSERT(c[0] == 5);
        ASSERT(c[1] == 5);
        ASSERT(c[2] == 5);
        ASSERT(c[3] == 5);
        ASSERT(c[4] == 5);
    }
}
void CircBufferMethodEmplaceBack() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        a.EmplaceBack(6);

        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
        ASSERT(a[4] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        a.EmplaceBack(6);
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
        ASSERT(a[5] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        a.EmplaceBack(6);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
        ASSERT(a[4] == 6);
    }
}
void CircBufferMethodPushBack() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        a.PushBack(6);
        ASSERT(a.Size() == 5);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
        ASSERT(a[4] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        a.PushBack(6);
        ASSERT(a.Size() == 6);
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
        ASSERT(a[5] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        a.PushBack(6);
        ASSERT(a.Size() == 5);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
        ASSERT(a[4] == 6);
    }

}
void CircBufferMethodPopFront() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 2);
        ASSERT(a[1] == 3);
        ASSERT(a[2] == 4);
        ASSERT(a[3] == 5);
    }

}
void CircBufferMethodPopBack() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
    }
}
void CircBufferMethodGetFront() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetFront() == 1);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetFront() == 2);
        a.PopFront();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetFront() == 3);
        a.PopFront();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetFront() == 4);
        a.PopFront();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetFront() == 5);
        a.PopFront();
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetFront() == 1);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetFront() == 2);
        a.PopFront();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetFront() == 3);
        a.PopFront();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetFront() == 4);
        a.PopFront();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetFront() == 5);
        a.PopFront();
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetFront() == 1);
        a.PopFront();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetFront() == 2);
        a.PopFront();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetFront() == 3);
        a.PopFront();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetFront() == 4);
        a.PopFront();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetFront() == 5);
        a.PopFront();
        ASSERT(a.Size() == 0);
    }
}
void CircBufferMethodGetBack() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetBack() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetBack() == 4);
        a.PopBack();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetBack() == 3);
        a.PopBack();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetBack() == 2);
        a.PopBack();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetBack() == 1);
        a.PopBack();
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetBack() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetBack() == 4);
        a.PopBack();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetBack() == 3);
        a.PopBack();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetBack() == 2);
        a.PopBack();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetBack() == 1);
        a.PopBack();
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.GetBack() == 5);
        a.PopBack();
        ASSERT(a.Size() == 4);
        ASSERT(a.GetBack() == 4);
        a.PopBack();
        ASSERT(a.Size() == 3);
        ASSERT(a.GetBack() == 3);
        a.PopBack();
        ASSERT(a.Size() == 2);
        ASSERT(a.GetBack() == 2);
        a.PopBack();
        ASSERT(a.Size() == 1);
        ASSERT(a.GetBack() == 1);
        a.PopBack();
        ASSERT(a.Size() == 0);
    }

}
void CircBufferMethodReleaseFront() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseFront() == 1);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseFront() == 2);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseFront() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseFront() == 4);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseFront() == 5);
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseFront() == 1);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseFront() == 2);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseFront() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseFront() == 4);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseFront() == 5);
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseFront() == 1);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseFront() == 2);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseFront() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseFront() == 4);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseFront() == 5);
        ASSERT(a.Size() == 0);
    }

}
void CircBufferMethodReleaseBack() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseBack() == 5);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseBack() == 4);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseBack() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseBack() == 2);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseBack() == 1);
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5000, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseBack() == 5);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseBack() == 4);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseBack() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseBack() == 2);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseBack() == 1);
        ASSERT(a.Size() == 0);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        ASSERT(a.ReleaseBack() == 5);
        ASSERT(a.Size() == 4);
        ASSERT(a.ReleaseBack() == 4);
        ASSERT(a.Size() == 3);
        ASSERT(a.ReleaseBack() == 3);
        ASSERT(a.Size() == 2);
        ASSERT(a.ReleaseBack() == 2);
        ASSERT(a.Size() == 1);
        ASSERT(a.ReleaseBack() == 1);
        ASSERT(a.Size() == 0);
    }

}
void CircBufferMethodSize() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        CircularBufferArray<int, 10> b = {1, 2, 3};
        ASSERT(b.Size() == 3);
        CircularBufferArray<int, 10> c = {};
        ASSERT(c.Size() == 0);
        CircularBufferArray<int, 10> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Size() == 10);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        CircularBufferArray<int, 2000> b = {1, 2, 3};
        ASSERT(b.Size() == 3);
        CircularBufferArray<int, 2000> c = {};
        ASSERT(c.Size() == 0);
        CircularBufferArray<int, 2000> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Size() == 10);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        CircularBufferArray<int, 10, int> b = {1, 2, 3};
        ASSERT(b.Size() == 3);
        CircularBufferArray<int, 10, int> c = {};
        ASSERT(c.Size() == 0);
        CircularBufferArray<int, 10, int> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Size() == 10);
    }
}
void CircBufferMethodCapacity() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        ASSERT(a.Capacity() == 10);
        CircularBufferArray<int, 10> b = {1, 2, 3};
        ASSERT(b.Capacity() == 10);
        CircularBufferArray<int, 10> c = {};
        ASSERT(c.Capacity() == 10);
        CircularBufferArray<int, 10> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Capacity() == 10);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a.Capacity() == 2000);
        CircularBufferArray<int, 2000> b = {1, 2, 3};
        ASSERT(b.Capacity() == 2000);
        CircularBufferArray<int, 2000> c = {};
        ASSERT(c.Capacity() == 2000);
        CircularBufferArray<int, 2000> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Capacity() == 2000);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Capacity() == 10);
        CircularBufferArray<int, 10, int> b = {1, 2, 3};
        ASSERT(b.Capacity() == 10);
        CircularBufferArray<int, 10, int> c = {};
        ASSERT(c.Capacity() == 10);
        CircularBufferArray<int, 10, int> d = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        ASSERT(d.Capacity() == 10);
    }
}
void CircBufferMethodClear() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 2000);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 2000);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 2000);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 2000);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.Size() == 0);
        ASSERT(a.Capacity() == 10);
    }
}
void CircBufferMethodEmpty() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 2000);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 2000);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 2000);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 2000);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
        a.PushBack(1);
        ASSERT(a.Size() == 1);
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
        a.Clear();
        ASSERT(a.IsEmpty());
        ASSERT(a.Capacity() == 10);
    }
}
void CircBufferMethodIsFull() {
    //StackAllocator
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        ASSERT(a.IsFull());
        a.PopBack();
        ASSERT(!a.IsFull());
        a.EmplaceBack(6);
        ASSERT(a.IsFull());
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a(5);
        ASSERT(a.IsFull());
        a.PopBack();
        ASSERT(!a.IsFull());
        a.EmplaceBack(6);
        ASSERT(a.IsFull());
        ASSERT(a.GetBack() == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.IsFull());
        a.PopBack();
        ASSERT(!a.IsFull());
        a.EmplaceBack(6);
        ASSERT(a.IsFull());
    }
}
void CircBufferSquareOperator() {
    //StackAllocator
    {
        CircularBufferArray<int, 6> a = {1, 2, 3, 4, 5};
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
        a.EmplaceBack(6);
        ASSERT(a[5] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
        a.EmplaceBack(6);
        ASSERT(a[5] == 6);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 6, int> a = {1, 2, 3, 4, 5};
        ASSERT(a[0] == 1);
        ASSERT(a[1] == 2);
        ASSERT(a[2] == 3);
        ASSERT(a[3] == 4);
        ASSERT(a[4] == 5);
        a.EmplaceBack(6);
        ASSERT(a[5] == 6);
    }
}
void CircBufferIteratorOperatorAssignment() {
    //StackAllocator
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        CircularBufferArray<int, 10> a(vec.begin(), vec.end());
        a.PushBack(6);
        vec.push_back(6);
        a.PopFront();
        vec.erase(vec.begin());
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        auto it_end = a.cend();
        for (size_t i = 0; i < vec.size() && it == it_end; ++i, ++it) {
            ASSERT(vec[i] == *it);
        }
    }
    //HeapAllocator
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        CircularBufferArray<int, 2000> a(vec.begin(), vec.end());
        a.PushBack(6);
        vec.push_back(6);
        a.PopFront();
        vec.erase(vec.begin());
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        auto it_end = a.cend();
        for (size_t i = 0; i < vec.size() && it == it_end; ++i, ++it) {
            ASSERT(vec[i] == *it);
        }
    }
    //HeapAllocator
    {
        std::vector<int> vec = {1, 2, 3, 4, 5};
        CircularBufferArray<int, 10, int> a(vec.begin(), vec.end());
        a.PushBack(6);
        vec.push_back(6);
        a.PopFront();
        vec.erase(vec.begin());
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        auto it_end = a.cend();
        for (size_t i = 0; i < vec.size() && it == it_end; ++i, ++it) {
            ASSERT(vec[i] == *it);
        }
    }
}
void CircBufferIteratorOperatorIncrement() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        ASSERT(*it == 1);
        ++it;
        ASSERT(*it == 2);
        ++it;
        ASSERT(*it == 3);
        ++it;
        ASSERT(*it == 4);
        ++it;
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == a.cend());
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        ASSERT(*it == 1);
        ++it;
        ASSERT(*it == 2);
        ++it;
        ASSERT(*it == 3);
        ++it;
        ASSERT(*it == 4);
        ++it;
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == a.cend());
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        ASSERT(a.Size() == 5);
        auto it = a.cbegin();
        ASSERT(*it == 1);
        ++it;
        ASSERT(*it == 2);
        ++it;
        ASSERT(*it == 3);
        ++it;
        ASSERT(*it == 4);
        ++it;
        ASSERT(*it == 5);
        ++it;
        ASSERT(it == a.cend());
    }
}
void CircBufferIteratorOperatorEquality() {
    //StackAllocator
    {
        CircularBufferArray<int, 10> a = {1, 2, 3, 4, 5};
        auto it_one = a.begin();
        auto it_two = a.begin();
        ASSERT(it_one == it_two);
        it_one++;
        ASSERT(it_one != it_two);
        it_two++;
        ASSERT(it_one == it_two);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        auto it_one = a.begin();
        auto it_two = a.begin();
        ASSERT(it_one == it_two);
        it_one++;
        ASSERT(it_one != it_two);
        it_two++;
        ASSERT(it_one == it_two);
    }
    //HeapAllocator
    {
        CircularBufferArray<int, 10, int> a = {1, 2, 3, 4, 5};
        auto it_one = a.begin();
        auto it_two = a.begin();
        ASSERT(it_one == it_two);
        it_one++;
        ASSERT(it_one != it_two);
        it_two++;
        ASSERT(it_one == it_two);
    }
}
void CircBufferIteration() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    {
        CircularBufferArray<int, 5> a = {1, 2, 3, 4, 5};
        auto it_vec = vec.begin();
        for (int i : a) {
            ASSERT(*it_vec == i);
            it_vec++;
        }
    }
    {
        CircularBufferArray<int, 2000> a = {1, 2, 3, 4, 5};
        auto it_vec = vec.begin();
        for (int i : a) {
            ASSERT(*it_vec == i);
            it_vec++;
        }
    }
    {
        CircularBufferArray<int, 5, int> a = {1, 2, 3, 4, 5};
        auto it_vec = vec.begin();
        for (int i : a) {
            ASSERT(*it_vec == i);
            it_vec++;
        }
    }
}
void CircBufferTimeTest() {
    auto StackAllocator_128 = []()
    {
        CircularBufferArray<int, 128> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 128; ++j) {
                a.EmplaceBack(j);
            }
        }
    };
    auto HeapAllocator_128 = []()
    {
        CircularBufferArray<int, 128, int> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 128; ++j) {
                a.EmplaceBack(j);
            }
        }
    };
    auto StackAllocator_1024 = []()
    {
        CircularBufferArray<int, 1024> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 1024; ++j) {
                a.EmplaceBack(j);
            }
        }
    };
    auto HeapAllocatorMax_1025 = []()
    {
        CircularBufferArray<int, 1025> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 1025; ++j) {
                a.EmplaceBack(j);
            }
        }
    };
    auto HeapAllocator_1024 = []()
    {
        CircularBufferArray<int, 1024, int> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 1024; ++j) {
                a.EmplaceBack(j);
            }
        }
    };

    struct LageDataStruct {
        std::variant<double, int> a, b, c, d, f, g, h, i;
    };

    auto StackAllocator_LageDataStruct_32 = []()
    {
        CircularBufferArray<LageDataStruct, 32> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 32; ++j) {
                a.PushBack({j, j, j, j, j, j, j, j});
            }
        }
    };
    auto HeapAllocator_LageDataStruct_32 = []()
    {
        CircularBufferArray<LageDataStruct, 32, int> a;
        for (int i = 0; i < 100000; ++i) {
            for (int j = 0; j < 32; ++j) {
                a.PushBack({j, j, j, j, j, j, j, j});
            }
        }
    };


    TIME_DIF(StackAllocator_128)
    TIME_DIF(HeapAllocator_128)
    TIME_DIF(StackAllocator_1024)
    TIME_DIF(HeapAllocatorMax_1025)
    TIME_DIF(HeapAllocator_1024)
    TIME_DIF(StackAllocator_LageDataStruct_32)
    TIME_DIF(HeapAllocator_LageDataStruct_32)
}
