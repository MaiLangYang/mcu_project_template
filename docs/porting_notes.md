# Porting Notes

## Change Board Pins

Edit `bsp/board_config.h` first. Driver source files use only the `BOARD_*` macros for peripheral instances, clocks, GPIO ports, pins, and alternate functions.

## Add a Standard Peripheral Driver

1. Include the peripheral header in `bsp/gd32a50x_libopt.h`.
2. Add the matching source file from `firmware/GD32A50x_standard_peripheral/Source/` to the Keil project group `GD32A50x_StdPeriph`.
3. Keep board-level pins and clocks in `bsp/board_config.h`.

## Keil Output Cleanup

Build outputs go to:

```text
mdk/Objects
mdk/Listings
```

Run `keilkill.bat` from the project root to remove common generated files.
