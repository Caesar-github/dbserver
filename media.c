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
                     "sSTREAM_TYPE TEXT DEFAULT 'main'," \
                     "sVIDEO_TYPE TEXT DEFAULT 'complex'," \
                     "sRESOLUTION TEXT DEFAULT '1920*1080P'," \
                     "sRC_MODE TEXT DEFAULT 'cbr'," \
                     "sIMAGE_QUALITY TEXT DEFAULT 'middle'," \
                     "iFRAME_RATE INT DEFAULT 30," \
                     "iMAX_RATE INT DEFAULT 4096," \
                     "sOUTPUT_DATA_TYPE TEXT DEFAULT 'video:h264'," \
                     "sSMART264 TEXT DEFAULT 'close'," \
                     "sRC_QUALITY TEXT DEFAULT 'high'," \
                     "iGOP INT DEFAULT 30," \
                     "sSVC TEXT DEFAULT 'close'," \
                     "iSTREAM_SMOOTH INT DEFAULT 50";

    rkdb_creat(TABLE_VIDEO, col_para);
    /* TODO: Three tables need different initial values */
    rkdb_insert(TABLE_VIDEO, "iID, sSTREAM_TYPE", "0, 'main'");
    rkdb_insert(TABLE_VIDEO, "iID, sSTREAM_TYPE", "1, 'sub'");
    rkdb_insert(TABLE_VIDEO, "iID, sSTREAM_TYPE", "2, 'stream3'");

    col_para = "iID INTEGER PRIMARY KEY," \
               "sENCODE_TYPE TEXT DEFAULT 'AAC'," \
               "iSAMPLE_RATE INT DEFAULT 16000," \
               "iBIT_RATE INT DEFAULT 32," \
               "sINPUT TEXT DEFAULT 'MicIn'," \
               "iVOLUME INT DEFAULT 50," \
               "sANS TEXT DEFAULT 'close'";

    rkdb_creat(TABLE_AUDIO, col_para);
    rkdb_insert(TABLE_AUDIO, "iID", "0");

    dbus_manager_init(dbus_conn);
}
