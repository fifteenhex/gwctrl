#define GETTEXT_PACKAGE "gtk20"

#include <glib.h>
#include <json-glib/json-glib.h>
#include <mosquitto.h>

#include "gwctrl.h"

static gboolean heartbeat(gpointer data) {

	struct context* cntx = data;

	if (cntx->mosqcntx.connected) {
		JsonBuilder* jsonbuilder = json_builder_new_immutable();
		json_builder_begin_object(jsonbuilder);

		json_builder_end_object(jsonbuilder);

		JsonNode* root = json_builder_get_root(jsonbuilder);
		JsonGenerator* generator = json_generator_new();
		json_generator_set_root(generator, root);

		gsize jsonlen;
		gchar* json = json_generator_to_data(generator, &jsonlen);
		json_node_free(root);
		g_object_unref(generator);

		GString* topicstr = g_string_new(TOPICROOT);
		g_string_append(topicstr, "/");
		g_string_append(topicstr, cntx->gwid);
		g_string_append(topicstr, "/");
		g_string_append(topicstr, SUBTOPIC_HEARTBEAT);

		gchar* topic = g_string_free(topicstr, FALSE);

		mosquitto_publish(cntx->mosqcntx.mosq, NULL, topic, jsonlen, json, 0,
		FALSE);
	}

	return TRUE;
}

int main(int argc, char** argv) {

	int ret = 0;
	struct context cntx = { 0 };

	gchar* gwid = NULL;
	gchar* mqtthost = "localhost";
	gint mqttport = 1883;

	GOptionEntry entries[] = {
	MQTTOPTS, { "gatewayid", 'i', 0, G_OPTION_ARG_STRING, &gwid, "", "" }, {
	NULL } };

	GOptionContext* context = g_option_context_new("");
	GError* error = NULL;
	g_option_context_add_main_entries(context, entries, GETTEXT_PACKAGE);
	if (!g_option_context_parse(context, &argc, &argv, &error)) {
		g_print("option parsing failed: %s\n", error->message);
	}

	if (gwid == NULL) {
		g_message("You must provide a gateway id");
		ret = 1;
		goto out;
	} else
		cntx.gwid = gwid;

	mosquittomainloop(&cntx.mosqcntx, mqtthost, mqttport, TRUE, NULL, NULL);

	g_timeout_add(30 * 1000, heartbeat, &cntx);

	GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	out: return ret;
}
