/*
 * This file is part of the Nice GLib ICE library.
 *
 * (C) 2006, 2007 Collabora Ltd.
 *  Contact: Dafydd Harries
 * (C) 2006, 2007 Nokia Corporation. All rights reserved.
 *  Contact: Kai Vehmanen
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Nice GLib ICE library.
 *
 * The Initial Developers of the Original Code are Collabora Ltd and Nokia
 * Corporation. All Rights Reserved.
 *
 * Contributors:
 *   Dafydd Harries, Collabora Ltd.
 *
 * Alternatively, the contents of this file may be used under the terms of the
 * the GNU Lesser General Public License Version 2.1 (the "LGPL"), in which
 * case the provisions of LGPL are applicable instead of those above. If you
 * wish to allow use of your version of this file only under the terms of the
 * LGPL and not to allow others to use your version of this file under the
 * MPL, indicate your decision by deleting the provisions above and replace
 * them with the notice and other provisions required by the LGPL. If you do
 * not delete the provisions above, a recipient may use your version of this
 * file under either the MPL or the LGPL.
 */
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <arpa/inet.h>

#include <glib.h>

#include "socket.h"
#include "udp-bsd.h"
#include "udp-turn.h"


NICEAPI_EXPORT NiceSocketFactory *
nice_socket_factory_new (NiceSocketFactoryType type)
{
  NiceSocketFactory *man = g_new0 (NiceSocketFactory, 1);

  if (man) {
    switch(type) {
      case NICE_SOCKET_FACTORY_UDP_BSD:
        nice_udp_bsd_socket_factory_init (man);
        break;
      case NICE_SOCKET_FACTORY_UDP_RELAY:
        nice_udp_turn_socket_factory_init (man);
        break;
      default:
        g_free (man);
        man = NULL;
    }
  }

  return man;
}

NICEAPI_EXPORT void
nice_socket_factory_free (NiceSocketFactory *man)
{
  if (man) {
    man->close (man);
    g_free (man);
  }
}

NICEAPI_EXPORT NiceSocket *
nice_socket_new (
  NiceSocketFactory *man,
  NiceAddress *addr)
{
  NiceSocket *sock = g_slice_new0 (NiceSocket);

  if (!man || !sock || !man->init (man, sock, addr)) {
    g_free (sock);
    sock = NULL;
  }

  return sock;
}

NICEAPI_EXPORT gint
nice_socket_recv (
  NiceSocket *sock,
  NiceAddress *from,
  guint len,
  gchar *buf)
{
  return sock->recv (sock, from, len, buf);
}

NICEAPI_EXPORT void
nice_socket_send (
  NiceSocket *sock,
  const NiceAddress *to,
  guint len,
  const gchar *buf)
{
  sock->send (sock, to, len, buf);
}

NICEAPI_EXPORT void
nice_socket_free (NiceSocket *sock)
{
  if (sock) {
    sock->close (sock);
    g_slice_free (NiceSocket,sock);
  }
}
