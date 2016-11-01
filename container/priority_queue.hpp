#pragma once

#include <functional>
#include "vector.hpp"

namespace mrsuyi
{
template <class T, class Container = vector<T>,
          class Compare = std::less<typename Container::value_type>>
class priority_queue
{
public:
    using container_type = Container;
    using value_compare = Compare;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;
    using reference = typename Container::reference;
    using const_reference = typename Container::const_reference;

    // ctor & dtor
    // default
    priority_queue(const Compare&, const Container&);
    explicit priority_queue(const Compare& = Compare(),
                            Container&& = Container());
    // copy
    priority_queue(const priority_queue&);
    // move
    priority_queue(priority_queue&&);
    // from alloc
    template <class Alloc>
    priority_queue(const Alloc&);
    template <class Alloc>
    priority_queue(const Compare&, const Alloc&);
    template <class Alloc>
    priority_queue(const Compare&, const Container& cont, const Alloc& alloc);
    template <class Alloc>
    priority_queue(const Compare&, Container&&, const Alloc& alloc);
    // range
    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare&,
                   const Container&);
    template <class InputIt>
    priority_queue(InputIt first, InputIt last, const Compare& = Compare(),
                   Container&& = Container());
    // ~
    ~priority_queue();
    // =
    priority_queue& operator = (const priority_queue&);
    priority_queue& operator = (priority_queue&&);

    // element access
    const_reference top() const; 

    // capacity
    bool empty() const;
    size_type size() const;

    // modifiers
    void push(const value_type& value);    
    void push(value_type&& value);

    template <class... Args>
    void emplace(Args&&... args);

    void pop();

    void swap(priority_queue& other);

protected:
    Container cont_;
    Compare cmp_;
};


}
