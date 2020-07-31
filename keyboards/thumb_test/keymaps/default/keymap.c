/* Copyright 2020 Nicolas <BuraBure> Fernandez
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
//#include "champ40j.h"
#include "pro_micro.h"
#include "analog.c"
#include "math.h"
#include "pincontrol.h"
#include "pointing_device.h"
#include "print.h"
#include "report.h"
#include "timer.h"

// Joystick
// Set Pins
int xPin = MOUSE_X; // VRx
int yPin = MOUSE_Y; // VRy
int swPin = MOUSE_LEFT; // SW

// Set Parameters
int minAxisValue = 300; // 143, 110
int maxAxisValue = 700; // 785, 845

float maxCursorSpeed = 0.75;

int xPolarity = 1;
int yPolarity = -1;

int cursorTimeout = 10;

int xOrigin, yOrigin;

uint16_t lastCursor = 0;

int16_t analogPinRead(uint8_t pin)
{
	uint8_t mux;
	switch (pin) {
    case F4: mux = 4; break;
    case F5: mux = 5; break;
	case F6: mux = 6; break;
	case F7: mux = 7; break;
	// …
	case D6: mux = 0x21; break;
	default: return 0;
	}
	return adc_read(mux);
}

int axisCoordinate(int pin, int origin) {
  int direction;
  int distanceFromOrigin;
  int range;

  int position = analogPinRead(pin);

  if (origin == position) {
    return 0;
  } else  if (origin > position) {
    distanceFromOrigin = origin - position;
    range = origin - minAxisValue;
    direction = -1;
  } else {
    distanceFromOrigin = position - origin;
    range = maxAxisValue - origin;
    direction = 1;
  }

  float percent = (float)distanceFromOrigin / range;
    int coordinate = (int)(percent * 100);
    if (coordinate < 0) {
      return 0;
    }
    else if (coordinate > 100) {
      return 100 * direction;
    }
    else {
      return coordinate * direction;
    }
}

int axisToMouseComponent(int pin, int origin, float maxSpeed, int polarity) {
  int coordinate = axisCoordinate(pin, origin);

  //print_decs(coordinate);
  //println("");

  if (coordinate == 0) {
    return 0;
  } else {
    float percent = (float)coordinate / 100;
    return percent * maxCursorSpeed * polarity * (abs(coordinate)/6);
  }
}

void pointing_device_task(void) {
  report_mouse_t report;
  report.x = 0;
  report.y = 0;
  report.h = 0;
  report.v = 0;
  report.buttons = 0;

  // todo read as one vector
  if (timer_elapsed(lastCursor) > cursorTimeout) {
    lastCursor = timer_read();
    report.x = axisToMouseComponent(xPin, xOrigin, maxCursorSpeed, xPolarity);
    report.y = axisToMouseComponent(yPin, yOrigin, maxCursorSpeed, yPolarity);
  }

  if (digitalRead(swPin) == 0) {
    report.buttons |= MOUSE_BTN1;
  }

  pointing_device_set_report(report);
  pointing_device_send();
}

void pointing_device_init(void) {
  timer_init();
  xOrigin = analogPinRead(xPin);
  yOrigin = analogPinRead(yPin);
  digitalWrite(swPin, PinLevelHigh);
}

// static void print_analog_pins(void) {
//   int16_t val0 = analogPinRead(MOUSE_X);
//   int16_t val1 = analogPinRead(MOUSE_Y);

//   print("  A0:"); print_dec(F7);
//   print(" val:"); print_decs(val0); println();
//   print("  A1:"); print_dec(F6);
//   print(" val:"); print_decs(val1); println();
// }

// #define PRINT_INTERVAL 100
// void matrix_scan_user(void) {
//   static int16_t counter = PRINT_INTERVAL;
//   counter++;
//   if (PRINT_INTERVAL < counter) {
//     print_analog_pins();
//     counter = 0;
//   }
// }

//-----------------------------------------------------

// Defines the keycodes used by our macros in process_record_user
enum custom_keycodes {
    QMKBEST = SAFE_RANGE,
    QMKURL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [0] = LAYOUT( /* Base */
        KC_A, KC_1, KC_H,
          KC_TAB, KC_SPC
    ),
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case QMKBEST:
            if (record->event.pressed) {
                // when keycode QMKBEST is pressed
                SEND_STRING("QMK is the best thing ever!");
            } else {
                // when keycode QMKBEST is released
            }
            break;
        case QMKURL:
            if (record->event.pressed) {
                // when keycode QMKURL is pressed
                SEND_STRING("https://qmk.fm/" SS_TAP(X_ENTER));
            } else {
                // when keycode QMKURL is released
            }
            break;
    }
    return true;
}

void matrix_init_user(void) {

}

void led_set_user(uint8_t usb_led) {

}
