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
 * Free Software Foundation, Inc., 51 Franklin St, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef _GST_INTER_SUB_SINK_H_
#define _GST_INTER_SUB_SINK_H_

#include <gst/base/gstbasesink.h>
#include "gstintersurface.h"

G_BEGIN_DECLS

#define DVS_TYPE_INTER_SUB_SINK   (dvs_inter_sub_sink_get_type())
#define GST_INTER_SUB_SINK(obj)   (G_TYPE_CHECK_INSTANCE_CAST((obj),DVS_TYPE_INTER_SUB_SINK,DvsInterSubSink))
#define GST_INTER_SUB_SINK_CLASS(klass)   (G_TYPE_CHECK_CLASS_CAST((klass),DVS_TYPE_INTER_SUB_SINK,DvsInterSubSinkClass))
#define GST_IS_INTER_SUB_SINK(obj)   (G_TYPE_CHECK_INSTANCE_TYPE((obj),DVS_TYPE_INTER_SUB_SINK))
#define GST_IS_INTER_SUB_SINK_CLASS(obj)   (G_TYPE_CHECK_CLASS_TYPE((klass),DVS_TYPE_INTER_SUB_SINK))

typedef struct _DvsInterSubSink DvsInterSubSink;
typedef struct _DvsInterSubSinkClass DvsInterSubSinkClass;

struct _DvsInterSubSink
{
  GstBaseSink base_intersubsink;

  GstPad *sinkpad;
  DvsInterSurface *surface;
  char *channel;

  int fps_n;
  int fps_d;
};

struct _DvsInterSubSinkClass
{
  GstBaseSinkClass base_intersubsink_class;
};

GType dvs_inter_sub_sink_get_type (void);

G_END_DECLS

#endif
