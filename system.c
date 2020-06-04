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

int system_dbus_register(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_SYSTEM_INTERFACE,
                              methods,
                              signals, NULL, DB_SYSTEM_INTERFACE, NULL);

    return 0;
}

void system_init(void)
{
    char *col_para;

    if (equal_version(TABLE_SYSTEM_VERSION, SYSTEM_VERSION))
        return;

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
    layout: key is part of web, value Array include para show in this part
    capability: the option of para, para tree is similar to what's in the table
    order: url to use; url para to get; remark
    0: storage/plan/snap; screenshot time; unit: millionseconds
    1: storage/plan/snap; screenshot-schedule; define default type, eg:[{name: xxx, color: '#ffffff'}]
    2: storage/plan/video; video-plan-schedule; define default type, eg:[{name: xxx, color: '#ffffff'}]
    3: event/smart/cover event/smart/overlay; smart-cover;
    4: video/0; video-encoder; disabledOption: key checkItem, value toCheck:(key: disabled key, value: set when disabled null for not),item whose value change must be top
    */
    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "name TEXT," \
               "para TEXT";
    g_free(rkdb_create(TABLE_SYSTEM_PARA, col_para));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "0, 'storagePlanSnap', '{\"imageType\":[\"JPEG\"]," \
        "\"time2number\":{\"milliseconds\":1,\"seconds\":1000,\"minutes\":60000,\"hours\":3600000,\"days\":86400000}," \
        "\"resolutionList\":[\"2688*1520\"]," \
        "\"quality2number\":{\"low\":1,\"middle\":5,\"high\":10},"
        "\"limitRange\":{\"timing\":{\"max\":604800000,\"min\":1000},\"event\":{\"max\":65535,\"min\": 1000},\"shot\":{\"max\":120,\"min\":1}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "1, 'screenshotSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}]'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "2, 'videoPlanSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}, {\"name\":\"motion-detect\",\"color\":\"#74B558\"}," \
        "{\"name\":\"alarm\",\"color\":\"#D71820\"}, {\"name\":\"motionOrAlarm\",\"color\":\"#E58705\"}, {\"name\":\"motionAndAlarm\",\"color\":\"#B9E2FE\"}," \
        "{\"name\":\"event\",\"color\":\"#AA6FFF\"}]'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "3, 'smartCover', '{\"layout\":{\"enabled\":[\"iStreamOverlayEnabled\", \"iImageOverlayEnabled\"]," \
        "\"snap\":[\"sTargetImageType\", \"iWidthRatio\", \"sImageQuality\"], \"infoEnabled\":[\"deviceNum\", \"positonInfo\"]}, \"capability\":{\"SmartCover\":{\"sTargetImageType\":[\"head\"], \"sImageQuality\":" \
        "[\"best\",\"good\",\"general\"]}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "4, 'videoEncoder'," \
        "'{\"public\":["\
                "{\"name\":\"streamType\","\
                 "\"items\": [\"mainStream\",\"subStream\",\"thirdStream\"]},"\
                "{\"name\":\"videoType\","\
                 "\"items\": [\"videoStream\",\"compositeStream\"]},"\
                "{\"name\":\"RCMode\","\
                 "\"items\": [\"CBR\",\"VBR\"]},"\
                "{\"name\":\"RCQuality\","\
                 "\"items\": [\"lowest\",\"lower\",\"low\",\"medium\",\"high\",\"higher\",\"highest\"]},"\
                "{\"name\":\"frameRate\","\
                 "\"items\": [\"1/16\",\"1/8\",\"1/4\",\"1/2\",\"1\",\"2\",\"4\",\"6\",\"8\",\"10\",\"12\",\"14\",\"16\",\"18\",\"20\",\"25\",\"30\"]},"\
                "{\"name\":\"outputDataType\","\
                 "\"items\": [\"H.264\",\"H.265\"]},"\
                "{\"name\":\"smart\","\
                 "\"items\": [\"open\",\"close\"]},"\
                "{\"name\":\"H264Profile\","\
                 "\"items\": [\"high\",\"main\",\"baseline\"]},"\
                "{\"name\":\"SVC\","\
                 "\"items\": [\"open\",\"close\"]}],"\
            "\"mainStream\": ["\
                "{\"name\":\"resolution\","\
                 "\"items\": [\"2688*1520\"]},"\
                "{\"name\":\"maxRate\","\
                 "\"items\": [256,512,1024,2048,3072,4096,6144,8192,12288,16384]}],"\
            "\"subStream\": ["\
                "{\"name\":\"resolution\","\
                 "\"items\": [\"640*480\",\"704*576\"]},"\
                "{\"name\":\"maxRate\","\
                 "\"items\": [256,512,1024,2048,3072,4096,6144,8192]}],"\
            "\"thirdStream\": ["\
                "{\"name\":\"resolution\","\
                 "\"items\": [\"640*480\",\"704*576\",\"1280*720\",\"1920*1080\"]},"\
                "{\"name\":\"maxRate\","\
                 "\"items\": [256,512,1024,2048,3072,4096,6144,8192,12288,16384]}],"\
            "\"disabledOption\": ["\
                "{\"name\":\"sStreamType\","\
                 "\"options\": {"\
                    "\"subStream\":{"\
                        "\"sOutputDataType\":\"H.264\","\
                        "\"sSmart\": \"close\"},"\
                    "\"thirdStream\":{"\
                        "\"sSmart\": \"close\"}}},"\
                "{\"name\":\"sSmart\","\
                 "\"options\": {"\
                    "\"open\":{"\
                        "\"iGOP\":null,"\
                        "\"sRCMode\":null,"\
                        "\"sH264Profile\":null,"\
                        "\"sSVC\":null,"\
                        "\"iStreamSmooth\":null,"\
                        "\"sRCQuality\": null}}},"\
                "{\"name\":\"sRCMode\","\
                 "\"options\": {"\
                    "\"CBR\":{"\
                        "\"sRCQuality\": null}}},"\
                "{\"name\":\"sOutputDataType\","\
                 "\"options\": {"\
                    "\"H.265\":{"\
                        "\"sH264Profile\": null}}},"\
                "{\"name\":\"unspport\","\
                 "\"options\": {"\
                    "\"sVideoType\":null,"\
                    "\"sSVC\":null,"\
                    "\"iStreamSmooth\": null}}]}'"));

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
}
