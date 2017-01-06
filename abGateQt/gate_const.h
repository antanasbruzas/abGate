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
#define p_output 0
#define p_input 1
#define p_switch 2
#define p_threshold 3
#define p_attack 4
#define p_hold 5
#define p_decay 6
#define p_gaterange 7
#define p_n_ports 8

#define BYPASS_MIN 0
#define BYPASS_MAX 1
#define BYPASS_DEFAULT 0

#define THRESHOLD_MIN -70
#define THRESHOLD_MAX 12
#define THRESHOLD_DEFAULT -70

#define ATTACK_MIN 0.1
#define ATTACK_MAX 500
#define ATTACK_DEFAULT 30

#define HOLD_MIN 5
#define HOLD_MAX 3000
#define HOLD_DEFAULT 500

#define DECAY_MIN 5
#define DECAY_MAX 4000
#define DECAY_DEFAULT 1000

#define RANGE_MIN -90
#define RANGE_MAX -20
#define RANGE_DEFAULT -90

#define p_uri "http://hippie.lt/lv2/gate"
