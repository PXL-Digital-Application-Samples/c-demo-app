cmake_minimum_required(VERSION 3.16)
project(UserManagementAPI C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find packages
find_package(Threads REQUIRED)

# Windows-specific settings
if(WIN32)
    # Use static runtime on Windows for easier deployment
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    # Windows sockets
    set(ADDITIONAL_LIBS ws2_32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(ADDITIONAL_LIBS "")
endif()

# FetchContent for dependencies
include(FetchContent)

# Fetch cJSON
FetchContent_Declare(
    cjson
    GIT_REPOSITORY https://github.com/DaveGamble/cJSON.git
    GIT_TAG v1.7.15
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)

# Fetch mongoose - lightweight HTTP server for C
FetchContent_Declare(
    mongoose
    GIT_REPOSITORY https://github.com/cesanta/mongoose.git
    GIT_TAG 7.14
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
)

FetchContent_GetProperties(cjson)
if(NOT cjson_POPULATED)
    FetchContent_Populate(cjson)
endif()

FetchContent_GetProperties(mongoose)
if(NOT mongoose_POPULATED)
    FetchContent_Populate(mongoose)
endif()

# Main executable
add_executable(user_api
    src/main.c
    src/users.c
    src/routes.c
    src/swagger.c
    ${cjson_SOURCE_DIR}/cJSON.c
    ${mongoose_SOURCE_DIR}/mongoose.c
)

# Include cJSON headers
target_include_directories(user_api PRIVATE ${cjson_SOURCE_DIR})

# Create a directory alias for cjson include path
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include/cjson)
file(CREATE_LINK ${cjson_SOURCE_DIR}/cJSON.h ${CMAKE_BINARY_DIR}/include/cjson/cJSON.h COPY_ON_ERROR)
target_include_directories(user_api PRIVATE ${CMAKE_BINARY_DIR}/include)

# Include mongoose
target_include_directories(user_api PRIVATE ${mongoose_SOURCE_DIR})
target_compile_definitions(user_api PRIVATE MG_ENABLE_THREADS=1)

# Link libraries
target_link_libraries(user_api 
    PRIVATE 
    Threads::Threads
    ${ADDITIONAL_LIBS}
)

# Define that we have HTTP server capability with mongoose
target_compile_definitions(user_api PRIVATE HAVE_MONGOOSE USE_MONGOOSE)

# Tests
add_executable(test_users tests/test_users.c src/users.c ${cjson_SOURCE_DIR}/cJSON.c)
add_executable(test_routes tests/test_routes.c src/users.c src/routes.c src/swagger.c ${cjson_SOURCE_DIR}/cJSON.c ${mongoose_SOURCE_DIR}/mongoose.c)
add_executable(test_basic test_basic.c)

# Add include directories for tests
target_include_directories(test_users PRIVATE src tests ${CMAKE_BINARY_DIR}/include)
target_include_directories(test_routes PRIVATE src tests ${CMAKE_BINARY_DIR}/include ${mongoose_SOURCE_DIR})
target_include_directories(test_basic PRIVATE tests)

# Add compile definitions for tests
target_compile_definitions(test_routes PRIVATE MG_ENABLE_THREADS=1 HAVE_MONGOOSE USE_MONGOOSE)

# Link libraries for tests
target_link_libraries(test_users 
    PRIVATE
    Threads::Threads
    ${ADDITIONAL_LIBS}
)

target_link_libraries(test_routes 
    PRIVATE
    Threads::Threads
    ${ADDITIONAL_LIBS}
)

# Enable testing
enable_testing()
add_test(NAME unit_tests COMMAND test_users)
add_test(NAME integration_tests COMMAND test_routes)