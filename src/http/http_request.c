//
// Created by jordan on 2021-11-10.
//

#include "http/http_request.h"


// ===========================
// R E Q U E S T    C L A S S
// ===========================

struct HttpRequest
{
    char *  version;
    char *  method;          // request method
    char *  url;             // location that HTTP is referring to
    char *  host;            // host that the request is for
    char *  user_agent;      // specifies the client
    char *  accept;          // defines the sort of response to accept. Can be HTML files, images, audio/video, etc.
};

/*
 * constructor
 */
HttpRequestPtr http_request_constructor(char * method, char * url, char * version)
{
    HttpRequestPtr http_request = malloc(sizeof * http_request);
    if (http_request)
    {
        set_method(http_request, method);
        set_url(http_request, url);
        set_version(http_request, version);

        return http_request;
    }

    return NULL;

}

/*
 * Getters
 */

char * get_version(HttpRequestPtr http)
{
    if (http)
    {
        return http->version;
    }

    return NULL;

}

char * get_method(HttpRequestPtr http)
{
    if (http)
    {
        return http->method;
    }


    return NULL;

}

char * get_url(HttpRequestPtr http)
{
    if (http)
    {
        return http->url;
    }

    return NULL;
}

char * get_host(HttpRequestPtr http)
{
    if (http)
    {
        return http->host;
    }

    return NULL;
}

char * get_user_agent(HttpRequestPtr http)
{
    if (http)
    {
        return http->user_agent;
    }

    return NULL;
}

char * get_accept(HttpRequestPtr http)
{
    if (http)
    {
        return http->accept;
    }

    return NULL;
}

/*
 * Setters
 */

void set_version(HttpRequestPtr http, char * version)
{
    if (http)
    {
        http->version = version;
    }
}

void set_method(HttpRequestPtr http, char * method)
{
    if (http)
    {
        http->method = method;
    }
}

void set_url(HttpRequestPtr http, char * url)
{
    if (http)
    {
        http->url = url;
    }
}

void set_host(HttpRequestPtr http, char * host)
{
    if (http)
    {
        http->host = host;
    }
}

void set_user_agent(HttpRequestPtr http, char * user_agent)
{
    if (http)
    {
        http->user_agent = user_agent;
    }
}

void set_accept(HttpRequestPtr http, char * accept)
{
    if (http)
    {
        http->accept = accept;
    }
}

void destroy_http_request(HttpRequestPtr http)
{
    if (http->version)          { free(http->version);    http->version = NULL;     }
    if (http->method)           { free(http->method);     http->method = NULL;      }
    if (http->url)              { free(http->url);        http->url = NULL;         }
    if (http->host)             { free(http->host);       http->host = NULL;        }
    if (http->user_agent)       { free(http->user_agent); http->user_agent = NULL;  }
    if (http->accept)           { free(http->accept);     http->accept = NULL;      }

    free(http);
}



// ==================================
// CHILD HTTP PARSE REQUEST FUNCTIONS
// ==================================

HttpRequestPtr parse_http_request(char * http_message)
{

    HttpRequestPtr http;
    char request_line[REQ_LINE_BUFF];
    const char * request_line_start;
    char * request_line_end;

    char * header_lines;

    request_line_start = http_message;
    request_line_end = strchr(http_message, '\n');

    strncpy(request_line, request_line_start, (unsigned long)(request_line_end - request_line_start) + 1);
    http = parse_request_line(request_line);

    header_lines = request_line_end + 1;
    parse_header_lines(http, header_lines);

    return http;
}


void parse_header_lines(HttpRequestPtr http, char * header_lines)
{
    header_lines = parse_header_line(http, header_lines, set_host);
    header_lines = parse_header_line(http, header_lines, set_user_agent);
    parse_header_line(http, header_lines, set_accept);
}


char * parse_header_line(HttpRequestPtr http, char * header_line, void (setter)(HttpRequestPtr, char *)) {

    char * attr_start = NULL;
    char * attr_end = NULL;
    char * attr = NULL;

    attr_start = strchr(header_line, ' ');
    attr_end = strchr(attr_start, '\n');
    attr = calloc((unsigned long) ((attr_end - attr_start) + 1), sizeof(char));
    memmove(attr, attr_start, (unsigned long) ((attr_end - attr_start) - 1));
    setter(http, attr);

    return (attr_end + 1);
}


HttpRequestPtr parse_request_line(char * request_line)
{
    HttpRequestPtr http;

    const char * method_start;
    const char * method_end;
    const char * url_start;
    const char * url_end;
    const char * version_start;
    const char * version_end;
    char * method;
    char * url;
    char * version;

    method_start = request_line;
    method_end = strchr(request_line, ' ');

    url_start = method_end + 1;
    url_end = strchr(url_start, ' ');

    version_start = url_end + 1;
    version_end = strchr(version_start, '\n');

    method = calloc((unsigned long) ((method_end - method_start) + 1), sizeof(char));
    url = calloc((unsigned long) ((url_end - url_start) + 1), sizeof(char));
    version = calloc((unsigned long) ((version_end - version_start) + 1), sizeof(char));

    memmove(method, method_start, (unsigned long) (method_end - method_start));
    memmove(url, url_start, (unsigned long) (url_end - url_start));
    memmove(version, version_start, (unsigned long) ((version_end - version_start) - 1));

    http = http_request_constructor(method, url, version);

    return http;
}

void parse_query(char * url, struct Query * query)
{
    char * key_start = NULL;
    char * key_end = NULL;
    char * value_start = NULL;
    char * value_end = NULL;

    key_start   = strchr(url, '?') + 1;
    key_end     = strchr(key_start, '=');

    value_start = key_end + 1;
    value_end   = strchr(value_start, '\0');

    query->key      = calloc((unsigned long) (key_end - key_start),     sizeof(char));
    query->value    = calloc((unsigned long) (value_end - value_start), sizeof(char));

    memmove(query->key,     key_start,  (unsigned long) (key_end - key_start)       );
    memmove(query->value,   value_start,(unsigned long) (value_end - value_start)   );
}

void destroy_query(struct Query * query)
{
    if (query)
    {
        if (query->key)
        {
            free(query->key);
            query->key = NULL;
        }

        if (query->value)
        {
            free(query->value);
            query->value = NULL;
        }
    }
}


