#include <sys/reboot.h>

#include "ctrl.h"

void ctrl_onconnected(const gchar* topicroot, const gchar* gwid,
		MosquittoClient* client) {
	GString* topicstr = g_string_new(topicroot);
	g_string_append(topicstr, "/");
	g_string_append(topicstr, gwid);
	g_string_append(topicstr, "/");
	g_string_append(topicstr, SUBTOPIC_CTRL);
	g_string_append(topicstr, "/#");
	gchar* topic = g_string_free(topicstr, FALSE);
	mosquitto_subscribe(mosquitto_client_getmosquittoinstance(client),
	NULL, topic, 0);
	g_free(topic);
}

void ctrl_onmessage(MosquittoClient* client,
		const struct mosquitto_message* msg) {
	char** splittopic;
	int count;
	mosquitto_sub_topic_tokenise(msg->topic, &splittopic, &count);

	if (strcmp(splittopic[count - 2], SUBTOPIC_CTRL) == 0) {
		if (strcmp(splittopic[count - 1], "reboot") == 0) {
			g_message("reboot requested");
			reboot(RB_AUTOBOOT);
		}
	}

	mosquitto_sub_topic_tokens_free(&splittopic, count);
}
