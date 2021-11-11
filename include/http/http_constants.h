//
// Created by jordan on 2021-11-08.
//

#ifndef IBEACON_HTTP_CONSTANTS_H
#define IBEACON_HTTP_CONSTANTS_H

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

#endif //IBEACON_HTTP_CONSTANTS_H
