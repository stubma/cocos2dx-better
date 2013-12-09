#ifndef _TESTS_H_
#define _TESTS_H_

#include "CommonTest/CommonTest.h"
#include "DBTest/DBTest.h"

enum
{
    TEST_COMMON = 0,
    TEST_DB,
    TESTS_COUNT,    
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "CommonTest",
    "DBTest",
};

#endif
