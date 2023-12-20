#pragma once
            
                            // VECTOR IMPLEMENTATION
//CONSTRUCTORS
template <typename Type, typename Allocator>
Vector<Type, Allocator>& Vector<Type, Allocator>::operator=(Vector&& other)
{
    if (this == &other)
        return *this;

    if (!AllocatorTraits::propagate_on_container_move_assignment::value && (allocator != other.allocator))
    {
        pointer new_start = AllocatorTraits::allocate(allocator, other.Capacity());
        pointer new_finish = new_start;
        pointer new_end_of_storage_ = new_start + other.Capacity();
        try
        {
            new_finish = UninitializedMove(other.start_, other.finish_, new_start);

        } catch(...) {
            AllocatorTraits::deallocate(allocator, new_start, other.Capacity());
            throw;
        }

        Clear();
        AllocatorTraits::deallocate(allocator, start_, Capacity());

        start_ = new_start;
        finish_ = new_finish;
        end_of_storage_ = new_end_of_storage_;

    } else {
        Vector<Type, Allocator> copy(std::move(other), other.allocator);
        FullSwap(copy);
    }

    return *this;
}

template <typename Type, typename Allocator>
Vector<Type, Allocator>& Vector<Type, Allocator>::operator=(const Vector<Type, Allocator>& other)
{
    if (this == &other)
        return *this;

    Allocator new_allocator = allocator;
    if (AllocatorTraits::propogate_on_container_copy_assignment::value && allocator != other.allocator)
    {
        new_allocator = other.allocator;
    }

    Vector<Type, Allocator> copy(other, new_allocator);
    FullSwap(copy);
    return *this;
}

template <typename Type, typename Allocator>
Vector<Type, Allocator>& Vector<Type, Allocator>::operator=(std::initializer_list<Type> init)
{
    Vector<Type, Allocator> copy(init.begin(), init.end(), allocator);
    FullSwap(copy);

    return *this;
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::Assign(size_t count, const Type& value)
{
    Vector<Type, Allocator> copy(count, value, allocator);
    FullSwap(copy);
}

template <typename Type, typename Allocator>
template <typename InputIt, typename>
void Vector<Type, Allocator>::Assign(InputIt first, InputIt last)
{
    Vector<Type, Allocator> copy(first, last, allocator);
    FullSwap(copy);
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::Assign(std::initializer_list<Type> init)
{
    Vector<Type, Allocator> copy(init.begin(), init.end(), allocator);
    FullSwap(copy);
}

//ELEMENT ACCESS
template <typename Type, typename Allocator>
typename Vector<Type, Allocator>::reference Vector<Type, Allocator>::at (size_t index) 
{
    if ((index >= Size()))
    {
        throw std::out_of_range("Invalid index");
    }

    return *(start_ + index);
}

template <typename Type, typename Allocator>
typename Vector<Type, Allocator>::const_reference Vector<Type, Allocator>::at (size_t index) const
{
    if ((index >= Size()))
    {
        throw std::out_of_range("Invalid index");
    }

    return *(start_ + index);
}

//CAPACITY
template <typename Type, typename Allocator>
void Vector<Type, Allocator>::Reserve(size_t new_cap)
{
    if (new_cap > MaxSize())
    {
        throw std::length_error("Cannot hold so big storage");
    }
    if (new_cap <= Capacity()) 
    {
        return;
    }

    pointer new_start = AllocatorTraits::allocate(allocator, new_cap);
    pointer new_end_of_storage = new_start + new_cap;
    pointer new_finish = nullptr;

    try
    {
        new_finish = UninitializedMove(start_, finish_, new_start);

    } catch(...){
        AllocatorTraits::deallocate(allocator, new_start, new_cap);
        throw;
    }

    Clear();
    AllocatorTraits::deallocate(allocator, start_, Capacity());

    start_ = new_start;
    finish_ = new_finish;
    end_of_storage_ = new_end_of_storage;
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::ShrinkToFit()
{
    Vector<Type, Allocator> copy(allocator);
    copy.Reserve(Size());
    copy.finish_ = UninitializedMove(start_, finish_, copy.start_);
    FullSwap(copy);
}

//MODIFIERS
template <typename Type, typename Allocator>
void Vector<Type, Allocator>::Clear() noexcept
{
    for (pointer it = start_ ; it != finish_; ++it)
    {
        AllocatorTraits::destroy(allocator, it);
    }
    finish_ = start_;
}

template <typename Type, typename Allocator>
typename Vector<Type, Allocator>::Iterator Vector<Type, Allocator>::Insert(ConstIterator position, size_t n, const Type& value)
{
    if (n == 0)
        return Iterator(position.element);

    const size_t pos_index = (position.element - start_);

    if (n > (end_of_storage_ - finish_))
    {
        const size_t length = check_length(n, "Vector::Insert");
        Reserve(length);
        position.element = start_ + pos_index;
    }

    size_t elements_after = (finish_ - position.element);
    pointer old_finish = finish_;

    if (elements_after > n)
    {
        finish_ = UninitializedMove(finish_ - n, finish_, finish_);

        for (pointer i = (old_finish - n - 1); i > position.element; --i)
        {
            std::swap(*i, *(i + n));
        }          

        std::fill(position.element, position.element + n, value);

    } else {
        UninitializedFill(finish_, finish_ + (n - elements_after), value);
        finish_ += (n - elements_after);
        finish_ = UninitializedMove(position.element, old_finish, finish_);
        std::fill(position.element, old_finish, value);
    }

    return Iterator(start_ + pos_index);
}    

template <typename Type, typename Allocator>
template <typename InputIt, typename>
typename Vector<Type, Allocator>::Iterator Vector<Type, Allocator>::Insert(ConstIterator position, InputIt first, InputIt last)
{
    const size_t pos_index = (position.element - start_);
    const size_t n = (last - first);
    size_t elements_after = (finish_ - position.element);

    if (!n)
    {
        return Iterator(position.element);
    }

    if (n > (end_of_storage_ - finish_))
    {
        const size_t length = check_length(n, "Vector::Insert");
        Reserve(length);
        position.element = start_ + pos_index;
    }

    pointer old_finish = finish_;

    if (elements_after > n)
    {
        finish_ = UninitializedMove(finish_ - n, finish_, finish_);

        for (pointer i = (old_finish - n - 1); i > position.element; --i)
        {
            std::swap(*i, *(i + n));
        }          

        for (pointer i = position.element; i != position.element + n; ++i, ++first)
        {
            *i = *first;
        }

    } else {
        UninitializedCopy(finish_, finish_ + (n - elements_after), first + elements_after);
        finish_ += (n - elements_after);
        finish_ = UninitializedMove(position.element, old_finish, finish_);

        for (pointer i = position.element; i != old_finish; ++i, ++first)
        {
            *i = *first;
        }
    }

    return Iterator(start_ + pos_index);
}

template <typename Type, typename Allocator>
template <typename... Args>
typename Vector<Type, Allocator>::Iterator Vector<Type, Allocator>::Emplace(ConstIterator position, Args&&... args)
{
    const size_t elements_before = (position - cbegin());

    if (Size() >= Capacity())
    {
        const size_t length = check_length(1, "Vector::Insert");
        Vector<Type, Allocator> copy(allocator);
        copy.Reserve(length);
        
        AllocatorTraits::construct(copy.allocator, copy.start_ + elements_before, std::forward<Args>(args)...);

        try
        {
            copy.finish_ = UninitializedMove(start_, position.element, copy.start_);
            copy.finish_ = UninitializedMove(position.element, finish_, copy.finish_ + 1);

        } catch (...) {
            AllocatorTraits::destroy(copy.allocator, start_ + elements_before);
            copy.finish_ = nullptr;
            throw;
        }

        FullSwap(copy);

    } else {
        AllocatorTraits::construct(allocator, finish_, std::forward<Args>(args)...);

        for (pointer i = finish_; i != position.element; --i)
        {
            pointer prev = i - 1;
            std::swap(*i, *prev);
        }

        ++finish_;
    }

    return start_ + elements_before;
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::Resize(size_t count, const Type& value)
{
    if (count > Capacity())
    {
        Vector<Type, Allocator> copy(allocator);
        copy.Reserve(count);

        UninitializedFill(copy.start_ + Size(), copy.end_of_storage_, value);
        UninitializedMove(start_, finish_, copy.start_);
        copy.finish_ = copy.start_ + count;

        FullSwap(copy);
        return;
    }

    if (count > Size())
    {
        UninitializedFill(finish_, start_ + count, value);
        finish_ = start_ + count;
    }

    if (count < Size())
    {
        for (pointer j = start_ + count; j != finish_; ++j)
        {
            AllocatorTraits::destroy(allocator, j);
        }
        finish_ = start_ + count;
    }
}

template <typename Type, typename Allocator>
template <typename... Args>
void Vector<Type, Allocator>::EmplaceBack(Args&&... args)
{
    if (Size() >= Capacity())
    {
        const size_t length = check_length(1, "Vector::EmplaceBack");
        Vector<Type, Allocator> copy;
        copy.Reserve(length);
        copy.finish_ = nullptr;

        AllocatorTraits::construct(copy.allocator, copy.start_ + Size(), std::forward<Args>(args)...);
        try
        {
            copy.finish_ = UninitializedMove(start_, finish_, copy.start_);
            ++copy.finish_;

        } catch(...) {
            AllocatorTraits::destroy(copy.allocator, copy.start_ + Size());
            copy.finish_ = nullptr;
            throw;
        }
        
        FullSwap(copy);
        return;
    }

    AllocatorTraits::construct(allocator, finish_, std::forward<Args>(args)...);
    ++finish_;
}

template <typename Type, typename Allocator>
typename Vector<Type, Allocator>::Iterator Vector<Type, Allocator>::Erase(ConstIterator position) 
{
    for (pointer i = position.element + 1; i != finish_; ++i)
    {
        std::swap(*i, *(i - 1));
    }
    AllocatorTraits::destroy(allocator, finish_ - 1);
    --finish_;

    return Iterator(position.element);
}

template <typename Type, typename Allocator>
typename Vector<Type, Allocator>::Iterator Vector<Type, Allocator>::Erase(ConstIterator first, ConstIterator last)
{
    size_t delete_area = (last - first);
    size_t elements_after = (finish_ - last);

    for (pointer i = last.element; i != finish_; ++i)
    {
        std::swap(*i, *(i - delete_area));
    }

    for (pointer i = first.element + elements_after; i != finish_; ++i)
    {
        AllocatorTraits::destroy(allocator, i);
    }
    finish_ -= delete_area;

    return Iterator(first.element);
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::PopBack()
{
    AllocatorTraits::destroy(allocator, finish_- 1);
    --finish_;
}

template <typename Type, typename Allocator>
void Vector<Type, Allocator>::swap(Vector<Type, Allocator>& other)
{
    std::swap(start_, other.start_);
    std::swap(finish_, other.finish_);
    std::swap(end_of_storage_, other.end_of_storage_);
    if (AllocatorTraits::propagate_on_container_swap::value)
    {
        std::swap(allocator, other.allocator);
    }
}

//FUNCTIONS
template <typename Type, typename Allocator>
void Vector<Type, Allocator>::FullSwap(Vector<Type, Allocator>& other)
{
    std::swap(start_, other.start_);
    std::swap(finish_, other.finish_);
    std::swap(end_of_storage_, other.end_of_storage_);
    std::swap(allocator, other.allocator);
}

template <typename Type, typename Allocator>
size_t Vector<Type, Allocator>::check_length(size_t n, const char* error_message)
{
    if ((MaxSize() - Size()) < n)
    {
        throw std::length_error(error_message);
    }

    const size_t length = Size() + (std::max)(Size(), n);

    return (length < Size() || length > MaxSize()) ? MaxSize() : length;
}

template <typename Type, typename Allocator>
template<class ForwardIt>
void Vector<Type, Allocator>::UninitializedFill(ForwardIt first, ForwardIt last, const Type& value)
{
    ForwardIt current = first;
    try
    {
        for ( ; current != last; ++current)
        {
            AllocatorTraits::construct(allocator, std::addressof(*current), value);
        }
    } 
    catch (...)
    {
        for ( ; first != current; ++first)
        {
            AllocatorTraits::destroy(allocator, std::addressof(*first));
        }
        throw;
    }
}

template <typename Type, typename Allocator>
template <typename InputIt, typename NoThrowForwardIt>
NoThrowForwardIt Vector<Type, Allocator>::UninitializedCopy(InputIt first, InputIt last, NoThrowForwardIt result)
{
    NoThrowForwardIt current = result;
    try
    {
        for ( ; first != last; ++first, ++current)
        {
            AllocatorTraits::construct(allocator, std::addressof(*current), *first);
        }
        return current;
    }
    catch (...)
    {
        for (; result != current; ++result)
        {
            AllocatorTraits::destroy(allocator, std::addressof(*result));
        }
        throw;
    }
}

template <typename Type, typename Allocator>
template<class InputIt, class NoThrowForwardIt>
NoThrowForwardIt Vector<Type, Allocator>::UninitializedMove(InputIt first, InputIt last, NoThrowForwardIt d_first)
{
    NoThrowForwardIt current = d_first;
    try
    {
        for ( ; first != last; ++first, ++current)
        {
            AllocatorTraits::construct(allocator, (std::addressof(*current)), std::move(*first));
        }
        return current;
    }
    catch (...)
    {
        for ( ; d_first != current; ++d_first)
        {
            AllocatorTraits::destroy(allocator, std::addressof(*d_first));
        }
        throw;
    }
}

//NON-MEMBER FUNCTIONS
template <typename T>
void std::swap(Vector<T>& lhs, Vector<T>& rhs)
{
    lhs.swap(rhs);
}

template <typename T>
bool operator==(const Vector<T>& lhs, const Vector<T>& rhs)
{
    if (&lhs == &rhs)
    {
        return true;
    }

    return (lhs.Size() == rhs.Size()) ? std::equal(lhs.begin(), lhs.end(), rhs.begin()) : false;
}

template <typename T>
bool operator!=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs == rhs);
}

template <typename T>
bool operator<(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename T>
bool operator>=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(lhs < rhs);
}

template <typename T>
bool operator>(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return rhs < lhs;
}

template <typename T>
bool operator<=(const Vector<T>& lhs, const Vector<T>& rhs)
{
    return !(rhs < lhs);
}