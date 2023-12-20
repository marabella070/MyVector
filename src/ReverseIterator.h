#pragma once
                                        //Reverse Iterator implementation
//Iterator Comparison operators
template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator==(const VectorReverseIterator<const Type>& other) const noexcept
{
    return element == other.element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator!=(const VectorReverseIterator<const Type>& other) const noexcept
{
    return !(*this == other);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator==(const VectorReverseIterator<Type>& other) const noexcept
{
    return element == other.element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator!=(const VectorReverseIterator<Type>& other) const noexcept
{
    return !(*this == other);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator< (const VectorReverseIterator<const Type>& other) const noexcept
{
    return element > other.element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator> (const VectorReverseIterator<const Type>& other) const noexcept
{
    return (other < *this);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator<= (const VectorReverseIterator<const Type>& other) const noexcept
{
    return !(other < *this);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator>= (const VectorReverseIterator<const Type>& other) const noexcept
{
    return !(*this < other);
}


template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator< (const VectorReverseIterator<Type>& other) const noexcept
{
    return element < other.element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator> (const VectorReverseIterator<Type>& other) const noexcept
{
    return (other < *this);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator<= (const VectorReverseIterator<Type>& other) const noexcept
{
    return !(other < *this);
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] bool Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator>= (const VectorReverseIterator<Type>& other) const noexcept
{
    return !(*this < other);
}

//Iterator Arithmetic operations
template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType>& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator+=(const int n) noexcept
{
    assert(element != nullptr);
    // assert((element + n) < (data + size));
    // assert(data <= (element + n));

    element -= n;
    return *this; 
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType>& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator-=(const int n) noexcept
{
    return (*this += (-n));
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType>& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator++() noexcept
{
    // assert(size > 0);
    assert(element != nullptr);
    // assert((element + 1) < (data + size));

    --element;
    return *this;
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType> Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator++(int) noexcept
{
    Type* buffer = element;
    ++(*this);

    return {buffer};
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType>& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator--() noexcept
{
    assert(element != nullptr);
    // assert(data < element);

    ++element;
    return *this;
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType> Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator--(int) noexcept
{
    Type* buffer = element;
    --(*this);

    return {buffer};
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType> Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator+(int n) const noexcept
{
    self buffer(element);
    return (buffer += n);
}

template <typename Type, typename Allocator>
template <typename ValueType>
Vector<Type, Allocator>::VectorReverseIterator<ValueType> Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator-(int n) const noexcept
{
    self buffer(element);
    return (buffer += (-n));
}

//Iterator Access operations
template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] ValueType& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator*() const noexcept
{
    assert(element != nullptr);
    return *element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] ValueType* Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator->() const noexcept
{
    assert(element != nullptr);
    return element;
}

template <typename Type, typename Allocator>
template <typename ValueType>
[[nodiscard]] ValueType& Vector<Type, Allocator>::VectorReverseIterator<ValueType>::operator[](int n) const noexcept
{
    assert(element != nullptr);

    return *(element - n);
} 