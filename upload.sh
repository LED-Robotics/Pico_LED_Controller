cd ./build
cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DPICO_BOARD=pico2 ..
make -j16
picotool load main.uf2 -f
