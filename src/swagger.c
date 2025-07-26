#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cjson/cJSON.h"
#include "swagger.h"

char* get_swagger_ui(void) {
    static const char *html = 
        "<!DOCTYPE html>\n"
        "<html>\n"
        "<head>\n"
        "    <title>User Management API</title>\n"
        "    <link rel=\"stylesheet\" type=\"text/css\" href=\"https://unpkg.com/swagger-ui-dist@5.9.0/swagger-ui.css\" />\n"
        "    <style>\n"
        "        .swagger-ui .topbar { display: none; }\n"
        "        body { margin: 0; padding: 20px; }\n"
        "    </style>\n"
        "</head>\n"
        "<body>\n"
        "    <div id=\"swagger-ui\"></div>\n"
        "    <script src=\"https://unpkg.com/swagger-ui-dist@5.9.0/swagger-ui-bundle.js\"></script>\n"
        "    <script>\n"
        "        console.log('Starting Swagger UI with inline spec...');\n"
        "        \n"
        "        // Inline API specification to avoid CORS issues\n"
        "        const apiSpec = {\n"
        "            \"openapi\": \"3.0.0\",\n"
        "            \"info\": {\n"
        "                \"title\": \"User Management API\",\n"
        "                \"version\": \"1.0.0\",\n"
        "                \"description\": \"A simple API for managing users\"\n"
        "            },\n"
        "            \"servers\": [\n"
        "                {\n"
        "                    \"url\": \"http://localhost:5000\",\n"
        "                    \"description\": \"Development server\"\n"
        "                }\n"
        "            ],\n"
        "            \"paths\": {\n"
        "                \"/users\": {\n"
        "                    \"get\": {\n"
        "                        \"summary\": \"Get all users\",\n"
        "                        \"description\": \"Retrieve a list of all users\",\n"
        "                        \"responses\": {\n"
        "                            \"200\": {\n"
        "                                \"description\": \"List of users\",\n"
        "                                \"content\": {\n"
        "                                    \"application/json\": {\n"
        "                                        \"schema\": {\n"
        "                                            \"type\": \"array\",\n"
        "                                            \"items\": {\n"
        "                                                \"$ref\": \"#/components/schemas/User\"\n"
        "                                            }\n"
        "                                        }\n"
        "                                    }\n"
        "                                }\n"
        "                            }\n"
        "                        }\n"
        "                    },\n"
        "                    \"post\": {\n"
        "                        \"summary\": \"Create a new user\",\n"
        "                        \"description\": \"Add a new user to the system\",\n"
        "                        \"requestBody\": {\n"
        "                            \"required\": true,\n"
        "                            \"content\": {\n"
        "                                \"application/json\": {\n"
        "                                    \"schema\": {\n"
        "                                        \"$ref\": \"#/components/schemas/UserInput\"\n"
        "                                    }\n"
        "                                }\n"
        "                            }\n"
        "                        },\n"
        "                        \"responses\": {\n"
        "                            \"201\": {\n"
        "                                \"description\": \"User created successfully\",\n"
        "                                \"content\": {\n"
        "                                    \"application/json\": {\n"
        "                                        \"schema\": {\n"
        "                                            \"$ref\": \"#/components/schemas/User\"\n"
        "                                        }\n"
        "                                    }\n"
        "                                }\n"
        "                            },\n"
        "                            \"400\": {\n"
        "                                \"description\": \"Invalid input\"\n"
        "                            }\n"
        "                        }\n"
        "                    }\n"
        "                },\n"
        "                \"/users/{id}\": {\n"
        "                    \"get\": {\n"
        "                        \"summary\": \"Get user by ID\",\n"
        "                        \"description\": \"Retrieve a specific user by their ID\",\n"
        "                        \"parameters\": [\n"
        "                            {\n"
        "                                \"name\": \"id\",\n"
        "                                \"in\": \"path\",\n"
        "                                \"required\": true,\n"
        "                                \"description\": \"User ID\",\n"
        "                                \"schema\": {\n"
        "                                    \"type\": \"integer\"\n"
        "                                }\n"
        "                            }\n"
        "                        ],\n"
        "                        \"responses\": {\n"
        "                            \"200\": {\n"
        "                                \"description\": \"User found\",\n"
        "                                \"content\": {\n"
        "                                    \"application/json\": {\n"
        "                                        \"schema\": {\n"
        "                                            \"$ref\": \"#/components/schemas/User\"\n"
        "                                        }\n"
        "                                    }\n"
        "                                }\n"
        "                            },\n"
        "                            \"404\": {\n"
        "                                \"description\": \"User not found\"\n"
        "                            }\n"
        "                        }\n"
        "                    },\n"
        "                    \"put\": {\n"
        "                        \"summary\": \"Update user\",\n"
        "                        \"description\": \"Update an existing user\",\n"
        "                        \"parameters\": [\n"
        "                            {\n"
        "                                \"name\": \"id\",\n"
        "                                \"in\": \"path\",\n"
        "                                \"required\": true,\n"
        "                                \"description\": \"User ID\",\n"
        "                                \"schema\": {\n"
        "                                    \"type\": \"integer\"\n"
        "                                }\n"
        "                            }\n"
        "                        ],\n"
        "                        \"requestBody\": {\n"
        "                            \"required\": true,\n"
        "                            \"content\": {\n"
        "                                \"application/json\": {\n"
        "                                    \"schema\": {\n"
        "                                        \"$ref\": \"#/components/schemas/UserInput\"\n"
        "                                    }\n"
        "                                }\n"
        "                            }\n"
        "                        },\n"
        "                        \"responses\": {\n"
        "                            \"200\": {\n"
        "                                \"description\": \"User updated successfully\",\n"
        "                                \"content\": {\n"
        "                                    \"application/json\": {\n"
        "                                        \"schema\": {\n"
        "                                            \"$ref\": \"#/components/schemas/User\"\n"
        "                                        }\n"
        "                                    }\n"
        "                                }\n"
        "                            },\n"
        "                            \"404\": {\n"
        "                                \"description\": \"User not found\"\n"
        "                            },\n"
        "                            \"400\": {\n"
        "                                \"description\": \"Invalid input\"\n"
        "                            }\n"
        "                        }\n"
        "                    },\n"
        "                    \"delete\": {\n"
        "                        \"summary\": \"Delete user\",\n"
        "                        \"description\": \"Remove a user from the system\",\n"
        "                        \"parameters\": [\n"
        "                            {\n"
        "                                \"name\": \"id\",\n"
        "                                \"in\": \"path\",\n"
        "                                \"required\": true,\n"
        "                                \"description\": \"User ID\",\n"
        "                                \"schema\": {\n"
        "                                    \"type\": \"integer\"\n"
        "                                }\n"
        "                            }\n"
        "                        ],\n"
        "                        \"responses\": {\n"
        "                            \"200\": {\n"
        "                                \"description\": \"User deleted successfully\"\n"
        "                            },\n"
        "                            \"404\": {\n"
        "                                \"description\": \"User not found\"\n"
        "                            }\n"
        "                        }\n"
        "                    }\n"
        "                }\n"
        "            },\n"
        "            \"components\": {\n"
        "                \"schemas\": {\n"
        "                    \"User\": {\n"
        "                        \"type\": \"object\",\n"
        "                        \"properties\": {\n"
        "                            \"id\": {\n"
        "                                \"type\": \"integer\",\n"
        "                                \"description\": \"User ID\",\n"
        "                                \"example\": 1\n"
        "                            },\n"
        "                            \"name\": {\n"
        "                                \"type\": \"string\",\n"
        "                                \"description\": \"User name\",\n"
        "                                \"example\": \"John Doe\"\n"
        "                            },\n"
        "                            \"email\": {\n"
        "                                \"type\": \"string\",\n"
        "                                \"format\": \"email\",\n"
        "                                \"description\": \"User email address\",\n"
        "                                \"example\": \"john.doe@example.com\"\n"
        "                            }\n"
        "                        },\n"
        "                        \"required\": [\"id\", \"name\", \"email\"]\n"
        "                    },\n"
        "                    \"UserInput\": {\n"
        "                        \"type\": \"object\",\n"
        "                        \"properties\": {\n"
        "                            \"name\": {\n"
        "                                \"type\": \"string\",\n"
        "                                \"description\": \"User name\",\n"
        "                                \"example\": \"John Doe\"\n"
        "                            },\n"
        "                            \"email\": {\n"
        "                                \"type\": \"string\",\n"
        "                                \"format\": \"email\",\n"
        "                                \"description\": \"User email address\",\n"
        "                                \"example\": \"john.doe@example.com\"\n"
        "                            }\n"
        "                        },\n"
        "                        \"required\": [\"name\", \"email\"]\n"
        "                    }\n"
        "                }\n"
        "            }\n"
        "        };\n"
        "\n"
        "        // Initialize Swagger UI with inline spec\n"
        "        window.onload = function() {\n"
        "            console.log('Initializing Swagger UI with inline specification');\n"
        "            \n"
        "            const ui = SwaggerUIBundle({\n"
        "                spec: apiSpec,\n"
        "                dom_id: '#swagger-ui',\n"
        "                deepLinking: true,\n"
        "                presets: [\n"
        "                    SwaggerUIBundle.presets.apis,\n"
        "                    SwaggerUIBundle.presets.standalone\n"
        "                ],\n"
        "                requestInterceptor: function(request) {\n"
        "                    console.log('Intercepting request:', request.method, request.url);\n"
        "                    console.log('Request headers:', request.headers);\n"
        "                    console.log('Request body:', request.body);\n"
        "                    \n"
        "                    // Ensure proper headers\n"
        "                    request.headers = request.headers || {};\n"
        "                    if (request.method !== 'GET') {\n"
        "                        request.headers['Content-Type'] = 'application/json';\n"
        "                    }\n"
        "                    request.headers['Accept'] = 'application/json';\n"
        "                    \n"
        "                    return request;\n"
        "                },\n"
        "                responseInterceptor: function(response) {\n"
        "                    console.log('Response received:', response.status, response.statusText);\n"
        "                    console.log('Response headers:', response.headers);\n"
        "                    if (response.text) {\n"
        "                        console.log('Response body:', response.text);\n"
        "                    }\n"
        "                    return response;\n"
        "                },\n"
        "                onComplete: function() {\n"
        "                    console.log('Swagger UI loaded successfully!');\n"
        "                },\n"
        "                onFailure: function(error) {\n"
        "                    console.error('Swagger UI failed to load:', error);\n"
        "                }\n"
        "            });\n"
        "            \n"
        "            console.log('Swagger UI initialization complete');\n"
        "        };\n"
        "    </script>\n"
        "</body>\n"
        "</html>";
    
    // Return a copy of the static string
    size_t len = strlen(html);
    char *result = (char*)malloc(len + 1);
    if (result) {
        strcpy(result, html);
    }
    return result;
}

char* get_swagger_json(void) {
    cJSON *root = cJSON_CreateObject();
    cJSON *openapi = cJSON_CreateString("3.0.0");
    cJSON *info = cJSON_CreateObject();
    cJSON *title = cJSON_CreateString("User Management API");
    cJSON *version = cJSON_CreateString("1.0.0");
    cJSON *description = cJSON_CreateString("A simple API for managing users");
    
    // Add info object
    cJSON_AddItemToObject(info, "title", title);
    cJSON_AddItemToObject(info, "version", version);
    cJSON_AddItemToObject(info, "description", description);
    
    // Add root properties
    cJSON_AddItemToObject(root, "openapi", openapi);
    cJSON_AddItemToObject(root, "info", info);
    
    // Create servers array
    cJSON *servers = cJSON_CreateArray();
    cJSON *server = cJSON_CreateObject();
    cJSON *server_url = cJSON_CreateString("http://localhost:5000");
    cJSON_AddItemToObject(server, "url", server_url);
    cJSON_AddItemToArray(servers, server);
    cJSON_AddItemToObject(root, "servers", servers);
    
    // Create paths object
    cJSON *paths = cJSON_CreateObject();
    
    // Users collection endpoint
    cJSON *users_path = cJSON_CreateObject();
    
    // GET /users
    cJSON *get_users = cJSON_CreateObject();
    cJSON *get_summary = cJSON_CreateString("Get all users");
    cJSON *get_responses = cJSON_CreateObject();
    cJSON *get_200 = cJSON_CreateObject();
    cJSON *get_200_desc = cJSON_CreateString("List of users");
    cJSON_AddItemToObject(get_200, "description", get_200_desc);
    cJSON_AddItemToObject(get_responses, "200", get_200);
    cJSON_AddItemToObject(get_users, "summary", get_summary);
    cJSON_AddItemToObject(get_users, "responses", get_responses);
    cJSON_AddItemToObject(users_path, "get", get_users);
    
    // POST /users
    cJSON *post_users = cJSON_CreateObject();
    cJSON *post_summary = cJSON_CreateString("Create a new user");
    cJSON *post_requestBody = cJSON_CreateObject();
    cJSON *post_content = cJSON_CreateObject();
    cJSON *post_json = cJSON_CreateObject();
    cJSON *post_schema = cJSON_CreateObject();
    cJSON *post_schema_type = cJSON_CreateString("object");
    cJSON *post_properties = cJSON_CreateObject();
    
    // Name property
    cJSON *name_prop = cJSON_CreateObject();
    cJSON *name_type = cJSON_CreateString("string");
    cJSON_AddItemToObject(name_prop, "type", name_type);
    cJSON_AddItemToObject(post_properties, "name", name_prop);
    
    // Email property
    cJSON *email_prop = cJSON_CreateObject();
    cJSON *email_type = cJSON_CreateString("string");
    cJSON_AddItemToObject(email_prop, "type", email_type);
    cJSON_AddItemToObject(post_properties, "email", email_prop);
    
    cJSON_AddItemToObject(post_schema, "type", post_schema_type);
    cJSON_AddItemToObject(post_schema, "properties", post_properties);
    cJSON_AddItemToObject(post_json, "schema", post_schema);
    cJSON_AddItemToObject(post_content, "application/json", post_json);
    cJSON_AddItemToObject(post_requestBody, "content", post_content);
    
    cJSON *post_responses = cJSON_CreateObject();
    cJSON *post_201 = cJSON_CreateObject();
    cJSON *post_201_desc = cJSON_CreateString("User created");
    cJSON_AddItemToObject(post_201, "description", post_201_desc);
    cJSON_AddItemToObject(post_responses, "201", post_201);
    
    cJSON_AddItemToObject(post_users, "summary", post_summary);
    cJSON_AddItemToObject(post_users, "requestBody", post_requestBody);
    cJSON_AddItemToObject(post_users, "responses", post_responses);
    cJSON_AddItemToObject(users_path, "post", post_users);
    
    // User by ID endpoint
    cJSON *user_by_id_path = cJSON_CreateObject();
    
    // GET /users/{id}
    cJSON *get_user = cJSON_CreateObject();
    cJSON *get_user_summary = cJSON_CreateString("Get user by ID");
    cJSON *get_user_parameters = cJSON_CreateArray();
    cJSON *id_param = cJSON_CreateObject();
    cJSON *param_name = cJSON_CreateString("id");
    cJSON *param_in = cJSON_CreateString("path");
    cJSON *param_required = cJSON_CreateBool(1);
    cJSON *param_schema = cJSON_CreateObject();
    cJSON *param_type = cJSON_CreateString("integer");
    cJSON_AddItemToObject(param_schema, "type", param_type);
    cJSON_AddItemToObject(id_param, "name", param_name);
    cJSON_AddItemToObject(id_param, "in", param_in);
    cJSON_AddItemToObject(id_param, "required", param_required);
    cJSON_AddItemToObject(id_param, "schema", param_schema);
    cJSON_AddItemToArray(get_user_parameters, id_param);
    
    cJSON *get_user_responses = cJSON_CreateObject();
    cJSON *get_user_200 = cJSON_CreateObject();
    cJSON *get_user_200_desc = cJSON_CreateString("User details");
    cJSON_AddItemToObject(get_user_200, "description", get_user_200_desc);
    cJSON_AddItemToObject(get_user_responses, "200", get_user_200);
    cJSON *get_user_404 = cJSON_CreateObject();
    cJSON *get_user_404_desc = cJSON_CreateString("User not found");
    cJSON_AddItemToObject(get_user_404, "description", get_user_404_desc);
    cJSON_AddItemToObject(get_user_responses, "404", get_user_404);
    
    cJSON_AddItemToObject(get_user, "summary", get_user_summary);
    cJSON_AddItemToObject(get_user, "parameters", get_user_parameters);
    cJSON_AddItemToObject(get_user, "responses", get_user_responses);
    cJSON_AddItemToObject(user_by_id_path, "get", get_user);
    
    // PUT /users/{id}
    cJSON *put_user = cJSON_CreateObject();
    cJSON *put_user_summary = cJSON_CreateString("Update user by ID");
    cJSON *put_user_parameters = cJSON_CreateArray();
    cJSON *put_id_param = cJSON_CreateObject();
    cJSON *put_param_name = cJSON_CreateString("id");
    cJSON *put_param_in = cJSON_CreateString("path");
    cJSON *put_param_required = cJSON_CreateBool(1);
    cJSON *put_param_schema = cJSON_CreateObject();
    cJSON *put_param_type = cJSON_CreateString("integer");
    cJSON_AddItemToObject(put_param_schema, "type", put_param_type);
    cJSON_AddItemToObject(put_id_param, "name", put_param_name);
    cJSON_AddItemToObject(put_id_param, "in", put_param_in);
    cJSON_AddItemToObject(put_id_param, "required", put_param_required);
    cJSON_AddItemToObject(put_id_param, "schema", put_param_schema);
    cJSON_AddItemToArray(put_user_parameters, put_id_param);
    
    cJSON *put_requestBody = cJSON_CreateObject();
    cJSON *put_content = cJSON_CreateObject();
    cJSON *put_json = cJSON_CreateObject();
    cJSON *put_schema = cJSON_CreateObject();
    cJSON *put_schema_type = cJSON_CreateString("object");
    cJSON *put_properties = cJSON_CreateObject();
    
    cJSON *put_name_prop = cJSON_CreateObject();
    cJSON *put_name_type = cJSON_CreateString("string");
    cJSON_AddItemToObject(put_name_prop, "type", put_name_type);
    cJSON_AddItemToObject(put_properties, "name", put_name_prop);
    
    cJSON *put_email_prop = cJSON_CreateObject();
    cJSON *put_email_type = cJSON_CreateString("string");
    cJSON_AddItemToObject(put_email_prop, "type", put_email_type);
    cJSON_AddItemToObject(put_properties, "email", put_email_prop);
    
    cJSON_AddItemToObject(put_schema, "type", put_schema_type);
    cJSON_AddItemToObject(put_schema, "properties", put_properties);
    cJSON_AddItemToObject(put_json, "schema", put_schema);
    cJSON_AddItemToObject(put_content, "application/json", put_json);
    cJSON_AddItemToObject(put_requestBody, "content", put_content);
    
    cJSON *put_user_responses = cJSON_CreateObject();
    cJSON *put_user_200 = cJSON_CreateObject();
    cJSON *put_user_200_desc = cJSON_CreateString("User updated successfully");
    cJSON_AddItemToObject(put_user_200, "description", put_user_200_desc);
    cJSON_AddItemToObject(put_user_responses, "200", put_user_200);
    cJSON *put_user_404 = cJSON_CreateObject();
    cJSON *put_user_404_desc = cJSON_CreateString("User not found");
    cJSON_AddItemToObject(put_user_404, "description", put_user_404_desc);
    cJSON_AddItemToObject(put_user_responses, "404", put_user_404);
    
    cJSON_AddItemToObject(put_user, "summary", put_user_summary);
    cJSON_AddItemToObject(put_user, "parameters", put_user_parameters);
    cJSON_AddItemToObject(put_user, "requestBody", put_requestBody);
    cJSON_AddItemToObject(put_user, "responses", put_user_responses);
    cJSON_AddItemToObject(user_by_id_path, "put", put_user);
    
    // DELETE /users/{id}
    cJSON *delete_user = cJSON_CreateObject();
    cJSON *delete_user_summary = cJSON_CreateString("Delete user by ID");
    cJSON *delete_user_parameters = cJSON_CreateArray();
    cJSON *delete_id_param = cJSON_CreateObject();
    cJSON *delete_param_name = cJSON_CreateString("id");
    cJSON *delete_param_in = cJSON_CreateString("path");
    cJSON *delete_param_required = cJSON_CreateBool(1);
    cJSON *delete_param_schema = cJSON_CreateObject();
    cJSON *delete_param_type = cJSON_CreateString("integer");
    cJSON_AddItemToObject(delete_param_schema, "type", delete_param_type);
    cJSON_AddItemToObject(delete_id_param, "name", delete_param_name);
    cJSON_AddItemToObject(delete_id_param, "in", delete_param_in);
    cJSON_AddItemToObject(delete_id_param, "required", delete_param_required);
    cJSON_AddItemToObject(delete_id_param, "schema", delete_param_schema);
    cJSON_AddItemToArray(delete_user_parameters, delete_id_param);
    
    cJSON *delete_user_responses = cJSON_CreateObject();
    cJSON *delete_user_200 = cJSON_CreateObject();
    cJSON *delete_user_200_desc = cJSON_CreateString("User deleted successfully");
    cJSON_AddItemToObject(delete_user_200, "description", delete_user_200_desc);
    cJSON_AddItemToObject(delete_user_responses, "200", delete_user_200);
    cJSON *delete_user_404 = cJSON_CreateObject();
    cJSON *delete_user_404_desc = cJSON_CreateString("User not found");
    cJSON_AddItemToObject(delete_user_404, "description", delete_user_404_desc);
    cJSON_AddItemToObject(delete_user_responses, "404", delete_user_404);
    
    cJSON_AddItemToObject(delete_user, "summary", delete_user_summary);
    cJSON_AddItemToObject(delete_user, "parameters", delete_user_parameters);
    cJSON_AddItemToObject(delete_user, "responses", delete_user_responses);
    cJSON_AddItemToObject(user_by_id_path, "delete", delete_user);
    
    // Add paths to main object
    cJSON_AddItemToObject(paths, "/users", users_path);
    cJSON_AddItemToObject(paths, "/users/{id}", user_by_id_path);
    cJSON_AddItemToObject(root, "paths", paths);
    
    // Add components section with schemas
    cJSON *components = cJSON_CreateObject();
    cJSON *schemas = cJSON_CreateObject();
    
    // User schema
    cJSON *user_schema = cJSON_CreateObject();
    cJSON *user_schema_type = cJSON_CreateString("object");
    cJSON *user_schema_properties = cJSON_CreateObject();
    
    cJSON *user_id_prop = cJSON_CreateObject();
    cJSON *user_id_type = cJSON_CreateString("integer");
    cJSON *user_id_example = cJSON_CreateNumber(1);
    cJSON_AddItemToObject(user_id_prop, "type", user_id_type);
    cJSON_AddItemToObject(user_id_prop, "example", user_id_example);
    cJSON_AddItemToObject(user_schema_properties, "id", user_id_prop);
    
    cJSON *user_name_prop = cJSON_CreateObject();
    cJSON *user_name_type = cJSON_CreateString("string");
    cJSON *user_name_example = cJSON_CreateString("John Doe");
    cJSON_AddItemToObject(user_name_prop, "type", user_name_type);
    cJSON_AddItemToObject(user_name_prop, "example", user_name_example);
    cJSON_AddItemToObject(user_schema_properties, "name", user_name_prop);
    
    cJSON *user_email_prop = cJSON_CreateObject();
    cJSON *user_email_type = cJSON_CreateString("string");
    cJSON *user_email_format = cJSON_CreateString("email");
    cJSON *user_email_example = cJSON_CreateString("john@example.com");
    cJSON_AddItemToObject(user_email_prop, "type", user_email_type);
    cJSON_AddItemToObject(user_email_prop, "format", user_email_format);
    cJSON_AddItemToObject(user_email_prop, "example", user_email_example);
    cJSON_AddItemToObject(user_schema_properties, "email", user_email_prop);
    
    cJSON_AddItemToObject(user_schema, "type", user_schema_type);
    cJSON_AddItemToObject(user_schema, "properties", user_schema_properties);
    cJSON_AddItemToObject(schemas, "User", user_schema);
    
    cJSON_AddItemToObject(components, "schemas", schemas);
    cJSON_AddItemToObject(root, "components", components);
    
    // Convert to string and cleanup
    char *json_string = cJSON_Print(root);
    cJSON_Delete(root);
    
    return json_string;
}
