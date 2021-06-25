#include "RequestParser.h"
#include "Message.hpp"

#define CR '\r'
#define LR '\n'

ft::http::RequestParser::RequestParser()
    : m_state(EStart)
{
}

std::pair<ft::http::RequestParser::EResult, size_t> ft::http::RequestParser::parse(Message& msg, const char* buf, size_t size)
{
   for (size_t i = 0; i < size; ++i)
   {
       EResult res = _consume(msg, buf[i]);
       if (res != EParse)
           return std::make_pair(res, i);
   }
   return std::make_pair(EParse, size);
}

ft::http::RequestParser::EResult ft::http::RequestParser::_consume(Message& msg, char c)
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
            else if (!std::isalpha(c))
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
				msg.m_uri.push_back(c);
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
				msg.m_headers.back().name.push_back(c);
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
				msg.m_headers.back().name.push_back(c);
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
				msg.m_headers.back().value.push_back(c);
                return EParse;
            }
        case ENewLine2:
            return (c == LR) ? EOk : EError;
        default:
            return EError;
    }
}

void ft::http::RequestParser::reset()
{
    m_state = EStart;
}
