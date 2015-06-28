#include "blockarea.h"
#include "block.h"
#include "game.h"
#include <QApplication>
#include <vector>
#include <time.h>
#include <cstdlib>
#include <QGridLayout>

int game_map[10][10];
bool check[10][10],u;
int step,score;
QGridLayout *mainLayout;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BlockArea w;
    w.start_game();
    w.show();
    return a.exec();
}
