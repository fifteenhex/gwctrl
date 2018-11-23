#pragma once

#include <glib.h>
#include <json-glib/json-glib.h>

struct location {
	gboolean valid;
	double lat, lon;
	guint64 timestamp;
};

void location_init(struct location* location);
void location_heartbeat(struct location* location, JsonBuilder* jsonbuilder);
