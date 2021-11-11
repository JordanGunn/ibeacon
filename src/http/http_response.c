//
// Created by jordan on 2021-11-10.
//

#include "http/http_response.h"


// ============================
// R E S P O N S E    C L A S S
// ============================
struct HttpResponse {
    HttpPtr super;
    char *  status;          // response status
    char *  status_code;     // response status code
    char *  date;            // e.g. Th, 08 Aug 2013 23:54:35 GMT
    char *  server;          // Server software spec (e.g. Apache/2.4.39 (CentOS))
    char *  last_modified;   // e.g. Th, 08 Aug 2013 23:54:35 GMT
    char *  content_type;    // e.g. "text/html"
    ssize_t content_length; // Length of object in bytes
} ;

/*
 * RESPONSE CONSTRUCTOR
 */
HttpResponsePtr http_response_constructor(enum HTTP_TYPE http_type, char * http_version, char * connection)
{
    HttpResponsePtr http_response = malloc(sizeof * http_response);
    if (http_response)
    {
        HttpPtr super = http_constructor(RESPONSE, http_version, connection);
        http_response->super = super;
    }

    return http_response;
}

/*
 * Getters
 */
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

ssize_t get_content_length(HttpResponsePtr http)
{
    if (http)
    {
        return http->content_length;
    }
}


/*
 * Setters
 */
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

void set_content_length(HttpResponsePtr http, ssize_t content_length)
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

/*
 * destructor
 */
void destroy_http_response(HttpResponsePtr http)
{
    if (http)
    {
        free(http);
    }
}

void parse_http_response(HttpResponsePtr http, char * http_message)
{
    //even the response, we may be able to just use hardcoded messages.
}
