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
#include <stdlib.h>
#include <lv2.h>
#include <cmath>

#include "gate_const.h"

#define CLOSED 1
#define ATTACK 2
#define OPENED 3
#define DECAY 4

static LV2_Descriptor *gateDescriptor = NULL;

class Gate {
public:
	Gate() {
		state = CLOSED;
		gate = 0;
		holding = 0;
	}
	~Gate() {
	}

	float *switch_button, *threshold, *attack, *hold, *decay, *range, sample_rate, gate, *output;
	const float *input;
	int state, holding;
};

static LV2_Handle instantiateGate(const LV2_Descriptor *descriptor, double s_rate, const char *path, const LV2_Feature * const * features) {
	Gate *plugin_data = new Gate;
	plugin_data->sample_rate = s_rate;
	return (LV2_Handle) plugin_data;
}

static void connectPortGate(LV2_Handle instance, uint32_t port, void *data) {
	Gate *plugin = (Gate *) instance;

	switch (port) {
	case p_switch:
		plugin->switch_button = (float*) data;
		break;
	case p_threshold:
		plugin->threshold = (float*) data;
		break;
	case p_attack:
		plugin->attack = (float*) data;
		break;
	case p_hold:
		plugin->hold = (float*) data;
		break;
	case p_decay:
		plugin->decay = (float*) data;
		break;
	case p_gaterange:
		plugin->range = (float*) data;
		break;
	case p_input:
		plugin->input = (const float*) data;
		break;
	case p_output:
		plugin->output = (float*) data;
		break;
	}
}

static void runGate(LV2_Handle instance, uint32_t sample_count) {

	Gate *plugin_data = (Gate *) instance;

	float * const output = plugin_data->output;
	const float * const input = plugin_data->input;

	// Checking bypass state
	float switch_button = *(plugin_data->switch_button);
	switch_button = switch_button < 0 ? 0 : switch_button;
	switch_button = switch_button > 1 ? 1 : switch_button;

	bool active = switch_button > 0;
	if (active) {

		// Getting port values
		const float threshold = *(plugin_data->threshold);
		const float attack = *(plugin_data->attack);
		const float hold = *(plugin_data->hold);
		const float decay = *(plugin_data->decay);
		const float range = *(plugin_data->range);

		const float sample_rate = plugin_data->sample_rate;

		const float threshold_value = pow(10, threshold * 0.05);
		const float attack_coef = 1000 / (attack * sample_rate);
		const int hold_samples = round(hold * sample_rate * 0.001);
		const float decay_coef = 1000 / (decay * sample_rate);
		const float range_coef = range > -90 ? pow(10, range * 0.05) : 0;

		int state = plugin_data->state;
		float gate = plugin_data->gate;
		int holding = plugin_data->holding;

		for (uint32_t i = 0; i < sample_count; ++i) {

			// Counting input dB
			float sample = input[i];
			float abs_sample = fabs(sample);

			switch (state) {
			case CLOSED:
			case DECAY:
				if (abs_sample >= threshold_value) {
					state = ATTACK;
				}
				break;
			case ATTACK:
				break;
			case OPENED:
				if (abs_sample >= threshold_value) {
					holding = hold_samples;
				} else if (holding <= 0) {
					state = DECAY;
				} else {
					holding--;
				}
				break;
			default:
				// shouldn't happen
				state = CLOSED;
			}

			// handle attack/decay in a second pass to avoid unnecessary one-sample delay
			switch (state) {
			case CLOSED:
				output[i] = sample * range_coef;
				break;
			case DECAY:
				gate -= decay_coef;
				if (gate <= 0) {
					gate = 0;
					state = CLOSED;
				}
				output[i] = sample * (range_coef * (1 - gate) + gate);
				break;
			case ATTACK:
				gate += attack_coef;
				if (gate >= 1) {
					gate = 1;
					state = OPENED;
					holding = hold_samples;
				}
				output[i] = sample * (range_coef * (1 - gate) + gate);
				break;
			case OPENED:
				output[i] = sample;
				break;
			}
		}

		plugin_data->gate = gate;
		plugin_data->state = state;
		plugin_data->holding = holding;
	} else {
		// Bypassing
		if (output != input) {
			for (uint32_t i = 0; i < sample_count; ++i) {
				output[i] = input[i];
			}
		}
	}
}

static void cleanupGate(LV2_Handle instance) {
	Gate *plugin_data = (Gate *) instance;
	delete plugin_data;
}

static void init() {

	gateDescriptor = (LV2_Descriptor *) malloc(sizeof(LV2_Descriptor));
	gateDescriptor->URI = p_uri;
	gateDescriptor->instantiate = instantiateGate;
	gateDescriptor->connect_port = connectPortGate;
	gateDescriptor->activate = NULL;
	gateDescriptor->run = runGate;
	gateDescriptor->deactivate = NULL;
	gateDescriptor->cleanup = cleanupGate;
	gateDescriptor->extension_data = NULL;
}

LV2_SYMBOL_EXPORT
const LV2_Descriptor *lv2_descriptor(uint32_t index) {

	if (!gateDescriptor) {
		init();
	}

	switch (index) {
	case 0:
		return gateDescriptor;
	default:
		return NULL;
	}
}
