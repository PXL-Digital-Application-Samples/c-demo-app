/* Unity Test Framework - Minimal Single Header Version
 * Based on Unity by ThrowTheSwitch.org
 * Simplified for single-header inclusion
 */

#ifndef UNITY_H
#define UNITY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

// Test state
static int Unity_TestsFailed = 0;
static int Unity_TestsRun = 0;
static int Unity_CurrentTestFailed = 0;
static int Unity_CurrentTestIgnored = 0;
static const char* Unity_CurrentTestName = NULL;
static int Unity_CurrentTestLineNumber = 0;

// Color codes
#define UNITY_COLOR_RED     "\033[31m"
#define UNITY_COLOR_GREEN   "\033[32m"
#define UNITY_COLOR_YELLOW  "\033[33m"
#define UNITY_COLOR_DEFAULT "\033[0m"

// Core Unity functions
#define TEST_PROTECT() (setjmp(Unity_JumpBuffer) == 0)

#include <setjmp.h>
static jmp_buf Unity_JumpBuffer;

static void UnityPrint(const char* str) {
    printf("%s", str);
}

static void UnityPrintNumber(int num) {
    printf("%d", num);
}

static void UnityBegin(void) {
    Unity_TestsFailed = 0;
    Unity_TestsRun = 0;
    printf("\n%s========================================%s\n", UNITY_COLOR_DEFAULT, UNITY_COLOR_DEFAULT);
    printf("UNITY TEST FRAMEWORK\n");
    printf("========================================\n");
}

static int UnityEnd(void) {
    printf("\n%s========================================%s\n", UNITY_COLOR_DEFAULT, UNITY_COLOR_DEFAULT);
    printf("%d Tests %d Failures %d Ignored\n", 
           Unity_TestsRun, Unity_TestsFailed, 0);
    
    if (Unity_TestsFailed == 0) {
        printf("%sOK%s\n", UNITY_COLOR_GREEN, UNITY_COLOR_DEFAULT);
        return 0;
    } else {
        printf("%sFAILED%s\n", UNITY_COLOR_RED, UNITY_COLOR_DEFAULT);
        return Unity_TestsFailed;
    }
}

static void UnityDefaultTestRun(void (*Func)(void), const char* FuncName, int FuncLineNum) {
    Unity_CurrentTestName = FuncName;
    Unity_CurrentTestLineNumber = FuncLineNum;
    Unity_CurrentTestFailed = 0;
    Unity_CurrentTestIgnored = 0;
    Unity_TestsRun++;
    
    if (TEST_PROTECT()) {
        Func();
    }
    
    if (Unity_CurrentTestFailed) {
        Unity_TestsFailed++;
        printf("%s[FAIL]%s %s\n", UNITY_COLOR_RED, UNITY_COLOR_DEFAULT, FuncName);
    } else {
        printf("%s[PASS]%s %s\n", UNITY_COLOR_GREEN, UNITY_COLOR_DEFAULT, FuncName);
    }
}

static void UnityFail(const char* message, int line) {
    Unity_CurrentTestFailed = 1;
    printf("\n%s:%d: %s%s%s\n", Unity_CurrentTestName, line, 
           UNITY_COLOR_RED, message, UNITY_COLOR_DEFAULT);
}

static void UnityAssertEqualNumber(int expected, int actual, const char* msg, int line) {
    if (expected != actual) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s Expected %d Was %d", msg ? msg : "Values Not Equal!", expected, actual);
        UnityFail(buffer, line);
    }
}

static void UnityAssertEqualString(const char* expected, const char* actual, const char* msg, int line) {
    if (strcmp(expected, actual) != 0) {
        char buffer[256];
        snprintf(buffer, sizeof(buffer), "%s Expected '%s' Was '%s'", msg ? msg : "Strings Not Equal!", expected, actual);
        UnityFail(buffer, line);
    }
}

static void UnityAssertNull(void* pointer, const char* msg, int line) {
    if (pointer != NULL) {
        UnityFail(msg ? msg : "Expected NULL", line);
    }
}

static void UnityAssertNotNull(void* pointer, const char* msg, int line) {
    if (pointer == NULL) {
        UnityFail(msg ? msg : "Expected Not NULL", line);
    }
}

static void UnityAssertTrue(int condition, const char* msg, int line) {
    if (!condition) {
        UnityFail(msg ? msg : "Expected TRUE Was FALSE", line);
    }
}

// Macros
#define RUN_TEST(func) UnityDefaultTestRun(func, #func, __LINE__)

#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    UnityAssertEqualNumber((expected), (actual), NULL, __LINE__)

#define TEST_ASSERT_EQUAL_INT_MESSAGE(expected, actual, message) \
    UnityAssertEqualNumber((expected), (actual), (message), __LINE__)

#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    UnityAssertEqualString((expected), (actual), NULL, __LINE__)

#define TEST_ASSERT_EQUAL_STRING_MESSAGE(expected, actual, message) \
    UnityAssertEqualString((expected), (actual), (message), __LINE__)

#define TEST_ASSERT_NULL(pointer) \
    UnityAssertNull((pointer), NULL, __LINE__)

#define TEST_ASSERT_NULL_MESSAGE(pointer, message) \
    UnityAssertNull((pointer), (message), __LINE__)

#define TEST_ASSERT_NOT_NULL(pointer) \
    UnityAssertNotNull((pointer), NULL, __LINE__)

#define TEST_ASSERT_NOT_NULL_MESSAGE(pointer, message) \
    UnityAssertNotNull((pointer), (message), __LINE__)

#define TEST_ASSERT_TRUE(condition) \
    UnityAssertTrue((condition), NULL, __LINE__)

#define TEST_ASSERT_TRUE_MESSAGE(condition, message) \
    UnityAssertTrue((condition), (message), __LINE__)

#define TEST_ASSERT_FALSE(condition) \
    UnityAssertTrue(!(condition), NULL, __LINE__)

#define TEST_ASSERT_FALSE_MESSAGE(condition, message) \
    UnityAssertTrue(!(condition), (message), __LINE__)

#define TEST_ASSERT(condition) TEST_ASSERT_TRUE(condition)
#define TEST_ASSERT_MESSAGE(condition, message) TEST_ASSERT_TRUE_MESSAGE(condition, message)

#define TEST_FAIL() UnityFail("Test Failed", __LINE__)
#define TEST_FAIL_MESSAGE(message) UnityFail((message), __LINE__)

#endif // UNITY_H