/**
 * @file util.h
 *
 * @brief Macros for traces and assertions
 *
 * @warning TRACE functions work only with gcc compiler
 *
 * @date April 2020
 *
 * @authors Jerome DELATOUR, Louis FROGER, Clément PUYBAREAU
 *
 */

#ifndef UTIL_H
#define UTIL_H


#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

/**
 * @brief Return the number of arguments
 * _NUM_ARGS is only to be used inside the NUM_ARGS macro
 */
#define _NUM_ARGS(X100, X99, X98, X97, X96, X95, X94, X93, X92, X91, X90, X89, X88, X87, X86, X85, X84, X83, X82, X81, X80, X79, X78, X77, X76, X75, X74, X73, X72, X71, X70, X69, X68, X67, X66, X65, X64, X63, X62, X61, X60, X59, X58, X57, X56, X55, X54, X53, X52, X51, X50, X49, X48, X47, X46, X45, X44, X43, X42, X41, X40, X39, X38, X37, X36, X35, X34, X33, X32, X31, X30, X29, X28, X27, X26, X25, X24, X23, X22, X21, X20, X19, X18, X17, X16, X15, X14, X13, X12, X11, X10, X9, X8, X7, X6, X5, X4, X3, X2, X1, N, ...)   N
#define NUM_ARGS(...) _NUM_ARGS(__VA_ARGS__, 100, 99, 98, 97, 96, 95, 94, 93, 92, 91, 90, 89, 88, 87, 86, 85, 84, 83, 82, 81, 80, 79, 78, 77, 76, 75, 74, 73, 72, 71, 70, 69, 68, 67, 66, 65, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

#define EXPAND(X)       X
#define FIRSTARG(X, ...)    (X)
#define RESTARGS(X, ...)    (__VA_ARGS__)

/**
 * @brief Apply the MACRO to every element of LIST
 */
#define FOREACH(MACRO, LIST)    FOREACH_(NUM_ARGS LIST, MACRO, LIST)
#define FOREACH_(N, M, LIST)    FOREACH__(N, M, LIST)
#define FOREACH__(N, M, LIST)   FOREACH_##N(M, LIST)
#define FOREACH_1(M, LIST)  M LIST
#define FOREACH_2(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_1(M, RESTARGS LIST)
#define FOREACH_3(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_2(M, RESTARGS LIST)
#define FOREACH_4(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_3(M, RESTARGS LIST)
#define FOREACH_5(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_4(M, RESTARGS LIST)
#define FOREACH_6(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_5(M, RESTARGS LIST)
#define FOREACH_7(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_6(M, RESTARGS LIST)
#define FOREACH_8(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_7(M, RESTARGS LIST)
#define FOREACH_9(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_8(M, RESTARGS LIST)
#define FOREACH_10(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_9(M, RESTARGS LIST)
#define FOREACH_11(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_10(M, RESTARGS LIST)
#define FOREACH_12(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_11(M, RESTARGS LIST)
#define FOREACH_13(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_12(M, RESTARGS LIST)
#define FOREACH_14(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_13(M, RESTARGS LIST)
#define FOREACH_15(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_14(M, RESTARGS LIST)
#define FOREACH_16(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_15(M, RESTARGS LIST)
#define FOREACH_17(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_16(M, RESTARGS LIST)
#define FOREACH_18(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_17(M, RESTARGS LIST)
#define FOREACH_19(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_18(M, RESTARGS LIST)
#define FOREACH_20(M, LIST)  EXPAND(M FIRSTARG LIST) FOREACH_19(M, RESTARGS LIST)

#define _toEnum(x) x,
#define toEnum(...) FOREACH(_toEnum, (__VA_ARGS__))

/**
 * @brief Converts each element of a list to its string equivalent
 */
#define _toString(x) #x,
#define toString(...) FOREACH(_toString, (__VA_ARGS__))

/**
 * @brief return the max of 2 values
 */
#define max(a,b) ({ \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a > _b ? _a : _b; })

/**
 * @brief return the min of 2 values
 */
#define min(a,b) ({ \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b; })

/**
 * @brief Create a Wrapper union type of the given
 * struct TYPE. Interprets TYPE as a char[]
 */
#define wrapperOf(TYPE) \
    typedef union { \
        TYPE data; \
        char toString[sizeof(TYPE)]; \
    } Wrapper;



#ifndef NDEBUG

/**
 * @brief Creates an enum based on a name and a list
 * of enum values and an array of values' names for
 * easy string translation.
 * How to use instructions :
 *  - name has to be uppercase
 *  - a last value for getting the number of enum
 *  values is automatically append. By convention,
 *  its name is "NB_<name>"
 *  - By convention, the string array is called "<name>_toString"
 *
 * @param name Name of the enum
 * @param ARGS List of values
 */
#define ENUM_DECL(name, ARGS...) \
    typedef enum { toEnum(ARGS) NB_##name } name; \
    static const char * name##_toString[] = { toString(ARGS) #name };

/**
 * @def TRACE
 *
 * @brief Display debug messages
 *
 * @param fmt - Description to display
 */
#define TRACE(fmt, ...) do {\
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    fflush (stderr);\
} while(0);

/**
 * @def STOP_ON_ERROR
 *
 * @brief Kill the process when an error occurs
 *
 * @param errorCondition - True boolean when an error occurs
 * @param fmt - Error description
 */
#define STOP_ON_ERROR(errorCondition, fmt, ...) do {\
    if (errorCondition) { \
        fprintf (stderr, "[Error] (%s) at %s:%d\n : " fmt, \
        #errorCondition, __FILE__, __LINE__, ##__VA_ARGS__); \
        perror (""); \
        exit (1);\
    } \
} while (0);

/**
 * @def ERROR
 *
 * @brief Log an error without kill the process
 *
 * @param errorCondition - True boolean when an error occurs
 * @param fmt - Error description
 */
#define ERROR(errorCondition, fmt, ...) do {\
    if (errorCondition) { \
        fprintf (stderr, "[Error] (%s) at %s:%d\n : " fmt, \
        #errorCondition, __FILE__, __LINE__, ##__VA_ARGS__); \
        perror (""); \
    } \
} while (0);



#else
    #define ENUM_DECL(name, ARGS...) typedef enum { toEnum(ARGS) NB_##name } name;
    #define ERROR(errorCondition, fmt, ...)
    #define TRACE(fmt, ...)
    #define STOP_ON_ERROR(errorCondition, fmt, ...)
#endif


/**
 * Define a generic FLAG enum
 */
ENUM_DECL(FLAG, DOWN, UP)


#endif /* UTIL_H */
