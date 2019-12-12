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
#include "dbus_helpers.h"

static gboolean signal_datachanged(DBusConnection *conn, char *interface, char *json_str)
{
    DBusMessage *signal;
    DBusMessageIter iter;

    if (interface == NULL)
        return FALSE;

    signal = dbus_message_new_signal(DB_PATH,
                                     interface, "DataChanged");
    if (!signal)
        return FALSE;

    dbus_message_iter_init_append(signal, &iter);
    dbus_message_iter_append_basic(&iter, DBUS_TYPE_STRING, &json_str);

    dbus_connection_send(conn, signal, NULL);
    dbus_message_unref(signal);

    return TRUE;
}

static void cmd_delete(json_object *j_cfg)
{
    char *where = 0;
    char *table = 0;
    json_object *j_key = 0;

    table = (char *)json_object_get_string(json_object_object_get(j_cfg, "table"));
    j_key = json_object_object_get(j_cfg, "key");

    json_object_object_foreach(j_key, key, val) {
        if (where) {
            char *tmp = where;
            if (json_object_get_type(val) == json_type_int)
                where = g_strdup_printf("%s and %s=%d", tmp, key, (int)json_object_get_int(val));
            else
                where = g_strdup_printf("%s and %s='%s'", tmp, key, (char *)json_object_get_string(val));
            g_free(tmp);
        } else {
            if (json_object_get_type(val) == json_type_int)
                where = g_strdup_printf("%s=%d", key, (int)json_object_get_int(val));
            else
                where = g_strdup_printf("%s='%s'", key, (char *)json_object_get_string(val));
        }
    }
    rkdb_delete(table, where);
}

static json_object *cmd_select(json_object *j_cfg)
{
    char *col = 0;
    char *where = 0;
    char *table = 0;

    json_object *j_key;
    json_object *j_data;
    json_object *j_array;

    table = (char *)json_object_get_string(json_object_object_get(j_cfg, "table"));
    j_key = json_object_object_get(j_cfg, "key");
    j_data = json_object_object_get(j_cfg, "data");

    json_object_object_foreach(j_key, key, val) {
        if (where) {
            char *tmp = where;
            if (json_object_get_type(val) == json_type_int)
                where = g_strdup_printf("%s and %s=%d", tmp, key, (int)json_object_get_int(val));
            else
                where = g_strdup_printf("%s and %s='%s'", tmp, key, (char *)json_object_get_string(val));
            g_free(tmp);
        } else {
            if (json_object_get_type(val) == json_type_int)
                where = g_strdup_printf("%s=%d", key, (int)json_object_get_int(val));
            else
                where = g_strdup_printf("%s='%s'", key, (char *)json_object_get_string(val));
        }
    }

    col = (char *)json_object_get_string(j_data);
    j_array = rkdb_select(table, col, where, NULL, NULL);
    g_free(where);

    return j_array;
}

static void cmd_update(json_object *j_cfg)
{
    char *cols = 0;
    char *vals = 0;
    int ret;
    char *table = 0;
    json_object *j_key = 0;
    json_object *j_data = 0;

    table = (char *)json_object_get_string(json_object_object_get(j_cfg, "table"));
    j_key = json_object_object_get(j_cfg, "key");
    j_data = json_object_object_get(j_cfg, "data");

    json_object_object_foreach(j_key, key, val) {
        if (cols) {
            char *tmp = cols;
            cols = g_strdup_printf("%s,%s", tmp, key);
            g_free(tmp);
        } else {
            cols = g_strdup_printf("%s", key);
        }

        if (vals) {
            char *tmp = vals;
            if (json_object_get_type(val) == json_type_int)
                vals = g_strdup_printf("%s,%d", tmp, (int)json_object_get_int(val));
            else
                vals = g_strdup_printf("%s,'%s'", tmp, (char *)json_object_get_string(val));
            g_free(tmp);
        } else {
            if (json_object_get_type(val) == json_type_int)
                vals = g_strdup_printf("%d", (int)json_object_get_int(val));
            else
                vals = g_strdup_printf("'%s'", (char *)json_object_get_string(val));
        }
    }

    json_object_object_foreach(j_data, key1, val1) {
        if (cols) {
            char *tmp = cols;
            cols = g_strdup_printf("%s,%s", tmp, key1);
            g_free(tmp);
        } else {
            cols = g_strdup_printf("%s", key1);
        }
        if (vals) {
            char *tmp = vals;
            if (json_object_get_type(val1) == json_type_int)
                vals = g_strdup_printf("%s,%d", tmp, (int)json_object_get_int(val1));
            else
                vals = g_strdup_printf("%s,'%s'", tmp, (char *)json_object_get_string(val1));
            g_free(tmp);
        } else {
            if (json_object_get_type(val1) == json_type_int)
                vals = g_strdup_printf("%d", (int)json_object_get_int(val1));
            else
                vals = g_strdup_printf("'%s'", (char *)json_object_get_string(val1));
        }
    }

    ret = rkdb_insert(table, cols, vals);

    if (cols)
        g_free(cols);
    if (vals)
        g_free(vals);
    cols = 0;
    vals = 0;

    if (ret == -1) {
        char *where = 0;
        char *set = 0;
        json_object_object_foreach(j_key, key, val) {
            if (where) {
                char *tmp = where;
                if (json_object_get_type(val) == json_type_int)
                    where = g_strdup_printf("%s and %s=%d", tmp, key, (int)json_object_get_int(val));
                else
                    where = g_strdup_printf("%s and %s='%s'", tmp, key, (char *)json_object_get_string(val));
                g_free(tmp);
            } else {
                if (json_object_get_type(val) == json_type_int)
                    where = g_strdup_printf("%s=%d", key, (int)json_object_get_int(val));
                else
                    where = g_strdup_printf("%s='%s'", key, (char *)json_object_get_string(val));
            }
        }

        json_object_object_foreach(j_data, key1, val1) {
            if (set) {
                char *tmp = set;
                if (json_object_get_type(val1) == json_type_int)
                    set = g_strdup_printf("%s,%s=%d", tmp, key1, (int)json_object_get_int(val1));
                else
                    set = g_strdup_printf("%s,%s='%s'", tmp, key1, (char *)json_object_get_string(val1));
                g_free(tmp);
            } else {
                if (json_object_get_type(val1) == json_type_int)
                    set = g_strdup_printf("%s=%d", key1, (int)json_object_get_int(val1));
                else
                    set = g_strdup_printf("%s='%s'", key1, (char *)json_object_get_string(val1));
            }
        }

        rkdb_update(table, set, where);

        if (where)
            g_free(where);
        if (set)
            g_free(set);
    }
}

DBusMessage *method_delete(DBusConnection *conn,
                           DBusMessage *msg, void *data)
{
    int ret;
    const char *sender;
    char *json_str;
    json_object *j_cfg;

    sender = dbus_message_get_sender(msg);

    dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &json_str,
                          DBUS_TYPE_INVALID);

    j_cfg = json_tokener_parse(json_str);

    cmd_delete(j_cfg);
    signal_datachanged(conn, data, json_str);

    json_object_put(j_cfg);

    return g_dbus_create_reply(msg, DBUS_TYPE_INVALID);
}

DBusMessage *method_update(DBusConnection *conn,
                           DBusMessage *msg, void *data)
{
    const char *sender;
    char *json_str;
    json_object *j_cfg;

    sender = dbus_message_get_sender(msg);

    dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &json_str,
                          DBUS_TYPE_INVALID);

    j_cfg = json_tokener_parse(json_str);

    cmd_update(j_cfg);
    signal_datachanged(conn, data, json_str);
    json_object_put(j_cfg);

    return g_dbus_create_reply(msg, DBUS_TYPE_INVALID);
}

DBusMessage *method_select(DBusConnection *conn,
                           DBusMessage *msg, void *data)
{
    const char *sender;
    DBusMessage *reply;
    DBusMessageIter array, dict;
    const char *str;
    char *json_str;
    json_object *j_cfg;
    json_object *j_array;

    sender = dbus_message_get_sender(msg);

    dbus_message_get_args(msg, NULL, DBUS_TYPE_STRING, &json_str,
                          DBUS_TYPE_INVALID);

    j_cfg = json_tokener_parse(json_str);

    j_array = cmd_select(j_cfg);

    str = json_object_to_json_string(j_array);

    reply = dbus_message_new_method_return(msg);
    if (!reply)
        return NULL;

    dbus_message_iter_init_append(reply, &array);
    dbus_message_iter_append_basic(&array, DBUS_TYPE_STRING, &str);

    json_object_put(j_array);
    json_object_put(j_cfg);

    return reply;
}

const GDBusMethodTable methods[] = {
    {
        GDBUS_METHOD("Select",
        GDBUS_ARGS({ "json", "s" }), GDBUS_ARGS({ "json", "s" }),
        method_select)
    },
    {
        GDBUS_ASYNC_METHOD("Update",
        GDBUS_ARGS({ "json", "s" }), NULL,
        method_update)
    },
    {
        GDBUS_ASYNC_METHOD("Delete",
        GDBUS_ARGS({ "json", "s" }), NULL,
        method_delete)
    },
    { },
};

const GDBusSignalTable signals[] = {
    {
        GDBUS_SIGNAL("DataChanged",
        GDBUS_ARGS({ "json", "s" }))
    },
    { },
};