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
#define TABLE_VIDEO_ADVANCED_ENC "video_advanced_enc"
#define TABLE_VIDEO_REGION_CLIP "video_region_clip"
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

#define TABLE_NORMALIZED_SCREEN_SIZE "normalized_screen_size"
#define TABLE_OSD "osd"
#define TABLE_ROI "roi"

#define TABLE_PROFILE                       "profile"
#define TABLE_VIDEO_SOURCE_CONFIGURATION    "video_source_configuration"

#define TABLE_MEDIA_VERSION       "MediaVersion"

#define MEDIA_VERSION             "1.0.1"

int media_dbus_register(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_MEDIA_INTERFACE,
                              methods,
                              signals, NULL, DB_MEDIA_INTERFACE, NULL);

    return 0;
}

void media_init(void)
{
    char *col_para;

    if (equal_version(TABLE_MEDIA_VERSION, MEDIA_VERSION))
        return;

    g_free(rkdb_drop(TABLE_VIDEO));
    g_free(rkdb_drop(TABLE_VIDEO_ADVANCED_ENC));
    g_free(rkdb_drop(TABLE_VIDEO_REGION_CLIP));
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
    g_free(rkdb_drop(TABLE_NORMALIZED_SCREEN_SIZE));
    g_free(rkdb_drop(TABLE_OSD));
    g_free(rkdb_drop(TABLE_ROI));
    g_free(rkdb_drop(TABLE_PROFILE));
    g_free(rkdb_drop(TABLE_VIDEO_SOURCE_CONFIGURATION));
    g_free(rkdb_drop(TABLE_MEDIA_VERSION));

    creat_version_table(TABLE_MEDIA_VERSION, MEDIA_VERSION);

    col_para = "id INTEGER PRIMARY KEY," \
               "sStreamType TEXT DEFAULT 'mainStream'," \
               "sVideoType TEXT DEFAULT 'compositeStream'," \
               "sResolution TEXT DEFAULT '2688*1520'," \
               "sRCMode TEXT DEFAULT 'CBR'," \
               "sImageQuality TEXT DEFAULT 'medium'," \
               "sFrameRate TEXT DEFAULT '25'," \
               "iTargetRate INT DEFAULT 0," \
               "iMaxRate INT DEFAULT 4096," \
               "iMinRate INT DEFAULT 0," \
               "sOutputDataType TEXT DEFAULT 'H.264'," \
               "sSmart TEXT DEFAULT 'close'," \
               "sRCQuality TEXT DEFAULT 'high'," \
               "iGOP INT DEFAULT 50," \
               "sSVC TEXT DEFAULT 'close'," \
               "iStreamSmooth INT DEFAULT 50,"\
               "sVideoSourceToken TEXT DEFAULT 'mainVSToken'," \
               "sVideoEncoderConfigurationToken TEXT DEFAULT 'mainVECToken'," \
               "sVideoEncoderConfigurationName TEXT DEFAULT 'mainVECName'";
    g_free(rkdb_create(TABLE_VIDEO, col_para));
    /* TODO: Three tables need different initial values */
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType, sResolution, iMaxRate, sOutputDataType, sVideoSourceToken, sVideoEncoderConfigurationToken, sVideoEncoderConfigurationName",
                                    "0, 'mainStream', '2688*1520', 8192, 'H.265', 'mainVSToken', 'mainVECToken', 'mainVECName'"));
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType, sResolution, iMaxRate, sVideoSourceToken, sVideoEncoderConfigurationToken, sVideoEncoderConfigurationName",
                                    "1, 'subStream', '640*480', 1024, 'subVSToken', 'subVECToken', 'subVECName'"));
    g_free(rkdb_insert(TABLE_VIDEO, "id, sStreamType, sResolution, iMaxRate, sOutputDataType, sVideoSourceToken, sVideoEncoderConfigurationToken, sVideoEncoderConfigurationName",
                                    "2, 'thirdStream', '1920*1080', 2048, 'H.265', 'thirdVSToken', 'thirdVECToken', 'thirdVECName'"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sStreamType TEXT DEFAULT 'mainStream'," \
               "sFunction TEXT DEFAULT '',"\
               "sParameters TEXT DEFAULT ''";
    g_free(rkdb_create(TABLE_VIDEO_ADVANCED_ENC, col_para));
    g_free(rkdb_insert(TABLE_VIDEO_ADVANCED_ENC, "id, sStreamType, sFunction, sParameters", \
            "0, 'mainStream', 'qp', '{\"qp_init\":24,\"qp_step\":4,\"qp_min\":12,\"qp_max\":48,\"min_i_qp\":10,\"max_i_qp\":20}'"));
    g_free(rkdb_insert(TABLE_VIDEO_ADVANCED_ENC, "id, sStreamType, sFunction, sParameters", \
            "1, 'mainStream', 'split', '{\"mode\":0,\"size\":1024}'"));

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
               "sHDR TEXT DEFAULT 'open'," \
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
               "sGrayScaleMode TEXT DEFAULT '[0-255]'," \
               "sFEC TEXT DEFAULT 'close'";
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
               "iWidth INT DEFAULT 0," \
               "iHeight INT DEFAULT 0," \
               "sIsPersistentText TEXT DEFAULT 'true'," \
               "sDisplayText TEXT DEFAULT ''," \
               "iDisplayWeekEnabled INT DEFAULT 1," \
               "sDateStyle TEXT DEFAULT ''," \
               "sTimeStyle TEXT DEFAULT ''," \
               "iTransparentColorEnabled INT DEFAULT 0," \
               "sOSDAttribute TEXT DEFAULT 'transparent/not-flashing'," \
               "sOSDFontSize TEXT DEFAULT '32*32'," \
               "sOSDFrontColorMode TEXT DEFAULT 'customize'," \
               "sOSDFrontColor TEXT DEFAULT 'fff799'," \
               "sAlignment TEXT DEFAULT 'customize'," \
               "iBoundary INT DEFAULT 0";
    g_free(rkdb_create(TABLE_OSD, col_para));
    g_free(rkdb_insert(TABLE_OSD, "id,sType,iEnabled,sDisplayText,iPositionX,iPositionY", "0,'channelName',1,'Camera 01',560,432"));
    g_free(rkdb_insert(TABLE_OSD, "id,sType,iEnabled,sDateStyle,sTimeStyle,iPositionX,iPositionY", "1,'dateTime',1,'CHR-YYYY-MM-DD','24hour',16,16"));
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
    g_free(rkdb_insert(TABLE_OSD, "id,sType,iEnabled,iPositionX,iPositionY", "14,'image',1,16,388"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sStreamType TEXT," \
               "iStreamEnabled INT DEFAULT 0," \
               "iROIId INT," \
               "iROIEnabled INT DEFAULT 0," \
               "sName TEXT DEFAULT 'test'," \
               "iQualityLevelOfROI INT DEFAULT 3," \
               "iPositionX INT DEFAULT 0," \
               "iPositionY INT DEFAULT 0," \
               "iWidth INT DEFAULT 0," \
               "iHeight INT DEFAULT 0";
    g_free(rkdb_create(TABLE_ROI, col_para));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "0,'mainStream',1"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "1,'mainStream',2"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "2,'mainStream',3"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "3,'mainStream',4"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "4,'subStream',1"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "5,'subStream',2"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "6,'subStream',3"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "7,'subStream',4"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "8,'thirdStream',1"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "9,'thirdStream',2"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "10,'thirdStream',3"));
    g_free(rkdb_insert(TABLE_ROI, "id,sStreamType,iROIId", "11,'thirdStream',4"));

    col_para = "id INTEGER PRIMARY KEY," \
               "iRegionClipEnabled INT DEFAULT 0," \
               "iPositionX INT DEFAULT 0," \
               "iPositionY INT DEFAULT 0," \
               "iWidth INT DEFAULT 640," \
               "iHeight INT DEFAULT 480";
    g_free(rkdb_create(TABLE_VIDEO_REGION_CLIP, col_para));
    g_free(rkdb_insert(TABLE_VIDEO_REGION_CLIP, "id", "0"));

    col_para = "sToken TEXT PRIMARY KEY," \
               "sName TEXT," \
               "iFixed INT DEFAULT 0," \
               "sVideoSourceConfigurationToken TEXT DEFAULT ''," \
               "sAudioSourceConfigurationToken TEXT DEFAULT ''," \
               "sVideoEncoderConfigurationToken TEXT DEFAULT ''," \
               "sAudioEncoderConfigurationToken TEXT DEFAULT ''," \
               "sVideoAnalyticsConfigurationToken TEXT DEFAULT ''," \
               "sPTZConfigurationToken TEXT DEFAULT ''," \
               "sMetadataConfigurationToken TEXT DEFAULT ''," \
               "sAudioOutputConfigurationToken TEXT DEFAULT ''," \
               "sAudioDecoderConfigurationToken TEXT DEFAULT ''";
    g_free(rkdb_create(TABLE_PROFILE, col_para));
    g_free(rkdb_insert(TABLE_PROFILE, "sToken, sName, sVideoSourceConfigurationToken, sVideoEncoderConfigurationToken",
                       "'mainProfileToken', 'mainProfileName', 'mainVSCToken', 'mainVECToken'"));
    g_free(rkdb_insert(TABLE_PROFILE, "sToken, sName, sVideoSourceConfigurationToken, sVideoEncoderConfigurationToken",
                       "'subProfileToken', 'subProfileName', 'subVSCToken', 'subVECToken'"));
    g_free(rkdb_insert(TABLE_PROFILE, "sToken, sName, sVideoSourceConfigurationToken, sVideoEncoderConfigurationToken",
                       "'thirdProfileToken', 'thirdProfileName', 'thirdVSCToken', 'thirdVECToken'"));

    col_para = "sToken TEXT PRIMARY KEY," \
               "sName TEXT DEFAULT ''," \
               "sSourceToken TEXT DEFAULT ''";
    g_free(rkdb_create(TABLE_VIDEO_SOURCE_CONFIGURATION, col_para));
    g_free(rkdb_insert(TABLE_VIDEO_SOURCE_CONFIGURATION, "sToken, sName, sSourceToken", "'mainVSCToken', 'mainVSCName', 'mainVSToken'"));
    g_free(rkdb_insert(TABLE_VIDEO_SOURCE_CONFIGURATION, "sToken, sName, sSourceToken", "'subVSCToken', 'subVSCName', 'subVSToken'"));
    g_free(rkdb_insert(TABLE_VIDEO_SOURCE_CONFIGURATION, "sToken, sName, sSourceToken", "'thirdVSCToken', 'thirdVSCName', 'thirdVSToken'"));
}
