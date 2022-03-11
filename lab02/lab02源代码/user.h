#pragma once
//状态
#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2
//字节处理宏定义
#define check_byte(byte,  bit) (((byte) & (1 << ((bit)))) != 0)//检查byte的第bit位为1还是0
#define set_byte1(byte, bit) ((byte) |= (1<<bit))//设置byte的第bit位为1
#define set_byte0(byte, bit) ((byte) &= (~(1 << bit)))//设置byte的第bit位为0
#define dihead_byte(res, byte) ((res = (byte>>4)&0xF))//获取byte的前四位即前半字节
#define ditail_byte(res, byte) ((res = byte&0xF))//获取byte的后四位即后半字节
#define sethead_byte(byte, data) ((byte = (data<<4)&0xF0))//将byte的前半字节置为data
#define settail_byte(byte, data) ((byte = (byte|0xF)&(data|0xF0)))//将byte的后半字节置位data
// #define num_0 "0000"
// #define num_1 "0001"
// #define num_2 "0010"
// #define num_3 "0011"
// #define num_4 "0100"
// #define num_5 "0101"
// #define num_6 "0110"
// #define num_7 "0111"
// #define num_8 "1000"
// #define num_9 "1001"
// #define num_10 "1010"
// #define num_11 "1011"
// #define num_12 "1100"
// #define num_13 "1101"
// #define num_14 "1110"
// #define num_15 "1111"
#include <iostream>
#include <ostream>
#include <sstream>
#include <fstream>
#include <string>
#include <string.h>
#include <cmath>

using namespace std;
typedef int Status;

const string num_c[16]={
            "0000",
            "0001",
            "0010",
            "0011",
            "0100",
            "0101",
            "0110",
            "0111",
            "1000",
            "1001",
            "1010",
            "1011",
            "1100",
            "1101",
            "1110",
            "1111",
};

inline void printline(){
    cout<<"__________________________"<< endl;
}

inline string numtostr(int i,int length){
    string str;
    int place = 4-length;
    str=num_c[i];
    str.erase(0,place);
    return str;
}
