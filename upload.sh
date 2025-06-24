cd ./build
cmake -DPICO_BOARD=pico2 -DCMAKE_EXPORT_COMPILE_COMMANDS=1 ..
make -j16
picotool load main.uf2 -f
