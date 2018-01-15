#pragma once

#include "mosquittomainloop/mosquittomainloop.h"

#define TOPICROOT			"gwctrl"
#define SUBTOPIC_HEARTBEAT	"heartbeat"

struct context {
	struct mosquitto_context mosqcntx;
	gchar* gwid;
};
