#pragma once

#include <mosquittomainloop.h>
#include "location.h"
#include "thermal.h"

#define TOPICROOT			"gwctrl"
#define SUBTOPIC_HEARTBEAT	"heartbeat"

struct context {
	MosquittoClient* mosqclient;
	gchar* gwid;
	struct location location;
	struct thermal thermal;
};
