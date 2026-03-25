# esphome-ln882h-pwm

Hardware PWM output component for **Lightning LN882H** chips in ESPHome.

ESPHome's built-in `libretiny_pwm` component does not work on LN882H because `analogWrite()` is not implemented in LibreTiny for this chip. This component bypasses that limitation by directly using the LN882H SDK's Advanced Timer peripheral for true hardware PWM — no flicker, no workarounds.

## Features

- True hardware PWM using the LN882H Advanced Timer peripheral
- Up to **6 simultaneous PWM channels** (channels 0–5)
- Works on any GPIOA or GPIOB pin
- Configurable frequency (default 1 kHz)
- Compatible with ESPHome's `light`, `fan`, and `output` components
- Fully dimmable LED control via Home Assistant

## Installation

Add this to your ESPHome YAML:

```yaml
external_components:
  - source:
      type: git
      url: https://github.com/Nouxii/esphome-ln882h-pwm
    components: [ln882h_pwm]
```

## Usage

### Basic dimmable light

```yaml
output:
  - platform: ln882h_pwm
    id: my_light_pwm
    port: 0           # 0 = GPIOA, 1 = GPIOB
    pin_num: 10        # Pin number within the port (e.g. 10 = GPIOA10)
    timer_channel: 0   # Advanced Timer channel (0–5)
    frequency: 1000    # PWM frequency in Hz

light:
  - platform: monochromatic
    name: "My Dimmable Light"
    output: my_light_pwm
```

### RGBCW LED strip (5 channels)

```yaml
output:
  - platform: ln882h_pwm
    id: pwm_red
    port: 0
    pin_num: 13
    timer_channel: 0
    frequency: 1000

  - platform: ln882h_pwm
    id: pwm_green
    port: 0
    pin_num: 14
    timer_channel: 1
    frequency: 1000

  - platform: ln882h_pwm
    id: pwm_blue
    port: 0
    pin_num: 15
    timer_channel: 2
    frequency: 1000

  - platform: ln882h_pwm
    id: pwm_cold_white
    port: 0
    pin_num: 9
    timer_channel: 3
    frequency: 1000

  - platform: ln882h_pwm
    id: pwm_warm_white
    port: 0
    pin_num: 12
    timer_channel: 4
    frequency: 1000

light:
  - platform: rgbww
    name: "LED Strip"
    red: pwm_red
    green: pwm_green
    blue: pwm_blue
    cold_white: pwm_cold_white
    warm_white: pwm_warm_white
    cold_white_color_temperature: 6500 K
    warm_white_color_temperature: 2700 K
```

## Configuration

| Parameter       | Required | Default | Description                                       |
|----------------|----------|---------|---------------------------------------------------|
| `port`         | Yes      | —       | GPIO port: `0` for GPIOA, `1` for GPIOB          |
| `pin_num`      | Yes      | —       | Pin number within the port (0–15)                 |
| `timer_channel`| No       | `0`     | Advanced Timer channel (0–5). Each output needs its own channel. |
| `frequency`    | No       | `1000`  | PWM frequency in Hz                               |

## Pin Reference

The LN882H QFN32 (LN882HKI) has 20 GPIOs across two ports. Here are the commonly used PWM-capable pins:

| GPIO   | ESPHome name | Port | Pin Num | QFN32 Pin | Notes           |
|--------|-------------|------|---------|-----------|-----------------|
| GPIOA7 | PA7         | 0    | 7       | 15        | PWM capable     |
| GPIOA9 | PA9         | 0    | 9       | 17        | Also BOOT pin   |
| GPIOA10| PA10        | 0    | 10      | 21        | PWM capable     |
| GPIOA11| PA11        | 0    | 11      | 22        | PWM capable     |
| GPIOA12| PA12        | 0    | 12      | 23        | PWM capable     |
| GPIOB3 | PB3         | 1    | 3       | 24        | PWM capable     |

Pins GPIOA13–GPIOB2 are reserved for QSPI flash and should **not** be used.

## How It Works

The LN882H has 6 Advanced Timer channels, each capable of generating independent PWM signals. This component:

1. Enables the Advanced Timer clock gate
2. Configures the timer with the desired frequency using the APB0 clock
3. Maps the selected GPIO to the timer channel via the AFIO (Alternate Function I/O) multiplexer
4. Sets the duty cycle by adjusting the timer's compare register

The implementation is based on the PWM code from [OpenBeken](https://github.com/openshwprojects/OpenBK7231T_App), adapted to work as a native ESPHome external component.

## Background

This component was created because:

- The LN882H chip is increasingly used in Tuya WiFi devices as a replacement for BK7231/ESP modules
- ESPHome supports LN882H via LibreTiny, but `libretiny_pwm` fails to compile because `analogWrite()` is not implemented for this platform
- OpenBeken has working PWM for LN882H, proving the hardware supports it — the gap was only in the ESPHome/LibreTiny Arduino layer

## Compatibility

- **Tested on:** LN882HKI (QFN32, 2MB flash)
- **ESPHome:** 2026.3.1+
- **LibreTiny:** v1.12.1+
- **Board:** `generic-ln882hki`

## License

MIT

## Credits

- PWM implementation based on [OpenBeken's LN882H HAL](https://github.com/openshwprojects/OpenBK7231T_App/blob/main/src/hal/ln882h/hal_pins_ln882h.c)
- [LibreTiny](https://github.com/libretiny-eu/libretiny) for LN882H ESPHome platform support
- [Elektroda.com](https://www.elektroda.com) community for LN882H documentation and teardowns
