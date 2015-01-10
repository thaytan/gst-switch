/* GStreamer
 * Copyright (C) 2011 David Schleef <ds@entropywave.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 51 Franklin Street, Suite 500,
 * Boston, MA 02110-1335, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gstinteraudiosrc.h"
#include "gstinteraudiosink.h"
#include "gstintersubsrc.h"
#include "gstintersubsink.h"
#include "gstintervideosrc.h"
#include "gstintervideosink.h"
#include "gstintersurface.h"

static gboolean
plugin_init (GstPlugin * plugin)
{
  gst_element_register (plugin, "dvsinteraudiosrc", GST_RANK_NONE,
      DVS_TYPE_INTER_AUDIO_SRC);
  gst_element_register (plugin, "dvsinteraudiosink", GST_RANK_NONE,
      DVS_TYPE_INTER_AUDIO_SINK);
  gst_element_register (plugin, "dvsintersubsrc", GST_RANK_NONE,
      DVS_TYPE_INTER_SUB_SRC);
  gst_element_register (plugin, "dvsintersubsink", GST_RANK_NONE,
      DVS_TYPE_INTER_SUB_SINK);
  gst_element_register (plugin, "dvsintervideosrc", GST_RANK_NONE,
      DVS_TYPE_INTER_VIDEO_SRC);
  gst_element_register (plugin, "dvsintervideosink", GST_RANK_NONE,
      DVS_TYPE_INTER_VIDEO_SINK);

  return TRUE;
}

GST_PLUGIN_DEFINE (GST_VERSION_MAJOR, GST_VERSION_MINOR,
    dvsinter, "plugin for inter-pipeline communication",
    plugin_init, VERSION, "LGPL",
    "GStreamer", "https://github.com/duzy/gst-switch")
