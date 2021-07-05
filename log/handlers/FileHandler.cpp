#include "FileHandler.hpp"

ft::log::FileHandler::FileHandler(FormatFunc func)
    : AHandler(func)
{
}

ft::log::FileHandler::~FileHandler()
{
    m_File.close();
}

void ft::log::FileHandler::write(const detail::Message & msg)
{
    m_File << m_Format(msg) << std::endl;
}

bool ft::log::FileHandler::open(const std::string & filename)
{
    m_File.open(filename.c_str());
    return m_File.is_open();
}
