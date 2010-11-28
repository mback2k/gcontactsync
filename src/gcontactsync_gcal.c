/*
 * Google Contact Sync Plugin
 *
 * Copyright (C) 2010, Marc Hörsken <info@marc-hoersken.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02111-1301, USA.
 */

#include <gcontactsync_gcal.h>

gboolean plugin_check_gcal(int result, const char* function) {
	purple_debug_misc(PLUGIN_ID, "%s result = %d\n", function, result);
	if (result != 0) {
		if (gcontactsync_gcal) {
			purple_debug_error(PLUGIN_ID, "%s: %d\n%s\n", function, gcal_status_httpcode(gcontactsync_gcal), gcal_status_msg(gcontactsync_gcal));
		}
		return FALSE;
	}
	return TRUE;
}

gboolean plugin_check_protocol(const char* protocol_id) {
	return (strcmp(protocol_id, "prpl-aim") == 0)
	    || (strcmp(protocol_id, "prpl-msn") == 0)
	    || (strcmp(protocol_id, "prpl-yahoo") == 0)
	    || (strcmp(protocol_id, "prpl-qq") == 0)
	    || (strcmp(protocol_id, "prpl-icq") == 0)
	    || (strcmp(protocol_id, "prpl-jabber") == 0);
}

gboolean plugin_compare_protocols(const char* protocol_id, const char* protocol) {
	return ((strcmp(protocol_id, "prpl-aim") == 0) && (strcmp(protocol, "AIM") == 0))
	    || ((strcmp(protocol_id, "prpl-msn") == 0) && (strcmp(protocol, "MSN") == 0))
	    || ((strcmp(protocol_id, "prpl-yahoo") == 0) && (strcmp(protocol, "YAHOO") == 0))
	    || ((strcmp(protocol_id, "prpl-qq") == 0) && (strcmp(protocol, "QQ") == 0))
	    || ((strcmp(protocol_id, "prpl-icq") == 0) && (strcmp(protocol, "ICQ") == 0))
	    || ((strcmp(protocol_id, "prpl-jabber") == 0) && (strcmp(protocol, "JABBER") == 0))
	    || ((strcmp(protocol_id, "prpl-jabber") == 0) && (strcmp(protocol, "GOOGLE_TALK") == 0));
}

const char* plugin_get_protocol(const char* protocol_id, const char* buddy_name) {
	if (strcmp(protocol_id, "prpl-aim") == 0)
		return "AIM";

	if (strcmp(protocol_id, "prpl-msn") == 0)
		return "MSN";

	if (strcmp(protocol_id, "prpl-yahoo") == 0)
		return "YAHOO";

	if (strcmp(protocol_id, "prpl-qq") == 0)
		return "QQ";

	if (strcmp(protocol_id, "prpl-icq") == 0)
		return "ICQ";

	if (strcmp(protocol_id, "prpl-jabber") == 0 && (strstr(buddy_name, "@gmail.com") || strstr(buddy_name, "@googlemail.com")))
		return "GOOGLE_TALK";

	if (strcmp(protocol_id, "prpl-jabber") == 0)
		return "JABBER";

	return NULL;
}
