#!/bin/sh
echo "Warning: This makes /sys/class/backlight/intel_backlight/brightness world writeable!"
sudo chmod ugo+w /sys/class/backlight/intel_backlight/brightness 

