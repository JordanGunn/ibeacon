//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_HTTP_H
#define IBEACON_HTTP_H

#include <common.h>

typedef void (*parse_http) (char * http_message);
typedef struct HttpResponse * HttpResponsePtr;
typedef struct HttpRequest * HttpRequestPtr;
typedef struct Http * HttpPtr;

static const char response_okay[4] = "200";
static const char response_file_not_found[4] = "404";
static const char response_bad_request[4] = "400";
static const char response_server_error[4] = "500";
static const char response_version_not_supported[4] = "505";

static const char response_msg_okay[3] = "OK";
static const char response_msg_file_not_found[15] = "File Not Found";
static const char response_msg_bad_request[12] = "Bad Request";
static const char response_msg_server_error[27] = "HTTP Internal Server Error";
static const char response_msg_version_not_supported[27] = "HTTP Version Not Supported";


enum HTTP_METHODS {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};

enum HTTP_TYPE {
    REQUEST,
    RESPONSE
};

typedef struct {
    const char * code;
    const char * status;
} status_code_pair;



status_code_pair status_code_map[] = {
        { response_okay,                  response_msg_okay                  },
        { response_file_not_found,        response_msg_file_not_found        },
        { response_bad_request,           response_msg_bad_request           },
        { response_server_error,          response_msg_server_error          },
        { response_version_not_supported, response_msg_version_not_supported }
};


// ==========================
// PARENT HTTP CLASS
// ==========================
/*
 * Constructor
 */
HttpPtr http_constructor(enum HTTP_TYPE, char * version, char * connection, parse_http);

    /*
     * Destructor
     */
    void destroy_http(HttpPtr);

    /*
     * Getters
     */
    enum HTTP_TYPE get_http_type(HttpPtr http);
    char * get_version(HttpPtr http);
    char * get_connection(HttpPtr http);

    /*
     * Setters
     */
    void set_http_type(HttpPtr http, enum HTTP_TYPE http_type);
    void set_version(HttpPtr http, char * version);
    void set_connection(HttpPtr http, char * connection);


// ============================
// CHILD HTTP RESPONSE CLASS
// ============================

/*
 * CONSTRUCTOR
 */
HttpResponsePtr http_response_constructor(HttpPtr);

    /*
     * destructor
     */
    void destroy_http_response(HttpResponsePtr);

    /*
     * Getters
     */
    char *  get_status(void);
    char *  get_status_code(void);
    char *  get_date(void);
    char *  get_server(void);
    char *  get_last_modified(void);
    char *  get_content_type(void);
    ssize_t get_content_length(void);

    /*
     * Setters
     */
    void  set_status(char * status);
    void  set_status_code(char * status_code);
    void  set_date(char * date);
    void  set_server(char * server);
    void  set_last_modified(char * last_modified);
    void  set_content_length(ssize_t content_length);
    void  set_content_type(char * content_type);


// ============================
// CHILD HTTP RESPONSE CLASS
// ============================
/*
 * CONSTRUCTOR
 */
HttpRequestPtr http_request_constructor(HttpPtr);

    /*
     * Destructor
     */
    void destroy_http_request(HttpRequestPtr);

    /*
     * Getters
     */
    char * get_method(void);
    char * get_url(void);
    char * get_host(void);
    char * get_user_agent(void);
    char * get_accept_language(void);
    char * get_accept(void);

    /*
     * Setters
     */
    void set_method(char * method);
    void set_url(char * url);
    void set_host(char * host);
    void set_user_agent(char * user_agent);
    void set_accept_language(char * accept_language);
    void set_accept(char * accept);



// ==================================
// CHILD HTTP PARSE REQUEST FUNCTIONS
// ==================================
/**
 * Parse incoming HTTP Request.
 *
 * @param http_message
 */
void parse_http_request(char * http_message);

/**
 * Parse incoming HTTP request.
 *
 * @param http_message
 */
void parse_http_response(char * http_message);




#endif IBEACON_HTTP_H
