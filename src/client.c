#include <dc_posix/dc_netdb.h>
#include <dc_posix/dc_posix_env.h>
#include <dc_posix/dc_unistd.h>
#include <dc_posix/dc_signal.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/sys/dc_socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <client.h>

static void quit_handler(int sig_num);


static volatile sig_atomic_t exit_flag;


//int main(void)
//{
////    struct ibeaconInfo* ibeacon;
////    struct client_params* clientParamsPt;
////
////    ibeacon = malloc(sizeof(struct ibeaconInfo));
////    clientParamsPt = malloc(sizeof(struct client_params));
////
//////    initializeClient(clientParamsPt);
////
//
//    //ncurses??
//    dc_error_reporter reporter;
//    dc_posix_tracer tracer;
//    struct dc_error err;
//    struct dc_posix_env env;
//    const char *host_name;
//    struct addrinfo hints;
//    struct addrinfo *result;
//
//    reporter = error_reporter;
//    tracer = trace_reporter;
//    tracer = NULL;
//    dc_error_init(&err, reporter);
//    dc_posix_env_init(&env, tracer);
//
//    host_name = "localhost";
//    dc_memset(&env, &hints, 0, sizeof(hints));
//    hints.ai_family =  PF_INET; // PF_INET6;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_flags = AI_CANONNAME;
//    dc_getaddrinfo(&env, &err, host_name, NULL, &hints, &result);
//
//    if(dc_error_has_no_error(&err))
//    {
//        int socket_fd;
//
//        socket_fd = dc_socket(&env, &err, result->ai_family, result->ai_socktype, result->ai_protocol);
//
//        if(dc_error_has_no_error(&err))
//        {
//            struct sockaddr *sockaddr;
//            in_port_t port;
//            in_port_t converted_port;
//            socklen_t sockaddr_size;
//
//            sockaddr = result->ai_addr;
//            port = PORT;
//            converted_port = htons(port);
//
//            if(sockaddr->sa_family == AF_INET)
//            {
//                struct sockaddr_in *addr_in;
//
//                addr_in = (struct sockaddr_in *)sockaddr;
//                addr_in->sin_port = converted_port;
//                sockaddr_size = sizeof(struct sockaddr_in);
//            }
//            else
//            {
//                if(sockaddr->sa_family == AF_INET6)
//                {
//                    struct sockaddr_in6 *addr_in;
//
//                    addr_in = (struct sockaddr_in6 *)sockaddr;
//                    addr_in->sin6_port = converted_port;
//                    sockaddr_size = sizeof(struct sockaddr_in6);
//                }
//                else
//                {
//                    DC_ERROR_RAISE_USER(&err, "sockaddr->sa_family is invalid", -1);
//                    sockaddr_size = 0;
//                }
//            }
//
//            // CONNECT
////            struct client_params* clientParams; //need to be initialized.
//
//            if(dc_error_has_no_error(&err))
//            {
////                sendPUTrequest(&env, &err, clientParams);
//
//                dc_connect(&env, &err, socket_fd, sockaddr, sockaddr_size);
//
//                if(dc_error_has_no_error(&err))
//                {
//                    struct sigaction old_action;
//
//                    dc_sigaction(&env, &err, SIGINT, NULL, &old_action);
//
//                    if(old_action.sa_handler != SIG_IGN)
//                    {
//                        struct sigaction new_action;
//                        char data[1024];
//
//                        exit_flag = 0;
//                        new_action.sa_handler = quit_handler;
//                        sigemptyset(&new_action.sa_mask);
//                        new_action.sa_flags = 0;
//                        dc_sigaction(&env, &err, SIGINT, &new_action, NULL);
//
//                        //start GUI.
//                        //get input from the user.
//                        //build request for specific action.
//                        //accept response -> parse response.
//                        //and show the response.
//                        strcpy(data, "GET / HTTP/1.0 yes\r\n\r\n");
//
//                        if(dc_error_has_no_error(&err))
//                        {
//                            socket_fd = STDOUT_FILENO; //TESTING
//
//                            dc_write(&env, &err, socket_fd, data, strlen(data)); //writing it out to the socket.
//
//                            while(dc_read(&env, &err, socket_fd, data, 1024) > 0 && dc_error_has_no_error(&err))
//                            {
//                                printf("READ %s\n", data); //reading from the socket.
//                            }
//
//                            if(err.type == DC_ERROR_ERRNO && err.errno_code == EINTR)
//                            {
//                                dc_error_reset(&err);
//                            }
//                        }
//                    }
//                }
//            }
//        }
//
//    if(dc_error_has_no_error(&err))
//    {
//        dc_close(&env, &err, socket_fd); //CLOSE
//    }
//    }
//
//    return EXIT_SUCCESS;
//}

//void close_client(const struct dc_posix_env *env, struct dc_error *err, void* args, struct client_params* clientParamsPt ) {
//    //close all clients clienters,
//    dc_close(env, err, clientParamsPt->socket_fd); //CLOSE
//    free(clientParamsPt);
//    //free any memory allocated for each.
//
//}

static void quit_handler(int sig_num)
{
    exit_flag = 1;
}

void initializeClient(const struct dc_posix_env* env, struct dc_error* err, struct client_params* clientParams) {
    dc_error_reporter reporter;
//    dc_posix_tracer tracer;
//    struct dc_error err;
//    struct dc_posix_env env;
    const char *host_name;
    struct addrinfo hints;
    struct addrinfo *result;

//    reporter = error_reporter;
//    tracer = trace_reporter;
//    tracer = NULL;
//    dc_error_init(err, reporter);
//    dc_posix_env_init(env, NULL);

    host_name = "localhost";
    dc_memset(env, &hints, 0, sizeof(hints));
    hints.ai_family = PF_INET; // PF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    dc_getaddrinfo(env, err, host_name, NULL, &(hints), &(result));

    if (dc_error_has_no_error(err)) {
//        int socket_fd;
        clientParams->socket_fd = dc_socket(env, err, result->ai_family, result->ai_socktype, result->ai_protocol);
        dc_setsockopt(
                env, err, clientParams->socket_fd,
                SOL_SOCKET, SO_REUSEADDR,
                &(int){1}, sizeof(int)
        );

        if (dc_error_has_no_error(err)) {
            struct sockaddr *sockaddr;
            in_port_t port;
            in_port_t converted_port;
            socklen_t sockaddr_size;

            sockaddr = result->ai_addr;
            port = 8080;
            converted_port = htons(port);

            if (sockaddr->sa_family == AF_INET) {
                struct sockaddr_in *addr_in;

                addr_in = (struct sockaddr_in *) sockaddr;
                addr_in->sin_port = converted_port;
                sockaddr_size = sizeof(struct sockaddr_in);
            } else {
                if (sockaddr->sa_family == AF_INET6) {
                    struct sockaddr_in6 *addr_in;

                    addr_in = (struct sockaddr_in6 *) sockaddr;
                    addr_in->sin6_port = converted_port;
                    sockaddr_size = sizeof(struct sockaddr_in6);
                } else {
                    DC_ERROR_RAISE_USER(err, "sockaddr->sa_family is invalid", -1);
                    sockaddr_size = 0;
                }
            }

            if (dc_error_has_no_error(err))
            {
                dc_connect(env, err, clientParams->socket_fd, sockaddr, sockaddr_size);
            }
        }
    }
}


//send_request(const struct dc_posix_env *env, struct dc_error *err, struct client_params* clientParamsPt)
//{
//    //            // CONNECT
//////            struct client_params* clientParams; //need to be initialized.
////
////            if(dc_error_has_no_error(&err))
////            {
//////                sendPUTrequest(&env, &err, clientParams);
////
////
////                if(dc_error_has_no_error(&err))
////                {
////                    struct sigaction old_action;
////
////                    dc_sigaction(&env, &err, SIGINT, NULL, &old_action);
////
////                    if(old_action.sa_handler != SIG_IGN)
////                    {
////                        struct sigaction new_action;
////                        char data[1024];
////
////                        exit_flag = 0;
////                        new_action.sa_handler = quit_handler;
////                        sigemptyset(&new_action.sa_mask);
////                        new_action.sa_flags = 0;
////                        dc_sigaction(&env, &err, SIGINT, &new_action, NULL);
////
////                        //start GUI.
////                        //get input from the user.
////                        //build request for specific action.
////                        //accept response -> parse response.
////                        //and show the response.
////                        strcpy(data, "GET / HTTP/1.0 yes\r\n\r\n");
////
////                        if(dc_error_has_no_error(&err))
////                        {
////                            socket_fd = STDOUT_FILENO; //TESTING
////
////                            dc_write(&env, &err, socket_fd, data, strlen(data)); //writing it out to the socket.
////
////                            while(dc_read(&env, &err, socket_fd, data, 1024) > 0 && dc_error_has_no_error(&err))
////                            {
////                                printf("READ %s\n", data); //reading from the socket.
////                            }
////
////                            if(err.type == DC_ERROR_ERRNO && err.errno_code == EINTR)
////                            {
////                                dc_error_reset(&err);
////                            }
////                        }
////                    }
////                }
////            }
////        }
////
////    if(dc_error_has_no_error(&err))
////    {
////        dc_close(&env, &err, socket_fd); //CLOSE
////    }
////    }
////
////    return EXIT_SUCCESS;
////}
//}
//
//void error_reporter(const struct dc_error *err)
//{
//    fprintf(stderr, "Error: \"%s\" - %s : %s @ %zu\n", err->message, err->file_name, err->function_name, err->line_number);
//}
//
//void trace_reporter(const struct dc_posix_env *env, const char *file_name,
//                    const char *function_name, size_t line_number)
//{
//    fprintf(stderr, "Entering: %s : %s @ %zu\n", file_name, function_name, line_number);
//}

void sendPUTrequest(const struct dc_posix_env *env, struct dc_error *err, void* args, struct client_params* clientParamsPt)// this happens every time?
{
    dc_connect(env, err, clientParamsPt->socket_fd, clientParamsPt->sockaddr, clientParamsPt->sockaddr_size);
    if(dc_error_has_no_error(err))
    {
        struct sigaction old_action;

        dc_sigaction(env, err, SIGINT, NULL, &old_action);

        if(old_action.sa_handler != SIG_IGN)
        {
            struct sigaction new_action;
            char data[1024];

            exit_flag = 0;
            new_action.sa_handler = quit_handler;
            sigemptyset(&new_action.sa_mask);
            new_action.sa_flags = 0;
            dc_sigaction(env, err, SIGINT, &new_action, NULL);
            strcpy(data, "GET / HTTP/1.0 yes\r\n\r\n");

            if(dc_error_has_no_error(err))
            {
                clientParamsPt->socket_fd = STDOUT_FILENO; //TESTING

                dc_write(env, err, clientParamsPt->socket_fd, data, strlen(data)); //writing it out to the socket.

                while(dc_read(env, err, clientParamsPt->socket_fd, data, 1024) > 0 && dc_error_has_no_error(err))
                {
                    printf("READ %s\n", data); //reading from the socket.
                }

                if(err->type == DC_ERROR_ERRNO && err->errno_code == EINTR)
                {
                    dc_error_reset(err);
                }
            }
        }
    }
}

void build_request(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    char *  req_version;
    char *  req_url;
    char *  req_host;
    char *  req_user_agent;
    char *  req_accept;

// for GUI
    struct client_params * client;
    client = (struct client_params *) arg; //key, value, method,
    req_version         = malloc( sizeof(http_version));
    req_url             = malloc(2 + sizeof(client->userInput->key) +
                                      1 + sizeof (client->userInput->value) + 1);
    sprintf(req_url, "/?%s=%s", client->userInput->key, client->userInput->value);

    req_host            = malloc( sizeof(http_host)           );
    req_user_agent      = malloc( sizeof(http_user_agent)   );
    req_accept          = malloc( sizeof(accept_type)   );

    memmove( req_version, http_version, sizeof(http_version));

    client->request = http_request_constructor(client->userInput->method, req_url, req_version);

    memmove( req_host,           http_host,           sizeof(http_host));
    memmove( req_user_agent,         http_user_agent,         sizeof(http_user_agent));
    memmove( req_accept,  accept_type,  sizeof(accept_type)  );

    set_host(           client->request, req_host           );
    set_user_agent(     client->request, req_user_agent         );
    set_accept(         client->request, req_accept  );
}


int send_request(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params * client;
    client = (struct client_params *) arg;
    HttpRequestPtr http = client->request;
    if (dc_error_has_no_error(err))
    {
        char request[
                // ============================================================================
                strlen(get_method(http)) +                         // REQUEST LINE
                strlen( get_url(http)) +                         // REQUEST LINE
                strlen( get_version(http)) + 4 +                          // REQUEST LINE
                // =============================================================================
                5  + strlen( get_host(http))               + 3 +         // HEADER LINES
                11  + strlen( get_user_agent(http))         + 3 +         // HEADER LINES
                7 + strlen( get_accept(http))            + 3 + 2     // HEADER LINES
                // ============================================================================
        ];

        sprintf(request,
                "%s %s %s\r\n" \
                "Host: %s\r\n" \
                "User-agent: %s\r\n" \
                "Accept: %s\r\n" \
                "\r\n",

                get_method(http),
                get_url(http),
                get_version(http),
                get_host(http),
                get_user_agent(http),
                get_accept(http)
        );

        if(dc_error_has_no_error(err))
        {
            struct sigaction old_action;

            dc_sigaction(env, err, SIGINT, NULL, &old_action);

            if(old_action.sa_handler != SIG_IGN)
            {
                struct sigaction new_action;

                exit_flag = 0;
                new_action.sa_handler = quit_handler;
                sigemptyset(&new_action.sa_mask);
                new_action.sa_flags = 0;
                dc_sigaction(env, err, SIGINT, &new_action, NULL);

                if(dc_error_has_no_error(err))
                {
                    dc_write(env, err, client->socket_fd, request, strlen(request));

                    if(err->type == DC_ERROR_ERRNO && err->errno_code == EINTR)
                    {
                        dc_error_reset(err);
                    }
                }
            }
        }
    }
//        destroy_http_request(client->request);
//        destroy_http_response(client->response);

}


void error_reporter(const struct dc_error *err)
{
    fprintf(stderr, "Error: \"%s\" - %s : %s @ %zu\n", err->message, err->file_name, err->function_name, err->line_number);
}
