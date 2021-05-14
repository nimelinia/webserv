#include "FileHandler.h"

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
    m_File.write(m_Format(msg));
    m_File.write("\n");
}

bool ft::log::FileHandler::open(const std::string & filename)
{
    m_File.setFileName(filename);
    return m_File.open(util::File::EWriteOnly);
}

const std::string & ft::log::FileHandler::errorString() const
{
    return m_File.errorString();
}
