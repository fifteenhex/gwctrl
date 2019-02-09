#pragma once

#include <nodectrl/nodectrl.h>
#include <mosquitto_client.h>

void ctrl_onconnected(const gchar* topicroot, const gchar* gwid,
		MosquittoClient* client);
void ctrl_onmessage(MosquittoClient* client,
		const struct mosquitto_message* msg);
