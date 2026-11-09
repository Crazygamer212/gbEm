# GBEmulator
This is a basic gameboy emulator I'm making to learn c++



## TODO
 - modular debug GUI 
 - graphics
 - finish all opcodes
 - add interupt support
 - implement a memory system
 - tests for opcodes
 - tests for ppu
 - tests for memory




## how to build
 git clone https://github.com/Crazygamer212/gbEm.git 

 cd gbEm 

 git clone https://github.com/libsdl-org/SDL.git vendored/SDL 

 cmake -S . -B build 

 cmake --build build 


### To run tests:
./build/Debug/test

### To run emulator:
./build/Debug/gbEmu <Path/To/Rom>




## Resources used:
 - [Pandocs](<https://gbdev.io/pandocs/>)
 - [GB: (not so) Complete Technical Reference](<https://gekkio.fi/files/gb-docs/gbctr.pdf>)  -- there are a lot of todos still
 - [Game Boy Opcodes](<https://gb.insertcoin.dev/>)