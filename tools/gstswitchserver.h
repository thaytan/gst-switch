/* GstSwitch
 * Copyright (C) 2012,2013 Duzy Chan <code@duzy.info>
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef __GST_SWITCH_SERVER_H__by_Duzy_Chan__
#define __GST_SWITCH_SERVER_H__by_Duzy_Chan__ 1
#include <gio/gio.h>
#include "gstcomposite.h"
#include "gstswitchcontroller.h"
#include "../logutils.h"

#define GST_TYPE_SWITCH_SERVER (gst_switch_server_get_type())
#define GST_SWITCH_SERVER(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), GST_TYPE_SWITCH_SERVER, GstSwitchServer))
#define GST_SWITCH_SERVER_CLASS(class) (G_TYPE_CHECK_CLASS_CAST ((class), GST_TYPE_SWITCH_SERVER, GstSwitchServerClass))
#define GST_IS_SWITCH_SERVER(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), GST_TYPE_SWITCH_SERVER))
#define GST_IS_SWITCH_SERVER_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), GST_TYPE_SWITCH_SERVER))

#define GST_SWITCH_MIN_SINK_PORT 1
#define GST_SWITCH_MAX_SINK_PORT 65535

typedef struct _GstSwitchServerClass GstSwitchServerClass;
typedef struct _GstSwitchServerOpts GstSwitchServerOpts;

struct _GstSwitchServerOpts
{
  gchar * test_switch;
  gchar * record_filename;
  gchar * controller_address;
  gint video_input_port;
  gint audio_input_port;
  gint control_port;
};

struct _GstSwitchServer
{
  GObject base;

  gchar *host;

  GMainLoop *main_loop;

  GCancellable *cancellable;
  GMutex video_acceptor_lock;
  GThread *video_acceptor;
  GSocket *video_acceptor_socket;
  gint video_acceptor_port;

  GMutex audio_acceptor_lock;
  GThread *audio_acceptor;
  GSocket *audio_acceptor_socket;
  gint audio_acceptor_port;

  GMutex controller_lock;
  GThread *controller_thread;
  GSocket *controller_socket;
  gint controller_port;
  GstSwitchController *controller;

  GMutex alloc_port_lock;
  gint alloc_port_count;

  GMutex serve_lock;
  GMutex cases_lock;
  GList *cases;

  GstComposite *composite;
  GstCompositeMode new_composite_mode;

  GMutex pip_lock;
  gint pip_x, pip_y, pip_w, pip_h;
};

struct _GstSwitchServerClass
{
  GObjectClass base_class;
};

GType gst_switch_server_get_type (void);
gint gst_switch_server_get_composite_sink_port (GstSwitchServer * srv);
gint gst_switch_server_get_encode_sink_port (GstSwitchServer * srv);
gint gst_switch_server_get_audio_sink_port (GstSwitchServer * srv);
GArray *gst_switch_server_get_preview_sink_ports (GstSwitchServer * srv, GArray **serves, GArray **types);

gboolean gst_switch_server_set_composite_mode (GstSwitchServer * srv, gint mode);
gboolean gst_switch_server_switch (GstSwitchServer * srv, gint channel, gint port);
guint gst_switch_server_adjust_pip (GstSwitchServer * srv, gint dx, gint dy, gint dw, gint dh);
gboolean gst_switch_server_new_record (GstSwitchServer * srv);

extern GstSwitchServerOpts opts;

#endif//__GST_SWITCH_SERVER_H__by_Duzy_Chan__
