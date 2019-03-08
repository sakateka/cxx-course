#ifndef RUN_TESTS
#define RUN_TESTS
#endif // RUN_TESTS

#include "acutest.h"
#include "pmemory.cc"
#include "pnumber.cc"

// acutest provide main func
TEST_LIST = {
    {"pnumber_constructor", test_pnumber_constructor},
    {"pnumber_constructor-exception", test_pnumber_constructor_exception},
    {"pnumber_setters", test_pnumber_setters},
    {"pnumber_to_string", test_pnumber_to_string},
    {"pnumber_operations", test_pnumber_operations},
    {NULL, NULL}};
