//
// Created by jordan on 2021-11-10.
//

#ifndef IBEACON_HTTP_REQUEST_H
#define IBEACON_HTTP_REQUEST_H

#include "common.h"

typedef struct HttpRequest * HttpRequestPtr;

// ============================
// CHILD HTTP REQUEST CLASS
// ============================
/*
 * CONSTRUCTOR
 */
HttpRequestPtr http_request_constructor(char *, char *, char *);

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
char * get_connection(HttpRequestPtr);
char * get_version(HttpRequestPtr);

/*
 * Setters
 */
void set_method(HttpRequestPtr, char *);
void set_url(HttpRequestPtr, char *);
void set_host(HttpRequestPtr, char *);
void set_user_agent(HttpRequestPtr, char *);
void set_accept_language(HttpRequestPtr, char *);
void set_accept(HttpRequestPtr, char *);
void set_connection(HttpRequestPtr http, char * connection);
void set_version(HttpRequestPtr http, char * version);



// ==================================
// CHILD HTTP PARSE REQUEST FUNCTIONS
// ==================================
/**
 * Parse incoming HTTP Request.
 *
 * @param http_message
 */
HttpRequestPtr parse_http_request(char *);
HttpRequestPtr parse_request_line(char *);
void parse_header_lines(HttpRequestPtr http, char *header_lines);
char * parse_header_line(HttpRequestPtr,char *,void (setter)(HttpRequestPtr, char *));

#endif IBEACON_HTTP_REQUEST_H
