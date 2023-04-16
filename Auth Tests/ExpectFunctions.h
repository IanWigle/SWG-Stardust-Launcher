#pragma once
#include "AuthTestsPCH.h"

static void ExpectFalse(const char* test, bool value) {
    if (value) {
        LogMessage("ERROR: %s is true instead of false", test);
    }
    else {
        LogMessage("%s is false, as expected", test);
    }
}

static void ExpectTrue(const char* test, bool value) {
    if (value) {
        LogMessage("%s is true, as expected", test);
    }
    else {
        LogMessage("ERROR: %s is false instead of true", test);
    }
}

// Log results of a string comparison for `test`.
static void ExpectStringsEqual(const char* test, const char* expected,
    const char* actual) {
    if (strcmp(expected, actual) == 0) {
        LogMessage("%s is '%s' as expected", test, actual);
    }
    else {
        LogMessage("ERROR: %s is '%s' instead of '%s'", test, actual, expected);
    }
}