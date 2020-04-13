/**
 * @file util.h
 *
 * @brief Fonctions utilitaires
 *
 * @date 13/04/2020
 * @author Thomas Cravic, Nathan Le Granvallet, Clément PUYBAREAU
 *
 * @copyright CCBY 4.0
 */


#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>


/**
 * @def ERROR
 *
 * Macro utilisée pour logger une erreur sans tuer le programme
 *
 * @param errorCondition Booléen qui est vrai quand il y a une erreur
 * @param fmt Texte décrivant l'erreur
 */
#define ERROR(errorCondition, fmt, ...) do {\
    if (errorCondition) { \
        fprintf (stderr, "[Error] (%s) at %s:%d\n : " fmt, \
        #errorCondition, __FILE__, __LINE__, ##__VA_ARGS__); \
        perror (""); \
    } \
} while (0);

#ifndef NDEBUG

/**
 * @def TRACE
 *
 * Fonction utilisée pour afficher des messages de débug
 *
 * @param fmt Texte à afficher
 */
#define TRACE(fmt, ...) do {\
    fprintf(stderr, "%s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##__VA_ARGS__);\
    fflush (stderr);\
} while(0);

/**
 * @def STOP_ON_ERROR
 *
 * Fonction d'erreur tuant le programme
 *
 * @param errorCondition Booléen qui est vrai quand il y a une erreur
 * @param fmt Texte décrivant l'erreur
 */
#define STOP_ON_ERROR(errorCondition, fmt, ...) do {\
    if (errorCondition) { \
        fprintf (stderr, "[Error] (%s) at %s:%d\n : " fmt, \
        #errorCondition, __FILE__, __LINE__, ##__VA_ARGS__); \
        perror (""); \
        exit (1);\
    } \
} while (0);


#else

#define ERROR(errorCondition, fmt, ...)
    #define TRACE(fmt, ...)
    #define STOP_ON_ERROR(errorCondition, fmt, ...)
#endif

#endif /* UTIL_H */
