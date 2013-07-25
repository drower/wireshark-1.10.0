/* conversations_sctp.c
 * conversations_sctp   2005 Oleg Terletsky <oleg.terletsky@comverse.com>
 *
 * $Id: conversations_sctp.c 48448 2013-03-21 02:58:59Z wmeier $
 *
 * Wireshark - Network traffic analyzer
 * By Gerald Combs <gerald@wireshark.org>
 * Copyright 1998 Gerald Combs
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

#include "config.h"

#include <string.h>

#include <gtk/gtk.h>

#include <epan/packet.h>
#include <epan/stat_cmd_args.h>
#include <epan/tap.h>
#include <epan/dissectors/packet-sctp.h>

#include "../stat_menu.h"

#include "ui/gtk/gui_stat_menu.h"
#include "ui/gtk/conversations_table.h"

static int
sctp_conversation_packet(void *pct, packet_info *pinfo, epan_dissect_t *edt _U_, const void *vip)
{
	const struct _sctp_info *sctphdr=(struct _sctp_info *)vip;

	add_conversation_table_data((conversations_table *)pct,
		&sctphdr->ip_src,
		&sctphdr->ip_dst,
		sctphdr->sport,
		sctphdr->dport,
		1,
		pinfo->fd->pkt_len,
		&pinfo->fd->rel_ts,
                SAT_NONE,
		PT_SCTP);


	return 1;
}



static void
sctp_conversation_init(const char *opt_arg, void* userdata _U_)
{
	const char *filter=NULL;

	if(!strncmp(opt_arg,"conv,sctp,",10)){
		filter=opt_arg+10;
	} else {
		filter=NULL;
	}

	init_conversation_table(FALSE, "SCTP", "sctp", filter, sctp_conversation_packet);

}

void
sctp_conversation_cb(GtkAction *action _U_, gpointer user_data _U_)
{
	sctp_conversation_init("conv,sctp",NULL);
}

void
register_tap_listener_sctp_conversation(void)
{
	register_stat_cmd_arg("conv,sctp", sctp_conversation_init,NULL);
	register_conversation_table(FALSE, "SCTP", "sctp", NULL /*filter*/, sctp_conversation_packet);
}
