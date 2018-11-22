#pragma once

#include <glib.h>
#include <json-glib/json-glib.h>

struct sensor {
	gchar* name;
	guint32 millidegrees;
};

struct thermal {
	GSList* sensors;
};

void thermal_init(struct thermal* t);
void thermal_heartbeat(struct thermal* t, JsonBuilder* jsonbuilder);
