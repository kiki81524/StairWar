#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <Windows.h>
using namespace std;

#define UP VK_UP // 輸入常數名VK_UP或者輸入value值0x26都行
#define DOWN VK_DOWN // VK_DOWN/0x28
#define LEFT VK_LEFT // VK_LEFT/0x25
#define RIGHT VK_RIGHT // VK_RIGHT/0x27
#define KEY_BOARD_SPEED 1
#define X_RANGE 100
#define Y_RANGE 30

// 不做函數多載了，統一都是給x,y座標
void locate(short x, short y) {
    COORD position = {.X = x, .Y = y}; // COORD結構的兩個成員X和Y都是short型態
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
}

// 待修改，有點希望將它們拆成不同函數
// 隱藏游標閃爍(否則會出現游標不停在好幾個位置閃爍的視覺效果)
void Initialize() // set console title and hide console cursor
{
	// set console title
	// SetConsoleTitle("Flunk You"); 

	CONSOLE_CURSOR_INFO setting = {1,FALSE};
    // SetConsoleCursorInfo: 設定指定主控台畫面緩衝區之游標的大小和可見性。
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &setting);
}

class character {
private:
    // 統一我設計的物件的資訊都用x, y傳遞，除非遇到必須要其他型態(例如COORD)的情況
    short x, y;
    void is_out() {
        // 這邊設定的5和3是考慮了遊戲角色的形狀做的因應，變動人物印出風格的話需要改這些參數
        if ((x+5 > X_RANGE || x < 0) || (y+3 > Y_RANGE || y < 0)) return true;
    }
public:
    // 建構子
    character(): x(0), y(0) {}
    character(short X, short Y): x(X), y(Y) {}
    void clean() {
        locate(x+1, y); // x細部微調避免刪掉不是它的一部分的東西
        cout << " ";
        locate(x, y+1);
        cout << "    ";
        locate(x+1, y+2); // x細部微調避免刪掉不是它的一部分的東西
        cout << "  ";
    }
    void print() {
        locate(x, y);
        cout << " O";
        locate(x, y+1);
        cout << "/||\\";
        locate(x, y+2);
        cout << " /\\";
    }
    

};