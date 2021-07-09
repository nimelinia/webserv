#include "StatusCodes.hpp"

namespace ft {namespace http { namespace status_string
{
    const char* ok =
            "HTTP/1.1 200 OK\r\n";
    const char* created =
            "HTTP/1.1 201 Created\r\n";
    const char* accepted =
            "HTTP/1.1 202 Accepted\r\n";
    const char* no_content =
            "HTTP/1.1 204 No Content\r\n";
    const char* multiple_choices =
            "HTTP/1.1 300 Multiple Choices\r\n";
    const char* moved_permanently =
            "HTTP/1.1 301 Moved Permanently\r\n";
    const char* moved_temporarily =
            "HTTP/1.1 302 Moved Temporarily\r\n";
    const char* not_modified =
            "HTTP/1.1 304 Not Modified\r\n";
    const char* bad_request =
            "HTTP/1.1 400 Bad Request\r\n";
    const char* forbidden =
            "HTTP/1.1 403 Forbidden\r\n";
    const char* not_found =
            "HTTP/1.1 404 Not Found\r\n";
    const char* not_allowed =
            "HTTP/1.1 405 Method Not Allowed\r\n";
    const char* not_acceptable =
            "HTTP/1.1 406 Not Acceptable\r\n";
    const char* timeout =
            "HTTP/1.1 408 Request Timeout\r\n";
    const char* too_large =
    		"HTTP/1.1 413 Payload Too Large\r\n";
    const char* unsupported_media_type =
    		"HTTP/1.1 415 Unsupported Media Type\r\n";
    const char* internal_server_error =
            "HTTP/1.1 500 Internal Server Error\r\n";
    const char* not_implemented =
            "HTTP/1.1 501 Not Implemented\r\n";
    const char* service_unavailable =
            "HTTP/1.1 503 Service Unavailable\r\n";
    const char* version_not_supported =
            "HTTP/1.1 505 HTTP Version Not Supported\r\n";
} } }

const char * ft::http::status_to_string(size_t status)
{
    switch (status)
    {
        case 200:
            return status_string::ok;
        case 201:
            return status_string::created;
        case 202:
            return status_string::accepted;
        case 204:
            return status_string::no_content;
        case 300:
            return status_string::multiple_choices;
        case 301:
            return status_string::moved_permanently;
        case 302:
            return status_string::moved_temporarily;
        case 304:
            return status_string::not_modified;
        case 400:
            return status_string::bad_request;
        case 403:
            return status_string::forbidden;
        case 404:
            return status_string::not_found;
        case 405:
            return status_string::not_allowed;
        case 406:
            return status_string::not_acceptable;
        case 408:
            return status_string::timeout;
		case 413:
			return status_string::too_large;
		case 415:
			return status_string::unsupported_media_type;
        case 501:
            return status_string::not_implemented;
        case 503:
            return status_string::service_unavailable;
        case 505:
            return status_string::version_not_supported;
        default:
            return status_string::internal_server_error;
    }
}

namespace ft {namespace http { namespace status_body
{
    const char* ok = "";
    const char* created =
            "<html>"
            "<head><title>Created</title></head>"
            "<body><h1>201 Created</h1></body>"
            "</html>";
    const char* accepted =
            "<html>"
            "<head><title>Accepted</title></head>"
            "<body><h1>202 Accepted</h1></body>"
            "</html>";
    const char* no_content =
            "<html>"
            "<head><title>No Content</title></head>"
            "<body><h1>204 Content</h1></body>"
            "</html>";
    const char* multiple_choices =
            "<html>"
            "<head><title>Multiple Choices</title></head>"
            "<body><h1>300 Multiple Choices</h1></body>"
            "</html>";
    const char* moved_permanently =
            "<html>"
            "<head><title>Moved Permanently</title></head>"
            "<body><h1>301 Moved Permanently</h1></body>"
            "</html>";
    const char* moved_temporarily =
            "<html>"
            "<head><title>Moved Temporarily</title></head>"
            "<body><h1>302 Moved Temporarily</h1></body>"
            "</html>";
    const char* not_modified =
            "<html>"
            "<head><title>Not Modified</title></head>"
            "<body><h1>304 Not Modified</h1></body>"
            "</html>";
    const char* bad_request =
            "<html>"
            "<head><title>Bad Request</title></head>"
            "<body><h1>400 Bad Request</h1></body>"
            "</html>";
    const char* forbidden =
            "<html>"
            "<head><title>Forbidden</title></head>"
            "<body><h1>403 Forbidden</h1></body>"
            "</html>";
    const char* not_found =
            "<html>"
            "<head><title>Not Found</title></head>"
            "<body><h1>404 Not Found</h1></body>"
            "</html>";
    const char* not_allowed =
            "<html>"
            "<head><title>Method Not Allowed</title></head>"
            "<body><h1>405 Method Not Allowed</h1></body>"
            "</html>";
    const char* not_acceptable =
            "<html>"
            "<head><title>Not Acceptable</title></head>"
            "<body><h1>406 Not Acceptable</h1></body>"
            "</html>";
    const char* timeout =
            "<html>"
            "<head><title>Request Timeout</title></head>"
            "<body><h1>408 Request Timeout</h1></body>"
            "</html>";
    const char* too_large =
    		"<html>"
			"<head><title>Payload Too Large</title></head>"
			"<body><h1>413 Payload Too Large</h1></body>"
			"</html>";
    const char* unsupported_media_type =
			"<html>"
			"<head><title>Request Timeout</title></head>"
			"<body><h1>415 Unsupported Media Type</h1></body>"
			"</html>";
    const char* internal_server_error =
            "<html>"
            "<head><title>Internal Server Error</title></head>"
            "<body><h1>500 Internal Server Error</h1></body>"
            "</html>";
    const char* not_implemented =
            "<html>"
            "<head><title>Not Implemented</title></head>"
            "<body><h1>501 Not Implemented</h1></body>"
            "</html>";
    const char* service_unavailable =
            "<html>"
            "<head><title>Service Unavailable</title></head>"
            "<body><h1>503 Service Unavailable</h1></body>"
            "</html>";
    const char* version_not_supported =
            "<html>"
            "<head><title>HTTP Version Not Supported</title></head>"
            "<body><h1>505 HTTP Version Not Supported</h1></body>"
            "</html>";
} } }

const char* ft::http::default_status_body(size_t status)
{
    switch (status)
    {
        case 200:
            return status_body::ok;
        case 201:
            return status_body::created;
        case 202:
            return status_body::accepted;
        case 204:
            return status_body::no_content;
        case 300:
            return status_body::multiple_choices;
        case 301:
            return status_body::moved_permanently;
        case 302:
            return status_body::moved_temporarily;
        case 304:
            return status_body::not_modified;
        case 400:
            return status_body::bad_request;
        case 403:
            return status_body::forbidden;
        case 404:
            return status_body::not_found;
        case 405:
            return status_body::not_allowed;
        case 406:
            return status_body::not_acceptable;
        case 408:
            return status_body::timeout;
		case 413:
			return status_body::too_large;
		case 415:
			return status_body::unsupported_media_type;
        case 501:
            return status_body::not_implemented;
        case 503:
            return status_body::service_unavailable;
        case 505:
            return status_body::version_not_supported;
        default:
            return status_body::internal_server_error;
    }
}