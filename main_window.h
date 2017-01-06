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

#include "plugin_configuration.h"
#include <lv2.h>
#include "ui.h"
#include <gtkmm/image.h>
#include <gtkmm/fixed.h>
#include <gtkmm/box.h>
#include <gtkmm/window.h>
#include <gtkmm/widget.h>
#include <gtkmm/eventbox.h>

#include "knob.h"
#include "toggle.h"
#include "gate_const.h"
//-----
#include "preset_widget.h"
//-------
using namespace sigc;
using namespace Gtk;

class main_window: public EventBox {

public:
	main_window();
	virtual ~main_window();

	// Presets
	//-------------------------------------------------------------------------
	void set_all(float th, float at, float ho, float de, float ra);
	void get_all(float &th, float &at, float &ho, float &de, float &ra);
	//-------------------------------------------------------------------------

	// Informing GUI about changes in the control ports
	void gui_port_event(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void * buffer) {

		float val;
		val = *static_cast<const float*>(buffer);

		// Checking if params are the same as specified in the LV2 documentation
		if (format != 0) {
			return;
		}
		if (buffer_size != 4) {
			return;
		}

		// Updating values for GUI changes
		switch (port_index) {
		case p_switch:
			bypass->set_toggle_value(val);
			break;
		case p_threshold:
			threshold->set_knob_value(val);
			break;
		case p_attack:
			attack->set_knob_value(val);
			break;
		case p_hold:
			hold->set_knob_value(val);
			break;
		case p_decay:
			decay->set_knob_value(val);
			break;
		case p_gaterange:
			gaterange->set_knob_value(val);
			break;
		default:
			return;
		}
	}

	LV2UI_Controller controller;
	LV2UI_Write_Function write_function;

	void write_control(uint32_t port_index, float value);

private:
	float get_bypass();
	float get_closethr();
	float get_threshold();
	float get_attack();
	float get_hold();
	float get_decay();
	float get_gaterange();

	// Setting some bg params
	void bg();

	VBox main_box;
	Fixed button_fix;
	knob *threshold, *attack, *hold, *decay, *gaterange;
	toggle *bypass;

	// Presets
	//-----------------------
	preset_widget *m_presets;
	//-----------------------
};
