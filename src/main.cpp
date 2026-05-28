#include "../include/Chip8.h"
#include<chrono>
#include<cstdlib>
#include<iostream>


int main(int argc,char* argv[]){ //no of arguments and storing those arguments in an array (scale,delay,filename)
   if(argc != 4){
      std::cerr<<"usage: " <<argv[0] <<" <scale> <delay> <rom>\n"; //error message 
      std::exit(EXIT_FAILURE);
   }
   int videoScale = std::stoi(argv[1]); //string to integer 
   int cycleDelay = std::stoi(argv[2]);  // cycle delay is ammount of time to wait before running the next cycle
   char const* romFilename = argv[3];
Platform platform("CHIP-8 Emulator",VIDEO_WIDTH*videoScale,VIDEO_HEIGHT*videoScale,VIDEO_WIDTH,VIDEO_HEIGHT);
Chip8 chip8;
chip8.LoadRom(romFilename);

int videoPitch = sizeof(chip8.video[0] * VIDEO_WIDTH); //basically to find the size of onerow or where the next row is located
auto lastCycleTime=std::chrono::high_resolution_clock::now();
bool quit=false;

while(!quit){
  quit=platform.ProcessInput(chip8.keypad);

  auto currentTime= std::chrono::high_resolution_clock::now();
  float dt=std::chrono::duration<float,std::chrono::milliseconds::period>(currentTime - lastCycleTime).count(); // milliseconds passed between last cycle and now 

if(dt>cycleDelay){
  lastCycleTime=currentTime; // updates the time to the time when last cycle completed its execution
  platform.cycle();
  platform.update(chip8.video,videoPitch);
  }
}
 } 
