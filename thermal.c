#include "thermal.h"

#include "json-glib-macros/jsonbuilderutils.h"

#define W1DEVICESDIR "/sys/bus/w1/devices"
#define W1THERMPATTERN "28-[0-9a-f]{12}"
#define W1SLAVE "w1_slave"

#define W1TEMPPATTERN "t=([0-9]{3,6})"

static GRegex* tempregex;

void thermal_init(struct thermal* t) {
	tempregex = g_regex_new(W1TEMPPATTERN, 0, 0, NULL);

	GDir* w1devices = g_dir_open(W1DEVICESDIR, 0, NULL);
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

static gboolean readsensor(const gchar* sensor, int* reading) {
	gboolean ret = FALSE;
	GString* pathstr = g_string_new(NULL);
	g_string_printf(pathstr, "%s/%s/%s", W1DEVICESDIR, sensor, W1SLAVE);
	gchar* path = g_string_free(pathstr, FALSE);
	gchar* data;
	if (g_file_get_contents(path, &data, NULL, NULL)) {
		GMatchInfo* matches;
		g_regex_match(tempregex, data, 0, &matches);
		while (g_match_info_matches(matches)) {
			gchar* temp = g_match_info_fetch(matches, 1);
			*reading = g_ascii_strtoll(temp, NULL, 10);
			ret = TRUE;
			g_free(temp);
			g_match_info_next(matches, NULL);
		}
		g_match_info_free(matches);
	} else
		g_message("failed to read sensor %s", sensor);
	g_free(path);
	return ret;
}

static void sensorfunc(gpointer data, gpointer user_data) {
	struct sensor* s = data;

	int reading;
	if (readsensor(s->name, &reading)) {
		s->millidegrees = reading;
	}

	JsonBuilder* jsonbuilder = user_data;
	json_builder_begin_object(jsonbuilder);
	JSONBUILDER_ADD_STRING(jsonbuilder, "name", s->name);
	JSONBUILDER_ADD_INT(jsonbuilder, "millidegrees", s->millidegrees);
	json_builder_end_object(jsonbuilder);

}

void thermal_heartbeat(struct thermal* t, JsonBuilder* jsonbuilder) {
	JSONBUILDER_START_OBJECT(jsonbuilder, "thermal");
	JSONBUILDER_START_ARRAY(jsonbuilder, "sensors");
	g_slist_foreach(t->sensors, sensorfunc, jsonbuilder);
	json_builder_end_array(jsonbuilder);
	json_builder_end_object(jsonbuilder);
}
