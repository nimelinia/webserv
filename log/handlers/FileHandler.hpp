#ifndef LOG_FILEHANDLER_H
#define LOG_FILEHANDLER_H

#include "../../AllowedStd.hpp"
#include "AHandler.hpp"

namespace ft { namespace log
{
    class FileHandler: public AHandler
    {
    private:
        std::ofstream m_File;

    public:
        explicit FileHandler(FormatFunc func);
        virtual ~FileHandler();

        virtual void write(const detail::Message & msg);

        bool open(const std::string & filename);
    };
}}

#endif //LOG_FILEHANDLER_H
