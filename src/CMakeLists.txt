add_compile_definitions(_POSIX_C_SOURCE=200809L _XOPEN_SOURCE=700)

if(APPLE)
    add_definitions(-D_DARWIN_C_SOURCE)
endif()

find_program(LINT "clang-tidy")
IF (LINT)
    set(CMAKE_C_CLANG_TIDY "clang-tidy;-checks=*,-llvmlibc-restrict-system-libc-headers,-cppcoreguidelines-init-variables,-clang-analyzer-security.insecureAPI.strcpy,-concurrency-mt-unsafe,-android-cloexec-accept,-android-cloexec-dup,-google-readability-todo,-cppcoreguidelines-avoid-magic-numbers,-readability-magic-numbers,-cert-dcl03-c,-hicpp-static-assert,-misc-static-assert,-altera-struct-pack-align,-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling;--quiet")
ENDIF ()

# Make an executable
add_executable(client ${SOURCE_LIST} ${CLIENT_SOURCE})
add_executable(server ${SOURCE_LIST} ${SERVER_SOURCE})
add_executable(db ${SOURCE_LIST} ${CLIENT_SOURCE})
add_executable(http ${SOURCE_LIST} ${SERVER_SOURCE})
add_executable(ibeacon ${SOURCE_LIST} ${CLIENT_SOURCE})

# We need this directory, and users of our library will need it too
target_include_directories(client PRIVATE ../include)
target_include_directories(client PRIVATE /usr/include)
target_include_directories(client PRIVATE /usr/local/include)
target_link_directories(client PRIVATE /usr/lib)
target_link_directories(client PRIVATE /usr/local/lib)

target_include_directories(server PRIVATE ../include)
target_include_directories(server PRIVATE /usr/include)
target_include_directories(server PRIVATE /usr/local/include)
target_link_directories(server PRIVATE /usr/lib)
target_link_directories(server PRIVATE /usr/local/lib)
#
#target_include_directories(db PRIVATE ../include)
#target_include_directories(db PRIVATE /usr/include)
#target_include_directories(db PRIVATE /usr/local/include)
#target_link_directories(db PRIVATE /usr/lib)
#target_link_directories(db PRIVATE /usr/local/lib)
#
#target_include_directories(ibeacon PRIVATE ../include)
#target_include_directories(ibeacon PRIVATE /usr/include)
#target_include_directories(ibeacon PRIVATE /usr/local/include)
#target_link_directories(ibeacon PRIVATE /usr/lib)
#target_link_directories(ibeacon PRIVATE /usr/local/lib)
#
#target_include_directories(http PRIVATE ../include)
#target_include_directories(http PRIVATE /usr/include)
#target_include_directories(http PRIVATE /usr/local/include)
#target_link_directories(http PRIVATE /usr/lib)
#target_link_directories(http PRIVATE /usr/local/lib)


# All users of this library will need at least C11

target_compile_features(client PUBLIC c_std_11)
target_compile_options(client PRIVATE -g)
target_compile_options(client PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(client PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(client PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn -Walloca -Wfloat-equal -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

target_compile_features(server PUBLIC c_std_11)
target_compile_options(server PRIVATE -g)
target_compile_options(server PRIVATE -fstack-protector-all -ftrapv)
target_compile_options(server PRIVATE -Wpedantic -Wall -Wextra)
target_compile_options(server PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn  -Walloca -Wfloat-equal  -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
#
#target_compile_features(db PUBLIC c_std_11)
#target_compile_options(db PRIVATE -g)
#target_compile_options(db PRIVATE -fstack-protector-all -ftrapv)
#target_compile_options(db PRIVATE -Wpedantic -Wall -Wextra)
#target_compile_options(db PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn  -Walloca -Wfloat-equal  -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
#
#target_compile_features(ibeacon PUBLIC c_std_11)
#target_compile_options(ibeacon PRIVATE -g)
#target_compile_options(ibeacon PRIVATE -fstack-protector-all -ftrapv)
#target_compile_options(ibeacon PRIVATE -Wpedantic -Wall -Wextra)
#target_compile_options(ibeacon PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn  -Walloca -Wfloat-equal  -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)
#
#target_compile_features(http PUBLIC c_std_11)
#target_compile_options(http PRIVATE -g)
#target_compile_options(http PRIVATE -fstack-protector-all -ftrapv)
#target_compile_options(http PRIVATE -Wpedantic -Wall -Wextra)
#target_compile_options(http PRIVATE -Wdouble-promotion -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wnull-dereference -Winit-self -Wmissing-include-dirs -Wswitch-default -Wswitch-enum -Wunused-local-typedefs -Wstrict-overflow=5 -Wmissing-noreturn  -Walloca -Wfloat-equal  -Wdeclaration-after-statement -Wshadow -Wpointer-arith -Wabsolute-value -Wundef -Wexpansion-to-defined -Wunused-macros -Wno-endif-labels -Wbad-function-cast -Wcast-qual -Wwrite-strings -Wconversion -Wdangling-else -Wdate-time -Wempty-body -Wsign-conversion -Wfloat-conversion -Waggregate-return -Wstrict-prototypes -Wold-style-definition -Wmissing-prototypes -Wmissing-declarations -Wpacked -Wredundant-decls -Wnested-externs -Winline -Winvalid-pch -Wlong-long -Wvariadic-macros -Wdisabled-optimization -Wstack-protector -Woverlength-strings)

find_library(LIBM m REQUIRED)
find_library(LIBSOCKET socket)
find_library(LIBDC_UTIL dc_util REQUIRED)
find_library(LIBDC_ERROR dc_error REQUIRED)
find_library(LIBDC_POSIX dc_posix REQUIRED)
find_library(LIBDC_APPLICATION dc_application REQUIRED)
find_library(NCURSES_LIB ncurses REQUIRED)


target_link_libraries(client PRIVATE ${LIBM})
target_link_libraries(client PRIVATE ${LIBDC_UTIL})
target_link_libraries(client PRIVATE ${LIBDC_ERROR})
target_link_libraries(client PRIVATE ${LIBDC_POSIX})
target_link_libraries(client PRIVATE ${LIBDC_APPLICATION})
target_link_libraries(client PRIVATE ${NCURSES_LIB})

target_link_libraries(server PRIVATE ${LIBM})
target_link_libraries(server PRIVATE ${LIBDC_UTIL})
target_link_libraries(server PRIVATE ${LIBDC_ERROR})
target_link_libraries(server PRIVATE ${LIBDC_POSIX})
target_link_libraries(server PRIVATE ${LIBDC_APPLICATION})
#
#target_link_libraries(http PRIVATE ${LIBM})
#target_link_libraries(http PRIVATE ${LIBDC_UTIL})
#target_link_libraries(http PRIVATE ${LIBDC_ERROR})
#target_link_libraries(http PRIVATE ${LIBDC_POSIX})
#target_link_libraries(http PRIVATE ${LIBDC_APPLICATION})
#
#target_link_libraries(db PRIVATE ${LIBM})
#target_link_libraries(db PRIVATE ${LIBDC_UTIL})
#target_link_libraries(db PRIVATE ${LIBDC_ERROR})
#target_link_libraries(db PRIVATE ${LIBDC_POSIX})
#target_link_libraries(db PRIVATE ${LIBDC_APPLICATION})
#
#target_link_libraries(ibeacon PRIVATE ${LIBM})
#target_link_libraries(ibeacon PRIVATE ${LIBDC_UTIL})
#target_link_libraries(ibeacon PRIVATE ${LIBDC_ERROR})
#target_link_libraries(ibeacon PRIVATE ${LIBDC_POSIX})
#target_link_libraries(ibeacon PRIVATE ${LIBDC_APPLICATION})

set_target_properties(server PROPERTIES OUTPUT_NAME "server")
set_target_properties(client PROPERTIES OUTPUT_NAME "client")
set_target_properties(http PROPERTIES OUTPUT_NAME "http")
set_target_properties(db PROPERTIES OUTPUT_NAME "db")
set_target_properties(ibeacon PROPERTIES OUTPUT_NAME "ibeacon")

# IDEs should put the headers in a nice place
source_group(
        TREE "${PROJECT_SOURCE_DIR}/include"
        PREFIX "Header Files"
        FILES ${HEADER_LIST}
)

add_custom_target(
        format
        COMMAND clang-format
        -i
        ${HEADER_LIST}
        ${SOURCE_LIST}
        ${MAIN_SOURCE}
)