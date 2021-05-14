#ifndef LOG_LEVEL_H
#define LOG_LEVEL_H

namespace ft { namespace log
{
    enum ELevel
    {
        ENone = 0,
        EFatal,
        EError,
        EWarning,
        EInfo,
        EDebug,
    };

    const char* LevelToString(ELevel s);
    ELevel LevelFormString( const char* s );
} }

#endif //LOG_LEVEL_H
