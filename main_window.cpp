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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <iostream>
#include "main_window.h"

// Presets
//--------------------------------------------------------------------------------------
void global_set_params(main_window *main_ptr, float th, float at, float ho, float de, float ra) {
	main_ptr->set_all(th, at, ho, de, ra);
}

void global_get_params(main_window *main_ptr, float &th, float &at, float &ho, float &de, float &ra) {
	main_ptr->get_all(th, at, ho, de, ra);
}
//--------------------------------------------------------------------------------------

main_window::main_window() {

	// Asking Glib if it is already handling threads and if it isn’t we initialize the threading system.
	// This has the additional perk of aborting the program if threads are not supported at all. 
	if (!Glib::thread_supported()) {
		Glib::thread_init();
	}

	// Changing the background
	signal_realize().connect(mem_fun(*this, &main_window::bg), false);

	// Slots	
	slot<void> bypass_slot, threshold_slot, attack_slot, hold_slot, decay_slot, gaterange_slot;
	// Will be used to update LV2 port values
	bypass_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_switch), mem_fun(*this, &main_window::get_bypass));
	threshold_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_threshold), mem_fun(*this, &main_window::get_threshold));
	attack_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_attack), mem_fun(*this, &main_window::get_attack));
	hold_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_hold), mem_fun(*this, &main_window::get_hold));
	decay_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_decay), mem_fun(*this, &main_window::get_decay));
	gaterange_slot = compose(bind < 0 > (mem_fun(*this, &main_window::write_control), p_gaterange), mem_fun(*this, &main_window::get_gaterange));

	// Bypass Button
	bypass = manage(new toggle(bypass_slot));

	// knobs
	threshold = manage(new knob(threshold_slot, THRESHOLD_DEFAULT, THRESHOLD_MIN,
	THRESHOLD_MAX, 0.01));
	attack = manage(new knob(attack_slot, ATTACK_DEFAULT, ATTACK_MIN, ATTACK_MAX, 0.01));
	hold = manage(new knob(hold_slot, HOLD_DEFAULT, HOLD_MIN, HOLD_MAX, 0.01));
	decay = manage(new knob(decay_slot, DECAY_DEFAULT, DECAY_MIN, DECAY_MAX, 0.01));
	gaterange = manage(new knob(gaterange_slot, RANGE_DEFAULT, RANGE_MIN, RANGE_MAX, 0.01));

	// Using Dispatcher to redraw
	bypass->dis_sig.connect(mem_fun(*this, &main_window::queue_draw));
	threshold->sig_done.connect(mem_fun(*this, &main_window::queue_draw));
	attack->sig_done.connect(mem_fun(*this, &main_window::queue_draw));
	hold->sig_done.connect(mem_fun(*this, &main_window::queue_draw));
	decay->sig_done.connect(mem_fun(*this, &main_window::queue_draw));
	gaterange->sig_done.connect(mem_fun(*this, &main_window::queue_draw));

	// Skins
	//skins_button.signal_clicked().connect(sigc::mem_fun(*this, &main_window::skins_clicked));

// Presets
//-----------------------------------------------------------------------------------------------
	m_presets = manage(new preset_widget(this, &global_set_params, &global_get_params));
	main_box.pack_start(*m_presets);
	m_presets->load_combo_list();
//-----------------------------------------------------------------------------------------------

	// Adding knobs in the fixed position
	button_fix.put(*threshold, 20, 8);
	button_fix.put(*attack, 120, 8);
	button_fix.put(*hold, 220, 8);
	button_fix.put(*decay, 320, 8);
	button_fix.put(*gaterange, 420, 8);
	button_fix.put(*bypass, 520, 8);
	//button_fix.put(skins_button, 600, 110);

	// Adding to the main box
	main_box.pack_start(button_fix);
	// Setting the size of the box
	main_box.set_size_request(640, 200);

	// Adding to the GUI
	add(main_box);
	show_all_children();
}

// Methods to get the values of knobs and bypass
float main_window::get_bypass() {
	return bypass->get_toggle_value();
}
float main_window::get_threshold() {
	return threshold->get_knob_value();
}
float main_window::get_attack() {
	return attack->get_knob_value();
}
float main_window::get_hold() {
	return hold->get_knob_value();
}
float main_window::get_decay() {
	return decay->get_knob_value();
}
float main_window::get_gaterange() {
	return gaterange->get_knob_value();
}

// Changing background image and window resizable settings
void main_window::bg() {
	Window *this_win = &((Window&) (*this->get_toplevel()));
	this_win->set_resizable(false);
	this->modify_bg_pixmap(STATE_NORMAL, BACKGROUND_PNG);
}

void main_window::write_control(uint32_t port_index, float value) {
	write_function(controller, port_index, sizeof(float), 0, &value);
}

// Presets
//------------------------------------------------------------------------
void main_window::set_all(float th, float at, float ho, float de, float ra) {
	threshold->set_knob_value(th);
	attack->set_knob_value(at);
	hold->set_knob_value(ho);
	decay->set_knob_value(de);
	gaterange->set_knob_value(ra);
}

void main_window::get_all(float &th, float &at, float &ho, float &de, float &ra) {
	th = get_threshold();
	at = get_attack();
	ho = get_hold();
	de = get_decay();
	ra = get_gaterange();
}
//------------------------------------------------------------------------

// Destructor
main_window::~main_window() {
}
