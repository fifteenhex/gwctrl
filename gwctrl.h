#pragma once

#include <mosquittomainloop.h>
#include "thermal.h"

#define TOPICROOT			"gwctrl"
#define SUBTOPIC_HEARTBEAT	"heartbeat"

struct location {
	gboolean valid;
	double lat, lon;
	guint64 timestamp;
};

struct context {
	MosquittoClient* mosqclient;
	gchar* gwid;
	struct location location;
	struct thermal thermal;
};
