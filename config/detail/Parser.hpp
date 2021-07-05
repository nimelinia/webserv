#ifndef CONFIG_PARSER_H
#define CONFIG_PARSER_H

#include "../../AllowedStd.hpp"
#include "Node.hpp"

namespace ft { namespace cfg { namespace detail
{
    class Parser
    {
    private:
        enum State
        {
            EKey,
            EValue
        };

    private:
        detail::Node & m_RootNode;

    public:
        explicit Parser(detail::Node & rootNode);

        void load(const std::string & filename);

    private:
        static void SkipWhitespaces(const char* & str);
        static std::string ReadWord(const char* & str);
    };
} } }

#endif //CONFIG_PARSER_H
