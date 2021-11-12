//
// Created by Giwoun Bae on 2021-11-11.
//

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

void showWelcomePage();
void showOptions(WINDOW *);
void printInTheMiddle(WINDOW *win, char * msg);
WINDOW * createWindowInTheMiddle();
void gettingInput(WINDOW *win);



int main() {
    //
    initscr();
    clear();
    cbreak();
    noecho();

    showWelcomePage();
    WINDOW *win = createWindowInTheMiddle();
    showOptions(win);
    gettingInput(win);

//    getch();
    //waits for user input, returns in value of the key.
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
    WINDOW *win = newwin(yMax * SCREEN_RATIO, xMax * SCREEN_RATIO, posY, posX);
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

void showOptions(WINDOW *win) {

    int yMax, xMax, yBeg, xBeg;
    getmaxyx(win, yMax, xMax);
    getbegyx(win, yBeg, xBeg);

    int posX, posY;
    posX = xBeg + (xMax - (xMax - xBeg))/2;
    posY = (yMax - yBeg)/2;
    WINDOW *optionWindow = newwin((yMax - yBeg) * 0.5, xMax-xBeg, posY, posX);
    box(optionWindow, 0, 0);
    wrefresh(optionWindow);

    posY = 2;
    posX = (xMax - xBeg) / 2;
    mvwprintw(win, posY, posX, "IBEACON");
    wrefresh(win);
    //option 1: FIND_ibeacon
    mvwprintw(optionWindow, 1, 1, "PRESS 1: FIND IBEACON");
    mvwprintw(optionWindow, 2, 1, "PRESS 2: ADD NEW IBEACON");
    //displayIbeacons
    //selectIbeacon
    //ShowIbeacon
    wrefresh(optionWindow);
}

void gettingInput(WINDOW *win) {
    //creating a window for the input
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

    //This is how you are a string.
//    char input[BUFSIZ];
//    wgetstr(inputWindow, input);

    //printout the input to the input window.
    int choice;
    while ((choice = getch()) != 'q') {
        switch(choice) {
            case '1':
                wclear(inputWindow);
                box(inputWindow, 0, 0);
                mvwprintw(inputWindow, 1, 1, "You have selected to find your ibeacon.");
                //press to confirm.

                break;
            case '2':
                wclear(inputWindow);
                box(inputWindow, 0, 0);
                mvwprintw(inputWindow, 1, 1, "You have selected to find add your ibeacon.");
                //press to confirm.

                break;
            default:
                wclear(inputWindow);
                box(inputWindow, 0, 0);
                mvwprintw(inputWindow, 1, 1, "It is not a valid respond.");
                break;
        }
        wrefresh(inputWindow);
    }

    getbegyx(inputWindow, yBeg, xBeg);
//    move(yBeg + 1, xBeg);
    wrefresh(win);
}


#define DELAY_MS 2000
void showWelcomePage() {
//    int y, x, yBeg, xBeg, yMax, xMax;
//    getyx(stdscr, y, x);
//    xBeg = getbegx(stdscr);
//    yBeg = getbegy(stdscr);
//    xMax = getmaxx(stdscr);
//    yMax = getmaxy(stdscr);
//
//    mvprintw(yMax/2, xMax/2, "Welcome");
    //refreshes the screen to match whats in the memory.
    printInTheMiddle(stdscr, "Welcome");
    refresh();
    delay_output(DELAY_MS);
}