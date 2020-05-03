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
#define TABLE_REGIONAL_INVASION         "RegionalInvasion"
#define TABLE_MOVE_DETECTION            "MoveDetection"
#define TABLE_FACE_CONFIG               "FaceConfig"
#define TABLE_FACE_LIST                 "FaceList"
#define TABLE_FACE_SNAPSHOT_RECORD      "FaceSnapshotRecord"
#define TABLE_FACE_CONTROL_RECORD       "FaceControlRecord"
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
    g_free(rkdb_drop(TABLE_REGIONAL_INVASION));
    g_free(rkdb_drop(TABLE_MOVE_DETECTION));
    g_free(rkdb_drop(TABLE_FACE_CONFIG));
    g_free(rkdb_drop(TABLE_FACE_LIST));
    g_free(rkdb_drop(TABLE_FACE_SNAPSHOT_RECORD));
    g_free(rkdb_drop(TABLE_FACE_CONTROL_RECORD));
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
               "iEnabled INT DEFAULT 0," \
               "iTimeThreshold INT DEFAULT 3," \
               "iProportion INT DEFAULT 0," \
               "iSensitivityLevel INT DEFAULT 50," \
               "iPositionX INT DEFAULT 0," \
               "iPositionY INT DEFAULT 0," \
               "iWidth INT DEFAULT 0," \
               "iHeight INT DEFAULT 0";
    g_free(rkdb_create(TABLE_REGIONAL_INVASION, col_para));
    g_free(rkdb_insert(TABLE_REGIONAL_INVASION, "id", "0"));

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
    g_free(rkdb_create(TABLE_MOVE_DETECTION, col_para));
    g_free(rkdb_insert(TABLE_MOVE_DETECTION, "id", "0"));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "iPromptVolume INT DEFAULT 50," \
               "sLiveDetect TEXT DEFAULT 'open'," \
               "sLiveDetectBeginTime TEXT DEFAULT '00:00'," \
               "sLiveDetectEndTime TEXT DEFAULT '23:59'," \
               "iLiveDetectThreshold INT DEFAULT 90," \
               "iFaceDetectionThreshold INT DEFAULT 50," \
               "iFaceRecognitionThreshold INT DEFAULT 50," \
               "iFaceMinPixel INT DEFAULT 125," \
               "iLeftCornerX INT DEFAULT 0," \
               "iLeftCornerY INT DEFAULT 125," \
               "iDetectWidth INT DEFAULT 960," \
               "iDetectHeight INT DEFAULT 1280";
    g_free(rkdb_create(TABLE_FACE_CONFIG, col_para));
    g_free(rkdb_insert(TABLE_FACE_CONFIG, "id", "0"));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sPicturePath TEXT DEFAULT ''," \
               "sRegistrationTime TEXT DEFAULT '2020-01-01T00:00:00'," \
               "iAge INT DEFAULT 20," \
               "sListType TEXT DEFAULT 'permanent'," \
               "sType TEXT DEFAULT 'whiteList'," \
               "sName TEXT DEFAULT 'User'," \
               "sGender TEXT DEFAULT 'male'," \
               "sNation TEXT DEFAULT '汉'," \
               "sCertificateType TEXT DEFAULT 'identityCard'," \
               "sCertificateNumber TEXT DEFAULT ''," \
               "sBirthday TEXT DEFAULT '2000-01-01'," \
               "sTelephoneNumber TEXT DEFAULT ''," \
               "sHometown TEXT DEFAULT ''," \
               "sAddress TEXT DEFAULT ''," \
               "iAccessCardNumber INT DEFAULT 0," \
               "iLoadCompleted INT DEFAULT 0," \
               "sNote TEXT DEFAULT ''";
    g_free(rkdb_create(TABLE_FACE_LIST, col_para));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sTime TEXT DEFAULT '2020-01-01T00:00:00'," \
               "sPicturePath TEXT DEFAULT ''," \
               "sStatus TEXT DEFAULT 'Processed'," \
               "sNote TEXT DEFAULT 'Snapshot'";
    g_free(rkdb_create(TABLE_FACE_SNAPSHOT_RECORD, col_para));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sTime TEXT DEFAULT '2020-01-01T00:00:00'," \
               "iFaceId INT DEFAULT 0," \
               "sSnapshotPath TEXT DEFAULT ''," \
               "sStatus TEXT DEFAULT 'open'," \
               "sSimilarity TEXT DEFAULT '75.00'";
    g_free(rkdb_create(TABLE_FACE_CONTROL_RECORD, col_para));

    dbus_manager_init(dbus_conn);
}
