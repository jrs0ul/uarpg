/***************************************************************************
 *   Copyright (C) 2008 by jrs0ul                                          *
 *   jrs0ul@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "interpreter.h"
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include "DArray.h"


/*void removeQuotes(char* tmp){
char* pmp=new char[(int)strlen(tmp)-1];
for (int a=1;a<(int)strlen(tmp)-1;a++)
pmp[a-1]=tmp[a];

pmp[(int)strlen(tmp)-2]='\0';
strcpy(tmp,pmp);
delete []pmp;
}*/


//----
Arguments::Arguments(){

}

void Arguments::clear(){
    variables.destroy();
}

void AddOperators(int num, char oper, char *ops,int* nums,int& opc,bool* operate){
    ops[opc]=oper;
    nums[opc]=num;
    opc++;
    *operate=true;
}



//---------------------------------------------
void Arguments::getArguments(const char* cmdline, unsigned index, CVariableArray* vars){


    char tmp[255];

    unsigned char argindex;
    unsigned char i=++index;
    argindex=0;
    unsigned char oldi=0;
    bool value=true;
    bool isvar=false;
    bool operate=false;

    char ops[10];
    int nums[10];
    int opc=0;
    char separ=0;

    while(value){
        oldi=i;
        if (cmdline[i]=='"'){//reiksme?
            i++;
            value=GetValue(cmdline,i,"\"",tmp,&separ);
            //puts(tmp);

            if ((cmdline[i]!='*')&&(cmdline[i]!='/')&&
                (cmdline[i]!='+')&&(cmdline[i]!='-')) {
                    if (operate){
                        operate=false;

                        int result=nums[0];
                        nums[opc]=atoi(tmp);

                        ops[opc-1]=cmdline[i-strlen(tmp)-3];
                        for (int z=0;z<opc;z++){
                            switch (ops[z]){
                             case '+': result+=nums[z+1]; break;
                             case '-':result-=nums[z+1]; break;
                             case '*':result*=nums[z+1]; break;
                             case '/':result/=nums[z+1]; break;
                            }

                        }
                        sprintf(tmp,"%d",result);
                    }
            }else {
                if (!operate)
                    operate=true;
            }

            if (cmdline[i]==',')
                i++;
            else{
                if (cmdline[i]==')')
                    value=false;

         }
            isvar=false;
            //puts("yarr1");
        }
        else{//ne, tai kintamasis
            isvar=true;

            if ((cmdline[i]=='*')||(cmdline[i]=='/')||
                (cmdline[i]=='+')||(cmdline[i]=='-')){

                    ops[opc]=cmdline[i];
                    nums[opc]=atoi(tmp);
                    opc++;
                    operate=true;
                    i++;

            }
            else{
                value=GetValue(cmdline,i,",",tmp,&separ);
                if (value){
                    if ((cmdline[i]=='*')||(cmdline[i]=='/')||
                        (cmdline[i]=='+')||(cmdline[i]=='-')){
                            if (operate)
                                operate=false;

                            int result=nums[0];
                            ops[opc-1]=cmdline[i-strlen(tmp)-3];
                            if (vars->getValue(tmp))
                                nums[opc]=atoi(vars->getValue(tmp));
                            else
                                nums[opc] = 0;
                            for (int z=0;z<opc;z++){
                                switch (ops[z]){
                              case '+': result+=nums[z+1]; break;
                              case '-': result-=nums[z+1]; break;
                              case '*': result*=nums[z+1]; break;
                              case '/': result/=nums[z+1]; break;
                             }
                            }

                            isvar=false;
                            sprintf(tmp,"%d",result);
                    }
                }
                if (!value){
                    i=oldi;

                    value=GetValue(cmdline,i,"+-*/",tmp,&separ);

                    if (value){
                        if (vars->getValue(tmp))
                            AddOperators(atoi(vars->getValue(tmp)),separ,ops,nums,opc,&operate);
                    }
                    else{
                        i=oldi;
                        value=GetValue(cmdline,i,")",tmp,&separ);
                        //puts(tmp);
                        if (value){
                            if (operate){
                                operate=false;
                                int result=nums[0];
                                if (vars->getValue(tmp))
                                    nums[opc]=atoi(vars->getValue(tmp));
                                else
                                    nums[opc] = 0;
                                printf("%d\n",nums[opc]);
                                for (int z=0;z<opc;z++){
                                    switch (ops[z]){
                                                case '+': result+=nums[z+1];break;
                                                case '-': result-=nums[z+1];break;
                                                case '*': result*=nums[z+1];break;
                                                case '/': result/=nums[z+1];break;
                                    }
                                }

                                isvar=false;
                                sprintf(tmp,"%d",result);
                            }
                        }
                    }
                }
            }
            //puts("yarr2");
        }
        //-------
        if ((strcmp(tmp,"")!=0)&&(!operate)){
            //puts(tmp);
            if (isvar){
                if (vars->getValue(tmp))
                    strcpy(tmp,vars->getValue(tmp));
            }

            conststring newarg;
            //variables[argindex]=new char[(int)strlen(tmp)+1];
            strcpy(/*variables[argindex]*/newarg.s,tmp);
            variables.add(newarg);
            argindex++;
        }
        //puts("yarr3");
    }
    //puts("iend");
    //count=argindex;
}


/*
void Arguments::getArguments2(const char* cmdline, unsigned index, CVariableArray* vars){
    char * paramline = 0;
    int paramlen = strlen(cmdline)-index;
    if (paramlen > 0){
        paramline = new char[paramlen];
        memcpy(paramline,&cmline[index],paramlen);

        delete []paramline;
    }
}
*/

//--------------------------------------------------
void GetCommand(const char* cmdline, char* result){

    char tmp[255];
    unsigned  i = 0;
    if (cmdline){
        if (strlen(cmdline) >= 1){
            while (((cmdline[i]!='(')&&(cmdline[i]!='='))&&(i<=strlen(cmdline))){
                tmp[i]=cmdline[i];
                i++;
            }
            tmp[i]='\0';
            strncpy(result, tmp, 255);
        }
    }
}
//-------------------------

bool GetValue(const char* cmdline, unsigned char& index, const char* separators, char* result, char* separator){


    char tmp[100];
    unsigned int i;
    i=index;
    bool found=false;
    int sepcount=(int)strlen(separators);
    while ((!found)&&(i!=strlen(cmdline))){
        for (int a=0;a<sepcount;a++){
            if (cmdline[i]==separators[a]){
                found=true;
                *separator=cmdline[i];
            }
        }
        if (!found){
            tmp[i-index]=cmdline[i];
            i++;
        }
    }
    tmp[i-index]='\0';
    index=++i;
    if (found){
        strcpy(result,tmp);
        return true;
    }
    strcpy(result,"");
    return false;
    /*char *tmp=0;
    char *eilute = new char[strlen(cmdline)+1];
    memcpy(&eilute[0],&cmdline[index],strlen(cmdline)-index);
    tmp=strtok(eilute,separators);
    strcpy(result,tmp);
    *separator=cmdline[strlen(tmp)+index];
    index+=strlen(tmp);
    delete []eilute;
    int i=0;
    while (i<=strlen(separators)-1){
    if (*separator==separators[i])
    return true;
    i++;
    }
    return false;*/

}



