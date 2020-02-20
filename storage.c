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

#define TABLE_STORAGE_DISK_PATH     "StorageDiskPath"
#define TABLE_STORAGE_MEDIA_FOLDER  "StorageMediaFolder"
#define TABLE_STORAGE_CONFIG        "StorageConfig"

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
    char *col_para;

    col_para = "iId INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sPath TEXT NOT NULL UNIQUE," \
               "sName TEXT DEFAULT ''," \
               "iMount INT DEFAULT 0";

    rkdb_creat(TABLE_STORAGE_DISK_PATH, col_para);
    rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/mnt/sdcard','SD Card',0");
    rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/media/usb0','U Disk',0");
    rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/userdata','Emmc',0");

    col_para = "iId INTEGER PRIMARY KEY," \
               "sMediaFolder TEXT NOT NULL UNIQUE," \
               "sThumbFolder TEXT," \
               "sFormat TEXT," \
               "iCamId INT," \
               "iType INT," \
               "iDuty INT," \
               "iMaxNum INT";

    rkdb_creat(TABLE_STORAGE_MEDIA_FOLDER, col_para);
    rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "iId,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "0,'video0','video0/.thumb','VIDEO_%Y%m%d%H%M%S',0,0,90,-1");
    rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "iId,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "1,'photo0','photo0/.thumb','PHOTO_%Y%m%d%H%M%S',0,1,10,-1");

    col_para = "iId INTEGER PRIMARY KEY," \
               "iFreeSize INT DEFAULT -1," \
               "sMountPath TEXT NOT NULL UNIQUE";
    rkdb_creat(TABLE_STORAGE_CONFIG, col_para);
    //The unit of iFreeSize is MB
    rkdb_insert(TABLE_STORAGE_CONFIG, "iId,iFreeSize,sMountPath","0,1024,'/userdata'");

    dbus_manager_init(dbus_conn);
}
