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
#define TABLE_SYSTEM_VERSION       "SystemVersion"

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
    dbus_manager_init(dbus_conn);
}
