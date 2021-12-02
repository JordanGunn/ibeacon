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

static void quit_handler ( int signum ) {

    signum = 1;
}


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

int main(void)
{
    dc_error_reporter reporter;
    struct dc_error err;
    struct dc_posix_env env;

    reporter = NULL;
    dc_error_init(&err, reporter);
    dc_posix_env_init(&env, NULL);
    struct server_params * serv;

    //HERE !!!!!!!!!!!!!!!!!!!!!!!!!!
    serv = malloc(sizeof (struct server_params));

    dc_signal(&env, &err, SIGINT, quit_handler);

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

            {SEND_RESPONSE, SERVER_INIT, init_server},
            {SERVER_INIT, DC_FSM_EXIT, NULL}
    };

    dc_error_init(&err, error_reporter);
    dc_posix_env_init(&env, trace_reporter);
    fsm_info = dc_fsm_info_create(&env, &err, "traffic");
    dc_fsm_info_set_will_change_state(fsm_info, will_change_state);
    dc_fsm_info_set_did_change_state(fsm_info, did_change_state);
    dc_fsm_info_set_bad_change_state(fsm_info, bad_change_state);

    if(dc_error_has_no_error(&err))
    {
        ret_val = EXIT_SUCCESS;
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
    else
    {
        ret_val = 1;
    }

    free(serv);
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


// ============================================
// |    S E R V E R     F U N C T I O N S     |
// ============================================

int init_server(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    // server arguments for fsm
    struct server_params * serv;
    serv = (struct server_params *) arg;
    memset(serv, 0, sizeof(struct server_params));


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

    int return_value;
    struct server_params * serv;
    serv = (struct server_params *) arg;
    struct timeval timeout;
    timeout.tv_sec = 6 * 60 * 60;
    timeout.tv_usec = 0;

    if(dc_error_has_no_error(err))
    {
        int server_socket_fd;

        server_socket_fd = dc_socket(
                env, err, serv->AddressInfoPtr->ai_family,
                 serv->AddressInfoPtr->ai_socktype,
                 serv->AddressInfoPtr->ai_protocol
             );

        dc_setsockopt(
            env, err, server_socket_fd,
            SOL_SOCKET, SO_REUSEADDR,
            &(int){1}, sizeof(int)
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
                puts("bind");
                if(dc_error_has_no_error(err))
                {
                    int backlog;

                    backlog = 5;

                    dc_listen(env, err, server_socket_fd, backlog);

                    if ( dc_error_has_no_error(err) )
                    {
                        struct sigaction old_action;

                        dc_sigaction(env, err, SIGINT, NULL, &old_action);

                        if ( old_action.sa_handler != SIG_IGN )
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
                                    receive_data(env, err, serv->client_socket_fd, LARGE_BUFF, serv);
                                    exit_flag = (serv->request)
                                    ? 1 : 0;
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
            else
            {
                // otherwise, return an error
                serv->error = ERROR_500;
                return ERROR_500;
            }
        }


        if(dc_error_has_no_error(err))
        {
            dc_close(env, err, server_socket_fd);


            if (!dc_strcmp(env, get_method(serv->request), REQUEST_GET))
            {
                return_value = HTTP_GET;
            }
            else if (!dc_strcmp(env, get_method(serv->request), REQUEST_POST))
            {
                return_value = HTTP_POST;
            }
            else
            {
                return_value = DC_FSM_EXIT;
            }
        }
    }
    return return_value;
}

int http_get(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    char buffer[1024] = {0};


    struct Query query;
    if (dc_error_has_no_error(err))
    {
        int return_value = 1;
        if (is_html(env, get_url(serv->request)))
        {
            return_value = get_html(env, err, serv);
            if (return_value < 0)
            {
                dc_error_reset(err);
                dc_error_init(err, NULL);
                serv->error = ERROR_404;
            }
        }
        else if (is_query(get_url(serv->request)))
        {
            parse_query(get_url(serv->request), &query);
            if(dc_strcmp(env, query.key, "DUMP") != 0)
            {
                serv->fetched = fetch_data(env, err, serv->db->dbmPtr, query.key);
                serv->content = calloc((unsigned long) (serv->fetched.dsize), sizeof(char));
                memcpy(serv->content, serv->fetched.dptr, (unsigned long) serv->fetched.dsize);
            }
            else
            {
                getAllData(env, err, serv->db->dbmPtr, buffer);
                serv->content = calloc((unsigned long) (dc_strlen(env, buffer)), sizeof(char));
                memcpy(serv->content, buffer, (unsigned long) (dc_strlen(env, buffer)));
                serv->fetched.dsize = (int) dc_strlen(env, buffer);
            }

            if (!(serv->fetched.dsize))
            {
                dc_error_reset(err);
                dc_error_init(err, NULL);
                serv->error = ERROR_404;
            }
            destroy_query(&query);

        }
        else
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
        store_data(env, err, serv->db->dbmPtr, query.key, query.value, DBM_REPLACE);
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
    char    content_length[12] = {0};

    struct server_params * serv;
    serv = (struct server_params *) arg;

    response_construct_handler(env, err, serv);
    sprintf(content_length, "%d", serv->fetched.dsize);
    res_date            = malloc( sizeof(http_date)           );
    res_server          = malloc( sizeof(http_server)         );
    res_last_modified   = malloc( sizeof(http_last_modified)  );
    res_content_length  = malloc( sizeof(content_length) );
    res_content_type    = malloc( sizeof(http_content_type)   );

    memmove( res_date,           http_date,           sizeof(http_date)           );
    memmove( res_server,         http_server,         sizeof(http_server)         );
    memmove( res_last_modified,  http_last_modified,  sizeof(http_last_modified)  );
    memmove( res_content_length, content_length, sizeof(content_length) );
    memmove( res_content_type,   http_content_type,   sizeof(http_content_type)   );

    set_date(           serv->response, res_date           );
    set_server(         serv->response, res_server         );
    set_last_modified(  serv->response, res_last_modified  );
    set_content_length( serv->response, res_content_length );
    set_content_type(   serv->response, res_content_type   );
    set_content(        serv->response, serv->content      );

    return SEND_RESPONSE;
}

void response_construct_handler(const struct dc_posix_env *env, struct dc_error *err, struct server_params *serv) {
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
            get_html(env, err, serv);
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
    HttpResponsePtr http;
    http = serv->response;
    if (dc_error_has_no_error(err))
    {
        char response[
                // ============================================================================
                dc_strlen(env, get_res_version(http)) +                         // REQUEST LINE
                dc_strlen(env, get_status_code(http)) +                         // REQUEST LINE
                dc_strlen(env, get_status(http)) + STATUS_LINE_SPACE +                          // REQUEST LINE
                // =============================================================================
                DATE_LEN  + dc_strlen(env, get_date(http))               + SPACE_RN_LEN +         // HEADER LINES
                SERV_LEN  + dc_strlen(env, get_server(http))             + SPACE_RN_LEN +         // HEADER LINES
                LAST_MOD_LEN + dc_strlen(env, get_last_modified(http))      + SPACE_RN_LEN +         // HEADER LINES
                CONTENT_LENGTH_LEN + dc_strlen(env, get_content_length_str(http)) + SPACE_RN_LEN +         // HEADER LINES
                CONTENT_TYPE_LEN + dc_strlen(env, get_content_type(http))       + SPACE_RN_LEN +  RN_LEN +    // HEADER LINES
                // ============================================================================

                get_content_length_long(http)
            ];

        sprintf(response,
                "%s %s %s\r\n" \
                      "Date: %s\r\n" \
                      "Server: %s\r\n" \
                      "Last-Modified: %s\r\n" \
                      "Content-Length: %s\r\n" \
                      "Content-Type: %s\r\n\r\n" \
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


        destroy_http_request(serv->request);
        destroy_http_response(serv->response);
    }

    return SERVER_INIT;
}

// Look at the code in the client, you could do the same thing
void receive_data(const struct dc_posix_env *env, struct dc_error *err, int fd, size_t max_request_size, void * arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    char check_end[END_CHECK_BUFF] = {0};

    char data[max_request_size];
    ssize_t count;
    while(!(exit_flag) && (count = dc_read(env, err, fd, data, max_request_size)) > 0 && dc_error_has_no_error(err))
    {
        serv->request = parse_http_request(data);
        memcpy( check_end, &data[count - 4], 4 );

        if (!dc_strcmp(env, check_end, "\r\n\r\n")) { break; }
    }
}

int get_html(const struct dc_posix_env *env, struct dc_error *err, void * arg)
{
    struct server_params * serv;
    serv = (struct server_params *) arg;
    char * filename_start;
    char * filename_end;
    char * filename;
    int return_value;

    int     fd;
    char    chars[LARGE_BUFF];
    return_value = EXIT_SUCCESS;

    char abs_path[PATH_BUFF_SIZE] = {0};

    filename_start = strchr( get_url(serv->request), '/' );
    filename_end = strchr( filename_start, 'l' );
    if (serv->error == ERROR_404)
    {
        filename = malloc(9);
        memmove( filename, "404.html", 8 );
    }
    else
    {
        filename = malloc((unsigned long) (filename_end - filename_start) + 1);
        memmove( filename, filename_start + 1, (unsigned long) ( filename_end - filename_start) );
    }

    if (*filename)
    {
        sprintf(
                abs_path,
                "/home/jordan/work/protocol/projects/ibeacon/html/%s",
                filename
        );
    }


    if(dc_error_has_no_error(err)) {
        fd = dc_open(env, err, abs_path, DC_O_RDONLY, 0);
        while (dc_read(env, err, fd, chars, LARGE_BUFF) > 0)
        {
            if (dc_error_has_error(err))
            {
                break;
            }
        }

        if (dc_error_has_no_error(err))
        {
            serv->content = malloc(strlen(chars) + 1);
            memmove(serv->content, chars, strlen(chars));
            serv->fetched.dsize = (int)(strlen(chars) + 1);
            dc_close(env, err, fd);
        }
    }
    free(filename);
    return_value = fd;

    return return_value;
}

bool is_html(const struct dc_posix_env *env, char * url)
{
    bool result;
    char ext[5] = "html";
    char html_ext_buff[5] = {0};
    char * ext_start = strchr(url, '.');
    if (ext_start)
    {
        memcpy(html_ext_buff, ext_start + 1, sizeof(html_ext_buff) - 1);
        result = (!dc_strcmp(env, html_ext_buff, ext));
    }
    else { result = false ; }


    return result;
}

bool is_query(char * url)
{
    char * question = strchr(url, '?');
    char * equals = strchr(url, '=');

    return (question && equals);
}



