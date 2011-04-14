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

#ifndef GCONTACTSYNC_DEBUG_H_
#define GCONTACTSYNC_DEBUG_H_

#include <string.h>
#include <glib.h>

#include <gcontactsync.h>

int plugin_gcal_debug_callback(void *handle, char *data, size_t size);
int plugin_curl_debug_callback(void *handle, int type, char *data, size_t size, void *userptr);

#endif /* GCONTACTSYNC_DEBUG_H_ */
