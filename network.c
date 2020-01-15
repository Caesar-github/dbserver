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

static int dbus_manager_init(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_NET_INTERFACE,
                              methods,
                              signals, NULL, DB_NET_INTERFACE, NULL);

    return 0;
}

void network_init(DBusConnection *dbus_conn)
{
    char *col_para = "iID INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "sSERVICE TEXT NOT NULL UNIQUE," \
                     "sPASSWORD TEXT DEFAULT ''," \
                     "iFAVORITE INT DEFAULT 0," \
                     "iAUTOCONNECT INT DEFAULT 0," \
                     "sV4METHOD TEXT DEFAULT ''," \
                     "sV4ADDRESS TEXT DEFAULT ''," \
                     "sV4NETMASK TEXT DEFAULT ''," \
                     "sV4GATEWAY TEXT DEFAULT ''," \
                     "sNAMESERVERS TEXT DEFAULT ''";

    rkdb_creat("netconfig", col_para);
    rkdb_insert("netconfig", "sSERVICE", "'service test'");

    col_para = "iID INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sNAME TEXT NOT NULL UNIQUE," \
               "iPOWER INT DEFAULT 0";

    rkdb_creat("power", col_para);
    rkdb_insert("power", "sNAME,iPOWER", "'wifi',0");
    rkdb_insert("power", "sNAME,iPOWER", "'ethernet',1");

    col_para = "iID INTEGER PRIMARY KEY," \
               "sSERVERS TEXT NOT NULL," \
               "sZONE TEXT NOT NULL," \
               "iAUTO INT DEFAULT 0," \
               "iTIME INT DEFAULT 120";
    rkdb_creat("ntp", col_para);
    rkdb_insert("ntp", "iID,sSERVERS,sZONE,iAUTO,iTIME", "0,'122.224.9.29 94.130.49.186','CST',0,120");

    col_para = "iID INTEGER PRIMARY KEY," \
               "sName TEXT NOT NULL," \
               "iHavedst INT DEFAULT 0," \
               "sZONE TEXT NOT NULL," \
               "sZONEdst TEXT NOT NULL";
    rkdb_creat("zone", col_para);
    rkdb_insert("zone", "iID,sName,iHavedst,sZONE,sZONEdst", "0,'Shanghai',0,'GMT-8',''");
    rkdb_insert("zone", "iID,sName,iHavedst,sZONE,sZONEdst", "1,'Los_Angeles',1,'GMT+8','Los_Angeles'");

    dbus_manager_init(dbus_conn);
}
