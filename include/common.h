//
// Created by jordan on 2021-10-28.
//

#ifndef HAMMING_COMMON_H
#define HAMMING_COMMON_H

#include "http/http_response.h"
#include "http/http_request.h"

#include <dc_application/command_line.h>
#include <dc_application/options.h>
#include <dc_application/config.h>
#include <dc_posix/sys/dc_stat.h>
#include <dc_posix/dc_stdlib.h>
#include <dc_posix/dc_string.h>
#include <dc_posix/dc_unistd.h>
#include <dc_posix/dc_fcntl.h>
#include <dc_util/streams.h>
#include <dc_util/types.h>
#include <dc_util/dump.h>
#include <dc_util/bits.h>
#include <sys/fcntl.h>
#include <getopt.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

struct application_settings
{
    struct dc_opt_settings opts;
    struct dc_setting_string *parity;
    struct dc_setting_string *prefix;
};


/**
 * Create
 *
 * @param env
 * @param err
 * @return
 */

void error_reporter(const struct dc_error *err);

void trace_reporter(
        const struct dc_posix_env *env, const char *file_name, const char *function_name, size_t line_number
);

#endif HAMMING_COMMON_H
