#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>


#define LOG std::cout << __FILE__ << " " << __LINE__ << ": "

#define LOG_ENDL std::endl

#define DUMP_TEST_LINE LOG << "gets here!" << LOG_ENDL
#define DUMP_TEST LOG << "here with: " 

#endif
