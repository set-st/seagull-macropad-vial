# Seagull Macropad Vial Firmware

Firmware for [Seagull Macropad](https://github.com/klouderone/SeagullMacropad), customized with real-time OLED visualization.

## Build Command

```bash
qmk compile -kb seagull -km vial
```

## Matrix Specifications

- **Physical Matrix Size:** 4 rows x 5 columns.
- **Key Layout:**
  - Standard keys: Rows 0-3, Columns 0-3 (16 keys total).
  - Encoder button: Row 2, Column 4.
- **Diode Direction:** COL2ROW.

## OLED Features (32x128)

1. **Layer Information:**
   - Displays a large icon representing the active layer.
   - Shows the layer number and name (`MACRO`, `NUM`, `GAMES`, `SETGS`).
2. **System Status:**
   - Displays `CAPS`, `NUM`, and `SCRL` status text at the bottom, synchronized with the host computer.
3. **Key Matrix Visualization:**
   - A real-time 4x4 grid of 7x7 pixel squares representing the main keys.
   - Lights up the corresponding square on keypress.
   - **Wave Animation:** Pressing the encoder button triggers a center-to-edge wave animation across the grid.

## Configuration Details

- **Controller:** RP2040 (configured via `rp2040_ce` converter).
- **Display:** SSD1306 OLED (I2C), rotated 270 degrees.
