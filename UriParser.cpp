//
// Created by Streetmentioner Copycat on 6/23/21.
//

#include "UriParser.hpp"

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
	std::string::size_type last_slash = cur_path.size() - 1;
	do
	{
		std::string search = cur_path.substr(0, last_slash + 1);
		std::list<Locations>::const_iterator cit = std::find(m_root_list.begin(), m_root_list.end(), search);
		if (cit != m_root_list.end())
		{
			uri.root = "./";
			uri.path = search.substr(1);
			break;
		}
	} while ((last_slash = path.find_last_of('/', last_slash - 1)) != std::string::npos);
	if (uri.root.empty())
		return false;
	cur_path = cur_path.substr(last_slash + 1);
	cur_path.erase(cur_path.size() - 1);
	std::list<std::string> split = util::str::Split(cur_path, '/');
	struct stat path_stat;
	while (!split.empty())
	{
		const std::string check = uri.root + uri.path + split.front();
		lstat(check.c_str(), &path_stat);
		if ((path_stat.st_mode & S_IFMT) == S_IFDIR)
		{
			uri.path += split.front();
			split.pop_front();
		}
		else if ((path_stat.st_mode & S_IFMT) == S_IFREG)
		{
			const std::string::size_type dot_pos = split.front().find_last_of('.');
			if (dot_pos != std::string::npos)
			{
				uri.file_name = split.front().substr(0, dot_pos);
				uri.file_ext = split.front().substr(dot_pos);
			}
			else
				uri.file_name = split.front();
			split.pop_front();
			break;
		}
		else
			break;
	}
	if (uri.file_name.empty())
	{
//		if (has no index)
//			return false;
		uri.file_name = "index.html";
		uri.file_ext = "html";
	}
	for (std::list<std::string>::iterator it = split.begin(); it != split.end(); ++it)
		uri.extra_path += "/" + *it;
	return true;
}