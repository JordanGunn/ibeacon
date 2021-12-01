//
// Created by jordan on 2021-11-10.
//

#include "http/http_response.h"


// ============================
// R E S P O N S E    C L A S S
// ============================
struct HttpResponse {
    char * version;
    char * status_code;
    char * status;
    char * date;            // e.g. Th, 08 Aug 2013 23:54:35 GMT
    char * server;          // Server software spec (e.g. Apache/2.4.39 (CentOS))
    char * last_modified;   // e.g. Th, 08 Aug 2013 23:54:35 GMT
    char * content_length;
    char * content_type;  // Length of object in bytes
    char * content;

};

/*
 * RESPONSE CONSTRUCTOR
 */
HttpResponsePtr http_response_constructor(char * version, char * status_code, char * status)
{
    HttpResponsePtr http_response = malloc(sizeof * http_response);
    if (http_response)
    {
        set_status(http_response, status);
        set_status_code(http_response, status_code);
        set_res_version(http_response, version);
    }

    return http_response;
}

/*
 * Getters
 */
char * get_res_version(HttpResponsePtr http)
{
    if (http)
    {
        return http->version;
    }
}

char * get_status(HttpResponsePtr http)
{
    if (http)
    {
        return http->status;
    }
}

char * get_status_code(HttpResponsePtr http)
{
    if (http)
    {
        return http->status_code;
    }
}

char * get_date(HttpResponsePtr http)
{
    if (http)
    {
        return http->date;
    }
}

char * get_server(HttpResponsePtr http)
{
    if (http)
    {
        return http->server;
    }
}

char * get_last_modified(HttpResponsePtr http)
{
    if (http)
    {
        return http->last_modified;
    }
}

char * get_content_type(HttpResponsePtr http)
{
    if (http)
    {
        return http->content_type;
    }
}

//Giwoun trying to initialize values for mac.
ssize_t get_content_length_long(HttpResponsePtr http)
{
    long content_length = 0;
    if (http)
    {
        size_t len = strlen(http->content_length);
        char * end = http->content_length + len;
        content_length = strtol(http->content_length, &end,10);

    }
    return content_length;

}

char * get_content_length_str(HttpResponsePtr http)
{
    if (http)
    {

        return http->content_length;
    }
}

char * get_content(HttpResponsePtr http)
{
    if (http)
    {
        return http->content;
    }
}


/*
 * Setters
 */

void set_res_version(HttpResponsePtr http, char * version)
{
    if (http)
    {
        http->version = version;
    }
}

void set_status(HttpResponsePtr http, char * status)
{
    if (http)
    {
        http->status = status;
    }
}

void set_status_code(HttpResponsePtr http, char * status_code)
{
    if (http)
    {
        http->status_code = status_code;
    }
}

void set_date(HttpResponsePtr http, char * date)
{
    if (http)
    {
        http->date = date;
    }
}

void set_server(HttpResponsePtr http, char * server)
{
    if (http)
    {
        http->server = server;
    }
}

void set_last_modified(HttpResponsePtr http, char * last_modified)
{
    if (http)
    {
        http->last_modified = last_modified;
    }
}

void set_content_length(HttpResponsePtr http, char * content_length)
{
    if (http)
    {
        http->content_length = content_length;
    }
}

void set_content_type(HttpResponsePtr http, char * content_type)
{
    if (http)
    {
        http->content_type = content_type;
    }
}

void set_content(HttpResponsePtr http, char * content)
{
    if (http)
    {
        http->content = content;
    }
}

/*
 * destructor
 */
void destroy_http_response(HttpResponsePtr http)
{
    if ( *(http->date) )           { free(http->date);           }
    if ( *(http->server) )         { free(http->server);         }
    if ( *(http->last_modified) )  { free(http->last_modified);  }
    if ( *(http->content_length) ) { free(http->content_length); }
    if ( *(http->content_type) )   { free(http->content_type);   }

    if ( http->content )
    {
        if ( *(http->content_type) )
        {
            free(http->content);
        }
    }

    free(http);
    memset(http, 0, sizeof(*http));
}

HttpResponsePtr parse_http_response(const char * http_message)
{
    const char * status_line_start = http_message;
    char * status_line_end = strchr(status_line_start, '\n');

    char status_line[(status_line_end - status_line_start) + 1];
    strncpy(status_line, status_line_start, (unsigned long)(status_line_end - status_line_start) + 1);
    HttpResponsePtr http = parse_status_line(status_line);

    char * header_lines = status_line_end + 1;
    parse_response_lines(http, header_lines);

    return http;
}


void parse_response_lines(HttpResponsePtr http, char *header_lines)
{
    header_lines = parse_response_line(http, header_lines, set_date);
    header_lines = parse_response_line(http, header_lines, set_server);
    header_lines = parse_response_line(http, header_lines, set_last_modified);
    header_lines = parse_response_line(http, header_lines, set_content_length);
    header_lines = parse_response_line(http, header_lines, set_content_type);
    parse_content(http, header_lines);
}


char * parse_response_line(HttpResponsePtr http, char * header_line, void (setter)(HttpResponsePtr, char *))
{
    char * attr_start = NULL;
    char * end = NULL;

    attr_start = strchr(header_line, ' ') + 1;
    end = strchr(attr_start, '\r');

    char * copy = malloc((unsigned long) (end - attr_start));
    memmove(copy, attr_start,(unsigned long) (end - attr_start));

    setter(http, copy);
    return (end);
}


void parse_content(HttpResponsePtr http, char * header_line)
{
    char * content = malloc((unsigned long) (get_content_length_long(http) + 3));
    memmove(content, header_line, (unsigned long) get_content_length_long(http) + 1);
    set_content(http, content);
}


HttpResponsePtr parse_status_line(char * request_line)
{
    // get version start/end
    const char * version_start = request_line;
    const char * version_end = strchr(request_line, ' ');

    // get status code start/end
    const char * status_code_start = version_end + 1;
    const char * status_code_end = strchr(status_code_start, ' ');

    // get status start end
    const char * status_start = status_code_end + 1;
    const char * status_end = strchr(status_start, '\n');

    char * version = malloc((unsigned long) ((version_end - version_start) + 1));
    char * status_code = malloc((unsigned long) ((status_code_end - status_code_start) + 1));
    char * status = malloc((unsigned long) ((status_end - status_start) + 1));

    memmove(version, version_start, (unsigned long) (version_end - version_start));
    memmove(status_code, status_code_start, (unsigned long) (status_code_end - status_code_start));
    memmove(status, status_start, (unsigned long) (status_end - status_start));

    return http_response_constructor(version, status_code, status);
}
