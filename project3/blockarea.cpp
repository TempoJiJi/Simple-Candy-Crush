#include "blockarea.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <QTimer>
#include <QtCore>
#include <QPixmap>

using namespace std;

BlockArea::BlockArea(QWidget *parent):
    QWidget(parent)
{
    QPixmap bckground(":/images/images/background.jpg");
    bckground = bckground.scaled(this->size());
    QPalette palette;
    palette.setBrush(QPalette::Background, bckground);
    this->setPalette(palette);
}


void BlockArea::start_game()
{
    step=15;score=0;u=false;
    memset(check,false,sizeof(check));
    srand(time(NULL));
    for(int i=0;i<10;i++)
        for(int j=0;j<10;j++)
            game_map[i][j]=rand()%4+1;
    int temp;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if( (j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+2]) ){
                temp=1;
                while(1){
                    if( (temp==game_map[i-1][j+1] || temp==game_map[i+1][j+1]) || (temp==game_map[i][j]) ){
                        temp++;
                        continue;
                    }
                    else{
                        game_map[i][j+1]=temp;
                        break;
                    }
                }
            }
            else if( (i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i][j]==game_map[i+2][j]) ){
                temp=1;
                while(1){
                    if(temp==game_map[i][j]){
                        temp++;
                        continue;
                    }
                    else{
                        game_map[i+1][j]=temp;
                        break;
                    }
                }
            }
        }
    }
    mainLayout = new QGridLayout(this);
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            mainLayout->addWidget(new Block(game_map[i][j],i*10+j,true),i,j);
        }
    }
    mainLayout->addWidget(new Block(step,0,false),10,0);
    mainLayout->addWidget(new Block(score,0,false),10,9);
}

void BlockArea::go_check_click(){
    Game go;
    go.check_click();
}
