#ifndef HTTP_REQUESTPARSER_H
#define HTTP_REQUESTPARSER_H

#include "AllowedStd.h"

namespace ft { namespace http
{
    struct Header
    {
        std::string name;
        std::string value;
    };

    class RequestParser
    {
    public:
        enum EResult
        {
            EError = 0,
            EOk,
            EParse
        };

    private:
        enum EState
        {
            EStart,
            EMethod,
            EUri,
            EVer_H,
            EVer_HT,
            EVer_HTT,
            EVer_HTTP,
            EVer_Slash,
            EVer_MajorStart,
            EVer_MajorEnd,
            EVer_MinorStart,
            EVer_MinorEnd,
            ENewLine1,
            ENewLine2,
            EHeaderStart,
            EHeaderName,
            EHeaderSpace,
            EHeaderValue
        };

    public:
        EState m_state;
        std::string m_method;
        std::string m_uri;
        int m_ver_major;
        int m_ver_minor;
        std::vector<Header> m_headers;

    public:
        RequestParser();

        std::pair<EResult, size_t> parse(const char* buf, size_t size);
        void reset();

    private:
        EResult _consume(char c);
    };
} }

#endif //HTTP_REQUESTPARSER_H
