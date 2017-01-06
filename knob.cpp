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

#include <cmath>

#include "knob.h"

using namespace std;

knob::knob(const sigc::slot<void> knob_slot, double default_value, double min, double max, double step) :
		current_frame(0), all_frames(50), a_knb(new Adjustment(default_value, min, max, step, 1.0, 0)) {
	set_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK | Gdk::POINTER_MOTION_MASK);
	set_double_buffered(true);

	pixbuf = Gdk::Pixbuf::create_from_file(KNOB_PNG);
	connecting(a_knb, knob_slot);

	set_picture_size(100, 100);
	value_changed();
}

knob::~knob() {
}

void knob::set_picture_size(int x, int y) {
	width = x;
	center_x = (gint)(x * 0.5);
	height = y;
	center_y = (gint)(y * 0.5);
	set_size_request(x, y);
}

void knob::connecting(Adjustment* knb, const sigc::slot<void> slot) {
	a_knb = knb;

	// Triggering value_changed method when value of the knob changes
	a_knb->signal_value_changed().connect(mem_fun(*this, &knob::value_changed));

	// Updating LV2 port values when value of the knob changes
	a_knb->signal_value_changed().connect(slot);
	value_changed();
}

void knob::value_changed() {

	// Calculating new frame from the knob value
	new_frame = (int) (-0.5 + all_frames * (a_knb->get_value() - a_knb->get_lower()) / (a_knb->get_upper() - a_knb->get_lower()));

	if (current_frame != new_frame) {
		current_frame = new_frame;

		// Also can be used like this: sig_done.emit()
		sig_done();
	}
}

bool knob::on_expose_event(GdkEventExpose *ev) {

	if (pixbuf && ev) {
		// Drawing new pixbuf
		m_drawable = get_window();
		m_drawable->draw_pixbuf(get_style()->get_black_gc(), pixbuf, get_width(), get_height() * current_frame - 1, 0, 0, get_width(), get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
		// This one is deprecated: pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(), get_width(), get_height()*current_frame-1, 0, 0, get_width(), get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
	}
	return TRUE;
}

bool knob::on_button_press_event(GdkEventButton *ev) {
	widget_x = (gint)(ev->x_root - ev->x);
	widget_y = (gint)(ev->y_root - ev->y);

	gint x, y;
	GdkModifierType modifiers;
	gdk_window_get_pointer(ev->window, &x, &y, &modifiers);

	switch (ev->button) {
	case 1:
		if (modifiers & GDK_CONTROL_MASK) {
			a_knb->set_value((a_knb->get_lower() + a_knb->get_upper()) / 2);
		} else {
			add_modal_grab();
			mouse_pos_change((gint) ev->x_root, (gint) ev->y_root);
		}
		break;
	case 4: // mouse wheel up
		a_knb->set_value((a_knb->get_value() + a_knb->get_step_increment()));
		break;
	case 5: // mouse wheel down
		a_knb->set_value((a_knb->get_value() - a_knb->get_step_increment()));
		break;
	default:
		break;
	}
	return true;
}

bool knob::on_button_release_event(GdkEventButton *) {
	if (has_grab()) {
		remove_modal_grab();
	}
	return true;
}

bool knob::on_motion_notify_event(GdkEventMotion *ev) {
	if (has_grab()) {
		mouse_pos_change((gint) ev->x_root, (gint) ev->y_root);
	}
	return true;
}

void knob::mouse_pos_change(gint x_abs, gint y_abs) {
	gfloat x = x_abs - (widget_x + center_x);
	gfloat y = y_abs - (widget_y + center_y);

	gfloat angle = atan(y / x);

	// map angle to range [0-2*PI] (0-360 degrees)
	if (x < 0) {
		angle += M_PI;
	} else if (y <= 0) {
		angle += 2 * M_PI;
	}

	// map so 0 degrees/rads is at the bottom of the dial
	angle -= M_PI / 2;
	if ((x > 0) && (y > 0)) {
		angle += 2 * M_PI;
	}

	// map to range 30deg - 330 deg
	if (angle < M_PI / 6) {
		angle = M_PI / 6;
	} else if (angle > 11 * M_PI / 6) {
		angle = M_PI * 11 / 6;
	}

	// set adjustment value accordingly
	float val = (angle - M_PI / 6) / (M_PI * 10 / 6);
	val *= (a_knb->get_upper() - a_knb->get_lower());
	val += (a_knb->get_lower());

	a_knb->set_value(val);
}

// Returns knob value
float knob::get_knob_value() {
	return a_knb->get_value();
}

// Sets knob value
void knob::set_knob_value(float value) {
	a_knb->set_value(value);
}
