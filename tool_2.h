#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <Windows.h>
using namespace std;

#define UP VK_UP // 輸入常數名VK_UP或者輸入value值0x26都行
#define DOWN VK_DOWN // VK_DOWN/0x28
#define LEFT VK_LEFT // VK_LEFT/0x25
#define RIGHT VK_RIGHT // VK_RIGHT/0x27
#define JUMP VK_SPACE
#define KEY_BOARD_SPEED 1
#define X_lRANGE 0
#define X_rRANGE 140
#define Y_uRANGE 0 // 到頂不知道為什麼，print會有殘影->因為你第一行print了別的數字
#define Y_dRANGE 35
#define UPDATE_INTERVAL 25

// 不做函數多載了，統一都是給x,y座標
void locate(short x, short y) {
    COORD position = {.X = x, .Y = y}; // COORD結構的兩個成員X和Y都是short型態
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
}

// 待修改，有點希望將它們拆成不同函數
// 隱藏游標閃爍(否則會出現游標不停在好幾個位置閃爍的視覺效果)
// win 10終端機上面不知道為什麼就是無法隱藏游標-->要在while(1)中每輪都設隱藏
void Initialize() // set console title and hide console cursor
{
	// set console title
    // string title = "Kiki123";
	// SetConsoleTitle((LPCTSTR)title.c_str()); 
    // cout << "Initialed";
	const CONSOLE_CURSOR_INFO setting = {.dwSize = 1, .bVisible = FALSE};
    // SetConsoleCursorInfo: 設定指定主控台畫面緩衝區之游標的大小和可見性。
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &setting);
}

class character {
private:
    // 統一我設計的物件的資訊都用x, y傳遞，除非遇到必須要其他型態(例如COORD)的情況
    short x, y;
    short speed; // speed設成整數，因為你的任何移動都只能是整數操作，行跟列的定位沒有在跟你小數的啦
    bool is_out() {
        // 這邊設定的5和3是考慮了遊戲角色的形狀做的因應，變動人物印出風格的話需要改這些參數
        if ((x+5 > X_rRANGE || x < X_lRANGE) || (y+3 > Y_dRANGE || y < Y_uRANGE)) return true;
    }
public:
    // 建構子
    character(): x(0), y(0), speed(1) {}
    character(short X, short Y, short SPEED = 1): x(X), y(Y), speed(SPEED) {}

    // 處理顯像的相關函數
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
    void move() {
        short up = 0, down = 0, left = 0, right = 0;//, jump = 0;
        if (GetAsyncKeyState(UP)) up++; // GetAsyncKeyState
        if (GetAsyncKeyState(DOWN)) down++;
        if (GetAsyncKeyState(LEFT)) left++;
        if (GetAsyncKeyState(RIGHT)) right++;
        // if (GetAsyncKeyState(JUMP)) jump++;
        // Sleep(100);
        
        Sleep(UPDATE_INTERVAL); // 如果不加Sleep，按一下鍵會直接跑到邊界去，Sleep數值越小，移動速度越快
        if (( up | down | left | right )!=0) { //| jump 
            // locate(0,0);
            // cout << up << " " << down << " " << left << " " << right;
            clean();
            if (x + right*speed > X_rRANGE) x = X_rRANGE;
            else if (x - left*speed < X_lRANGE) x = X_lRANGE;
            else x = x + right*speed - left*speed;

            if (y + down*speed > Y_dRANGE) y= Y_dRANGE;
            else if (y - up*speed < Y_uRANGE) y = Y_uRANGE;
            else y = y + down*speed - up*speed;
        }
        locate(x, y);
        // Sleep(100);
        print();
    }
    

};