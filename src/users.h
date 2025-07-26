#ifndef USERS_H
#define USERS_H

#include <cjson/cJSON.h>

typedef struct User {
    int id;
    char *name;
    char *email;
    struct User *next;
} User;

// Initialize user system
void init_users(void);

// Cleanup user system
void cleanup_users(void);

// Shutdown user system (for final cleanup)
void shutdown_users(void);

// Seed initial users
void seed_users(void);

// Create a new user
User* create_user(const char *name, const char *email);

// Get all users as JSON array
cJSON* get_all_users(void);

// Get user by ID
User* get_user_by_id(int id);

// Update user
User* update_user(int id, const char *name, const char *email);

// Delete user
int delete_user(int id);

// Convert user to JSON
cJSON* user_to_json(User *user);

#endif // USERS_H