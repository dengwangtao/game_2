```
git clone https://github.com/libsdl-org/SDL_mixer.git
cd SDL_mixer/
git branch
git switch -c date39 5ee0e7f
cmake -S . -B build -DCMAKE_INSTALL_PREFIX="C:\ENV\SDL3_mixer"
cmake --build build --config Debug
cmake --install build --config Debug
```

拷贝整个`C:\ENV\SDL3_mixer`的内容到`C:\ENV\SDL3`,

并将`lib\SDL3_mixer.lib`拷贝到`lib\x64\SDL3_mixer.lib`下

将`bin`下的`SDL3_mixer.dll`拷贝到`bin\x64`下

将`bin`下的`SDL3_mixer.pdb`拷贝到`bin\x64`下