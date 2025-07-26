#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _WIN32
#include <windows.h>
// Windows threading
typedef CRITICAL_SECTION pthread_mutex_t;
#define PTHREAD_MUTEX_INITIALIZER {0}
static inline int pthread_mutex_init(pthread_mutex_t *mutex, void *attr) {
    InitializeCriticalSection(mutex);
    return 0;
}
static inline int pthread_mutex_lock(pthread_mutex_t *mutex) {
    EnterCriticalSection(mutex);
    return 0;
}
static inline int pthread_mutex_unlock(pthread_mutex_t *mutex) {
    LeaveCriticalSection(mutex);
    return 0;
}
static inline int pthread_mutex_destroy(pthread_mutex_t *mutex) {
    DeleteCriticalSection(mutex);
    return 0;
}
#else
#include <pthread.h>
#endif
#include "users.h"

static User *users_head = NULL;
static int next_id = 1;
static pthread_mutex_t users_mutex;
static int mutex_initialized = 0;

void init_users(void) {
    if (!mutex_initialized) {
        pthread_mutex_init(&users_mutex, NULL);
        mutex_initialized = 1;
    }
    pthread_mutex_lock(&users_mutex);
    users_head = NULL;
    next_id = 1;
    pthread_mutex_unlock(&users_mutex);
}

void cleanup_users(void) {
    if (mutex_initialized) {
        pthread_mutex_lock(&users_mutex);
        User *current = users_head;
        while (current) {
            User *next = current->next;
            free(current->name);
            free(current->email);
            free(current);
            current = next;
        }
        users_head = NULL;
        next_id = 1;
        pthread_mutex_unlock(&users_mutex);
        // Don't destroy mutex in tests - let it persist for multiple test runs
        // pthread_mutex_destroy(&users_mutex);
        // mutex_initialized = 0;
    }
}

void shutdown_users(void) {
    cleanup_users();
    if (mutex_initialized) {
        pthread_mutex_destroy(&users_mutex);
        mutex_initialized = 0;
    }
}

void seed_users(void) {
    create_user("Alice", "alice@example.com");
    create_user("Bob", "bob@example.com");
    create_user("Charlie", "charlie@example.com");
}

User* create_user(const char *name, const char *email) {
    if (!name || !email) return NULL;
    
    pthread_mutex_lock(&users_mutex);
    
    User *new_user = (User*)malloc(sizeof(User));
    if (!new_user) {
        pthread_mutex_unlock(&users_mutex);
        return NULL;
    }
    
    new_user->id = next_id++;
    new_user->name = strdup(name);
    new_user->email = strdup(email);
    new_user->next = users_head;
    users_head = new_user;
    
    pthread_mutex_unlock(&users_mutex);
    return new_user;
}

cJSON* get_all_users(void) {
    pthread_mutex_lock(&users_mutex);
    
    cJSON *array = cJSON_CreateArray();
    User *current = users_head;
    
    while (current) {
        cJSON *user_json = user_to_json(current);
        cJSON_AddItemToArray(array, user_json);
        current = current->next;
    }
    
    pthread_mutex_unlock(&users_mutex);
    return array;
}

User* get_user_by_id(int id) {
    pthread_mutex_lock(&users_mutex);
    
    User *current = users_head;
    while (current) {
        if (current->id == id) {
            pthread_mutex_unlock(&users_mutex);
            return current;
        }
        current = current->next;
    }
    
    pthread_mutex_unlock(&users_mutex);
    return NULL;
}

User* update_user(int id, const char *name, const char *email) {
    pthread_mutex_lock(&users_mutex);
    
    User *user = NULL;
    User *current = users_head;
    
    while (current) {
        if (current->id == id) {
            user = current;
            break;
        }
        current = current->next;
    }
    
    if (user) {
        if (name) {
            free(user->name);
            user->name = strdup(name);
        }
        if (email) {
            free(user->email);
            user->email = strdup(email);
        }
    }
    
    pthread_mutex_unlock(&users_mutex);
    return user;
}

int delete_user(int id) {
    pthread_mutex_lock(&users_mutex);
    
    User *current = users_head;
    User *prev = NULL;
    
    while (current) {
        if (current->id == id) {
            if (prev) {
                prev->next = current->next;
            } else {
                users_head = current->next;
            }
            free(current->name);
            free(current->email);
            free(current);
            pthread_mutex_unlock(&users_mutex);
            return 1;
        }
        prev = current;
        current = current->next;
    }
    
    pthread_mutex_unlock(&users_mutex);
    return 0;
}

cJSON* user_to_json(User *user) {
    if (!user) return NULL;
    
    cJSON *json = cJSON_CreateObject();
    cJSON_AddNumberToObject(json, "id", user->id);
    cJSON_AddStringToObject(json, "name", user->name);
    cJSON_AddStringToObject(json, "email", user->email);
    return json;
}