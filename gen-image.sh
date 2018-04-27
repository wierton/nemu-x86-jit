#!/bin/bash

IMAGE=src/monitor/image.h

echo '#include "common.h"' > $IMAGE
echo 'const uint8_t img []  = {' >> $IMAGE
hexdump -v -e '1/1 "0x%02x, "' $1 | fmt --width 80 >> $IMAGE
echo '};' >> $IMAGE
