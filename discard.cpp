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