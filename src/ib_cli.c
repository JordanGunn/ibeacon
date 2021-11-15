//
// Created by Giwoun Bae on 2021-11-11.
//

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

void showWelcomePage();
void showFirstPageOptions(WINDOW *);


void printInTheMiddle(WINDOW *win, char * msg);
WINDOW * createWindowInTheMiddle();
void gettingInput_firstPage(WINDOW *win);
static bool confirm();
static void createInputMessage(WINDOW * inputWindow, char* msg);
WINDOW * createOptionWindow(WINDOW *win);
void showIbeaconsList(WINDOW *win);
static WINDOW *createInputWindow(WINDOW * win);
void ask_info(WINDOW *optionWindow, WINDOW * inputWindow, char * question);
void input_with_echo_current_window(WINDOW *win);

struct ibeaconInfo {
    char* major;
    char* minor;
    char* location;
    char* timestamp;
    char* list;
};

//initialization
//deinialization


int main() {
    //
    //Client-side initialization code??
    //How to send messages??
    // can I use those code from Client.h?

    initscr();
    clear();
    cbreak();
    noecho(); //does not echo the user input.
    curs_set(0); //0- invisible

//    showWelcomePage();
    WINDOW *win = createWindowInTheMiddle();
    showFirstPageOptions(win);
    gettingInput_firstPage(win);



    endwin();
    return 0;
}


//create the window in the middle
#define SCREEN_RATIO 0.75
WINDOW * createWindowInTheMiddle() {
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


void printInTheMiddle(WINDOW *win, char * msg) {
    int y, x, yBeg, xBeg, yMax, xMax;
    getyx(win, y, x);
    getbegyx(win, yBeg, xBeg);
    getmaxyx(win, yMax, xMax);

    int shift = (int)strlen(msg)/2;
    mvprintw(yMax/2, xMax/2 - shift, msg);
    wrefresh(win);
}

void showFirstPageOptions(WINDOW *win) {
    WINDOW * optionWindow = createOptionWindow(win);

    //option 1: FIND_ibeacon
    mvwprintw(optionWindow, 1, 1, "PRESS 1: FIND IBEACON"); //if these options change, we also need to change the switch options.
    mvwprintw(optionWindow, 2, 1, "PRESS 2: ADD NEW IBEACON");
    //displayIbeacons
    //selectIbeacon
    //ShowIbeacon
    wrefresh(optionWindow);
}

void showIbeaconsList(WINDOW *win) {
    WINDOW * optionWindow = createOptionWindow(win);
    mvwprintw(optionWindow, 1, 1, "Beacon 1");
    mvwprintw(optionWindow, 2, 1, "Beacon 2");
    //displayIbeacons
    //selectIbeacon
    wrefresh(optionWindow);
    WINDOW *inputWindow = createInputWindow(win);
    createInputMessage(inputWindow, "Please select the beacon.");

    //ShowIbeacon
    char selection;
    while ((selection = (char) wgetch(inputWindow)) != 'q') {
        //Highlight the beacons?
        switch (selection) {
            case 1:
                //need to dynamically get the beacon name?? or unique key?? name?
                createInputMessage(inputWindow, "selected beacon 1");
                break;
            case 2:
                createInputMessage(inputWindow, "selected beacon 2");
                break;
            default:
                createInputMessage(inputWindow, "invalid option");
        }
    }
    wrefresh(optionWindow);
}

void showAddPage(WINDOW * win){
    WINDOW * optionWindow = createOptionWindow(win);
    //Question.
    WINDOW * inputWindow = createInputWindow(win);
    //and get input.
    ask_info(optionWindow, inputWindow, "Please enter MAJOR: ");
    ask_info(optionWindow, inputWindow, "Please enter MINOR: ");
}

void ask_info(WINDOW *optionWindow, WINDOW * inputWindow, char * question) {
    //ask a question an and return an answer...???? or save an answer.
    //eventually we want to create a client's request to send to the server to save.
    createInputMessage(optionWindow, question);
    input_with_echo_current_window(inputWindow);
    wrefresh(inputWindow);
}

void input_with_echo_current_window(WINDOW *win) {
    createInputMessage(win, "");
    char input[100];
    int begY, begX;
    getbegyx(win, begY, begX);
    move(begY + 1, begX + 1);
    refresh();
    echo();
    getstr(input);
    refresh();
    createInputMessage(win, "Saved.");
    noecho();
    wrefresh(win);
    //need to save the input to a struct..?
}

WINDOW * createOptionWindow(WINDOW *win) {
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);

    int posX, posY;
    posX = xBeg + (xMax - (xMax - xBeg))/2;
    posY = (yMax - yBeg)/2;
    WINDOW *optionWindow = newwin((yMax - yBeg)/2, xMax-xBeg, posY, posX);
    box(optionWindow, 0, 0);
    wrefresh(optionWindow);

    posY = 2;
    posX = (xMax - xBeg) / 2;
    mvwprintw(win, posY, posX, "IBEACON");
    wrefresh(win);
    return optionWindow;
}

void gettingInput_firstPage(WINDOW *win) {
    //creating a window for the input
    WINDOW * inputWindow = createInputWindow(win);

    //printout the input to the input window.
    int choice;
    while ((choice = getch()) != 'q') {
        switch(choice) {
            case '1':
                createInputMessage(inputWindow, "You have selected to find your ibeacon.");
                //press to confirm.
                if (confirm(win)) {
                    //move on to the next window.
                    showIbeaconsList(win);
                };
                break;
            case '2':
                createInputMessage(inputWindow, "You have selected to find add your ibeacon.");
                //press to confirm.
                if (confirm(win)) {
                    showAddPage(win);
                }
                break;
            default:
                wclear(inputWindow);
                box(inputWindow, 0, 0);
                mvwprintw(inputWindow, 1, 1, "It is not a valid respond.");
                break;
        }
        wrefresh(inputWindow);
    }

    wrefresh(win);
}

static WINDOW *createInputWindow(WINDOW * win) {
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

static void createInputMessage(WINDOW * inputWindow, char* msg) {
    wclear(inputWindow);
    box(inputWindow, 0, 0);
    mvwprintw(inputWindow, 1, 1, msg);
    wrefresh(inputWindow);
}

static bool confirm(WINDOW *parentBox) {
    //create confirmation box;
    int yMax, xMax, yBeg, xBeg;
    getmaxyx(parentBox, yMax, xMax);
    getbegyx(parentBox, yBeg, xBeg);

    int posX, posY;
    posX = (xMax-xBeg)/2;
    posY = (yMax-yBeg)/2 + 2;
    WINDOW *confirmationBox = newwin(3, (xMax - xBeg)/2, posY, posX);
    box(confirmationBox, (int)'-', (int)'-');
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

    delwin(confirmationBox);
    return response;
}

#define DELAY_MS 2000
void showWelcomePage() {
    printInTheMiddle(stdscr, "Welcome");
    refresh();
    delay_output(DELAY_MS);
}