#ifndef CONFIG_CONFIG_H
#define CONFIG_CONFIG_H

#include "AllowedStd.h"
#include "config/detail/Node.h"
#include "config/detail/Parser.h"
#include "exceptions/ParseException.h"
#include "exceptions/PathException.h"
#include "exceptions/ValueException.h"
#include "Section.h"

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
