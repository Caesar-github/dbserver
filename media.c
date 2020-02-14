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
    char *col_para = "id INTEGER PRIMARY KEY," \
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

    rkdb_creat(TABLE_VIDEO, col_para);
    /* TODO: Three tables need different initial values */
    rkdb_insert(TABLE_VIDEO, "id, sStreamType", "0, 'mainStream'");
    rkdb_insert(TABLE_VIDEO, "id, sStreamType", "1, 'subStream'");
    rkdb_insert(TABLE_VIDEO, "id, sStreamType", "2, 'thirdStream'");

    col_para = "id INTEGER PRIMARY KEY," \
               "sEncodeType TEXT DEFAULT 'AAC'," \
               "iSampleRate INT DEFAULT 16000," \
               "iBitRate INT DEFAULT 32," \
               "sInput TEXT DEFAULT 'micIn'," \
               "iVolume INT DEFAULT 50," \
               "sANS TEXT DEFAULT 'close'";

    rkdb_creat(TABLE_AUDIO, col_para);
    rkdb_insert(TABLE_AUDIO, "id", "0");

    col_para = "id INTEGER PRIMARY KEY," \
               "sStreamProtocol TEXT NOT NULL";

    rkdb_creat(TABLE_STREAM_URL, col_para);
    rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "0,'RTSP'");
    rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "1,'RTMP'");
    rkdb_insert(TABLE_STREAM_URL, "id,sStreamProtocol", "2,'RTMP'");

    dbus_manager_init(dbus_conn);
}
