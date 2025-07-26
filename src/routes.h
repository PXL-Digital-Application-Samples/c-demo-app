#ifndef ROUTES_H
#define ROUTES_H

#include "mongoose.h"

// Main request handler for mongoose
void handle_mongoose_request(struct mg_connection *c, int ev, void *ev_data);

#endif // ROUTES_H