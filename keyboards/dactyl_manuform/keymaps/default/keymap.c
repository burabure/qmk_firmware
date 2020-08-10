/* Copyright 2019 Nicolas <BuraBure> Fernandez
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include QMK_KEYBOARD_H

#define CLMK 0
#define QWER 1
#define GAME 2
#define ARKY 3
#define NUMK 4
#define SYMB 5
#define MOUS 6
#define SYST 7

bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;
uint16_t last_song_played;

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    A_TAB_F = SAFE_RANGE,
    A_TAB_B,
    SW_DE_F,
    SW_DE_B,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [CLMK] = LAYOUT(
        KC_ESC,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                        KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    KC_MINS, \
        KC_TAB,  KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,                                        KC_J,   KC_L,   KC_U,    KC_Y,   KC_SCLN, KC_EQL,  \
        KC_BSPC, KC_A,    KC_R,    KC_S,    KC_T,    KC_D,                                        KC_H,   KC_N,   KC_E,    KC_I,   KC_O,    KC_QUOT, \
        KC_DEL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                        KC_K,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, TO(SYST),\
                          A_TAB_B, A_TAB_F, KC_ENT,  KC_SPC, KC_LCTL, KC_LSFT,  KC_LWIN, KC_LSFT, KC_SPC, KC_ENT, TG(MOUS),TG(NUMK), \
                                                             TT(ARKY),KC_LALT,  KC_ALGR, TT(SYMB) \
    ),

    [QWER] = LAYOUT(
        _______, KC_1,    KC_2,    KC_3,    KC_4,    KC_5,                                         KC_6,   KC_7,   KC_8,    KC_9,   KC_0,    _______, \
        _______, KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                         KC_Y,   KC_U,   KC_I,    KC_O,   KC_P,    _______, \
        _______, KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                         KC_H,   KC_J,   KC_K,    KC_L,   KC_SCLN, _______, \
        _______, KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                         KC_N,   KC_M,   KC_COMM, KC_DOT, KC_SLSH, _______, \
                          _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, \
                                                              _______, _______,  _______, _______ \
    ),

    [GAME] = LAYOUT(
        KC_ESC,  KC_GRV, KC_1,    KC_2,   KC_3,    KC_4,                                        _______, _______, _______, _______, _______, _______, \
        KC_TAB,  KC_T,   KC_Q,    KC_W,   KC_E,    KC_R,                                        _______, _______, _______, _______, _______, _______, \
        KC_BSPC, KC_G,   KC_A,    KC_S,   KC_D,    KC_F,                                        _______, _______, _______, _______, _______, _______, \
        KC_DEL,  KC_B,   KC_Z,    KC_X,   KC_C,    KC_V,                                        _______, _______, _______, _______, _______, _______, \
                         KC_COMM, KC_DOT, KC_SLSH, KC_SPC, KC_LCTL, KC_LSFT,  _______, _______, _______, _______, _______, _______, \
                                                           KC_QUOT, KC_LALT,  _______, _______ \
    ),

    [ARKY] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                                        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,     KC_F12,  \
        _______, _______, KC_WH_U, KC_UP,   KC_PGUP, KC_HOME,                                      KC_HOME, KC_PGUP, KC_UP,   _______, _______,    _______, \
        _______,C(KC_LEFT),KC_LEFT,KC_DOWN, KC_RGHT, C(KC_RGHT),                                C(KC_LEFT), KC_LEFT, KC_DOWN, KC_RGHT, C(KC_RGHT), _______, \
        _______, _______, KC_WH_D, _______, KC_PGDN, _______,                                      _______, KC_PGDN, _______, _______, _______,    _______, \
                          SW_DE_B, SW_DE_F, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, \
                                                              _______, _______,  _______, _______ \
    ),

    [NUMK] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                      KC_NO,KC_NUMLOCK, KC_PSLS, KC_PAST, KC_PMNS, XXXXXXX, \
        _______, _______, _______, _______, _______, _______,                                      KC_COMM, KC_P7,   KC_P8,   KC_P9,   KC_PPLS, XXXXXXX, \
        _______, _______, _______, _______, _______, _______,                                      KC_PDOT, KC_P4,   KC_P5,   KC_P6,   KC_PCMM, XXXXXXX, \
        _______, _______, _______, _______, _______, _______,                                      KC_P0,   KC_P1,   KC_P2,   KC_P3,   KC_PDOT, _______, \
                          _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, \
                                                              _______, _______,  _______, _______ \
    ),

    [SYMB] = LAYOUT(
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                                        KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_F11,  KC_F12,  \
        KC_GRV,  KC_HASH, KC_DLR,  KC_LCBR, KC_RCBR, KC_PLUS,                                      KC_PLUS, KC_RCBR, KC_LCBR, KC_DLR,  KC_HASH, KC_GRV,  \
        KC_ASTR, KC_LABK, KC_RABK, KC_LPRN, KC_RPRN, KC_PIPE,                                      KC_PIPE, KC_RPRN, KC_LPRN, KC_RABK, KC_LABK, KC_ASTR, \
        KC_BSLS, KC_PERC, KC_CIRC, KC_LBRC, KC_RBRC, KC_TILD,                                      KC_TILD, KC_RBRC, KC_LBRC, KC_CIRC, KC_PERC, KC_BSLS, \
                          _______, _______, _______, _______, _______, _______,  _______, _______, _______, _______, _______, _______, \
                                                              _______, _______,  _______, _______ \
    ),

    [MOUS] = LAYOUT(
        _______, _______, _______, _______, _______, _______,                                      _______, _______, _______, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______,                                      _______, KC_WH_U, KC_MS_U, _______, _______, _______, \
        _______, _______, _______, _______, _______, _______,                                      _______, KC_MS_L, KC_MS_D, KC_MS_R, _______, _______, \
        _______, _______, _______, _______, _______, _______,                                      _______, KC_WH_D, _______, _______, _______, _______, \
                          _______, _______, _______, _______, _______, _______,  _______, KC_ACL0, KC_BTN1, KC_BTN2, _______, _______, \
                                                              _______, _______,  _______, _______ \
    ),

    [SYST] = LAYOUT(
        RESET,   RGB_TOG, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                      RESET,   XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  \
        EEP_RST, RGB_VAI, RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX,                                      EEP_RST, RGB_VAI, RGB_MOD, XXXXXXX, XXXXXXX, XXXXXXX,  \
        XXXXXXX, RGB_VAD, RGB_RMOD,XXXXXXX, XXXXXXX, XXXXXXX,                                      XXXXXXX, RGB_VAD, RGB_RMOD,XXXXXXX, XXXXXXX, TO(GAME), \
        XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(GAME),                                     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, TO(QWER),TO(CLMK), \
                          XXXXXXX, XXXXXXX, TO(QWER),TO(CLMK),XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
                                                              XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX \
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case A_TAB_F:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_TAB);
        } else {
            unregister_code(KC_TAB);
        }
        break;

        case A_TAB_B:
        if (record->event.pressed) {
            if (!is_alt_tab_active) {
            is_alt_tab_active = true;
            register_code(KC_LALT);
            }
            alt_tab_timer = timer_read();
            register_code(KC_LSFT);
            register_code(KC_TAB);
        } else {
            unregister_code(KC_LSFT);
            unregister_code(KC_TAB);
        }
        break;

        case SW_DE_F:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LWIN);
            register_code(KC_RGHT);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LWIN);
            unregister_code(KC_RGHT);
        }
        break;

        case SW_DE_B:
        if (record->event.pressed) {
            register_code(KC_LCTL);
            register_code(KC_LWIN);
            register_code(KC_LEFT);
        } else {
            unregister_code(KC_LCTL);
            unregister_code(KC_LWIN);
            unregister_code(KC_LEFT);
        }
        break;
    }
  return true;
}

void matrix_init_user(void) {}

void matrix_scan_user(void) {
  if (is_alt_tab_active) {
    if (timer_elapsed(alt_tab_timer) > 1000) {
      unregister_code(KC_LALT);
      is_alt_tab_active = false;
    }
  }
}

void led_set_user(uint8_t usb_led) {}

layer_state_t layer_state_set_user(layer_state_t state) {
    float colemak[][2] = SONG(COLEMAK_SOUND);
    float qwerty[][2] = SONG(QWERTY_SOUND);
    float game[][2] = SONG(PREONIC_SOUND);
    float system[][2] = SONG(TERMINAL_SOUND);

    switch (get_highest_layer(state)) {
        case CLMK:
            if(last_song_played != CLMK) {
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(colemak);
                    last_song_played = CLMK;
                #endif //AUDIO_ENABLE
            }
            break;
        case QWER:
            if(last_song_played != QWER) {
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(qwerty);
                    last_song_played = QWER;
                #endif //AUDIO_ENABLE
            }
            break;
        case GAME:
            if(last_song_played != GAME) {
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(game);
                    last_song_played = GAME;
                #endif //AUDIO_ENABLE
            }
            break;
        case SYST:
            if(last_song_played != SYST) {
                #ifdef AUDIO_ENABLE
                    PLAY_SONG(system);
                    last_song_played = SYST;
                #endif //AUDIO_ENABLE
            }
            break;
        default: //  for any other layers, or the default layer
            break;
    }
  return state;
}
