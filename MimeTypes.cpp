#include "MimeTypes.hpp"

namespace ft { namespace util { namespace mime_types
{
    struct mime_type
    {
        const char* extension;
        const char* type;
    };
    mime_type mapping[] = {
//        { "", "application/octet-stream" },
        { "html", "text/html" },
        { "htm", "text/html" },
		{ "txt", "text/plain" },
        { "css", "text/css" },
        { "jpeg", "image/jpeg" },
        { "png", "image/png" },
        { "svg", "image/svg" },
        { "gif", "image/gif" },
        { "pdf", "application/pdf" }
    };
} } }

const char* ft::util::extension_to_mime_type(const std::string& extension)
{
    const size_t size = sizeof(mime_types::mapping) / sizeof(mime_types::mime_type);
    for (size_t i = 0; i < size; ++i)
    {
        if (mime_types::mapping[i].extension == extension)
            return mime_types::mapping[i].type;
    }
    return "";
}