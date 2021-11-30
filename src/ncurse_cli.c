#include <dc_fsm/fsm.h>
#include <dc_posix/dc_time.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <stdio.h>
#include <string.h>
#include <client.h>
#include <dc_posix/dc_poll.h>
#include <dc_posix/dc_netdb.h>

#define MAX_NUM_BEACONS 20 //number of ibeacons
#define MAX_MAJOR_MINOR 35 //This is max length for major + minor for the beacons.
#define DELAY_BETWEEN_PAGES 1000
#define GET_ALL_LIST_OF_BEACONS "DUMP" //key


static void initiateNCURSES();
static void closeWindow();

static int openClient(struct dc_posix_env *env, struct dc_error *err, void *arg);

static int landingPage(const struct dc_posix_env *env, struct dc_error *err, void *arg);
void showWelcomePage();
void printInTheMiddle(WINDOW *win, char * msg);
int landingPageOptions(const struct dc_posix_env *env, struct dc_error *err, WINDOW *win, struct client_params* client);
int firstSelection(WINDOW *win, struct client_params * client);



static void show_nav(WINDOW *win);
WINDOW * createOptionWindow(WINDOW *win);
static WINDOW *createInputWindow(WINDOW * win);
static void createInputMessage(WINDOW * inputWindow, char* msg);


static int getOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg);



static int getAllListOfBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg);
static int showAllBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg);
WINDOW * createWindowInTheMiddle();






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
static int red(const struct dc_posix_env *env, struct dc_error *err, void *arg);
static int green(const struct dc_posix_env *env, struct dc_error *err, void *arg);
static int yellow(const struct dc_posix_env *env, struct dc_error *err, void *arg);
static int change_colour(const struct dc_posix_env *env, struct dc_error *err, const char *name, const struct timespec *time, int next_state);
static int state_error(const struct dc_posix_env *env, struct dc_error *err, void *arg);




enum application_states
{
    LANDPAGE = DC_FSM_USER_START,
    CONNECT,    // 2
    GET_ALL,
    GET_ONE,
    SHOW_ALL,
    SHOW_ONE,
    INPUT_BEACON,
    ERROR_404,
    ERROR_500,
    ERROR,
    EXIT
};

struct pages
{
    WINDOW *homepage;
    WINDOW *optionpage;
    WINDOW *inputpage;
};

#define LOCATION_LENGTH 100
#define MAX_METHOD_LENGTH 10

int main(void)
{
    //have to ask for host name.
    dc_error_reporter reporter;
    dc_posix_tracer tracer;
    struct dc_error err;
    struct dc_posix_env env;

    reporter = NULL;
    tracer = trace_reporter;
    tracer = NULL;
    dc_error_init(&err, reporter);
    dc_posix_env_init(&env, NULL);

    int ret_val;
    struct dc_fsm_info *fsm_info;

    struct client_params * client = malloc(sizeof (struct client_params));
    static struct dc_fsm_transition transitions[] = {
            {DC_FSM_INIT,   LANDPAGE,    landingPage}, //first welcome page.
            {LANDPAGE,   GET_ALL,    getAllListOfBeacons}, //initial dump
            {LANDPAGE,   GET_ONE,    getOneBeacon}, //initial dump
            {GET_ALL,   SHOW_ALL,    showAllBeacons}, //initial dump
//            {CONNECT,   SHOW_ONE,    showOneBeacon}, //initial dump
            {SHOW_ONE,   LANDPAGE,    landingPage}, //initial dump
            {SHOW_ALL,   LANDPAGE,    landingPage}, //initial dump

//            {LANDPAGE,   EXIT,    clean_up}, //initial dump
            {EXIT,   DC_FSM_EXIT,    NULL}, //initial dump
            {ERROR_404,   DC_FSM_EXIT,    NULL}, //initial dump
            {ERROR_500,   DC_FSM_EXIT,    NULL}, //initial dump
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

        initiateNCURSES();
        ret_val = dc_fsm_run(&env, &err, fsm_info, &from_state, &to_state, &client, transitions);
        closeWindow();
        dc_fsm_info_destroy(&env, &fsm_info);
    }

    return ret_val;
}

static int getOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{

    printf("Printing get one beacon.");
    return SHOW_ONE;
}

static int landingPage(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* clientParams;
    clientParams = (struct client_params*) arg;
    int selection_made = -1;

//    showWelcomePage();

    while (selection_made < 0) { //trying this to keep the window open until quit.
        WINDOW *win = createWindowInTheMiddle(); //this is the master window.
        selection_made = landingPageOptions(env, err, win, clientParams);
    }

    return GET_ALL;
}



//int firstSelection(WINDOW *win, struct client_params * client)
//{
//    //creating a window for the input
//    WINDOW * inputWindow = createInputWindow(win);
//
//    //printout the input to the input window.
//    int choice;
//    int cont_application = false;
//
//    bool input_flag = false;
//    while ((choice = getch()) != QUIT_CHAR) {
//        if (choice == HOME_CHAR) {
//            cont_application = true;
//            break;
//        }
//        switch(choice) {
//            case '1':
//                createInputMessage(inputWindow, "You have selected to find your ibeacon.");
//                //press to confirm.
//                if (confirm(win)) {
//                    //move on to the next window.
//                    showIbeaconsList(win, client);
//                };
//                break;
//            case '2':
//                createInputMessage(inputWindow, "You have selected to find add your ibeacon.");
//                //press to confirm.
//                if (confirm(win)) {
//                    showAddPage(win);
//                }
//                break;
//            default:
//                wclear(inputWindow);
//                box(inputWindow, 0, 0);
//                mvwprintw(inputWindow, 1, 1, "It is not a valid respond.");
//                break;
//        }
//        wrefresh(inputWindow);
//    }
//    wrefresh(win);
//
//    return cont_application;
//}


int landingPageOptions(const struct dc_posix_env *env, struct dc_error *err, WINDOW *win, struct client_params* client)
{
    WINDOW * optionWindow;
    int selection;
    optionWindow = createOptionWindow(win);

    int ret_val;

    char method[MAX_METHOD_LENGTH] ={0};
    char * method_begin = method;

    //option 1: FIND_ibeacon
    do
    {
        mvwprintw(optionWindow, 1, 1, "PRESS 1: FIND IBEACON"); //if these options change, we also need to change the switch options.
        mvwprintw(optionWindow, 2, 1, "PRESS 2: ADD NEW IBEACON");
        wrefresh(optionWindow);
        selection = wgetch(optionWindow);
    } while ((selection != '1') && (selection != '2'));

    printf("landing page: before input window?");

    //displayIbeacons
    //selectIbeacon
    //ShowIbeacon
    client->userInput = malloc(sizeof(struct userInput));

    WINDOW *inputWindow = createInputWindow(win);
    switch (selection)
    {
        case '1':
            createInputMessage(inputWindow, "YOU HAVE PRESSED TO FIND IBEACONS.");
            dc_memcpy(env, method, REQUEST_GET, sizeof (REQUEST_GET) + 1);
            ret_val = SHOW_ALL;
            break;
        case '2':
            createInputMessage(createInputWindow(win), "YOU HAVE PRESSED TO ADD NEW IBEACON");
            dc_memcpy(env, method, REQUEST_PUT, sizeof (REQUEST_PUT) + 1);
            ret_val = INPUT_BEACON;
            break;
    }

    client->userInput->method = dc_malloc(env, err, sizeof (method) + 1);
    dc_memcpy(env, client->userInput->method, method, sizeof (method) + 1);
    delay_output(DELAY_BETWEEN_PAGES);
    return ret_val;
}
/**
 * Creates the window and display.
 *
 * @param env
 * @param err
 * @param arg
 * @return
 */
static int showAllBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* clientParams;
    clientParams = (struct client_params*) arg;

    printf("In showing all beacons.\n");

    return CONNECT;
}
//
//static int openClient(struct dc_posix_env *env, struct dc_error *err, void *arg)
//{
//    const char *host_name;
//    struct addrinfo hints;
//    struct addrinfo *result;
//    int ret_val;
//
//    struct client_params* clientParams;
//    clientParams = (struct client_params*) arg;
//
//    host_name = "localhost";
//    dc_memset(env, &clientParams->hints, 0, sizeof(clientParams->hints));
//    clientParams->hints.ai_family = PF_INET; // PF_INET6;
//    clientParams->hints.ai_socktype = SOCK_STREAM;
//    clientParams->hints.ai_flags = AI_CANONNAME;
//    env->tracer = NULL;
//    dc_getaddrinfo(env, err, host_name, NULL, &(clientParams->hints), &(clientParams->result));
//
//    if (dc_error_has_no_error(err))
//    {
//        clientParams->socket_fd = dc_socket(env, err, clientParams->result->ai_family, clientParams->result->ai_socktype, clientParams->result->ai_protocol);
//
//        if (dc_error_has_no_error(err))
//        {
//            in_port_t port;
//            in_port_t converted_port;
//
//            clientParams->sockaddr = clientParams->result->ai_addr;
//            port = 8080;
//            converted_port = htons(port);
//
//            if (clientParams->sockaddr->sa_family == AF_INET)
//            {
//                struct sockaddr_in *addr_in;
//
//                addr_in = (struct sockaddr_in *) clientParams->sockaddr;
//                addr_in->sin_port = converted_port;
//                clientParams->sockaddr_size = sizeof(struct sockaddr_in);
//            } else
//            {
//                if (clientParams->sockaddr->sa_family == AF_INET6)
//                {
//                    struct sockaddr_in6 *addr_in;
//
//                    addr_in = (struct sockaddr_in6 *) clientParams->sockaddr;
//                    addr_in->sin6_port = converted_port;
//                    clientParams->sockaddr_size = sizeof(struct sockaddr_in6);
//                } else {
//                    DC_ERROR_RAISE_USER(&err, "sockaddr->sa_family is invalid", -1);
//                    clientParams->sockaddr_size = 0;
//                }
//            }
//
//            if (dc_error_has_no_error(err))
//            {
//                dc_connect(env, err, clientParams->socket_fd, clientParams->sockaddr, clientParams->sockaddr_size);
//            }
//        }
//    }
//    return 0;
//}


static int getAllListOfBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* client;

    client = (struct client_params*) arg;

    initializeClient(env, err, client);

    char dump[5] = GET_ALL_LIST_OF_BEACONS;

    client->userInput->key = dump;
    client->userInput->value = dump;

    char data[1024];

    build_request(env, err, client);
    send_request(env, err, client);

    while(dc_read(env, err, client->socket_fd, data, 1024) > 0 && dc_error_has_no_error(err)) {

    }
    if (dc_error_has_no_error(err))
    {
        client->response = parse_http_response(data);
    }

    if (dc_error_has_no_error(err))
    {
        dc_close(env, err, client->socket_fd);
    }

    if (dc_error_has_error(err))
    {
        return ERROR;
    }

    return SHOW_ALL;
}



// ============================================
// |        F S M    F U N C T I O N S        |
// ============================================


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
// |    N C U R S E S    F U N C T I O N S    |
// ============================================


static void initiateNCURSES()
{
    initscr();
    clear();
    cbreak();
    noecho();
    curs_set(0); //0 - invisible
}

static void closeWindow()
{
    endwin();
}

#define DELAY_MS 2000
void showWelcomePage()
{
    printInTheMiddle(stdscr, "Welcome");
    refresh();
    delay_output(DELAY_MS);
}



#define SCREEN_RATIO 0.75
/**
 * This creates the input window in the middle.
 *
 * @return WINDOW pointer.
 */
WINDOW * createWindowInTheMiddle()
{
    int y, x, yBeg, xBeg, yMax, xMax;
    int posY, posX;
    getyx(stdscr, y, x);
    xBeg = getbegx(stdscr);
    yBeg = getbegy(stdscr);
    xMax = getmaxx(stdscr);
    yMax = getmaxy(stdscr);

    posY = (int) (yBeg + yMax * (1 - SCREEN_RATIO) / 2);
    posX = (int) (xBeg + xMax * (1 - SCREEN_RATIO) / 2);
    WINDOW *win = newwin((int) (yMax * SCREEN_RATIO), (int) (xMax * SCREEN_RATIO), posY, posX);
    refresh();

    box(win, 0, 0);
    wrefresh(win);

    return win;
}

/**
 * This creates the option window where all the information is shown.
 *
 * @param win background window
 * @return win pointer to the option window.
 */
WINDOW * createOptionWindow(WINDOW *win)
{
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);

    int posX, posY;
    posX = xBeg + (xMax - (xMax - xBeg))/2;
    posY = (yMax - yBeg)/2;
    WINDOW *optionWindow = newwin((yMax - yBeg)/2, xMax-xBeg, posY, posX);
    box(optionWindow, 0, 0);
    wrefresh(optionWindow);

    char * title = "IBEACON";
    posY = 2;
    posX = (xMax - xBeg) / 2 - (int)strlen(title)/2; //printing in the middle.
    mvwprintw(win, posY, posX, title);
    wrefresh(win);
    return optionWindow;
}

/**
 * This creates the input window on the bottom of the main window
 *
 * @param win main window pointer
 * @return WINDOW pointer to the input window.
 */
static WINDOW *createInputWindow(WINDOW * win)
{
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);

    int posX, posY;
    posX = xBeg + 2;
    posY = yMax - 1;
    WINDOW *inputWindow = newwin(3, xMax - 4, posY, posX);
    box(inputWindow, 0, 0);
    wmove(inputWindow,1, 1);
    wrefresh(inputWindow);
    return inputWindow;
}

static void createInputMessage(WINDOW * inputWindow, char* msg)
{
    wclear(inputWindow);
    box(inputWindow, 0, 0);
    mvwprintw(inputWindow, 1, 1, msg);
    wrefresh(inputWindow);
}

void printInTheMiddle(WINDOW *win, char * msg)
{
    int y, x, yBeg, xBeg, yMax, xMax;
    getyx(win, y, x);
    getbegyx(win, yBeg, xBeg);
    getmaxyx(win, yMax, xMax);

    int shift = (int)strlen(msg)/2;
    mvprintw(yMax/2, xMax/2 - shift, msg);
    wrefresh(win);
}

void trace_reporter(const struct dc_posix_env *env, const char *file_name,
                    const char *function_name, size_t line_number)
{
    fprintf(stderr, "Entering: %s : %s @ %zu\n", file_name, function_name, line_number);
}
//no need for nav bar.
//static void show_nav(WINDOW *win)
//{
//    int y, x, yBeg, xBeg, yMax, xMax;
//    getyx(win, y, x);
//    getbegyx(win, yBeg, xBeg);
//    getmaxyx(win, yMax, xMax);
//
//    mvprintw(yBeg, xBeg + 1, "q: QUIT   h: HOME");
//    wrefresh(win);
//}
