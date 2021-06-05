#include "config/Config.h"
#include "log/Log.h"
#include "http/RequestParser.h"

static void InitLogging()
{
    LOGGER.addHandler(new ft::log::ColorConsoleHandler(ft::log::TextOnlyFormatter));
    LOGGER.setMaxLevel(ft::log::EDebug);

    ft::log::FileHandler* h = new ft::log::FileHandler(ft::log::FullFormatter);
    if (!h->open("config.log"))
    {
        LOGE << "Failed to open file for Config logging: " << h->errorString();
        delete h;
    }
    else
    {
        LOGGER_(CFG).addHandler(h);
        LOGGER_(CFG).setMaxLevel(ft::log::EDebug);
    }
}

int main()
{
    InitLogging();

    LOGF << "GLOBAL TEST";
    LOGE << "GLOBAL TEST";
    LOGW << "GLOBAL TEST";
    LOGI << "GLOBAL TEST";
    LOGD << "GLOBAL TEST";

    LOGF_(CFG) << "CFG TEST";
    LOGE_(CFG) << "CFG TEST";
    LOGW_(CFG) << "CFG TEST";
    LOGI_(CFG) << "CFG TEST";
    LOGD_(CFG) << "CFG TEST";

    ft::cfg::Config cfg;
    try {
        cfg.load("./examples/example.config");
    } catch (const ft::cfg::ConfigException & e)
    {
        LOGF << "Failed to load config: " << e.what();
        return 1;
    }

    std::string key1 = "key4";
    ft::cfg::Section s = cfg.section(key1);
    std::string key2 = "subkey1";
    LOGD << key2 << " = " << s.value(key2);
    key2 = "subkey2";
    std::list<std::string> lst = s.valueList(key2);
    for (std::list<std::string>::const_iterator it = lst.begin(); it != lst.end(); ++it)
        LOGD << key2 << " = " << *it;

    std::string request = "GET / HTTP/1.1\r\n"
                          "Host: localhost:8080\r\n"
                          "User-Agent: curl/7.64.1\r\n"
                          "Accept: */*\r\n"
                          "\r\n"
                          "Hello World!";

    ft::http::RequestParser parser;
    std::pair<ft::http::RequestParser::EResult, size_t> res = parser.parse(request.data(), request.size());

    if (res.first == ft::http::RequestParser::EError)
        LOGE << "Error parsing request";
    else
    {
        LOGD;
        LOGD << "METHOD: " << parser.m_method;
        LOGD << "URI: " << parser.m_uri;
        LOGD << "MAJOR_VER: " << parser.m_ver_major;
        LOGD << "MINOR_VER: " << parser.m_ver_minor;
        LOGD << "Headers: ";
        for (std::vector<ft::http::Header>::iterator it = parser.m_headers.begin(); it != parser.m_headers.end(); ++it)
            LOGD << "\t" << it->name << ": " << it->value;

        LOGD;
        LOGD << "Request left: " << request.substr(res.second + 1);
        return 0;
    }

    return 0;
}
