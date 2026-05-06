// function to load the contents of the ROM file
#include<fsteam>
#include<members.h>
using namespace std;

const unsigned int START_ADDFRESS=0x200;

void Chip8::LoadRom(char const* filename){
   ifstream file(filename,ios::binary | ios::ate); //opens the file in binary and moves the file pointer to the end

   if(file.is_open()){
     // gets size of file and allocate a buffer to hold the contents 
     streampos size = file.tellg(); //tellg return streampos 
     char* buffer=new char[size]; //stored in char because a file is a sequence of bytes(1 byte, char is 1 byte too. Buffer basically stores every byte in that file 
      
     //go back to beginning of the file to fill buffer 
     file.seekg(0,ios::beg);
     file.read(buffer,size)
     file.close();

     //load the ROM's content into the CHIP_* memory starting from 0x200 
    for(long i=0;i<size;++i){
       memory[START_ADDFRESS+i] = buffer[i];
     }
     delete[buffer];
   }
}

Chip8::CHIP(){
  pc=START_ADDFRESS;
 }
  //loading fonts into memory
  const unsigned int FONT_START_ADDRESS=0X50;
  for(unsigned int i=0;i<FONT_SIZE;++i){
    memory[FONT_START_ADDRESS + i] = font_set[i];
  }
void Chip8::OP_00E0()
{
  memset(video,0,sizeof(video)); //clears display 
}

void Chip8::op_00EE(){
  --sp; //sp goes to the next address in the stack, below the currently called one 
  pc=stack[sp];
}


