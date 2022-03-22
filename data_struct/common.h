#pragma once

#include "stdlib.h"
#include "stdio.h"

#define STRINGIFY(x) #x
#define STR(x) STRINGIFY(x)
#define GENERIC_LOG(message) printf("[FILE:'" __FILE__ "'] " "[LINE:" STR(__LINE__) "]: " message)

#define INVALID_PARAM_1 "invalid param 1\n"
#define INVALID_PARAM_2 "invalid param 2\n"
#define INVALID_PARAM_3 "invalid param 3\n"
#define INVALID_PARAMS "invalid params\n"
#define MEMORY_ALLOCATION_FAIL "memory allocation fail\n"

#define CC_UNREFERENCED_PARAMETER(X) X
