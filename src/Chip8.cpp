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

void Chip8::OP_00EE(){
  --sp; // sp goes to the address that was called into the stack (subroutine)
  pc=stack[sp];
}
void Chip8::OP_1nnn(){
  uint16_t address = opcode & 0XFFFFu // anything & 1111 = number itself,seperates the address to where the jump will be done to from the opcode
  pc=address;
}
void Chip8::OP_2nnn(){
    uint16_t address = opcode & 0XFFFu;
    stack[sp]=pc; //adds the previous opcode to the top of the stack 
    sp++;
    pc=address; //stores the address of the next opcode 
}
void Chip8::OP_3xkk(){
 uint8_t Vx= (opcode & Ox0F00U) >> 8u; // finding resister number,usually the second digit of the opcode 
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
void Chip8::OP_5xy(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u; //shifts digits by 8 cuz we only need the number of register
  uint8_t Vy=(opcode & 0x00F0u) >> 4u;
 if(register[Vx] == register[Vy]){
   pc+=2;
 }
}
void Chip8::OP_6xkk(){
  uint8_t Vx= (opcode & 0x0F00U) >> 8u; // 0fuu to extract the register number 
  uint8_t byte= opcode & 0x00FFu >> 4u;
  
  register[Vx] = byte;
}
void Chip8::OP_7xkk(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  uint8_t byte=opcode & 0x00FFu;

  register[Vx] += byte;
}
void CHIP::OP_8xy(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;

  register[Vx] = register[Vy];
}
void Chip8::OP_8xy(){
  uint8_t Vx = (opcode & 0x0F00) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u; 

  register[Vx] |= register[Vy];
} 
void Chip8::OP_8xy2(){
  uint8_t Vx = (opcode & 0x0F00) >> 8u;
  uint8_t Vy = (opcode & 0x00F0) >> 4u; 
 
  register[Vx] &= register[Vy];
}
void Chip8::OP_8xy3(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;

  register[Vx] ^= register[Vy];
}
void Chip8::OP_8xy4(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;
  
  uint16_t sum=register[Vx]+register[Vy];
  if(sum > 255u){
    register[0xF]=1;
  }
  else{
    register[OxF]=0;
  }
   register[Vx] = sum & 0xFFu;
}
void Chip8::OP_8xy5(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  uint8_t Vy=(opcode & 0x00F0) >> 4u;
   
  if(register[Vx] > register[Vy]){
    register[VF]=1;
  }
  else{
    register[VF]=0;
  }
  register[Vx] -= register[Vy]
}
void Chip8::OP_8xy6(){
  uint8_t Vx=(opcode & 0x0F00) >> 8u;
  
  register[VF] = register[Vx] & 0x0001u; //to find the lsb because the shift operation destroys it
  register[Vx] >>=1u;
}

void Chip8::OP_8xy7(){
  uint8_t Vx=(opcode & 0x0f00) >> 8u;
  uint8_t Vy=(opcode & 0x00f0) >> 4u;
   
  if(register[Vy] > register[Vx]){
    register[VF]=1;
  }
  else{
    register[VF] = 0;
  }
  register[Vx] = register[Vy] - register[Vy];
}
void Chip8::OP_8xyE(){
  uint8_t Vx=(opcode & 0x0f00) >> 8u;
  register[VF] =  opcode & 0x1000; // storing msb 
  
  register[Vx] <<=2; //left shift 
}
void Chip8::OP_8xy7(){
  uint8_t Vx=(opcode & 0x0f00) >> 8u;
  uint8_t Vy= (opcode & 0x0f00) >> 4u;

  if(register[Vx] != register[Vy]){
    pc += 2;
  }
}
void Chip8::OP_Annn(){
 uint16_t address = opcode & 0x0FFFu // last 12 bits 
 index=addresss; 
}
void Chip8::Bnnn(){
uint16_t Hello=opcode & 0x0FFFu; //HELLO IS ADDRESS I WAS BORED
pc=register[0]+hello;
}
void Chip8::OP_Dxyn(){
uint8_t Vx=(opcode & 0x0F00u) >> 8u;
uint8_t Vy=(opcode & 0x00F0u) >> 4u;
uint8_t height = opcode & 0x000Fu; //video_width is 64 here 

uint8_t xPos=register[Vx] % VIDEO_WIDTH; //for out of boundary conditions
uint8_t yPos=register[Vy] % VIDEO_HEIGHT; //yPos is where it starts drawing 

register[0xF]=0; // 1 if collision of new sprite and sprite on displsy,otherwise 0 
    
  for(unsigned int row=0;row<height;row++){ //iterating through sprite rows 
     uint8_t spriteByte=memory[index+row]; //byte data of the row 
     for(unsigned int col=0;col<8;col++){

       uint8_t spritePixel=spriteByte & (0x80u >> col); //extracts column one by one 

       //pointer to the current row and column we are in 
       uint32_t* screenPixel=&video[(yPos+row)*VIDEO_WIDTH+(xPos+col)];
       

       if(spritePixel){ //spritePixel is on 

         if(*screenPixel == 0xFFFFFFFF){ // screenPixel is also on 
           register[0xFu]=1; // collision condition 
         }
         *screenPixel ^= 0xFFFFFFFF; //spritepixel is just one bit (eg 100000000), screenpixel can only be either FFFFFFFF or 00000000. If spritePixel is 1,flip the screenpixel
                                   //xor makes it so if both are on it will display nothing 
       }
     }
  }
} 
void  Chip8::OP_Ex9E(){
  uint8_t Vx= (opcode & 0x0F00) >> 8u;
  uint8_t key=register[Vx];

  if[keypad[key]]{
    pc +=2;
  }
}
void Chip8::OP_ExA1{
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  uint8_t key=register[vx];
  
  if(!keypad[key]){
    pc +=2;
  }
}
void Chip8::Fx07(){
  uint8_t Vx = (opcode & 0x0F00u) >> 8u;
  register[Vx] = delayTimer;
}
void Chip8::Fx0A(){
     uint8_t Vx = (opcode & 0x0F00u) >> 8u;
     bool keypressed=false;

     for(int i=0; i<16; i++){
       if(keypad[i]){
         register[Vx]=i;
         keypressed = true;
         break;
       }
     }
     if(!kepressed){
       pc -= 2; //to wait for a key press 
     }
}
void OP_Fx15(){
  uint8_t Vx= (opcode & 0x0F00u) >> 8u;
  delayTimer=register[Vx];
}
void OP_Fx18(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;
  soundTimer=register[Vx];
}
void OP_Fx1E(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;
  index += register[Vx];
}
void OP_Fx29(){
  uint8_t Vx=(opcode & 0x0F00u) >> 8u;

  index=FONT_START_ADDRESS + 5*(register[Vx]) // fonts are 5 bytes each, index is now at the starting byte of the number in Vx
}


