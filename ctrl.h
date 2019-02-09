#pragma once

#include <mosquitto_client.h>

void ctrl_onconnected(const gchar* gwid, MosquittoClient* client);
void ctrl_onmessage(MosquittoClient* client,
		const struct mosquitto_message* msg);
