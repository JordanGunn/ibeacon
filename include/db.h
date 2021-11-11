//
// Created by jordan on 2021-11-07.
//

#ifndef IBEACON_DB_H //what's this??
#define IBEACON_DB_H

//Main purpose of this thing is: to handle HTTP requests, and responses.
//This is to interact with DB.
//use the ndbm.

//possible methods: GET, PUT, POST, DELETE http
//These stuff will be dealt with on server side.
//After parsing of the HTTP message, it will know which method they are.

//When it gets the GET:
    //It will call to search the db to grab.

//use struct
//struct parent {int type;}
//struct ndbm.c //struct ndbm_db {struct parent super;}

#include <dc_application/command_line.h>
#include <dc_application/config.h>
#include <dc_application/defaults.h>
#include <dc_application/environment.h>
#include <dc_application/options.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_ndbm.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Database {
    DBM * dbmPtr;
};

//initialize database.
//save the dbmPtr to Database struct.
struct Database* initialize_database(const struct dc_posix_env *env, struct dc_error *err, char* fileName);
void deinitialize_database(const struct dc_posix_env *env, struct dc_error *err, void* database);

int store_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name, const char *value, uint8_t type);
datum fetch_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name);
void test_display(const char *name, datum *content);
int delete_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name);

#endif //IBEACON_DB_H
