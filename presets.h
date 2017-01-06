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

#include <iostream>
#include <fstream>
#include <cstring>
#include <list>
#include <vector>

#include "preset.h"

using namespace std;

class presets {

public:
	presets();
	virtual ~presets();

	vector<string> get_names_xml(string dir);
	list<preset> get_xml(string dir);
	preset get_one_xml(string name, string dir);
	bool set_xml(preset new_preset, bool clr, string dir);

protected:
	string line, preset_name;
	char *float_string;
	float param_value[5];
	long pos;
	size_t found_preset, found_param, line_value;

	vector<string> names_vector;
	list<preset> presets_list;
	preset *tmp_preset;

	int line_nr, preset_nr;
};
