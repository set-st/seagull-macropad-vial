# Build Options
#   change yes to no to disable
#
BOOTLOADER = caterina
BOOTMAGIC_ENABLE = no      # Enable Bootmagic Lite
MOUSEKEY_ENABLE = no      # Mouse keys
EXTRAKEY_ENABLE = yes      # Audio control and System control
CONSOLE_ENABLE = no         # Console for debug
COMMAND_ENABLE = no         # Commands for debug and configuration
NKRO_ENABLE = no          # Enable N-Key Rollover
BACKLIGHT_ENABLE = no       # Enable keyboard backlight functionality
RGBLIGHT_ENABLE = no        # Enable keyboard RGB underglow
AUDIO_ENABLE = no           # Audio output

OLED_ENABLE = yes
OLED_DRIVER = ssd1306
OLED_TRANSPORT = i2c

LTO_ENABLE = yes

UNICODE_ENABLE = no
ENCODER_ENABLE = yes
DIP_SWITCH_ENABLE = yes
WPM_ENABLE      = yes
CONVERT_TO=rp2040_ce
