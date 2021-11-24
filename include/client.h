//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_CLIENT_H
#define IBEACON_CLIENT_H

#include "./http/http_constants.h"
#include "./http/http_request.h"
#include "./http/http_response.h"
#include "common.h"
#include <dc_posix/sys/dc_socket.h>
#include "netdb.h"
#include <dc_posix/dc_signal.h>

struct client_params
{
    int socket_fd;
    struct sockaddr *sockaddr;
    socklen_t sockaddr_size;
    struct addrinfo hints;
    struct addrinfo *result;

    struct userInput* userInput;
    HttpRequestPtr request;
    HttpResponsePtr response;

    char ** arrayOfCurrentBeacon;
};

struct userInput
{
    char *key;
    char *value;
    char *method;
};

/**
 * initializes the client side.
 *
 * @param arg this is the data we will parse into.
 */
void initializeClient(struct client_params* clientParams); //the arg in this case will be the ibeacon info.
void build_request(const struct dc_posix_env *env, struct dc_error *err, void *arg);
int send_request(const struct dc_posix_env *env, struct dc_error *err, void *arg);








void sendPUTrequest(const struct dc_posix_env *env, struct dc_error *err, void* args, struct client_params* clientParamsPt);// this happens every time?

void createPUTrequest();
void sendPUTrequest();

void createGETrequest();
void receiveGETmessage();
void parseGETmessage();

//Create request string to send.
    //It will be a string composed of major and minor
    //need to get rid of all string and new lines??
    //just save the string as is.

//Parse the response I get from the server.
    //is it time? or is it location?
    //Make the string: concatenating LAST FOUND TIME:
                    //LAST LOCATION:
//Somehow need to send to the ncurses to show the string.


//Need this to be working with the server.
//Need to create the branch from the master.


//initializing method for ncurses to use to create socket()? bind()? and connect()??
//or will it be in the same client.c????




#endif //IBEACON_CLIENT_H
