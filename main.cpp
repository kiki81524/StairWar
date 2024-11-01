#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "tool_2.h"

using namespace std;
#define PROBABILITY 5 //機率會是 (3/PROBABILITY)

int main() {
    Initialize();
    // change_size(200,100);
    Sleep(1000);
    character wang(15,25);
    stair s1;
    enermy chung(s1);
    // character lee(50,10);
    // 建立樓梯物件
    list<stair*> stairs;
    list<enermy*> enermies, dead;
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
    list<bullet*> a, b;
    for (int i=0;i<30;i++) {
        bullet* p = new bullet;
        a.push_back(p);
    }
    wang.print();
    Sleep(2000);
    clean_screen(150,50,0,0);
    while (1) {
        Initialize();
        print_edge();
        if (GetAsyncKeyState(VK_ESCAPE)) break;
        
        // wang.print();
        // chang.move();
        // chung.move();
        wang.move();
        scroll_screen(enermies,b,stairs,wang);
        character_stair_interaction(stairs,wang);
        character_enermy_interaction(enermies,wang);
        shoot(a,b,wang);
        // bullet_move(b);
        bullet_enermy_interaction(enermies,dead,b,a);
        locate(X_rRANGE+6, Y_uRANGE+10);
        cout << "blood: " << wang.health;
        // for (int i=0;i<wang.health;i++) {
        //     cout << "|";
        // }
    }
    Sleep(1000);
    cout << "Bye";//✲ ✳ ✴ ✵ ✶ ✷ ✸ ";
    Sleep(1000);
    // 銷毀所有的樓梯物件
    for (auto p : stairs) {
        delete p;
    }
    Sleep(1000);
    
// ==================== 測試coordinate類
    // coordinate obj1(10,20), obj2(30,40), obj3(35,43);
    // coordinate obj4;
    // obj3=obj1;
    // obj1.show();
    // obj1+=obj2;
    // obj1.show();
    // (obj2+obj3).show();
    // obj2.show();
    // obj4 = obj2+obj3;
    // obj4.show();
    // if (obj1==(obj4)) cout << "true" << endl;
    // else cout << "false" << endl;
    // obj1-=obj2;
    // cout << endl;
    // obj1.show();
// ==================== 測試object類
    // object ob1(10,20), ob2(0,2);
    // coordinate sh(30,12);
    // ob1.relocate_position(3,2);
    // ob2 = ob2.calculate_shift_position(sh);
    // // ob1.show();
    // // ob2.show();
    // ob1.clean();
    // ob2.clean();
// ====================
    // system("pause");
    return 0;
}