#ifndef CONFIG_SECTION_H
#define CONFIG_SECTION_H

#include "../AllowedStd.h"
#include "detail/Node.h"

namespace ft { namespace cfg
{
    namespace detail
    {
        typedef std::list<std::string>::const_iterator PathCIt;
        typedef std::multimap<std::string, ft::cfg::detail::Node>::const_iterator NodeCIt;
        typedef std::pair<NodeCIt, NodeCIt> NodeRange;
    }

    class Section
    {
    private:
        const detail::Node & m_Node;

    public:
        explicit Section(const detail::Node & node);

        const std::string & value(const std::string & path, size_t pos = 0) const;
        const std::list<std::string> & valueList(const std::string & path) const;

        Section section(const std::string & path, size_t pos = 0) const;
        std::list<Section> sectionList(const std::string & path) const;

    private:
        detail::NodeRange _getRange(detail::PathCIt begin, detail::PathCIt end,
                detail::NodeRange range) const;
    };
} }

#endif //CONFIG_SECTION_H
