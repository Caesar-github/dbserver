#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <inttypes.h>

#include <glib.h>

#include <pthread.h>
#include <gdbus.h>

#include "json-c/json.h"
#include "rkdb.h"
#include "common.h"

#define TABLE_EVENT_TRIGGERS            "EventTriggers"
#define TABLE_EVENT_SCHEDULES           "EventSchedules"
#define TABLE_EVENT_MOTION_DETECTION    "EventMotionDetection"
#define TABLE_EVENT_VERSION             "EventVersion"

#define EVENT_VERSION             "1.0.1"

static int dbus_manager_init(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_EVENT_INTERFACE,
                              methods,
                              signals, NULL, DB_EVENT_INTERFACE, NULL);

    return 0;
}

void event_init(DBusConnection *dbus_conn)
{
    char *col_para;

    if (equal_version(TABLE_EVENT_VERSION, EVENT_VERSION)) {
        dbus_manager_init(dbus_conn);
        return;
    }

    g_free(rkdb_drop(TABLE_EVENT_TRIGGERS));
    g_free(rkdb_drop(TABLE_EVENT_SCHEDULES));
    g_free(rkdb_drop(TABLE_EVENT_MOTION_DETECTION));
    g_free(rkdb_drop(TABLE_EVENT_VERSION));

    creat_version_table(TABLE_EVENT_VERSION, EVENT_VERSION);

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sEventType TEXT," \
               "iVideoInputChannelID INT DEFAULT 0," \
               "iNotificationIO1Enabled INT DEFAULT 0," \
               "iNotificationRecord1Enabled INT DEFAULT 0," \
               "iNotificationEmailEnabled INT DEFAULT 0," \
               "iNotificationCenterEnabled INT DEFAULT 0," \
               "iNotificationFTPEnabled INT DEFAULT 0";
    g_free(rkdb_create(TABLE_EVENT_TRIGGERS, col_para));
    g_free(rkdb_insert(TABLE_EVENT_TRIGGERS, "id, sEventType", "0, 'VMD'"));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sEventType TEXT," \
               "iVideoInputChannelID INT DEFAULT 0," \
               "sDay1BeginTime TEXT DEFAULT '00:00'," \
               "sDay1EndTime TEXT DEFAULT '24:00'," \
               "sDay2BeginTime TEXT DEFAULT '00:00'," \
               "sDay2EndTime TEXT DEFAULT '24:00'," \
               "sDay3BeginTime TEXT DEFAULT '00:00'," \
               "sDay3EndTime TEXT DEFAULT '24:00'," \
               "sDay4BeginTime TEXT DEFAULT '00:00'," \
               "sDay4EndTime TEXT DEFAULT '24:00'," \
               "sDay5BeginTime TEXT DEFAULT '00:00'," \
               "sDay5EndTime TEXT DEFAULT '24:00'," \
               "sDay6BeginTime TEXT DEFAULT '00:00'," \
               "sDay6EndTime TEXT DEFAULT '24:00'," \
               "sDay7BeginTime TEXT DEFAULT '00:00'," \
               "sDay7EndTime TEXT DEFAULT '24:00'";
    g_free(rkdb_create(TABLE_EVENT_SCHEDULES, col_para));
    g_free(rkdb_insert(TABLE_EVENT_SCHEDULES, "id, sEventType", "0, 'VMD'"));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "iMotionDetectionEnabled INT DEFAULT 0," \
               "iHighlightEnabled INT DEFAULT 0," \
               "iSamplingInterval INT DEFAULT 2," \
               "iStartTriggerTime INT DEFAULT 500," \
               "iEndTriggerTime INT DEFAULT 500," \
               "sRegionType TEXT DEFAULT 'grid'," \
               "iRowGranularity INT DEFAULT 18," \
               "iColumnGranularity INT DEFAULT 22," \
               "iSensitivityLevel INT DEFAULT 0," \
               "sGridMap TEXT DEFAULT '000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000'";
    g_free(rkdb_create(TABLE_EVENT_MOTION_DETECTION, col_para));
    g_free(rkdb_insert(TABLE_EVENT_MOTION_DETECTION, "id", "0"));

    dbus_manager_init(dbus_conn);
}
