//
// Created by jordan on 2021-11-06.
//

#include "server.h"

#include <dc_posix/dc_netdb.h>
#include <dc_posix/dc_posix_env.h>
#include <dc_posix/dc_unistd.h>

#include <dc_posix/dc_string.h>
#include <stdio.h>
#include <stdlib.h>

static volatile sig_atomic_t exit_flag;

static void quit_handler(int sig_num);


static void will_change_state(const struct dc_posix_env *env,
                              struct dc_error *err,
                              const struct dc_fsm_info *info,
                              int from_state_id,
                              int to_state_id);
static void did_change_state(const struct dc_posix_env *env,
                             struct dc_error *err,
                             const struct dc_fsm_info *info,
                             int from_state_id,
                             int to_state_id,
                             int next_id);
static void bad_change_state(const struct dc_posix_env *env,
                             struct dc_error *err,
                             const struct dc_fsm_info *info,
                             int from_state_id,
                             int to_state_id);


void response_construct_handler(const struct dc_error *err, struct server_params *serv);

int main(void)
{
    dc_error_reporter reporter;
    dc_posix_tracer tracer;
    struct dc_error err;
    struct dc_posix_env env;

    reporter = NULL;
    tracer = trace_reporter;
    tracer = NULL;
    dc_error_init(&err, reporter);
    dc_posix_env_init(&env, tracer);

    struct server_params * serv = malloc(sizeof (struct server_params));



    int ret_val;
    struct dc_fsm_info *fsm_info;

    static struct dc_fsm_transition transitions[] = {

            {DC_FSM_INIT, SERVER_INIT,      init_server},
            {SERVER_INIT, ACCEPT_REQUEST,   accept_request},
            {SERVER_INIT, ERROR_500,        build_response},
            {ACCEPT_REQUEST, HTTP_POST, http_post},
            {ACCEPT_REQUEST, HTTP_GET, http_get},
            {ACCEPT_REQUEST, ERROR_500, build_response},

            {HTTP_POST,  BUILD_RESPONSE, build_response},
            {HTTP_POST,  ERROR_500, build_response},

            {HTTP_GET, BUILD_RESPONSE, build_response},
            {HTTP_GET, ERROR_404, build_response},

            {ERROR_500, BUILD_RESPONSE, build_response},
            {ERROR_404, BUILD_RESPONSE, build_response},

            {BUILD_RESPONSE, SEND_RESPONSE, send_response},

            {SEND_RESPONSE, DC_FSM_EXIT, NULL}
    };

    dc_error_init(&err, error_reporter);
    dc_posix_env_init(&env, trace_reporter);
    ret_val = EXIT_SUCCESS;
    fsm_info = dc_fsm_info_create(&env, &err, "traffic");
    dc_fsm_info_set_will_change_state(fsm_info, will_change_state);
    dc_fsm_info_set_did_change_state(fsm_info, did_change_state);
    dc_fsm_info_set_bad_change_state(fsm_info, bad_change_state);

    if(dc_error_has_no_error(&err))
    {
        int from_state;
        int to_state;

        ret_val = dc_fsm_run(&env, &err, fsm_info, &from_state, &to_state, serv, transitions);
        dc_fsm_info_destroy(&env, &fsm_info);
        deinitialize_database(&env, &err, serv->db);
        if (dc_error_has_no_error(&err))
        {
            dc_close(&env, &err, serv->client_socket_fd);
        }
    }

    return ret_val;
}

void error_reporter(const struct dc_error *err)
{
    fprintf(stderr, "Error: \"%s\" - %s : %s @ %zu\n", err->message, err->file_name, err->function_name, err->line_number);
}

void trace_reporter(const struct dc_posix_env *env, const char *file_name,
                           const char *function_name, size_t line_number)
{
    fprintf(stderr, "Entering: %s : %s @ %zu\n", file_name, function_name, line_number);
}

static void will_change_state(const struct dc_posix_env *env,
                              struct dc_error *err,
                              const struct dc_fsm_info *info,
                              int from_state_id,
                              int to_state_id)
{
    printf("%s: will change %d -> %d\n", dc_fsm_info_get_name(info), from_state_id, to_state_id);
}

static void did_change_state(const struct dc_posix_env *env,
                             struct dc_error *err,
                             const struct dc_fsm_info *info,
                             int from_state_id,
                             int to_state_id,
                             int next_id)
{
    printf("%s: did change %d -> %d moving to %d\n", dc_fsm_info_get_name(info), from_state_id, to_state_id, next_id);
}

static void bad_change_state(const struct dc_posix_env *env,
                             struct dc_error *err,
                             const struct dc_fsm_info *info,
                             int from_state_id,
                             int to_state_id)
{
    printf("%s: bad change %d -> %d\n", dc_fsm_info_get_name(info), from_state_id, to_state_id);
}

static int state_error(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    printf("ERROR\n");

    return DC_FSM_EXIT;
}


// ============================================
// |    S E R V E R     F U N C T I O N S     |
// ============================================

int init_server(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    // server arguments for fsm
    struct server_params * serv;
    serv = (struct server_params *) arg;


    // server connection
    struct addrinfo hints;
    struct addrinfo * result;
    const char * host_name;

    host_name = "localhost";
    dc_memset(env, &hints, 0, sizeof(hints));
    hints.ai_family =  PF_INET; // PF_INET6;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_CANONNAME;
    dc_getaddrinfo(env, err, host_name, NULL, &hints, &result);

    serv->db = initialize_database(env, err, db_name);
    serv->address_info = hints;
    serv->AddressInfoPtr = result;

//    fetch_data(env, err, serv->db->dbmPtr, name) // GET DATA

    puts("Initialize Server!");

    if (dc_error_has_error(err))
    {
        serv->error = ERROR_500;
        return ERROR_500;
    }

    return ACCEPT_REQUEST;
}


int accept_request(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{


    struct server_params * serv;
    serv = (struct server_params *) arg;

    if(dc_error_has_no_error(err))
    {
        int server_socket_fd;

        server_socket_fd = dc_socket(
                env, err, serv->AddressInfoPtr->ai_family,
                 serv->AddressInfoPtr->ai_socktype,
                 serv->AddressInfoPtr->ai_protocol
             );

        if (dc_error_has_no_error(err))
        {
            struct sockaddr * sockaddr;
            in_port_t port;
            in_port_t converted_port;
            socklen_t sockaddr_size;
            port = PORT;
            converted_port = htons(port);
            sockaddr = serv->AddressInfoPtr->ai_addr;
            if(sockaddr->sa_family == AF_INET)
            {
                struct sockaddr_in *addr_in;

                addr_in = (struct sockaddr_in *)sockaddr;
                addr_in->sin_port = converted_port;
                sockaddr_size = sizeof(struct sockaddr_in);
            }
            else
            {
                if(sockaddr->sa_family == AF_INET6)
                {
                    struct sockaddr_in6 *addr_in;

                    addr_in = (struct sockaddr_in6 *)sockaddr;
                    addr_in->sin6_port = converted_port;
                    sockaddr_size = sizeof(struct sockaddr_in6);
                }
                else
                {
                    DC_ERROR_RAISE_USER(err, "sockaddr->sa_family is invalid", -1);
                    sockaddr_size = 0;
                }
            }
            if(dc_error_has_no_error(err))
            {
                dc_bind(env, err, server_socket_fd, sockaddr, sockaddr_size);

                if(dc_error_has_no_error(err))
                {
                    int backlog;

                    backlog = 5;
                    dc_listen(env, err, server_socket_fd, backlog);

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

                            while(!(exit_flag) && dc_error_has_no_error(err))
                            {
                                serv->client_socket_fd = dc_accept(env, err, server_socket_fd, NULL, NULL);

                                if(dc_error_has_no_error(err))
                                {
                                    receive_data(env, err, serv->client_socket_fd, 8192, serv);
                                    exit_flag = 1;
                                }
                                else
                                {
                                    if(err->type == DC_ERROR_ERRNO && err->errno_code == EINTR)
                                    {
                                        dc_error_reset(err);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        if(dc_error_has_no_error(err))
        {
            dc_close(env, err, server_socket_fd);
        }



        if (!dc_strcmp(env, get_method(serv->request), REQUEST_GET))
        {
            return HTTP_GET;
        }
        else if (!dc_strcmp(env, get_method(serv->request), REQUEST_POST))
        {
            return HTTP_POST;
        }
    }

    // otherwise, return an error
    serv->error = ERROR_500;
    return ERROR_500;
}

int http_get(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    char buffer[1024] = {0};

    struct Query query;
    if (dc_error_has_no_error(err))
    {
        parse_query(get_url(serv->request), &query);
        if(dc_strcmp(env, query.key, "DUMP") != 0) {
            serv->fetched = fetch_data(env, err, serv->db->dbmPtr, query.key);
            serv->content = calloc((unsigned long) (serv->fetched.dsize), sizeof(char));
            memcpy(serv->content, serv->fetched.dptr, (unsigned long) serv->fetched.dsize);
        } else {
            getAllData(env, err, serv->db->dbmPtr, buffer);
            serv->content = calloc((unsigned long) (dc_strlen(env, buffer)), sizeof(char));
            memcpy(serv->content, buffer, (unsigned long) (dc_strlen(env, buffer)));
            serv->fetched.dsize = dc_strlen(env, buffer);
        }
        destroy_query(&query);

        if (!(*serv->content))
        {
            serv->error = ERROR_404;
        }
    }
    return BUILD_RESPONSE;
}

int http_post(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;

    struct Query query;
    if (dc_error_has_no_error(err))
    {
        parse_query(get_url(serv->request), &query);
        store_data(env, err, serv->db->dbmPtr, query.key, query.value, DBM_INSERT);
        destroy_query(&query);
    }
    else
    {
        serv->error = ERROR_404;
        return ERROR_404;
    }
    return BUILD_RESPONSE;
}

int build_response(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{

    char *  res_date;
    char *  res_server;
    char *  res_last_modified;
    char *  res_content_type;
    char *  res_content_length;
    char    content_length[12];

    struct server_params * serv;
    serv = (struct server_params *) arg;

    sprintf(content_length, "%d", serv->fetched.dsize);

    response_construct_handler(err, serv);

    res_date            = malloc( sizeof(date)           );
    res_server          = malloc( sizeof(server)         );
    res_last_modified   = malloc( sizeof(last_modified)  );
    res_content_length  = malloc( sizeof(content_length) );
    res_content_type    = malloc( sizeof(content_type)   );

    memmove( res_date,           date,           sizeof(date)           );
    memmove( res_server,         server,         sizeof(server)         );
    memmove( res_last_modified,  last_modified,  sizeof(last_modified)  );
    memmove( res_content_length, content_length, sizeof(content_length) );
    memmove( res_content_type,   content_type,   sizeof(content_type)   );

    set_date(           serv->response, res_date           );
    set_server(         serv->response, res_server         );
    set_last_modified(  serv->response, res_last_modified  );
    set_content_length( serv->response, res_content_length );
    set_content_type(   serv->response, res_content_type   );
    set_content(        serv->response, serv->content      );

    return SEND_RESPONSE;
}

void response_construct_handler(const struct dc_error *err, struct server_params *serv) {
    const char * response_code;
    const char * response_status;

    if (dc_error_has_no_error(err))
    {
        if (serv->error == ERROR_500)
        {
            // server error
            response_code   = status_code_map[ SERVER_ERROR ].code;
            response_status = status_code_map[ SERVER_ERROR ].status;
        }
        else if (serv->error == ERROR_404)
        {
            // file not found
            response_code   = status_code_map[ FILE_NOT_FOUND ].code;
            response_status = status_code_map[ FILE_NOT_FOUND ].status;
        }
        else
        {
            // OK :)
            response_code   = status_code_map[ OKAY ].code;
            response_status = status_code_map[ OKAY ].status;
        }

        serv->response = http_response_constructor
        (
            get_version(serv->request), response_code, response_status
        );
    }
}


int send_response(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    HttpResponsePtr http = serv->response;
    if (dc_error_has_no_error(err))
    {
        char response[
                // ============================================================================
                dc_strlen(env, get_res_version(http)) +                         // REQUEST LINE
                dc_strlen(env, get_status_code(http)) +                         // REQUEST LINE
                dc_strlen(env, get_status(http)) + 4 +                          // REQUEST LINE
                // =============================================================================
                5  + dc_strlen(env, get_date(http))               + 3 +         // HEADER LINES
                7  + dc_strlen(env, get_server(http))             + 3 +         // HEADER LINES
                14 + dc_strlen(env, get_last_modified(http))      + 3 +         // HEADER LINES
                15 + dc_strlen(env, get_content_length_str(http)) + 3 +         // HEADER LINES
                13 + dc_strlen(env, get_content_type(http))       + 3 +         // HEADER LINES
                // ============================================================================

                get_content_length_long(http)
            ];

        sprintf(response,
                "%s %s %s\r\n" \
                      "Date: %s\r\n" \
                      "Server: %s\r\n" \
                      "Last-Modified: %s\r\n" \
                      "Content-Length: %s\r\n" \
                      "Content-Type: %s\r\n" \
                      "%s",

                    get_res_version         ( http ),
                    get_status_code         ( http ),
                    get_status              ( http ),
                    get_date                ( http ),
                    get_server              ( http ),
                    get_last_modified       ( http ),
                    get_content_length_str  ( http ),
                    get_content_type        ( http ),
                    get_content             ( http )
        );

        if (dc_error_has_no_error(err))
        {
            dc_write(env, err, serv->client_socket_fd, response, sizeof(response));
        }


//        destroy_http_request(serv->request);
//        destroy_http_response(serv->response);
    }
}

// Look at the code in the client, you could do the same thing
void receive_data(const struct dc_posix_env *env, struct dc_error *err, int fd, size_t max_request_size, void * arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    char check_end[5] = {0};

    char data[max_request_size];
    ssize_t count;
    while(!(exit_flag) && (count = dc_read(env, err, fd, data, max_request_size)) > 0 && dc_error_has_no_error(err))
    {
        serv->request = parse_http_request(data);
        memcpy( check_end, &data[count - 4], 4 );

        if (!dc_strcmp(env, check_end, "\r\n\r\n")) { break; }
    }
}

static void quit_handler(int sig_num)
{
    exit_flag = 1;
}
