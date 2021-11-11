//
// Created by jordan on 2021-11-10.
//

#ifndef IBEACON_HTTP_RESPONSE_H
#define IBEACON_HTTP_RESPONSE_H

#include "common.h"
#include "http_constants.h"


typedef struct HttpResponse * HttpResponsePtr;

// ============================
// CHILD HTTP RESPONSE CLASS
// ============================

/*
 * CONSTRUCTOR
 */
HttpResponsePtr http_response_constructor(char * version, char * status_code, char * status);

/*
 * destructor
 */
void destroy_http_response(HttpResponsePtr);

/*
 * Getters
 */
char *  get_res_version(HttpResponsePtr);
char *  get_res_connection(HttpResponsePtr);
char *  get_status(HttpResponsePtr);
char *  get_status_code(HttpResponsePtr);
char *  get_date(HttpResponsePtr);
char *  get_server(HttpResponsePtr);
char *  get_last_modified(HttpResponsePtr);
char *  get_content_type(HttpResponsePtr);
ssize_t get_content_length(HttpResponsePtr);
char * get_content(HttpResponsePtr http);

/*
 * Setters
 */
void  set_res_version(HttpResponsePtr, char *);
void  set_res_connection(HttpResponsePtr, char *);
void  set_status(HttpResponsePtr, char *);
void  set_status_code(HttpResponsePtr, char *);
void  set_date(HttpResponsePtr, char *);
void  set_server(HttpResponsePtr, char *);
void  set_last_modified(HttpResponsePtr, char *);
void  set_content_type(HttpResponsePtr, char *);
void  set_content_length(HttpResponsePtr, char *);
void  set_content(HttpResponsePtr, char *);



/**
 * Parse incoming HTTP request.
 *
 * @param http_message
 */
HttpResponsePtr parse_http_response(const char *);
HttpResponsePtr parse_status_line(char *);
void parse_response_lines(HttpResponsePtr, char *);
char * parse_response_line(HttpResponsePtr, char *, void (setter)(HttpResponsePtr, char *));
void parse_content(HttpResponsePtr http, char * header_line);

#endif IBEACON_HTTP_RESPONSE_H
