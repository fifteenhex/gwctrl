#pragma once

#include "mosquittomainloop/mosquittomainloop.h"

#define TOPICROOT			"gwctrl"
#define SUBTOPIC_HEARTBEAT	"heartbeat"

struct location {
	gboolean valid;
	double lat, lon;
	guint64 timestamp;
};

struct context {
	struct mosquitto_context mosqcntx;
	gchar* gwid;
	struct location location;
};
