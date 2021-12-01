////
//// Created by Giwoun Bae on 2021-11-11.
////
//
//#include <ncurses.h>
//#include <stdio.h>
//#include <string.h>
//#include <client.h>
//#include <dc_posix/dc_poll.h>
//
//#define MAX_NUM_BEACONS 20 //number of ibeacons
//#define MAX_MAJOR_MINOR 35 //This is max length for major + minor for the beacons.
//
//void showWelcomePage();
//void showFirstPageOptions(WINDOW *);
//
//void getBeaconChoices(WINDOW * win, struct client_params *client);
//
//void printInTheMiddle(WINDOW *win, char * msg);
////WINDOW * createWindowInTheMiddle();
//bool gettingInput_firstPage(WINDOW *win, struct client_params * client);
//static bool confirm();
//
//void getAllListOfBeacons(const struct dc_posix_env *env, struct dc_error *err, struct client_params* client);
//
//static void createInputMessage(WINDOW * inputWindow, char* msg);
////WINDOW * createOptionWindow(WINDOW *win);
//bool showIbeaconsList(WINDOW *win, struct client_params * client);
//static WINDOW *createInputWindow(WINDOW * win);
//void ask_info(WINDOW *optionWindow, WINDOW * inputWindow, char * question);
//void input_with_echo_current_window(WINDOW *win);
//void showIbeaconLocation(WINDOW *inputWindow);
//static void show_nav(WINDOW *win);
//
////initialization
////deinialization
//
//#define HOME_CHAR 'h'
//#define QUIT_CHAR 'q'
//
//int main() {
//    //
//    //Client-side initialization code??
//    //How to send messages??
//    // can I use those code from Client.h?
//
//    dc_error_reporter reporter;
//    dc_posix_tracer tracer;
//    struct dc_error err;
//    struct dc_posix_env env;
//    const char *host_name;
////    struct addrinfo hints;
////    struct addrinfo *result;
//
//    reporter = error_reporter;
//    tracer = trace_reporter;
//    tracer = NULL;
//    dc_error_init(&err, reporter);
//    dc_posix_env_init(&env, tracer);
//
//    struct client_params* client;
//    client = malloc(sizeof(struct client_params));
//    client->userInput = malloc(sizeof(struct userInput));
//    client->arrayOfCurrentBeacon = malloc(sizeof(char [MAX_NUM_BEACONS][MAX_MAJOR_MINOR]));
//
//    initializeClient(client);
////  grab all the information from the database.
//    getAllListOfBeacons(&env, &err, client);
//
//    initscr();
//    clear();
//    cbreak();
//    noecho(); //does not echo the user input.
//    curs_set(0); //0- invisible
//
//    bool continue_application = true;
//
//    showWelcomePage(); //start of the ncurses.
//    while (continue_application) { //trying this to keep the window open until quit.
//        WINDOW *win = createWindowInTheMiddle(); //this is the master window.
//        show_nav(win);
//        showFirstPageOptions(win);
//
//        continue_application = gettingInput_firstPage(win, client);
//    }
//    endwin();
//    return 0;
//}
//
//void getAllListOfBeacons(const struct dc_posix_env *env, struct dc_error *err, struct client_params* client){
//
//    char get[4] = "GET";
//    char dump[5] = "DUMP";
//
////    struct pollfd * PollFd = malloc(sizeof(struct pollfd));
//
//    client->userInput->method = get;
//    client->userInput->key = dump;
//    client->userInput->value = dump;
//
//    char data[1024];
//
//    build_request(env, err, client);
//    send_request(env, err, client);
//
////    PollFd->fd = client->socket_fd;
////    PollFd->events = POLLIN;
////    dc_poll(env, err, PollFd, 1, 100);
//
//    while(dc_read(env, err, client->socket_fd, data, 1024) > 0 && dc_error_has_no_error(err)) { }
//    if (dc_error_has_no_error(err))
//    {
//        client->response = parse_http_response(data);
//    }
//
//    if (dc_error_has_no_error(err))
//    {
//        dc_close(env, err, client->socket_fd);
//    }
//
//    client->userInput->method = NULL;
//    client->userInput->key = NULL;
//    client->userInput->value = NULL;
////    destroy_http_request(client->request);
//}
//
//
////create the window in the middle
//#define SCREEN_RATIO 0.75
//WINDOW * createWindowInTheMiddle() {
//    int y, x, yBeg, xBeg, yMax, xMax;
//    int posY, posX;
//    getyx(stdscr, y, x);
//    xBeg = getbegx(stdscr);
//    yBeg = getbegy(stdscr);
//    xMax = getmaxx(stdscr);
//    yMax = getmaxy(stdscr);
//
//    posY = (int) (yBeg + yMax * (1 - SCREEN_RATIO) / 2);
//    posX = (int) (xBeg + xMax * (1 - SCREEN_RATIO) / 2);
//    WINDOW *win = newwin((int) (yMax * SCREEN_RATIO), (int) (xMax * SCREEN_RATIO), posY, posX);
//    refresh();
//
//    box(win, 0, 0);
//    wrefresh(win);
//
//    return win;
//}
//static void show_nav(WINDOW *win) {
//    int y, x, yBeg, xBeg, yMax, xMax;
//    getyx(win, y, x);
//    getbegyx(win, yBeg, xBeg);
//    getmaxyx(win, yMax, xMax);
//
//    mvprintw(yBeg, xBeg + 1, "q: QUIT   h: HOME");
//    wrefresh(win);
//}
//
//void printInTheMiddle(WINDOW *win, char * msg) {
//    int y, x, yBeg, xBeg, yMax, xMax;
//    getyx(win, y, x);
//    getbegyx(win, yBeg, xBeg);
//    getmaxyx(win, yMax, xMax);
//
//    int shift = (int)strlen(msg)/2;
//    mvprintw(yMax/2, xMax/2 - shift, msg);
//    wrefresh(win);
//}
//
//void showFirstPageOptions(WINDOW *win) {
//    WINDOW * optionWindow = createOptionWindow(win);
//
//    //option 1: FIND_ibeacon
//    mvwprintw(optionWindow, 1, 1, "PRESS 1: FIND IBEACON"); //if these options change, we also need to change the switch options.
//    mvwprintw(optionWindow, 2, 1, "PRESS 2: ADD NEW IBEACON");
//    //displayIbeacons
//    //selectIbeacon
//    //ShowIbeacon
//    wrefresh(optionWindow);
//    delwin(optionWindow);
//}
//
///**
// * Shows Ibeacon Lists
// *
// * @param win
// * @return true if user selects b
// */
//bool showIbeaconsList(WINDOW *win, struct client_params * client) {
//
//    int window_index = 0;
//    char current_beacon[50] = {0};
//
//    long content_length = get_content_length_long(client->response) + 3;
//    char content[content_length];
//    memmove(content, get_content(client->response), (unsigned long) (content_length - 3));
//    memmove(content + (content_length - 3), "\r\n\0", 2);
//    content[content_length - 1] = '\0';
//
//    bool cont_application = false;
//    WINDOW *optionWindow = createOptionWindow(win);
//    char * beacon_begin = content;
//    char  * beacon_end = beacon_begin;
//    int numberOfBeacons;
//
//    while (beacon_end && ++window_index)
//    {
//        beacon_begin += 2;
//        beacon_end = strchr(beacon_begin, '\r');
//        memcpy(current_beacon, beacon_begin, (unsigned long) (beacon_end - beacon_begin));
//
//        mvwprintw(optionWindow, window_index, 1, current_beacon);
//        //store into an array at same time and increment with window.
//        memcpy(client->arrayOfCurrentBeacon + window_index - 1, current_beacon, (unsigned long) (beacon_end - beacon_begin));
//        memset(current_beacon, 0, sizeof(current_beacon));
//
//        beacon_begin = beacon_end;
//        beacon_end = (strchr(beacon_begin + 2, '\r')) ? beacon_end : NULL;
//    }
//
//    numberOfBeacons = window_index - 1;
//    // display and select iBeacons
//    wrefresh(optionWindow);
//    WINDOW *inputWindow = createInputWindow(win);
//    createInputMessage(inputWindow, "Please select the beacon.");
//    keypad(optionWindow, TRUE);   // for KEY_UP, KEY_DOWN
////    start_color();
////    init_pair(1, COLOR_YELLOW, COLOR_GREEN);
//
//    //make a switch with arrow keys.
//    //increment/ decrememt window(selection) to select the
//    int selection;
//    int y,x;
//    getyx(optionWindow, y, x);
//
//    curs_set(1);
//    move(y - 1, x + 1);
//    mvwprintw(optionWindow, window_index, 1, *(client->arrayOfCurrentBeacon + window_index));
////
////    while (((selection = wgetch(optionWindow)) != QUIT_CHAR)) {
////        //Highlight the beacons?
////        if (selection == HOME_CHAR) {
////            cont_application = true;
////            break;
////        }
////        switch (selection) {
////            case KEY_UP:
////                mvwprintw(optionWindow, window_index, 1, "Hello There");
////                move(y - 1 - window_index, x + 1);
////                wattron(optionWindow,A_STANDOUT);
////                //need to dynamically get the beacon name?? or unique key?? name?
////                //how to show list of ibeacons?
////                //should I keep a list somewhere in the database?
////                //or at the initializing state grab all?? //no good....
//////                mvwprintw(optionWindow, window_index, 1, *(client->arrayOfCurrentBeacon + window_index));
////                wrefresh(optionWindow);
////                if (window_index < numberOfBeacons) {
////                    window_index++;
////                }
////                wattroff(optionWindow,A_STANDOUT);
////
////                if (confirm(win)) {
////                    showIbeaconLocation(win);
////                    //SEND REQUEST TO GET DATA.
//////                    sendPUTrequest();
//////                    showData(client);
////                    createInputMessage(inputWindow, "IBEACON1 location.");
////                }
////                break;
////            case KEY_DOWN:
////                attron(COLOR_PAIR(1));
//////                mvwprintw(optionWindow, window_index, 1, client->arrayOfCurrentBeacon[window_index - 1]);
////                attroff(COLOR_PAIR(1));
////                if (window_index > 0) {
////                    window_index--;
////                }
////                createInputMessage(inputWindow, "selected beacon 2");
////                if (confirm(win)) {
////                    createInputMessage(inputWindow, "IBEACON2 location.");
////                }
////                break;
////            default:
////                createInputMessage(inputWindow, "invalid option");
////        }
////        wrefresh(inputWindow);
////    }
//    getBeaconChoices(optionWindow, client);
//
//    return cont_application;
//}
//
//void getBeaconChoices(WINDOW * win, struct client_params *client) {
//    keypad(win, true);
//
//    int choice;
//    int highlight = 0;
//
//    while (1) {
//        for(int i = 0; i < )
//    }
//
//    wrefresh(win);
//}
//
//void showIbeaconLocation(WINDOW *win) {
//    WINDOW * optionWindow = createOptionWindow(win);
//    mvwprintw(optionWindow, 1, 1, "ID: ");
//    mvwprintw(optionWindow, 2, 1, "Location: ");
//    wrefresh(optionWindow);
//}
//
//
//void showAddPage(WINDOW * win){
//    WINDOW * optionWindow = createOptionWindow(win);
//    //Question.
//    WINDOW * inputWindow = createInputWindow(win);
//    //and get input.
//    ask_info(optionWindow, inputWindow, "Please enter MAJOR: ");
//    ask_info(optionWindow, inputWindow, "Please enter MINOR: ");
//}
//
//void ask_info(WINDOW *optionWindow, WINDOW * inputWindow, char * question) {
//    //ask a question an and return an answer...???? or save an answer.
//    //eventually we want to create a client's request to send to the server to save.
//    createInputMessage(optionWindow, question);
//    input_with_echo_current_window(inputWindow);
//    wrefresh(inputWindow);
//}
//
//void input_with_echo_current_window(WINDOW *win) {
//    createInputMessage(win, "");
//    char input[100];
//    int begY, begX;
//    getbegyx(win, begY, begX);
//    move(begY + 1, begX + 1);
//    refresh();
//    echo();
//    getstr(input);
//    refresh();
//    createInputMessage(win, "Saved.");
//    noecho();
//    wrefresh(win);
//    //need to save the input to a struct..?
//}
//
//WINDOW * createOptionWindow(WINDOW *win) {
//    int yMax, xMax, yBeg, xBeg;
//    getmaxyx(win, yMax, xMax);
//    getbegyx(win, yBeg, xBeg);
//
//    int posX, posY;
//    posX = xBeg + (xMax - (xMax - xBeg))/2;
//    posY = (yMax - yBeg)/2;
//    WINDOW *optionWindow = newwin((yMax - yBeg)/2, xMax-xBeg, posY, posX);
//    box(optionWindow, 0, 0);
//    wrefresh(optionWindow);
//
//    char * title = "IBEACON";
//    posY = 2;
//    posX = (xMax - xBeg) / 2 - (int)strlen(title)/2; //printing in the middle.
//    mvwprintw(win, posY, posX, title);
//    wrefresh(win);
//    return optionWindow;
//}
//
//
//
//static WINDOW *createInputWindow(WINDOW * win) {
//    int yMax, xMax, yBeg, xBeg;
//    getmaxyx(win, yMax, xMax);
//    getbegyx(win, yBeg, xBeg);
//
//    int posX, posY;
//    posX = xBeg + 2;
//    posY = yMax - 1;
//    WINDOW *inputWindow = newwin(3, xMax - 4, posY, posX);
//    box(inputWindow, 0, 0);
//    wmove(inputWindow,1, 1);
//    wrefresh(inputWindow);
//    return inputWindow;
//}
//
//static void createInputMessage(WINDOW * inputWindow, char* msg) {
//    wclear(inputWindow);
//    box(inputWindow, 0, 0);
//    mvwprintw(inputWindow, 1, 1, msg);
//    wrefresh(inputWindow);
//}
//
//static bool confirm(WINDOW *parentBox) {
//    //create confirmation box;
//    int yMax, xMax, yBeg, xBeg;
//    getmaxyx(parentBox, yMax, xMax);
//    getbegyx(parentBox, yBeg, xBeg);
//
//    int posX, posY;
//    posX = (xMax-xBeg)/2;
//    posY = (yMax-yBeg)/2 + 2;
//    WINDOW *confirmationBox = newwin(3, (xMax - xBeg)/2, posY, posX);
//    box(confirmationBox, (int)'-', (int)'-'); // I don't know why I cant change the border.
//    wrefresh(confirmationBox);
//
//    bool response = false;
//    createInputMessage(confirmationBox, "PRESS Y to confirm.");
//    bool yes_no_flag = false;
//    char confirmation;
//    do {
//        confirmation = (char)wgetch(confirmationBox);
//        if (confirmation == 'Y') {
//            response = true;
//            yes_no_flag = true; // to indicate Y or N.
//        } else if (confirmation == 'N') {
//            yes_no_flag = true;
//        }
//    } while (!yes_no_flag);
//    wclear(confirmationBox);
//    wrefresh(parentBox);
//    delwin(confirmationBox);
//    return response;
//}
//
//#define DELAY_MS 2000
//void showWelcomePage() {
//    printInTheMiddle(stdscr, "Welcome");
//    refresh();
//    delay_output(DELAY_MS);
//}
//
