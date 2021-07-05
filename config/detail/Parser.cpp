#include "Parser.hpp"
#include "../exceptions/ParseException.hpp"

ft::cfg::detail::Parser::Parser(ft::cfg::detail::Node & rootNode)
    : m_RootNode(rootNode)
{
}

void ft::cfg::detail::Parser::SkipWhitespaces(const char* & str)
{
    while(std::isspace(*str))
        ++str;
}

std::string ft::cfg::detail::Parser::ReadWord(const char* & str)
{
    const char* start = str;
    while(*str != '\0' && *str != '#' && *str != ';' && !isspace(*str))
        str++;
    return std::string(start, str - start);
}

void ft::cfg::detail::Parser::load(const std::string& filename)
{
    unsigned lineNo = 0;
    State state = EKey;

    std::ifstream file(filename.c_str());
    if (!file.is_open())
        throw ParseException("Cannot open file: " + filename, 0);

    std::stack<detail::Node*> stack;
    detail::Node* last = NULL;

    stack.push(&m_RootNode);
    // Чтение файла построчно
    while (file.good())
    {
        ++lineNo;
        std::string line;
        std::getline(file, line);
        if (!file.good() && !file.eof())
            throw ParseException("Read error: " + filename, 0);

        const char* str = line.c_str();

        // Разбор прочитанной строчки
        while (true)
        {
            SkipWhitespaces(str);

            if (*str == '\0' || *str == '#')
                break;

            switch (state)
            {
                case EKey:
                {
                    if (*str == '}')
                    {
                        str++;
                        last = stack.top();
                        stack.pop();
                        state = EKey;
                    }
                    else
                    {
                        const std::string key = ReadWord(str);
                        if (key.empty())
                            throw ParseException("Key has zero length", lineNo);
                        last = &stack.top()->children.insert(std::make_pair(key, detail::Node()))->second;
                        state = EValue;
                    }
                    break;
                }
                case EValue:
                {
                    if (*str == ';')
                    {
                        if (last->value_list.empty())
                            throw ParseException("Key with no values", lineNo);
                        str++;
                        state = EKey;
                    }
                    else if (*str == '{')
                    {
                        str++;
                        stack.push(last);
                        last = NULL;
                        state = EKey;
                    }
                    else if (*str == '}')
                        throw ParseException("Value list was not closed with ;", lineNo);
                    else
                    {
                        const std::string value = ReadWord(str);
                        last->value_list.push_back(value);
                    }
                }
            }
        }
    }

    if (stack.size() != 1)
        throw ParseException("Unmatched {", 0);

    if (state == EValue)
        throw ParseException("Value list was not closed with ;", lineNo);
}
