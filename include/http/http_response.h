//
// Created by jordan on 2021-11-10.
//

#ifndef IBEACON_HTTP_RESPONSE_H
#define IBEACON_HTTP_RESPONSE_H

#include "http.h"

typedef struct HttpResponse * HttpResponsePtr;

// ============================
// CHILD HTTP RESPONSE CLASS
// ============================

/*
 * CONSTRUCTOR
 */
HttpResponsePtr http_response_constructor(enum HTTP_TYPE, char *, char *);

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


/**
 * Parse incoming HTTP request.
 *
 * @param http_message
 */
void parse_http_response(HttpResponsePtr, char *);

#endif //IBEACON_HTTP_RESPONSE_H
