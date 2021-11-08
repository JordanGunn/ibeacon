//
// Created by jordan on 2021-11-07.
//

#include "http.h"


// ===============================
// H T T P    B A S E    C L A S S
// ===============================
struct Http {
    enum HTTP_TYPE  type;           // REQUEST or RESPONSE
    char *          http_version;   // http version
    char *          connection;     // connection type
    parse_http      parse;          // pointer to parse function (for either REQUEST or RESPONSE)
};

/*
 *  HTTP CONSTRUCTOR
 */
    HttpPtr http_constructor(enum HTTP_TYPE http_type, char * http_version, char * connection, parse_http parse)
    {
        HttpPtr http = malloc(sizeof * http);
        if (http)
        {
            http->type = http_type;
            http->http_version = http_version;
            http->connection =connection;
            http->parse = parse;
        }
    }

/*
 * Getters
 */
    enum HTTP_TYPE get_http_type(HttpPtr http)
    {
        if (http)
        {
            return http->type;
        }
    }

    char * get_version(HttpPtr http)
    {
        if (http)
        {
            return http->http_version;
        }
    }

    char * get_connection(HttpPtr http)
    {
        if (http)
        {
            return http->connection;
        }
    }

/*
 * Setters
 */
    void set_http_type(HttpPtr http, enum HTTP_TYPE http_type)
    {
        if (http)
        {
            http->type = http_type;
        }
    }

    void set_version(HttpPtr http, char * version)
    {
        if (http)
        {
            http->http_version = version;
        }
    }

    void set_connection(HttpPtr http, char * connection)
    {
        if (http)
        {
            http->connection = connection;
        }
    }

/*
* Destructor
*/
    void destroy_http(HttpPtr http)
    {
        if (http)
        {
            free(http);
        }
    }


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
HttpResponsePtr http_response_constructor(HttpPtr * const http, enum HTTP_TYPE http_type, char * http_version, char * connection, parse_http parse)
{
    HttpResponsePtr http_response = malloc(sizeof * http_response);
    if (http_response)
    {
        HttpPtr super = http_constructor(RESPONSE, http_version, connection, parse);
        http_response->super = super;
    }
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
            return http->status;
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
HttpRequestPtr http_request_constructor(HttpPtr http, char * http_version, char * connection, parse_http parse)
{
    HttpRequestPtr http_request = malloc(sizeof * http_request);
    if (http_request)
    {
        HttpPtr super = http_constructor(REQUEST, http_version, connection, parse);
        http_request->super = super;
    }
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

void parse_http_request(char * http_message)
{

}

void parse_http_response(char * http_message)
{

}





