#include "Level.hpp"
#include "../AllowedStd.hpp"

ft::log::ELevel ft::log::LevelFormString(const char* s)
{
    switch (std::toupper(s[0]))
    {
        case 'F':
            return EFatal;
        case 'E':
            return EError;
        case 'W':
            return EWarning;
        case 'I':
            return EInfo;
        case 'D':
            return EDebug;
        default:
            return ENone;
    }
}

const char * ft::log::LevelToString(ELevel s)
{
    switch (s)
    {
        case EFatal:
            return "FATAL";
        case EError:
            return "ERROR";
        case EWarning:
            return "WARN";
        case EInfo:
            return "INFO";
        case EDebug:
            return "DEBUG";
        default:
            return "NONE";
    }
}