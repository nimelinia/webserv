#include "ConsoleHandler.hpp"

ft::log::ConsoleHandler::ConsoleHandler(ft::log::FormatFunc func)
        :AHandler(func)
{
}

void ft::log::ConsoleHandler::write(const detail::Message & msg)
{
    std::cout << m_Format(msg) << std::endl;
}