//
// Created by jordan on 2021-11-07.
//

//#include "http/http_request.h"

#include "http/http_response.h"
#include "http/http_request.h"

//// =================================================================
//// --- T O -- D O -- L I S T ---------------------------------------
//// =================================================================
////    1. Figure out where dc_posix env/err will be required
////    2. Change necessary function prototypes to include dc_posix
////    3. Move parsing function to separate header/source file ????
////    4. *** IMPLEMENT PARSING FUNCTIONS ***
//// =================================================================

int main(void)
{
    parse_http_request(test_request_message);
//    destroy_http_request(http);
//    parse_http_response(test_response_message);
}
//// ===============================
//// H T T P    B A S E    C L A S S
//// ===============================
//struct Http {
//    enum HTTP_TYPE  type;           // REQUEST or RESPONSE
//    char *          http_version;   // http version
//    char *          connection;     // connection type
//    parse_http      parse;          // pointer to parse function (for either REQUEST or RESPONSE)
//};
//
///*
// *  HTTP CONSTRUCTOR
// */
//    HttpPtr http_constructor(enum HTTP_TYPE http_type, char * http_version, char * connection)
//    {
//        HttpPtr http = malloc(sizeof * http);
//        if (http)
//        {
//            http->type = http_type;
//            http->http_version = http_version;
//            http->connection =connection;
//        }
//
//        return http;
//    }
//
///*
// * Getters
// */
//    enum HTTP_TYPE get_http_type(HttpPtr http)
//    {
//        if (http)
//        {
//            return http->type;
//        }
//    }
//
//    char * get_version(HttpPtr http)
//    {
//        if (http)
//        {
//            return http->http_version;
//        }
//    }
//
//    char * get_connection(HttpPtr http)
//    {
//        if (http)
//        {
//            return http->connection;
//        }
//    }
//
///*
// * Setters
// */
//    void set_http_type(HttpPtr http, enum HTTP_TYPE http_type)
//    {
//        if (http)
//        {
//            http->type = http_type;
//        }
//    }
//
//    void set_version(HttpPtr http, char * version)
//    {
//        if (http)
//        {
//            http->http_version = version;
//        }
//    }
//
//    void set_connection(HttpPtr http, char * connection)
//    {
//        if (http)
//        {
//            http->connection = connection;
//        }
//    }
//
///*
//* Destructor
//*/
//    void destroy_http(HttpPtr http)
//    {
//        if (http)
//        {
//            free(http);
//        }
//    }
















