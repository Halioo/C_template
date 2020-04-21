/**
 * @file example.h
 *
 * @brief This is an example of active class
 *
 * You can copy and paste this example to create an active class.
 * To use it :
 * - Search and replace (Distinguish upper and lower case) the following
 * words in the .c and .h file: example, Example
 * - Follow the "TODO" instructions
 * - Remove the Example events and parameters.
 *
 * You can explain here the way to use the module (preconditions, etc).
 *
 * You can create several paragraphs by letting a blank space.
 *
 * You must code and document everything in english.
 *
 * @date April 2020
 *
 * @authors Thomas CRAVIC, Clément PUYBAREAU, Louis FROGER
 *
 * @version 1.0
 *
 * @copyright CCBY 4.0
 */

#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <watchdog.h>

typedef struct Example_t Example;

/* ----------------------- PUBLIC FUNCTIONS PROTOTYPES -----------------------*/

/**
 * @brief Function that raises the E_EXAMPLE1 event
 *
 * @param[in] param paramter of the function
 */
extern void ExampleEventOne(Example * this, int param);

/**
 * @brief Function that raises the E_EXAMPLE2 event
 *
 * @param[in] param paramter of the function
 */
extern void ExampleEventTwo(Example * this, int param);

/**
 * @brief Example function that treats a wathdog event.
 */
extern void ExampleTimeout(Watchdog * wd, void * caller);

/* ----------------------- NEW START STOP FREE -----------------------*/

/**
 * @brief Example class constructor
 *
 * Allocates an Example object
 *
 *
 * @retval 0 If the allocation worked
 * @retval -1 if the allocation didn't work
 */
extern Example * ExampleNew();


/**
 * @brief Example class starter
 *
 * Starts the Example object
 *
 * @retval 0 If the start worked
 * @retval -1 If the start didn't work
 */
extern int ExampleStart();


/**
 * @brief Example singleton stopper
 *
 * @retval 0 If the object stopped properly
 * @retval -1 If the object didn't stopped properly
 */

extern int ExampleStop ();

/**
 * @brief Example singleton destructor
 *
 * @retval 0 If the destruction worked
 * @retval -1 if the destruction didn't work
 */

extern int ExampleFree ();

#endif //EXAMPLE_H
