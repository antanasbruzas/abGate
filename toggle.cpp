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

#include "toggle.h"

using namespace std;

toggle::toggle(const sigc::slot<void> toggle_slot) :
		a_tog(new Adjustment(0, 0, 1, 1, 1, 0)) {

	set_events(Gdk::EXPOSURE_MASK | Gdk::BUTTON_PRESS_MASK | Gdk::BUTTON_RELEASE_MASK);
	set_double_buffered(true);

	// Setting bypass images
	on = Gdk::Pixbuf::create_from_file(BYPASS_ON_PNG);
	off = Gdk::Pixbuf::create_from_file(BYPASS_OFF_PNG);
	pixbuf = off;

	connecting(a_tog, toggle_slot);
	set_size_request(100, 100);
}

toggle::~toggle() {
}

void toggle::connecting(Adjustment* tog, const sigc::slot<void> slot) {
	a_tog = tog;

	// Triggering value_changed method when value of the toggle changes
	a_tog->signal_value_changed().connect(mem_fun(*this, &toggle::value_changed));

	// Updating LV2 port value when value of bypass changes
	a_tog->signal_value_changed().connect(slot);
}

void toggle::value_changed() {

	// Setting bypass value to either 1 or 0
	float value = a_tog->get_value() > 0 ? 1 : 0;

	// Changing the image of the toggle
	pixbuf = value > 0 ? on : off;

	// Also can be used like this: dis_sig.emit()
	dis_sig();
}

bool toggle::on_expose_event(GdkEventExpose *event) {

	if (event && pixbuf) {
		// Drawing new pixbuf
		m_drawable = get_window();
		m_drawable->draw_pixbuf(get_style()->get_black_gc(), pixbuf, get_width(), -1, 0, 0, get_width(), get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
		// This one is deprecated: pixbuf->render_to_drawable(get_window(), get_style()->get_black_gc(), get_width(), -1, 0, 0, get_width(), get_height(), Gdk::RGB_DITHER_NONE, 0, 0);
	}
	return true;
}

bool toggle::on_button_press_event(GdkEventButton *event) {

	// Updating bypass value
	float value = a_tog->get_value() > 0 ? 0 : 1;
	a_tog->set_value(value);
	return true;
}

bool toggle::on_button_release_event(GdkEventButton *event) {
	if (has_grab()) {
		remove_modal_grab();
	}
	return true;
}

// Returns bypass value
float toggle::get_toggle_value() {
	return a_tog->get_value();
}

// Sets bypass value
void toggle::set_toggle_value(float value) {
	a_tog->set_value(value);
}
