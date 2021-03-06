#ifndef RUN_TESTS
#define RUN_TESTS
#endif // RUN_TESTS

#include "acutest.h"
#include "pmemory.cc"
#include "pnumber.cc"
#include "proc.cc"
#include "complex.cc"
#include "fractional.cc"
#include "converter.cc"
#include "editor.cc"
#include "history.cc"
#include "control.cc"

// acutest provide main func
TEST_LIST = {
    // TPNumber
    {"pnumber_constructor", test_pnumber_constructor},
    {"pnumber_constructor-exception", test_pnumber_constructor_exception},
    {"pnumber_setters", test_pnumber_setters},
    {"pnumber_to_string", test_pnumber_to_string},
    {"pnumber_operations", test_pnumber_operations},
    {"pnumber_fraction_carry", test_pnumber_fraction_carry},
    // TMemory
    {"pmemory_constructor", test_pmemory_constructor},
    {"pmemory_operations", test_pmemory_operations},
    // TProc
    {"proc_constructor_and_operands", test_proc_construction},
    {"proc_functions", test_proc_functions},
    {"proc_operations", test_proc_operations},
    // Complex
    {"complex_constructor", test_complex_constructor},
    {"complex_operations", test_complex_operations},
    // Fractional
    {"fractional_constructor", test_fractional_construction},
    {"fractional_operations", test_fractional_operations},
    // Converter
    {"converter_10_p_operations", TestNConverter::test_converter_10_p_operations},
    {"converter_p_10_operations", TestNConverter::test_converter_p_10_operations},
    // Editor
    {"editor_operations", test_editor_operations},
    // History
    {"history", test_history},
    // Control
    {"control", test_control_operations},
    {NULL, NULL}};
