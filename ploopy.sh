qmk compile -kb ploopyco/trackball -km custom && \
dfu-programmer atmega32u4 erase --force && \
dfu-programmer atmega32u4 flash ploopyco_trackball_rev1_005_custom.hex && \
dfu-programmer atmega32u4 reset