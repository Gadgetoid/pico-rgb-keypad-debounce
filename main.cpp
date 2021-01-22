#include <stdint.h>
#include "pico/stdlib.h"
#include "pico_rgb_keypad.hpp"

using namespace pimoroni;

PicoRGBKeypad pico_rgb_keypad;

uint16_t state = 0;

class AutoRepeat {
  public:
    AutoRepeat(uint32_t repeat_time=200, uint32_t hold_time=1000) {
      this->repeat_time = repeat_time;
      this->hold_time = hold_time;
    }
    bool next(uint32_t time, bool state) {
      bool changed = state != last_state;
      last_state = state;

      if(changed) {
        if(state) {
          pressed_time = time;
          pressed = true;
          last_time = time;
          return true;
        }
        else {
          pressed_time = 0;
          pressed = false;
          last_time = 0;
        }
      }
      // Shortcut for no auto-repeat
      if(repeat_time == 0) return false;

      if(pressed) {
        uint32_t repeat_rate = repeat_time;
        if(hold_time > 0 && time - pressed_time > hold_time) {
          repeat_rate /= 3;
        }
        if(time - last_time > repeat_rate) {
          last_time = time;
          return true;
        }
      }

      return false;
    }
  private:
    uint32_t repeat_time;
    uint32_t hold_time;
    bool pressed = false;
    bool last_state = false;
    uint32_t pressed_time = 0;
    uint32_t last_time = 0;
};

AutoRepeat autorepeat[pico_rgb_keypad.NUM_PADS];

uint16_t debounce_buttons(uint32_t t) {
    // This escalated quickly
    uint16_t buttons = pico_rgb_keypad.get_button_states();

    for(auto i = 0u; i < pico_rgb_keypad.NUM_PADS; i++) {
        uint16_t bit = 1 << i;
        bool result = autorepeat[i].next(t, buttons & bit);
        buttons &= ~bit;
        if(result) buttons |= bit;
    }

    return buttons;
}

int main() {
    pico_rgb_keypad.init();
    pico_rgb_keypad.set_brightness(0.2);

    uint32_t t = 0;

    while(true) {

        state ^= debounce_buttons(t);
    
        for(auto i = 0u; i < pico_rgb_keypad.NUM_PADS; i++) {
            if((state & (1 << i)) == 0) {
                pico_rgb_keypad.illuminate(i, 0x00, 0x00, 0x00);
                continue;
            };

            int step = (i / 4) + (i & 0b11); // magic so that `step & 0b1` gives an alternating pattern 
            if(step & 0b1) {
                pico_rgb_keypad.illuminate(i, 0xaa, 0x33, 0xff);
            }
            else
            {
                pico_rgb_keypad.illuminate(i, 0x33, 0xff, 0xaa);
            }
        }
        pico_rgb_keypad.update();
        sleep_ms(1);
        t++;
    }
}