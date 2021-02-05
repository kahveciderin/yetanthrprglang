unsigned int str2int(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2int(str, h+1) * 33) ^ str[h];
}

constexpr unsigned int str2intc(const char* str, int h = 0)
{
    return !str[h] ? 5381 : (str2intc(str, h+1) * 33) ^ str[h];
}



uint32_t* convert(string line){
    static uint32_t data[4];
    string cmd;
    cmd += line[0];
    cmd += line[1];
    cmd += line[2];

    switch(str2int(cmd.c_str())){
    case str2intc("add"):
        data[0] = 0x0;
    break;
    case str2intc("mov"):
        data[0] = 0x1;
    break;
    case str2intc("hlt"):
        data[0] = 0x2;
    break;
    case str2intc("jmp"):
        data[0] = 0x3;
    break;
    case str2intc("joz"):
        data[0] = 0x4;
    break;
    case str2intc("out"):
        data[0] = 0x5;
    break;
    }

    int j = 0;
    uint16_t addrmode = 0;
    char* line_converted = (char*)line.c_str();
    char *ptr = strtok(line_converted, " ");
    ptr = strtok(NULL, " ");
	while(ptr != NULL)
	{
        string s(1, ptr[2]);
        
        char* gstr = (char*)malloc(sizeof(char) * 15);
        sprintf(gstr, "%.*s", 15, ptr + 1);
        switch(ptr[0]){
            case '%':
            addrmode = addrmode | 1 << ((2 - j) * 4);
            data[j + 1] = stoi(s, 0, 16);
            
            break;

            case '$':
            addrmode = addrmode | 2 << ((2 - j) * 4);
            data[j + 1] = stoi(gstr, 0, 16);
            break;
        }
		ptr = strtok(NULL, " ");
        j++;
	}
    data[0] = data[0] | addrmode << 16;
    //printf("%08x\n", data[0]);
    return data;
}


reg32bit* compile(string code){
    static reg32bit data[0xFFFFFFF];
    label labels[65535];


    int q = 1;
    memcpy(labels[0].name, "", 0);
    labels[0].addr = 0x0000;
    int g=0;
    for(int i = 0; i < code.length(); i++){

        string line;
        
        while(code[i] != '\n'){
            string s;
            s = code[i]; 
            line.append(s);
            i++;
        }
        uint32_t *buff = convert(line);
        data[g].r = buff[0];
        data[g+1].r = buff[1];
        data[g+2].r = buff[2];
        data[g+3].r = buff[3];

        g=4;
        }
return data;   
}







void run(machine* cpu){
    
    //0000 00 first four is the addr mode, second two is the opcode. Let's get them!
    uint16_t addrmode = (cpu->addrspace[cpu->pc.r].r & 0xFFFF0000) >> 16; //addrmode here
    uint8_t opcode = (cpu->addrspace[cpu->pc.r].r & 0x00FF);//opcode here

    addrmodes regvals[3];

    uint8_t increase = 4;


    reg32bit *reg0;
    reg32bit *reg1;
    reg32bit *reg2;

    cpu->null.r = 0;

    cpu->tmp0.r = 0;
    cpu->tmp1.r = 0;
    cpu->tmp2.r = 0;
    //printf("%08x %08x %08x %08x\n", cpu->addrspace[cpu->pc.r].r, cpu->addrspace[cpu->pc.r + 1].r, cpu->addrspace[cpu->pc.r + 2].r, cpu->addrspace[cpu->pc.r +3].r);
    switch(addrmode & 0x0F00){
        case 0x0000:
        regvals[0] = AM_NONE;
        reg0 = &(cpu->tmp0);
        break;

        case 0x0100:
        regvals[0] = AM_REG;
        reg0 = &(cpu->reg[(cpu->addrspace[cpu->pc.r + 1].r & 0xF)]);
        break;

        case 0x0200:
        regvals[0] = AM_VAL;
        reg0 = &(cpu->tmp0);
        cpu->tmp0.r = cpu->addrspace[cpu->pc.r + 1].r;
        break;
    }

    switch(addrmode & 0x00F0){
        case 0x0000:
        regvals[1] = AM_NONE;
        reg1 = &(cpu->tmp1);
        break;

        case 0x0010:
        regvals[1] = AM_REG;
        reg1 = &(cpu->reg[(cpu->addrspace[cpu->pc.r + 2].r & 0xF)]);
        break;

        case 0x0020:
        regvals[1] = AM_VAL;
        reg1 = &(cpu->tmp1);
        cpu->tmp1.r = cpu->addrspace[cpu->pc.r + 2].r;
        break;
    }

    switch(addrmode & 0xF){
        case 0x0000:
        regvals[2] = AM_NONE;
        reg2 = &(cpu->tmp2);
        break;

        case 0x0001:
        regvals[2] = AM_REG;
        reg2 = &(cpu->reg[(cpu->addrspace[cpu->pc.r + 3].r & 0xF)]);
        break;

        case 0x0002:
        regvals[2] = AM_VAL;
        reg2 = &(cpu->tmp2);
        cpu->tmp2.r = cpu->addrspace[cpu->pc.r + 23].r;
        break;
    }

    switch(opcode){
        case 0x0: //ADD add r0 to r1 and write to r2
            (*reg2).r = (*reg0).r + (*reg1).r;
        break;
        case 0x1: //MOV move r0 to r1
            (*reg1).r = (*reg0).r;
        break;
        case 0x2://HLT just halt
            cpu->halt = 1;
        break;
        case 0x3: //JMP jmp r0
            cpu->pc.r = (*reg0).r;
        break;
        case 0x4: //JOZ jump r1 if r0 is zero
            if((*reg0).r == 0){
                cpu->pc.r = (*reg1).r;
            }
        break;
        case 0x5: //OUT
            printf("%c", (*reg0).r);
        break;
    }

    cpu->pc.r+=increase;
    
}