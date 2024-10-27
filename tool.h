#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <Windows.h>
using namespace std;

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
// 不繼承coordinate，因為我的目的是使用coordinate物件的運算子多載的特性
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

    // 清除物件殘留影像
    void clean() {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position.get_COORD());
        cout << "** **";
    }
    // virtual void print() = 0;
    // virtual void move() = 0;
};

// 以防寫垮先備份
// class object { // 建立一個父類別object，建構2D物件的一般屬性，例如位置資訊、移動資訊
// private:
//     // short x; short y;
//     COORD position;
// public:
//     // constructor:
//     object(){position.X = 0; position.Y = 0;}
//     object(short X, short Y){position.X = X; position.Y = Y;}  // test
//     object(COORD POSITION): position(POSITION) {}
//     // destructor我就不寫了
//     COORD get_position() {return position;}
//     COORD relocate_position(short X, short Y) {position.X = X; position.Y = Y;} // test
//     COORD relocate_position(COORD POSITION) {position = POSITION;}
//     COORD shift_position(short X, short Y) {position.X += X; position.Y += Y;} // test
//     COORD shift_position(COORD POSITION) {position += POSITION;}
//     void clean() {locate(position); cout << " ";}
//     virtual void print() = 0;
//     virtual void move() = 0;
// };

// class character: public object {
// public:
//     // character(): object() {}
//     character(short X, short Y): object(X, Y) {cout << "here";} // object(short X, short Y); 
//     // character(COORD POSITION): object(COORD POSITION) {}
//     virtual void print() {
//         locate(get_position());
//         cout << " O" << endl;
//         cout << "/||\\" << endl;
//         cout << " /\\" << endl;
//     }
//     virtual void move() {
//         sleep(200);
//         clean();
//         locate(get_position());
//         sleep(200);
//         cout << "---";
//         COORD sp;
//         sp.X = 3;
//         sp.Y = 4;
//         sleep(200);
//         locate(shift_position(sp));
//         cout << "---";
//     }
// };


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