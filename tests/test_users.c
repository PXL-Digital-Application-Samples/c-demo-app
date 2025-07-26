#include "unity.h"
#include "users.h"

void setUp(void) {
    init_users();
}

void tearDown(void) {
    cleanup_users();
}

void test_create_user_should_create_new_user(void) {
    User *user = create_user("Test User", "test@example.com");
    
    TEST_ASSERT_NOT_NULL(user);
    TEST_ASSERT_EQUAL_INT(1, user->id);
    TEST_ASSERT_EQUAL_STRING("Test User", user->name);
    TEST_ASSERT_EQUAL_STRING("test@example.com", user->email);
}

void test_create_user_should_increment_ids(void) {
    User *user1 = create_user("User 1", "user1@example.com");
    User *user2 = create_user("User 2", "user2@example.com");
    User *user3 = create_user("User 3", "user3@example.com");
    
    TEST_ASSERT_EQUAL_INT(1, user1->id);
    TEST_ASSERT_EQUAL_INT(2, user2->id);
    TEST_ASSERT_EQUAL_INT(3, user3->id);
}

void test_create_user_should_reject_null_input(void) {
    User *user1 = create_user(NULL, "test@example.com");
    User *user2 = create_user("Test", NULL);
    User *user3 = create_user(NULL, NULL);
    
    TEST_ASSERT_NULL(user1);
    TEST_ASSERT_NULL(user2);
    TEST_ASSERT_NULL(user3);
}

void test_get_user_by_id_should_find_existing_user(void) {
    User *created = create_user("Find Me", "findme@example.com");
    int id = created->id;
    
    User *found = get_user_by_id(id);
    
    TEST_ASSERT_NOT_NULL(found);
    TEST_ASSERT_EQUAL_INT(id, found->id);
    TEST_ASSERT_EQUAL_STRING("Find Me", found->name);
    TEST_ASSERT_EQUAL_STRING("findme@example.com", found->email);
}

void test_get_user_by_id_should_return_null_for_nonexistent(void) {
    User *not_found = get_user_by_id(9999);
    TEST_ASSERT_NULL(not_found);
}

void test_update_user_should_update_name_only(void) {
    User *user = create_user("Original Name", "original@example.com");
    int id = user->id;
    
    User *updated = update_user(id, "New Name", NULL);
    
    TEST_ASSERT_NOT_NULL(updated);
    TEST_ASSERT_EQUAL_STRING("New Name", updated->name);
    TEST_ASSERT_EQUAL_STRING("original@example.com", updated->email);
}

void test_update_user_should_update_email_only(void) {
    User *user = create_user("Test User", "original@example.com");
    int id = user->id;
    
    User *updated = update_user(id, NULL, "new@example.com");
    
    TEST_ASSERT_NOT_NULL(updated);
    TEST_ASSERT_EQUAL_STRING("Test User", updated->name);
    TEST_ASSERT_EQUAL_STRING("new@example.com", updated->email);
}

void test_update_user_should_update_both_fields(void) {
    User *user = create_user("Old Name", "old@example.com");
    int id = user->id;
    
    User *updated = update_user(id, "New Name", "new@example.com");
    
    TEST_ASSERT_NOT_NULL(updated);
    TEST_ASSERT_EQUAL_STRING("New Name", updated->name);
    TEST_ASSERT_EQUAL_STRING("new@example.com", updated->email);
}

void test_update_user_should_return_null_for_nonexistent(void) {
    User *updated = update_user(9999, "Ghost", "ghost@example.com");
    TEST_ASSERT_NULL(updated);
}

void test_delete_user_should_remove_existing_user(void) {
    User *user = create_user("Delete Me", "delete@example.com");
    int id = user->id;
    
    // Verify user exists
    User *found = get_user_by_id(id);
    TEST_ASSERT_NOT_NULL(found);
    
    // Delete user
    int result = delete_user(id);
    TEST_ASSERT_EQUAL_INT(1, result);
    
    // Verify user is gone
    found = get_user_by_id(id);
    TEST_ASSERT_NULL(found);
}

void test_delete_user_should_fail_for_nonexistent(void) {
    int result = delete_user(9999);
    TEST_ASSERT_EQUAL_INT(0, result);
}

void test_get_all_users_should_return_json_array(void) {
    // Start with empty list
    cJSON *users = get_all_users();
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(0, cJSON_GetArraySize(users));
    cJSON_Delete(users);
    
    // Add some users
    create_user("User 1", "user1@example.com");
    create_user("User 2", "user2@example.com");
    create_user("User 3", "user3@example.com");
    
    // Get all users
    users = get_all_users();
    TEST_ASSERT_NOT_NULL(users);
    TEST_ASSERT_TRUE(cJSON_IsArray(users));
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users));
    
    // Verify first user in array
    cJSON *first_user = cJSON_GetArrayItem(users, 0);
    TEST_ASSERT_NOT_NULL(first_user);
    
    cJSON *name = cJSON_GetObjectItem(first_user, "name");
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(cJSON_IsString(name));
    
    cJSON_Delete(users);
}

void test_user_to_json_should_convert_user_struct(void) {
    User *user = create_user("JSON User", "json@example.com");
    cJSON *json = user_to_json(user);
    
    TEST_ASSERT_NOT_NULL(json);
    
    // Check ID
    cJSON *id = cJSON_GetObjectItem(json, "id");
    TEST_ASSERT_NOT_NULL(id);
    TEST_ASSERT_TRUE(cJSON_IsNumber(id));
    TEST_ASSERT_EQUAL_INT(user->id, id->valueint);
    
    // Check name
    cJSON *name = cJSON_GetObjectItem(json, "name");
    TEST_ASSERT_NOT_NULL(name);
    TEST_ASSERT_TRUE(cJSON_IsString(name));
    TEST_ASSERT_EQUAL_STRING("JSON User", name->valuestring);
    
    // Check email
    cJSON *email = cJSON_GetObjectItem(json, "email");
    TEST_ASSERT_NOT_NULL(email);
    TEST_ASSERT_TRUE(cJSON_IsString(email));
    TEST_ASSERT_EQUAL_STRING("json@example.com", email->valuestring);
    
    cJSON_Delete(json);
}

void test_user_to_json_should_handle_null(void) {
    cJSON *json = user_to_json(NULL);
    TEST_ASSERT_NULL(json);
}

void test_seed_users_should_create_three_users(void) {
    seed_users();
    
    cJSON *users = get_all_users();
    TEST_ASSERT_EQUAL_INT(3, cJSON_GetArraySize(users));
    
    // Verify seeded users exist
    User *alice = get_user_by_id(1);
    TEST_ASSERT_NOT_NULL(alice);
    TEST_ASSERT_EQUAL_STRING("Alice", alice->name);
    TEST_ASSERT_EQUAL_STRING("alice@example.com", alice->email);
    
    User *bob = get_user_by_id(2);
    TEST_ASSERT_NOT_NULL(bob);
    TEST_ASSERT_EQUAL_STRING("Bob", bob->name);
    TEST_ASSERT_EQUAL_STRING("bob@example.com", bob->email);
    
    User *charlie = get_user_by_id(3);
    TEST_ASSERT_NOT_NULL(charlie);
    TEST_ASSERT_EQUAL_STRING("Charlie", charlie->name);
    TEST_ASSERT_EQUAL_STRING("charlie@example.com", charlie->email);
    
    cJSON_Delete(users);
}

void test_concurrent_operations_should_maintain_consistency(void) {
    // Create multiple users quickly
    for (int i = 0; i < 10; i++) {
        char name[50];
        char email[50];
        sprintf(name, "User %d", i);
        sprintf(email, "user%d@example.com", i);
        User *user = create_user(name, email);
        TEST_ASSERT_NOT_NULL(user);
        TEST_ASSERT_EQUAL_INT(i + 1, user->id);
    }
    
    // Verify all users exist
    cJSON *users = get_all_users();
    TEST_ASSERT_EQUAL_INT(10, cJSON_GetArraySize(users));
    cJSON_Delete(users);
    
    // Delete even-numbered users
    for (int i = 2; i <= 10; i += 2) {
        int result = delete_user(i);
        TEST_ASSERT_EQUAL_INT(1, result);
    }
    
    // Verify 5 users remain
    users = get_all_users();
    TEST_ASSERT_EQUAL_INT(5, cJSON_GetArraySize(users));
    cJSON_Delete(users);
}

int main(void) {
    UnityBegin();
    
    RUN_TEST(test_create_user_should_create_new_user);
    RUN_TEST(test_create_user_should_increment_ids);
    RUN_TEST(test_create_user_should_reject_null_input);
    RUN_TEST(test_get_user_by_id_should_find_existing_user);
    RUN_TEST(test_get_user_by_id_should_return_null_for_nonexistent);
    RUN_TEST(test_update_user_should_update_name_only);
    RUN_TEST(test_update_user_should_update_email_only);
    RUN_TEST(test_update_user_should_update_both_fields);
    RUN_TEST(test_update_user_should_return_null_for_nonexistent);
    RUN_TEST(test_delete_user_should_remove_existing_user);
    RUN_TEST(test_delete_user_should_fail_for_nonexistent);
    RUN_TEST(test_get_all_users_should_return_json_array);
    RUN_TEST(test_user_to_json_should_convert_user_struct);
    RUN_TEST(test_user_to_json_should_handle_null);
    RUN_TEST(test_seed_users_should_create_three_users);
    RUN_TEST(test_concurrent_operations_should_maintain_consistency);
    
    return UnityEnd();
}