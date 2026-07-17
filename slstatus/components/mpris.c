/* See LICENSE file for copyright and license details. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dbus/dbus.h>

#include "../slstatus.h"
#include "../util.h"

static int
get_player_name(DBusConnection *conn, char *player_name, size_t max_len)
{
	DBusMessage *msg;
	DBusMessage *reply;
	DBusError err;
	DBusMessageIter args, array;

	dbus_error_init(&err);

	msg = dbus_message_new_method_call("org.freedesktop.DBus",
	                                   "/org/freedesktop/DBus",
	                                   "org.freedesktop.DBus",
	                                   "ListNames");
	if (!msg)
		return 0;

	reply = dbus_connection_send_with_reply_and_block(conn, msg, 1000, &err);
	dbus_message_unref(msg);

	if (dbus_error_is_set(&err)) {
		dbus_error_free(&err);
		return 0;
	}
	if (!reply)
		return 0;

	if (!dbus_message_iter_init(reply, &args)) {
		dbus_message_unref(reply);
		return 0;
	}

	if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_ARRAY) {
		dbus_message_unref(reply);
		return 0;
	}

	dbus_message_iter_recurse(&args, &array);
	while (dbus_message_iter_get_arg_type(&array) != DBUS_TYPE_INVALID) {
		char *name;
		dbus_message_iter_get_basic(&array, &name);
		if (strncmp(name, "org.mpris.MediaPlayer2.", 23) == 0) {
			strncpy(player_name, name, max_len - 1);
			player_name[max_len - 1] = '\0';
			dbus_message_unref(reply);
			return 1;
		}
		dbus_message_iter_next(&array);
	}
	dbus_message_unref(reply);
	return 0;
}

const char *
mpris(const char *unused)
{
	static DBusConnection *conn = NULL;
	DBusError err;
	char player[256] = {0};
	DBusMessage *msg;
	DBusMessage *reply;
	DBusMessageIter args, variant, array, dict_entry, var_val, artist_array;
	const char *iface = "org.mpris.MediaPlayer2.Player";
	const char *prop = "Metadata";
	char *artist = NULL;
	char *title = NULL;

	dbus_error_init(&err);
	if (!conn || !dbus_connection_get_is_connected(conn)) {
		conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
		if (dbus_error_is_set(&err)) {
			dbus_error_free(&err);
			return NULL;
		}
		if (!conn)
			return NULL;
	}

	if (!get_player_name(conn, player, sizeof(player)))
		return NULL;

	msg = dbus_message_new_method_call(player,
	                                   "/org/mpris/MediaPlayer2",
	                                   "org.freedesktop.DBus.Properties",
	                                   "Get");
	if (!msg)
		return NULL;

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &iface, DBUS_TYPE_STRING, &prop, DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(conn, msg, 1000, &err);
	dbus_message_unref(msg);

	if (dbus_error_is_set(&err)) {
		dbus_error_free(&err);
		return NULL;
	}
	if (!reply)
		return NULL;

	if (!dbus_message_iter_init(reply, &args)) {
		dbus_message_unref(reply);
		return NULL;
	}

	if (dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_VARIANT) {
		dbus_message_unref(reply);
		return NULL;
	}
	dbus_message_iter_recurse(&args, &variant);

	if (dbus_message_iter_get_arg_type(&variant) != DBUS_TYPE_ARRAY) {
		dbus_message_unref(reply);
		return NULL;
	}
	dbus_message_iter_recurse(&variant, &array);

	while (dbus_message_iter_get_arg_type(&array) != DBUS_TYPE_INVALID) {
		char *key;
		dbus_message_iter_recurse(&array, &dict_entry);
		dbus_message_iter_get_basic(&dict_entry, &key);
		dbus_message_iter_next(&dict_entry);

		if (dbus_message_iter_get_arg_type(&dict_entry) == DBUS_TYPE_VARIANT) {
			dbus_message_iter_recurse(&dict_entry, &var_val);
			if (strcmp(key, "xesam:title") == 0 && dbus_message_iter_get_arg_type(&var_val) == DBUS_TYPE_STRING) {
				dbus_message_iter_get_basic(&var_val, &title);
			} else if (strcmp(key, "xesam:artist") == 0 && dbus_message_iter_get_arg_type(&var_val) == DBUS_TYPE_ARRAY) {
				dbus_message_iter_recurse(&var_val, &artist_array);
				if (dbus_message_iter_get_arg_type(&artist_array) == DBUS_TYPE_STRING) {
					dbus_message_iter_get_basic(&artist_array, &artist);
				}
			}
		}
		dbus_message_iter_next(&array);
	}

	if (artist && title)
		bprintf("%s - %s", artist, title);
	else if (title)
		bprintf("%s", title);
	else
		buf[0] = '\0';

	dbus_message_unref(reply);
	
	if (buf[0] == '\0')
		return NULL;
	
	return buf;
}

const char *
mpris_art_url(const char *unused)
{
	static DBusConnection *conn = NULL;
	DBusError err;
	char player[256] = {0};
	DBusMessage *msg;
	DBusMessage *reply;
	DBusMessageIter args, variant, array, dict_entry, var_val;
	const char *iface = "org.mpris.MediaPlayer2.Player";
	const char *prop = "Metadata";
	char *art_url = NULL;

	dbus_error_init(&err);
	if (!conn || !dbus_connection_get_is_connected(conn)) {
		conn = dbus_bus_get(DBUS_BUS_SESSION, &err);
		if (dbus_error_is_set(&err)) {
			dbus_error_free(&err);
			return NULL;
		}
		if (!conn)
			return NULL;
	}

	if (!get_player_name(conn, player, sizeof(player)))
		return NULL;

	msg = dbus_message_new_method_call(player,
	                                   "/org/mpris/MediaPlayer2",
	                                   "org.freedesktop.DBus.Properties",
	                                   "Get");
	if (!msg)
		return NULL;

	dbus_message_append_args(msg, DBUS_TYPE_STRING, &iface, DBUS_TYPE_STRING, &prop, DBUS_TYPE_INVALID);

	reply = dbus_connection_send_with_reply_and_block(conn, msg, 1000, &err);
	dbus_message_unref(msg);

	if (dbus_error_is_set(&err)) {
		dbus_error_free(&err);
		return NULL;
	}
	if (!reply)
		return NULL;

	if (!dbus_message_iter_init(reply, &args) || dbus_message_iter_get_arg_type(&args) != DBUS_TYPE_VARIANT) {
		dbus_message_unref(reply);
		return NULL;
	}
	dbus_message_iter_recurse(&args, &variant);

	if (dbus_message_iter_get_arg_type(&variant) != DBUS_TYPE_ARRAY) {
		dbus_message_unref(reply);
		return NULL;
	}
	dbus_message_iter_recurse(&variant, &array);

	while (dbus_message_iter_get_arg_type(&array) != DBUS_TYPE_INVALID) {
		char *key;
		dbus_message_iter_recurse(&array, &dict_entry);
		dbus_message_iter_get_basic(&dict_entry, &key);
		dbus_message_iter_next(&dict_entry);

		if (dbus_message_iter_get_arg_type(&dict_entry) == DBUS_TYPE_VARIANT) {
			dbus_message_iter_recurse(&dict_entry, &var_val);
			if (strcmp(key, "mpris:artUrl") == 0 && dbus_message_iter_get_arg_type(&var_val) == DBUS_TYPE_STRING) {
				dbus_message_iter_get_basic(&var_val, &art_url);
			}
		}
		dbus_message_iter_next(&array);
	}

	if (art_url) {
		/* Some players like spotify return a URI e.g. file:///path/to/image or http:// */
		/* Often it's a file:// URL, we can optionally strip the prefix but returning as-is is safer */
		if (strncmp(art_url, "file://", 7) == 0)
			bprintf("%s", art_url + 7);
		else
			bprintf("%s", art_url);
	} else {
		buf[0] = '\0';
	}

	dbus_message_unref(reply);
	
	if (buf[0] == '\0')
		return NULL;
	
	return buf;
}

