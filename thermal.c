#include "thermal.h"

#include "json-glib-macros/jsonbuilderutils.h"

#define W1DEVICESDIR "/sys/bus/w1/devices/"
#define W1THERMPATTERN "28-[0-9a-f]{12}"

void thermal_init(struct thermal* t) {
	GDir* w1devices = g_dir_open("/sys/bus/w1/devices/", 0, NULL);
	if (w1devices != NULL) {
		GRegex* thermregex = g_regex_new(W1THERMPATTERN, 0, 0, NULL);
		const gchar* node;
		while ((node = g_dir_read_name(w1devices)) != NULL) {
			g_message("checking %s for w1 thermal sensor", node);
			if (g_regex_match(thermregex, node, 0, NULL)) {
				g_message("found sensor at %s", node);
				struct sensor* s = g_malloc(sizeof(*s));
				s->name = g_strdup(node);
				s->millidegrees = 0;
				t->sensors = g_slist_append(t->sensors, s);
			}
		}
		g_dir_close(w1devices);
		g_regex_unref(thermregex);

	} else
		g_message("couldn't open w1 devices sysfs directory, w1 not enabled?");
}

void sensorfunc(gpointer data, gpointer user_data) {
	struct sensor* s = data;
	JsonBuilder* jsonbuilder = user_data;
	json_builder_begin_object(jsonbuilder);
	JSONBUILDER_ADD_STRING(jsonbuilder, "name", s->name);
	json_builder_end_object(jsonbuilder);

}

void thermal_heartbeat(struct thermal* t, JsonBuilder* jsonbuilder) {
	JSONBUILDER_START_OBJECT(jsonbuilder, "thermal");
	JSONBUILDER_START_ARRAY(jsonbuilder, "sensors");
	g_slist_foreach(t->sensors, sensorfunc, jsonbuilder);
	json_builder_end_array(jsonbuilder);
	json_builder_end_object(jsonbuilder);
}
