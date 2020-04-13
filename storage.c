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
#define TABLE_STORAGE_VERSION       "StorageVersion"

#define TYPE_VIDEO         0
#define TYPE_PHOTO         1
#define TYPE_BLACK_LIST    2
#define TYPE_SNAPSHOT      3
#define TYPE_WHITE_LIST    4

#define STORAGE_VERSION             "1.0.2"

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

    if (equal_version(TABLE_STORAGE_VERSION, STORAGE_VERSION)) {
        dbus_manager_init(dbus_conn);
        return;
    }

    g_free(rkdb_drop(TABLE_STORAGE_DISK_PATH));
    g_free(rkdb_drop(TABLE_STORAGE_MEDIA_FOLDER));
    g_free(rkdb_drop(TABLE_STORAGE_CONFIG));
    g_free(rkdb_drop(TABLE_STORAGE_VERSION));

    creat_version_table(TABLE_STORAGE_VERSION, STORAGE_VERSION);

    col_para = "id INTEGER PRIMARY KEY AUTOINCREMENT," \
               "sPath TEXT NOT NULL UNIQUE," \
               "sName TEXT DEFAULT ''," \
               "iMount INT DEFAULT 0";

    g_free(rkdb_create(TABLE_STORAGE_DISK_PATH, col_para));
    g_free(rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/mnt/sdcard','SD Card',0"));
    g_free(rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/media/usb0','U Disk',0"));
    g_free(rkdb_insert(TABLE_STORAGE_DISK_PATH, "sPath,sName,iMount", "'/userdata','Emmc',0"));

    col_para = "id INTEGER PRIMARY KEY," \
               "sMediaFolder TEXT NOT NULL UNIQUE," \
               "sThumbFolder TEXT," \
               "sFormat TEXT," \
               "iCamId INT," \
               "iType INT," \
               "iDuty INT," \
               "iMaxNum INT";

    g_free(rkdb_create(TABLE_STORAGE_MEDIA_FOLDER, col_para));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "0,'video0','video0/.thumb','VIDEO_%Y%m%d%H%M%S',0,0,45,-1"));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "1,'photo0','photo0/.thumb','PHOTO_%Y%m%d%H%M%S',0,1,5,-1"));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "2,'video1','video1/.thumb','VIDEO_%Y%m%d%H%M%S',1,0,45,-1"));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "3,'photo1','photo1/.thumb','PHOTO_%Y%m%d%H%M%S',1,1,5,-1"));

    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "4,'black_list','','%Y%m%d%H%M%S',0,2,0,-1"));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "5,'snapshot','','%Y%m%d%H%M%S',0,3,0,-1"));
    g_free(rkdb_insert(TABLE_STORAGE_MEDIA_FOLDER, "id,sMediaFolder,sThumbFolder,sFormat,iCamId,iType,iDuty,iMaxNum",
                                          "6,'white_list','','%Y%m%d%H%M%S',0,4,0,-1"));

    col_para = "id INTEGER PRIMARY KEY," \
               "iFreeSize INT DEFAULT -1," \
               "sMountPath TEXT NOT NULL UNIQUE";
    g_free(rkdb_create(TABLE_STORAGE_CONFIG, col_para));
    //The unit of iFreeSize is MB
    g_free(rkdb_insert(TABLE_STORAGE_CONFIG, "id,iFreeSize,sMountPath","0,1024,'/userdata'"));

    dbus_manager_init(dbus_conn);
}
