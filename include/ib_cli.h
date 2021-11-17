//
// Created by Giwoun Bae on 2021-11-11.
//

#ifndef IBEACON_IB_CLI_H
#define IBEACON_IB_CLI_H

//What do I need in the client.c?
//1. showWelcomePage?
//2. Need data.
//3. struct????
//4. the client code will grab whatever..?

struct ibeaconInfo {
    char * major;
    char * minor;
    char * location,
    char * timeStamp;
};

//Maybe I need init_ncurses() to initialize the screen inside the client??
//or the other way around.........


//WelcomePage
void showWelcomePage();

//showOptions
void showOptions();
//option 1: FIND_ibeacon
        //displayIbeacons
        //selectIbeacon
        //ShowIbeacon

//option 2: ADD_ibeacon
        //getInput
        //optionToReset (or return to enter the first one (while loop then break out?: maybe two while loops?)
        //saveTheData
// back to the option menu.

#endif //IBEACON_IB_CLI_H
