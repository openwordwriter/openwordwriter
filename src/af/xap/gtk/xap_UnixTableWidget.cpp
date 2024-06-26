/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/* 
 * xap_TableWidget.cpp
 * Copyright 2002 Joaquin Cuenca Abela
 * Copyright 2016 Hubert Figuiere
 *
 * Authors:
 *   Joaquin Cuenca Abela (e98cuenc@yahoo.com)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License, version 2, as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301 USA.
 */

#include <stdio.h>
#include <stdlib.h>

#include <algorithm>

#include <gdk/gdkkeysyms.h>
#include <gdk/gdk.h>
#include <gtk/gtk.h>

#include "xap_GtkUtils.h"
#include "xap_GtkStyle.h"
#include "xap_UnixTableWidget.h"
#include "ut_debugmsg.h"
#include "ut_assert.h"

/* NONE:UINT,UINT (/dev/stdin:1) */
static void
g_cclosure_user_marshal_VOID__UINT_UINT (GClosure     *closure,
                                         GValue       * /*return_value*/,
                                         guint         n_param_values,
                                         const GValue *param_values,
                                         gpointer      /*invocation_hint*/,
                                         gpointer      marshal_data)
{
  typedef void (*GMarshalFunc_VOID__UINT_UINT) (gpointer     data1,
                                                guint        arg_1,
                                                guint        arg_2,
                                                gpointer     data2);
  GMarshalFunc_VOID__UINT_UINT callback;
  GCClosure *cc = (GCClosure*) closure;
  gpointer data1, data2;

  UT_return_if_fail (n_param_values == 3);
  UT_DEBUGMSG(("IN AbiTable g_cclose_.. \n"));
  if (G_CCLOSURE_SWAP_DATA (closure))
    {
      data1 = closure->data;
      data2 = g_value_get_pointer (param_values + 0);
    }
  else
    {
      data1 = g_value_get_pointer (param_values + 0);
      data2 = closure->data;
    }
  callback = (GMarshalFunc_VOID__UINT_UINT) (marshal_data ? marshal_data : cc->callback);
  UT_DEBUGMSG(("Calling callback marshell data %p cc %p \n",callback,cc));

  callback (data1,
            g_value_get_uint (param_values + 1),
            g_value_get_uint (param_values + 2),
            data2);
}

enum
{
	SELECTED,
	LAST_SIGNAL
};

static gint abi_table_signals [LAST_SIGNAL] = { 0 };

static GtkWidgetClass *abi_table_parent_class;

/* ------------------- now the guts of AbiTable ---------------------- */

static const guint cell_width = 24;
static const guint cell_height = 24;
static const guint cell_spacing = 4;
static const guint init_rows = 0;
static const guint init_cols = 0;

static inline void
cells_to_pixels(guint cols, guint rows, guint* w, guint* h)
{
	UT_return_if_fail(w);
	UT_return_if_fail(h);

	*w = cell_width * cols + cell_spacing * (cols + 1);
	*h = cell_height * rows + cell_spacing * (rows + 1);
}

static inline void
pixels_to_cells(guint w, guint h, guint* cols, guint* rows)
{
	UT_return_if_fail(cols);
	UT_return_if_fail(rows);

	*cols = w / (cell_width + cell_spacing) + 1;
	*rows = h / (cell_height + cell_spacing) + 1;
}

static void
abi_table_resize(AbiTable* table)
{
	guint width;
	guint height;
	char* text;
	GtkRequisition size;

	UT_return_if_fail(table);

	if (table->selected_rows == 0 && table->selected_cols == 0)
		text = g_strdup(table->szCancel);
	else
	{
		text = g_strdup_printf("%d x %d %s", table->selected_rows, table->selected_cols, table->szTable);
	}
	cells_to_pixels(table->total_cols, table->total_rows, &width, &height);
	gtk_widget_get_preferred_size(GTK_WIDGET(table->window_label), &size, nullptr);

	gtk_label_set_text(table->window_label, text);
	gtk_window_resize(table->window, width + 1, height + size.height);

	g_free(text);
}

extern "C" void
abi_table_set_selected (AbiTable* abi_table, guint rows, guint cols)
{
	UT_return_if_fail (abi_table);

	abi_table->selected_rows = rows;
	abi_table->selected_cols = cols;

	abi_table_resize(abi_table);
}

extern "C" void
abi_table_get_selected (const AbiTable* abi_table, guint* rows, guint* cols)
{
	UT_return_if_fail (abi_table);

	if (rows)
		*rows = abi_table->selected_rows;

	if (cols)
		*cols = abi_table->selected_cols;
}

extern "C" void
abi_table_set_max_size (AbiTable* abi_table, guint rows, guint cols)
{
	UT_return_if_fail (abi_table);

	abi_table->total_rows = rows;
	abi_table->total_cols = cols;

	abi_table_resize(abi_table);
}


extern "C" void
abi_table_set_labels(AbiTable* abi_table, const gchar * szTable, const gchar * szCancel)
{
	if(abi_table->szTable)
		g_free(abi_table->szTable);
	abi_table->szTable = g_strdup(szTable);	
	if(abi_table->szCancel)
		g_free(abi_table->szCancel);
	abi_table->szCancel = g_strdup(szCancel);	
}

extern "C" void
abi_table_get_max_size (const AbiTable* abi_table, guint* rows, guint* cols)
{
	UT_return_if_fail (abi_table);

	if (rows)
		*rows = abi_table->total_rows;

	if (cols)
		*cols = abi_table->total_cols;
}

static gboolean
on_drawing_area_event (GtkWidget *area, cairo_t *cr, gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);

	if (!table || !table->style_context) {
		return TRUE;
	}

	guint i;
	guint j;
	guint selected_rows = table->selected_rows;
	guint selected_cols = table->selected_cols;
	guint x;
	guint y;

	GtkStyleContext* ctxt = gtk_widget_get_style_context(GTK_WIDGET(area));
	gtk_style_context_save(ctxt);
	gtk_style_context_set_state(ctxt, GTK_STATE_FLAG_FOCUSED);
	for (i = 0; i < table->total_rows; ++i) {
		for (j = 0; j < table->total_cols; ++j) {
			cells_to_pixels(j, i, &x, &y);

			if (i < selected_rows && j < selected_cols) {
				gtk_style_context_set_state(table->style_context, GTK_STATE_FLAG_SELECTED);
			} else {
				gtk_style_context_set_state(table->style_context, GTK_STATE_FLAG_NORMAL);
			}
			gtk_render_background(table->style_context, cr, x + 1, y + 1,
								  cell_width - 1, cell_height - 1);

			gtk_render_frame(ctxt, cr, x, y, cell_width, cell_height);
		}
	}
	gtk_style_context_restore(ctxt);

	return TRUE;
}

static inline guint
my_max(guint a, guint b)
{
	return a < b ? b : a;
}

static gboolean
on_motion_notify_event (GtkWidget *window, GdkEventMotion *ev, gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);
	guint selected_cols;
	guint selected_rows;
	gdouble x, y;

	x = y = 0.0f;
	gdk_event_get_coords((GdkEvent*)ev, &x, &y);

	// In Gtk3, the coordinate of the motion event are relative to the top-level
	x -= table->pos_x;
	y -= table->pos_y;

	if (x < 0.0f || y < 0.0f)
		return TRUE;

	pixels_to_cells(static_cast<guint>(x), static_cast<guint>(y), &selected_cols, &selected_rows);

	if ((selected_cols != table->selected_cols) || (selected_rows != table->selected_rows))
	{
		/* grow or shrink the table widget as necessary */
		table->selected_cols = selected_cols;
		table->selected_rows = selected_rows;

		if (table->selected_rows <= 0 || table->selected_cols <= 0)
			table->selected_rows = table->selected_cols = 0;

		table->total_rows = my_max(table->selected_rows + 1, 3);
		table->total_cols = my_max(table->selected_cols + 1, 3);

		abi_table_resize(table);
		gtk_widget_queue_draw (window);
	}

	return TRUE;
}

static void
restart_widget (AbiTable *table)
{
	table->selected_cols = init_cols;
	table->selected_rows = init_rows;
	table->total_cols = my_max(init_cols + 1, 5);
	table->total_rows = my_max(init_rows + 1, 6);
	g_signal_emit_by_name(table, "released");
	gtk_widget_hide(GTK_WIDGET(table->window));
}

/*
 * Fires signal "selected", and reset and hide the widget
 */
static void
emit_selected (AbiTable *table)
{
	gtk_widget_hide(GTK_WIDGET(table->window));

	while (g_main_context_pending(nullptr)) {
		g_main_context_iteration(nullptr, false);
	}

	if (table->selected_rows > 0 && table->selected_cols > 0)
		g_signal_emit (G_OBJECT (table),
			       abi_table_signals [SELECTED], 0,
			       table->selected_rows, table->selected_cols);

	restart_widget(table);
}

static gboolean
on_button_release_event (GtkWidget *, GdkEventButton *ev, gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);

	gdouble x, y;
	x = y = 0.0f;
	gdk_event_get_coords((GdkEvent*)ev, &x, &y);

	/* Quick test to know if we're possibly over the button */
	if (y < 0.0 && x >= 0.0)
	{
		GtkRequisition size;

		gtk_widget_get_preferred_size(GTK_WIDGET(table), &size, nullptr);

		/* And now, precise and slightly slower test.
		   I wonder if the double test really matters from a speed pov */
		if (-y < size.height && x < size.width)
			return TRUE;
	}

	emit_selected(table);

	return TRUE;
}

static gboolean
on_leave_event (GtkWidget *area,
				GdkEventCrossing *event,
				gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);

	gdouble x, y;
	x = y = 0.0f;
	gdk_event_get_coords((GdkEvent*)event, &x, &y);

	if (gtk_widget_get_visible(GTK_WIDGET(table->window)) && (x < 0 || y < 0))
	{
		table->selected_rows = 0;
		table->selected_cols = 0;
		table->total_rows = my_max(table->selected_rows + 1, 3);
		table->total_cols = my_max(table->selected_cols + 1, 3);

		abi_table_resize(table);
		gtk_widget_queue_draw (area);
	}

	return TRUE;
}

static gboolean
popup_grab_on_window (GdkWindow *window)
{
	GdkSeat *seat = gdk_display_get_default_seat(gdk_window_get_display(window));
	return gdk_seat_grab(seat, window, GDK_SEAT_CAPABILITY_ALL,
						 FALSE, nullptr, nullptr, nullptr, nullptr) == GDK_GRAB_SUCCESS;
}

static void
on_pressed(GtkButton* button, gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);
	int left, top;
	GtkAllocation alloc;

	/* Temporarily grab pointer and keyboard on a window we know exists; we
	 * do this so that the grab (with owner events == TRUE) affects
	 * events generated when the window is mapped, such as enter
	 * notify events on subwidgets. If the grab fails, bail out.
	 */
	if (!popup_grab_on_window(gtk_widget_get_window(GTK_WIDGET(button))))
		return;

	auto toplevel = gtk_widget_get_toplevel(GTK_WIDGET(table));
	gtk_window_set_transient_for(table->window, GTK_WINDOW(toplevel));
	gdk_window_get_origin (gtk_widget_get_window(GTK_WIDGET(table)), &left, &top);
	gtk_widget_get_allocation(GTK_WIDGET(table), &alloc);
	table->pos_x = left + alloc.x;
	table->pos_y = top + alloc.y + alloc.height;
	gtk_window_move(table->window, table->pos_x, table->pos_y);
	abi_table_resize(table);

	gtk_widget_show(GTK_WIDGET(table->window));
	gtk_widget_grab_focus(GTK_WIDGET(table->window));

	/* Now transfer our grabs to the popup window; this
	 * should always succeed.
	 */
	popup_grab_on_window (gtk_widget_get_window(GTK_WIDGET(table->area)));
}

gboolean
on_key_event(GtkWidget *widget, GdkEventKey *event, gpointer user_data)
{
	AbiTable* table = static_cast<AbiTable*>(user_data);
	gboolean grew = FALSE;
	guint keyval = 0;
	gdk_event_get_keyval((GdkEvent*)event, &keyval);

	switch (keyval)
	{
	case GDK_KEY_Up:
	case GDK_KEY_KP_Up:
		if (table->selected_rows > 0)
			--table->selected_rows;
		break;
	case GDK_KEY_Down:
	case GDK_KEY_KP_Down:
		grew = TRUE;
		++table->selected_rows;
		break;
	case GDK_KEY_Left:
	case GDK_KEY_KP_Left:
		if (table->selected_cols > 0)
			--table->selected_cols;
		break;
	case GDK_KEY_Right:
	case GDK_KEY_KP_Right:
		grew = TRUE;
		++table->selected_cols;
		break;
	case GDK_KEY_Escape:
		restart_widget(table);
		return TRUE;
	case GDK_KEY_KP_Space:
	case GDK_KEY_KP_Enter:
	case GDK_KEY_space:
	case GDK_KEY_3270_Enter:
	case GDK_KEY_ISO_Enter:
	case GDK_KEY_Return:
		emit_selected(table);
		return TRUE;
	}

	if(table->selected_rows == 0 || table->selected_cols == 0)
		table->selected_rows = table->selected_cols = (grew ? 1 : 0) ;

	table->total_rows = my_max(table->selected_rows + 1, 3);
	table->total_cols = my_max(table->selected_cols + 1, 3);

	abi_table_resize(table);
	gtk_widget_queue_draw (widget);

	return TRUE;
}


/* XPM */
static const char * widget_tb_insert_table_xpm[] = {
	"24 24 32 1",
	" 	c None",
	".	c #000000",
	"+	c #FFFFFF",
	"@	c #B1B1B1",
	"#	c #BABABA",
	"$	c #E4E4E4",
	"%	c #ECECEC",
	"&	c #A8A8A8",
	"*	c #ADADAD",
	"=	c #B9B9B9",
	"-	c #ABABAB",
	";	c #B8B8B8",
	">	c #B5B5B5",
	",	c #AAAAAA",
	"'	c #858585",
	")	c #828282",
	"!	c #707070",
	"~	c #BCBCBC",
	"{	c #A7A7A7",
	"]	c #8D8D8D",
	"^	c #737373",
	"/	c #ACACAC",
	"(	c #878787",
	"_	c #747474",
	":	c #A9A9A9",
	"<	c #E9E9E9",
	"[	c #C3C3C3",
	"}	c #BFBFBF",
	"|	c #757575",
	"1	c #7E7E7E",
	"2	c #BBBBBB",
	"3	c #A3A3A3",
	"                        ",
	"                        ",
	"                        ",
	"   ...................  ",
	"   .+++++@+++++#++++$.  ",
	"   .+%%%%&%%%%%*%%%%=.  ",
	"   .+%%%%-%%%%%-%%%%;.  ",
	"   .>&,,&'&----)&---!.  ",
	"   .+%%%%-%%%%%-%%%%~.  ",
	"   .+%%%%-%%%%%-%%%%~.  ",
	"   .+%%%%-%%%%%-%%%%~.  ",
	"   .#{---'-----]*---^.  ",
	"   .+%%%%-%%%%%-%%%%~.  ",
	"   .+%%%%-%%%%%-%%%%;.  ",
	"   .+%%%%-%%%%%-%%%%~.  ",
	"   .;&--/(&{--&'&---_.  ",
	"   .+%%%%-%%%%%-%%%%;.  ",
	"   .+%%%%&%%%%%:%%%%~.  ",
	"   .<[[[}|~~~~~1;~~23.  ",
	"   ...................  ",
	"                        ",
	"                        ",
	"                        ",
	"                        "};


void abi_table_set_icon(AbiTable* abi_table,GtkWidget * gtkImageIcon)
{
	if(!GTK_IS_IMAGE(G_OBJECT(gtkImageIcon)))
	   return;
	g_object_unref (G_OBJECT (abi_table->icon));
	abi_table->icon = gtkImageIcon;
}

/* ------------------- and now the GObject part ---------------------- */

static void
abi_table_dispose (GObject *instance)
{
	AbiTable* self = ABITABLE_WIDGET(instance);

// For some reason I get an alert
	if(self->szTable) {
		g_free(self->szTable);
		self->szTable = nullptr;
	}
	if(self->szCancel) {
		g_free(self->szCancel);
		self->szCancel = nullptr;
	}

	g_clear_object(&self->style_context);

	G_OBJECT_CLASS (abi_table_parent_class)->dispose (instance);
}

static void
abi_table_class_init (AbiTableClass *klass, gpointer)
{
	GtkWidgetClass *object_class = reinterpret_cast<GtkWidgetClass*>(klass);

	G_OBJECT_CLASS(object_class)->dispose = abi_table_dispose;

	abi_table_parent_class = static_cast<GtkWidgetClass *>(g_type_class_peek (GTK_TYPE_BUTTON));
	abi_table_signals [SELECTED] =
		g_signal_new ("selected",
			      G_OBJECT_CLASS_TYPE (object_class),
			      G_SIGNAL_RUN_LAST,
			      G_STRUCT_OFFSET (AbiTableClass, selected),
			      nullptr, nullptr,
			      g_cclosure_user_marshal_VOID__UINT_UINT,
			      G_TYPE_NONE, 2, G_TYPE_UINT, G_TYPE_UINT);
}

static void
abi_table_init (AbiTable* table, gpointer)
{
	char* text = g_strdup_printf("%d x %d ", init_rows, init_cols);

	g_type_ensure(GTK_TYPE_TREE_VIEW);
	table->style_context = XAP_GtkStyle_get_style(nullptr, "GtkTreeView.view"); // "textview.view"

	table->button_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);

	table->window = GTK_WINDOW(gtk_window_new(GTK_WINDOW_POPUP));
	table->window_vbox = GTK_BOX(gtk_box_new(GTK_ORIENTATION_VERTICAL, 0));

	table->area = GTK_DRAWING_AREA(gtk_drawing_area_new());

	table->handlers = nullptr;
	table->window_label = GTK_LABEL(gtk_label_new(text));
	g_free(text);
	table->szTable = nullptr;
	table->szCancel = nullptr;
	gtk_container_add(GTK_CONTAINER(table->window), GTK_WIDGET(table->window_vbox));
	gtk_box_pack_end(GTK_BOX(table->window_vbox), GTK_WIDGET(table->window_label), FALSE, FALSE, 0);
	gtk_box_pack_end(GTK_BOX(table->window_vbox), GTK_WIDGET(table->area), TRUE, TRUE, 0);

	gtk_widget_show_all(GTK_WIDGET(table->window_vbox));

	table->pos_x = 0;
	table->pos_y = 0;
	table->selected_rows = init_rows;
	table->selected_cols = init_cols;

	table->total_rows = my_max(init_rows + 1, 5);
	table->total_cols = my_max(init_cols + 1, 6);

	abi_table_resize(table);

	table->icon = nullptr;

	GdkPixbuf* pixbuf = gdk_pixbuf_new_from_xpm_data((const char **)widget_tb_insert_table_xpm);
	if (pixbuf) {
		table->icon = gtk_image_new_from_pixbuf(pixbuf);
		g_object_unref(pixbuf);
	}

	gtk_widget_show(table->icon);
	gtk_box_pack_end(GTK_BOX(table->button_box), table->icon, FALSE, FALSE, 0);
	UT_DEBUGMSG(("abi-table icon loaded %p !\n",table->icon));

	gtk_container_add(GTK_CONTAINER(table), GTK_WIDGET(table->button_box));

	g_signal_connect(G_OBJECT(table), "pressed",
			 G_CALLBACK(on_pressed), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->area), "draw",
			 G_CALLBACK(on_drawing_area_event), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->area), "motion_notify_event",
			 G_CALLBACK(on_motion_notify_event), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->area), "button_release_event",
			 G_CALLBACK(on_button_release_event), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->area), "button_press_event",
			 G_CALLBACK(on_button_release_event), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->area), "leave_notify_event",
			 G_CALLBACK(on_leave_event), static_cast<gpointer>(table));
	g_signal_connect(G_OBJECT(table->window), "key_press_event",
			 G_CALLBACK(on_key_event), static_cast<gpointer>(table));

	gtk_widget_set_events (GTK_WIDGET(table->area), GDK_EXPOSURE_MASK
						   | GDK_LEAVE_NOTIFY_MASK
						   | GDK_BUTTON_PRESS_MASK
						   | GDK_BUTTON_RELEASE_MASK
						   | GDK_POINTER_MOTION_MASK
						   | GDK_KEY_PRESS_MASK
						   | GDK_KEY_RELEASE_MASK);

	gtk_button_set_relief (GTK_BUTTON (table), GTK_RELIEF_NORMAL);
}


 GType
abi_table_get_type (void)
{
	static GType type = 0;

	if (!type)
	{
		static const GTypeInfo info =
			{
				sizeof (AbiTableClass),
				nullptr,		/* base_init */
				nullptr,		/* base_finalize */
				reinterpret_cast<GClassInitFunc>(abi_table_class_init),
				nullptr,		/* class_finalize */
				nullptr,		/* class_data */
				sizeof (AbiTable),
				0,		/* n_preallocs */
				reinterpret_cast<GInstanceInitFunc>(abi_table_init),
				nullptr
			};

		type = g_type_register_static (GTK_TYPE_BUTTON, "AbiTable", &info, static_cast<GTypeFlags>(0));
	}

	return type;
}

 GtkWidget*
abi_table_new (void)
{
	UT_DEBUGMSG(("COnstructing ABITABLE widget \n"));
	return GTK_WIDGET (g_object_new (abi_table_get_type (), nullptr));
}

