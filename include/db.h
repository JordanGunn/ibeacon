//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_DB_H //what's this??
#define IBEACON_DB_H

#include <dc_posix/dc_netdb.h>
#include <dc_posix/dc_posix_env.h>
#include <stdio.h>
#include <stdlib.h>

//Main purpose of this thing is: to handle HTTP requests, and responses.
//This is to interact with DB.
//use the ndbm.

//possible methods: GET, PUT, POST http
//These stuff will be dealt with on server side.
//After parsing of the HTTP message, it will know which method they are.

//When it gets the GET:
    //It will call to search the db to grab.


//use struct
//struct parent {int type;}
//struct ndbm.c
    //struct ndbm_db {struct parent super;}

typedef struct DB * DBptr;


#endif //IBEACON_DB_H
