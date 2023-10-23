# Usage

For usage see the <em>[example](example/main.c)</em> program.

# Installation

For installation run this :
```bash

git clone --depth=1 https://github.com/Luminous-99/mos6502_emulator.git &&
cd mos6502_emulator &&
mkdir cmake &&
cd cmake &&
cmake .. &&
sudo cmake -DCOMPONENT=headers -P=cmake_install.cmake &&
cmake --build . &&
sudo cmake -DCOMPONENT=lib -P=cmake_install.cmake

```
