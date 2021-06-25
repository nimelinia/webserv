#ifndef CONFIG_NODE_H
#define CONFIG_NODE_H

#include "../../AllowedStd.h"

namespace ft { namespace cfg { namespace detail
{
    struct Node
    {
        std::list<std::string> value_list;
        std::multimap<std::string, Node> children;
    };
} } }

#endif //CONFIG_NODE_H
