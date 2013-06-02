#ifndef _TESTS_H_
#define _TESTS_H_

#include "CommonTest/CommonTest.h"

enum
{
    TEST_SHAKE = 0,
	TEST_COLOR_LABEL,
    TESTS_COUNT,    
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "CommonTest",
};

#endif
