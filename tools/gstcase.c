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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdlib.h>
#include <string.h>
#include "gstswitchserver.h"
#include "gstcase.h"

enum
{
  PROP_0,
  PROP_TYPE,
  PROP_SERVE,
  PROP_STREAM,
  PROP_INPUT,
  PROP_BRANCH,
  PROP_PORT,
  PROP_A_WIDTH,
  PROP_A_HEIGHT,
  PROP_B_WIDTH,
  PROP_B_HEIGHT,
};

enum
{
  SIGNAL__LAST,
};

//static guint gst_case_signals[SIGNAL__LAST] = { 0 };
extern gboolean verbose;

#define gst_case_parent_class parent_class
G_DEFINE_TYPE (GstCase, gst_case, GST_TYPE_WORKER);

static void
gst_case_init (GstCase * cas)
{
  cas->type = GST_CASE_UNKNOWN;
  cas->stream = NULL;
  cas->input = NULL;
  cas->branch = NULL;
  cas->serve_type = GST_SERVE_NOTHING;
  cas->sink_port = 0;
  cas->a_width = 0;
  cas->a_height = 0;
  cas->b_width = 0;
  cas->b_height = 0;

  //INFO ("init %p", cas);
}

static void
gst_case_dispose (GstCase * cas)
{
  if (cas->stream) {
#if 0
    GError *error = NULL;
    g_input_stream_close (cas->stream, NULL, &error);
    if (error) {
      ERROR ("%s", error->message);
    }
#endif
    g_object_unref (cas->stream);
    cas->stream = NULL;
  }

  if (cas->input) {
    g_object_unref (cas->input);
    cas->input = NULL;
  }

  if (cas->branch) {
    g_object_unref (cas->branch);
    cas->branch = NULL;
  }

  //INFO ("dispose %p", cas);
  G_OBJECT_CLASS (parent_class)->dispose (G_OBJECT (cas));
}

static void
gst_case_finalize (GstCase * cas)
{
  if (G_OBJECT_CLASS (parent_class)->finalize)
    (*G_OBJECT_CLASS (parent_class)->finalize) (G_OBJECT (cas));
}

static void
gst_case_get_property (GstCase *cas, guint property_id,
    GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  case PROP_TYPE:
    g_value_set_uint (value, cas->type);
    break;
  case PROP_SERVE:
    g_value_set_uint (value, cas->serve_type);
    break;
  case PROP_STREAM:
    g_value_set_object (value, cas->stream);
    break;
  case PROP_INPUT:
    g_value_set_object (value, cas->input);
    break;
  case PROP_BRANCH:
    g_value_set_object (value, cas->branch);
    break;
  case PROP_PORT:
    g_value_set_uint (value, cas->sink_port);
    break;
  case PROP_A_WIDTH:
    g_value_set_uint (value, cas->a_width);
    break;
  case PROP_A_HEIGHT:
    g_value_set_uint (value, cas->a_height);
    break;
  case PROP_B_WIDTH:
    g_value_set_uint (value, cas->b_width);
    break;
  case PROP_B_HEIGHT:
    g_value_set_uint (value, cas->b_height);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (cas, property_id, pspec);
    break;
  }
}

static void
gst_case_set_property (GstCase *cas, guint property_id,
    const GValue *value, GParamSpec *pspec)
{
  switch (property_id) {
  case PROP_TYPE:
    cas->type = (GstCaseType) g_value_get_uint (value);
    break;
  case PROP_SERVE:
    cas->serve_type = (GstSwitchServeStreamType) g_value_get_uint (value);
    break;
  case PROP_STREAM: {
    GObject *stream = g_value_dup_object (value);
    if (cas->stream)
      g_object_unref (cas->stream);
    cas->stream = G_INPUT_STREAM (stream);
  } break;
  case PROP_INPUT: {
    GObject *input = g_value_dup_object (value);
    if (cas->input)
      g_object_unref (cas->input);
    cas->input = GST_CASE (input);
  } break;
  case PROP_BRANCH: {
    GObject *branch = g_value_dup_object (value);
    if (cas->branch)
      g_object_unref (cas->branch);
    cas->branch = GST_CASE (branch);
  } break;
  case PROP_PORT:
    cas->sink_port = g_value_get_uint (value);
    break;
  case PROP_A_WIDTH:
    cas->a_width = g_value_get_uint (value);
    break;
  case PROP_A_HEIGHT:
    cas->a_height = g_value_get_uint (value);
    break;
  case PROP_B_WIDTH:
    cas->b_width = g_value_get_uint (value);
    break;
  case PROP_B_HEIGHT:
    cas->b_height = g_value_get_uint (value);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (G_OBJECT (cas), property_id, pspec);
    break;
  }
}

static GString *
gst_case_get_pipeline_string (GstCase * cas)
{
  GString *desc;
  gchar *channel = NULL;
  gchar *convert = NULL;
  gchar *srctype = NULL;
  gchar *sink = NULL;

  desc = g_string_new ("");

  switch (cas->type) {
  case GST_CASE_INPUT_a:
  case GST_CASE_INPUT_v:
    g_string_append_printf (desc, "giostreamsrc name=source ");
    break;
  case GST_CASE_COMPOSITE_A:
  case GST_CASE_COMPOSITE_B:
  case GST_CASE_COMPOSITE_a:
  case GST_CASE_PREVIEW:
    if (srctype == NULL) srctype = "input";
  case GST_CASE_BRANCH_A:
  case GST_CASE_BRANCH_B:
  case GST_CASE_BRANCH_a:
  case GST_CASE_BRANCH_p:
    if (srctype == NULL) srctype = "branch";
    if (cas->serve_type == GST_SERVE_AUDIO_STREAM) {
      g_string_append_printf (desc, "interaudiosrc");
    } else {
      g_string_append_printf (desc, "intervideosrc");
    }
    g_string_append_printf (desc, " name=source channel=%s_%d ",
	srctype, cas->sink_port);
    break;
  default:
    ERROR ("unknown case %d", cas->type);
    break;
  }

  srctype = NULL;

  switch (cas->type) {
  case GST_CASE_COMPOSITE_A:
    if (channel == NULL) channel = "a";
  case GST_CASE_COMPOSITE_B:
    if (channel == NULL) channel = "b";
    sink = "intervideosink";
    convert = g_strdup_printf ("video/x-raw,width=%d,height=%d",
	cas->a_width, cas->a_height);
  case GST_CASE_COMPOSITE_a:
    if (channel == NULL) channel = "audio";
    if (sink == NULL) sink = "interaudiosink";
    if (convert == NULL) convert = g_strdup ("identity");
    g_string_append_printf (desc, "source. ! %s ! tee name=s ", convert);
    g_string_append_printf (desc, "s. ! queue2 "
	"! %s name=sink1 channel=branch_%d ", sink, cas->sink_port);
    g_string_append_printf (desc, "s. ! queue2 "
	"! %s name=sink2 channel=composite_%s ", sink, channel);
    g_free (convert), convert = NULL;
    break;
  case GST_CASE_PREVIEW:
    if (srctype == NULL) srctype = "branch";
  case GST_CASE_INPUT_a:
  case GST_CASE_INPUT_v:
    if (srctype == NULL) srctype = "input";
    if (cas->serve_type == GST_SERVE_AUDIO_STREAM) {
      g_string_append_printf (desc, "interaudiosink");
    } else {
      g_string_append_printf (desc, "intervideosink");
    }
    g_string_append_printf (desc, " name=sink channel=%s_%d ",
	srctype, cas->sink_port);
    if (cas->serve_type == GST_SERVE_AUDIO_STREAM) {
      //g_string_append_printf (desc, "source. ! gdpdepay ! sink. ");
      g_string_append_printf (desc, "source. ! sink. ");
    } else {
      if (cas->type == GST_CASE_PREVIEW) {
	g_string_append_printf (desc, "source. "
	    "! video/x-raw,width=%d,height=%d ! sink. ",
	    cas->a_width, cas->a_height);
      } else {
	g_string_append_printf (desc, "source. ! gdpdepay ! sink. ");
      }
    }
    break;
  case GST_CASE_BRANCH_A:
  case GST_CASE_BRANCH_B:
  case GST_CASE_BRANCH_a:
  case GST_CASE_BRANCH_p:
    g_string_append_printf (desc, "tcpserversink name=sink port=%d ",
	cas->sink_port);
    g_string_append_printf (desc, "source. ");
    if (cas->serve_type == GST_SERVE_AUDIO_STREAM) {
      g_string_append_printf (desc, "! faac ");
    } else {
      g_string_append_printf (desc, "! video/x-raw,width=%d,height=%d ",
	  cas->a_width, cas->a_height);
    }
    g_string_append_printf (desc, "! gdppay ! sink. ");
    break;
  case GST_CASE_UNKNOWN:
    ERROR ("unknown case (%d)", cas->type);
    break;
  }

  return desc;
}

static void
gst_case_client_socket_added (GstElement *element,
    GSocket *socket, GstCase *cas)
{
  g_return_if_fail (G_IS_SOCKET (socket));

  INFO ("client-socket-added: %d", g_socket_get_fd (socket));
}

static void
gst_case_client_socket_removed (GstElement *element,
    GSocket *socket, GstCase *cas)
{
  g_return_if_fail (G_IS_SOCKET (socket));

  INFO ("client-socket-removed: %d", g_socket_get_fd (socket));

  g_socket_close (socket, NULL);
}

static gboolean
gst_case_prepare (GstCase *cas)
{
  GstWorker *worker = GST_WORKER (cas);
  GstElement *source = NULL;
  switch (cas->type) {
  case GST_CASE_INPUT_a:
  case GST_CASE_INPUT_v:
    if (!cas->stream) {
      ERROR ("no stream for new case");
      return FALSE;
    }
    source = gst_worker_get_element_unlocked (worker, "source");
    if (!source) {
      ERROR ("no source");
      return FALSE;
    }
    g_object_set (source, "stream", cas->stream, NULL);
    gst_object_unref (source);
    break;

  case GST_CASE_BRANCH_A:
  case GST_CASE_BRANCH_B:
  case GST_CASE_BRANCH_a:
  case GST_CASE_BRANCH_p: {
    GstElement *sink = gst_worker_get_element_unlocked (worker, "sink");

    g_return_val_if_fail (GST_IS_ELEMENT (sink), FALSE);

    g_signal_connect (sink, "client-added",
	G_CALLBACK (gst_case_client_socket_added), cas);

    g_signal_connect (sink, "client-socket-removed",
	G_CALLBACK (gst_case_client_socket_removed), cas);
  } break;

  default:
    break;
  }
 
  return TRUE;
}

static void
gst_case_class_init (GstCaseClass * klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  GstWorkerClass *worker_class = GST_WORKER_CLASS (klass);

  object_class->dispose = (GObjectFinalizeFunc) gst_case_dispose;
  object_class->finalize = (GObjectFinalizeFunc) gst_case_finalize;
  object_class->set_property = (GObjectSetPropertyFunc) gst_case_set_property;
  object_class->get_property = (GObjectGetPropertyFunc) gst_case_get_property;

  g_object_class_install_property (object_class, PROP_TYPE,
      g_param_spec_uint ("type", "Type", "Case type",
          GST_CASE_UNKNOWN, GST_CASE__LAST_TYPE, GST_CASE_UNKNOWN,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_SERVE,
      g_param_spec_uint ("serve", "Serve", "Serve type",
          GST_SERVE_NOTHING, GST_SERVE_AUDIO_STREAM, GST_SERVE_NOTHING,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_STREAM,
      g_param_spec_object ("stream", "Stream", "Stream to read from",
          G_TYPE_INPUT_STREAM, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_INPUT,
      g_param_spec_object ("input", "Input", "The input of the case",
          GST_TYPE_CASE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_BRANCH,
      g_param_spec_object ("branch", "Branch", "The branch of the case",
          GST_TYPE_CASE, G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_PORT,
      g_param_spec_uint ("port", "Port", "Sink port",
          GST_SWITCH_MIN_SINK_PORT, GST_SWITCH_MAX_SINK_PORT,
	  GST_SWITCH_MIN_SINK_PORT,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_A_WIDTH,
      g_param_spec_uint ("awidth", "A Width", "Channel A width",
          1, G_MAXINT, GST_SWITCH_COMPOSITE_DEFAULT_WIDTH,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_A_HEIGHT,
      g_param_spec_uint ("aheight", "A Height", "Channel A height",
          1, G_MAXINT, GST_SWITCH_COMPOSITE_DEFAULT_HEIGHT,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_B_WIDTH,
      g_param_spec_uint ("bwidth", "B Width", "Channel B width",
          1, G_MAXINT, GST_SWITCH_COMPOSITE_DEFAULT_WIDTH,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  g_object_class_install_property (object_class, PROP_B_HEIGHT,
      g_param_spec_uint ("bheight", "B Height", "Channel B height",
          1, G_MAXINT, GST_SWITCH_COMPOSITE_DEFAULT_HEIGHT,
	  G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

  worker_class->prepare = (GstWorkerPrepareFunc) gst_case_prepare;
  worker_class->get_pipeline_string = (GstWorkerGetPipelineStringFunc)
    gst_case_get_pipeline_string;
}
