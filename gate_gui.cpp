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
#include <gtkmm/main.h>

#include "main_window.h"

static LV2UI_Descriptor *gate_guiDescriptor = NULL;

static LV2_Handle instantiateGate_gui(const _LV2UI_Descriptor *descriptor, const char *plugin_uri, const char *bundle_path, LV2UI_Write_Function write_function, LV2UI_Controller controller,
		LV2UI_Widget *widget, const LV2_Feature * const *features) {

	Gtk::Main::init_gtkmm_internals();

	main_window* gui_data = new main_window();
	gui_data->controller = controller;
	gui_data->write_function = write_function;

	*widget = gui_data->gobj();

	return (LV2_Handle) gui_data;
}

static void cleanupGate_gui(LV2UI_Handle instance) {
	delete static_cast<main_window*>(instance);
	//free(instance);
}

static void portEventGate_gui(LV2UI_Handle ui, uint32_t port_index, uint32_t buffer_size, uint32_t format, const void *buffer) {
	main_window *gui = (main_window *) ui;
	gui->gui_port_event(ui, port_index, buffer_size, format, buffer);
}

static void init_gui() {

	gate_guiDescriptor = (LV2UI_Descriptor *) malloc(sizeof(LV2UI_Descriptor));
	gate_guiDescriptor->URI = "http://hippie.lt/lv2/gate/gui";
	gate_guiDescriptor->instantiate = instantiateGate_gui;
	gate_guiDescriptor->cleanup = cleanupGate_gui;
	gate_guiDescriptor->port_event = portEventGate_gui;
	gate_guiDescriptor->extension_data = NULL;

}

//LV2_SYMBOL_EXPORT
const LV2UI_Descriptor *lv2ui_descriptor(uint32_t index) {

	if (!gate_guiDescriptor) {
		init_gui();
	}

	switch (index) {
	case 0:
		return gate_guiDescriptor;
	default:
		return NULL;
	}
}

