#include "Config.h"

#include "detail/Parser.h"

ft::cfg::Config::Config()
    :Section(m_RootNode), detail::Parser(m_RootNode)
{
}
