#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mongoose.h"
#include "cjson/cJSON.h"
#include "routes.h"
#include "users.h"
#include "swagger.h"

static void send_json_response(struct mg_connection *c, int status_code, cJSON *json) {
    char *response_str = cJSON_Print(json);
    mg_printf(c, "HTTP/1.1 %d %s\r\n"
                 "Content-Type: application/json\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                 "Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With, Accept, Origin\r\n"
                 "Content-Length: %d\r\n\r\n%s",
              status_code, 
              status_code == 200 ? "OK" : 
              status_code == 201 ? "Created" : 
              status_code == 404 ? "Not Found" : "Bad Request",
              (int)strlen(response_str), response_str);
    free(response_str);
}

static void send_text_response(struct mg_connection *c, int status_code, const char *content_type, const char *body) {
    mg_printf(c, "HTTP/1.1 %d %s\r\n"
                 "Content-Type: %s\r\n"
                 "Access-Control-Allow-Origin: *\r\n"
                 "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                 "Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With, Accept, Origin\r\n"
                 "Content-Length: %d\r\n\r\n%s",
              status_code,
              status_code == 200 ? "OK" : 
              status_code == 404 ? "Not Found" : "Error",
              content_type, (int)strlen(body), body);
}

static void handle_get_users(struct mg_connection *c) {
    cJSON *users = get_all_users();
    send_json_response(c, 200, users);
    cJSON_Delete(users);
}

static void handle_get_user(struct mg_connection *c, int user_id) {
    User *user = get_user_by_id(user_id);
    if (user == NULL) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "User not found");
        send_json_response(c, 404, error);
        cJSON_Delete(error);
        return;
    }
    
    cJSON *user_json = user_to_json(user);
    send_json_response(c, 200, user_json);
    cJSON_Delete(user_json);
}

static void handle_create_user(struct mg_connection *c, const char *data) {
    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "Invalid JSON");
        send_json_response(c, 400, error);
        cJSON_Delete(error);
        return;
    }
    
    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *email = cJSON_GetObjectItem(json, "email");
    
    if (!cJSON_IsString(name) || !cJSON_IsString(email)) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "Missing name or email");
        send_json_response(c, 400, error);
        cJSON_Delete(error);
        cJSON_Delete(json);
        return;
    }
    
    User *user = create_user(name->valuestring, email->valuestring);
    cJSON *user_json = user_to_json(user);
    send_json_response(c, 201, user_json);
    cJSON_Delete(user_json);
    cJSON_Delete(json);
}

static void handle_update_user(struct mg_connection *c, int user_id, const char *data) {
    User *existing_user = get_user_by_id(user_id);
    if (existing_user == NULL) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "User not found");
        send_json_response(c, 404, error);
        cJSON_Delete(error);
        return;
    }
    
    cJSON *json = cJSON_Parse(data);
    if (json == NULL) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "Invalid JSON");
        send_json_response(c, 400, error);
        cJSON_Delete(error);
        return;
    }
    
    cJSON *name = cJSON_GetObjectItem(json, "name");
    cJSON *email = cJSON_GetObjectItem(json, "email");
    
    if (cJSON_IsString(name)) {
        strncpy(existing_user->name, name->valuestring, sizeof(existing_user->name) - 1);
        existing_user->name[sizeof(existing_user->name) - 1] = '\0';
    }
    if (cJSON_IsString(email)) {
        strncpy(existing_user->email, email->valuestring, sizeof(existing_user->email) - 1);
        existing_user->email[sizeof(existing_user->email) - 1] = '\0';
    }
    
    cJSON *user_json = user_to_json(existing_user);
    send_json_response(c, 200, user_json);
    cJSON_Delete(user_json);
    cJSON_Delete(json);
}

static void handle_delete_user(struct mg_connection *c, int user_id) {
    if (!delete_user(user_id)) {
        cJSON *error = cJSON_CreateObject();
        cJSON_AddStringToObject(error, "error", "User not found");
        send_json_response(c, 404, error);
        cJSON_Delete(error);
        return;
    }
    
    cJSON *success = cJSON_CreateObject();
    cJSON_AddStringToObject(success, "message", "User deleted successfully");
    send_json_response(c, 200, success);
    cJSON_Delete(success);
}

static void handle_swagger_ui(struct mg_connection *c) {
    char *html = get_swagger_ui();
    send_text_response(c, 200, "text/html", html);
}

static void handle_swagger_json(struct mg_connection *c) {
    char *json = get_swagger_json();
    send_text_response(c, 200, "application/json", json);
    free(json);
}

void handle_mongoose_request(struct mg_connection *c, int ev, void *ev_data) {
    if (ev == MG_EV_HTTP_MSG) {
        struct mg_http_message *hm = (struct mg_http_message *) ev_data;
        
        // Log incoming request
        printf("Incoming HTTP request received\n");
        fflush(stdout);
        
        // Handle CORS preflight
        if (mg_strcmp(hm->method, mg_str("OPTIONS")) == 0) {
            printf("Handling OPTIONS request\n");
            fflush(stdout);
            mg_printf(c, "HTTP/1.1 200 OK\r\n"
                        "Access-Control-Allow-Origin: *\r\n"
                        "Access-Control-Allow-Methods: GET, POST, PUT, DELETE, OPTIONS\r\n"
                        "Access-Control-Allow-Headers: Content-Type, Authorization, X-Requested-With, Accept, Origin\r\n"
                        "Access-Control-Max-Age: 86400\r\n"
                        "Content-Length: 0\r\n\r\n");
            return;
        }
        
        // Swagger UI
        if (mg_match(hm->uri, mg_str("/swagger"), NULL) || 
            mg_match(hm->uri, mg_str("/"), NULL)) {
            printf("Serving Swagger UI\n");
            fflush(stdout);
            handle_swagger_ui(c);
            return;
        }
        
        // Simple test page
        if (mg_match(hm->uri, mg_str("/test"), NULL)) {
            printf("Serving test page\n");
            fflush(stdout);
            const char* test_html = 
                "<!DOCTYPE html><html><head><title>API Test</title></head><body>"
                "<h1>API Test Page</h1>"
                "<button id=\"testBtn\" onclick=\"testAPI()\">Test GET /users</button>"
                "<button id=\"simpleBtn\" onclick=\"window.location.href='/users'\">Direct Link Test</button>"
                "<div id=\"result\">Click the button to test the API</div>"
                "<script>"
                "console.log('Test page loaded');"
                "function testAPI() {"
                "  console.log('Button clicked - Testing API...');"
                "  document.getElementById('result').innerHTML = 'Making request to /users...';"
                "  "
                "  var xhr = new XMLHttpRequest();"
                "  xhr.open('GET', '/users', true);"
                "  xhr.setRequestHeader('Accept', 'application/json');"
                "  xhr.timeout = 10000; // 10 second timeout"
                "  "
                "  xhr.onloadstart = function() {"
                "    console.log('Request started');"
                "    document.getElementById('result').innerHTML = 'Request started...';"
                "  };"
                "  "
                "  xhr.onload = function() {"
                "    console.log('Response received:', xhr.status, xhr.statusText);"
                "    document.getElementById('result').innerHTML = 'Got response: ' + xhr.status + ' ' + xhr.statusText + '<br>Data:<br><pre>' + xhr.responseText + '</pre>';"
                "  };"
                "  "
                "  xhr.onerror = function() {"
                "    console.error('Network error');"
                "    document.getElementById('result').innerHTML = 'Network error occurred';"
                "  };"
                "  "
                "  xhr.ontimeout = function() {"
                "    console.error('Request timed out');"
                "    document.getElementById('result').innerHTML = 'Request timed out after 10 seconds';"
                "  };"
                "  "
                "  xhr.send();"
                "}"
                "</script></body></html>";
            send_text_response(c, 200, "text/html", test_html);
            return;
        }
        
        // Swagger JSON
        if (mg_match(hm->uri, mg_str("/swagger.json"), NULL)) {
            handle_swagger_json(c);
            return;
        }
        
        // Users endpoints
        struct mg_str caps[3];
        
        if (mg_match(hm->uri, mg_str("/users"), NULL)) {
            if (mg_strcmp(hm->method, mg_str("GET")) == 0) {
                handle_get_users(c);
            } else if (mg_strcmp(hm->method, mg_str("POST")) == 0) {
                handle_create_user(c, hm->body.buf);
            } else {
                mg_http_reply(c, 405, "", "Method not allowed");
            }
            return;
        }
        
        if (mg_match(hm->uri, mg_str("/users/#"), caps)) {
            int user_id = atoi(caps[0].buf);
            if (mg_strcmp(hm->method, mg_str("GET")) == 0) {
                handle_get_user(c, user_id);
            } else if (mg_strcmp(hm->method, mg_str("PUT")) == 0) {
                handle_update_user(c, user_id, hm->body.buf);
            } else if (mg_strcmp(hm->method, mg_str("DELETE")) == 0) {
                handle_delete_user(c, user_id);
            } else {
                mg_http_reply(c, 405, "", "Method not allowed");
            }
            return;
        }
        
        // 404 for unknown routes
        mg_http_reply(c, 404, "", "Not found");
    }
}
