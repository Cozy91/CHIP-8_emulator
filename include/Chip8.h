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
    OP_5xy(); // skip if vx=vy 
    OP_6xkk(); //set vx=kk 
    OP_7xkk(); // set Vx=Vx+kk 
    OP_8xy(); // set vx=vy
    OP_8xy(); // set vx=vx OR vy 
    OP_8xy2(); // set Vx=Vx and Vy
    OP_8xy3(); // xor vx vy 
    OP_8xy4(); //do vx = vx+vy and if the sum is greater than 8 bits,give carry to VF 
    OP_8xy5(); // sub vx,vy  
    OP_8xy6(); // shifting of pixels basically, lsb method
    OP_8xy7(); // sub vy,vx
    OP_8xyE(); //msb method
    OP_8xyE(); //skip next instruction if vx!=vy 
    OP_Annn(); // setting index to the current address
    OP_Bnnn(); //jump to location nnn
    OP_Dxyn(); // disaplay instruction 
    OP_Ex9E(); //skip if key pressed is the value in vx
    OP_ExA1(); //skip if key pressed is not the value in vx
    OP_Fx07(); //set delay time=value in vx,get timer 
    OP_Fx0A(); //wait for key press,store the value in the key in vx;
    OP_Fx15(); // set delay timer = Vx
    OP_Fx18(); // set soundTimer=Vx;
    OP_Fx1E(); // set index I = I+Vx;
    OP_Fx29(); // index=location of sprite for digit Vx
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
