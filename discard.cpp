// 此處放置寫過但棄用的code

// #include <winuser.h> // CreateWindowExA

// 想改變console視窗大小，但沒有用
// void change_size(short x, short y) {
//     COORD size = {.X = x, .Y = y};
//     SetConsoleScreenBufferSize(GetStdHandle(STD_OUTPUT_HANDLE), size);
// }


// 都有了move的移動"前"判斷，就不需要is_out來移動"後"判斷了
// bool is_out() {
//     // 這邊設定的5和3是考慮了遊戲角色的形狀做的因應，變動人物印出風格的話需要改這些參數
//     if ((x+5 > X_rRANGE || x < X_lRANGE) || (y+3 > Y_dRANGE || y < Y_uRANGE)) return true;
// }


// void bullet_move(list<bullet*> &busy){
//     for (auto &b : busy) {
//         b->move();
//     }
// }


// if (((((*it1)->x)-((*it2)->x) <= 3 && ((*it1)->x)-((*it2)->x) >= 0) || (((*it2)->x)-((*it1)->x) <= 1 && ((*it2)->x)-((*it1)->x) >= -1)) && (((*it1)->y)-((*it2)->y) <= 2)) {
//     (*it1)->clean(); // 或許安排成敵人閃爍會比較有感
//     (*it2)->clean();
//     dead.push_back((*it1)); // 注意，待修改，如果當前已經沒有一樓的蹤影，一樓的守衛或許要永久性地刪除
//     rest.push_back((*it2));
//     living.erase(it1);
//     active.erase(it2);
// }
// cout << "?";
// it1++; it2++;


// else { // 因為erase後，所有後面的iterator會前挪，所以不能自動對每個迴圈做it++，要判斷有沒有刪，沒刪才++ // 但這樣做好像也不行?.exe會當掉退出的樣子，所以我學別人新增了_next指標去吃迭代器
//     it1++; it2++; // 不可刪掉包住它們的else的大括號
// }

// 打到敵人的頭
// if ((*it1)->x - (*it2)->x == 1 && (*it1)->y == (*it2)->y)
// 打到敵人的身
// if ((*it1)->x - (*it2)->x <= 2 && (*it2)->x - (*it1)->x <= 1 && (*it1)->y == (*it2)->y)
// 打到敵人的腿
// if ((*it1)->x - (*it2)->x <= 1 && (*it2)->x - (*it1)->x <= 0 && (*it1)->y == (*it2)->y)

// // 處理人物遇到敵人被扣血的狀況，harmful值是敵人傷害你一次需要回復攻擊力的時間，越高表示越不具傷害性
// void character_enermy_interaction(list<enermy*> &ENERMY, character &person) {
//     for (auto p : ENERMY) {
//         if ((abs(p->x-person.x)<=3) && (abs(p->y-person.y)<=3) && p->harmful == HARMFUL) {
//             person.health--;
//             while (p->harmful-- % HARMFUL > 2*HARMFUL/3){
//                 person.clean();
//             }
//             while ((p->harmful % HARMFUL <= 2*HARMFUL/3) && (p->harmful % HARMFUL > HARMFUL/3)){
//                 person.clean();
//                 set_color(FOREGROUND_RED);
//                 person.print();
//             }
//             while (p->harmful-- % HARMFUL < HARMFUL/3){
//                 person.clean();
//             }
//             set_color();
//             // p->harmful = 0;
//         }
//         else {
//             person.clean();
//             set_color(14); // 14=黃色
//             person.print();
//             set_color(); // 畫完後要把顏色還原，方便下一個使用者
//             if (p->harmful<HARMFUL) p->harmful++;
//         }
//         // p->move();
//     }
// }

// 實作出捲動遊戲畫面的效果(其實就是相對運動，將所有物件的y座標增加(增加就是往下移))
// 本來是順便利用scroll_screen統一管控各物件的打印，但為了實作出某些互動功能，所以部分物件的打印挪到別的函數中做實現
// 例如遊戲角色的打印就挪到bullet_enermy_interaction函數中
// void scroll_screen(list<enermy*> &living, list<bullet*> &active, list<stair*> &STAIR, character &person){
//     // 如果遊戲人物他並非處在跳躍狀態，且他所在的高度達到了某個閾值，那麼就捲動畫面
//     if (person.velocity==0 && person.y < SCROLL_THRESHOLD) {
//         scroll_count++;
//         // 只是想在離開第一層時取消地板的顯示
//         if (show_floor && (scroll_count > int(1/SCROLL_SPEED))) {
//             show_floor = false;
//         }
//         person.y += SCROLL_SPEED;
//         for (enermy* e : living) {
//             e->clean();
//             e->y += SCROLL_SPEED;
//             e->move();
//             // e->print();
//         }
//         for (stair* s : STAIR) {
//             s->clean();
//             s->y += SCROLL_SPEED;
//             s->print();
//         }
//         for (bullet* b : active) {
//             b->clean();
//             b->y += SCROLL_SPEED;
//             b->move();
//             // b->print();
//         }
//     }
//     else {
//         for (enermy* e : living) {
//             e->move();
//             // e->print();
//         }
//         for (stair* s : STAIR) {
//             s->print();
//         }
//         for (bullet* b : active) {
//             b->move();
//             // b->print();
//         }
//     }
// }
