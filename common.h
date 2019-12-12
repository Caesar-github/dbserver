#ifndef __COMMON_H__
#define __COMMON_H__

#define DB_SERVER   "rockchip.dbserver"
#define DB_PATH      "/"
#define DB_NET_INTERFACE    DB_SERVER ".net"

#define DB_POWER_INTERFACE  DB_SERVER ".power"
#define DB_ETHERNET_INTERFACE   DB_SERVER ".ethernet"
#define DB_WIFI_INTERFACE   DB_SERVER ".wifi"

#define DB_ERROR_INTERFACE      DB_SERVER ".Error"

#define DB_POWER_WIFI_PATH  DB_POWER_INTERFACE ".wifi"
#define DB_POWER_ETHERNET_PATH  DB_POWER_INTERFACE ".ethernet"

#define DB_STORAGE_INTERFACE    DB_SERVER ".storage"

extern const GDBusMethodTable methods[];
extern const GDBusSignalTable signals[];

DBusMessage *method_delete(DBusConnection *conn,
                           DBusMessage *msg, void *data);
DBusMessage *method_update(DBusConnection *conn,
                           DBusMessage *msg, void *data);
DBusMessage *method_select(DBusConnection *conn,
                           DBusMessage *msg, void *data);

#endif