#include<fstream>
#include<iostream>
#include "../include/Chip8.h"
#include<chrono>
#include<cstdlib>
using namespace std;
const unsigned int START_ADDRESS=0x200;
void Chip8::LoadRom(char const* filename){
   ifstream file(filename,ios::binary | ios::ate); //opens the file in binary and moves the file pointer to the end

   if(file.is_open()){
     // gets size of file and allocate a buffer to hold the contents 
     streampos size = file.tellg(); //tellg return streampos 
     char* buffer=new char[size]; //stored in char because a file is a sequence of bytes(1 byte, char is 1 byte too. Buffer basically stores every byte in that file 
      
     //go back to beginning of the file to fill buffer 
     file.seekg(0,ios::beg);
     file.read(buffer,size);
     file.close();

     //load the ROM's content into the CHIP_* memory starting from 0x200 
    for(long i=0;i<size;++i){
       memory[START_ADDRESS+i] = buffer[i];
     }
     delete[] buffer;
    }
}
Chip8::Chip8()
    : randGen(std::chrono::system_clock::now().time_since_epoch().count())
{
    randByte = std::uniform_int_distribution<uint8_t>(0, 255U);


table[0x0] = &Chip8::Table0; //addresses of various member functions 
table[0x1] = &Chip8::OP_1nnn;
table[0x2] = &Chip8::OP_2nnn;
table[0x3] = &Chip8::OP_3xkk;
table[0x4] = &Chip8::OP_4xkk;
table[0x5] = &Chip8::OP_5xy0;
table[0x6] = &Chip8::OP_6xkk;
table[0x7] = &Chip8::OP_7xkk;
table[0x8] = &Chip8::Table8;
table[0x9] = &Chip8::OP_9xy0;
table[0xA] = &Chip8::OP_Annn;
table[0xB] = &Chip8::OP_Bnnn;
table[0xC] = &Chip8::OP_Cxkk;
table[0xD] = &Chip8::OP_Dxyn;
table[0xE] = &Chip8::TableE;
table[0xF] = &Chip8::TableF;  

for(size_t i=0;i<=0xE;i++){
  table0[i] = &Chip8::OP_NULL; //this is the subtable containing 000E and 00EE 
  table8[i] = &Chip8::OP_NULL;
  tableE[i] = &Chip8::OP_NULL;
}
table0[0x0] = &Chip8::OP_00E0;
table0[0xE] =  &Chip8::OP_00EE;

table8[0x0] = &Chip8::OP_8xy0;
table8[0x1] = &Chip8::OP_8xy1;
table8[0x2] = &Chip8::OP_8xy2;
table8[0x3] = &Chip8::OP_8xy3;
table8[0x4] = &Chip8::OP_8xy4;
table8[0x5] = &Chip8::OP_8xy5;
table8[0x6] = &Chip8::OP_8xy6;
table8[0x7] = &Chip8::OP_8xy7;
table8[0xE] = &Chip8::OP_8xyE;

tableE[0x1] = &Chip8::OP_ExA1;
tableE[0xE] = &Chip8::OP_Ex9E;

for (size_t i = 0; i <= 0x65; i++) // we are basically filling them with null function as default
{
			tableF[i] = &Chip8::OP_NULL;
}
tableF[0x07] = &Chip8::OP_Fx07;
tableF[0x0A] = &Chip8::OP_Fx0A;
tableF[0x15] = &Chip8::OP_Fx15;
tableF[0x18] = &Chip8::OP_Fx18;
tableF[0x1E] = &Chip8::OP_Fx1E;
tableF[0x29] = &Chip8::OP_Fx29;
tableF[0x33] = &Chip8::OP_Fx33;
tableF[0x55] = &Chip8::OP_Fx55;
tableF[0x65] = &Chip8::OP_Fx65;
pc=START_ADDRESS;
  //loading fonts into memory
  const unsigned int FONT_START_ADDRESS=0X50;
  for(unsigned int i=0;i<FONTSET_SIZE;++i){
    memory[FONT_START_ADDRESS + i] = font_set[i];
  }
}

void Chip8::OP_NULL(){}

void Chip8::Table0(){
  ((*this).*table0[opcode & 0x000Fu])(); //deferencing the object then indexing the opcode we want 
}
void Chip8::Table8(){
  ((*this).*table8[opcode & 0x000Fu])();
}
void Chip8::TableE(){
  ((*this).*tableE[opcode & 0x000Fu])();
}
void Chip8::TableF()
{
    uint8_t idx = opcode & 0x00FFu;

    std::cout << "F opcode: 0x" << std::hex << (int)idx << std::endl;

    ((*this).*tableF[idx])();
}

void Chip8::OP_00E0()
{
  memset(VIDEO,0,sizeof(VIDEO)); //clears display 
}

void Chip8::OP_00EE(){
  --sp; // sp goes to the address that was called into the stack (subroutine)
  pc=stack[sp];
}
void Chip8::OP_1nnn(){
  uint16_t address = opcode & 0X0FFFu; // anything & 1111 = number itself,seperates the address to where the jump will be done to from the opcode
  pc=address;
}
void Chip8::OP_2nnn(){
    uint16_t address = opcode & 0XFFFu;
    stack[sp]=pc; //adds the previous opcode to the top of the stack 
    sp++;
    pc=address; //stores the address of the next opcode 
} void Chip8::OP_3xkk(){
 uint8_t Vx= (opcode & 0x0F00U) >> 8u; // finding resister number,usually the second digit of the opcode 
 uint8_t byte=opcode&0x00FFu; // 8 byte because we only need the last two digits of the opcode 

 if(registers[Vx] == byte){
   pc+=2; //skip
 }
}
void Chip8::OP_4xkk(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  uint8_t byte= opcode & 0x00FFu;

  if(registers[Vx]!=byte){
    pc+=2;
  }
}
void Chip8::OP_5xy0(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u; //shifts digits by 8 cuz we only need the number of s
  uint8_t Vy=(opcode & 0x00F0u) >> 4u;
 if(registers[Vx] == registers[Vy]){
   pc+=2;
 }
}
void Chip8::OP_6xkk(){
  uint8_t Vx= (opcode & 0x0F00U) >> 8u; // 0fuu to extract the s number 
  uint8_t byte= opcode & 0x00FFu;
  
  registers[Vx] = byte;
}
void Chip8::OP_7xkk(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  uint8_t byte=opcode & 0x00FFu;

  registers[Vx] += byte;
}
void Chip8::OP_8xy0(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;

  registers[Vx] = registers[Vy];
}
void Chip8::OP_8xy1(){
  uint8_t Vx = (opcode & 0x0F00) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u; 

  registers[Vx] |= registers[Vy];
} 
void Chip8::OP_8xy2(){
  uint8_t Vx = (opcode & 0x0F00) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u; 
 
  registers[Vx] &= registers[Vy];
}
void Chip8::OP_8xy3(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;

  registers[Vx] ^= registers[Vy];
}
void Chip8::OP_8xy4(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;
  
  uint16_t sum=registers[Vx]+registers[Vy];
  if(sum > 255u){
    registers[0xF]=1;
  }
  else{
    registers[0xF]=0;
  }
   registers[Vx] = sum & 0xFFu;
}
void Chip8::OP_8xy5(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;
   
  if(registers[Vx] > registers[Vy]){
    registers[0xF]=1;
  }
  else{
    registers[0xF]=0;
  }
  registers[Vx] -= registers[Vy];
}
void Chip8::OP_8xy6(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  
  registers[0xF] = registers[Vx] & 0x0001u; //to find the lsb because the shift operation destroys it
  registers[Vx] >>=1u;
}

void Chip8::OP_8xy7(){
  uint8_t Vx=(opcode & 0x0f00) >> 8u;
  uint8_t Vy=(opcode & 0x00f0) >> 4u;
   
  if(registers[Vy] > registers[Vx]){
    registers[0xF]=1;
  }
  else{
    registers[0xF] = 0;
  }
  registers[Vx] = registers[Vy] - registers[Vx];
}
void Chip8::OP_8xyE(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  registers[0xF] =  opcode & 0x80u >> 7u; // storing msb 
  
  registers[Vx] <<=1; //left shift 
}
void Chip8::OP_9xy0(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy= (opcode & 0x00F0) >> 4u;

  if(registers[Vx] != registers[Vy]){
    pc += 2;
  }
}
void Chip8::OP_Annn(){
 uint16_t address = opcode & 0x0FFFu; // last 12 bits 
 index=address; 
}
void Chip8::OP_Bnnn(){
uint16_t Hello=opcode & 0x0FFFu; //HELLO IS ADDRESS I WAS BORED
pc=registers[0]+Hello;
}
void Chip8::OP_Cxkk()
{
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = randByte(randGen) & byte;
}
void Chip8::OP_Dxyn(){
uint8_t Vx=(opcode & 0x0F00u) >> 8u;
uint8_t Vy=(opcode & 0x00F0u) >> 4u;
uint8_t height = opcode & 0x000Fu; //VIDEO_width is 64 here 

uint8_t xPos=registers[Vx] % VIDEO_WIDTH; //for out of boundary conditions
uint8_t yPos=registers[Vy] % VIDEO_HEIGHT; //yPos is where it starts drawing 

registers[0xF]=0; // 1 if collision of new sprite and sprite on displsy,otherwise 0 
    
  for(unsigned int row=0;row<height;row++){ //iterating through sprite rows 
     uint8_t spriteByte=memory[index+row]; //byte data of the row 
     for(unsigned int col=0;col<8;col++){

        if ((xPos + col) >= VIDEO_WIDTH ||
            (yPos + row) >= VIDEO_HEIGHT)
            continue;

       uint8_t spritePixel=spriteByte & (0x80u >> col); //extracts column one by one 

       //pointer to the current row and column we are in 
       uint32_t* screenPixel=&VIDEO[(yPos+row)*VIDEO_WIDTH+(xPos+col)];
       

       if(spritePixel){ //spritePixel is on 

         if(*screenPixel == 0xFFFFFFFF){ // screenPixel is also on 
           registers[0xFu]=1; // collision condition 
         }
         *screenPixel ^= 0xFFFFFFFF; //spritepixel is just one bit (eg 100000000), screenpixel can only be either FFFFFFFF or 00000000. If spritePixel is 1,flip the screenpixel
                                   //xor makes it so if both are on it will display nothing 
       }
     }
  }
} 
void  Chip8::OP_Ex9E(){
  uint8_t Vx= (opcode & 0x0F00) >> 8u;
  uint8_t key=registers[Vx];

  if(keypad[key]){
    pc +=2;
  }
}
void Chip8::OP_ExA1(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  uint8_t key=registers[Vx];
  
  if(!keypad[key]){
    pc +=2;
  }
}
void Chip8::OP_Fx07(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  registers[Vx] = delaytimer;
}
void Chip8::OP_Fx0A(){
     uint8_t Vx = (opcode & 0x0F00u) >> 8u;
     bool keypressed=false;

     for(int i=0; i<16; i++){
       if(keypad[i]){
         registers[Vx]=i;
         keypressed = true;
         break;
       }
     } if(!keypressed){
       pc -= 2; //to wait for a key press 
     }
}
void Chip8::OP_Fx15(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  delaytimer=registers[Vx];
}
void Chip8::OP_Fx18(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;
  soundTimer=registers[Vx];
}
void Chip8::OP_Fx1E(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;
  index += registers[Vx];
}
void Chip8::OP_Fx29(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;

  index=0x50 + 5*(registers[Vx]); // fonts are 5 bytes each, index is now at the starting byte of the number in Vx  
}
void Chip8::OP_Fx33(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;
  int digit = registers[Vx];

  memory[index+2] = digit%10;
  digit /= 10;
   
  memory[index+1] = digit%10;
  digit /= 10;

  memory[index] = digit%10;
  digit /= 10;
} 
void Chip8::OP_Fx55(){ 
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  for(int i=0;i<=Vx;i++){
    memory[index+i] = registers[i];
  }
}
void Chip8::OP_Fx65(){
   uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    for(int i=0;i<=Vx;i++){
     registers[i] = memory[index+i];
  }

}


void Chip8::cycle(){
  //fetch 
  opcode=(memory[pc] << 8u) | memory[pc+1]; //opcode is of 16 bytes  
  pc +=2;

  ((*this).*(table[(opcode & 0xF000u)>>12u]))(); //decodes and executes
  }


void Chip8::UpdateTimers(){

static int count = 0;

    count++;

    if(count % 60 == 0)
        std::cout << "1 second\n";
if (delaytimer > 0)
         --delaytimer;

if (soundTimer > 0)
        --soundTimer;
}
