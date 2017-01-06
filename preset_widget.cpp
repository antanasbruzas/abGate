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

#include "preset_widget.h"

preset_widget::preset_widget(main_window *m_win, void (*set_parameters)(main_window *main_w, float th, float at, float ho, float de, float ra),
		void (*get_parameters)(main_window *main_w, float &th, float &at, float &ho, float &de, float &ra)) :
		load_button("Load"), save_button("Save"), delete_button("Delete"), presets_label("<span foreground='#111111' weight='heavy'>Presets:</span>") {

	// External functions
	global_get = get_parameters;
	global_set = set_parameters;

	main_win = m_win;

	m_fix.set_size_request(500, 40);

	/* ========== Colors ========== */
	Gdk::Color c_black("#111111");
	Gdk::Color c_green("#436d0d");
	Gdk::Color c_red("#870b0b");
	Gdk::Color c_dred("#4e0707");
	Gdk::Color c_dgreen("#273f09");

	modify_bg(STATE_NORMAL, c_green);

	/* ===== Background modifications ===== */
	load_button.modify_bg(STATE_NORMAL, c_black);
	load_button.modify_bg(STATE_ACTIVE, c_dgreen);
	load_button.modify_bg(STATE_PRELIGHT, c_green);

	save_button.modify_bg(STATE_NORMAL, c_black);
	save_button.modify_bg(STATE_ACTIVE, c_dgreen);
	save_button.modify_bg(STATE_PRELIGHT, c_green);

	delete_button.modify_bg(STATE_NORMAL, c_black);
	delete_button.modify_bg(STATE_ACTIVE, c_dred);
	delete_button.modify_bg(STATE_PRELIGHT, c_red);

	/* ===== Pango markup for the label ===== */
	presets_label.set_use_markup();

	m_box.pack_start(presets_label);
	m_box.pack_start(preset_combo);
	m_box.pack_start(load_button);
	m_box.pack_start(save_button);
	m_box.pack_start(delete_button);
	m_box.set_homogeneous(false);
	m_box.set_size_request(424, 28);
	m_box.set_spacing(2);
	m_fix.put(m_box, 80, 25);
	add(m_fix);
	show_all_children();

	/* ===== Moving to directories part ===== */
	home_dir = getenv("HOME");
	string cmd;
	dir = home_dir + FILE_NAME;
	const char *directory = (home_dir + "/.abGate").c_str();

	struct stat st;
	if (stat(directory, &st) != 0) {
		cmd = "mkdir " + home_dir + "/.abGate && touch " + dir;
		if (system(cmd.c_str())) {
			bool ignor = true;
		}
	}

	// Signals
	load_button.signal_clicked().connect(sigc::mem_fun(*this, &preset_widget::load_clicked));
	save_button.signal_clicked().connect(sigc::mem_fun(*this, &preset_widget::save_clicked));
	delete_button.signal_clicked().connect(sigc::mem_fun(*this, &preset_widget::delete_clicked));
}

void preset_widget::load_clicked() {
	Entry* entry = preset_combo.get_entry();
	if (entry) {
		ustring presetname = entry->get_text();
		if (!presetname.empty()) {
			presets *p_load = new presets();
			// Getting all the names
			vector<string> names_vector = p_load->get_names_xml(dir);
			// Checking if we already have this name
			bool have;
			for (int i = 0; i < names_vector.size(); i++) {
				if (presetname == names_vector[i]) {
					have = true;
					break;
				} else {
					have = false;
				}
			}
			if (have == true) {
				preset pre_load = p_load->get_one_xml(presetname, dir);
				global_set(main_win, pre_load.param_value[1], pre_load.param_value[2], pre_load.param_value[3], pre_load.param_value[4], pre_load.param_value[5]);
			}
		}
	}
}

void preset_widget::save_clicked() {
	Entry* entry = preset_combo.get_entry();
	if (entry) {
		ustring presetname = entry->get_text();
		if (!presetname.empty()) {
			presets *p_save = new presets();
			// Getting all the names
			vector<string> names_vector = p_save->get_names_xml(dir);
			// Checking if we already have this name
			bool have;
			for (int i = 0; i < names_vector.size(); i++) {
				if (presetname == names_vector[i]) {
					have = true;
					break;
				} else {
					have = false;
				}
			}

			if (have == true) {
				list<preset> preset_list = p_save->get_xml(dir);
				list<preset>::iterator it;

				for (it = preset_list.begin(); it != preset_list.end(); it++) {
					if (it->name == presetname) {
						preset_list.erase(it);
						break;
					}
				}

				// open and after close the file to clear the content of it
				ofstream presetsfile(dir.c_str());
				presetsfile.close();

				preset *pre_delete = new preset();
				for (it = preset_list.begin(); it != preset_list.end(); it++) {
					float parameters[6] = { it->param_value[0], it->param_value[1], it->param_value[2], it->param_value[3], it->param_value[4], it->param_value[5] };
					pre_delete->construct(it->name, parameters);
					p_save->set_xml(*pre_delete, false, dir);
				}
				// Adding new preset
				preset *pre_save = new preset();
				global_get(main_win, th, at, ho, de, ra);
				float parameters[6] = { 1, th, at, ho, de, ra };
				pre_save->construct(presetname, parameters);
				p_save->set_xml(*pre_save, false, dir);
			} else {
				// Adding new preset
				preset *pre_save = new preset();
				global_get(main_win, th, at, ho, de, ra);
				float parameters[6] = { 1, th, at, ho, de, ra };
				pre_save->construct(presetname, parameters);
				p_save->set_xml(*pre_save, false, dir);
				preset_combo.append_text(presetname);
			}
		}
	}
}

void preset_widget::delete_clicked() {
	Entry* entry = preset_combo.get_entry();
	if (entry) {
		ustring presetname = entry->get_text();
		if (!presetname.empty()) {
			presets *p_delete = new presets();
			// Getting all the names
			vector<string> names_vector = p_delete->get_names_xml(dir);
			// Checking if we already have this name
			bool have;
			for (int i = 0; i < names_vector.size(); i++) {
				if (presetname == names_vector[i]) {
					have = true;
					break;
				} else {
					have = false;
				}
			}
			if (have == true) {
				MessageDialog dialog((Window&) (*this->get_toplevel()), "Do you really want to delete preset <span weight='heavy'><i>" + presetname + "</i></span>?", true, MESSAGE_QUESTION,
						BUTTONS_OK_CANCEL);
				int result = dialog.run();
				if (result == RESPONSE_OK) {
					list<preset> preset_list = p_delete->get_xml(dir);
					list<preset>::iterator it;

					for (it = preset_list.begin(); it != preset_list.end(); it++) {
						if (it->name == presetname) {
							preset_list.erase(it);
							break;
						}
					}

					// open and after close the file to clear the content of it
					ofstream presetsfile(dir.c_str());
					presetsfile.close();

					preset *pre_delete = new preset();
					for (it = preset_list.begin(); it != preset_list.end(); it++) {
						float parameters[6] = { it->param_value[0], it->param_value[1], it->param_value[2], it->param_value[3], it->param_value[4], it->param_value[5] };
						pre_delete->construct(it->name, parameters);
						p_delete->set_xml(*pre_delete, false, dir);
					}
					preset_combo.remove_text(presetname);
					entry->set_text("");
				}
			}
		}
	}
}

void preset_widget::load_combo_list() {

	preset_combo.clear_items();
	ustring presetname;

	presets *p_names = new presets();
	vector<string> names_vector = p_names->get_names_xml(dir);

	for (int i = 0; i < names_vector.size(); i++) {
		presetname = names_vector[i];
		preset_combo.append_text(presetname);
	}
}

preset_widget::~preset_widget() {
}
