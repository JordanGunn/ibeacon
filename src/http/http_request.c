//
// Created by jordan on 2021-11-10.
//

#include "http/http_request.h"


// ===========================
// R E Q U E S T    C L A S S
// ===========================

struct HttpRequest {
    HttpPtr super;
    char *  method;          // request method
    char *  url;             // location that HTTP is referring to
    char *  host;            // host that the request is for
    char *  user_agent;      // specifies the client
    char *  accept_language; // language that is preferred
    char *  accept;          // defines the sort of response to accept. Can be HTML files, images, audio/video, etc.
};

/*
 * constructor
 */
HttpRequestPtr http_request_constructor(char * http_version, char * connection)
{
    HttpRequestPtr http_request = malloc(sizeof * http_request);
    if (http_request)
    {
        HttpPtr super = http_constructor(REQUEST, http_version, connection);
        http_request->super = super;
    }

    return http_request;
}

/*
 * Getters
 */
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

char * get_accept_language(HttpRequestPtr http)
{
    if (http)
    {
        return http->accept_language;
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

void set_accept_language(HttpRequestPtr http, char * accept_language)
{
    if (http)
    {
        http->accept_language = accept_language;
    }
}

void set_accept(HttpRequestPtr http, char * accept)
{
    if (http)
    {
        http->accept = accept;
    }
}

// ==================================
// CHILD HTTP PARSE REQUEST FUNCTIONS
// ==================================

void parse_http_request(HttpRequestPtr http, char * http_message)
{
    //we are only querying by major and minor, time, location?
    //we can hard code the buffer size for the incoming http_message.
}
