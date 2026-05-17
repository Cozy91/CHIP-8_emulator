#include<cstdint>

class Chip8
{
  public:
    Chip8(); //constructor 
             
    //various opcodes(instructions) :         

    OP_00E0();//instruction for clearing display 
    OP_00EE();//instruction for RET, goes back to where the subroutine was called from
    OP_1nnn(); //insruction for jump addr
    OP_2nnn(); //insruction for call 
    OP_3xkk(); // skip if register(Vx)=byte(opcode last two digits)
    OP_4xkk(); // skip if regisrer!=byte 
    OP_5xy0(); // skip if vx=vy 
    OP_6xkk(); //set vx=kk 
    OP_7xkk(); // set Vx=Vx+kk 
    OP_8xy0(); // set vx=vy
    OP_8xy1(); // set vx=vx OR vy 
    OP_8xy2(); // set Vx=Vx and Vy
    OP_8xy3(); // xor vx vy 
    OP_8xy4(); //do vx = vx+vy and if the sum is greater than 8 bits,give carry to VF 
    OP_8xy5(); // sub vx,vy  
    OP_8xy6(); // shifting of pixels basically, lsb method
    OP_8xy7(); // sub vy,vx
    OP_8xyE(); //msb method
    OP_9xy0(); //skip next instruction if vx!=vy 
    OP_Annn(); // setting index to the current address
    OP_Bnnn(); //jump to location nnn
    OP_Cxkk(); // Set Vx = random byte AND kk
    OP_Dxyn(); // disaplay instruction 
    OP_Ex9E(); //skip if key pressed is the value in vx
    OP_ExA1(); //skip if key pressed is not the value in vx
    OP_Fx07(); //set delay time=value in vx,get timer 
    OP_Fx0A(); //wait for key press,store the value in the key in vx;
    OP_Fx15(); // set delay timer = Vx
    OP_Fx18(); // set soundTimer=Vx;
    OP_Fx1E(); // set index I = I+Vx;
    OP_Fx29(); // index=location of sprite for digit Vx
    OP_Fx33(); // stores bcd of Vx with 100th,tens,ones at I,I+1 and I+2
    OP_Fx55(); // stores registers from V0 to Vx
    OP_Fx65(); // fills registers V0 to Vx with values from memory
  private:
   uint8_t registers[16]{}; // 16 one byte register  or 16 8-bit registers
                             
    uint8_t memory[4096]{}; //4096 bytes for memory (each byte is one space of memory from 0 to 4096, memory is stored in bits)

    uint16_t index{}; //pointer to where data is in memory 
    uint16_t pc{}; 
    uint16_t stack[16]{};
    uint8_t  sp{};
    uint8_t  delaytimer{};
    uint8_t  soundTimer{};
    uint8_t  keypad[16]{};
    uint32_t VIDEO[64*32]{};
    uint16_t opcode; //instruction 
    void LoadRom(const char* filename)

  // giving memory to font bytes, each character sprite if of 5 bytes. We need to allocate memory for these bytes- 5 bytes for each character from 0 to F(15)

    const unsigned int FONTSET_SIZE=80; //16*5=80 

    uint8_t font_set[FONTSET_SIZE]= // could have made ab adjacency matrix of 16*5 but memory in Chip8 is 1 dimensional so ill stick to this 
   {
     0xf0, 0x90, 0x90, 0x90, 0xf0, // 0,stored as this as to display 0 in screen row by row 
     0x20, 0x60, 0x20, 0x20, 0x70, // 1
	   0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
     0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	   0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	   0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	   0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	   0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	   0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	   0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
 	   0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	   0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	   0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	   0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	   0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	   0xF0, 0x80, 0xF0, 0x80, 0x80  // F
   };
  }

typedef void Chip8::*Chip8Func() //okay so Chip8Func is basically a pointer to a member function of Chip8

//making a master table where every function pointer will point to a function that indexes corectly based on the relevant parts of the opcode. Ex we'll make a seperate table for all the functions startin from 8xy and then store a function pointer to that table in this master table. This will be a 1d table 
// first we will create like tables to store the addresses of the opcode functions that are similar like 0x000E and 0x00EE 
 Chip8Func table[0xFu +1]; // master table, array of pointers to member functions and some tables 
Chip8func table0[0xEu +1]; //000E and 00EE
Chip8Func table8[0xEu +1]; // opcodes starting from 8xy
Chip8Func tableE[0xEu +1]; // opcode with Ex
Chip8Func tableF[065u +1]; // opcodes starting with F, they end with 07,0A . . . goes upto max 65 so array has 65+1 
void Table0(){
  ((*this)).*(table0[opcode & 0x000Fu])(); //deferencing the object then indexing the opcode we want 
}
void Table8(){
  ((*this)).*(table8[opcode & 0x000Fu])();
}
void TableE(){
  ((*this)).*(tableE[opcode & 0x000Fu])();
}
void TableF(){
  ((*this)).*(tableE[opcode & 0x00FFu])();
}
void OP_NULL(){}

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
table[0xE] =  &Chip8::OP_00EE;

table8[0x0] = &Chip8::OP_8xy0;
table8[0x1] = &Chip8::OP_8xy1;
table8[0x2] = &Chip8::OP_8xy3;
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

