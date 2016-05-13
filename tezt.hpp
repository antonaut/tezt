#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <chrono>
#include <ctime>
#include <cstdlib>

namespace tezt {

    bool ALL_RIGHT = true;

    void (*before_fn)(void) = []{};
    void (*after_fn)(void) = []{};

    std::map<std::string, void (*)(void)> tests;
    template <typename T> void ane(T expected, T actual) {
        if (expected == actual) {
            std::cerr << "FAIL: Expected: " << expected
                      << ", got: " << actual << ". :(" << std::endl;
            ALL_RIGHT = false;
        }
    }
    

    template <typename A, typename B> void aneq_op(A a, B b) {
        if (a != b) {
            return;
        }
        std::cerr << "FAIL: Not equals operator fail. :(" << std::endl;
        ALL_RIGHT = false;
    }
    template <typename A, typename B> void aeq_op(A a, B b) {
        if (a == b) {
            return;
        }
        std::cerr << "FAIL: Equals operator fail. :(" << std::endl;
        ALL_RIGHT = false;
    }

    
    template <typename T> void ane(T expected, T actual, std::string tc) {
        if (expected == actual) {
            std::cerr << "FAIL: [" << tc << "] - Expected: " << expected
                      << ", got: " << actual << ". :(" << std::endl;
            ALL_RIGHT = false;
        }
    }


    template <typename T> void ae(T expected, T actual) {
        if (expected != actual) {
            std::cerr << "FAIL: Expected: " << expected
                      << ", got: " << actual << ". :(" << std::endl;
            ALL_RIGHT = false;
        }
    }

    template <typename T> void ae(T expected, T actual, std::string tc) {
        if (expected != actual) {
            std::cerr << "FAIL: [" << tc << "] - Expected: " << expected
                      << ", got: " << actual << ". :(" << std::endl;
            ALL_RIGHT = false;
        }
    }

    void fail(std::string msg, std::exception e) {
        std::cerr << "FAIL: Test: " << msg << " - Exception:" << e.what() << " :/" << std::endl;
        ALL_RIGHT = false;
    }
    
    void fail(std::string msg) {
        std::cerr << "FAIL: Msg: " << msg << " :/" << std::endl;
        ALL_RIGHT = false;
    }


    void fail() {
        std::cerr << "FAIL: Just fail. :/" << std::endl;
        ALL_RIGHT = false;
    }

        
    void add(std::string testname, void (*testfn)(void)) {
        tests[testname] = testfn;
    }

    void should_not_throw(std::string msg, void (*lambda)(void)) {
        try {
            lambda();
        } catch (std::exception const &ex) {
            fail("Should not throw. " + msg);
        }
    }
    template <class Ex>
    void should_throw(std::string msg, void (*lambda)(void)) {

        try {
            lambda();
        } catch (Ex const &ex) {
            return;
        }
        fail("Should have thrown. " + msg);
    }

    void before(void (*fn)(void)) {
        before_fn = fn;
    }
    
    void after(void (*fn)(void)) {
        after_fn = fn;
    }


    int run() {
        std::chrono::time_point<std::chrono::system_clock> total_start, total_end, test_start, test_end;
        total_start = std::chrono::system_clock::now();
        std::cout << "*** Starting test suite. ***" << std::endl;

        for (auto it : tests) {

            before_fn();

            std::cout << "Running: " << it.first << std::endl;
            test_start = std::chrono::system_clock::now();
            try {
                it.second();
            } catch (std::exception const &e) {
                fail(it.first, e);
            }

            test_end = std::chrono::system_clock::now();
            std::chrono::duration<double> test_elapsed_seconds = test_end-test_start;
            std::cout << "Done: It took "
                      << std::fixed
                      << std::setprecision(5)
                      << test_elapsed_seconds.count()
                      << "s."
                      << std::endl;
            
            after_fn();
        }
    
        total_end = std::chrono::system_clock::now();
        std::chrono::duration<double> total_elapsed_seconds = total_end-total_start;
        std::cout << std::endl << "*** Done with all tests. ***" << std::endl;

        std::cout << "Total time elapsed: " << total_elapsed_seconds.count() << "s" << std::endl;

        if (ALL_RIGHT) {
            std::cout << "Ran all tests without errors!" << std::endl;
            return EXIT_SUCCESS;
        }
        
        return -1;
    }

}
