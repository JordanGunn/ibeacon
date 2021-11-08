//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_HTTP_H
#define IBEACON_HTTP_H

#include <common.h>
#include <http_constants.h>


typedef void (*parse_http) (char * http_message);
typedef struct HttpResponse * HttpResponsePtr;
typedef struct HttpRequest * HttpRequestPtr;
typedef struct Http * HttpPtr;


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
HttpResponsePtr http_response_constructor(HttpPtr * , enum HTTP_TYPE, char *, char *, parse_http)
;

    /*
     * destructor
     */
    void destroy_http_response(HttpResponsePtr);

    /*
     * Getters
     */
    char *  get_status(HttpResponsePtr);
    char *  get_status_code(HttpResponsePtr);
    char *  get_date(HttpResponsePtr);
    char *  get_server(HttpResponsePtr);
    char *  get_last_modified(HttpResponsePtr);
    char *  get_content_type(HttpResponsePtr);
    ssize_t get_content_length(HttpResponsePtr);

    /*
     * Setters
     */
    void  set_status(HttpResponsePtr, char *);
    void  set_status_code(HttpResponsePtr, char *);
    void  set_date(HttpResponsePtr, char *);
    void  set_server(HttpResponsePtr, char *);
    void  set_last_modified(HttpResponsePtr, char *);
    void  set_content_type(HttpResponsePtr, char *);
    void  set_content_length(HttpResponsePtr, ssize_t);



// ============================
// CHILD HTTP REQUEST CLASS
// ============================
/*
 * CONSTRUCTOR
 */
HttpRequestPtr http_request_constructor(HttpPtr, char *, char *, parse_http);

    /*
     * Destructor
     */
    void destroy_http_request(HttpRequestPtr);

    /*
     * Getters
     */
    char * get_method(HttpRequestPtr);
    char * get_url(HttpRequestPtr);
    char * get_host(HttpRequestPtr);
    char * get_user_agent(HttpRequestPtr);
    char * get_accept_language(HttpRequestPtr);
    char * get_accept(HttpRequestPtr);

    /*
     * Setters
     */
    void set_method(HttpRequestPtr, char *);
    void set_url(HttpRequestPtr, char *);
    void set_host(HttpRequestPtr, char *);
    void set_user_agent(HttpRequestPtr, char *);
    void set_accept_language(HttpRequestPtr, char *);
    void set_accept(HttpRequestPtr, char *);



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
