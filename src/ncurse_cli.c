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

static int landingPage(const struct dc_posix_env *env, struct dc_error *err, void *arg);
void showWelcomePage();
void printInTheMiddle(WINDOW *win, char * msg);
int landingPageOptions(const struct dc_posix_env *env, struct dc_error *err, WINDOW *win, struct client_params* client);
int firstSelection(WINDOW *win, struct client_params * client);


static void show_nav(WINDOW *win);
WINDOW * createOptionWindow(WINDOW *win);
static WINDOW *createInputWindow(WINDOW * win);
static void createInputMessage(WINDOW * inputWindow, char* msg);
static int showAddPage(const struct dc_posix_env *env, struct dc_error *err, void *arg);
void ask_info(char * temp, WINDOW *optionWindow, WINDOW * inputWindow, char * question);
void input_with_echo_current_window(char * input, WINDOW *win);

static bool keyExistInServer(const struct dc_posix_env *env, char temp_array[MAX_NUM_BEACONS][MAX_MAJOR_MINOR], char* buffer, int num);

static int getOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg);

static int getAllListOfBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg);
static int showAllBeacons(const struct dc_posix_env *env, struct dc_error *err, void *arg);
WINDOW * createWindowInTheMiddle();
static int showOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg);






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
    SEND_INFO,
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
//            {LANDPAGE,   INPUT_BEACON,    showAddPage},
//            {INPUT_BEACON,  SEND_INFO,  }

            {GET_ALL,   SHOW_ALL,    showAllBeacons}, //initial dump
            {SHOW_ALL,   GET_ONE,    getOneBeacon}, //initial dump
            {GET_ONE,   SHOW_ONE,    showOneBeacon}, //initial dump
//            {SHOW_ONE,   LANDPAGE,    landingPage}, //initial dump
            {SHOW_ONE,   DC_FSM_EXIT,    NULL}, //initial dump

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

static int landingPage(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* clientParams;
    clientParams = (struct client_params*) arg;
    int selection_made = -1;

    clientParams->pages = dc_malloc(env, err, sizeof(struct pages));

//    showWelcomePage();

    while (selection_made < 0) { //trying this to keep the window open until quit.
        clientParams->pages->homepage = createWindowInTheMiddle(); //this is the master window.
        selection_made = landingPageOptions(env, err, clientParams->pages->homepage, clientParams);
    }

    return GET_ALL;
}

int landingPageOptions(const struct dc_posix_env *env, struct dc_error *err, WINDOW * win, struct client_params* client)
{
    WINDOW * optionWindow;
    int selection;

    client->pages->optionpage = createOptionWindow(win);
    int ret_val;

    char method[MAX_METHOD_LENGTH] ={0};
    char * method_begin = method;

    //option 1: FIND_ibeacon
    do
    {
        mvwprintw(client->pages->optionpage, 1, 1, "PRESS 1: FIND IBEACON"); //if these options change, we also need to change the switch options.
        mvwprintw(client->pages->optionpage, 2, 1, "PRESS 2: ADD NEW IBEACON");
        wrefresh(client->pages->optionpage);
        selection = wgetch(client->pages->optionpage);
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
            ret_val = GET_ALL;
            break;
        case '2':
            createInputMessage(createInputWindow(win), "YOU HAVE PRESSED TO ADD NEW IBEACON");
            dc_memcpy(env, method, REQUEST_PUT, sizeof (REQUEST_PUT) + 1);
            ret_val = INPUT_BEACON;
            break;
    }

    client->userInput->method = dc_malloc(env, err, sizeof (method) + 1);
    dc_memcpy(env, client->userInput->method, method, sizeof (method) + 1);
//    delay_output(DELAY_BETWEEN_PAGES);
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
    struct client_params* client;
    client = (struct client_params*) arg;

    int window_index = 0;
    char current_beacon[50] = {0};

    long content_length = get_content_length_long(client->response) + 3;
    char content[content_length];
    memmove(content, get_content(client->response), (unsigned long) (content_length - 3));
    memmove(content + (content_length - 3), "\r\n\0", 2);
    content[content_length - 1] = '\0';

    bool cont_application = false;
//    WINDOW *optionWindow = createOptionWindow(client->pages->homepage);
    client->pages->optionpage = createOptionWindow(client->pages->homepage);
    char * beacon_begin = content;
    char  * beacon_end = beacon_begin;
    int numberOfBeacons = 0;

//    client->arrayOfCurrentBeacon = dc_malloc(env, err, (sizeof (char*) * MAX_NUM_BEACONS));
//    char ** test = dc_malloc(env, err, (MAX_NUM_BEACONS) * (MAX_MAJOR_MINOR + 1) * sizeof(char *));
    char temp_array[MAX_NUM_BEACONS][MAX_MAJOR_MINOR];
    while (beacon_end && ++window_index)
    {
        beacon_begin += 2;
        beacon_end = strchr(beacon_begin, '\r');
        memcpy(current_beacon, beacon_begin, (unsigned long) (beacon_end - beacon_begin));

        mvwprintw(client->pages->optionpage, window_index, 1, current_beacon);
        //store into an array at same time and increment with window.
        memcpy(temp_array[window_index - 1], current_beacon, (unsigned long) (beacon_end - beacon_begin));
        memset(current_beacon, 0, sizeof(current_beacon));

        beacon_begin = beacon_end;
        beacon_end = (strchr(beacon_begin + 2, '\r')) ? beacon_end : NULL;
    }

    numberOfBeacons = window_index;
    client->numberOfBeacons = numberOfBeacons;
    // display and select iBeacons
    wrefresh(client->pages->optionpage);
    client->pages->inputpage = createInputWindow(client->pages->homepage);
    client->beacon_selection = dc_malloc(env, err, MAX_MAJOR_MINOR + 1);
    size_t size = 0;

    while (size == 0 || !keyExistInServer(env, temp_array, client->beacon_selection, numberOfBeacons)){
        if (size == 0) {
            ask_info(client->beacon_selection, client->pages->inputpage, client->pages->inputpage, "Please enter the beacon you want to see. Press enter to see.");
        } else {
            ask_info(client->beacon_selection, client->pages->inputpage, client->pages->inputpage, "beacon does not exist. Please enter correctly");
        }
        size = dc_strlen(env, client->beacon_selection);
    }

    return GET_ONE;
}

static int showOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* client;
    client = (struct client_params*) arg;
    char current_beacon[MAX_MAJOR_MINOR] = {0};

    int window_index = 0;

    long content_length = get_content_length_long(client->response) + 3;
    char content[content_length];
    memmove(content, get_content(client->response), (unsigned long) (content_length - 3));
    memmove(content + (content_length - 3), "\r\n", 2);
    content[content_length - 1] = '\0';

    bool cont_application = false;
    client->pages->optionpage = createOptionWindow(client->pages->homepage);
    mvwprintw(client->pages->optionpage, window_index, 1, current_beacon);
    // display and select iBeacons
    wrefresh(client->pages->optionpage);

//    delay_output(DELAY_BETWEEN_PAGES);

    return LANDPAGE;
}



static bool keyExistInServer(const struct dc_posix_env *env, char temp_array[MAX_NUM_BEACONS][MAX_MAJOR_MINOR], char* buffer, int num)
{
    for (int i = 0; i < num; i++) {
        if (dc_strcmp(env, temp_array[i], buffer) == 0) {
            return true;
        }
    }
    return false;
}

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
    long temp;

    while((temp = dc_read(env, err, client->socket_fd, data, 1024)) > 0 && dc_error_has_no_error(err))
    {
            //need to keep on appending to buffer.
            puts("Helloworld");
    }
    if (dc_error_has_no_error(err))
    {
        client->response = parse_http_response(data);
        puts("bytebyet");

    }

    if (dc_error_has_no_error(err))
    {
        puts("is this");
        dc_close(env, err, client->socket_fd);
        client->userInput->key = NULL;
        client->userInput->value = NULL;
    }

    if (dc_error_has_error(err))
    {
        return ERROR;
    }

    return SHOW_ALL;
}


static int getOneBeacon(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* client;
    client = (struct client_params*) arg;

    initializeClient(env, err, client);

    client->userInput->method = dc_malloc(env, err, dc_strlen(env, REQUEST_GET) + 1);
    client->userInput->key = dc_malloc(env, err, dc_strlen(env, client->beacon_selection) + 1);
//    size_t length = dc_strlen(env, client->userInput->key);
//    dc_memset(env, client->userInput->key, 0,  length + 1);
    memmove(client->userInput->key, client->beacon_selection, dc_strlen(env, client->beacon_selection) + 1);
    memcpy(client->userInput->method, REQUEST_GET, dc_strlen(env, REQUEST_GET) + 1);
    client->userInput->value = NULL;

    char data[1024];

    build_request(env, err, client);
    send_request(env, err, client);

    while(dc_read(env, err, client->socket_fd, data, 1024) > 0 && dc_error_has_no_error(err))
    {
        //need to keep on appending to buffer.
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

    return SHOW_ONE;
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




static int showAddPage(const struct dc_posix_env *env, struct dc_error *err, void *arg)
{
    struct client_params* client;

    client = (struct client_params*) arg;


//    WINDOW * optionWindow = createOptionWindow(win);
//    //Question.
//    WINDOW * inputWindow = createInputWindow(win);
//    //and get input.
//    ask_info(optionWindow, inputWindow, "Please enter MAJOR: ");
//    ask_info(optionWindow, inputWindow, "Please enter MINOR: ");
    char temp_major[MAX_MAJOR_MINOR];
    char temp_minor[MAX_MAJOR_MINOR];

    ask_info(temp_major, client->pages->optionpage, client->pages->inputpage, "Please enter MAJOR: ");
    ask_info(temp_minor, client->pages->optionpage, client->pages->inputpage,  "Please enter MINOR: ");

    client->userInput->key = dc_malloc(env, err, dc_strlen(env, temp_major) + 1);
    client->userInput->key = strcat(temp_major, temp_minor);
//    memmove(client->userInput->key, temp_major, sizeof (temp_major) + 1);
    return SEND_INFO;
}

#define DELAY_ASK_INFO 3000
void ask_info(char *temp, WINDOW *optionWindow, WINDOW * inputWindow, char * question)
{
    char *temp_val;
    createInputMessage(optionWindow, question);
    wrefresh(inputWindow);
    if (optionWindow == inputWindow) {
        //need delay
//        while(wgetch(inputWindow) != '\n') {}
        delay_output(DELAY_ASK_INFO);
    }
    //ask a question an and return an answer...???? or save an answer.
    //eventually we want to create a client's request to send to the server to save.
    input_with_echo_current_window(temp, inputWindow);
    wrefresh(inputWindow);
}

void input_with_echo_current_window(char * input, WINDOW *win)
{
    createInputMessage(win, "");
    char temp[100];
    int begY, begX;
    getbegyx(win, begY, begX);
    move(begY + 1, begX + 1);
    refresh();
    echo();
    wgetstr(win, temp);
    memmove(input, temp, sizeof (temp) + 1);
    refresh();
    createInputMessage(win, "Saved.");
    noecho();
    wrefresh(win);
}

static bool confirm(WINDOW *parentBox)
{
    //create confirmation box;
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(parentBox, yMax, xMax);
    getbegyx(parentBox, yBeg, xBeg);

    int posX, posY;
    posX = (xMax-xBeg)/2;
    posY = (yMax-yBeg)/2 + 2;
    WINDOW *confirmationBox = newwin(3, (xMax - xBeg)/2, posY, posX);
    box(confirmationBox, (int)'-', (int)'-'); // I don't know why I cant change the border.
    wrefresh(confirmationBox);

    bool response = false;
    createInputMessage(confirmationBox, "PRESS Y to confirm.");
    bool yes_no_flag = false;
    char confirmation;
    do {
        confirmation = (char)wgetch(confirmationBox);
        if (confirmation == 'Y') {
            response = true;
            yes_no_flag = true; // to indicate Y or N.
        } else if (confirmation == 'N') {
            yes_no_flag = true;
        }
    } while (!yes_no_flag);
    wclear(confirmationBox);
    wrefresh(parentBox);
    delwin(confirmationBox);
    return response;
}
