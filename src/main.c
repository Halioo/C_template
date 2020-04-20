
#include "example/example.h"

int main() {

     Example * test = ExampleNew();
     ExampleStart(test);

     ExampleEventOne(test, 1);
     ExampleEventTwo(test, 2);

     ExampleStop(test);
     ExampleFree(test);
}
