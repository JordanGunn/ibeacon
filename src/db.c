//
// Created by jordan on 2021-11-06.
//

#include "db.h"
//Is going to be ndbm_db.c

//Maybe we need to encapsulate the function calls.


/*
 * ATTENTION! The type will always be REPLACE.
 * REPLACE will create the key if not exist, and UPDATE when exist.
 */
int store_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *key_to_add, const char *value_to_add, uint8_t type)
{
    int ret_val;
    datum key = {key_to_add, strlen(key_to_add) + 1}; //adding one as a \n
    datum value = {value_to_add, strlen(value_to_add) + 1}; //adding one as a \n

    ret_val = dc_dbm_store(env, err, db, key, value, type);
    return ret_val; //returns 0 if successful, and negative value if fail.
}

datum fetch_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name)
{
    datum key = {(void*)name, strlen(name)};
    datum content;
    content = dc_dbm_fetch(env, err, db, key);
    return content;
}

int delete_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name)
{
    datum key = {(void*)name, strlen(name)};
    int ret_val;

    ret_val = dc_dbm_delete(env, err, db, key);
    return ret_val; //returns 0 if successful, and negative value if fail.
}

