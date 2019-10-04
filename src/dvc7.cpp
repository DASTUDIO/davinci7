//
//  main.cpp
//  dvc
//
//  Created by DavidJang on 2019/10/3.
//  Copyright © 2019 da studio. All rights reserved.
//

#include <iostream>
#include <sys/time.h>
using namespace std;

extern "C" int encode(char*,char*);
extern "C" int decode(char*,char*);
extern "C" int crypt(char*,char*,char*);
extern "C" int decrypt(char*,char*,char*);

string _encode(string);
string _decode(string);
string _crypt(string,string);
string _decrypt(string,string);
int _get_seed();
int _un_password(int,int);
string _remove_char(char,string);

int map[10][10]={
    {9,8,7,6,5,4,3,2,1,0},
    {6,8,3,5,4,1,2,0,9,7},
    {3,0,1,9,2,8,7,5,4,6},
    {7,4,5,8,0,9,2,1,6,3},
    {1,7,5,3,2,4,6,8,0,9},
    {2,3,4,7,1,6,5,9,8,0},
    {4,1,2,3,7,6,5,8,0,9},
    {0,8,2,9,5,4,6,3,7,1},
    {5,1,2,9,8,0,7,6,3,4},
    {8,9,2,3,5,6,0,1,4,2}};

int main(int argc, const char * argv[])
{
    string ccc = _crypt("你好吗", "1");
    cout<<ccc<<endl;
    cout<<_decrypt(ccc, "1")<<endl;
}

extern "C" int encode(char* input, char* output)
{
    strcpy(output, (_encode((string)input)).c_str());
    return 0;
}

extern "C" int decode(char* input, char* output)
{
    strcpy(output, (_decode((string)input)).c_str());
    return 0;
}

extern "C" int crypt(char* input, char* output, char* password)
{
    strcpy(output, (_crypt((string)input, (string)password)).c_str());
    return 0;
}

extern "C" int decrypt(char* input, char* output, char* password)
{
    strcpy(output, (_decrypt((string)input, (string)password)).c_str());
    return 0;
}

string _encode(string str)
{
    unsigned long len = str.length();
    
    char bytes[len/3==0?8:8*(len/3+(len%3==0?0:1))+1];
    
    unsigned long i;
    
    for(i=0;i<len;i+=3){                                                                        // 48: byte->ascii num
        bytes[(i/3)*8] = 48 + ((str[i] & 0b11100000)>>5);
        bytes[(i/3)*8+1] = 48 + ((str[i] & 0b00011100)>>2);
        bytes[(i/3)*8+2] = 48 + (((str[i] & 0b00000011)<<1)+((str[i+1] & 0b10000000)>>7));
        bytes[(i/3)*8+3] = 48 + ((str[i+1] & 0b01110000)>>4);
        bytes[(i/3)*8+4] = 48 + ((str[i+1] & 0b00001110)>>1);
        bytes[(i/3)*8+5] = 48 + (((str[i+1] & 0b00000001)<<2)+((str[i+2] & 0b11000000)>>6));
        bytes[(i/3)*8+6] = 48 + ((str[i+2] & 0b00111000)>>3);
        bytes[(i/3)*8+7] = 48 + (str[i+2] & 0b00000111);
    } bytes[(i/3)*8]=0;
    
    return (string)bytes;
}

string _decode(string str)
{
    unsigned long len = str.length();
    
    char bytes[len/8==0?3:3*(len/3+(len%3==0?0:1))+1];
   
    unsigned long i;
    
    for(i=0;i<len;i+=8){
        bytes[(i/8)*3] = ((str[i] - 48) << 5) + ((str[i+1] - 48) << 2) + ((str[i+2] - 48) >> 1);
        bytes[(i/8)*3+1] = ((str[i+2] - 48) << 7) + ((str[i+3] - 48) << 4) + ((str[i+4] - 48) << 1) + ((str[i+5] - 48) >> 2);
        bytes[(i/8)*3+2] = ((str[i+5] -48) << 6) + ((str[i+6] - 48) <<3) + (str[i+7] - 48);
    } bytes[(i/8)*3]='\0';
    
    return (string)bytes;
}

string _crypt(string source,string password)
{
    for(int p=0; p<password.length(); p++){
        
        source = _encode(source);                                                                // 转为dvc码
        
        srand((unsigned)(_get_seed()));                                                         // 重制随机数产生器

        for(int i=0; i<source.length()/4 + (rand()%source.length()/9/5); i++){                  // 加冗余
            srand((unsigned)(_get_seed()));
            char element[2] = {(char)(48 + (rand()%2==0?8:9)),'\0'};
            source.insert(rand()%(source.length()-1),element);
        }
        
        for(int i=0; i<source.length(); i++)                                                    // 凯撒置换
            source[i] = map[(int)(password[p] - 48)][(int)(source[i]-48)] + 48;
    }
    return source;
}

string _decrypt(string source,string password)
{
    for(int p=0; p<password.length(); p++){
        
        for(int i=0; i<source.length(); i++)                                                    // 还原凯撒
            source[i] = (char)(_un_password((int)(source[i] - 48), password[password.length()-p-1] - 48) + 48);
        
        source = _remove_char('8', source);                                                     // 去除冗余
        source = _remove_char('9', source);
        
        source = _decode(source);                                                                // 还原dvc码
    }
    return source;
}

int _un_password(int value,int password)
{
    for(int i=0;i<10;i++)
    {
        if(map[password][i] == value)
            return i;
    }
    return -1;
}

string _remove_char(char c,string source)
{
    while(source.find(c)!=source.npos)
    {
        source = source.erase(source.find(c), 1);
    }
    return source;
}

int _get_seed()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return t.tv_usec;
}
