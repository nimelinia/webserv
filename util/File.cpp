#include "File.hpp"

#include "../AllowedStd.hpp"

ft::util::File::File()
{
    _init();
}

ft::util::File::File(const std::string & filename)
{
    _init();
    m_Filename = filename;
}

ft::util::File::~File()
{
    close();
}

void ft::util::File::setFileName(const std::string & filename)
{
    m_Filename = filename;
}

bool ft::util::File::open(ft::util::File::EMode mode)
{
    int oflag = mode;
    mode_t mask = 0;
    if (mode != EReadOnly)
    {
        oflag |= O_CREAT;
        mask = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
    }
    m_Handle = ::open(m_Filename.c_str(), oflag, mask);
    if (m_Handle == -1)
        _onError();
    if (mode != EWriteOnly)
        m_Buffer = new char[BUF_SIZE];
    return m_Handle != -1;
}

void ft::util::File::close()
{
    if (!isOpen())
        return;

    delete[] m_Buffer;
    if (m_Handle != -1)
        ::close(m_Handle);
    _init();
}

bool ft::util::File::isOpen() const
{
    return m_Handle != -1;
}

bool ft::util::File::atEnd() const
{
    return m_BytesRead == 0;
}

bool ft::util::File::hasError() const
{
    return m_hasError;
}

const std::string & ft::util::File::errorString() const
{
    return m_ErrorString;
}

std::string ft::util::File::readLine()
{
    std::string line;
    do
    {
        if (m_Offset == m_BytesRead)
            _readFromFile();
        if (m_BytesRead == 0 || m_hasError)
            break;
        ssize_t len = m_BytesRead - m_Offset;
        char* endLine = static_cast<char*>(std::memchr(m_Buffer + m_Offset, '\n', len));
        if (endLine)
        {
            len = endLine - (m_Buffer + m_Offset);
            m_isEndFound = true;
        }
        line.append(m_Buffer + m_Offset, len);
        m_Offset += len;
        if (endLine)
            m_Offset += 1;
    } while (!m_isEndFound);
    m_isEndFound = false;
    return line;
}

ssize_t ft::util::File::write(const std::string & text)
{
    return ::write(m_Handle, text.c_str(), text.size());
}

void ft::util::File::_init()
{
    m_Filename.clear();
    m_Buffer = NULL;
    m_Handle = -1;
    m_CurPos = 0;
    m_BytesRead = BUF_SIZE;
    m_Offset = BUF_SIZE;
    m_isEndFound = false;
    m_hasError = false;
    m_ErrorString.clear();
}

void ft::util::File::_onError()
{
    m_hasError = true;
    m_ErrorString = ::strerror(errno);
}

void ft::util::File::_readFromFile()
{
    m_Offset = 0;
    m_BytesRead = ::read(m_Handle, m_Buffer, BUF_SIZE);
    if (m_BytesRead == -1)
    {
        _onError();
        return;
    }
    m_CurPos += m_BytesRead;
    if (m_BytesRead == 0)
        m_isEndFound = true;
}
