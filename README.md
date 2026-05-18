# GD32A7xx Single-Core Keil Template

This template is based on `GD32A7xx_B_Firmware_Library_V1.1.0/Template_SingleCore`.

## Directory

```text
app/                    Application entry, interrupts, SysTick, lib option header
driver/gd32_evb/        GigaDevice EVB board helper files
firmware/cmsis/         CMSIS core and GD32A7xx device files
firmware/driver/        GD32A7xx standard peripheral library
mdk/                    Keil uVision project
```

## Keil

Open:

```text
mdk/gd32a7_singlecore_template.uvprojx
```

Default target:

```text
GD32A711x_A712x_M7_0
```

The default device is `GD32A712AI:CM7_0`, following the official single-core
template. The project also keeps the official `GD32A714X` target for convenience.

## Notes

- `app/main.c` is a minimal empty-loop entry point.
- `systick_config()` is enabled by default and provides `delay_1ms()`.
- `led_spark()` is left as a 1 ms heartbeat hook called by `SysTick_Handler()`.
- Add board-specific GPIO, UART, clock, and peripheral code under `driver/`.
