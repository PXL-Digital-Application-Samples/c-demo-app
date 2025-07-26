# 🧩 User Management API (C Implementation)

A lightweight C-based REST API for managing users with in-memory storage, thread-safe operations, and Swagger UI documentation.

## 📦 Features

- ✅ Full CRUD operations (Create, Read, Update, Delete)
- 🔄 Thread-safe in-memory storage
- 🧪 Interactive Swagger UI at `/`
- 🚀 High-performance mongoose HTTP server
- ✔️ Comprehensive unit tests with Unity framework
- 📊 JSON API using cJSON library
- 🌱 Auto-seeded with 3 initial users
- 🌐 CORS support for browser compatibility

## 🛠️ Dependencies

This project uses **CMake with FetchContent** to automatically download and build dependencies:

- **mongoose** - High-performance HTTP server library
- **cJSON** - JSON parsing and generation
- **Unity** - Unit testing framework (included)

No manual dependency installation required!

## 🏗️ Building and Running

### Prerequisites

- **CMake 3.16+**
- **C11 compatible compiler** (GCC, Clang, MSVC)

### CMake Build Linux/Windows

```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

## 🚀 Running the Server

After building, start the server:

**Linux:**

```bash
./build/user_api
```

**Windows:**

```powershell
.\build\Release\user_api.exe
```

The server will start on `http://localhost:5000` and display:

```text
Server running on http://0.0.0.0:5000
Swagger UI available at http://localhost:5000/
Press Ctrl+C to stop...
```

## 📡 API Usage

The server runs on `http://localhost:5000` by default (set `PORT` env var to change).

### Endpoints

**Get all users:**

```bash
curl http://localhost:5000/users
```

**Get user by ID:**

```bash
curl http://localhost:5000/users/1
```

**Create user:**

```bash
curl -X POST http://localhost:5000/users \
  -H "Content-Type: application/json" \
  -d '{"name":"John Doe","email":"john@example.com"}'
```

**Update user:**

```bash
curl -X PUT http://localhost:5000/users/1 \
  -H "Content-Type: application/json" \
  -d '{"name":"Jane Doe","email":"jane@example.com"}'
```

**Delete user:**

```bash
curl -X DELETE http://localhost:5000/users/1
```

### Swagger UI

Open `http://localhost:5000/` in your browser for interactive API documentation with working Execute buttons.

## 🏗️ Project Structure

```text
├── src/
│   ├── main.c          # Entry point with mongoose server setup
│   ├── users.c/.h      # User management logic
│   ├── routes.c/.h     # HTTP request routing and CORS handling
│   └── swagger.c/.h    # OpenAPI documentation with inline spec
├── tests/
│   ├── test_users.c    # User management unit tests
│   ├── test_routes.c   # Route handling unit tests
│   └── unity.h         # Unity test framework
├── CMakeLists.txt      # CMake configuration with FetchContent
├── LICENSE            # Project license
└── README.md          # This file
```

## 🧪 Testing

The project uses Unity, a lightweight C testing framework. Tests cover:

- User creation, retrieval, update, deletion
- Thread safety
- JSON serialization
- Initial data seeding
- HTTP routing and CORS handling

Run tests with CMake:

```bash
# Build and run tests
cmake --build . --target test_users
cmake --build . --target test_routes

# Execute the test binaries
./test_users
./test_routes
```
