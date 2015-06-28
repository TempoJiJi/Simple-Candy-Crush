#ifndef BLOCK_H
#define BLOCK_H

#include "blockarea.h"
#include <QMainWindow>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>
#include <QtAlgorithms>
#include <QGridLayout>
#include <QPixmap>

extern int game_map[10][10];
extern bool check[10][10];

class Block:public QLabel
{
public:
    explicit Block(int number,int place,bool rule,QWidget *parent=0);
    void get_position();
    void print_map();
    void step_count(int step);
    void count(int score);
protected:
    void mousePressEvent(QMouseEvent *ev);
private:
    int a,b;
};

#endif // BLOCK_H
