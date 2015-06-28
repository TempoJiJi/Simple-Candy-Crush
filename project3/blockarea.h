#ifndef BLOCKAREA_H
#define BLOCKAREA_H

#include <QMainWindow>
#include <QGridLayout>
#include "game.h"
#include "block.h"

extern int game_map[10][10];
extern bool check[10][10],u;
extern int step,score;
extern QGridLayout *mainLayout;

class BlockArea:public QWidget
{
    Q_OBJECT
public:
    explicit BlockArea(QWidget *parent=0);
    void start_game();
    void go_check_click();
};

#endif // BLOCKAREA_H
