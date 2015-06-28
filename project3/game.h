#ifndef GAME_H
#define GAME_H
#include "block.h"
#include "blockarea.h"

extern int game_map[10][10];
extern bool check[10][10],u;
extern int step,score;
extern QGridLayout *mainLayout;

class Game
{
public:
    void print();
    void done_print();
    void check_click();
    void change_position(int p1, int p2, bool action);
    void game_process();
    void set_map();
    void game_over();
};

#endif // GAME_H
