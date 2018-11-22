#define GETTEXT_PACKAGE "gtk20"

#include <glib.h>
#include <json-glib/json-glib.h>
#include <mosquitto.h>

#include "gwctrl.h"
#include "location.h"
#include "thermal.h"
#include "json-glib-macros/jsonbuilderutils.h"

static gboolean heartbeat(gpointer data) {

	struct context* cntx = data;

	if (mosquitto_client_isconnected(cntx->mosqclient)) {
		JsonBuilder* jsonbuilder = json_builder_new_immutable();
		json_builder_begin_object(jsonbuilder);

		if (cntx->location.valid) {
			JSONBUILDER_START_OBJECT(jsonbuilder, "location");
			json_builder_set_member_name(jsonbuilder, "lat");
			json_builder_add_double_value(jsonbuilder, cntx->location.lat);
			json_builder_set_member_name(jsonbuilder, "lon");
			json_builder_add_double_value(jsonbuilder, cntx->location.lon);
			json_builder_end_object(jsonbuilder);
		}

		thermal_heartbeat(&cntx->thermal, jsonbuilder);

		json_builder_end_object(jsonbuilder);

		gsize jsonlen;
		gchar* json = jsonbuilder_freetostring(jsonbuilder, &jsonlen, TRUE);

		GString* topicstr = g_string_new(TOPICROOT);
		g_string_append(topicstr, "/");
		g_string_append(topicstr, cntx->gwid);
		g_string_append(topicstr, "/");
		g_string_append(topicstr, SUBTOPIC_HEARTBEAT);

		gchar* topic = g_string_free(topicstr, FALSE);

		mosquitto_publish(
				mosquitto_client_getmosquittoinstance(cntx->mosqclient), NULL,
				topic, jsonlen, json, 0, FALSE);

		g_free(topic);
		g_free(json);
	}

	return TRUE;
}

static gboolean connectedcallback(MosquittoClient* client, void* something,
		gpointer user_data) {
	return heartbeat(user_data);
}

int main(int argc, char** argv) {

	int ret = 0;
	struct context cntx = { 0 };

	gchar* gwid = NULL;
	gchar* mqttid = NULL;
	gchar* mqtthost = "localhost";
	gint mqttport = 1883;
	gchar* mqttrootcert = NULL;
	gchar* mqttdevicecert = NULL;
	gchar* mqttdevicekey = NULL;

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

	cntx.mosqclient = mosquitto_client_new_plaintext(mqttid, mqtthost,
			mqttport);

	g_timeout_add(30 * 1000, heartbeat, &cntx);

	g_signal_connect(cntx.mosqclient, MOSQUITTO_CLIENT_SIGNAL_CONNECTED,
			(GCallback )connectedcallback, &cntx);

	location_init(&cntx);
	thermal_init(&cntx.thermal);

	GMainLoop* mainloop = g_main_loop_new(NULL, FALSE);
	g_main_loop_run(mainloop);

	out: return ret;
}
