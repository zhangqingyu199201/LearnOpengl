#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <iostream>
#include <tuple>
#include <tuple>
#include <vector>
#include <sstream>
#include <map>
#include <array>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <vector>

#define MYASSERT(con)                                                            \
    if (!(con)) {                                                                \
        ::std::cout << "Error: " << __FILE__ << "  " << __LINE__ << ::std::endl; \
    }

#define START_TEST ::std::cout << "StartTest: " << __FILE__ << "  " << __LINE__ << ::std::endl;
#define END_TEST ::std::cout << "FinishTest: " << __FILE__ << "  " << __LINE__ << ::std::endl;

#ifndef RESOURCE_DIR
#define RESOURCE_DIR ""
#endif

#define ROOT_PATH RESOURCE_DIR
#define MOVIE_PATH ::std::string(RESOURCE_DIR) + "/movie.flv"
#define WAIT_TODO ::std::cout << "WaitToDo: " << __FILE__ << "  " << __LINE__ << ::std::endl;
