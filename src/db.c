#include "db.h"

int store_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name, const char *value, uint8_t type)
{
    int ret_val;
    datum key = {name, strlen(name)};
    datum value_to_add = {value, strlen(value)};

    ret_val = dc_dbm_store(env, err, db, key, value_to_add, type);
    return ret_val; //returns 0 if successful, negative value if failed.
}

datum fetch_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, char *name)
{
    datum key = {name, (int)strlen(name)}; //This should not have additional new line.
    datum content;
    content = dc_dbm_fetch(env, err, db, key);
    return content;
}

int delete_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, char *name)
{
    int ret_val;
    datum key = {(void*)name, strlen(name) + 1};
    ret_val = dc_dbm_delete(env, err, db, key);
    return ret_val; //returns 0 if successful, negative value if failed.
}
#define EXIT_CODE_INITIALIZE_FAIL 5
struct Database* initialize_database(const struct dc_posix_env *env, struct dc_error *err, char* fileName)
{
    struct Database *databasePtr;
    databasePtr = dc_malloc(env, err, sizeof(struct Database));

    if (databasePtr == NULL) {
        return NULL;
    }
    databasePtr->dbmPtr = dc_dbm_open(env, err, fileName, O_RDWR | O_CREAT, 0600);

    if (dc_error_has_error(err)) {
        exit(EXIT_CODE_INITIALIZE_FAIL);
    }

    return databasePtr;
}

void deinitialize_database(const struct dc_posix_env *env, struct dc_error *err, void* database)
{
    struct Database *databasePtr;
    databasePtr = (struct Database*)database;
    dc_dbm_close(env, err, databasePtr->dbmPtr);
    free(databasePtr);
}
