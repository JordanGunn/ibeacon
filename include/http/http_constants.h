//
// Created by jordan on 2021-11-08.
//

#ifndef IBEACON_HTTP_CONSTANTS_H
#define IBEACON_HTTP_CONSTANTS_H

// RESPONSE ATTRIBUTES
static const char connection[6] = "close";
static const char date[8] = "unknown";
static const char server[16] = "MegaFucker 5000";
static const char last_modified[8] = "unknown";
static const char content_type[10] = "text/html";

// REQUEST ATTRIBUTES
static const char version[9] = "HTTP/1.1";
static const char host[10] = "localhost";
static const char user_agent[12] = "Mozilla/5.0";
static const char accept_type[10] = "text/html";

static const int PORT = 8080;

//static const char test_request_message[400] = "GET /path/to/file/index.html HTTP/1.1\n\rHost: www.educative.io\n\rConnection: close\n\rUser-agent: Mozilla/5.0\n\rAccept-language: fr\n\rAccept: text/html\n\r";
//static const char test_response_message[400] = "HTTP/1.1 200 OK\n\rConnection: close\n\rDate: Tue, 18 Aug 2015 15: 44 : 04\n\rServer: Apache/2.2.3 (CentOS)\n\rLast-Modified: Tue, 18 Aug 2015 15:11:03 GMT \n\rContent-Length: 303\n\rContent-Type: text/html\n\r\n[The object that was requested]afsdfa\nadfasdfasdfasd\nadfasdfasdfafsda\nadsfasdfasd\n";

//

static const char REQUEST_GET[4] = "GET";
static const char REQUEST_PUT[4] = "PUT";
static const char REQUEST_POST[5] = "POST";


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



enum CODE_MAP_KEYS {
    OK,
    FILE_NOT_FOUND,
    BAD_REQUEST,
    SERVER_ERROR,
    VERSION_NOT_SUPPORTED
};

enum HTTP_METHODS {
    GET,
    POST,
    HEAD,
    PUT,
    DELETE
};

typedef struct {
    const char * code;
    const char * status;
} status_code_pair;

static status_code_pair status_code_map[] = {
        { response_okay,                  response_msg_okay                  },
        { response_file_not_found,        response_msg_file_not_found        },
        { response_bad_request,           response_msg_bad_request           },
        { response_server_error,          response_msg_server_error          },
        { response_version_not_supported, response_msg_version_not_supported }
};

#endif IBEACON_HTTP_CONSTANTS_H
