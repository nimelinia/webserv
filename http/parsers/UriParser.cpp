//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "UriParser.hpp"
#include "server/webserv.hpp"

bool ft::UriParser::parse_uri(const std::string& path, Uri& uri)
{
	if (path.empty() || path[0] != '/')
		return false;
	std::string cur_path;
	std::string::size_type query_pos = path.find_last_of('?');
	if (query_pos != std::string::npos)
	{
		uri.query = path.substr(query_pos + 1);
		if (uri.query.empty())
			return false;
		cur_path = path.substr(0, query_pos);
	}
	else
		cur_path = path;
	if (cur_path[cur_path.size() - 1] != '/')
		cur_path.append("/");
	if (!find_path_of_uri(cur_path, path, uri))
		return false;
	std::list<std::string> split = util::str::Split(cur_path, '/');
	struct stat path_stat;
	while (!split.empty())
	{
		const std::string check = uri.root + uri.path + split.front();
		if (stat(check.c_str(), &path_stat) == -1)
			break;
		if ((path_stat.st_mode & S_IFMT) == S_IFDIR)
		{
			uri.path += split.front() + "/";
			split.pop_front();
		}
		else if ((path_stat.st_mode & S_IFMT) == S_IFREG)
		{
			const std::string::size_type dot_pos = split.front().find_last_of('.');
			if (dot_pos != std::string::npos)
				uri.file_ext = split.front().substr(dot_pos + 1);
			uri.file_name = split.front();
			split.pop_front();
			break;
		}
		else
			break;
	}

	if (m_method == "GET")
	{
		if (uri.file_name.empty())
		{
			if (m_location->index.empty())
			{
				if (!m_location->autoindex)
					return (false);
				else
					return (true);
			}
			uri.file_name = m_location->index;
			const std::string::size_type dot_pos = uri.file_name.find_last_of('.');
			if (dot_pos != std::string::npos)
				uri.file_ext = uri.file_name.substr(dot_pos + 1);
		}
		for (std::list<std::string>::iterator it = split.begin(); it != split.end(); ++it)
			uri.extra_path += "/" + *it;
	}
	else if (m_method == "POST")
    {
        if (uri.file_name.empty())
        {
            if (m_location->autoindex)
                return true;
            if (!m_location->index.empty())
                uri.file_name = m_location->index;;
            const std::string::size_type dot_pos = uri.file_name.find_last_of('.');
            if (dot_pos != std::string::npos)
                uri.file_ext = uri.file_name.substr(dot_pos + 1);
        }
        for (std::list<std::string>::iterator it = split.begin(); it != split.end(); ++it)
            uri.extra_path += "/" + *it;
        return true;
    }
	else
	{
		if (uri.file_name.empty() && !split.empty())
		{
			std::list<std::string>::iterator endit = --split.end();
			for (std::list<std::string>::iterator it = split.begin(); it != endit; ++it)
				uri.extra_path += "/" + *it;
			if (path[path.length() - 1] == '/')
				uri.extra_path += "/" + *endit;
			else
				uri.file_name = *endit;
		} else if (!split.empty())
			return false;
	}
	return true;
}

bool ft::UriParser::find_path_of_uri(std::string &cur_path, const std::string &path, Uri& uri)
{
	std::string::size_type last_slash = cur_path.size() - 1;
	do
	{
		std::string search = cur_path.substr(0, last_slash + 1);
		std::list<Locations>::const_iterator cit = std::find_if(m_root_list.begin(), m_root_list.end(), FindLocation(search));
		if (cit != m_root_list.end())
		{
			uri.root = cit->root;
			m_location = &(*cit);
			uri.locations = m_location;
			break;
		}
	} while ((last_slash = path.find_last_of('/', last_slash - 1)) != std::string::npos);
	if (uri.root.empty())
		return false;
	cur_path = cur_path.substr(last_slash + 1);
	if (!cur_path.empty())
		cur_path.erase(cur_path.size() - 1);
	return true;
}

