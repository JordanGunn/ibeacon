//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_HTTP_H
#define IBEACON_HTTP_H

#include <common.h>
#include <http/http_constants.h>


typedef void (*parse_http) (char * http_message);
typedef struct Http * HttpPtr;


// ==========================
// PARENT HTTP CLASS
// ==========================
/*
 * Constructor
 */
HttpPtr http_constructor(enum HTTP_TYPE, char * version, char * connection);

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


#endif IBEACON_HTTP_H
