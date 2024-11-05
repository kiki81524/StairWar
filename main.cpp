#include <iostream>
#include <cstdlib>
#include <Windows.h>
#include "tool_2.h"

using namespace std;

int main() {
    Initialize();
    Sleep(1000);
    welcome_animation();
    while (1) {
        menu();
        char menu_choose = getch();
        if (menu_choose=='Y' || menu_choose=='y') {
            Game_Start();
        }
        else break;
    }
    
    Sleep(1000);
    byebye();
    return 0;
}