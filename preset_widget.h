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

#include <gtkmm/alignment.h>
#include <gtkmm/comboboxentrytext.h>
#include <gtkmm/messagedialog.h>
#include <gtkmm/fixed.h>

#include "presets.h"

#define FILE_NAME "/.abGate/presets.xml"

#ifndef  MAIN_WINDOW_WIDGET
#define MAIN_WINDOW_WIDGET
class main_window;
#endif

using namespace Glib;
using namespace Gtk;
using namespace std;

class preset_widget: public Alignment {
public:
	preset_widget(main_window *m_win, void (*set_parameters)(main_window *main_w, float th, float at, float ho, float de, float ra),
			void (*get_parameters)(main_window *main_w, float &th, float &at, float &ho, float &de, float &ra));

	virtual ~preset_widget();
	virtual void load_combo_list();

protected:
	HBox m_box;
	Fixed m_fix;
	Button load_button, save_button, delete_button;
	ComboBoxEntryText preset_combo;
	Label presets_label;

	virtual void load_clicked();
	virtual void save_clicked();
	virtual void delete_clicked();

	void (*global_set)(main_window *m_w, float th, float at, float ho, float de, float ra);
	void (*global_get)(main_window *m_w, float &th, float &at, float &ho, float &de, float &ra);

	float th, at, ho, de, ra;
	string home_dir, dir;
	main_window *main_win;
};
