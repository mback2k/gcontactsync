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

#ifndef GCONTACTSYNC_HACKS_H_
#define GCONTACTSYNC_HACKS_H_

#include <string.h>
#include <glib.h>

#include <curl/curl.h>

#include <internal_gcal.h>

// WORKAROUND: Need to disable the curl SSL peer verification on Windows
void plugin_set_curl_ssl_verifypeer(struct gcal_resource *gcal_data);

// WORKAROUND: Empty fields are not allow in the update, need to set all empty strings to NULL
void plugin_cleanup_contact_data(struct gcal_contact *gcontact_data);

#endif /* GCONTACTSYNC_HACKS_H_ */
