#include "ctrl.h"
#include "gwctrl.h"

void ctrl_onconnected(const gchar* gwid, MosquittoClient* client) {
	GString* topicstr = g_string_new(TOPICROOT);
	g_string_append(topicstr, "/");
	g_string_append(topicstr, gwid);
	g_string_append(topicstr, "/");
	g_string_append(topicstr, SUBTOPIC_CTRL);
	gchar* topic = g_string_free(topicstr, FALSE);
	mosquitto_subscribe(mosquitto_client_getmosquittoinstance(client),
	NULL, topic, 0);
	g_free(topic);
}
