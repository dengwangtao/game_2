```
git clone https://github.com/libsdl-org/SDL_mixer.git
cd SDL_mixer/
git branch
git switch -c date39 5ee0e7f
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="C:\ENV\SDL3_mixer"
cmake --build build --config Debug
cmake --install build --config Debug
```