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
    char *col_para = "iID INTEGER PRIMARY KEY," \
                     "sStreamType TEXT DEFAULT 'MainStream'," \
                     "sVideoType TEXT DEFAULT 'CompositeStream'," \
                     "sResolution TEXT DEFAULT '1920*1080P'," \
                     "sRCMode TEXT DEFAULT 'CBR'," \
                     "sImageQuality TEXT DEFAULT 'Medium'," \
                     "sFrameRate TEXT DEFAULT '30'," \
                     "iMaxRate INT DEFAULT 4096," \
                     "sOutputDataType TEXT DEFAULT 'H.264'," \
                     "sSmart TEXT DEFAULT 'Close'," \
                     "sRCQuality TEXT DEFAULT 'High'," \
                     "iGOP INT DEFAULT 30," \
                     "sSVC TEXT DEFAULT 'Close'," \
                     "iStreamSmooth INT DEFAULT 50";

    rkdb_creat(TABLE_VIDEO, col_para);
    /* TODO: Three tables need different initial values */
    rkdb_insert(TABLE_VIDEO, "iID, sStreamType", "0, 'MainStream'");
    rkdb_insert(TABLE_VIDEO, "iID, sStreamType", "1, 'SubStream'");
    rkdb_insert(TABLE_VIDEO, "iID, sStreamType", "2, 'ThirdStream'");

    col_para = "iID INTEGER PRIMARY KEY," \
               "sEncodeType TEXT DEFAULT 'AAC'," \
               "iSampleRate INT DEFAULT 16000," \
               "iBitRate INT DEFAULT 32," \
               "sInput TEXT DEFAULT 'MicIn'," \
               "iVolume INT DEFAULT 50," \
               "sANS TEXT DEFAULT 'Close'";

    rkdb_creat(TABLE_AUDIO, col_para);
    rkdb_insert(TABLE_AUDIO, "iID", "0");

    dbus_manager_init(dbus_conn);
}
