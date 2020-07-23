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
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "13, 'manufacturer', 'Rockchip', 'true'"));
    g_free(rkdb_insert(TABLE_SYSTEM_DEVICE_INFO, "id, name, value, ro", "14, 'hardwareId', 'c3d9b8674f4b94f6', 'true'"));

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
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "0, 'StoragePlanSnap', '{\"dynamic\":{\"id\":{\"0\":{\"iShotInterval\":{\"for\":\"timing\",\"range\":{\"max\":604800000,\"min\":1000},\"type\":\"range\"},\"timeUnit\":{\"for\":\"timing\","\
        "\"options\":[\"seconds\",\"minutes\",\"hours\",\"days\"],\"type\":\"options\"}},\"1\":{\"iShotInterval\":{\"for\":\"timing\",\"range\":{\"max\":65535,\"min\":1000},\"type\":\"range\"},\"timeUnit\":{\"for\":\"timing\",\"options\":[\"seconds\","\
        "\"minutes\"],\"type\":\"options\"}}}},\"relation\":{\"iImageQuality\":{\"1\":\"low\",\"10\":\"high\",\"5\":\"middle\"}},\"static\":{\"iImageQuality\":{\"options\":[1,5,10],\"type\":\"options\"},\"iShotNumber\":{\"range\":{\"max\":120,\"min\":1},"\
        "\"type\":\"range\"},\"sImageType\":{\"options\":[\"JPEG\"],\"type\":\"options\"},\"sResolution\":{\"refer\":[4,\"para\",\"dynamic\",\"sStreamType\",\"mainStream\",\"sResolution\"],\"type\":\"refer\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "1, 'screenshotSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}]'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "2, 'videoPlanSchedule', '[{\"name\":\"timing\",\"color\":\"#87CEEB\"}, {\"name\":\"motion-detect\",\"color\":\"#74B558\"}," \
        "{\"name\":\"alarm\",\"color\":\"#D71820\"}, {\"name\":\"motionOrAlarm\",\"color\":\"#E58705\"}, {\"name\":\"motionAndAlarm\",\"color\":\"#B9E2FE\"}," \
        "{\"name\":\"event\",\"color\":\"#AA6FFF\"}]'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "3, 'smartCover', '{\"capability\":{\"SmartCover\":{\"sImageQuality\":[\"best\",\"good\",\"general\"],\"sTargetImageType\":[\"head\"]}},\"layout\":{\"enabled\":[\"iFaceRecognitionEnabled\",\"iStreamOverlayEnabled\","\
        "\"iImageOverlayEnabled\"],\"infoEnabled\":[\"deviceNum\",\"positonInfo\"],\"snap\":[\"sTargetImageType\",\"iWidthRatio\",\"sImageQuality\"]}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "4, 'video', '{\"disabled\":[{\"name\":\"sStreamType\",\"options\":{\"subStream\":{\"sOutputDataType\":\"H.264\",\"sSmart\":\"close\"},\"thirdStream\":{\"sSmart\":\"close\"}},\"type\":\"disabled/limit\"},"\
        "{\"name\":\"sSmart\",\"options\":{\"open\":{\"iGOP\":null,\"iStreamSmooth\":null,\"sH264Profile\":null,\"sRCMode\":null,\"sRCQuality\":null,\"sSVC\":null}},\"type\":\"disabled\"},{\"name\":\"sRCMode\",\"options\":{\"CBR\":{\"sRCQuality\":null}},\"type\":\"disabled\"},"\
        "{\"name\":\"sOutputDataType\",\"options\":{\"H.265\":{\"sH264Profile\":null}},\"type\":\"disabled\"},{\"name\":\"unspport\",\"options\":{\"iStreamSmooth\":null,\"sSVC\":null,\"sVideoType\":null},\"type\":\"disabled\"}],\"dynamic\":{\"sSmart\":{\"open\":{\"iMinRate\":"\
        "{\"dynamicRange\":{\"max\":\"iMaxRate\",\"maxRate\":1,\"min\":\"iMaxRate\",\"minRate\":0.125},\"type\":\"dynamicRange\"}}},\"sStreamType\":{\"mainStream\":{\"iMaxRate\":{\"options\":[256,512,1024,2048,3072,4096,6144,8192,12288,16384],\"type\":\"options\"},\"sResolution\":"\
        "{\"options\":[\"2688*1520\"],\"type\":\"options\"}},\"subStream\":{\"iMaxRate\":{\"options\":[256,512,1024,2048,3072,4096,6144,8192],\"type\":\"options\"},\"sResolution\":{\"options\":[\"640*480\",\"704*576\"],\"type\":\"options\"}},\"thirdStream\":{\"iMaxRate\":{\"options\""\
        ":[256,512,1024,2048,3072,4096,6144,8192,12288,16384],\"type\":\"options\"},\"sResolution\":{\"options\":[\"640*480\",\"704*576\",\"1280*720\",\"1920*1080\"],\"type\":\"options\"}}}},\"layout\":{\"encoder\":[\"sStreamType\",\"sVideoType\",\"sResolution\",\"sRCMode\","\
        "\"sRCQuality\",\"sFrameRate\",\"sOutputDataType\",\"sSmart\",\"sH264Profile\",\"sSVC\",\"iMaxRate\",\"iMinRate\",\"iGOP\",\"iStreamSmooth\"]},\"static\":{\"iGOP\":{\"range\":{\"max\":400,\"min\":0},\"type\":\"range\"},\"iStreamSmooth\":{\"range\":{\"max\":100,\"min\":1,"\
        "\"step\":1},\"type\":\"range\"},\"sFrameRate\":{\"dynamicRange\":{\"max\":\"sFrameRateIn\",\"maxRate\":1},\"options\":[\"1/16\",\"1/8\",\"1/4\",\"1/2\",\"1\",\"2\",\"4\",\"6\",\"8\",\"10\",\"12\",\"14\",\"16\",\"18\",\"20\",\"25\",\"30\"],\"type\":\"options/dynamicRange\"},"\
        "\"sH264Profile\":{\"options\":[\"high\",\"main\",\"baseline\"],\"type\":\"options\"},\"sOutputDataType\":{\"options\":[\"H.264\",\"H.265\"],\"type\":\"options\"},\"sRCMode\":{\"options\":[\"CBR\",\"VBR\"],\"type\":\"options\"},\"sRCQuality\":{\"options\":[\"lowest\",\"lower\","\
        "\"low\",\"medium\",\"high\",\"higher\",\"highest\"],\"type\":\"options\"},\"sSVC\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sSmart\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sStreamType\":{\"options\":[\"mainStream\",\"subStream\","\
        "\"thirdStream\"],\"type\":\"options\"},\"sVideoType\":{\"options\":[\"videoStream\",\"compositeStream\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "5, 'video_advanced_enc', '{\"static\":{\"sStreamType\":{\"refer\":[4,\"para\",\"static\",\"sStreamType\"],\"type\":\"refer\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "6, 'roi', '{\"static\":{\"sStreamType\":{\"refer\":[4,\"para\",\"static\",\"sStreamType\"],\"type\":\"refer\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "7, 'image_adjustment', '{\"layout\":{\"image_adjustment\":[\"iBrightness\",\"iContrast\",\"iSaturation\",\"iSharpness\"]},\"static\":{\"iBrightness\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},"\
        "\"iContrast\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iSaturation\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iSharpness\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "8, 'image_exposure', '{\"dynamic\":{\"sIrisType\":{\"auto\":{\"iAutoIrisLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_exposure\":[\"sExposureTime\",\"iExposureGain\"]},"\
        "\"static\":{\"iExposureGain\":{\"range\":{\"max\":100,\"min\":1,\"step\":1},\"type\":\"range\"},\"sExposureTime\":{\"options\":[\"1\",\"1/3\",\"1/6\",\"1/12\",\"1/25\",\"1/50\",\"1/100\",\"1/150\",\"1/200\",\"1/250\",\"1/500\",\"1/750\",\"1/1000\","\
        "\"1/2000\",\"1/4000\",\"1/10000\",\"1/100000\"],\"type\":\"options\"},\"sIrisType\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "9, 'image_night_to_day', '{\"dynamic\":{\"sBrightnessAdjustmentMode\":{\"manual\":{\"iLightBrightness\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sNightToDay\":{\"alarmInputTrigger\":"\
        "{\"sIrcutFilterAction\":{\"options\":[\"day\",\"night\"],\"type\":\"options\"}},\"auto\":{\"iNightToDayFilterLevel\":{\"options\":[0,1,2,3,4,5,6,7],\"type\":\"options\"},\"iNightToDayFilterTime\":{\"range\":{\"max\":120,\"min\":5,\"step\":1},\"type\":\"range\"}},"\
        "\"manual\":{\"sBeginTime\":{\"input\":\"time\",\"type\":\"input\"},\"sEndTime\":{\"input\":\"time\",\"type\":\"input\"}}},\"sOverexposeSuppress\":{\"open\":{\"sOverexposeSuppressType\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"}}},\"sOverexposeSuppressType\":"\
        "{\"manual\":{\"iDistanceLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_night_to_day\":[\"sNightToDay\",\"iNightToDayFilterLevel\",\"iNightToDayFilterTime\",\"sBeginTime\",\"sEndTime\",\"sFillLightMode\","\
        "\"sIrcutFilterAction\"]},\"static\":{\"sBrightnessAdjustmentMode\":{\"options\":[\"auto\",\"manual\"],\"type\":\"options\"},\"sFillLightMode\":{\"options\":[\"IR\"],\"type\":\"options\"},\"sNightToDay\":{\"options\":[\"day\",\"night\",\"auto\"],\"type\":\"options\"},"\
        "\"sOverexposeSuppress\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "10, 'image_blc', '{\"dynamic\":{\"sBLCRegion\":{\"close\":{\"sHDR\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sWDR\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"}}},\"sHDR\":{\"open\":"\
        "{\"iHDRLevel\":{\"range\":{\"max\":10,\"min\":1,\"step\":1},\"type\":\"range\"}}},\"sHLC\":{\"open\":{\"iHLCLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sWDR\":{\"open\":{\"iWDRLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},"\
        "\"type\":\"range\"}}}},\"layout\":{\"image_blc\":[\"sHDR\",\"iHDRLevel\"]},\"static\":{\"sBLCRegion\":{\"options\":[\"close\",\"up\",\"down\",\"left\",\"right\",\"center\",\"customize\"],\"type\":\"options\"},\"sHLC\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "11, 'image_white_blance', '{\"dynamic\":{\"sWhiteBlanceStyle\":{\"manualWhiteBalance\":{\"iWhiteBalanceBlue\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iWhiteBalanceRed\":{\"range\":"\
        "{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_white_blance\":[\"sWhiteBlanceStyle\",\"iWhiteBalanceRed\",\"iWhiteBalanceBlue\"]},\"static\":{\"sWhiteBlanceStyle\":{\"options\":[\"manualWhiteBalance\",\"autoWhiteBalance1\","\
        "\"lockingWhiteBalance\",\"fluorescentLamp\",\"incandescent\",\"warmLight\",\"naturalLight\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "12, 'image_enhancement', '{\"dynamic\":{\"sDehaze\":{\"open\":{\"iDehazeLevel\":{\"range\":{\"max\":10,\"min\":0,\"step\":1},\"type\":\"range\"}}},\"sNoiseReduceMode\":{\"advanced\":{\"iSpatialDenoiseLevel\":{\"range\":"\
		"{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"},\"iTemporalDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}},\"general\":{\"iDenoiseLevel\":{\"range\":{\"max\":100,\"min\":0,\"step\":1},\"type\":\"range\"}}}},\"layout\":{\"image_enhancement\":"\
		"[\"sNoiseReduceMode\",\"iDenoiseLevel\",\"iImageRotation\",\"iSpatialDenoiseLevel\",\"iTemporalDenoiseLevel\",\"sDehaze\",\"iDehazeLevel\",\"sGrayScaleMode\",\"sFEC\"]},\"static\":{\"iImageRotation\":{\"options\":[0,90,270],\"type\":\"options\"},\"sDIS\":{\"options\":[\"open\","\
		"\"close\"],\"type\":\"options\"},\"sDehaze\":{\"options\":[\"open\",\"close\",\"auto\"],\"type\":\"options\"},\"sFEC\":{\"options\":[\"open\",\"close\"],\"type\":\"options\"},\"sGrayScaleMode\":{\"options\":[\"[0-255]\",\"[16-235]\"],\"type\":\"options\"},\"sNoiseReduceMode\":"\
		"{\"options\":[\"close\",\"general\",\"advanced\"],\"type\":\"options\"}}}'"));
    g_free(rkdb_insert(TABLE_SYSTEM_PARA, "id, name, para", "13, 'image_video_adjustment', '{\"layout\":{\"image_video_adjustment\":[\"sPowerLineFrequencyMode\"]},\"static\":{\"sImageFlip\":{\"options\":[\"leftRight\",\"upDown\",\"center\",\"close\"],\"type\":\"options\"},"\
        "\"sPowerLineFrequencyMode\":{\"options\":[\"PAL(50HZ)\",\"NTSC(60HZ)\"],\"type\":\"options\"},\"sSceneMode\":{\"options\":[\"indoor\",\"outdoor\"],\"type\":\"options\"}}}'"));

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
