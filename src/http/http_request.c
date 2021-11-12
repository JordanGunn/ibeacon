//
// Created by jordan on 2021-11-10.
//

#include "http/http_request.h"


// ===========================
// R E Q U E S T    C L A S S
// ===========================

struct HttpRequest {
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
        http_request->method = method;
        http_request->url = url;
        http_request->version = version;

        return http_request;
    }
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
}

char * get_method(HttpRequestPtr http)
{
    if (http)
    {
        return http->method;
    }
}

char * get_url(HttpRequestPtr http)
{
    if (http)
    {
        return http->url;
    }
}

char * get_host(HttpRequestPtr http)
{
    if (http)
    {
        return http->host;
    }
}

char * get_user_agent(HttpRequestPtr http)
{
    if (http)
    {
        return http->user_agent;
    }
}

char * get_accept(HttpRequestPtr http)
{
    if (http)
    {
        return http->accept;
    }
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
    if (http->version)          { free(http->version);          }
    if (http->method)           { free(http->method);           }
    if (http->url)              { free(http->url);              }
    if (http->host)             { free(http->host);             }
    if (http->user_agent)       { free(http->user_agent);       }
    if (http->accept)           { free(http->accept);           }

    free(http);
}

// ==================================
// CHILD HTTP PARSE REQUEST FUNCTIONS
// ==================================

HttpRequestPtr parse_http_request(char * http_message)
{
    const char * request_line_start = http_message;
    char * request_line_end = strchr(http_message, '\n');

    char request_line[(request_line_end - request_line_start) + 1];
    strncpy(request_line, request_line_start, (unsigned long)(request_line_end - request_line_start) + 1);
    HttpRequestPtr http = parse_request_line(request_line);

    char * header_lines = request_line_end + 1;
    parse_header_lines(http, header_lines);

    return http;
}


void parse_header_lines(HttpRequestPtr http, char *header_lines)
{
    header_lines = parse_header_line(http, header_lines, set_host);
    header_lines = parse_header_line(http, header_lines, set_user_agent);
    parse_header_line(http, header_lines, set_accept);
}


char * parse_header_line(HttpRequestPtr http, char * header_line, void (setter)(HttpRequestPtr, char *)) {

    char * attr_start = NULL;
    char * attr_end = NULL;

    attr_start = strchr(header_line, ' ');
    attr_end = strchr(attr_start, '\n');
    char * attr = malloc((unsigned long) ((attr_end - attr_start) + 1));
    memmove(attr, attr_start, (unsigned long) ((attr_end - attr_start) - 1));
    setter(http, attr);

    return (attr_end + 1);
}


HttpRequestPtr parse_request_line(char * request_line)
{
    const char * method_start = request_line;
    const char * method_end = strchr(request_line, ' ');

    const char * url_start = method_end + 1;
    const char * url_end = strchr(url_start, ' ');

    const char * version_start = url_end + 1;
    const char * version_end = strchr(version_start, '\n');

    char * method = malloc((unsigned long) ((method_end - method_start) + 1));
    char * url = malloc((unsigned long) ((url_end - url_start) + 1));
    char * version = malloc((unsigned long) ((version_end - version_start) + 1));

    memmove(method, method_start, (unsigned long) (method_end - method_start));
    memmove(url, url_start, (unsigned long) (url_end - url_start));
    memmove(version, version_start, (unsigned long) (version_end - version_start));

    return http_request_constructor(method, url, version);
}
