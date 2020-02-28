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

#define TABLE_NETWORK_IP            "NetworkIP"
#define TABLE_NETWORK_SERVICE       "NetworkService"
#define TABLE_NETWORK_POWER         "NetworkPower"
#define TABLE_NETWORK_NTP           "ntp"
#define TABLE_NETWORK_ZONE          "zone"
#define TABLE_NETWORK_PORT          "port"
#define TABLE_NETWORK_VERSION       "NetworkVersion"

#define NETWORK_VERSION             "1.0.2"

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
    char *col_para;

    if (equal_version(TABLE_NETWORK_VERSION, NETWORK_VERSION)) {
        dbus_manager_init(dbus_conn);
        return;
    }

    g_free(rkdb_drop(TABLE_NETWORK_IP));
    g_free(rkdb_drop(TABLE_NETWORK_SERVICE));
    g_free(rkdb_drop(TABLE_NETWORK_POWER));
    g_free(rkdb_drop(TABLE_NETWORK_NTP));
    g_free(rkdb_drop(TABLE_NETWORK_ZONE));
    g_free(rkdb_drop(TABLE_NETWORK_PORT));
    g_free(rkdb_drop(TABLE_NETWORK_VERSION));

    creat_version_table(TABLE_NETWORK_VERSION, NETWORK_VERSION);

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sService TEXT NOT NULL UNIQUE," \
               "sPassword TEXT DEFAULT ''," \
               "iFavorite INT DEFAULT 0," \
               "iAutoconnect INT DEFAULT 0";

    g_free(rkdb_create(TABLE_NETWORK_SERVICE, col_para));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sInterface TEXT NOT NULL UNIQUE," \
               "sType TEXT NOT NULL," \
               "sV4Method TEXT DEFAULT ''," \
               "sV4Address TEXT DEFAULT ''," \
               "sV4Netmask TEXT DEFAULT ''," \
               "sV4Gateway TEXT DEFAULT ''," \
               "sV6Method TEXT DEFAULT ''," \
               "sV6Address TEXT DEFAULT ''," \
               "sV6Netmask TEXT DEFAULT ''," \
               "sV6Gateway TEXT DEFAULT ''," \
               "sDNS1 TEXT DEFAULT ''," \
               "sDNS2 TEXT DEFAULT ''";

    g_free(rkdb_create(TABLE_NETWORK_IP, col_para));
    g_free(rkdb_insert(TABLE_NETWORK_IP, "sInterface,sType,sV4Method", "'wlan0','wifi', 'dhcp'"));
    g_free(rkdb_insert(TABLE_NETWORK_IP, "sInterface,sType,sV4Method", "'eth0','ethernet', 'dhcp'"));

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sType TEXT NOT NULL UNIQUE," \
               "iPower INT DEFAULT 0";

    g_free(rkdb_create(TABLE_NETWORK_POWER, col_para));
    g_free(rkdb_insert(TABLE_NETWORK_POWER, "sType,iPower", "'wifi',0"));
    g_free(rkdb_insert(TABLE_NETWORK_POWER, "sType,iPower", "'ethernet',1"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sNtpServers TEXT NOT NULL," \
               "sTimeZone TEXT NOT NULL," \
               "iAutoMode INT DEFAULT 0," \
               "iRefreshTime INT DEFAULT 120";
    g_free(rkdb_create(TABLE_NETWORK_NTP, col_para));
    g_free(rkdb_insert(TABLE_NETWORK_NTP, "id,sNtpServers,sTimeZone,iAutoMode,iRefreshTime", "0,'122.224.9.29 94.130.49.186','CST',0,120"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sName TEXT NOT NULL," \
               "iHavedst INT DEFAULT 0," \
               "sTimeZone TEXT NOT NULL," \
               "sTimeZoneDst TEXT NOT NULL";
    g_free(rkdb_create(TABLE_NETWORK_ZONE, col_para));
    g_free(rkdb_insert(TABLE_NETWORK_ZONE, "id,sName,iHavedst,sTimeZone,sTimeZoneDst", "0,'Shanghai',0,'GMT-8',''"));
    g_free(rkdb_insert(TABLE_NETWORK_ZONE, "id,sName,iHavedst,sTimeZone,sTimeZoneDst", "1,'Los_Angeles',1,'GMT+8','Los_Angeles'"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sProtocol TEXT NOT NULL," \
               "iPortNo INT DEFAULT 0";
    g_free(rkdb_create(TABLE_NETWORK_PORT, col_para));
    g_free(rkdb_insert(TABLE_NETWORK_PORT, "id,sProtocol,iPortNo", "0,'HTTP',80"));
    g_free(rkdb_insert(TABLE_NETWORK_PORT, "id,sProtocol,iPortNo", "1,'HTTPS',443"));
    g_free(rkdb_insert(TABLE_NETWORK_PORT, "id,sProtocol,iPortNo", "2,'DEV_MANAGE',8080"));
    g_free(rkdb_insert(TABLE_NETWORK_PORT, "id,sProtocol,iPortNo", "3,'RTSP',554"));
    g_free(rkdb_insert(TABLE_NETWORK_PORT, "id,sProtocol,iPortNo", "4,'RTMP',1935"));

    dbus_manager_init(dbus_conn);
}
