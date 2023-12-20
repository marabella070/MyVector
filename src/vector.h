#pragma once

#include <initializer_list>
#include <cassert>
#include <algorithm>
#include <iterator>
#include <memory>

template <typename Type, typename Allocator = std::allocator<Type>>
class Vector
{
private:
    Allocator allocator = Allocator();
    Type* start_ = nullptr;
    Type* finish_ = nullptr;
    Type* end_of_storage_ = nullptr;
public:
    //Vector names
    using reference = Type&;
    using const_reference = const Type&;
    using pointer = Type*;
    using const_pointer = const Type*;
    using AllocatorTraits = std::allocator_traits<Allocator>;

    template <typename ValueType>
    class VectorIterator
    {
    public:
        //Iterator names
        using self = VectorIterator<ValueType>;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
    public:
        //Iterator Constructors
        VectorIterator() = default;
        VectorIterator(const VectorIterator<Type>& other) noexcept { element = other.element; }
        self& operator= (const self& other) noexcept = default;
    private:
        VectorIterator(Type* ptr) 
            : element(ptr) {}
    public:
        //Iterator Comparison operators
        [[nodiscard]] bool operator==(const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator!=(const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator==(const VectorIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator!=(const VectorIterator<Type>& other) const noexcept;

        [[nodiscard]] bool operator< (const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator> (const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator<= (const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator>= (const VectorIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator< (const VectorIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator> (const VectorIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator<= (const VectorIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator>= (const VectorIterator<Type>& other) const noexcept;

    public:
        //Iterator Arithmetic operations
        self& operator+=(const int n) noexcept;
        self& operator-=(const int n) noexcept;

        self& operator++() noexcept;
        self operator++(int) noexcept;
        self& operator--() noexcept;
        self operator--(int) noexcept;

        self operator+(int n) const noexcept;
        self operator-(int n) const noexcept;

        //lhs - rhs

        friend self operator+ (int n, const self& other) noexcept { return (other + n); }
        friend difference_type operator- (const self& lhs, const self& rhs) { return (lhs.element - rhs.element); }

        //Iterator Access operations
        [[nodiscard]] reference operator*() const noexcept;
        [[nodiscard]] pointer operator->() const noexcept;
        [[nodiscard]] reference operator[](int n) const noexcept;

    private:
        friend class Vector;
        Type* element = nullptr;
    };

    template <typename ValueType>
    class VectorReverseIterator
    {
            public:
        //Iterator names
        using self = VectorReverseIterator<ValueType>;
        using iterator_category = std::random_access_iterator_tag;
        using value_type = Type;
        using difference_type = std::ptrdiff_t;
        using pointer = ValueType*;
        using reference = ValueType&;
    public:
        //Iterator Constructors
        VectorReverseIterator() = default;
        VectorReverseIterator(const VectorReverseIterator<Type>& other) noexcept { element = other.element; }
        self& operator= (const self& other) noexcept = default;
    private:
        VectorReverseIterator(Type* ptr) 
            : element(ptr) {}
    public:
        //Iterator Comparison operators
        [[nodiscard]] bool operator==(const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator!=(const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator==(const VectorReverseIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator!=(const VectorReverseIterator<Type>& other) const noexcept;

        [[nodiscard]] bool operator< (const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator> (const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator<= (const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator>= (const VectorReverseIterator<const Type>& other) const noexcept;
        [[nodiscard]] bool operator< (const VectorReverseIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator> (const VectorReverseIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator<= (const VectorReverseIterator<Type>& other) const noexcept;
        [[nodiscard]] bool operator>= (const VectorReverseIterator<Type>& other) const noexcept;

    public:
        //Iterator Arithmetic operations
        self& operator+=(const int n) noexcept;
        self& operator-=(const int n) noexcept;

        self& operator++() noexcept;
        self operator++(int) noexcept;
        self& operator--() noexcept;
        self operator--(int) noexcept;

        self operator+(int n) const noexcept;
        self operator-(int n) const noexcept;

        //lhs - rhs

        friend self operator+ (int n, const self& other) noexcept { return (other + n); }
        friend difference_type operator- (const self& lhs, const self& rhs) { return (rhs.element - lhs.element); }

        //Iterator Access operations
        [[nodiscard]] reference operator*() const noexcept;
        [[nodiscard]] pointer operator->() const noexcept;
        [[nodiscard]] reference operator[](int n) const noexcept;

    private:
        friend class Vector;
        Type* element = nullptr;
    };

public:
    //ITERATORS
    using Iterator = VectorIterator<Type>;
    using ConstIterator = VectorIterator<const Type>;

    [[nodiscard]] Iterator begin() noexcept { return Iterator(start_); }
    [[nodiscard]] ConstIterator cbegin() const noexcept { return ConstIterator(start_); }
    [[nodiscard]] ConstIterator begin() const noexcept { return cbegin(); }

    [[nodiscard]] Iterator end() noexcept { return Iterator(finish_); }
    [[nodiscard]] ConstIterator cend() const noexcept { return ConstIterator(finish_); }
    [[nodiscard]] ConstIterator end() const noexcept { return cend(); }

    using ReverseIterator = VectorReverseIterator<Type>;
    using ConstReverseIterator = VectorReverseIterator<const Type>;

    [[nodiscard]] ReverseIterator rbegin() noexcept { return ReverseIterator(end_of_storage_ - 1); }
    [[nodiscard]] ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(end_of_storage_ - 1); }
    [[nodiscard]] ConstReverseIterator rbegin() const noexcept { return crbegin(); }

    [[nodiscard]] ReverseIterator rend() noexcept { return ReverseIterator(start_ - 1); }
    [[nodiscard]] ConstReverseIterator crend() const noexcept { return ConstReverseIterator(start_ - 1); }
    [[nodiscard]] ConstReverseIterator rend() const noexcept { return crend(); }

    //VECTOR CONSTRUCTORS
    Vector() noexcept = default;

    Vector(const Allocator& alloc) : allocator(alloc) {}

    Vector(Vector&& other, const Allocator& alloc) 
        : start_(other.start_), 
        finish_(other.finish_), 
        end_of_storage_(other.end_of_storage_),
        allocator(alloc)
    {
        other.start_ = other.finish_ = other.end_of_storage_ = nullptr;
    }

    Vector(Vector&& other)
        : start_(other.start_), 
        finish_(other.finish_), 
        end_of_storage_(other.end_of_storage_),
        allocator(Allocator())
    {
        if (AllocatorTraits::propagate_on_container_move_assignment::value)
        {
            allocator = other.allocator;
        }
        other.start_ = other.finish_ = other.end_of_storage_ = nullptr;
    }

    Vector(size_t count, const Type& value = Type(), const Allocator& alloc = Allocator()) : allocator(alloc)
    {
        Reserve(count);
        try
        {
            UninitializedFill(start_, end_of_storage_, value);
            finish_ = start_ + count;

        } catch(...) {
            AllocatorTraits::deallocate(allocator, start_, count);
            throw;
        }
    }

    template <typename InputIt, typename = std::enable_if<!std::is_same<typename std::iterator_traits<InputIt>::value_type, void>::value>>
    Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator()) : allocator(alloc)
    {
        Reserve(last - first);
        try
        {
            finish_ = UninitializedCopy(first, last, start_);

        } catch(...) {
            AllocatorTraits::deallocate(allocator, start_, last - first);
            throw;
        }
    }

    Vector<Type, Allocator>(const Vector<Type, Allocator>& other) 
    : Vector<Type, Allocator>(other, AllocatorTraits::select_on_container_copy_construction(other.allocator)) {}

    Vector<Type, Allocator>(const Vector<Type, Allocator>& other, const Allocator& alloc) : allocator(alloc)
    {
        Reserve(other.Capacity());
        try
        {
            finish_ = UninitializedCopy(other.start_, other.finish_, start_);

        } catch(...) {
            AllocatorTraits::deallocate(allocator, start_, other.Capacity());
            throw;
        }
    }

    Vector(std::initializer_list<Type> init, const Allocator& allocator = Allocator()) 
        : Vector(init.begin(), init.end(), allocator) {}

    ~Vector()
    {
        Clear();
        AllocatorTraits::deallocate(allocator, start_, Capacity());
    }

    Vector& operator=(Vector&& other);
    Vector<Type, Allocator>& operator=(const Vector<Type, Allocator>& other);
    Vector& operator=(std::initializer_list<Type> init);

    template <typename InputIt, typename = std::enable_if<!std::is_same<typename std::iterator_traits<InputIt>::value_type, void>::value>>
    void Assign(InputIt first, InputIt last);

    void Assign(size_t count, const Type& value);
    void Assign(std::initializer_list<Type> init);

    Allocator GetAllocator() const noexcept { return allocator; };

    //VECTOR ELEMENT ACCESS
    reference at (size_t index);
    const_reference at (size_t index) const;

    reference operator[] (size_t index) noexcept { return *(start_ + index); }
    const_reference operator[] (size_t index) const noexcept { return *(start_ + index); }

    reference front() { return *start_; }
    const_reference front() const { return *start_; }

    reference back() { return *(finish_ - 1); }
    const_reference back() const { return *(finish_ - 1); }

    pointer Data() { return start_; }
    const_pointer Data() const { return start_; }

    //VECTOR CAPACITY
    [[nodiscard]] bool empty() const noexcept { return !start_; }
    [[nodiscard]] size_t Size() const noexcept { return (finish_ - start_); } 
    [[nodiscard]] size_t MaxSize() const noexcept { return allocator.max_size(); }
    [[nodiscard]] size_t Capacity() const noexcept { return (end_of_storage_ - start_); }
    void Reserve(size_t new_cap);
    void ShrinkToFit();

    //VECTOR MODIFFIERS
    template <typename... Args>
    void EmplaceBack(Args&&... args);

    template <typename InputIt, typename = std::enable_if<!std::is_same<typename std::iterator_traits<InputIt>::value_type, void>::value>>
    Iterator Insert(ConstIterator position, InputIt first, InputIt last);

    template <typename... Args>
    Iterator Emplace(ConstIterator position, Args&&... args);

    Iterator Insert(ConstIterator position, const Type& value) { return Emplace(position, value); }
    Iterator Insert(ConstIterator position, Type&& value) { return Emplace(position, std::move(value)); }
    Iterator Insert(ConstIterator position, size_t n, const Type& value);
    Iterator Insert(ConstIterator position, std::initializer_list<Type> init) { return Insert(init.begin(), init.end()); }
    void Resize(size_t count, const Type& value = Type());
    void PushBack(const Type& value) { EmplaceBack(value); }
    void PushBack(Type&& value) { EmplaceBack(std::move(value)); }
    Iterator Erase(ConstIterator position);
    Iterator Erase(ConstIterator first, ConstIterator last);
    void PopBack();
    void Clear() noexcept;
    void swap(Vector<Type, Allocator>& other);

private:
    template<class ForwardIt>
    void UninitializedFill(ForwardIt first, ForwardIt last, const Type& value);

    template <typename InputIt, typename NoThrowForwardIt>
    NoThrowForwardIt UninitializedCopy(InputIt first, InputIt last, NoThrowForwardIt result);

    template<class InputIt, class NoThrowForwardIt>
    NoThrowForwardIt UninitializedMove(InputIt first, InputIt last, NoThrowForwardIt d_first);

    size_t check_length(size_t n, const char* error_message);
    void FullSwap(Vector<Type, Allocator>& other);

};

#include "vector_impl.h"
#include "Iterator.h"
#include "ReverseIterator.h"