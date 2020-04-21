/**
 * @file main.c
 *
 * @brief Launch Example program with one active object
 *
 * @date April 2020

 * @authors Clément PUYBAREAU, Louis FROGER
 *
 * @version 1.0
 *
 * @copyright CCBY 4.0
 */


#include "example/example.h"

int main() {

     Example * test = ExampleNew();
     ExampleStart(test);

     ExampleEventOne(test, 1);
     ExampleEventTwo(test, 2);

     ExampleStop(test);
     ExampleFree(test);
}
