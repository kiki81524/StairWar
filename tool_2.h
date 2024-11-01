#pragma once // 避免重複被插入引用(call)它的cpp檔
#include <iostream>
#include <cstdlib>
#include <ctime> // time_t, difftime, time
#include <cmath>
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
#define X_rRANGE 138
#define Y_uRANGE 4 // 到頂不知道為什麼，print會有殘影->因為你第一行print了別的數字
#define Y_dRANGE 44
#define UPDATE_INTERVAL 25
#define GRAVITY 0.16 // 定義跳躍用的重力加速度大小
#define VELOCITY 2.1 // 跳躍的初速度(v_0) // v_0 = g*t可以算到最頂端的所需的時間->v_0*t-g*t*t = 0
#define STAIR_LEN 10
#define HEALTH 10
#define HARMFUL 20
#define BULLET_SPEED 0.35
#define COOL_TIME 2 // 子彈發射的冷卻期
#define EDGE_ADJUST 5
#define SCROLL_THRESHOLD (Y_dRANGE-((Y_dRANGE-Y_uRANGE)/2))
#define SCROLL_SPEED 0.1
#define PROBABILITY 5 //機率會是 (3/PROBABILITY)
int scroll_count = 0; // 紀錄捲動數量，用來給子彈和決定何時不畫地板
bool show_floor = true;

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
	// 想要設定視窗為最大，但似乎沒有作用
    // ShowWindow(GetForegroundWindow(),SW_MAXIMIZE);

    // set console title設定視窗的名字顯示
    // string title = "Kiki123";
	// SetConsoleTitle((LPCTSTR)title.c_str()); 
    // cout << "Initialed";
	const CONSOLE_CURSOR_INFO setting = {.dwSize = 1, .bVisible = FALSE};
    // SetConsoleCursorInfo: 設定指定主控台畫面緩衝區之游標的大小和可見性。
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &setting);
}

// 檢查位置有沒有在遊戲畫面範圍內
bool is_in(double x, double y, int x_rRange = X_rRANGE, int y_dRange = Y_dRANGE, int x_lRange = X_lRANGE, int y_uRange = Y_uRANGE){
    if ((int(x) >= x_lRange && int(x) <= x_rRange) && (int(y) >= y_uRange && int(y) <= y_dRange)) return true;
    else return false;
}

class character {
public:
    // 統一我設計的物件的資訊都用x, y傳遞，除非遇到必須要其他型態(例如COORD)的情況
    double x, y;
    double speed; // speed設成整數，因為你的任何移動都只能是整數操作，行跟列的定位沒有在跟你小數的啦
    double velocity;
    int health;
    // 判斷是否觸地
    bool grounded;

    // 建構子
    character(): x(0), y(0), speed(1), velocity(0), grounded(true), health(HEALTH) {}
    character(double X, double Y, double SPEED = 1, double _VELOCITY = 0, bool GROUNDED = true): x(X), y(Y), speed(SPEED), velocity(_VELOCITY), grounded(GROUNDED), health(HEALTH) {}

    // 處理顯像的相關函數
    void clean() {
        locate(x-1, y-2);//(x+1, y); // x細部微調避免刪掉不是它的一部分的東西
        cout << " ";
        locate(x-2, y-1);//(x, y+1);
        cout << "    ";
        locate(x-1, y);//(x+1, y+2); // x細部微調避免刪掉不是它的一部分的東西
        cout << "  ";
    }
    void print() {
        // 雖然有在下方move的邏輯中限制人物的範圍，但下方邏輯限制的是第一個點的位置，其他身體部件還是會超出界線
        // 所以要改一下print/clean定位的邏輯(相對位置修改)
        locate(x-1, y-2);//(x, y); 
        cout << "O";
        locate(x-2, y-1);//(x, y+1); 
        cout << "/||\\";
        locate(x-1, y);//(x, y+2); 
        cout << "/\\";
    }
    // 嘗試加入jump的支援
    void move(){
        short up = 0, down = 0, left = 0, right = 0, jump = 0;
        // 其實以下if只會被進去1次，所以他們不是1就是0，我準備之後改成bool值
        if (GetAsyncKeyState(UP)) up++; // GetAsyncKeyState
        if (GetAsyncKeyState(DOWN)) down++;
        if (GetAsyncKeyState(LEFT)) left++;
        if (GetAsyncKeyState(RIGHT)) right++;
        if (GetAsyncKeyState(JUMP)) jump++; // 要可以跳才能跳，暫時先放一個佔位TRUE
        
        Sleep(UPDATE_INTERVAL); // 如果不加Sleep，按一下鍵會直接跑到邊界去，Sleep數值越小，移動速度越快
        clean();
        if (( up | down | left | right)!=0) { 
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
        // print();
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
        if (y == Y_dRANGE) grounded = true;
        // else grounded==false;
    }

};

class stair {
private:
    static int cnt; // 計算場上stair的數量，初值不可in-class initialization，static也不允許不給初值
public:
    // 雖然隨機分配給x, y的會是整數，但為了後續位移的操作，還是設成double
    double x, y;

    // 已知bug: 隨機生成的stair很近的話，且又有敵人在stair上，會看到很詭異的敵人穿牆運動
    stair() {
        time_t random_seed;
        cnt++;
        srand(time(&random_seed) + cnt*97);
        x = (rand() % (X_rRANGE-X_lRANGE-EDGE_ADJUST)) + X_lRANGE + EDGE_ADJUST; // 如果stair超出畫面就不要print出來?
        y = (rand() % (Y_dRANGE-Y_uRANGE-EDGE_ADJUST)) + Y_uRANGE + EDGE_ADJUST; // 有些stair會生成在邊緣，enermy站上去會被分屍
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
                locate(x+i, y); // 沒有做is_in判斷
                cout << " ";
            }
        }
    }
    void print() {
        for (int i=0;i<STAIR_LEN;i++) {
            if ((x+i>X_lRANGE) || (x+i<X_rRANGE)) {
                if (is_in(x+i,y)) {
                    locate(x+i, y);
                    cout << "H"; // █
                }
            }
        }
    }
    // void move() {

    // }
};
// 靜態類別成員的初始化只能在外部進行
int stair::cnt = 0;

// 敵人class，分為最下層的敵人(應該只有新手村時才看得到)，以及樓梯上負責巡視的敵人(暫時不設定武器給他們)
class enermy {
private:
    static int cnt;
public:
    double x, y, speed;
    double left, right;
    int harmful;
    enermy() { // 最下層的敵人，他的移動範圍和stair上的人不同
        time_t random_seed;
        cnt++;
        srand(time(&random_seed));
        x = (rand() % (X_rRANGE-X_lRANGE)) + X_lRANGE;
        y = Y_dRANGE;
        speed = (rand() % 10)*0.1; //(rand() % 3) + 
        left = X_lRANGE; right = X_rRANGE;
        harmful = HARMFUL;
        print();
    }
    enermy(stair &s) { // stair上的敵人，所以要給他stair的資訊，這樣實作起來比較簡單
        time_t random_seed;
        cnt++;
        srand(time(&random_seed) + cnt*97);
        x = s.x + (rand() % STAIR_LEN);
        y = s.y - 1; // 站在stair上所以y要減1
        speed = (rand() % 5)*0.1; // 慢一點
        left = s.x; right = s.x + STAIR_LEN;
        harmful = HARMFUL;
        print();
    }
    ~enermy() {
        clean();
        cnt--;
    }
    // 照抄遊戲人物的外觀(所以之後要以顏色區分)
    void clean() { // 沒有做is_in判斷，先暫時不改(懶)
        locate(x-1, y-2);
        cout << " ";
        locate(x-2, y-1);
        cout << "    ";
        locate(x-1, y);
        cout << "  ";
    }
    void print() {
        if (is_in(x-1,y-2)) {
            locate(x-1, y-2);
            cout << "O";
        }
        if (is_in(x-2,y-1)) {
            locate(x-2, y-1);
            cout << "/";
        }
        if (is_in(x-1,y-1)) {
            locate(x-1, y-1);
            cout << "|";
        }
        if (is_in(x,y-1)) {
            locate(x, y-1);
            cout << "|";
        }
        if (is_in(x+1,y-1)) {
            locate(x+1, y-1);
            cout << "\\";
        }
        if (is_in(x-1,y)) {
            locate(x-1, y);
            cout << "/";
        }
        if (is_in(x,y)) {
            locate(x, y);
            cout << "\\";
        }
    }
    void move() {
        clean();
        if (x <= left) {
            x = left;
            speed = -speed; // (換方向走)
        }
        if (x >= right) {
            x = right;
            speed = -speed; // (換方向走)
        }
        x = x + speed;
        print();
    }
};
int enermy::cnt = 0;


class bullet { // 子彈重複利用，射死敵人後回歸子彈匣就不用一直new/delete
public:
    double x,y,speed;
    bool direction; // 左: false/右: true;
    int cool_time;
    bullet(): speed(BULLET_SPEED), cool_time(0) {} // 先不指派位置及方向，等遊戲人物發射時再決定
    void clean() {
        locate(x, y);
        cout << " ";
    }
    void print() {
        locate(x, y);
        cout << "o";
    }
    void move() {
        clean();
        // 不在範圍內的子但我們只是讓它不顯現(假裝沒有在場上)
        if (direction && is_in(x,y)) { // 右邊 // x <= X_rRANGE
            x += speed;
            print();
        }
        if (!direction && is_in(x,y)) { // 左邊 // x >= X_lRANGE
            x -= speed;
            print();
        }
    }
};

// 處理遊戲人物跳到stair上的"腳踏實地"狀態
// 放在stair list中的stair物件才有觸地且不會被清掉螢幕顯示的效果
void character_stair_interaction(list<stair*> &STAIRS, character &person) {
    for (auto it=STAIRS.begin();it!=STAIRS.end();it++) {
        auto s = *it;
        if ((s->x <= person.x && person.x <= s->x+STAIR_LEN) && (!person.grounded)) {
            // 如果不將s->和person.y取整後做比較，就很難符合剛好差1的情況，因為小數點不同的機會佔絕大多數
            // 當人物在移動時，有時候速度會太快，所以要提早處理他的grounded性質
            // 已知bug: 當人物站在stair上且頭頂距離天花板只有1格空間時，無法起跳，可能是只要起跳就會馬上被這邊還原成站在stair上的位置
            // 已知bug: 剛好跳到頂端時是在別的stair上，會直接穿過那個stair掉回去
            if ((int(s->y)-int(person.y)>=1) && s->y - person.y < VELOCITY) { // VELOCITYperson.velocity
                person.grounded = true;
                person.velocity = 0;
                person.clean();
                person.y = s->y-1;
                // 下面這兩行必須寫在這裡，不然人物在stair上彈跳的效果會變成踏在空氣上彈跳
                locate(person.x, person.y);
                person.print();
                if (person.health<HEALTH) person.health++;
            }
        }
        // s->print();
    }
}

// 處理人物遇到敵人被扣血的狀況，harmful值是敵人傷害你一次需要回復攻擊力的時間，越高表示越不具傷害性
void character_enermy_interaction(list<enermy*> &ENERMY, character &person) {
    locate(0,0);
    cout << ENERMY.size();
    for (auto p : ENERMY) {
        if ((abs(p->x-person.x)<=3) && (abs(p->y-person.y)<=3) && p->harmful == HARMFUL) {
            person.health--;
            p->harmful = 0;
        }
        if (p->harmful<HARMFUL) p->harmful++;
        // p->move();
    }
}

// 已知bug: 發射子彈鍵設成a和s，輸入法預設是中文的情況下，子彈後方會有注音符號的殘影閃爍
void shoot(list<bullet*> &available, list<bullet*> &busy, character &person) {
    // while (1)太快，你這邊會一次射出好多顆子彈，要想辦法限制瞬發數量
    if (GetAsyncKeyState(0x41) || GetAsyncKeyState(0x53)) { // A / S
        if (!available.empty()) {
            bullet* p = available.front();
            if (p->cool_time==COOL_TIME) {
                p->cool_time = 0;
                available.pop_front();
                if (GetAsyncKeyState(0x53)) { // 右邊發子彈
                    p->x = person.x+2;
                    p->y = person.y-1;
                    p->direction = true;
                }
                else {
                    p->x = person.x-2; // 雖然理論上位置是person.x-3才對，但視覺效果差了一些，用person.x-2比較好
                    p->y = person.y-1;
                    p->direction = false;
                }
                busy.push_back(p);
            }
            else {
                p->cool_time++;
            }
        }
    }
}

// 處理敵人被子彈打死的情況
// 注意你設定的射出去的子彈是被歸類在不能再發射的子彈的list中，小心勿放反
void bullet_enermy_interaction(list<enermy*> &living, list<enermy*> &dead, list<bullet*> &active, list<bullet*> &rest) {
    // 使用iter_next避免for loop中erase迭代器的bug
    for (auto it1=living.begin(), it1_next=it1;it1!=living.end();it1=it1_next) { // 如果變數取跟類別一樣的名字可以嗎?
        it1_next++;
        for (auto it2=active.begin(), it2_next=it2;it2!=active.end();it2=it2_next) { 
            it2_next++;
            if  ((int((*it1)->x) - int((*it2)->x) == 1 && int((*it1)->y) - int((*it2)->y) == 2) || ((*it1)->x - (*it2)->x <= 2 && (*it2)->x - (*it1)->x <= 1 && int((*it1)->y) - int((*it2)->y) == 1) || ((*it1)->x - (*it2)->x <= 1 && (*it2)->x - (*it1)->x <= 0 && int((*it1)->y) == int((*it2)->y))) {
                (*it1)->clean(); // 或許安排成敵人閃爍會比較有感
                (*it2)->clean();
                dead.push_back((*it1)); // 注意，待修改，如果當前已經沒有一樓的蹤影，一樓的守衛或許要永久性地刪除
                rest.push_back((*it2));
                living.erase(it1);
                active.erase(it2);
                // cout << "erase\n";
            }
        }
    }
}

void clean_screen(int x_rRange = X_rRANGE, int y_dRange = Y_dRANGE, int x_lRange = X_lRANGE, int y_uRange = Y_uRANGE) {
    for (int i=x_lRange;i<=x_rRange;i++) {
        for (int j=y_uRange;j<=y_dRange;j++) {
            locate(i,j);
            cout << " ";
        }
    }
}

// 印出遊戲戰場範圍 (跟clean_screen並不是完全配套，這邊畫的外框比clean_screen的範圍還要外面)
void print_edge(int x_rRange = X_rRANGE, int y_dRange = Y_dRANGE, int x_lRange = X_lRANGE, int y_uRange = Y_uRANGE){
    for (int i=x_lRange-3;i<=x_rRange+2;i++) {
        locate(i,y_uRange-3);
        cout << "=";
        locate(i,y_dRange+1);
        // 如果往上捲動到一定程度，就沒有地板了
        if (show_floor) cout << "=";
        else cout << ".";
    }
    for (int i=y_uRange-3;i<=y_dRange+1;i++) {
        locate(x_lRange-3,i);
        cout << "|";
        locate(x_rRange+2,i);
        cout << "|";
    }
}

// 實作出捲動遊戲畫面的效果(其實就是相對運動，將所有物件的y座標增加(增加就是往下移))
void scroll_screen(list<enermy*> &living, list<bullet*> &active, list<stair*> &STAIR, character &person){
    // 如果遊戲人物他並非處在跳躍狀態，且他所在的高度達到了某個閾值，那麼就捲動畫面
    if (person.velocity==0 && person.y < SCROLL_THRESHOLD) {
        scroll_count++;
        if (scroll_count > 15) show_floor = false;
        person.clean();
        person.y += SCROLL_SPEED;
        person.print();
        for (enermy* e : living) {
            e->clean();
            e->y += SCROLL_SPEED;
            e->move();
            // e->print();
        }
        for (stair* s : STAIR) {
            s->clean();
            s->y += SCROLL_SPEED;
            s->print();
        }
        for (bullet* b : active) {
            b->clean();
            b->y += SCROLL_SPEED;
            b->move();
            // b->print();
        }
    }
    else {
        person.print();
        for (enermy* e : living) {
            e->move();
            // e->print();
        }
        for (stair* s : STAIR) {
            s->print();
        }
        for (bullet* b : active) {
            b->move();
            // b->print();
        }
    }
}


void Game_Start() {
    character player(X_lRANGE+2,Y_dRANGE);

    // 建立樓梯物件
    list<stair*> stairs, block; // stairs是在遊戲介面中的stair，block則是不在遊戲介面中的stair(待命中)
    list<enermy*> enermies, dead; // enermies: 遊戲介面中/ dead: 待命中
    list<bullet*> bullet_in_gun, bullet_in_field; //  bullet_in_field: 遊戲介面中/ bullet_in_gun: 待命中
    // 以下尚未整理
    enermy* first_floor = new enermy();
    enermies.push_back(first_floor);
    for (int i=0;i<10;i++) {
        time_t random_seed;
        srand(time(&random_seed));
        stair* p = new stair();
        if (rand() % PROBABILITY < 3) {
            enermy* e = new enermy(*p);
            enermies.push_back(e);
        }
        // 可以在生成stair時順便決定要不要生成敵人，這樣可以隨機分配部分stair上有敵人
        stairs.push_back(p);
    }
    for (int i=0;i<30;i++) {
        bullet* p = new bullet;
        bullet_in_gun.push_back(p);
    }
    player.print();
    Sleep(2000);
    clean_screen(150,50,0,0);
    while (1) {
        Initialize();
        print_edge();
        if (GetAsyncKeyState(VK_ESCAPE)) break;
        
        // wang.print();
        // chang.move();
        // chung.move();
        player.move();
        scroll_screen(enermies,bullet_in_field,stairs,player);
        character_stair_interaction(stairs,player);
        character_enermy_interaction(enermies,player);
        shoot(bullet_in_gun,bullet_in_field,player);
        // bullet_move(b);
        bullet_enermy_interaction(enermies,dead,bullet_in_field,bullet_in_gun);
        locate(X_rRANGE+6, Y_uRANGE+10);
        cout << "blood: " << player.health;
        // for (int i=0;i<wang.health;i++) {
        //     cout << "|";
        // }
    }
    // 銷毀所有的樓梯物件
    for (auto p : stairs) {
        delete p;
    }
}



// to do list:
// 改變顏色
// 一些其他美觀和guide、血條、積分等等的設定
// 爬上樓要捲動畫面
// 捲動上去的話可以獲得子彈數量，上限暫定10
// O 子彈可以攻擊敵人
// 殺掉敵人可以獲得積分
// O 如何隨機分配敵人在部分stair上
// O 敵人撞到你，你要扣血
// 被敵人撞到，你要閃紅表示受傷，血條也要閃
// 殲滅敵人要獲得積分(積分可以幹嘛?我們目標是賺積分還是爬樓?)
// O 殲滅敵人用的子彈
// 我想使用中文字，可能要改編碼?
// X 固定.exe打開的視窗大小
// 為避免變動視窗大小造成螢幕出現殘留的物件影像，要設計flush機制
// 要注意最後有將所有物件解構/delete
// X 希望能設定視窗大小 [放棄!!!]
// O 畫出遊戲戰場範圍
// O 處理超出畫面時的物件clean()，因為如果你不處理的話，螢幕顯示出的字元會溢出到別的行列去，你的畫面會漸漸變得可怕
// 要檢視一遍各個物件的交互作用，尤其是move和print的使用要控管，不然各處都一起用，印出容易有bug
