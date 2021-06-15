#include "Section.h"
#include "config/exceptions/PathException.h"
#include "config/exceptions/ValueException.h"
#include "util/String.h"

ft::cfg::Section::Section(const ft::cfg::detail::Node& node)
    : m_Node(node)
{
}

const std::string& ft::cfg::Section::value(const std::string& path, size_t pos /* = 0*/) const
{
    Section s = section(path);
    if (pos >= s.m_Node.value_list.size())
        throw ValueException("No such value");
    const std::list<std::string> & lst = s.m_Node.value_list;
    for (std::list<std::string>::const_iterator it = lst.begin(); it != lst.end(); ++it)
    {
        if (pos == 0)
            return *it;
        --pos;
    }
    throw ValueException("Unexpected end of value list");
}

const std::list<std::string> & ft::cfg::Section::valueList(const std::string & path) const
{
    Section s = section(path);
    return s.m_Node.value_list;
}

ft::cfg::Section ft::cfg::Section::section(const std::string& path, size_t pos /* = 0*/) const
{
    std::list<Section> lst = sectionList(path);
    if (pos >= lst.size())
        throw PathException("No such section", path);
    for (std::list<Section>::const_iterator it = lst.begin(); it != lst.end(); ++it)
    {
        if (pos == 0)
            return *it;
        --pos;
    }
    throw PathException("Unexpected end of section list", path);
}

std::list<ft::cfg::Section> ft::cfg::Section::sectionList(const std::string & path) const
{
    std::list<std::string> pathList = util::str::Split(path, '/');
    if (pathList.empty())
        throw PathException("Path is empty", path);

    const detail::NodeRange range = _getRange(pathList.begin(), pathList.end(), m_Node.children.equal_range(pathList.front()));
    if (range.first == range.second)
        throw PathException("No such key", path);
    std::list<Section> lst;
    for (detail::NodeCIt it = range.first; it != range.second; ++it)
        lst.push_back(Section(it->second));
    return lst;
}

bool ft::cfg::Section::contains(const std::string& path)
{
    std::list<std::string> pathList = util::str::Split(path, '/');
    if (pathList.empty())
        throw PathException("Path is empty", path);

    const detail::NodeRange range = _getRange(pathList.begin(), pathList.end(), m_Node.children.equal_range(pathList.front()));
    return range.first != range.second;
}

ft::cfg::detail::NodeRange
ft::cfg::Section::_getRange(ft::cfg::detail::PathCIt begin, ft::cfg::detail::PathCIt end,
        ft::cfg::detail::NodeRange range) const
{
    if (++begin == end)
        return range;
    for (detail::NodeCIt it = range.first; it != range.second; ++it)
    {
        const detail::NodeRange next = it->second.children.equal_range(*begin);
        if (next.first != next.second)
            return _getRange(begin, end, next);
    }
    return std::make_pair(range.first, range.first);
}
