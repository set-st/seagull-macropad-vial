/* Copyright 2021 gkeyboard
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  See <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

// State variables for OLED visualization
volatile uint16_t key_matrix_state = 0;
volatile uint8_t  wave_step = 0;
volatile uint8_t  wave_mode = 0; // 1: center, 2: right, 3: left
volatile uint32_t wave_timer = 0;

#define _BL 0 
#define _______ KC_TRNS

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BL] = LAYOUT(
        KC_KP_7, KC_KP_8, KC_KP_9, KC_KP_SLASH,       
        KC_KP_4, KC_KP_5, KC_KP_6, KC_KP_ASTERISK,
        KC_KP_1, KC_KP_2, KC_KP_3, KC_KP_MINUS, TO(1),
        KC_KP_0, KC_KP_DOT, KC_KP_EQUAL, KC_KP_PLUS
    )
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [0] = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) }
};
#endif

// Trigger animation on rotation
bool encoder_update_user(uint8_t index, bool clockwise) {
    wave_mode = clockwise ? 2 : 3;
    wave_step = 1;
    wave_timer = timer_read32();
    return true; // Allow volume control to work
}

bool dip_switch_update_user(uint8_t index, bool active) {
    if (index == 0 && active) tap_code(KC_MUTE);
    return true;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    uint8_t row = record->event.key.row;
    uint8_t col = record->event.key.col;

    // Encoder button (Row 2, Col 4 based on info.json)
    if (row == 2 && col == 4) {
        if (record->event.pressed) {
            wave_mode = 1;
            wave_step = 1;
            wave_timer = timer_read32();
        }
        return true;
    }

    // Main 4x4 grid (Rows 0-3, Cols 0-3)
    if (row < 4 && col < 4) {
        uint8_t key_index = row * 4 + col;
        if (record->event.pressed) {
            key_matrix_state |= (1 << key_index);
        } else {
            key_matrix_state &= ~(1 << key_index);
        }
    }
    return true;
}

#ifdef OLED_ENABLE
#include "oled.c"
#endif
