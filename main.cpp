#include "config/Config.h"
#include "log/Log.h"

static void InitLogging()
{
    LOGGER.addHandler(new ft::log::ColorConsoleHandler(ft::log::SourceFormatter));
    LOGGER.setMaxLevel(ft::log::EWarning);

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
    std::cout << key2 << " = " << s.value(key2) << std::endl;
    key2 = "subkey2";
    std::list<std::string> lst = s.valueList(key2);
    for (std::list<std::string>::const_iterator it = lst.begin(); it != lst.end(); ++it)
        std::cout << key2 << " = " << *it << std::endl;

    return 0;
}
