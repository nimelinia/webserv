#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include "../AllowedStd.hpp"
#include "detail/Node.hpp"
#include "detail/Parser.hpp"
#include "exceptions/ParseException.hpp"
#include "exceptions/PathException.hpp"
#include "exceptions/ValueException.hpp"
#include "Section.hpp"

namespace ft { namespace cfg
{
    class Config: public Section, public detail::Parser
    {
    private:
        detail::Node m_RootNode;

    public:
        Config();
    };
} }

#endif //CONFIG_CONFIG_H
