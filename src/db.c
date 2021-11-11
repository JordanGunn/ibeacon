#include "db.h"

struct application_settings
{
    struct dc_opt_settings opts;
    struct dc_setting_string *message;
};



static struct dc_application_settings *create_settings(const struct dc_posix_env *env, struct dc_error *err);
static int destroy_settings(const struct dc_posix_env *env,
                            struct dc_error *err,
                            struct dc_application_settings **psettings);
static int run(const struct dc_posix_env *env, struct dc_error *err, struct dc_application_settings *settings);
static void error_reporter(const struct dc_error *err);
static void trace_reporter(const struct dc_posix_env *env,
                           const char *file_name,
                           const char *function_name,
                           size_t line_number);


int main(int argc, char *argv[])
{
    dc_posix_tracer tracer;
    dc_error_reporter reporter;
    struct dc_posix_env env;
    struct dc_error err;
    struct dc_application_info *info;
    int ret_val;

    reporter = error_reporter;
    tracer = trace_reporter;
    tracer = NULL;
    dc_error_init(&err, reporter);
    dc_posix_env_init(&env, tracer);
    info = dc_application_info_create(&env, &err, "Settings Application");
    ret_val = dc_application_run(&env, &err, info, create_settings, destroy_settings, run, dc_default_create_lifecycle, dc_default_destroy_lifecycle, NULL, argc, argv);
    dc_application_info_destroy(&env, &info);
    dc_error_reset(&err);

    return ret_val;
}

static struct dc_application_settings *create_settings(const struct dc_posix_env *env, struct dc_error *err)
{
    struct application_settings *settings;

    DC_TRACE(env);
    settings = dc_malloc(env, err, sizeof(struct application_settings));

    if(settings == NULL)
    {
        return NULL;
    }

    settings->opts.parent.config_path = dc_setting_path_create(env, err);
    settings->message = dc_setting_string_create(env, err);

    struct options opts[] = {
            {(struct dc_setting *)settings->opts.parent.config_path,
                    dc_options_set_path,
                    "config",
                    required_argument,
                    'c',
                    "CONFIG",
                    dc_string_from_string,
                    NULL,
                    dc_string_from_config,
                    NULL},
            {(struct dc_setting *)settings->message,
                    dc_options_set_string,
                    "message",
                    required_argument,
                    'm',
                    "MESSAGE",
                    dc_string_from_string,
                    "message",
                    dc_string_from_config,
                    "Hello, Default World!"},
    };

    // note the trick here - we use calloc and add 1 to ensure the last line is all 0/NULL
    settings->opts.opts_count = (sizeof(opts) / sizeof(struct options)) + 1;
    settings->opts.opts_size = sizeof(struct options);
    settings->opts.opts = dc_calloc(env, err, settings->opts.opts_count, settings->opts.opts_size);
    dc_memcpy(env, settings->opts.opts, opts, sizeof(opts));
    settings->opts.flags = "m:";
    settings->opts.env_prefix = "DC_EXAMPLE_";

    return (struct dc_application_settings *)settings;
}

static int destroy_settings(const struct dc_posix_env *env,
                            __attribute__((unused)) struct dc_error *err,
                            struct dc_application_settings **psettings)
{
    struct application_settings *app_settings;

    DC_TRACE(env);
    app_settings = (struct application_settings *)*psettings;
    dc_setting_string_destroy(env, &app_settings->message);
    dc_free(env, app_settings->opts.opts, app_settings->opts.opts_count);
    dc_free(env, *psettings, sizeof(struct application_settings));

    if(env->null_free)
    {
        *psettings = NULL;
    }

    return 0;
}


#define NAME       "Arturo Crespo"
#define PHONE_NO   "723-9273"
#define DB_NAME    "hello"

static int run(const struct dc_posix_env *env, struct dc_error *err, struct dc_application_settings *settings)
{
    struct application_settings *app_settings;
    const char *message;
    DBM *db;
    uint8_t ret_code;
    datum content;

    DC_TRACE(env);

    app_settings = (struct application_settings *)settings;
    message = dc_setting_string_get(env, app_settings->message);
    printf("prog1 says \"%s\"\n", message);

    // Open the database and store the record
    db = dc_dbm_open(env, err, DB_NAME, O_RDWR | O_CREAT, 0600);
    ret_code = EXIT_SUCCESS;
    //making these after we are able to open it.
    if (dc_error_has_no_error(err)) {
        store_data(env, err, db, "foo", "123-4567", DBM_REPLACE);
        store_data(env, err, db, "BAE", "444-5555", DBM_REPLACE);
        store_data(env, err, db, "BAE", "666-7777", DBM_REPLACE);
        content = fetch_data(env, err, db, "foo");
        test_display("foo", &content);
        content = fetch_data(env, err, db, "Bae");
        test_display("Bae", &content);
        content = fetch_data(env, err, db, "BAE");
        test_display("BAE", &content);
        delete_data(env, err, db, "BAE");
        content = fetch_data(env, err, db, "BAE");
        test_display("BAE", &content);

        dc_dbm_close(env, err, db);
    } else {
        ret_code = EXIT_FAILURE;
    }

    return ret_code;
}

int store_data(const struct dc_posix_env *env, struct dc_error *err, DBM *db, const char *name, const char *phone_number, uint8_t type)
{
    int ret_val;
    datum key = {name, strlen(name)};
    datum value = {phone_number, strlen(phone_number)};

    ret_val = dc_dbm_store(env, err, db, key, value, type);
    return ret_val;
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
    int ret_val;
    datum key = {(void*)name, strlen(name)};
    ret_val = dc_dbm_delete(env, err, db, key);
    return ret_val;
}

void test_display(const char *name, datum *content)
{
    if (content->dsize > 0) {
        printf("%s: %s\n", name, (char*)content->dptr);
    } else
    {
        printf("%s: NOT FOUND\n", name);
    }
}

static void error_reporter(const struct dc_error *err)
{
    fprintf(stderr, "ERROR: %s : %s : @ %zu : %d\n", err->file_name, err->function_name, err->line_number, 0);
    fprintf(stderr, "ERROR: %s\n", err->message);
}

static void trace_reporter(__attribute__((unused)) const struct dc_posix_env *env,
                           const char *file_name,
                           const char *function_name,
                           size_t line_number)
{
    fprintf(stdout, "TRACE: %s : %s : @ %zu\n", file_name, function_name, line_number);
}
