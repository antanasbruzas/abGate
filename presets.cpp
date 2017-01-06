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

/* DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY DIRTY */

#include "presets.h"

presets::presets() {
}

presets::~presets() {
}

vector<string> presets::get_names_xml(string dir) {

	ifstream presetsfile(dir.c_str());

	if (presetsfile.is_open()) {
		while (getline(presetsfile, line)) {
			found_preset = line.rfind("\">");
			if (found_preset != string::npos) {
				names_vector.push_back(line.substr(30, line.length() - 32));
			}
		}
		presetsfile.close();
	} else {
		cerr << "Unable to open file";
	}

	return names_vector;
}

list<preset> presets::get_xml(string dir) {

	line_nr = 0;
	preset_nr = 0;

	ifstream presetsfile(dir.c_str());

	if (presetsfile.is_open()) {
		while (getline(presetsfile, line)) {
			found_preset = line.rfind("\">");
			found_param = line.rfind("\" />");

			if (found_preset != string::npos) {
				line_nr = 0;
				preset_name = line.substr(30, line.length() - 32);
				preset_nr++;
			} else if (found_param != string::npos) {
				line_value = line.find("value");
				pos = (int) line_value;

				float_string = new char[line.substr(pos + 7, line.length() - pos - 11).length() + 1];
				strcpy(float_string, line.substr(pos + 7, line.length() - pos - 11).c_str());
				sscanf(float_string, "%f", &param_value[line_nr]);

				line_nr++;

				if (line_nr == 6) {
					tmp_preset = new preset();
					tmp_preset->construct(preset_name, param_value);
					presets_list.push_back(*tmp_preset);
				}
			}
		}
		presetsfile.close();
	} else {
		cerr << "Unable to open file";
	}

	return presets_list;
}

preset presets::get_one_xml(string name, string dir) {

	ifstream presetsfile(dir.c_str());
	tmp_preset = new preset();
	if (presetsfile.is_open()) {
		while (getline(presetsfile, line)) {
			found_preset = line.rfind("\"" + name + "\"");
			if (found_preset != string::npos) {

				for (int i = 0; i < 6; i++) {
					getline(presetsfile, line);
					line_value = line.find("value");
					pos = (int) line_value;

					float_string = new char[line.substr(pos + 7, line.length() - pos - 11).length() + 1];
					strcpy(float_string, line.substr(pos + 7, line.length() - pos - 11).c_str());
					sscanf(float_string, "%f", &param_value[i]);

					if (i == 5) {
						tmp_preset->construct(name, param_value);
						return *tmp_preset;
					}
				}
			}
		}
		presetsfile.close();

	} else {
		cerr << "Unable to open file";
	}
	return *tmp_preset;
}

bool presets::set_xml(preset new_preset, bool clr, string dir) {

	ofstream presetsfile;

	if (clr == true) {
		presetsfile.open(dir.c_str());
	} else {
		presetsfile.open(dir.c_str(), ios::app);
	}

	if (presetsfile.is_open()) {

		presetsfile << "<preset plugin=\"abgate\" name=\"" << new_preset.name << "\">\n";
		presetsfile << "\t<param name=\"switch\" value=\"" << new_preset.param_value[0] << "\" />\n";
		presetsfile << "\t<param name=\"threshold\" value=\"" << new_preset.param_value[1] << "\" />\n";
		presetsfile << "\t<param name=\"attack\" value=\"" << new_preset.param_value[2] << "\" />\n";
		presetsfile << "\t<param name=\"hold\" value=\"" << new_preset.param_value[3] << "\" />\n";
		presetsfile << "\t<param name=\"decay\" value=\"" << new_preset.param_value[4] << "\" />\n";
		presetsfile << "\t<param name=\"gaterange\" value=\"" << new_preset.param_value[5] << "\" />\n";
		presetsfile << "</preset>\n";

		presetsfile.close();

	} else {
		cerr << "Unable to open file";
		return false;
	}

	return true;
}

/* ==================== Examples ==================== */

/*
 // isprintina visus names

 presets *test = new presets();
 vector<string>names_vector = test->get_names_xml();

 for (int i = 0; i < names_vector.size(); i++) {
 cout << names_vector[i] << endl;
 }

 */
// isprintina visa xml
/*
 presets *test = new presets();
 list<preset>preset_list = test->get_xml();
 list<preset>::iterator it;

 for (it = preset_list.begin(); it != preset_list.end(); it++) {
 cout << it->name << endl;
 cout << it->param_value[0] << endl;
 cout << it->param_value[1] << endl;
 cout << it->param_value[2] << endl;
 cout << it->param_value[3] << endl;
 cout << it->param_value[4] << endl;
 cout << it->param_value[5] << endl;

 }
 */

// senas su vektoriais
/*presets *test = new presets();
 vector<preset>preset_vector = test->get_xml();
 for (int i = 0; i < preset_vector.size(); i++) {
 cout << preset_vector[i].name << endl;
 cout << preset_vector[i].param_value[0] << endl;
 cout << preset_vector[i].param_value[1] << endl;
 cout << preset_vector[i].param_value[2] << endl;
 cout << preset_vector[i].param_value[3] << endl;
 cout << preset_vector[i].param_value[4] << endl;
 cout << preset_vector[i].param_value[5] << endl;
 }*/

// iterpia nauja preseta, priklausomai nuo true false parametro įterps arba į galą arba viską ištrins ir įterps
/*
 presets *testas = new presets();
 preset *presetas666 = new preset();
 float parametrai[6] = { 1, -5, 250, 200, 600, -30 };
 presetas666->construct("zjbisa", parametrai);
 testas->set_xml(*presetas666, false);
 */

// išprintina vieną presetą pagal name
/*presets *test = new presets();
 preset presetas1 = test->get_one_xml("zjbis ne");

 cout << presetas1.name << endl;
 cout << presetas1.param_value[0] << endl;
 cout << presetas1.param_value[1] << endl;
 cout << presetas1.param_value[2] << endl;
 cout << presetas1.param_value[3] << endl;
 cout << presetas1.param_value[4] << endl;
 cout << presetas1.param_value[5] << endl;

 */

/*
 // paima visa xml, ištrina vieną presetą pagal name ir updeitina visa xml

 presets *test = new presets();
 list<preset>preset_list = test->get_xml();

 list<preset>::iterator it;

 for (it = preset_list.begin(); it != preset_list.end(); it++) {
 if (it->name == "Defaulta") { preset_list.erase(it); break; }
 }

 // opening and after closing the file to clear the content of it
 ofstream presetsfile("presets.xml");
 presetsfile.close();

 preset *presetas = new preset();
 for (it = preset_list.begin(); it != preset_list.end(); it++) {
 float parametrai[6] = { it->param_value[0], it->param_value[1], it->param_value[2], it->param_value[3], it->param_value[4], it->param_value[5] };
 presetas->construct(it->name, parametrai);
 test->set_xml(*presetas, false);
 }
 */

