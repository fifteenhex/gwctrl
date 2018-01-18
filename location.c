#include <glib.h>
#include <gps.h>
#include <errno.h>
#include <unistd.h>

#include "location.h"

static gpointer location_gps_threadfunc(gpointer data) {

	struct context* cntx = data;

	int ret;
	struct gps_data_t gpsdata;
	connect: do {
		ret = gps_open("localhost", "2947", &gpsdata);
		if (ret == -1) {
			g_message("failed to connect to gpsd: %s", gps_errstr(errno));
			sleep(60);
		}
	} while (ret != 0);
	g_message("connected to gpsd");

	gps_stream(&gpsdata, WATCH_ENABLE | WATCH_JSON, NULL);

	do {
		if (gps_waiting(&gpsdata, 500)) {
			if (gps_read(&gpsdata) == -1) {
				g_message("gpsd mainloop error (%d): %s", errno,
						gps_errstr(errno));
				break;
			} else {
				if (gpsdata.status == STATUS_FIX && gpsdata.fix.mode >= MODE_2D) {
					g_message("gps data lat %f lon %f", gpsdata.fix.latitude,
							gpsdata.fix.longitude);
					cntx->location.lat = gpsdata.fix.latitude;
					cntx->location.lon = gpsdata.fix.longitude;
					cntx->location.timestamp = g_get_monotonic_time();
					cntx->location.valid = TRUE;
				}
			}
		}
	} while (true);

	gps_close(&gpsdata);
	goto connect;

	return NULL;
}

void location_init(struct context* cntx) {
	g_thread_new("location_gps", location_gps_threadfunc, cntx);
}
