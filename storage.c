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
                     "iCAPACITY INT DEFAULT 0";

    rkdb_creat(TABLE_STORAGE, col_para);

    rkdb_insert(TABLE_STORAGE, "sMOUNTPATH,sSCANPATH,iMODE,iCAPACITY", 
                               "'/mnt/sdcard'," \
                                "'[{\"iCAMID\":0, \"aSCANPATH\":[{\"iPATHID\":0,\"sPATH\":\"video0\",\"sTHUMBPATH\":\"video0/.thumb\",\"iTYPE\":0,\"sFORMAT\":\"VIDEO_%Y%m%d%H%M%S\",\"iDUTY\":10,\"iMAXNUM\":-1,\"iCOUNT\":0}," \
                                                                "{\"iPATHID\":1,\"sPATH\":\"photo0\",\"sTHUMBPATH\":\"photo0/.thumb\",\"iTYPE\":1,\"sFORMAT\":\"PHOTO_%Y%m%d%H%M%S\",\"iDUTY\":10,\"iMAXNUM\":50,\"iCOUNT\":0}]}," \
                                  "{\"iCAMID\":1, \"aSCANPATH\":[{\"iPATHID\":3,\"sPATH\":\"video1\",\"sTHUMBPATH\":\"video1/.thumb\",\"iTYPE\":0,\"sFORMAT\":\"VIDEO_CCCCCC\",\"iDUTY\":10,\"iMAXNUM\":-1,\"iCOUNT\":0}," \
                                                                "{\"iPATHID\":4,\"sPATH\":\"photo1\",\"sTHUMBPATH\":\"photo1/.thumb\",\"iTYPE\":1,\"sFORMAT\":\"PHOTO_CCCCCC\",\"iDUTY\":10,\"iMAXNUM\":50,\"iCOUNT\":0}]}" \
                                 "]'," \
                               "0,500");
    rkdb_insert(TABLE_STORAGE, "sMOUNTPATH,sSCANPATH,iMODE,iCAPACITY", 
                               "'/media/usb0'," \
                                "'[{\"iCAMID\":0, \"aSCANPATH\":[{\"iPATHID\":0,\"sPATH\":\"video0\",\"sTHUMBPATH\":\"video0/.thumb\",\"iTYPE\":0,\"sFORMAT\":\"VIDEO_%Y%m%d%H%M%S\",\"iDUTY\":10,\"iMAXNUM\":-1,\"iCOUNT\":0}," \
                                                                "{\"iPATHID\":1,\"sPATH\":\"photo0\",\"sTHUMBPATH\":\"photo0/.thumb\",\"iTYPE\":1,\"sFORMAT\":\"PHOTO_%Y%m%d%H%M%S\",\"iDUTY\":10,\"iMAXNUM\":50,\"iCOUNT\":0}]}," \
                                  "{\"iCAMID\":1, \"aSCANPATH\":[{\"iPATHID\":3,\"sPATH\":\"video1\",\"sTHUMBPATH\":\"video1/.thumb\",\"iTYPE\":0,\"sFORMAT\":\"VIDEO_CCCCCC\",\"iDUTY\":10,\"iMAXNUM\":-1,\"iCOUNT\":0}," \
                                                                "{\"iPATHID\":4,\"sPATH\":\"photo1\",\"sTHUMBPATH\":\"photo1/.thumb\",\"iTYPE\":1,\"sFORMAT\":\"PHOTO_CCCCCC\",\"iDUTY\":10,\"iMAXNUM\":50,\"iCOUNT\":0}]}" \
                                 "]'," \
                               "0,500");

    col_para = "iID INTEGER PRIMARY KEY," \
               "iVIDEOLEN INT DEFAULT -1," \
               "iVIDEOMAXSIZE INT DEFAULT -1," \
               "sMOUNTPATH TEXT NOT NULL UNIQUE";
    rkdb_creat(TABLE_STORAGECONFIG, col_para);
    rkdb_insert(TABLE_STORAGECONFIG, "iID,iVIDEOLEN,iVIDEOMAXSIZE,sMOUNTPATH","0,-1,-1,'/media/usb0'");
    
    dbus_manager_init(dbus_conn);
}
