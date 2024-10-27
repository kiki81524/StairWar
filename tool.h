#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <Windows.h>
using namespace std;

// 新增一個coordinate class方便處理COORD的位置資訊，使我們能做到讓帶有位置資訊的物件可以使用到一些運算子的功能
class coordinate {
private:
    COORD info;
public:
    // 建構子多載
    coordinate(){
        info.X = 0; info.Y = 0;
    }
    coordinate(short x, short y) {
        info.X = x; info.Y = y;
    }
    coordinate(COORD xy): info(xy) {}


    // 獲得COORD用來與handle溝通(要用COORD)
    COORD get_COORD() {return info;}

    // 用來開放外界更改內容的接口(採用函數多載)
    void assign_coordinate(short x, short y) {
        info.X = x; info.Y = y;
    }
    void assign_coordinate(COORD xy) { // 注意參數放的是COORD不是coordinate
        info = xy;
    }


    // debug用
    void show() {cout<< info.X << " " << info.Y <<endl;}


    // 運算子多載
    coordinate operator+(coordinate &obj_speed) {
        return coordinate(info.X + obj_speed.info.X, info.Y + obj_speed.info.Y);
    }
    coordinate operator-(coordinate &obj_speed) {
        return coordinate(info.X - obj_speed.info.X, info.Y - obj_speed.info.Y);
    }
    coordinate &operator+=(coordinate &obj_speed) {
        this->info.X += obj_speed.info.X;
        this->info.Y += obj_speed.info.Y;
        return *this;
    }
    coordinate &operator-=(coordinate &obj_speed) {
        this->info.X -= obj_speed.info.X;
        this->info.Y -= obj_speed.info.Y;
        return *this;
    }
    coordinate &operator=(coordinate obj_position) { // 參數值不可以加& (**)
        this->info.X = obj_position.info.X;
        this->info.Y = obj_position.info.Y;
        return *this;
    }
    bool operator==(coordinate &obj_position) {
        return (this->info.X == obj_position.info.X) && (this->info.Y == obj_position.info.Y);
    }
};

// 建立一個父類別object，建構2D物件的一般屬性，例如位置資訊、移動資訊
// 不繼承coordinate，因為我的目的是使用coordinate物件的運算子多載的特性，繼承無法繼承運算子多載
class object { 
private:
    // short x; short y;
    coordinate position;
public:
    // constructor:
    object(){}
    object(short X, short Y){
        position.assign_coordinate(X, Y);
    }  // test
    object(COORD POSITION) {
        position.assign_coordinate(POSITION);
    }
    object(coordinate POSITION) {
        position = POSITION;
    }
    // destructor我就不寫了

    // 獲得物件位置
    COORD get_COORD() {return position.get_COORD();}
    coordinate get_position() {return position;}

    // debug用
    void show() {position.show();}

    // 重新指派物件位置
    void relocate_position(short X, short Y) {
        position.assign_coordinate(X, Y);
    } // test
    void relocate_position(COORD POSITION) {
        position.assign_coordinate(POSITION);
    }
    void relocate_position(coordinate POSITION) {
        position = POSITION;
    }
    

    // 預估物件移動後的位置(不改變物件本身位置，想要改變的話你自己前面加指派)
    coordinate calculate_shift_position(short X, short Y) {
        coordinate shift(X, Y);
        return position + shift;
    } // test
    coordinate calculate_shift_position(COORD POSITION) {
        coordinate shift(POSITION);
        return position + shift;
    }
    coordinate calculate_shift_position(coordinate POSITION) {
        return position + POSITION;
    }

    // 跟硬體要位置 (是否要擴充?)
    void locate() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position.get_COORD());
    }

    // 為了要實現跨行print物件，對locate不足的補救
    void down_locate(short y) {
        coordinate down_position = coordinate(0,y);
        down_position += position;
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), down_position.get_COORD());
    }

    // 清除物件殘留影像
    // void clean() {
    //     locate(); // 寫this->locate()或是直接寫locate()都可以
    //     cout << "** **";
    // }

    // 純虛擬函數，定義寫在這，但要使用的話子類別要自己寫
    // 父類別寫成了虛擬函數，沒有給具體定義(實作)的，你忘記在子類別實作編譯器就會提醒你
    virtual void clean() = 0;
    virtual void print() = 0;
    virtual void move() = 0;
};

// 從object類別衍生的子類別，用來建立遊戲人物
class character: public object {
private:
    // 因為遊戲人物需要跨行print出來，暫定共3行，所以我建立了別的2個coordinate座標資訊，會與object物件的position綁定移動(但馬上放棄了~~用手動寫一寫比較快)
    // coordinate body_1, body_2;
public:
    // 子類別的建構子，交給對應的object類別建構子負責處理
    character(): object() {
        // body_1 = calculate_shift_position(0,1);
        // body_2 = calculate_shift_position(0,2);
    }
    character(short X, short Y): object(X, Y) {
        // body_1 = calculate_shift_position(0,1);
        // body_2 = calculate_shift_position(0,2);
    }
    character(COORD POSITION): object(POSITION) {
        // body_1 = calculate_shift_position(0,1);
        // body_2 = calculate_shift_position(0,2);
    }
    character(coordinate POSITION): object(POSITION) {
        // body_1 = calculate_shift_position(0,1);
        // body_2 = calculate_shift_position(0,2);
    }

    // 子類別實作虛擬函數時，可以不加virtual關鍵字
    void print() {
        locate();
        cout << "  O" ;//<< endl;
        down_locate(1);
        cout << "/||\\" ;//<< endl;
        down_locate(2);
        cout << " /\\" ;//<< endl;
    }
    void move() {
        Sleep(200);
        clean();
        locate();
        Sleep(200);
        cout << "---";
        COORD sp;
        sp.X = 3;
        sp.Y = 4;
        Sleep(200);
        locate();
        cout << "---";
    }
};


// void locate(short x, short y) {
//     COORD position; // COORD結構的兩個成員X和Y都是short型態
//     position.X = x; position.Y = y;
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
// }

// void locate(COORD position) { // 函數多載
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
// }
// void locate(COORD position, COORD shift) { // 函數多載
//     position.X = position.X + shift.X;
//     position.Y = position.Y + shift.Y;
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
// }
// void locate(coordinate position) { // 函數多載
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position.get_COORD()); // 將游標locate到你position指定的位置
// }
// void locate(coordinate position, coordinate shift) { // 函數多載
//     SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (position+shift).get_COORD()); // 將游標locate到你position指定的位置
// }