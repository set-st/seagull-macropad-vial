#include QMK_KEYBOARD_H

oled_rotation_t oled_init_user(oled_rotation_t rotation) { return OLED_ROTATION_270; }

// External state from keymap.c
extern volatile uint16_t key_matrix_state;
extern volatile uint8_t  wave_step;
extern volatile uint8_t  wave_mode;
extern volatile uint32_t wave_timer;

static void render_layer_info(void) {
    oled_write_P(PSTR("LAYER"), false);
    
    static const char PROGMEM default_layer[] = {0x20, 0x97, 0x98, 0x99, 0x20, 0x20, 0xb7, 0xb8, 0xb9, 0x20, 0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
    static const char PROGMEM raise_layer[]   = {0x20, 0x94, 0x95, 0x96, 0x20, 0x20, 0xb4, 0xb5, 0xb6, 0x20, 0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
    static const char PROGMEM lower_layer[]   = {0x20, 0x9a, 0x9b, 0x9c, 0x20, 0x20, 0xba, 0xbb, 0xbc, 0x20, 0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
    static const char PROGMEM adjust_layer[]  = {0x20, 0x9d, 0x9e, 0x9f, 0x20, 0x20, 0xbd, 0xbe, 0xbf, 0x20, 0x20, 0xdd, 0xde, 0xdf, 0x20, 0};

    uint8_t layer = get_highest_layer(layer_state);
    oled_set_cursor(0, 1);
    if (layer == 3) oled_write_P(adjust_layer, false);
    else if (layer == 2) oled_write_P(lower_layer, false);
    else if (layer == 1) oled_write_P(raise_layer, false);
    else oled_write_P(default_layer, false);

    const char *layer_names[] = {"MACRO", "NUM  ", "GAMES", "SETGS"};    
    oled_set_cursor(0, 5);
    if (layer < 4) oled_write(layer_names[layer], false);
}

static void render_status(void) {
    led_t led_state = host_keyboard_led_state();
    oled_set_cursor(0, 7);
    oled_write_P(led_state.num_lock ? PSTR("NUM ") : PSTR("    "), false);
    oled_set_cursor(0, 8);
    oled_write_P(led_state.caps_lock ? PSTR("CAPS") : PSTR("    "), false);
}

static void render_key_matrix(void) {
    // Animation progress
    uint16_t duration = (wave_mode == 1) ? 80 : 100;
    if (wave_step > 0 && timer_elapsed32(wave_timer) > duration) {
        wave_step++;
        wave_timer = timer_read32();
        if (wave_step > 4) {
            wave_step = 0;
            wave_mode = 0;
        }
    }

    for (uint8_t row = 0; row < 4; row++) {
        for (uint8_t col = 0; col < 4; col++) {
            bool is_active = false;
            if (wave_step > 0) {
                if (wave_mode == 1) { // Center
                    bool is_center = (row >= 1 && row <= 2 && col >= 1 && col <= 2);
                    if (wave_step == 1 && is_center) is_active = true;
                    if (wave_step == 2 && !is_center) is_active = true;
                    if (wave_step == 3) is_active = true;
                } else if (wave_mode == 2) { // Right
                    if (wave_step - 1 == col) is_active = true;
                } else if (wave_mode == 3) { // Left
                    if (4 - wave_step == col) is_active = true;
                }
            } else {
                is_active = (key_matrix_state & (1 << (row * 4 + col)));
            }

            uint8_t sx = col * 8;
            uint8_t sy = row * 8 + 90;
            for (uint8_t x = 0; x < 7; x++) {
                for (uint8_t y = 0; y < 7; y++) {
                    oled_write_pixel(sx + x, sy + y, is_active);
                }
            }
        }
    }
}

bool oled_task_user(void) {
    // Hardware-level encoder polling (Pins B4 and B5 from info.json)
    static uint8_t last_state = 0;
    uint8_t current_state = (readPin(B4) << 1) | readPin(B5);
    
    if (current_state != last_state) {
        if ((last_state == 0 && current_state == 2) || 
            (last_state == 2 && current_state == 3) || 
            (last_state == 3 && current_state == 1) || 
            (last_state == 1 && current_state == 0)) {
            wave_mode = 2; // Right
            wave_step = 1;
            wave_timer = timer_read32();
        } else if ((last_state == 0 && current_state == 1) || 
                   (last_state == 1 && current_state == 3) || 
                   (last_state == 3 && current_state == 2) || 
                   (last_state == 2 && current_state == 0)) {
            wave_mode = 3; // Left
            wave_step = 1;
            wave_timer = timer_read32();
        }
        last_state = current_state;
    }

    render_layer_info();
    render_status();
    render_key_matrix();
    return false;
}
