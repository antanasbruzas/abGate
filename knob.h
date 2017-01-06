/* abGate - LV2 Noise Gate Plugin
 *
 * Copyright 2011 Antanas Bružas
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef _KNOB_H
#define _KNOB_H

#include "plugin_configuration.h"

#include <gtkmm/adjustment.h>
#include <gtkmm/misc.h>

using namespace Gtk;

class knob: public Misc {
public:
	knob(const sigc::slot<void> knob_slot, double default_value, double min, double max, double step);
	~knob();

	void set_picture_size(int x, int y);

	void connecting(Adjustment*, const sigc::slot<void> slot);
	void value_changed();

	float get_knob_value();
	void set_knob_value(float value);

	// Dispacher used to update GUI from the main (GUI) thread
	Glib::Dispatcher sig_done;

	int new_frame;

protected:
	virtual bool on_expose_event(GdkEventExpose*);
	virtual bool on_button_press_event(GdkEventButton*);
	virtual bool on_button_release_event(GdkEventButton*);
	virtual bool on_motion_notify_event(GdkEventMotion*);

private:
	void mouse_pos_change(gint x, gint y);
	Glib::RefPtr<Gdk::Pixbuf> pixbuf;
	Glib::RefPtr<Gdk::Drawable> m_drawable;
	Adjustment *a_knb;
	gint current_frame, all_frames, width, height, center_x, center_y, widget_x, widget_y;

};

#endif
