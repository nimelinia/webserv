#include "Config.h"

#include "config/detail/Parser.h"

ft::cfg::Config::Config()
    :Section(m_RootNode), detail::Parser(m_RootNode)
{
}
