/* GstComposite
 * Copyright (C) 2012 Duzy Chan <code@duzy.info>
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

#ifndef __GST_COMPOSITE_H__by_Duzy_Chan__
#define __GST_COMPOSITE_H__by_Duzy_Chan__ 1
#include "gstworker.h"

#define GST_TYPE_COMPOSITE (gst_composite_get_type ())
#define GST_COMPOSITE(object) (G_TYPE_CHECK_INSTANCE_CAST ((object), GST_TYPE_COMPOSITE, GstComposite))
#define GST_COMPOSITE_CLASS(class) (G_TYPE_CHECK_CLASS_CAST ((class), GST_TYPE_COMPOSITE, GstCompositeClass))
#define GST_IS_COMPOSITE(object) (G_TYPE_CHECK_INSTANCE_TYPE ((object), GST_TYPE_COMPOSITE, GstComposite))
#define GST_IS_COMPOSITE_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), GST_TYPE_COMPOSITE, GstCompositeClass))

#define GST_SWITCH_COMPOSITE_DEFAULT_A_WIDTH	640
#define GST_SWITCH_COMPOSITE_DEFAULT_A_HEIGHT	480
#define GST_SWITCH_COMPOSITE_DEFAULT_B_WIDTH	100
#define GST_SWITCH_COMPOSITE_DEFAULT_B_HEIGHT	80

enum {
  COMPOSE_MODE_0,
  COMPOSE_MODE_1,
  COMPOSE_MODE_2,
};

typedef struct _GstComposite GstComposite;
typedef struct _GstCompositeClass GstCompositeClass;
typedef struct _GstSwitchServer GstSwitchServer;

struct _GstComposite
{
  GstWorker base;

  gint sink_port;

  guint a_width;
  guint a_height;
  guint b_width;
  guint b_height;
};

struct _GstCompositeClass
{
  GstWorkerClass base_class;

  void (*end_composite) (GstComposite *composite);
};

GType gst_composite_get_type (void);

#endif//__GST_COMPOSITE_H__by_Duzy_Chan__
