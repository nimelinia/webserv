#include "ColorConsoleHandler.hpp"

ft::log::ColorConsoleHandler::ColorConsoleHandler(FormatFunc func)
    : AHandler(func)
{
}

void ft::log::ColorConsoleHandler::write(const detail::Message & msg)
{
    SetColor(msg);
    std::cout << m_Format(msg);
    ResetColor(msg);
    std::cout << std::endl;
}

void ft::log::ColorConsoleHandler::SetColor(const detail::Message & msg)
{
    switch (msg.level())
    {
        case ft::log::EFatal:
            std::cout << "\x1B[90m\x1B[41m"; // black on red background
            break;
        case ft::log::EError:
            std::cout << "\x1B[91m"; // red
            break;
        case ft::log::EWarning:
            std::cout << "\x1B[93m"; // yellow
            break;
        case ft::log::EInfo:
            std::cout << "\x1B[96m"; // cyan
            break;
        default:
            break;
    }
}

void ft::log::ColorConsoleHandler::ResetColor(const detail::Message & msg)
{
    if (msg.level() != ft::log::EDebug)
        std::cout << "\033[0m\033[0K" << std::flush;
}