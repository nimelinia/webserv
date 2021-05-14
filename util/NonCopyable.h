#ifndef UTIL_NONCOPYABLE_H
#define UTIL_NONCOPYABLE_H

namespace ft { namespace util
{
    class NonCopyable
    {
    protected:
        NonCopyable() {}

    private:
        NonCopyable(const NonCopyable &);
        NonCopyable & operator=(const NonCopyable &);
    };
} }

#endif //UTIL_NONCOPYABLE_H
