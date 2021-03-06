# Tezt

This is a simple test framework for C++ (using at least c++11).


## Usage & features

I have put the tests in a file test.cpp, using Make for building and running.

### Example test.cpp for testing a Date class


```cpp
#include <chrono>
#include <iostream>

#include "tezt.hpp"
#include "gregorian.hpp" // Class under test
```


All test functions have the same signature.


```cpp
/* void (*fn)(void) */

void simple_lab_test_one( void ) {
    Gregorian g(1900, 1, 1);
    Gregorian j(1899, 12, 31);

    tezt::aneq_op(j, g);
    j++;
    tezt::aeq_op(j, g); 
}
```

A main function where you register the tests, what to do before and
after them etc.


```cpp
int main(int argc, char **argv) {

    // before is run before every test
    tezt::before([]{
            time_t tp;
            time(&tp);
            set_k_time(tp);    // Set time to today's date
    });


    // after is run after every test
    tezt::after([]{
    
    });

    // add the test
    tezt::add("Test name", simple_lab_test_one);

    return tezt::run();
}
```


## Example Makefile

I run the tests with `make test`.

```makefile
CC    = g++
LDFLAGS = -L /usr/lib/gcc/x86_64-linux-gnu/4.8
CXXFLAGS = -std=c++11 -g -Wall -pedantic

%.obj: %.cpp
	$(CC) $(CXXFLAGS) $*.cpp -c -o $*.obj 

%.out: %.cpp
	$(CC) $(CXXFLAGS) $*.cpp -o $*.out $(LDFLAGS)

clean:
	rm -f *.obj *.out

test: clean test.obj
	$(CC) $(CXXFLAGS) -o test.out test.obj $(LDFLAGS)
	./test.out

```
