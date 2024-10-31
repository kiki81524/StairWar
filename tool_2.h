#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <ctime> // time_t, difftime, time
#include <Windows.h>
#include <list>
using namespace std;

#define UP VK_UP // 輸入常數名VK_UP或者輸入value值0x26都行
#define DOWN VK_DOWN // VK_DOWN/0x28
#define LEFT VK_LEFT // VK_LEFT/0x25
#define RIGHT VK_RIGHT // VK_RIGHT/0x27
#define JUMP VK_SPACE
#define KEY_BOARD_SPEED 1
#define X_lRANGE 3
#define X_rRANGE 143
#define Y_uRANGE 3 // 到頂不知道為什麼，print會有殘影->因為你第一行print了別的數字
#define Y_dRANGE 38
#define UPDATE_INTERVAL 25
#define GRAVITY 0.16 // 定義跳躍用的重力加速度大小
#define VELOCITY 2.1 // 跳躍的初速度(v_0) // v_0 = g*t可以算到最頂端的所需的時間->v_0*t-g*t*t = 0
#define STAIR_LEN 10



// 不做函數多載了，統一都是給x,y座標
void locate(double x, double y) {
    COORD position = {.X = (short)x, .Y = (short)y}; // COORD結構的兩個成員X和Y都是short型態
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
public:
    // 統一我設計的物件的資訊都用x, y傳遞，除非遇到必須要其他型態(例如COORD)的情況
    double x, y;
    double speed; // speed設成整數，因為你的任何移動都只能是整數操作，行跟列的定位沒有在跟你小數的啦
    double velocity;
    // 判斷是否觸地
    bool grounded;
    // 都有了move的移動"前"判斷，就不需要is_out來移動"後"判斷了
    // bool is_out() {
    //     // 這邊設定的5和3是考慮了遊戲角色的形狀做的因應，變動人物印出風格的話需要改這些參數
    //     if ((x+5 > X_rRANGE || x < X_lRANGE) || (y+3 > Y_dRANGE || y < Y_uRANGE)) return true;
    // }

    // 建構子
    character(): x(0), y(0), speed(1), velocity(0), grounded(true) {}
    character(double X, double Y, double SPEED = 1, double _VELOCITY = 0, bool GROUNDED = true): x(X), y(Y), speed(SPEED), velocity(_VELOCITY), grounded(GROUNDED) {}

    // 處理顯像的相關函數
    void clean() {
        locate(x-2, y-2);//(x+1, y); // x細部微調避免刪掉不是它的一部分的東西
        cout << " ";
        locate(x-3, y-1);//(x, y+1);
        cout << "    ";
        locate(x-2, y);//(x+1, y+2); // x細部微調避免刪掉不是它的一部分的東西
        cout << "  ";
    }
    void print() {
        // 雖然有在下方move的邏輯中限制人物的範圍，但下方邏輯限制的是第一個點的位置，其他身體部件還是會超出界線
        // 所以要改一下print/clean定位的邏輯(相對位置修改)
        locate(x-2, y-2);//(x, y); 
        cout << "O";
        locate(x-3, y-1);//(x, y+1); 
        cout << "/||\\";
        locate(x-2, y);//(x, y+2); 
        cout << "/\\";
    }
    // 嘗試加入jump的支援
    void move(){//time_t &start_time, time_t now_time) { // bool can_jump
        short up = 0, down = 0, left = 0, right = 0, jump = 0;
        // 其實以下if只會被進去1次，所以他們不是1就是0，我準備之後改成bool值
        if (GetAsyncKeyState(UP)) up++; // GetAsyncKeyState
        if (GetAsyncKeyState(DOWN)) down++;
        if (GetAsyncKeyState(LEFT)) left++;
        if (GetAsyncKeyState(RIGHT)) right++;
        if (GetAsyncKeyState(JUMP)) jump++; // 要可以跳才能跳，暫時先放一個佔位TRUE
        // Sleep(100);
        
        Sleep(UPDATE_INTERVAL); // 如果不加Sleep，按一下鍵會直接跑到邊界去，Sleep數值越小，移動速度越快
        clean();
        if (( up | down | left | right)!=0) { //
            // locate(0,0);
            // cout << up << " " << down << " " << left << " " << right;
            // clean();
            if (x + right*speed > X_rRANGE) x = X_rRANGE;
            else if (x - left*speed < X_lRANGE) x = X_lRANGE;
            else x = x + right*speed - left*speed;

            if (y + down*speed > Y_dRANGE) y= Y_dRANGE;
            else if (y - up*speed < Y_uRANGE) y = Y_uRANGE;
            else y = y + down*speed - up*speed;
        }
        if (jump!=0) {
            increase_velocity();
        }
        // 感覺不可以用微小位移相加會是總位移量的算法，因為這邊涉及取整，這種估算法會非常不准
        // 所以我決定要將x,y改成double了，即使行列只會有整數，但數學計算的時候，還是會涉及小數，最後成像再換成整數
        update_status();
        locate(x, y);
        // Sleep(100);
        print();
    }
    void increase_velocity() {
        // bug點: 如果在拋物線頂端，速度也是0，這樣如果你剛好按下跳躍鍵，就會變成踏空跳躍(如果將加速度或速度設成非倍數關係或許可以規避)
        // 後來對遊戲人物狀態新增了是否在地面的屬性
        // 在地面上，則可以跳(給向上速度)
        // 負值才是向上
        if (grounded) {
            velocity = -VELOCITY;
            grounded = false;
        }
    }
    // update_status專門用來更新垂直方向的位置/速度的變化
    void update_status() {
        // 速度更新(算入加速度作用)
        if (!grounded) {
            velocity += GRAVITY;
            if (y + velocity > Y_dRANGE) {
                y= Y_dRANGE;
                // grounded = true; 下面有if做了，不用多此一舉
                velocity = 0;
            }
            else if (y + velocity < Y_uRANGE) {
                y = Y_uRANGE;
                // 撞到天花板，作用力等於反作用力，加速度化成速度應該沒毛病
                velocity = -velocity;
            }
            // 位移更新(算入速度作用)
            else y = y + velocity;
        }
        if (y == Y_dRANGE) grounded=true;
        // else grounded==false;
    }

};

class stair {
private:
    static int cnt; // 計算場上stair的數量，初值不可in-class initialization，static也不允許不給初值
public:
    // 雖然隨機分配給x, y的會是整數，但為了後續位移的操作，還是設成double
    double x, y;

    stair() {
        time_t random_seed;
        cnt++;
        srand(time(&random_seed) + cnt*7);
        x = (rand() % (X_rRANGE-X_lRANGE)) + X_lRANGE; // 如果stair超出畫面就不要print出來
        y = (rand() % (Y_dRANGE-Y_uRANGE)) + Y_uRANGE;
        print();
    }
    ~stair() {
        clean();
        cout << "stairs destructor!\n";
        Sleep(100);
        cnt--;
    }
    void clean() {
        for (int i=0;i<STAIR_LEN;i++) {
            if ((x+i>X_lRANGE) || (x+i<X_rRANGE)) {
                locate(x+i, y);
                cout << " ";
            }
        }
    }
    void print() {
        for (int i=0;i<STAIR_LEN;i++) {
            if ((x+i>X_lRANGE) || (x+i<X_rRANGE)) {
                locate(x+i, y);
                cout << "H"; // █
            }
        }
    }
    // void move() {

    // }

};
// 靜態類別成員的初始化只能在外部進行
int stair::cnt = 0;

// void pr(stair s) {
//     cout << s.x << " " << s.y;
// }

void character_stair_interaction(list<stair> &STAIRS, character &person) {
    for (auto s=STAIRS.begin();s!=STAIRS.end();s++) {
        // 如果不將s->和person.y取整後做比較，就很難符合剛好差1的情況，因為小數點不同的機會佔絕大多數 // (int(s->y)-int(person.y)==1)
        if ((s->x <= person.x && person.x <= s->x+STAIR_LEN) && (!person.grounded)) {
            // (person.y+person.velocity && (int(s->y)-int(person.y)<=10) && (int(s->y)-int(person.y)>=1)) ||

            // if ((person.velocity>0) && (int(s->y)-int(person.y+person.velocity)<0) && (int(s->y)-int(person.y)>=1)) { //(s->y - person.y > 0.5 && s->y - person.y < 1.8) && 
            //     person.grounded = true;
            // }
            // else if (int(s->y)-int(person.y)==1) person.grounded = true;
            if ((int(s->y)-int(person.y)>=1) && s->y - person.y < VELOCITY) {
                person.grounded = true;
                person.velocity = 0;
                person.clean();
                person.y = s->y-1;
                locate(person.x, person.y);
                person.print();
            }
        }
        (*s).print();
        // cout << (*s).x;
    }
}

