#ifndef LOG_FILEHANDLER_H
#define LOG_FILEHANDLER_H

#include "../../AllowedStd.h"
#include "AHandler.h"
#include "../../util/File.h"

namespace ft { namespace log
{
    class FileHandler: public AHandler
    {
    private:
        util::File m_File;

    public:
        explicit FileHandler(FormatFunc func);
        virtual ~FileHandler();

        virtual void write(const detail::Message & msg);

        bool open(const std::string & filename);
        const std::string & errorString() const;
    };
}}

#endif //LOG_FILEHANDLER_H
