//#if 0
#include <iostream>

#include <iomanip>
#include "Interpreter.h"
#include "q3.h"
using namespace std;


void strcopy(char* a, const char* b) {
    int i = 0;
    while (b[i] != '\0') {

        a[i] = b[i];
        i++;
    }
    a[i] = '\0';
}
class MainBoard;
class RegisterBank {
	unsigned short R[8];
	unsigned short PC, IR;

	unsigned char FLAGS;
public:
	RegisterBank() {
		for (int i = 0; i < 8; i++) {

			R[i] = 0;
		}
			PC = 0;
			IR = 0;
			FLAGS = 0;
	}
	unsigned short read(int i) {
		return R[i];
	}
    void write(int i, unsigned short v) {

        R[i] = v;
    }
	unsigned short getPC() {
		return PC;

	}
	void setPC(unsigned short v) {
		PC = v;
	}
    void incPC(int x) {
        PC += x; 
    }
    void setIR(unsigned short val) {
        IR = val;
    }
    unsigned short getIR() {
        return IR;
    }
	unsigned char getFLAGS() {
		return FLAGS;
	}
	void zf(bool v) {
		FLAGS &= ~static_cast < unsigned char>(0x07);

		if (v)
			FLAGS |= 0x01;
	}
	void setcmp(int r) {
        FLAGS &= ~static_cast < unsigned char>(0x07);

		if (r == 0) {
			FLAGS |= 0x01; // setting ZF
		}
		else if (r < 0) {
			FLAGS |= 0x02; // setting NF

		}
		else
		{
			FLAGS |= 0x04; // setting PF
		}
	}


	bool getzf(){
		return ((FLAGS & 0x01) != 0);
	}

	bool gethf() {
		return ((FLAGS & 0x80) != 0);
	}

	void hf() {
		FLAGS |= 0x80;

	}
};

class ALU {
public:
	unsigned short execute(int op, unsigned short A, unsigned short B, RegisterBank& regi) {
		unsigned short r = 0;
		if (op == 1) {
			r = A + B;
		}
		else if (op == 2) {

			r = A - B;
		}
		else if (op == 3) {
			r = A * B;
		}
		else if (op == 4) {
			if (B == 0) {
				r = 0;
			}
			else
				r = (A / B);

		}
		else if (op == 5) {
			r = A & B;
		}
		else if (op == 6) {
			r = A | B;
		}
		else if (op == 7) {
			r = A ^ B;
		}
		else if (op == 8) {
			r = ~A;
		}
		else if (op == 9) {
			r = A + 1;
		}
		else if (op == 11) {
			r = A - 1;
		}
		else if (op == 10) {
			if (A == B)

				regi.setcmp(0);
			else if (A < B)
				regi.setcmp(-1);
			else
				regi.setcmp(1);
			return 0;
		}
		regi.zf(r == 0);
		return r;
	}
};

class Keyboard {
    char* temp;
    int capacity;
    int length;
    void inittemp(int size = 1024) {
        capacity = size;

        temp = new char[capacity];
        length = 0;
        temp[0] = '\0';

    }
    int strlength(const char* s) {
        int i = 0;

        while (s[i] != '\0') i++;
        return i;
    }
    void strcopy(char* a, const char* b) {
        int i = 0;
        while (b[i] != '\0') {
            a[i] = b[i];
            i++;
        }
        a[i] = '\0';
    }

    void append(const char* input) {
        int inputlen = strlength(input);

        if (length + inputlen + 2 >= capacity) {
            capacity = (length + inputlen + 2) * 2;
            char* newtemp = new char[capacity];

            for (int i = 0; i < length; i++)
                newtemp[i] = temp[i];

            delete[] temp;
            temp = newtemp;
        }

        for (int i = 0; i < inputlen; i++) {
            temp[length++] = input[i];
        }
        temp[length++] = ' ';
        temp[length] = '\0';
    }

    void fill() {
        cout << "\n[Hardware Interrupt] Awaiting Keyboard Input: ";

        char input[1024];
        cin.getline(input, 1024);
        append(input);
    }

    void trim() {
        int i = 0;
        while (temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\t' || temp[i] == '\r') {
            i++;
        }

        if (i > 0) {
            int j = 0;
            while (temp[i] != '\0') {
                temp[j++] = temp[i++];
            }
            temp[j] = '\0';
            length = j;
        }
    }

public:
    Keyboard() {
        inittemp();
    }
    ~Keyboard() {
        delete[] temp;
    }

    unsigned char readchar() {
        trim();
        if (length == 0) 
            fill();
        trim();
        if (length == 0) 
            return 0;

        unsigned char c = static_cast<unsigned char>(temp[0]);
        for (int i = 1; i < length; i++) {


            temp[i - 1] = temp[i];
        }

        length--;
        temp[length] = '\0';
        return c;
    }

    unsigned char readint() {
        trim();
        if (length == 0) 
            fill();
        trim();

        int num = 0;
        int i = 0;
        while (temp[i] != '\0' && temp[i] != ' ' && temp[i] != '\n' && temp[i] != '\t') {

            if (temp[i] >= '0' && temp[i] <= '9') {

                num = num * 10 + (temp[i] - '0');
            }
            i++;
        }
        while (temp[i] == ' ' || temp[i] == '\n' || temp[i] == '\t')

            i++;
        int j = 0;
        while (temp[i] != '\0') {

            temp[j++] = temp[i++];
        }

        temp[j] = '\0';

        length = j;

        return static_cast<unsigned char>(num & 0xFF);
    }

};

class PhosphorDisplay {
    char* temp;
    int capacity;

    int length;

    /*void init(int size = 1024) {
        capacity = size;
        temp = new char[capacity];
        length = 0;
        temp[0] = '\0';
    }*/

    /*void resize(int extra) {
        if (length + extra >= capacity) {
            capacity = (length + extra) * 2;
            char* newtemp = new char[capacity];
            for (int i = 0; i < length; i++)
                newtemp[i] = temp[i];
            newtemp[length] = '\0';

            delete[] temp;
            temp = newtemp;
        }
    }*/

public:
    PhosphorDisplay() {

        capacity = 1024;
        temp = new char[capacity];

        length = 0;
        temp[0] = '\0';
    }

    ~PhosphorDisplay() {

        delete[] temp;
    }


    void putchar(char c) {
        int extra = 2;
        if (length + extra >= capacity) {

            capacity = (length + extra) * 2;
            char* newtemp = new char[capacity];
            for (int i = 0; i < length; i++)

                newtemp[i] = temp[i];
            newtemp[length] = '\0';


            delete[] temp;
            temp = newtemp;
        }
        temp[length] = c;
        length++;

        temp[length] = '\0';
    }

    void putint(int n) {
        if (n == 0) {

            putchar('0');

            return;
        }
        char digits[12];
        int i = 0;
        while (n > 0) {

            digits[i++] = static_cast<char>((n % 10) + '0');

            n /= 10;
        }

        for (int j = i - 1; j >= 0; j--) {

            putchar(digits[j]);
        }
    }
    void f() {
        const int col = 32;

        const int row = 16;

        char lines[row][col];
        int linelen[row];

        for (int r = 0; r < row; r++) {
            linelen[r] = 0;
            for (int c = 0; c < col; c++)
                lines[r][c] = ' ';
        }

        int r = 0, c = 0;
        for (int k = 0; k < length; k++) {
            if (r >= row)
                break;

            char ch = temp[k];

            if (ch == '\n') {
                r++;
                c = 0;
            }
            else {
                if (c >= col) {
                    r++;
                    c = 0;
                    if (r >= row)
                        break;
                }
                lines[r][c] = ch;
                c++;

                if (linelen[r] < c)
                    linelen[r] = c;
            }
        }

        cout << "\033[92m" << "+" << "\033[0m";
        for (int i = 0; i < col; i++) 
            cout << "\033[92m" << "-" << "\033[0m";
        cout << "\033[92m" << "+" << "\033[0m" << endl;

        cout << "\033[92m" << "|" << "\033[0m" << " PHOSPHOR CRT DISPLAY RENDER  ";
        for (int i = 30; i < col; i++) {
            cout << " ";
        }
        cout << "\033[92m" << "|" << "\033[0m" << endl;

        cout << "\033[92m" << "+" << "\033[0m";
        for (int i = 0; i < col; i++) {
            cout << "\033[92m" << "-" << "\033[0m";
        }
        cout << "\033[92m" << "+" << "\033[0m" <<endl;

        int rrows = 16;
        for (int r = 0; r < row; r++) {
            if (linelen[r] > 0)
                rrows = r + 1;
        }

        if (rrows < 10)
            rrows = 16;

       
        for (int r = 0; r < rrows; r++) {

            cout << "\033[92m" << "|" << "\033[0m";
            for (int c = 0; c < col; c++) {
                cout << lines[r][c];
            }
            cout << "\033[92m" << "|" << "\033[0m" << endl;
        }


        cout << "\033[92m" << "+" << "\033[0m";
        for (int i = 0; i < col; i++)
            cout << "\033[92m" << "-" << "\033[0m";

        cout << "\033[92m" << "+" << "\033[0m" << endl;
    }
};

class GraphicsAdapter {
    PhosphorDisplay* disp;

    bool act;

public:

    GraphicsAdapter(PhosphorDisplay* d) : disp(d), act(false) {}

    void sendchar(unsigned char c) { 
        act = true; 
        (*disp).putchar(static_cast<char>(c));

    }
    void sendint(unsigned char n) { 

        act = true;
        (*disp).putint(static_cast<int>(n));
    }
    double getpowerdraw() {
        double draw;
            if (act)
                draw = 15.0;
            else 
                draw = 2.0;

        act = false;
        return draw;
    }
};

class PSU {

    double capacity;
    MainBoard* m;

public:
    PSU(double cap) : capacity(cap), m(nullptr){}


    void attachMainboard(MainBoard* mm) {
        m = mm;
    }
    void checkpow(double totaldraw);
};

class MainBoard {
    MemoryModule* mem;
    Keyboard* kb;
    GraphicsAdapter* gpu;
    PSU* psu;
    bool powered;
    unsigned char read(unsigned short addr) {
        if (addr == 0x0FF0) 
            return kb ? kb->readchar() : 0;
        if (addr == 0x0FF3)
            return kb ? kb->readint() : 0;
        /*if (addr <= 0x0EFF) {
            if (mem)
                return mem->read(addr);
            return 0x00;
        }

        if (addr >= 0x0F00 && addr <= 0x0FEF) {
            cout << "[HARDWARE FAULT] Segmentation Fault: Read out of bounds" << endl;
            return 0xFF;
        }
        */

        if (addr <= 0x0EFF) { 
            return mem ? mem->read(addr) : 0x00;
        }
        
        if (addr <= 0x0FEF) { 
            cout << "[HARDWARE FAULT] Segmentation Fault: Read out of bounds" << endl; 
            
            return 0xFF;
        }

        cout << "[MOTHERBOARD FAULT] Invalid Read Address" << endl;
        return 0x00;

    }

    void write(unsigned short addr, unsigned char val) {
        if (addr == 0x0FF1) {
            if (gpu) 
                gpu->sendchar(val);
            return;
        }

        if (addr == 0x0FF2) { 
            if (gpu)
                gpu->sendint(val);
            return;
        }

       /* if (addr <= 0x0EFF) {
            if (mem)
                mem->write(addr, val);
            return;
        }

        if (addr >= 0x0F00 && addr <= 0x0FEF) {
            cout << "[HARDWARE FAULT] Segmentation Fault: Write out of bounds" << endl;
            return;
        }*/

        if (addr <= 0x0EFF) {
            if (mem) 
                
                mem->write(addr, val);
            return;
        }

        if (addr <= 0x0FEF) {
            cout << "[HARDWARE FAULT] Segmentation Fault: Write out of bounds" << endl;

            return;
        }

        cout << "[MOTHERBOARD FAULT] Invalid Write Address" << endl;
    }

public:
    MainBoard() : mem(nullptr), kb(nullptr), gpu(nullptr), psu(nullptr), powered(true) {}

    void attachMemory(MemoryModule* m) {
        mem = m;
    }
    void attachKeyboard(Keyboard* k) {
        kb = k;
    }
    void attachGPU(GraphicsAdapter* g) { 
        gpu = g;
    }
    void attachPSU(PSU* p) { 
        psu = p;
    }

    bool isPowered() {
        return powered;
    }

    unsigned char pulseClock(unsigned short addr, bool isWrite = false, unsigned char val = 0) {
        if (!powered)
            return 0;
        if (isWrite) {
            write(addr, val);
            return 0; 
        }
        return read(addr);
    }

    void killPower() {
        cout << "[CRITICAL WARNING] Power Draw exceeded PSU Capacity" << endl;

        powered = false;
    }

/*
    unsigned char read(unsigned short addr) {
        if (addr == 0x0FF0)
            return kb ? kb->readchar() : 0;
        if (addr == 0x0FF3)
            return kb ? kb->readint() : 0;

        if (addr <= 0x0EFF && mem) 
            return mem->read(addr);
        if (addr <= 0x0FFF && mem) 
            return mem->read(addr);

        cout << "[MOTHERBOARD FAULT] Invalid Read Address" << endl;
        return 0x00;
    }

    void write(unsigned short addr, unsigned char val) {
        if (addr == 0x0FF1) {
            if (gpu) 
                gpu->sendchar(val);
            return; }
        if (addr == 0x0FF2) {
            if (gpu)
                gpu->sendint(val); 
            return;
        }

        if (addr <= 0x0EFF && mem) {
            mem->write(addr, val);
            return;
        }
        if (addr <= 0x0FFF && mem) {
            mem->write(addr, val); 
            return; 
        }

        cout << "[MOTHERBOARD FAULT] Invalid Write Address" << endl;
    }
    */

    void pollPower() {
        if (!psu)
            return;
        double draw = 0;
        if (mem)
            draw += mem->getPow();
        if (gpu)
            draw += gpu->getpowerdraw();
        psu->checkpow(draw);
    }
};
void PSU::checkpow(double totalDraw) {

    if (totalDraw > capacity)
        m->killPower();
}


struct Signal {
    bool IS_VALID;
    bool USES_AEC;
    int AEC_OPERATION;
    bool IS_MEMORY_READ;
    bool IS_MEMORY_WRITE;
    bool IS_BRANCH;
    bool BRANCH_ON_ZERO;
    bool BRANCH_ON_NOT_ZERO;
    bool WRITES_TO_REGISTER;
    bool IS_4BIT_IMMEDIATE;
    bool IS_16BIT_IMMEDIATE;
};
//Signal decode[500];
/*
void initdecode() {
    for (int i = 0; i < 500; i++) {
        decode[i] = { false,false,0,false,false,false,false,false,false,false,false };
    }

    decode[0x00] = { true, false, 0, false, false, false, false, false, false, false, false };
    decode[0x01] = { true, true,  1, false, false, false, false, false, true,  false, false };
    decode[0x02] = { true, true,  2, false, false, false, false, false, true,  false, false };
    decode[0x03] = { true, true,  3, false, false, false, false, false, true,  false, false };
    decode[0x04] = { true, true,  4, false, false, false, false, false, true,  false, false };
    decode[0x0A] = { true, true, 10, false, false, false, false, false, false, false, false };
    decode[0x10] = { true, false, 0, false, false, true,  false, false, false, false, false };
    decode[0x11] = { true, false, 0, false, false, true,  true,  false, false, false, false };
    decode[0x12] = { true, false, 0, false, false, true,  false, true,  false, false, false };
    decode[0x1A] = { true, false, 0, false, false, false, false, false, true,  true,  false };
    decode[0x1B] = { true, false, 0, false, false, false, false, false, true,  false, true };
    decode[0x20] = { true, false, 0, true,  false, false, false, false, true,  false, false };
    decode[0x21] = { true, false, 0, false, true,  false, false, false, false, false, false };
}*/

class BlockCache {
    unsigned char block[16];
    unsigned short base;
    bool valid;

public:
    BlockCache() : base(0), valid(false) {}


    bool hit(unsigned short addr) {
        return valid && addr >= base && addr < static_cast<unsigned short>(base + 16);

    }

    unsigned char get(unsigned short addr) { 
        return block[addr - base]; 
    }

    void load(unsigned short addr, MainBoard* mb) {
        base = addr & (unsigned short)(~0x0F); 
        for (int i = 0; i < 16; i++)
            block[i] = mb->pulseClock(static_cast<unsigned short>(base + i));

            // block[i] = mb->read((unsigned short)(base + i));
        valid = true;
    }

    void invalidate(unsigned short addr) {
        if (valid && addr >= base && addr < static_cast<unsigned short>(base + 16))

            valid = false;
    }
};

class Processor {
    RegisterBank reg;
    ALU alu;
    BlockCache cache;
    MainBoard* mb;
    double temperature;
    bool v = false;
    Signal decode[500];

    void initdecode() {
        for (int i = 0; i < 500; i++) {
            decode[i] = { false,false,0,false,false,false,false,false,false,false,false };
        }

        decode[0x00] = { true, false, 0, false, false, false, false, false, false, false, false };
        decode[0x01] = { true, true,  1, false, false, false, false, false, true,  false, false };
        decode[0x02] = { true, true,  2, false, false, false, false, false, true,  false, false };
        decode[0x03] = { true, true,  3, false, false, false, false, false, true,  false, false };
        decode[0x04] = { true, true,  4, false, false, false, false, false, true,  false, false };
        decode[0x0A] = { true, true, 10, false, false, false, false, false, false, false, false };
        decode[0x10] = { true, false, 0, false, false, true,  false, false, false, false, false };
        decode[0x11] = { true, false, 0, false, false, true,  true,  false, false, false, false };
        decode[0x12] = { true, false, 0, false, false, true,  false, true,  false, false, false };
        decode[0x1A] = { true, false, 0, false, false, false, false, false, true,  true,  false };
        decode[0x1B] = { true, false, 0, false, false, false, false, false, true,  false, true };
        decode[0x20] = { true, false, 0, true,  false, false, false, false, true,  false, false };
        decode[0x21] = { true, false, 0, false, true,  false, false, false, false, false, false };
    }

    bool check() {
        temperature += 0.05;
        if (temperature > 90.0) {

            cout << "[CRITICAL ERROR] Thermal threshold exceeded" << endl;
            reg.hf();
            return true;
        }
        return false;
    }

    unsigned char fetch(unsigned short addr) {
       // if (addr >= 0x0F00) 
         //   return mb->pulseClock(addr);
        if (!cache.hit(addr))
            
            cache.load(addr, mb);
        return cache.get(addr);
    }

    unsigned char read(unsigned short addr) {
        return mb->pulseClock(addr);
    }

    void write(unsigned short addr, unsigned char val) {

        cache.invalidate(addr);
        mb->pulseClock(addr, true, val);
    }

    void state() {
        cout << "\n=== PROCESSOR STATE DUMP ===" << endl;
        cout << "Temperature: " << fixed << setprecision(1) << temperature << " C (Max: 90 C)" << endl<<endl;
        cout << "---- INTERNAL REGISTERS (16-bit) ----" << endl;
        for (int i = 0; i < 8; i++)
            cout << "R" << i << ": 0x" << hex << setw(4) << setfill('0') << reg.read(i) << dec << endl;

        cout << "PC: 0x" << hex << setw(4) << setfill('0') << reg.getPC() << dec << endl;
        cout << "IR: 0x" << hex << setw(4) << setfill('0') << reg.getIR() << dec << endl;

        cout << "FLAGS: 0x" << hex << setw(2) << setfill('0') << (int)reg.getFLAGS() << dec << endl;
        cout << "============================" << endl << endl;
    }

public:
    Processor(MainBoard* m) : mb(m), temperature(25.0) {
        initdecode();
    }

    void setV(bool val) { 

        v = val;
    }

    void run() {
        //initdecode();

        while (true) {
            
            if (!mb->isPowered())
                return;

            unsigned short pc = reg.getPC();

            unsigned char  high = fetch(pc);

            unsigned char  low = fetch(pc + 1);

            unsigned short i = static_cast<unsigned short>((high << 8) | low);
            reg.setIR(i);
            reg.incPC(2);

            int opcode = (i >> 8) & 0xFF;
            int a = (i >> 4) & 0x0F;

            int b = i & 0x0F;
            if (opcode == 0x00) {

                cout << "\n[PROCESSOR] End-Of-File (0x0000) reached at PC 0x" << hex << reg.getPC() << ". Halting." << dec << endl;

                cout << "\n------------------------------------------------------------" << endl;
                state();

                return;
            }

            Signal& sv = decode[opcode];

            if (!sv.IS_VALID) {
                cout << "[HARDWARE FAULT] Invalid Instruction" << endl;

                if (check())
                    return;
                /*temperature += 0.05;
                if (temperature > 90.0) {
                    cout << "[CRITICAL ERROR] Thermal threshold exceeded" << endl;

                    reg.hf();
                    return;
                }*/
                mb->pollPower();
                continue;
            }
            unsigned short imm16 = 0;
            if (sv.IS_16BIT_IMMEDIATE) {
                unsigned short pc2 = reg.getPC();
                imm16 = static_cast<unsigned short>((fetch(pc2) << 8) | fetch(pc2 + 1));

                reg.incPC(2);
                if (v)
                    cout << "\033[36m" << "-> Decode: Format D (2-Word). Fetched Immediate: 0x" << hex << setw(4) << setfill('0') << imm16 << dec << "\033[0m" << endl;
            }

            unsigned short A = reg.read(a);
            unsigned short B = reg.read(b);

            unsigned short result = 0;

            if (sv.USES_AEC)
                result = alu.execute(sv.AEC_OPERATION, A, B, reg);

            if (sv.IS_MEMORY_READ) {
                result = static_cast<unsigned short>(read(B));
                if (v)

                    cout << "\033[36m" << "-> Exec: Read 0x" << hex << result << " from Mem Address 0x" << B << dec << "\033[0m" << endl;
            }
            if (sv.IS_MEMORY_WRITE) {
                write(B, static_cast<unsigned char>(A & 0xFF));
                if (v)
                    cout << "\033[36m" << "-> Exec: Store 0x" << hex << (A & 0xFF) << " to Mem Address 0x" << B << dec << "\033[0m" << endl;
            }
            if (sv.IS_BRANCH) {
                bool jump = (!sv.BRANCH_ON_ZERO && !sv.BRANCH_ON_NOT_ZERO) || (sv.BRANCH_ON_ZERO && reg.getzf()) || (sv.BRANCH_ON_NOT_ZERO && !reg.getzf());

                if (jump) 

                    reg.setPC(B);
            }

            if (sv.IS_4BIT_IMMEDIATE)
                result = static_cast<unsigned short>(b & 0x0F);

            if (sv.IS_16BIT_IMMEDIATE)

                result = imm16;

            if (sv.WRITES_TO_REGISTER) {

                reg.write(a, result);
                if (v)
                    cout << "\033[36m" << "-> Writeback: R" << a << " = " << dec << result << " (0x" << hex << result << ")" << dec << "\033[0m" << endl;
            }
            if (check())
                return;
            /*temperature += 0.05;

            if (temperature > 90.0) {
                cout << "[CRITICAL ERROR] Thermal threshold exceeded" << endl;

                reg.hf();
                return;
            }*/

            mb->pollPower();
           
        }
    }
};
int main() {
    MemoryModule mem;
    Keyboard kb;

    PhosphorDisplay disp;
    GraphicsAdapter gpu(&disp);
    PSU psu(50.0);

    MainBoard mb;
    mb.attachMemory(&mem);
    mb.attachKeyboard(&kb);
    mb.attachGPU(&gpu);

    mb.attachPSU(&psu);
    psu.attachMainboard(&mb);

    Processor cpu(&mb);
    Interpreter I;


    cout << "\033[2J\033[H";
    
    cout << endl << endl << endl;
    cout << "                   L a z a r u s  M a c h i n a e" << endl;
    cout << endl << endl;
    cout << "\033[36m" <<"                           O N Y X - 1 6" << "\033[0m" << endl;
    cout << "\033[90m" << "                16 - Bit Virtual Turing Architecture" << "\033[0m" << endl << endl << endl<<endl;
    cout << "____________________________________________________________" << endl << endl;
    cout << "\033[92m" << "--- BOOTING SILICON PROTOCOL ---" << "\033[0m" << endl <<endl;
    cout << "\033[90m" <<"[BIOS] Initializing Custom OS Interpreter..." << "\033[0m" << endl <<endl;

    // Boot menu
    cout << "                ====================================" <<endl;
    cout << "                     O N Y X  B O O T  M E N U" << endl;
    cout << "                ====================================" << endl << endl;
    cout << "\033[36m" << "  [1]" << "\033[0m" << " Turing Complete Roman Urdu Calculator" << endl;
    cout << "\033[36m" << "  [2]" << "\033[0m" << " 'HELLO WORLD' Urdu Printer" << "\033[0m" << endl;
    cout << "\033[36m" << "  [3]" << "\033[0m" << " Hardware Authentication Firewall" << "\033[0m" << endl;

    int choice;
    cout << "\033[93m" <<  "\nSelection (1 - 3): " << "\033[0m";
    cin >> choice;
    cin.ignore();

    /*string file;
    if (choice == 1) file = "programs/calculator.txt";
    else if (choice == 2) file = "programs/hello.txt";
    else if (choice == 3) file = "programs/auth.txt";
    else { cout << "[BIOS FAULT] Invalid Selection" << endl; return 1; }
    */

    char file[100];
    if (choice == 1) 
        strcopy(file, "calculator.txt");
    else if (choice == 2)
        
        strcopy(file, "hello.txt");
    else if (choice == 3) 
        strcopy(file, "auth.txt");
    else { cout << "[BIOS FAULT] Invalid Selection" << endl; 
    return 1;
    
    }
  
    char vchoice;
    cout << "Enable Cycle-by-Cycle Verbose Debug Logging? (Y/N): ";
    cin >> vchoice;
    cin.ignore();
    bool verbose = (vchoice == 'Y' || vchoice == 'y');

    cpu.setV(verbose);

    I.loadProgramAndFlash(file, mem);
    cpu.run();

    disp.f();
    return 0;
}

//#endif