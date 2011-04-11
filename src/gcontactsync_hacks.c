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

#include <gcontactsync_hacks.h>

int plugin_curl_debug_callback(CURL *handle, curl_infotype type, char *data, size_t size, void *userptr) {
	char *text;

	if (type != CURLINFO_TEXT)
		return 0;

	text = strdup(data);
	if (!text)
		return 0;

	if (strlen(text) >= size) {
		text[size] = '\0';
		purple_debug_misc("libcurl", "%s", text);
	}

	free(text);

	return 0;	
}

// WORKAROUND: Forward curl debug logging to Pidgin debug window
void plugin_set_curl_debug_callback(struct gcal_resource *gcal_data) {
	curl_easy_setopt(gcal_data->curl, CURLOPT_DEBUGFUNCTION, &plugin_curl_debug_callback);
	curl_easy_setopt(gcal_data->curl, CURLOPT_VERBOSE, 1);
}

// WORKAROUND: Need to disable the curl SSL peer verification on Windows
void plugin_set_curl_ssl_verifypeer(struct gcal_resource *gcal_data) {
	curl_easy_setopt(gcal_data->curl, CURLOPT_CAPATH, "C:/Program Files (x86)/Pidgin/ca-certs/");
#ifdef WIN32
	curl_easy_setopt(gcal_data->curl, CURLOPT_SSL_VERIFYPEER, TRUE);
#else
	curl_easy_setopt(gcal_data->curl, CURLOPT_SSL_VERIFYPEER, TRUE);
#endif
}

// WORKAROUND: Empty fields are not allowed in the update, need to set all empty strings to NULL
void plugin_cleanup_contact_data(struct gcal_contact *gcontact_data) {
	if (!gcontact_data)
		return;

	if (gcontact_data->nickname && !strlen(gcontact_data->nickname)) {
		free(gcontact_data->nickname);
		gcontact_data->nickname = NULL;
	}

	if (gcontact_data->homepage && !strlen(gcontact_data->homepage)) {
		free(gcontact_data->homepage);
		gcontact_data->homepage = NULL;
	}

	if (gcontact_data->blog && !strlen(gcontact_data->blog)) {
		free(gcontact_data->blog);
		gcontact_data->blog = NULL;
	}

	if (gcontact_data->occupation && !strlen(gcontact_data->occupation)) {
		free(gcontact_data->occupation);
		gcontact_data->occupation = NULL;
	}

	if (gcontact_data->post_address && !strlen(gcontact_data->post_address)) {
		free(gcontact_data->post_address);
		gcontact_data->post_address = NULL;
	}

	if (gcontact_data->birthday && !strlen(gcontact_data->birthday)) {
		free(gcontact_data->birthday);
		gcontact_data->birthday = NULL;
	}
}
