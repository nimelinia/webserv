#include "Config.hpp"

#include "detail/Parser.hpp"

ft::cfg::Config::Config()
    :Section(m_RootNode), detail::Parser(m_RootNode)
{
}
