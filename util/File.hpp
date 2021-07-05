#ifndef UTIL_FILE_H
#define UTIL_FILE_H

#include "../AllowedStd.hpp"
#include "NonCopyable.hpp"

#define BUF_SIZE 4096

namespace ft { namespace util
{
    class File: public NonCopyable
    {
    public:
        enum EMode
        {
            EReadOnly = O_RDONLY,
            EWriteOnly = O_WRONLY,
            EReadWrite = O_RDWR
        };

    private:
        std::string m_Filename;
        char* m_Buffer;
        int m_Handle;
        ssize_t m_CurPos;
        ssize_t m_BytesRead;
        ssize_t m_Offset;
        bool m_isEndFound;
        bool m_hasError;
        std::string m_ErrorString;

    public:
        File();
        explicit File(const std::string & filename);
        ~File();

        void setFileName(const std::string & filename);

        bool open(EMode mode);
        void close();
        bool isOpen() const;
        bool atEnd() const;
        bool hasError() const;

        const std::string & errorString() const;

        std::string readLine();
        ssize_t write(const std::string & text);

    private:
        void _init();
        void _onError();
        void _readFromFile();
    };
} }

#endif //UTIL_FILE_H
