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

#define TABLE_STORAGE  "storage"
#define TABLE_STORAGECONFIG  "storageconfig"

static int dbus_manager_init(DBusConnection *dbus_conn)
{
    g_dbus_register_interface(dbus_conn, "/",
                              DB_STORAGE_INTERFACE,
                              methods,
                              signals, NULL, DB_STORAGE_INTERFACE, NULL);

    return 0;
}

void storage_init(DBusConnection *dbus_conn)
{
    char *col_para = "iROOTID INTEGER PRIMARY KEY AUTOINCREMENT," \
                     "sMOUNTPATH TEXT NOT NULL UNIQUE," \
                     "sSCANPATH TEXT DEFAULT ''," \
                     "iMODE INT DEFAULT 0," \
                     "iCAPACITY INT DEFAULT 0);";

    rkdb_creat(TABLE_STORAGE, col_para);

    rkdb_insert(TABLE_STORAGE, "sMOUNTPATH,sSCANPATH,iMODE,iCAPACITY", "'/mnt/sdcard','[{\"iPATHID\":0,\"sPATH\":\"video\"},{\"iPATHID\":1,\"sPATH\":\"photo\"},{\"iPATHID\":2,\"sPATH\":\"videothumb\"},{\"iPATHID\":3,\"sPATH\":\"photothumb\"}]',0,500");
    rkdb_insert(TABLE_STORAGE, "sMOUNTPATH,sSCANPATH,iMODE,iCAPACITY", "'/media/usb0','[{\"iPATHID\":0,\"sPATH\":\"video\"},{\"iPATHID\":1,\"sPATH\":\"photo\"},{\"iPATHID\":2,\"sPATH\":\"videothumb\"},{\"iPATHID\":3,\"sPATH\":\"photothumb\"}]',0,500");
    rkdb_select(TABLE_STORAGE, NULL, NULL, NULL, NULL);

    col_para = "iID INTEGER PRIMARY KEY," \
               "sMOUNTPATH TEXT NOT NULL UNIQUE);";
    rkdb_creat(TABLE_STORAGECONFIG, col_para);
    rkdb_insert(TABLE_STORAGECONFIG, "iID,sMOUNTPATH","0,'/media/usb0'");
    rkdb_select(TABLE_STORAGECONFIG, NULL, NULL, NULL, NULL);
    
    dbus_manager_init(dbus_conn);
}
