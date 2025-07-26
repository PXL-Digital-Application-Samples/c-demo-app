#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <cjson/cJSON.h>
#include "unity.h"
#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x) * 1000)
typedef struct {
    CRITICAL_SECTION cs;
} mutex_t;

static int mutex_init(mutex_t *mutex) {
    InitializeCriticalSection(&mutex->cs);
    return 0;
}

static int mutex_lock(mutex_t *mutex) {
    EnterCriticalSection(&mutex->cs);
    return 0;
}

static int mutex_unlock(mutex_t *mutex) {
    LeaveCriticalSection(&mutex->cs);
    return 0;
}

static int mutex_destroy(mutex_t *mutex) {
    DeleteCriticalSection(&mutex->cs);
    return 0;
}
#else
#include <pthread.h>
#include <unistd.h>
typedef pthread_mutex_t mutex_t;
#define mutex_init(m) pthread_mutex_init(m, NULL)
#define mutex_lock(m) pthread_mutex_lock(m)
#define mutex_unlock(m) pthread_mutex_unlock(m)
#define mutex_destroy(m) pthread_mutex_destroy(m)
#endif
#include "users.h"
#include "routes.h"

void setUp(void) {
    // Initialize users storage before each test
}

void tearDown(void) {
    // Clean up after each test
}

void test_users_basic_functionality(void) {
    // Test basic user creation and retrieval
    User *user = create_user("Test User", "test@example.com");
    TEST_ASSERT_NOT_NULL(user);
    TEST_ASSERT_EQUAL_INT(1, user->id);
    TEST_ASSERT_EQUAL_STRING("Test User", user->name);
    TEST_ASSERT_EQUAL_STRING("test@example.com", user->email);
    
    // Test finding user by ID
    User *found = find_user_by_id(1);
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_STRING("Test User", found->name);
    
    // Test user not found
    User *not_found = find_user_by_id(999);
    TEST_ASSERT_NULL(not_found);
}

void test_user_deletion(void) {
    // Create a user
    User *user = create_user("Delete Me", "delete@example.com");
    TEST_ASSERT_NOT_NULL(user);
    int user_id = user->id;
    
    // Verify user exists
    User *found = find_user_by_id(user_id);
    TEST_ASSERT_NOT_NULL(found);
    
    // Delete user
    int result = delete_user(user_id);
    TEST_ASSERT_TRUE(result);
    
    // Verify user is gone
    User *not_found = find_user_by_id(user_id);
    TEST_ASSERT_NULL(not_found);
    
    // Try to delete non-existent user
    int result2 = delete_user(999);
    TEST_ASSERT_FALSE(result2);
}

void test_multiple_users(void) {
    // Create multiple users
    User *user1 = create_user("User One", "one@example.com");
    User *user2 = create_user("User Two", "two@example.com");
    User *user3 = create_user("User Three", "three@example.com");
    
    TEST_ASSERT_NOT_NULL(user1);
    TEST_ASSERT_NOT_NULL(user2);
    TEST_ASSERT_NOT_NULL(user3);
    
    // Test that IDs are unique and incremental
    TEST_ASSERT_NOT_EQUAL(user1->id, user2->id);
    TEST_ASSERT_NOT_EQUAL(user2->id, user3->id);
    TEST_ASSERT_NOT_EQUAL(user1->id, user3->id);
    
    // Test finding each user
    User *found1 = find_user_by_id(user1->id);
    User *found2 = find_user_by_id(user2->id);
    User *found3 = find_user_by_id(user3->id);
    
    TEST_ASSERT_NOT_NULL(found1);
    TEST_ASSERT_NOT_NULL(found2);
    TEST_ASSERT_NOT_NULL(found3);
    
    TEST_ASSERT_EQUAL_STRING("User One", found1->name);
    TEST_ASSERT_EQUAL_STRING("User Two", found2->name);
    TEST_ASSERT_EQUAL_STRING("User Three", found3->name);
}

void test_user_json_conversion(void) {
    // Create a user
    User *user = create_user("JSON User", "json@example.com");
    TEST_ASSERT_NOT_NULL(user);
    
    // Convert to JSON
    cJSON *json = user_to_json(user);
    TEST_ASSERT_NOT_NULL(json);
    
    // Check JSON fields
    cJSON *id = cJSON_GetObjectItem(json, "id");
    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *email = cJSON_GetObjectItem(json, "email");
    
    TEST_ASSERT_NOT_NULL(id);
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_NOT_NULL(email);
    
    TEST_ASSERT_TRUE(cJSON_IsNumber(id));
    TEST_ASSERT_TRUE(cJSON_IsString(name));
    TEST_ASSERT_TRUE(cJSON_IsString(email));
    
    TEST_ASSERT_EQUAL_INT(user->id, id->valueint);
    TEST_ASSERT_EQUAL_STRING("JSON User", name->valuestring);
    TEST_ASSERT_EQUAL_STRING("json@example.com", email->valuestring);
    
    cJSON_Delete(json);
}

void test_get_all_users_json(void) {
    // Create some users
    create_user("First User", "first@example.com");
    create_user("Second User", "second@example.com");
    
    // Get all users as JSON
    cJSON *users_json = get_all_users();
    TEST_ASSERT_NOT_NULL(users_json);
    TEST_ASSERT_TRUE(cJSON_IsArray(users_json));
    
    int array_size = cJSON_GetArraySize(users_json);
    TEST_ASSERT_TRUE(array_size >= 2);  // At least the 2 we just created
    
    cJSON_Delete(users_json);
}

int main(void) {
    UNITY_BEGIN();
    
    RUN_TEST(test_users_basic_functionality);
    RUN_TEST(test_user_deletion);
    RUN_TEST(test_multiple_users);
    RUN_TEST(test_user_json_conversion);
    RUN_TEST(test_get_all_users_json);
    
    return UNITY_END();
}
