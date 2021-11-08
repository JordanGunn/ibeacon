//
// Created by jordan on 2021-11-07.
//

#include "http.h"


// ===============================
// H T T P    B A S E    C L A S S
// ===============================
struct Http {
    enum HTTP_TYPE type;    // REQUEST or RESPONSE
    char * http_version;    // http version
    char * connection;      // connection type
    parse_http parse;
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
HttpResponsePtr http_response_constructor(HttpPtr http)
{
    return NULL;
}
    /*
     * Getters
     */
    char *  get_status(void)
    {
        return NULL;
    }

    char *  get_status_code(void)
    {
        return NULL;
    }

    char *  get_date(void)
    {
        return NULL;
    }

    char *  get_server(void)
    {
        return NULL;
    }

    char *  get_last_modified(void)
    {
        return NULL;
    }

    char *  get_content_type(void)
    {
        return NULL;
    }

    ssize_t get_content_length(void)
    {
        return (ssize_t) NULL;
    }


    /*
     * Setters
     */
    void  set_status(char * status)
    {

    }

    void  set_status_code(char * status_code)
    {

    }

    void  set_date(char * date)
    {

    }

    void  set_server(char * server)
    {

    }

    void  set_last_modified(char * last_modified)
    {

    }

    void  set_content_length(ssize_t content_length)
    {

    }

    void  set_content_type(char * content_type)
    {

    }

    /*
     * destructor
     */
    void destroy_http_response(HttpResponsePtr http)
    {

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
HttpRequestPtr http_request_constructor(HttpPtr http)
{
    return NULL;
}

    /*
     * Getters
     */
    char * get_method(void)
    {
        return NULL;
    }

    char * get_url(void)
    {
        return NULL;
    }

    char * get_host(void)
    {
        return NULL;
    }

    char * get_user_agent(void)
    {
        return NULL;
    }

    char * get_accept_language(void)
    {
        return NULL;
    }

    char * get_accept(void)
    {
        return NULL;
    }

    /*
     * Setters
     */
    void set_method(char * method)
    {

    }

    void set_url(char * url)
    {

    }

    void set_host(char * host)
    {

    }

    void set_user_agent(char * user_agent)
    {

    }

    void set_accept_language(char * accept_language)
    {

    }

    void set_accept(char * accept)
    {

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





