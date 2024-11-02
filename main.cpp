#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "tool_2.h"

using namespace std;

int main() {
    Initialize();
    // change_size(200,100);
    Sleep(1000);
    Game_Start();
    Sleep(1000);
    cout << "Bye";//✲ ✳ ✴ ✵ ✶ ✷ ✸ ";
    Sleep(1000);

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