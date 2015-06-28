#include "block.h"
#include <iostream>
#include <QGridLayout>
#include <QToolBox>
#include <QPixmap>
using namespace std;


Block::Block(int number ,int place,bool rule, QWidget *parent):
    QLabel(parent)
{
    if(rule){
        setPixmap(QPixmap(":/images/images/"+QString("%1").arg(number)+".png"));
        //初始化位置
        a=place/10;
        b=place%10;
    }
    else{
        setText(QString::number(number));
        setStyleSheet("font:18pt;color:red");
    }
}

void Block::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button()==Qt::LeftButton){
        //看現在的位置是什麼就set什麼
        setPixmap(QPixmap(":/images/images/select_"+QString("%1").arg(game_map[a][b])+".png"));
        if(!check[a][b]){
            check[a][b]=true;
            get_position();
        }
    }
}

void Block::get_position()
{
    BlockArea go;
    go.go_check_click();
}

void Block::print_map()
{
    setPixmap(QPixmap(":/images/images/"+QString("%1").arg(game_map[a][b])+".png"));
}

void Block::count(int step)
{
    setText(QString::number(step));
    setStyleSheet("font:18pt;color:red");
}
