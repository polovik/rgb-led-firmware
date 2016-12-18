/**
 * Main entry point for RGB LED controller.
 *
 * Copyright (c) 2014--2015 Coredump Rapperswil
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

// Get pin definitions
#include "pins.h"

// Include effects
#include "effect_colorwheel.h"
#include "effect_rgb_control.h"
#include "effect_hsi_control.h"
#include "effect_blue_sin.h"
#include "effect_sinus_runner.h"
#include "effect_sinus_runner2.h"
#include "effect_colour_time.h"


// List of available effects
enum Effect {
    Colorwheel,
    RGBControl,
    HSIControl,
    BlueSinus,
    SinusRunner,
    SinusRunner2,
    ColourTime,
    EffectNone,
};

// Choose your effect
static Effect effect = SinusRunner2;

// Store previous button state - for detect button pressing only once
int buttonStatePrev = HIGH;

// Initialize GPIO pins
void setup() {
    // Set LED pins as output
    pinMode(LED_R, OUTPUT);
    pinMode(LED_G, OUTPUT);
    pinMode(LED_B, OUTPUT);
    pinMode(LED_LOW_POWER, OUTPUT);

    // Set button pin for digital input
    pinMode(BUTTON_MODE, INPUT);
    buttonStatePrev = digitalRead(BUTTON_MODE);
    
    // Set pot pins as input
    pinMode(POT_1, INPUT);
    pinMode(POT_2, INPUT);
    pinMode(POT_3, INPUT);

    // Set voltmeter pin for analog input
    pinMode(VOL_METER, INPUT);
}


// Main loop
void loop() {
    // Voltage divider has ratio = 3.6. For 9V threshold is = (9/3.6)/5*1023
    int powerValue = analogRead(VOL_METER); // 0 ... 1023
    if (powerValue < 511) {
        // Enter in idle state - long sleep
        digitalWrite(LED_LOW_POWER, HIGH);
        delay(100);
    } else {
        digitalWrite(LED_LOW_POWER, LOW);
    }
    
    int state = digitalRead(BUTTON_MODE);
    if (state != buttonStatePrev) {
        if (state == LOW) {
            effect = (Effect)(effect + 1);
            if (effect == EffectNone) {
                effect = RGBControl;
            }
        }
        buttonStatePrev = state;
    }
    
    switch (effect) {
        case Colorwheel:
            effect_colorwheel(); 
            break;
        case RGBControl:
            effect_rgb_control(); 
            break;
        case HSIControl:
            effect_hsi_control();
            break;
        case BlueSinus:
            effect_blue_sin(); 
            break;
        case SinusRunner:
            effect_sinus_runner();
            break;
        case SinusRunner2:
            effect_sinus_runner2();
            break;
        case ColourTime:
            effect_colour_time();
            break;
    }

}
