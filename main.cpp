#include "functions.h"


machine *virtmachine;

void endrun(){}
int main(int argc, char* argv[]){



    string data;
    ifstream file(argv[1]);
    string line ;
    while( getline( file, line ) )
    {
        data += line;
        data += "\n";

    }
    //||============||
    //||INIT MACHINE||
    //||============||
    virtmachine = (machine*)malloc(sizeof(machine));
    for(int i = 0; i < 16; i++){
        virtmachine->reg[i].r = 0x0;
    }
    virtmachine->pc.r = 0x0;
    virtmachine->sp.r = 0xFFFF;
    //virtmachine->addrspace = (reg32bit*) malloc(sizeof(reg32bit)*0xFFFFFFFF);
    
    virtmachine->addrspace = compile("out $30\nhlt\n");;

    while(1){
        run(virtmachine);
        //printf("=============\n");
        if(virtmachine->halt != 0)  break;

        //printf("%d",virtmachine->reg[0].r);
        
    }
    endrun();
    return 0;
}
