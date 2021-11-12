//
// Created by Giwoun Bae on 2021-11-11.
//

#include <ncurses.h>
#include <stdio.h>
#include <string.h>

void showWelcomePage();
void showOptions(WINDOW *);
WINDOW * createWindowInTheMiddle();



int main() {
    //
    initscr();
    clear();
    cbreak();

    showWelcomePage();
    WINDOW *win = createWindowInTheMiddle();
//    showOptions(win);

    getch();
    //waits for user input, returns in value of the key.
    endwin();

    return 0;
}


//create the window in the middle
#define SCREEN_RATIO 0.75
WINDOW * createWindowInTheMiddle() {
    int y, x, yBeg, xBeg, yMax, xMax;
    getyx(stdscr, y, x);
    xBeg = getbegx(stdscr);
    yBeg = getbegy(stdscr);
    xMax = getmaxx(stdscr);
    yMax = getmaxy(stdscr);

    WINDOW *win = newwin(yMax * SCREEN_RATIO, xMax * SCREEN_RATIO, yBeg + yMax * (1- SCREEN_RATIO)/2, xBeg + xMax *(1- SCREEN_RATIO)/2);
    refresh();

    box(win, 0, 0);
    wrefresh(win);
    return win;
}


void printInTheMiddle(char * msg) {
    int y, x, yBeg, xBeg, yMax, xMax;
    getyx(stdscr, y, x);
    xBeg = getbegx(stdscr);
    yBeg = getbegy(stdscr);
    xMax = getmaxx(stdscr);
    yMax = getmaxy(stdscr);

    size_t shift = strlen(msg)/2;
    mvprintw(yMax/2, xMax/2 - shift, msg);
}

void showOptions() {
    //option 1: FIND_ibeacon
    //displayIbeacons
    //selectIbeacon
    //ShowIbeacon
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
    printInTheMiddle("Welcome");
    refresh();
    delay_output(DELAY_MS);
}