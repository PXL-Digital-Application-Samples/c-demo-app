#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "mongoose.h"
#include "users.h"
#include "routes.h"
#include "swagger.h"

#ifdef _WIN32
#include <windows.h>
#define sleep(x) Sleep((x) * 1000)
#else
#include <unistd.h>
#endif

#define PORT 5000

static struct mg_mgr mgr;
static volatile int s_exit = 0;

void handle_shutdown(int sig) {
    printf("\nShutting down server...\n");
    s_exit = 1;
    shutdown_users();
    exit(0);
}

int main(int argc, char *argv[]) {
    int port = PORT;
    
    // Check for PORT environment variable
    char *env_port = getenv("PORT");
    if (env_port) {
        port = atoi(env_port);
    }
    
    // Initialize users
    init_users();
    seed_users();
    
    // Set up signal handler
    signal(SIGINT, handle_shutdown);
    signal(SIGTERM, handle_shutdown);
    
    // Start HTTP daemon
    // Initialize mongoose manager
    mg_mgr_init(&mgr);
    
    // Create listening address
    char addr[64];
    snprintf(addr, sizeof(addr), "http://0.0.0.0:%d", port);
    
    // Start HTTP server
    struct mg_connection *c = mg_http_listen(&mgr, addr, handle_mongoose_request, NULL);
    
    if (c == NULL) {
        fprintf(stderr, "Failed to start server on port %d\n", port);
        return 1;
    }
    
    printf("Server running on http://0.0.0.0:%d\n", port);
    printf("Swagger UI available at http://localhost:%d/\n", port);
    printf("Press Ctrl+C to stop...\n");
    
    // Event loop
    while (!s_exit) {
        mg_mgr_poll(&mgr, 1000);
    }
    
    mg_mgr_free(&mgr);
    
    return 0;
}