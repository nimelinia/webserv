#include "RequestParser.h"
#include "Message.hpp"
#include "util/String.h"
//#include "webserv.hpp"

#define CR '\r'
#define LR '\n'

//ft::http::RequestParser::RequestParser(size_t max_body_size)
//    : m_state(EStart)
//    , m_max_body_size(max_body_size)
//    , m_content_length(0)
//{
//}

ft::http::RequestParser::RequestParser()
		: m_state(EStart)
		, m_content_length(0)
{
}

ft::http::RequestParser::EResult ft::http::RequestParser::parse(std::list<Config> &configs, Message& msg, const char* buf, size_t size)
{
   for (size_t i = 0; i < size; ++i)
   {
       EResult res = _consume(configs, msg, buf[i]);
       if (res != EParse)
           return res;
   }
   return EParse;
}

ft::http::RequestParser::EResult ft::http::RequestParser::_consume(std::list<Config> &configs, Message& msg, char c)
{
    switch (m_state)
    {
        case EStart:
            if (!std::isalpha(c))
                return EError;
            else
            {
                m_state = EMethod;
                msg.m_method.push_back(c);
                return EParse;
            }
        case EMethod:
            if (c == ' ')
            {
                m_state = EUri;
                return EParse;
            }
            else if (!std::isalpha(c) || msg.m_method.size() > 7)
                return EError;
            else
            {
                msg.m_method.push_back(c);
                return EParse;
            }
        case EUri:
            if (c == ' ')
            {
                m_state = EVer_H;
                return EParse;
            }
            else if (std::iscntrl(c))
                return EError;
            else
            {
                msg.m_uri_str.push_back(c);
                return EParse;
            }
        case EVer_H:
            if (c == 'H')
            {
                m_state = EVer_HT;
                return EParse;
            }
            else
                return EError;
        case EVer_HT:
            if (c == 'T')
            {
                m_state = EVer_HTT;
                return EParse;
            }
            else
                return EError;
        case EVer_HTT:
            if (c == 'T')
            {
                m_state = EVer_HTTP;
                return EParse;
            }
            else
                return EError;
        case EVer_HTTP:
            if (c == 'P')
            {
                m_state = EVer_Slash;
                return EParse;
            }
            else
                return EError;
        case EVer_Slash:
            if (c == '/')
            {
                m_state = EVer_MajorStart;
                return EParse;
            }
            else
                return EError;
        case EVer_MajorStart:
            if (std::isdigit(c))
            {
                msg.m_ver_major = c - '0';
                m_state = EVer_MajorEnd;
                return EParse;
            }
            else
                return EError;
        case EVer_MajorEnd:
            if (c == '.')
            {
                m_state = EVer_MinorStart;
                return EParse;
            }
            else
                return EError;
        case EVer_MinorStart:
            if (std::isdigit(c))
            {
                msg.m_ver_minor = c - '0';
                m_state = EVer_MinorEnd;
                return EParse;
            }
            else
                return EError;
        case EVer_MinorEnd:
            if (c == CR)
            {
                m_state = ENewLine1;
                return EParse;
            }
            else if (c == LR)
            {
                m_state = EHeaderStart;
                return EParse;
            }
            else
                return EError;
        case ENewLine1:
            if (c == LR)
            {
                m_state = EHeaderStart;
                return EParse;
            }
            else
                return EError;
        case EHeaderStart:
            if (c == CR)
            {
                m_state = ENewLine2;
                return EParse;
            }
            else if (!std::isalpha(c))
                return EError;
            else
            {
                msg.m_headers.push_back(Header());
                msg.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                m_state = EHeaderName;
                return EParse;
            }
        case EHeaderName:
            if (c == ':')
            {
                m_state = EHeaderSpace;
                return EParse;
            }
            else if (std::isalpha(c) || c == '-')
            {
                msg.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                return EParse;
            }
            else
                return EError;
        case EHeaderSpace:
            if (c == ' ')
            {
                m_state = EHeaderValue;
                return EParse;
            }
            else
                return EError;
        case EHeaderValue:
            if (c == CR)
            {
                m_state = ENewLine1;
                return EParse;
            }
            else if (std::iscntrl(c))
                return EError;
            else
            {
                msg.m_headers.back().value.push_back(static_cast<char>(std::tolower(c)));
                return EParse;
            }
        case ENewLine2:
            return (c == LR) ? _init_body_size(configs, msg) : EError;
        case EBodyFull:
            msg.m_body.push_back(c);
            return --m_content_length == 0 ? EOk : EParse;
        case EBodyChunked_SizeStart:
            if (!std::isxdigit(c))
                return EError;
            else
            {
                m_state = EBodyChunked_SizeCont;
                m_content_length = util::str::DecodeHex<size_t>(c);
                return EParse;
            }
        case EBodyChunked_SizeCont:
            if (c == CR)
            {
                m_state = EBodyChunked_NewLine1;
                return EParse;
            }
            if (m_content_length != 0 && c == ';')
            {
                m_state = EBodyChunked_Ext;
                return EParse;
            }
            else if (!std::isxdigit(c))
                return EError;
            else
            {
                m_content_length = m_content_length * 16 + util::str::DecodeHex<size_t>(c);
                return EParse;
            }
        case EBodyChunked_Ext:
            if (c == CR)
            {
                m_state = EBodyChunked_NewLine1;
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_NewLine1:
            if (c == LR)
            {
                if (m_content_length != 0)
                    m_state = EBodyChunked_Data;
                else
                    m_state = EBodyChunked_TrailerStart;
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_Data:
            if (m_content_length > 0)
            {
                m_content_length -= 1;
                msg.m_body.push_back(c);
                return EParse;
            }
            else if (c == CR)
            {
                m_state = EBodyChunked_NewLine2;
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_NewLine2:
            if (c == LR)
            {
                m_state = EBodyChunked_SizeStart;
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_TrailerStart:
            if (c == CR)
            {
                m_state = EBodyChunked_NewLine3;
                return EParse;
            }
            else if (!std::isalpha(c))
                return EError;
            else
            {
                msg.m_headers.push_back(Header());
                msg.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                m_state = EBodyChunked_TrailerName;
                return EParse;
            }
        case EBodyChunked_TrailerName:
            if (c == ':')
            {
                m_state = EBodyChunked_TrailerSpace;
                return EParse;
            }
            else if (std::isalpha(c) || c == '-')
            {
                msg.m_headers.back().name.push_back(static_cast<char>(std::tolower(c)));
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_TrailerSpace:
            if (c == ' ')
            {
                m_state = EBodyChunked_TrailerValue;
                return EParse;
            }
            else
                return EError;
        case EBodyChunked_TrailerValue:
            if (c == CR)
            {
                m_state = ENewLine1;
                return EParse;
            }
            else if (std::iscntrl(c))
                return EError;
            else
            {
                msg.m_headers.back().value.push_back(static_cast<char>(std::tolower(c)));
                return EParse;
            }
        case EBodyChunked_NewLine3:
            return (c == LR) ? EOk : EError;
        default:
            return EError;
    }
}

void ft::http::RequestParser::reset()
{
    m_state = EStart;
}

ft::http::RequestParser::EResult ft::http::RequestParser::_init_body_size(std::list<Config> &configs, Message& msg)
{
	std::vector<http::Header>::iterator it = std::find_if(msg.m_headers.begin(),
														  msg.m_headers.end(),
														  http::FindHeader("host"));
	if (it != msg.m_headers.end())
		msg.host_name = it->value;
	else
	{
		msg.m_error_num = 400;
		return EError;
	}
	const std::string host = msg.host_name.substr(0, msg.host_name.find_last_of(':'));
	std::list<Config>::iterator cit = configs.begin();
	for (; cit != configs.end(); ++cit)
	{
		if (cit->server_name == host)
			break;
	}
	if (cit == configs.end())
		cit = configs.begin();
	msg.m_uri.config = &(*cit);

	UriParser parser(msg.m_uri.config->locations, msg.m_method);
	if (!parser.parse_uri(msg.m_uri_str, msg.m_uri))
	{
		msg.m_error_num = 404;
		return EError;
	}
	msg.m_uri.locations = parser.m_location;

	std::vector<http::Header>::iterator hit = std::find_if(msg.m_headers.begin(),
            msg.m_headers.end(),
            http::FindHeader("content-length"));
    if (hit != msg.m_headers.end())
    {
        std::pair<size_t, bool> res = util::str::FromString<size_t>(hit->value);
        if (!res.second)
		{
			msg.m_error_num = 400;
        	return EError;
		}

        m_content_length = res.first;
        if (m_content_length > msg.m_uri.locations->limit_body_size)
		{
        	msg.m_error_num = 413;
			return EError;
		}
        m_state = EBodyFull;
        return m_content_length == 0 ? EOk : EParse;
    }
    hit = std::find_if(msg.m_headers.begin(), msg.m_headers.end(),
            http::FindHeader("transfer-encoding"));
    if (hit != msg.m_headers.end())
    {
        if (hit->value != "chunked")
            return EError;
        m_state = EBodyChunked_SizeStart;
        return EParse;
    }
    return EOk;
}
