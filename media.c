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

#define TABLE_VIDEO "video"
#define TABLE_AUDIO "audio"
#define TABLE_STREAM_URL "stream_url"

#define TABLE_IMAGE_SCENARIO "image_scenario"
#define TABLE_IMAGE_ADJUSTMENT "image_adjustment"
#define TABLE_IMAGE_EXPOSURE "image_exposure"
#define TABLE_IMAGE_NIGHT_TO_DAY "image_night_to_day"
#define TABLE_IMAGE_BLC "image_blc"
#define TABLE_IMAGE_WHITE_BLANCE "image_white_blance"
#define TABLE_IMAGE_ENHANCEMENT "image_enhancement"
#define TABLE_IMAGE_VIDEO_ADJUSTMEN "image_video_adjustment"
#define TABLE_MEDIA_VERSION       "MediaVersion"

#define TABLE_NORMALIZED_SCREEN_SIZE "normalized_screen_size"
#define TABLE_OSD "osd"

#define MEDIA_VERSION             "1.0.1"

static int dbus_manager_init(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_MEDIA_INTERFACE,
                              methods,
                              signals, NULL, DB_MEDIA_INTERFACE, NULL);

    return 0;
}

void media_init(DBusConnection *dbus_conn)
{
    char *col_para;

    if (equal_version(TABLE_MEDIA_VERSION, MEDIA_VERSION)) {
        dbus_manager_init(dbus_conn);
        return;
    }

    g_free(rkdb_drop(TABLE_VIDEO));
    g_free(rkdb_drop(TABLE_AUDIO));
    g_free(rkdb_drop(TABLE_STREAM_URL));
    g_free(rkdb_drop(TABLE_IMAGE_SCENARIO));
    g_free(rkdb_drop(TABLE_IMAGE_ADJUSTMENT));
    g_free(rkdb_drop(TABLE_IMAGE_EXPOSURE));
    g_free(rkdb_drop(TABLE_IMAGE_NIGHT_TO_DAY));
    g_free(rkdb_drop(TABLE_IMAGE_BLC));
    g_free(rkdb_drop(TABLE_IMAGE_WHITE_BLANCE));
    g_free(rkdb_drop(TABLE_IMAGE_ENHANCEMENT));
    g_free(rkdb_drop(TABLE_IMAGE_VIDEO_ADJUSTMEN));
    g_free(rkdb_drop(TABLE_MEDIA_VERSION));

    creat_version_table(TABLE_MEDIA_VERSION, MEDIA_VERSION);

    col_para = "id INTEGER PRIMARY KEY," \
                     "sStreamType TEXT DEFAULT 'mainStream'," \
                     "sVideoType TEXT DEFAULT 'compositeStream'," \
                     "sResolution TEXT DEFAULT '1920*1080'," \
                     "sRCMode TEXT DEFAULT 'CBR'," \
                     "sImageQuality TEXT DEFAULT 'medium'," \
                     "sFrameRate TEXT DEFAULT '30'," \
                     "iMaxRate INT DEFAULT 4096," \
                     "sOutputDataType TEXT DEFAULT 'H.264'," \
                     "sSmart TEXT DEFAULT 'close'," \
                     "sRCQuality TEXT DEFAULT 'high'," \
                     "iGOP INT DEFAULT 30," \
                     "sSVC TEXT DEFAULT 'close'," \
                     "iStreamSmooth INT DEFAULT 50";

    g_free(rkdb_create(TABLE_VIDEO, col_para));
    /* TODO: Three tables need different initial values */
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType", "0, 'mainStream'"));
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType", "1, 'subStream'"));
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType", "2, 'thirdStream'"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sEncodeType TEXT DEFAULT 'AAC'," \
               "iSampleRate INT DEFAULT 16000," \
               "iBitRate INT DEFAULT 32000," \
               "sInput TEXT DEFAULT 'micIn'," \
               "iVolume INT DEFAULT 50," \
               "sANS TEXT DEFAULT 'close'";

    g_free(rkdb_create(TABLE_AUDIO, col_para));
    g_free(rkdb_insert(TABLE_AUDIO, "id", "0"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sStreamProtocol TEXT NOT NULL";

    g_free(rkdb_create(TABLE_STREAM_URL, col_para));
    g_free(rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "0,'RTSP'"));
    g_free(rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "1,'RTMP'"));
    g_free(rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "2,'RTMP'"));

    /* image */
    col_para = "id INTEGER PRIMARY KEY," \
               "sScenario TEXT DEFAULT 'normal'";
    g_free(rkdb_create(TABLE_IMAGE_SCENARIO, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_SCENARIO, "id", "0"));

    col_para = "id INTEGER PRIMARY KEY," \
               "iBrightness INT DEFAULT 50," \
               "iContrast INT DEFAULT 50," \
               "iSaturation INT DEFAULT 50," \
               "iSharpness INT DEFAULT 50";
    g_free(rkdb_create(TABLE_IMAGE_ADJUSTMENT, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_ADJUSTMENT, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sIrisType TEXT DEFAULT 'auto'," \
               "iAutoIrisLevel INT DEFAULT 5," \
               "sExposureTime TEXT DEFAULT '1/6'," \
               "iExposureGain INT DEFAULT 0";
    g_free(rkdb_create(TABLE_IMAGE_EXPOSURE, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_EXPOSURE, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sNightToDay TEXT DEFAULT 'auto'," \
               "iNightToDayFilterLevel INT DEFAULT 5," \
               "iNightToDayFilterTime INT DEFAULT 5," \
               "sBeginTime TEXT DEFAULT '07:00:00'," \
               "sEndTime TEXT DEFAULT '18:00:00'," \
               "sIrcutFilterAction TEXT DEFAULT 'day'," \
               "sOverexposeSuppress TEXT DEFAULT 'open'," \
               "sOverexposeSuppressType TEXT DEFAULT 'auto'," \
               "iDistanceLevel INT DEFAULT 1";
    g_free(rkdb_create(TABLE_IMAGE_NIGHT_TO_DAY, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_NIGHT_TO_DAY, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sBLCRegion TEXT DEFAULT 'close'," \
               "sWDR TEXT DEFAULT 'close'," \
               "iWDRLevel INT DEFAULT 0," \
               "sHLC TEXT DEFAULT 'close'," \
               "iHLCLevel INT DEFAULT 0," \
               "iPositionX INT DEFAULT 0," \
               "iPositionY INT DEFAULT 0," \
               "iBLCRegionWidth INT DEFAULT 120," \
               "iBLCRegionHeight INT DEFAULT 92";
    g_free(rkdb_create(TABLE_IMAGE_BLC, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_BLC, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sWhiteBlanceStyle TEXT DEFAULT 'naturalLight'," \
               "iWhiteBalanceRed INT DEFAULT 50," \
               "iWhiteBalanceBlue INT DEFAULT 50";
    g_free(rkdb_create(TABLE_IMAGE_WHITE_BLANCE, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_WHITE_BLANCE, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sNoiseReduceMode TEXT DEFAULT 'general'," \
               "iDenoiseLevel INT DEFAULT 0," \
               "iSpatialDenoiseLevel INT DEFAULT 0," \
               "iTemporalDenoiseLevel INT DEFAULT 0," \
               "sDehaze TEXT DEFAULT 'close'," \
               "iDehazeLevel INT DEFAULT 0," \
               "sDIS TEXT DEFAULT 'close'," \
               "sGrayScaleMode TEXT DEFAULT '[0-255]'";
    g_free(rkdb_create(TABLE_IMAGE_ENHANCEMENT, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_ENHANCEMENT, "id", "5"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sImageFlip TEXT DEFAULT 'close'," \
               "sSceneMode TEXT DEFAULT 'indoor'," \
               "sPowerLineFrequencyMode TEXT DEFAULT 'PAL(50HZ)'";
    g_free(rkdb_create(TABLE_IMAGE_VIDEO_ADJUSTMEN, col_para));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "0"));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "1"));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "2"));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "3"));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "4"));
    g_free(rkdb_insert(TABLE_IMAGE_VIDEO_ADJUSTMEN, "id", "5"));

    /* OSD */
    col_para = "id INTEGER PRIMARY KEY," \
               "iNormalizedScreenWidth INT DEFAULT 704," \
               "iNormalizedScreenHeight INT DEFAULT 480";
    g_free(rkdb_create(TABLE_NORMALIZED_SCREEN_SIZE, col_para));
    g_free(rkdb_insert(TABLE_NORMALIZED_SCREEN_SIZE, "id", "0"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sType TEXT," \
               "iEnabled INT DEFAULT 0," \
               "iPositionX INT DEFAULT 0," \
               "iPositionY INT DEFAULT 0," \
               "iWidth INT DEFAULT 120," \
               "iHeight INT DEFAULT 92," \
               "sIsPersistentText TEXT DEFAULT 'true'," \
               "sDisplayText TEXT DEFAULT 'test'," \
               "iDisplayWeekEnabled  INT DEFAULT 1," \
               "sDateStyle TEXT DEFAULT 'CHR-YYYY-MM-DD'," \
               "sTimeStyle TEXT DEFAULT '24hour'," \
               "iTransparentColorEnabled INT DEFAULT 0," \
               "sOSDAttribute TEXT DEFAULT 'transparent/flashing'," \
               "sOSDFontSize TEXT DEFAULT '16*16'," \
               "sOSDFrontColorMode TEXT DEFAULT 'auto'," \
               "sOSDFrontColor TEXT DEFAULT '000000'," \
               "sAlignment TEXT DEFAULT 'customize'," \
               "iBoundary INT DEFAULT 0";

    g_free(rkdb_create(TABLE_OSD, col_para));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "0,'channelName'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "1,'dateTime'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "2,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "3,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "4,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "5,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "6,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "7,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "8,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "9,'character'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "10,'privacyMask'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "11,'privacyMask'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "12,'privacyMask'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "13,'privacyMask'"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType", "14,'image'"));

    dbus_manager_init(dbus_conn);
}
