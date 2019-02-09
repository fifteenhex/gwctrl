#pragma once

#include <mosquitto_client.h>
#include "location.h"
#include "thermal.h"

#define TOPICROOT			"gwctrl"
#define SUBTOPIC_HEARTBEAT	"heartbeat"
#define SUBTOPIC_CTRL		"ctrl"

struct context {
	MosquittoClient* mosqclient;
	gchar* gwid;
	struct location location;
	struct thermal thermal;
};
