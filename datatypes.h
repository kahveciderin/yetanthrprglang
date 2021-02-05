#include <stdint.h>

class reg{

};

class reg8bit : reg{
    public:
    uint8_t r_h;
    uint8_t r;

    void convert(uint16_t reg){
        r_h = (reg & 0xFF00) >> 8;
        r = reg & 0xFF;
    }

    void increase(uint16_t val){
        
    }
};

class reg16bit : reg{
    public:
    uint16_t r_h;
    uint16_t r;

    void convert(uint32_t reg){
        r_h = (reg & 0xFFFF0000) >> 16;
        r = reg & 0xFFFF;
    }
};

class reg32bit : reg{
    public:
    uint32_t r_h;
    uint32_t r;

    void convert(uint64_t reg){
        r_h = (reg & 0xFFFFFFFF00000000) >> 32;
        r = reg & 0xFFFFFFFF;
    }
};


class reg64bit : reg{
    public:
    uint64_t r;
};


class machine{
    public:
    reg32bit pc;
    reg16bit sp;
    reg32bit* addrspace;
    reg32bit reg[16];
    reg32bit tmp0;
    reg32bit tmp1;
    reg32bit tmp2;
    reg32bit null;
    uint8_t halt = 0;

};

enum addrmodes{

    AM_NONE,
    AM_REG,
    AM_VAL

};


struct label{

    char name[32];
    uint16_t addr;

};
#include "functions.cpp"