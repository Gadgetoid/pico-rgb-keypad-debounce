# Pico RGB Keypad MAXIMUM DEBOUNCE

This project is just a simple demonstration of my `AutoRepeat` class, originally written for [32blit](https://github.com/32blit/).

`AutoRepeat` does three things:

1. Debounces input
2. Repeats presses slowly when a button is held
3. Repeats presses quickly after a button has been held for a duration

It's great for repeating key presses for scrolling up/down menus and similar things.

This example has a function that iterates and debounces all 16 keys from Keypad a little messily using an array of `AutoRepeat` instances.

Normally you would do something simple like:

```c++
AutoRepeat my_key(repeat_time, hold_time);

bool debounced_input = my_key.next(uint32_t time_ms, bool raw_gpio_input);
```