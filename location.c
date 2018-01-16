#include <glib.h>
#include <gps.h>
#include <errno.h>
#include <unistd.h>

#include "location.h"

static struct location* location; //ugh!

static void location_gps_hook(struct gps_data_t *gpsdata) {
	g_message("have gpsd data");
	if (gpsdata->status == STATUS_FIX && gpsdata->fix.mode >= MODE_2D) {
		g_message("gps data lat %f lon %f", gpsdata->fix.latitude,
				gpsdata->fix.longitude);
		location->lat = gpsdata->fix.latitude;
		location->lon = gpsdata->fix.longitude;
		location->valid = TRUE;
	}
}

static gpointer location_gps_threadfunc(gpointer data) {

	struct context* cntx = data;
	location = &cntx->location;

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
		ret = 0; // ret could be -1 even though there wasn't an error
	} while (ret == 0);

	gps_close(&gpsdata);
	goto connect;

	return NULL;
}

void location_init(struct context* cntx) {
	g_thread_new("location_gps", location_gps_threadfunc, cntx);
}
