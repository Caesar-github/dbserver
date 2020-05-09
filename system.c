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

#define TABLE_SYSTEM_DEVICE_INFO     "SystemDeviceInfo"
#define TABLE_SYSTEM_PARA            "SystemPara"
#define TABLE_SYSTEM_USER            "SystemUser"
#define TABLE_SYSTEM_VERSION         "SystemVersion"

#define SYSTEM_VERSION             "1.0.1"

static int dbus_manager_init(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_SYSTEM_INTERFACE,
                              methods,
                              signals, NULL, DB_SYSTEM_INTERFACE, NULL);

    return 0;
}

void system_init(DBusConnection *dbus_conn)
{
    char *col_para;

    if (equal_version(TABLE_SYSTEM_VERSION, SYSTEM_VERSION)) {
        dbus_manager_init(dbus_conn);
        return;
    }

    g_free(rkdb_drop(TABLE_SYSTEM_DEVICE_INFO));
    g_free(rkdb_drop(TABLE_SYSTEM_PARA));
    g_free(rkdb_drop(TABLE_SYSTEM_USER));
    g_free(rkdb_drop(TABLE_SYSTEM_VERSION));

    creat_version_table(TABLE_SYSTEM_VERSION, SYSTEM_VERSION);

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "name TEXT," \
               "value TEXT," \
               "ro TEXT";
    g_free(rkdb_create(TABLE_SYSTEM_DEVICE_INFO, col_para));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "0, 'deviceName', 'RK IP Camera', 'false'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "1, 'telecontrolID', '88', 'false'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "2, 'model', 'RK-003', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "3, 'serialNumber', 'RK-003-A', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "4, 'firmwareVersion', 'V0.2.6 build 200413', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "5, 'encoderVersion', 'V1.0 build 200413', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "6, 'webVersion', 'V1.12.2 build 200413', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "7, 'pluginVersion', 'V1.0.0.0', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "8, 'channelsNumber', '1', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "9, 'hardDisksNumber', '1', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "10, 'alarmInputsNumber', '0', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "11, 'alarmOutputsNumber', '0', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "12, 'firmwareVersionInfo', 'CP-3-B', 'true'"));

    /*
    0: storage/plan/snap time, unit: millionseconds
    1: storage/plan/snap screenshot-schedule, define default type, eg:[{name: xxx, color: '#ffffff'}]
    2: storage/plan/video video-plan-schedule, define default type, eg:[{name: xxx, color: '#ffffff'}]
    */
    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "name TEXT," \
               "para TEXT";
    g_free(rkdb_create(TABLE_SYSTEM_PARA, col_para));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "0, 'storagePlanSnap', '{\"imageType\":[\"JPEG\"]," \
        "\"time2number\":{\"milliseconds\":1,\"seconds\":1000,\"minutes\":60000,\"hours\":3600000,\"days\":86400000}," \
        "\"resolutionList\":[\"2688*1520\"]," \
        "\"quality2number\":{\"low\":40,\"middle\":60,\"high\":80},"
        "\"limitRange\":{\"timing\":{\"max\":604800000,\"min\":1000},\"event\":{\"max\":65535,\"min\": 1000},\"shot\":{\"max\":120,\"min\":1}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "1, 'screenshotSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}]'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "2, 'videoPlanSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}, {\"name\":\"motion-detect\",\"color\":\"#74B558\"}," \
        "{\"name\":\"alarm\",\"color\":\"#D71820\"}, {\"name\":\"motionOrAlarm\",\"color\":\"#E58705\"}, {\"name\":\"motionAndAlarm\",\"color\":\"#B9E2FE\"}," \
        "{\"name\":\"event\",\"color\":\"#AA6FFF\"}]'"));

    /*
    password encodes by base64
    */
    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sUserName TEXT UNIQUE," \
               "sPassword TEXT," \
               "iFixed INT DEFAULT 0," \
               "iUserLevel INT DEFAULT 0," \
               "iAuthLevel INTEGER DEFAULT 1";
    g_free(rkdb_create(TABLE_SYSTEM_USER, col_para));
    g_free(rkdb_insert(TABLE_SYSTEM_USER, "id, sUserName, sPassword, iFixed, iUserLevel, iAuthLevel", "0, 'admin', 'YWRtaW4=', 1, 0, 1"));

    dbus_manager_init(dbus_conn);
}
