#ifndef HTTP_REQUESTPARSER_H
#define HTTP_REQUESTPARSER_H

#include "../AllowedStd.h"


namespace ft {
	class Message;
	namespace http
{
    struct Header
    {
        std::string name;
        std::string value;
    };

	struct FindHeader: public std::unary_function<Header, bool>
	{
		std::string m_name;
		FindHeader(const std::string& name): m_name(name) {}
		bool operator()(const Header& header)
		{
			return header.name == m_name;
		}
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

    public:
        RequestParser();

        std::pair<EResult, size_t> parse(Message& msg, const char* buf, size_t size);
        void reset();

    private:
        EResult _consume(Message& msg, char c);
    };
} }

#endif //HTTP_REQUESTPARSER_H
