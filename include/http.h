//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_HTTP_H
#define IBEACON_HTTP_H

typedef struct HttpResponse * HttpResponsePtr;
typedef struct HttpRequest * HttpRequestPtr;
typedef struct Http * HttpPtr;


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

typedef struct status_code_pair {
    char * code;
    char * status;
} status_code_pair;


typedef status_code_pair status_code_map[] {
    { "200", "OK" },
    { "404", "File Not Found" },
    { "400", "Bad Request" },
    { "500", "HTTP Internal Server Error" },
    { "505", "HTTP Version Not Supported" }
} status_code_map;



// ==========================
// PARENT HTTP CLASS
// ==========================

typedef struct Http {
    enum HTTP_TYPE type;    // REQUEST or RESPONSE
    char * version;         // http version
    char * connection;      // connection type
} Http;

// constructor
HttpPtr http_constructor(enum HTTP_TYPE, char * version, char * connection);

// getters
enum HTTP_TYPE get_http_type(void);
char * get_version(void);
char * get_connection(void);

// setters
void set_http_type(HttpPtr http, enum HTTP_TYPE http_type);
void set_version(HttpPtr http, char * version);
void set_connection(HttpPtr http, char * connection)



// ============================
// CHILD HTTP RESPONSE CLASS
// ============================

typedef struct HttpResponse {
    Http super;
    char * status;          // response status
    char * status_code;     // response status code
    char * date;            // e.g. Th, 08 Aug 2013 23:54:35 GMT
    char * server;          // Server software spec (e.g. Apache/2.4.39 (CentOS))
    char * last_modified;   // e.g. Th, 08 Aug 2013 23:54:35 GMT
    ssize_t content_length; // Length of object in bytes
    char * content_type;    // e.g. "text/html"
} HttpResponse;

/*
 * constructor
 */
HttpResponsePtr http_response_constructor(HttpPtr);

/*
 * getters
 */
char *  get_status(void);
char *  get_status_code(void);
char *  get_date(void);
char *  get_server(void);
char *  get_last_modified(void);
char *  get_content_type(void);
ssize_t get_content_length(void);

/*
 * setters
 */
void  set_status(char * status);
void  set_status_code(char * status_code);
void  set_date(char * date);
void  set_server(char * server);
void  set_last_modified(char * last_modified);
void  set_content_length(ssize_t content_length);
void  set_content_type(char * content_type);


// ============================
// CHILD HTTP RESPONSE CLASS
// ============================

typedef struct HttpRequest{
    Http super;
    char * method;          // request method
    char * url;             // location that HTTP is referring to
    char * host;            // host that the request is for
    char * user_agent;      // specifies the client
    char * accept_language; // language that is preferred
    char * accept;          // defines the sort of response to accept. Can be HTML files, images, audio/video, etc.
} HttpRequest;

/*
 * constructor
 */
HttpRequestPtr http_request_constructor(HttpPtr);

/*
 * getters
 */
char * get_method(void);
char * get_url(void);
char * get_host(void);
char * get_user_agent(void);
char * get_accept_language(void);
char * get_accept(void);

/*
 * setters
 */
void  set_method(char * method);
void  set_url(char * url);
void  set_host(char * host);
void  set_user_agent(char * user_agent);
void  set_accept_language(char * accept_language);
void  set_accept(char * accept);



#endif //IBEACON_HTTP_H
