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

#include <gcontactsync_debug.h>

static void plugin_debug_helper(const char *category, char *data, size_t size) {
	char *text;

	text = strndup(data, size);
	if (!text)
		return;

	if (text[size-1] == '\n')
		purple_debug_misc(category, "%s", text);
	else
		purple_debug_misc(category, "%s\n", text);

	free(text);
}

int plugin_gcal_debug_callback(void *handle, char *data, size_t size) {
	plugin_debug_helper("gcal", data, size);

	return 0;
}

int plugin_curl_debug_callback(void *handle, int type, char *data, size_t size, void *userptr) {
	if (type)
		return 0;

	plugin_debug_helper("curl", data, size);

	return 0;
}
