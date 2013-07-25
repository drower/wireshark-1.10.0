/* capture-wpcap.h
 *
 * $Id: capture-wpcap.h 43536 2012-06-28 22:56:06Z darkjames $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 2001 Gerald Combs
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#ifndef CAPTURE_WPCAP_H
#define CAPTURE_WPCAP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

extern gboolean has_wpcap;


extern void load_wpcap(void);

/* error message, if WinPcap couldn't be loaded */
/* will use g_strdup, don't forget to g_free the returned string! */
extern char *cant_load_winpcap_err(const char *app_name);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CAPTURE_WPCAP_H */
