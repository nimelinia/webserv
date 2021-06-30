#ifndef HTTP_REQUESTPARSER_H
#define HTTP_REQUESTPARSER_H

#include "../AllowedStd.h"


namespace ft {
	class Message;
	struct Config;
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
            EHeaderValue,
            EBodyFull,
            EBodyChunked_SizeStart,
            EBodyChunked_SizeCont,
            EBodyChunked_Ext,
            EBodyChunked_NewLine1,
            EBodyChunked_Data,
            EBodyChunked_NewLine2,
            EBodyChunked_TrailerStart,
            EBodyChunked_TrailerName,
            EBodyChunked_TrailerSpace,
            EBodyChunked_TrailerValue,
            EBodyChunked_NewLine3,
        };

    public:
        EState m_state;
        size_t m_content_length;
        size_t m_max_body_size;

    public:
//        explicit RequestParser(size_t max_body_size);
		explicit RequestParser();

        EResult parse(std::list<Config> &configs, Message& msg, const char* buf, size_t size);
        void reset();

    private:
        EResult _consume(std::list<Config> &configs, Message& msg, char c);
        ft::http::RequestParser::EResult _init_body_size(std::list<Config> &configs, Message& msg);
    };
} }

#endif //HTTP_REQUESTPARSER_H
