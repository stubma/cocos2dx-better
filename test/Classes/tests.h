#ifndef _TESTS_H_
#define _TESTS_H_

#include "CommonTest/CommonTest.h"
#include "ActionTest/ActionTest.h"
#include "AnimTest/AnimTest.h"
#include "DBTest/DBTest.h"
#include "JSONTest/JSONTest.h"
#include "NetworkTest/NetworkTest.h"

enum
{
    TEST_COMMON = 0,
    TEST_ACTION,
    TEST_ANIM,
    TEST_DB,
    TEST_JSON,
	TEST_NETWORK,
    TESTS_COUNT,    
};

const std::string g_aTestNames[TESTS_COUNT] = {
    "CommonTest",
    "ActionTest",
    "AnimTest",
    "DBTest",
    "JSONTest",
	"NetworkTest"
};

#endif
