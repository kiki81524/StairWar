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
    // coordinate(COORD xy) {
    //     info.X = xy.X % X_RANGE;
    //     info.Y = xy.Y % Y_RANGE;
    // }


    // 獲得COORD用來與handle溝通(要用COORD)
    COORD get_COORD() {return info;}

    // 用來開放外界更改內容的接口(採用函數多載)
    void assign_coordinate(short x, short y) {
        info.X = x; info.Y = y;
    }
    void assign_coordinate(COORD xy) { // 注意參數放的是COORD不是coordinate
        info = xy;
        // info.X = xy.X % X_RANGE;
        // info.Y = xy.Y % Y_RANGE;
    }

    // 判斷是否超出打印範圍
    bool is_out() {
        if ((info.X > X_RANGE || info.X < 0) || (info.Y > Y_RANGE || info.Y < 0)) return true;
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
    // short speed;
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
        if (!position.is_out()) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position.get_COORD());
    }

    // 為了方便操作物件的位置，將此函數設成平面任何方向(以x,y調控)皆可移動
    // 暫時沒做多載設定
    coordinate shift_locate(short x, short y) {
        // coordinate down_position = coordinate(x,y);
        // down_position += position;
        coordinate new_position = calculate_shift_position(x,y);
        if (!new_position.is_out()) SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), new_position.get_COORD());
        return new_position;
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
    void clean() {
        locate();
        cout << "    " ;
        // shift_locate(0,1); // x為正值是往右，y為正值是往下
        // cout << "     " ;
        // shift_locate(0,2);
        // cout << "    " ;
    }
    void print() {
        locate();
        cout << " O" ;//<< endl;
        // shift_locate(0,1);
        // cout << "/||\\" ;//<< endl;
        // shift_locate(0,2);
        // cout << " /\\" ;//<< endl;
    }
    // 透過不斷刷新實時位置來體現出移動的效果
    // 而實際(遊戲人物))物件位置的(隨著鍵盤操作的)更動，可以另寫成一個函數，請該函數去判斷及調控
    // 但該函數不可單獨使用，必須在其前搭配clean()使用，所以需要把它們包成move()函數
    // 所有感覺還是直接在move()裡面寫整套就好...-->結論，move()中判斷keyboard status
    void move() {
        short x = 0, y = 0;
        // 只有在確實clean完原位的殘影後，才可以改動此(遊戲人物)物件的位置，否則會有殘影
        if (GetKeyState(UP)) y--; // GetAsyncKeyState
        if (GetKeyState(DOWN)) y++;
        if (GetKeyState(LEFT)) x--;
        if (GetKeyState(RIGHT)) x++;
        if (x!=0 || y!=0) {
            clean();
            x %= 100;
            y %= 30;
            relocate_position(shift_locate(x*KEY_BOARD_SPEED, y*KEY_BOARD_SPEED));
        }
        // Sleep(100);
        print();
    }
};

// 隱藏游標閃爍(否則會出現游標不停在好幾個位置閃爍的視覺效果)
void Initialize() // set console title and hide console cursor
{
	// set console title
	// SetConsoleTitle("Flunk You"); 

	CONSOLE_CURSOR_INFO setting = {1,FALSE};
    // SetConsoleCursorInfo: 設定指定主控台畫面緩衝區之游標的大小和可見性。
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &setting);
}

void locate(short x, short y) {
    COORD position; // COORD結構的兩個成員X和Y都是short型態
    position.X = x; position.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), position); // 將游標locate到你position指定的位置
}

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