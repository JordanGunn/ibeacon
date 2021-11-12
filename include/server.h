//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_SERVER_H
#define IBEACON_SERVER_H

#include <dc_posix/dc_signal.h>
#include <dc_posix/sys/dc_socket.h>
#include "common.h"
#include "dc_fsm/fsm.h"
#include "http/http_response.h"
#include "http/http_request.h"
#include "db.h"
#include "netdb.h"

char db_name[9] = "./deebee";


struct server_params
{
    HttpRequestPtr request;
    HttpResponsePtr response;
    struct addrinfo address_info;
    struct addrinfo * AddressInfoPtr;
    struct Database * db;
    datum fetched;
    int error;
    int client_socket_fd;
};

enum application_states
{
    SERVER_INIT = DC_FSM_USER_START,
    ACCEPT_REQUEST,
    HTTP_GET,
    HTTP_POST,
    BUILD_RESPONSE,
    SEND_RESPONSE,
    ERROR_404,
    ERROR_500
};



int init_server(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int accept_request(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int http_post(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int http_get(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int build_response(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int send_response(const struct dc_posix_env *env, struct dc_error *err, void *arg);

void receive_data(const struct dc_posix_env *env, struct dc_error *err, int fd, size_t, void * arg);



#endif //IBEACON_SERVER_H
