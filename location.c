#include <glib.h>
#include <gps.h>
#include <errno.h>
#include <unistd.h>

#include "location.h"

static void location_gps_hook(struct gps_data_t *gpsdata) {
	g_message("have gpsd data");
}

static gpointer location_gps_threadfunc(gpointer data) {
	struct gps_data_t gps_data;

	int ret;
	struct gps_data_t gpsdata;
	connect: do {
		ret = gps_open(GPSD_SHARED_MEMORY, 0, &gpsdata);
		if (ret == -1) {
			g_message("failed to connect to gpsd: %s", gps_errstr(errno));
			sleep(60);
		}
	} while (ret != 0);
	g_message("connected to gpsd");

	do {
		ret = gps_mainloop(&gpsdata, 60 * 1000000, location_gps_hook);
		if (ret == -1 && errno != 0) { // apparently errno isn't set for a timeout?
			g_message("gpsd mainloop error (%d): %s", errno, gps_errstr(errno));
			break;
		}
	} while (ret == 0);

	gps_close(&gpsdata);
	goto connect;

	return NULL;
}

void location_init(struct context* cntx) {
	g_thread_new("location_gps", location_gps_threadfunc, cntx);
}
