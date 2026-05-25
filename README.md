# GD32A503CCT3 Keil MDK Template

This template is based on GD32A50x Firmware Library V1.2.0 and targets GD32A503CCT3.

## Project Layout

```text
app/        Application entry and demo loop
bsp/        Board configuration, SysTick, interrupts, printf retarget
drivers/    ADC, I2C, USART board drivers
firmware/   Self-contained GD32 CMSIS and standard peripheral library
mdk/        Keil MDK project
docs/       Pin and porting notes
```

## Peripheral Mapping

| Peripheral | Function | Pin | Module |
| --- | --- | --- | --- |
| USART0 | TX | PB13 | `drivers/drv_usart.*` |
| USART0 | RX | PB14 | `drivers/drv_usart.*` |
| I2C1 | SCL | PE10 | `drivers/drv_i2c.*` |
| I2C1 | SDA | PE11 | `drivers/drv_i2c.*` |
| ADC0 | IN11 | PA3 | `drivers/drv_adc.*` |

## Open in Keil

Open:

```text
mdk/template_a503c.uvprojx
```

The project uses:

```text
USE_STDPERIPH_DRIVER,GD32A50X
```

## Application Behavior

`app/app.c` initializes the board, prints a startup message on USART0, samples `ADC0_IN11` once per second, and echoes received USART bytes.

I2C functions are provided as blocking master read/write helpers. The address argument is a 7-bit I2C address; the driver shifts it for the GD32 I2C peripheral.
