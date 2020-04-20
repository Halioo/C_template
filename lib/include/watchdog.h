/**
 * @file watchdog.h
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


#ifndef WATCHDOG_H
#define WATCHDOG_H

#include <stdint.h>

/**
 * The watchdog structure
 */
typedef struct Watchdog_t Watchdog;

/**
 * @brief Function that will be called by the watchdog
 *
 * @param this the watchdog which have called this function
 */
typedef void (*WatchdogCallback)(Watchdog *this, void * caller);

/**
 * @brief Watchdog's constructor.
 *
 * @param delay expressed in microseconds
 * @param callback function to be called at expiration
 * @param caller instance of the class that calls the watchdog
 */
extern Watchdog *WatchdogConstruct(uint32_t delay, WatchdogCallback callback, void * caller);

/**
 * @brief Arms the watchdog.
 *
 * @param this watchdog's instance
 */
extern void WatchdogStart(Watchdog *this);

/**
 * @brief Disarms the watchdog.
 *
 * @param this watchdog's instance
 */
extern void WatchdogCancel(Watchdog *this);

/**
 * @brief Watchdog's destructor
 *
 * @param this watchdog's instance
 */
extern void WatchdogDestroy(Watchdog *this);

#endif /* WATCHDOG_H */