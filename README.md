# Usage

For usage see the <em>[example](example/main.c)</em> program.

# Installation

For installation run this :
```bash

git clone --depth=1 https://github.com/Luminous-99/mos6502_emulator.git &&
cd mos6502_emulator &&
mkdir build &&
cd build &&
cmake .. &&
sudo cmake --install . --component headers &&
cmake --build .

```
if you want it as a shared library run :
```bash
sudo cmake --install . --component shared 
```
or for a static library :
```bash
sudo cmake --install . --component static 
```
