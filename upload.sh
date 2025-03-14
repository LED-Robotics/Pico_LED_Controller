cd ./build
cmake -DPICO_BOARD=pico2 ..
make -j16
picotool load main.uf2 -f
