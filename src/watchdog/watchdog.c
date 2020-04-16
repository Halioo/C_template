/**
 * @file watchdog.c
 *
 * @brief Watchdog class that abstracts an "after" UML event
 *
 * @date 06/05/2019
 * @author team FORMATO, Thomas CRAVIC
 *
 * @copyright Jonathan ILIAS-PILLET (ESEO), Desoutter
 *
 * @section License
 *
 * The MIT License
 *
 * Copyright (c) 2016, Jonathan ILIAS-PILLET (ESEO)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
*/

#define _POSIX_C_SOURCE 199309L

/**
 * @def Time unit used to calculate the amount of units that represents one second
 */
#define TIME_UNIT 1000 // Miliseconds

#include "../util.h"
#include "watchdog.h"
#include <malloc.h>
#include <time.h>
#include <signal.h>
#include <unistd.h>

struct Watchdog_t
{
    timer_t timer; // POSIX Timer
    uint32_t myDelay; /**< configured delay */
    WatchdogCallback myCallback; /**< function to be called at delay expiration */
    void * caller; ///< Caller instance of the watchdog
};

/**
 * @brief Calls the watchdog callback when the delay of the timer expires
 *
 * @param handlerParam must be the watchdog reference
 */
static void mainHandler (union sigval handlerParam)
{
    Watchdog *theWatchdog = handlerParam.sival_ptr; // Getting the timer reference

    theWatchdog->myCallback(theWatchdog, theWatchdog->caller); // Calling the callback function
}

Watchdog * WatchdogConstruct (uint32_t thisDelay, WatchdogCallback callback, void * caller)
{

    Watchdog *result;

    // allocates and initializes the watchdog's attributes
    result = (Watchdog *) malloc(sizeof(Watchdog));
    STOP_ON_ERROR(result == NULL, "Error during memory allocation of the watchdog : ")

    result->myDelay = thisDelay;
    result->myCallback = callback;
    result->caller = caller;

    // Create the POSIX timer

    struct sigevent event = {
            .sigev_notify = SIGEV_THREAD,
            .sigev_notify_function = mainHandler,
            .sigev_notify_attributes = NULL,
            .sigev_value.sival_ptr = (void *) result
    };


    int err = timer_create(CLOCK_REALTIME, &event, &result->timer);
    STOP_ON_ERROR(err != 0, "Error while creating the timer : ")
    return result;
}

void WatchdogStart (Watchdog *this)
{

    //Starts the POSIX timer
    struct itimerspec time;
    time.it_value.tv_sec = this->myDelay / TIME_UNIT; // Seconds
    time.it_value.tv_nsec = (this->myDelay % TIME_UNIT) * (1000000000 / TIME_UNIT); // Nanoseconds
    time.it_interval.tv_sec = time.it_value.tv_sec;
    time.it_interval.tv_nsec = time.it_value.tv_nsec;

    // Starts the POSIX timer
    int err = timer_settime(this->timer, 0, &time, NULL) ;
    STOP_ON_ERROR(err == -1, "Error when setting POSIX timer time : ")
}

void WatchdogCancel (Watchdog *this)
{
    struct itimerspec time;
    time.it_value.tv_sec = 0;
    time.it_value.tv_nsec = 0;
    time.it_interval.tv_sec = time.it_value.tv_sec;
    time.it_interval.tv_nsec = time.it_value.tv_nsec;

    // Disarms the POSIX timer
    int err = timer_settime(this->timer, 0, &time, NULL);
    STOP_ON_ERROR(err == -1, "Error when disarming the POSIX timer : ")
}

void WatchdogDestroy (Watchdog *this)
{
    // Disarms and deletes the POSIX timer
    WatchdogCancel(this);
    int err = timer_delete(this->timer);
    STOP_ON_ERROR(err==-1, "Error when deleting the POSIX timer : ")

    // Then we can free memory
    free (this);
}

