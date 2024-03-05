#pragma once

#include <stdexcept>
#include <cstdlib>
#include <cassert>
#include <type_traits>
#include <iterator>
#include <new>
#include <cstddef>
#include <tuple>


namespace {
constexpr std::size_t max_stack_size = 4096;

template<typename T, std::enable_if_t<std::is_integral_v<T>, bool> = true>
class CyclicRangeValue {
private:
    T min_value_ = 0;
    T max_value_ = 0;
    T current_value_ = 0;

private:

    CyclicRangeValue GetReductionToZero() const{
        T value = current_value_ - min_value_;
        T max = max_value_ - min_value_;
        return CyclicRangeValue(max, max);
    }

public:

    CyclicRangeValue() = default;

    explicit CyclicRangeValue(T max) : max_value_(max) {
        assert(min_value_ <= max_value_ && "min_value_ <= max_value_");
    }

    explicit CyclicRangeValue(T value, T max) : max_value_(max) {
        assert(min_value_ <= max_value_ && "min_value_ <= max_value_");
        current_value_ = CalculateValue(value);
    }

    explicit CyclicRangeValue(T value, T min, T max) : min_value_(min), max_value_(max) {
        assert(min_value_ <= max_value_ && "min_value_ <= max_value_");
        current_value_ = CalculateValue(value);
    }

    inline T CalculateValue(T value) const {
        if (value > max_value_) {
            T diff = (max_value_ + 1) - min_value_;
            value = min_value_ + (value % diff);
        } else if (min_value_ > value) {
            T diff = (max_value_ + 1) - min_value_;
            value = (max_value_ + 1) - (value % diff);
        }
        return value;
    }

    CyclicRangeValue operator+(T value) const {
        return CyclicRangeValue(current_value_ + value, min_value_, max_value_);
    }

    CyclicRangeValue operator-(T value) const {
        if (std::is_unsigned<T>::value) {
            T diff = (max_value_ + 1) - min_value_;
            return CyclicRangeValue((current_value_ + diff) - CalculateValue(value), min_value_, max_value_);
        }
        return CyclicRangeValue(current_value_ - value, min_value_, max_value_);
    }

    CyclicRangeValue& operator+=(T value) {
        current_value_ = CalculateValue(current_value_ + value);
        return *this;
    }

    CyclicRangeValue& operator-=(T value) {
        if (std::is_unsigned<T>::value) {
            T diff = (max_value_ + 1) - min_value_;
            current_value_ = CalculateValue((current_value_ + diff) - CalculateValue(value));
        } else {
            current_value_ = CalculateValue(current_value_ - value);
        }
        return *this;
    }

    CyclicRangeValue& operator++() {
        current_value_ = CalculateValue(++current_value_);
        return *this;
    }

    CyclicRangeValue operator++(int) {
        CyclicRangeValue temp(*this);
        ++(*this);
        return temp;
    }

    CyclicRangeValue& operator--() {
        if (std::is_unsigned<T>::value) {
            T diff = (max_value_ + 1) - min_value_;
            current_value_ = CalculateValue(current_value_ + diff - 1);
        } else {
            current_value_ = CalculateValue(--current_value_);
        }
        return *this;
    }

    CyclicRangeValue operator--(int) {
        CyclicRangeValue temp(*this);
        --(*this);
        return temp;
    }

    operator T() const {
        return current_value_;
    }

//    bool operator<(T value) const { return current_value_ < CalculateValue(value); }
//    bool operator>(T value) const { return current_value_ > CalculateValue(value); }
//    bool operator<=(T value) const { return current_value_ <= CalculateValue(value); }
//    bool operator>=(T value) const { return current_value_ >= CalculateValue(value); }
//    bool operator==(T value) const { return current_value_ == CalculateValue(value); }
//    bool operator!=(T value) const { return current_value_ != CalculateValue(value); }

    bool operator<(const CyclicRangeValue& rhs) const {
        T current_lhs = GetReductionToZero().current_value_;
        T current_rhs = rhs.GetReductionToZero().current_value_;
        return current_lhs < current_rhs;
    }
    bool operator>(const CyclicRangeValue& rhs) const {
        return rhs < *this;
    }
    bool operator<=(const CyclicRangeValue& rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const CyclicRangeValue& rhs) const {
        return !(*this < rhs);
    }
    bool operator==(const CyclicRangeValue& rhs) const {
        return current_value_ == rhs.current_value_ && min_value_ == rhs.min_value_ && max_value_ == rhs.max_value_;
    }
    bool operator!=(const CyclicRangeValue& rhs) const {
        return !(rhs == *this);
    }

    CyclicRangeValue& SetMin(T min) {
        assert(min <= max_value_ && "min must be <= max_value");
        min_value_ = min;
        if (current_value_ < min_value_) {
            current_value_ = CalculateValue(current_value_);
        }
        return *this;
    }

    CyclicRangeValue& SetMax(T max) {
        assert(max >= min_value_ && "max must be >= min_value");
        max_value_ = max;
        if (current_value_ > max_value_) {
            current_value_ = CalculateValue(current_value_);
        }
        return *this;
    }

    CyclicRangeValue& SetValue(T value) {
        current_value_ = CalculateValue(value);
        return *this;
    }

    CyclicRangeValue& ResetValue() {
        current_value_ = min_value_;
        return *this;
    }

    T GetMin() const { return min_value_; }

    T GetMax() const { return max_value_; }

    T GetValue() const { return current_value_; }

    T Difference() const {
        return max_value_ - min_value_;
    }
};

template<typename T, size_t _Capacity>
class HeapAllocator {
    alignas(std::max_align_t) std::byte* storage;

public:
    HeapAllocator() {
        storage = new std::byte[(sizeof(T) * _Capacity)];
    }
    ~HeapAllocator() {
        delete[] storage;
    }

    T* allocate() noexcept {
        return reinterpret_cast<T*>(storage);
    }
};

template<typename T, size_t _Capacity>
class StackAllocator {
    alignas(std::max_align_t) std::byte storage[sizeof(T) * _Capacity];

public:
    explicit StackAllocator() = default;
    ~StackAllocator() {}

    T* allocate() noexcept {
        return reinterpret_cast<T*>(storage);
    }
};

template<typename T, size_t _Capacity, typename Allocator>
class CircularBufferArrayBase {
    static_assert(_Capacity > 0, "_Capacity must be greater than 0");

protected:
    Allocator allocator_;
    T* buffer_ = nullptr;
    CyclicRangeValue<size_t> range_cursor_ = CyclicRangeValue<size_t>((_Capacity-1));
    size_t fullness_ = 0;

    void Destroy() {
        while (fullness_ > 0) {
            PopBack();
        }
    }

private:

    inline size_t GetCursor() const {
        return range_cursor_.GetValue();
    }

    inline size_t GetStart() const {
        CyclicRangeValue<size_t> tail = range_cursor_;
        tail -= (fullness_ - 1);
        return tail.GetValue();
    }

    template<typename ValueType>
    class const_iterator_base {
        const CircularBufferArrayBase* container_ptr_;
        CyclicRangeValue<size_t> iterator_cursor_;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = const value_type*;
        using reference = const value_type&;

        explicit const_iterator_base(const CircularBufferArrayBase* container, size_t offset = 0) noexcept
            : container_ptr_(container) {
            assert(offset <= container->fullness_ && "Offset must be less than to the fullness.");
            iterator_cursor_ = CyclicRangeValue<size_t>(offset, container->fullness_);
        }

        const_iterator_base(const const_iterator_base& Other)
            : container_ptr_(Other.container_ptr_), iterator_cursor_(Other.iterator_cursor_) {}

        const_iterator_base(const_iterator_base&& Other) noexcept
            : container_ptr_(Other.container_ptr_), iterator_cursor_(Other.iterator_cursor_) {}

        const_iterator_base& operator=(const const_iterator_base& Other) {
            if (this == &Other) return *this;
            container_ptr_ = Other.container_ptr_;
            iterator_cursor_ = Other.iterator_cursor_;
            return *this;
        }

        const_iterator_base& operator=(const_iterator_base&& Other) noexcept {
            if (this == &Other)
                return *this;
            container_ptr_ = Other.container_ptr_;
            iterator_cursor_ = Other.iterator_cursor_;
            return *this;
        }

        reference operator*() const noexcept {
            return *(container_ptr_->buffer_ + iterator_cursor_.GetValue());
        }

        pointer operator->() const noexcept {
            return container_ptr_->buffer_ + iterator_cursor_.GetValue();
        }

        const_iterator_base& operator++() noexcept {
            ++iterator_cursor_;
            return *this;
        }

        const_iterator_base operator++(int) noexcept {
            const_iterator_base Tmp = *this;
            ++*this;
            return Tmp;
        }

        bool operator==(const const_iterator_base& rhs) const {
            return container_ptr_ == rhs.container_ptr_ && iterator_cursor_ == rhs.iterator_cursor_;
        }

        bool operator!=(const const_iterator_base& rhs) const {
            return !(rhs == *this);
        }
    };

    template<typename ValueType>
    class iterator_base : public const_iterator_base<ValueType> {
    public:
        using Super = const_iterator_base<ValueType>;

        using iterator_category = std::forward_iterator_tag;
        using value_type = ValueType;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        using Super::Super;

        reference operator*() const noexcept {
            return const_cast<reference>(Super::operator*());
        }

        pointer operator->() const noexcept {
            return &(const_cast<reference>(Super::operator*()));
        }

        iterator_base& operator++() noexcept {
            Super::operator++();
            return *this;
        }

        iterator_base operator++(int) noexcept {
            iterator_base Tmp = *this;
            Super::operator++();
            return Tmp;
        }
    };

public:

    explicit CircularBufferArrayBase() {
        buffer_ = allocator_.allocate();
    }

    template<typename It>
    CircularBufferArrayBase(It begin, It end) : CircularBufferArrayBase() {
        It iter = begin;
        for (size_t i = 0; i < _Capacity && iter != end; ++i) {
            EmplaceBack(*iter);
            ++iter;
        }
    }

    CircularBufferArrayBase(const std::initializer_list<T>& initializer_list) : CircularBufferArrayBase() {
        auto iter = initializer_list.begin();
        auto it_end = initializer_list.end();
        for (size_t i = 0; i < _Capacity && iter != it_end; ++i) {
            EmplaceBack(*iter);
            ++iter;
        }
    }

    CircularBufferArrayBase(T value) : CircularBufferArrayBase() {
        for (size_t i = 0; i < _Capacity; ++i) {
            EmplaceBack(value);
        }
    }

    virtual ~CircularBufferArrayBase() {
        if (fullness_ == 0) return;
        Destroy();
    }

    using value_type = T;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using reference = value_type&;
    using const_reference = const value_type&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    /**
     *  @brief Emplace element in container on cursor position
     *  @return T*  A pointer to the construct element.
    */
    template<typename... Args>
    inline pointer EmplaceBack(Args&& ... args) {
        if (fullness_ != 0) {
            ++range_cursor_;
        }
        T* cursor_ptr = buffer_ + range_cursor_.GetValue();

        if (fullness_ == _Capacity) {
            cursor_ptr->~T();
        } else {
            ++fullness_;
        }

        new(cursor_ptr) T(std::forward<Args>(args)...);
        return cursor_ptr;
    }

    /**
     *  @brief Push element in container on cursor position
     *  @return T*  A pointer to the copied element.
    */
    template<typename U = T>
    inline pointer PushBack(U&& value) {
        return EmplaceBack(std::forward<T>(value));
    }

    /**
     *  @brief Erase first element in container
     *  A destructor will be called for the erased element.
    */
    inline void PopFront() {
        pointer head_ptr = buffer_ + GetStart();
        head_ptr->~value_type();
        --fullness_;
    }

    /**
     *  @brief Erase last element in container
     *  A destructor will be called for the erased element.
    */
    inline void PopBack() {
        assert(0 < fullness_ && "Fullness must be greater than 0");

        pointer head_ptr = buffer_ + GetCursor();
        head_ptr->~value_type();
        --range_cursor_;
        --fullness_;
    }

    /**
     *  @brief Returns a reference to the element at the front position in the container.
    */
    inline reference GetFront() {
        assert(0 < fullness_ && "Fullness must be greater than 0");
        return *(buffer_ + GetStart());
    }

    /**
     *  @brief Returns a reference to the element at the back position in the container.
    */
    inline reference GetBack() {
        assert(0 < fullness_ && "Fullness must be greater than 0");
        return *(buffer_ + GetCursor());
    }

    /**
     *  @brief Returns a copy of the element at the front position in the container.
     *  Element will be erased from the container
     */
    inline value_type ReleaseFront() {
        value_type temp = GetFront();
        PopFront();
        return std::move(temp);
    }

    /**
     *  @brief Returns a copy of the element at the back position in the container.
     *  Element will be erased from the container
     */
    inline value_type ReleaseBack() {
        value_type temp = GetBack();
        PopBack();
        return std::move(temp);
    }

    /**
     *  @brief Returns count of elements in the container.
    */
    inline size_t Size() const {
        return fullness_;
    }

    /**
     *  @brief Returns Capacity of the container.
    */
    inline size_t Capacity() const {
        return _Capacity;
    }

    /**
     *  @brief Erase all in the container.
    */
    inline void Clear() {
        Destroy();
        range_cursor_ = CyclicRangeValue<size_t>(_Capacity);
    }

    /**
     *  @brief Returns true if the container is IsEmpty.
    */
    inline bool IsEmpty() const {
        return !fullness_;
    }

    /**
     *  @brief Returns true if the container is full.
    */
    inline bool IsFull() const {
        return fullness_ == _Capacity;
    }

    reference operator[](size_t n) {
        assert(0 <= n && n < fullness_ && "n must be less than fullness.");
        CyclicRangeValue<size_t> range_n = range_cursor_;
        range_n -= (fullness_ - 1);
        range_n += n;
        return *(buffer_ + range_n.GetValue());
    }

    using iterator = iterator_base<value_type>;
    using const_iterator = const_iterator_base<value_type>;

    /**
     *  @brief Returns an iterator to the beginning of the container.
     */
    iterator begin() noexcept {
        return iterator(this);
    }

    /**
     *  @brief Returns an iterator to the end of the container.
     */
    iterator end() noexcept {
        return iterator(this, fullness_);
    }

    /**
     *  @brief Returns a const iterator to the beginning of the container.
     */
    const_iterator cbegin() const noexcept {
        return const_iterator(this);
    }

    /**
     *  @brief Returns a const iterator to the end of the container.
     */
    const_iterator cend() const noexcept {
        return const_iterator(this, fullness_);
    }

    /**
     *  @brief Returns a const iterator to the beginning of the container.
     */
    const_iterator begin() const noexcept {
        return cbegin();
    }

    /**
     *  @brief Returns a const iterator to the end of the container.
     */
    const_iterator end() const noexcept {
        return cend();
    }
};
}

/**
 *  @brief Circular Buffer Array
 *  Allocate data in heap
 *  @tparam T Type
 *  @tparam Capacity Container max Size
 *  @tparam IsHeap If "int", it is allocated in the heap regardless of Size
*/
template<typename T, std::size_t _Capacity, typename IsHeap = bool>
class CircularBufferArray : public CircularBufferArrayBase<T, _Capacity, HeapAllocator<T, _Capacity>> {
public:
    static_assert(_Capacity != 0, "_Capacity must be greater than 0");

    using Super = CircularBufferArrayBase<T, _Capacity, HeapAllocator<T, _Capacity>>;
    using Super::Super;
};

/**
 *  @brief Circular Buffer Array
 *  Allocate data in stack
 *  Max stack Size = 1024 byte
 *  @tparam T Type
 *  @tparam Capacity Container max Size
*/
template<typename T, std::size_t _Capacity>
class CircularBufferArray<T, _Capacity, std::enable_if_t<(0 < _Capacity && (_Capacity * sizeof(T)) <= max_stack_size), bool>>
    : public CircularBufferArrayBase<T, _Capacity, StackAllocator<T, _Capacity>> {

public:
    using Super = CircularBufferArrayBase<T, _Capacity, StackAllocator<T, _Capacity>>;
    using Super::Super;
};


namespace {

}