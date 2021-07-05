#ifndef UTIL_SINGLETON_H
#define UTIL_SINGLETON_H

#include "NonCopyable.hpp"
#include <cstddef>

namespace ft { namespace util
{
    template<class T>
    class Singleton: virtual NonCopyable
    {
    public:
        static T & get();

        template<size_t ID>
        static T & get();
    };
} }

template<class T>
T & ft::util::Singleton<T>::get()
{
    static T instance;
    return instance;
}

template<class T>
template<size_t ID>
T & ft::util::Singleton<T>::get()
{
    static T instance;
    return instance;
}

#endif //UTIL_SINGLETON_H
