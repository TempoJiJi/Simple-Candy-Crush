#include "game.h"
#include <vector>
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <QGridLayout>
#include <QThread>
#include <QTimer>
#include <QMessageBox>

using namespace std;
int click;
bool ch[10][10];

void Game::print()
{
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            Block* yoo=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
            yoo->print_map();
       }
    }
    set_map();
}

void Game::done_print()
{
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            Block* yooo=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
            yooo->print_map();
       }
    }
    Block* yoo=static_cast<Block*>(mainLayout->itemAtPosition(10,9)->widget());
    yoo->count(score);
    if(step==0)
        game_over();
}

//檢查外面click的位置
void Game::check_click()
{
    static int x,y;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(check[i][j] && (!ch[i][j]) ){
                click++;
                ch[i][j]=true;
                if(click==1){
                    x=i;y=j;
                }
            }
            if(click==2){
                if( (i==x && (j-1)==y) || (j==y && (i-1)==x) || (i==x && j+1==y) || (j==y && (i+1)==x)){
                    memset(ch,false,sizeof(ch));
                    click=0;x=100;y=100;
                    change_position(i,j,false);
                }
                else{
                    memset(check,false,sizeof(check));
                    memset(ch,false,sizeof(ch));
                    click=0;x=100;y=100;
                    done_print();
                }
            }
        }
    }
}

//換位還有檢查是不是星星，是星星就直接執行
void Game::change_position(int p1,int p2,bool action)
{
    int record,boom;
    BlockArea go;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            if(check[i][j] && (i!=p1 || j!=p2) ){
                record=game_map[i][j];
                //檢查兩個都不是星星
                if(game_map[i][j]!=5 && game_map[p1][p2]!=5){
                    game_map[i][j]=game_map[p1][p2];
                    game_map[p1][p2]=record;
                    break;
                }
                //其中一個是星星，就執行星星的演算法
                else if(game_map[i][j]==5 || game_map[p1][p2]==5){
                    //檢查另一個是什麼號碼
                    if(game_map[i][j]!=5){
                        if(game_map[i][j]>5)
                            game_map[i][j]/=10;
                        boom=game_map[i][j];
                        game_map[p1][p2]=0;
                    }
                    else{
                        if(game_map[p1][p2]>5)
                            game_map[p1][p2]/=10;
                        boom=game_map[p1][p2];
                        game_map[i][j]=0;
                    }
                    for(int k=0;k<10;k++){
                        for(int l=0;l<10;l++){
                            if(game_map[k][l]==boom)
                                game_map[k][l]=0;
                        }
                    }
                }
            }
        }
    }
    //！！！！呼叫print 圖function
    if(!action)
        print();
    else{
        memset(check,false,sizeof(check));
        done_print();
    }
}

void Game::game_process(){
    //temp=0 普通消去 temp=1 水平 temp=2 垂直 temp=3 炸彈 temp=4 星星 action表示換位了有做動作
    int temp=0,record,k,o,action=0;
    BlockArea go;
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            temp=0;
            //普通石頭的檢查
            if(game_map[i][j]!=0 && game_map[i][j]<5){
                //檢查水平的，還有炸彈
                if((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j+1]==game_map[i][j+2])){
                    if( (j<7) && (game_map[i][j+2]==game_map[i][j+3]) ){
                        temp=1;
                        //左邊T 型炸彈
                        if(game_map[i][j]==game_map[i+1][j+1] && game_map[i][j]==game_map[i+2][j+1])
                            temp=30;
                        //右邊T型炸彈
                        if(game_map[i][j]==game_map[i+1][j+2] && game_map[i][j]==game_map[i+2][j+2])
                            temp=34;
                        //星星
                        if(game_map[i][j]==game_map[i][j+4])
                            temp=4;
                    }
                    //T型炸彈
                    if(game_map[i][j]==game_map[i+1][j+1] && game_map[i][j]==game_map[i+2][j+1])
                        temp=35;
                    //C型炸彈
                    else if((i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i+1][j]==game_map[i+2][j]))
                        temp=3;
                    //7型炸彈
                    else if((i<8) && (game_map[i][j]==game_map[i+1][j+2]) && (game_map[i][j]==game_map[i+2][j+2]))
                        temp=33;
                    //根據不同的temp開始消去
                    if(temp==0){
                        for(o=j;o<j+3;o++)
                            game_map[i][o]=0;
                        action++;
                    }
                    else if(temp==1){
                        record=game_map[i][j];
                        for(o=j;o<j+4;o++)
                            game_map[i][o]=0;
                        game_map[i][j]=record*10+temp;
                        action++;
                    }
                    else if(temp==3){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        for(o=j;o<j+3;o++)
                            game_map[i][o]=0;
                        game_map[i][j]=record*10+temp;
                        action++;
                    }
                    else if(temp==30){
                        record=game_map[i][j];
                        for(o=j;o<j+4;o++)
                            game_map[i][o]=0;
                        for(k=i;k<i+3;k++)
                            game_map[k][j+1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==33){
                        record=game_map[i][j];
                        for(o=j;o<j+3;o++)
                            game_map[i][o]=0;
                        for(k=i;k<i+3;k++)
                            game_map[k][j+2]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==34){
                        record=game_map[i][j];
                        for(o=j;o<j+4;o++)
                            game_map[i][o]=0;
                        for(k=i;k<i+3;k++)
                            game_map[k][j+2]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==35){
                        record=game_map[i][j];
                        for(o=j;o<j+3;j++)
                            game_map[i][o]=0;
                        for(k=i;k<i+3;k++)
                            game_map[k][j+1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==4){
                        for(o=j;o<j+5;o++)
                            game_map[i][o]=0;
                        game_map[i][j]=5;
                        action++;
                    }
                }
                //檢查垂直
                else if((i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i+1][j]==game_map[i+2][j])){
                    if(game_map[i+2][j]==game_map[i+3][j])
                        temp=2;
                    if(game_map[i+3][j]==game_map[i+4][j])
                        temp=4;
                    //L型炸彈
                    else if((j<8) && game_map[i][j]==game_map[i+2][j+1] && game_map[i][j]==game_map[i+2][j+2])
                        temp=3;
                    //相反L型炸彈
                    else if( (j>1) && (game_map[i][j]==game_map[i+2][j-1]) && game_map[i][j]==game_map[i+2][j-2])
                        temp=30;
                    //倒反T型炸彈
                    else if(((j>0)&&(j<9)) && game_map[i][j]==game_map[i+2][j+1] && game_map[i][j]==game_map[i+2][j-1])
                        temp=31;
                    //轉左90 T型炸彈
                    else if((j<8) && game_map[i][j]==game_map[i+1][j+1] && game_map[i][j]==game_map[i+1][j+2])
                        temp=32;
                    //轉右90 T型炸彈
                    else if((j>1) && game_map[i][j]==game_map[i+1][j-1] && game_map[i][j]==game_map[i+1][j-2])
                        temp=33;
                    //十字架炸彈
                    else if((j>0&&j<9) && game_map[i][j]==game_map[i+1][j-1] && game_map[i][j]==game_map[i+1][j+1])
                        temp=34;
                    //開始消去
                    if(temp==0){
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        action++;
                    }
                    else if(temp==2){
                        record=game_map[i][j];
                        for(k=i;k<i+4;k++)
                            game_map[k][j]=0;
                        game_map[i][j]=record*10+temp;
                        action++;
                    }
                    else if(temp==3){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        for(o=j;o<j+3;o++)
                            game_map[i+2][o]=0;
                        if((j>0) && game_map[i][j]==game_map[i+2][j-1])
                            game_map[i+2][j-1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==30){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        for(o=j;o>j-3;o--)
                            game_map[i+2][o]=0;
                        if((j<9) && game_map[i][j]==game_map[i+2][j+1])
                            game_map[i+2][j+1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==31){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        game_map[k][j-1]=0;game_map[k][j+1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==32){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        for(o=j;o<j+3;o++)
                            game_map[i+1][o]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==33){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        for(o=j;o>j-3;o--)
                            game_map[i+1][o]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if(temp==34){
                        record=game_map[i][j];
                        for(k=i;k<i+3;k++)
                            game_map[k][j]=0;
                        game_map[i+1][j-1]=0;game_map[i+1][j+1]=0;
                        game_map[i][j]=record*10+3;
                        action++;
                    }
                    else if (temp==4){
                        for(k=i;k<i+5;k++)
                            game_map[k][j]=0;
                        game_map[i][j]=5;
                        action++;
                    }
                }
            }

            //特別石頭
            else if(game_map[i][j]>10){
                //rule=1 水平 rule=2垂直 rule=3炸彈
                int rule,save,act=0;
                save=game_map[i][j];     //先儲存現在的號碼
                rule=game_map[i][j]%10;
                game_map[i][j]/=10;
                if(rule==1){
                    //檢查 xoo , oxo , oox
                    if( ((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+2]))
                      ||((j>1) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j-2]))
                      ||((j>0)&&(j<9) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j+1]))){
                        for(int l=0;l<10;l++)
                            game_map[i][l]=0;
                        act++;
                        action++;
                    }
                    //檢查 x在中間，x在最上面，x在最下面
                    else if( ((i<8) && game_map[i][j]==game_map[i+1][j] && game_map[i][j]==game_map[i+2][j])
                          || ((i>1) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i-2][j])
                          || ((i>0)&&(i<9) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i+1][j])){
                        if((i<8) && game_map[i][j]==game_map[i+1][j] && game_map[i][j]==game_map[i+2][j]){
                            game_map[i+1][j]=0;game_map[i+2][j]=0;   //因為是水平消除 所以下面兩個要另外消
                        }
                        else if((i>1) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i-2][j]){
                            game_map[i-1][j]=0;game_map[i-2][j]=0;   //上面兩個另外消
                        }
                        else if((i>0)&&(i<9) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i+1][j]){
                            game_map[i-1][j]=0;game_map[i+1][j]=0;  //上下兩個另外消
                        }
                        for(int l=0;l<10;l++)
                            game_map[i][l]=0;
                        action++;
                        act++;
                    }
                }
                //垂直消去，跟水平的演算法一樣，有另外消的就另外消
                else if(rule==2){
                    if( ((i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i][j]==game_map[i+2][j]))
                     || ((i>1) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i-2][j])
                     || ((i>0)&&(i<9) && game_map[i][j]==game_map[i-1][j] && game_map[i][j]==game_map[i+1][j])){
                        for(int l=0;l<10;l++)
                            game_map[l][j]=0;
                        action++;
                        act++;
                    }
                    else if(((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+1]))
                         || ((j>1) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j-2]))
                         || ((j>0)&&(j<9) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j+1]))){
                        if((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+1]))
                            game_map[i][j+1]=0;game_map[i][j+2]=0;
                        if((j>1) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j-2]))
                            game_map[i][j-1]=0;game_map[i][j-2]=0;
                        if((j>0)&&(j<9) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j+1]))
                            game_map[i][j-1]=0;game_map[i][j+1]=0;
                        for(int l=0;l<10;l++)
                            game_map[l][j]=0;
                        action++;
                        act++;
                    }
                }
                else if(rule==3){
                    //檢查 x在最上面or最下面
                    if(((i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i][j]==game_map[i+2][j]))
                     ||((i>1) && (game_map[i][j]==game_map[i-1][j]) && (game_map[i][j]==game_map[i-2][j]))){
                        for(int t=i-1;t<i+2;t++)
                            for(int r=j-1;r<j+2;r++)   //消去炸彈周圍的
                                game_map[t][r]=0;
                        if((i<8) && (game_map[i][j]==game_map[i+1][j]) && (game_map[i][j]==game_map[i+2][j]))
                            game_map[i+2][j]=0;      //如果炸彈在最上面，那麼最下面的那粒要另外消，因為炸彈的範圍沒有到那裡
                        if((i>1) && (game_map[i][j]==game_map[i-1][j]) && (game_map[i][j]==game_map[i-2][j]))
                            game_map[i-2][j]=0;      //最上面的另外消
                        action++;
                        act++;
                    }
                    //檢查炸彈在最右邊還是左邊
                    else if(((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+2]))
                          ||((j>1) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j-2]))){
                        for(int t=i-1;t<i+2;t++)
                            for(int r=j-1;r<j+2;r++)
                                game_map[t][r]=0;
                        if((j<8) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j+2]))
                            game_map[i][j+2]=0;    //在最左邊的話，最右邊的那粒要另外消
                        if((j>1) && (game_map[i][j]==game_map[i][j-1]) && (game_map[i][j]==game_map[i][j-2]))
                            game_map[i][j-2]=0;    //另外消最左邊的
                        action++;
                        act++;
                    }
                    //檢查炸彈在中間
                    else if(((i>0)&&(i<9) && (game_map[i][j]==game_map[i-1][j]) && (game_map[i][j]==game_map[i+1][j]))
                          ||((j>0)&&(j<9) && (game_map[i][j]==game_map[i][j+1]) && (game_map[i][j]==game_map[i][j-1]))){
                        for(int t=i-1;t<i+2;t++)
                            for(int r=j-1;r<j+2;r++)    //炸彈在中間，所以沒有東西需要另外消
                                game_map[t][r]=0;
                        action++;
                        act++;
                    }
                }
                //沒做動作就把號碼換回去
                if(act==0){
                    game_map[i][j]=save;
                }
            }
        }
    }
    //換位了無任何反應
    if(action==0){
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                if(check[i][j]){
                    u=false;
                    change_position(i,j,true);
                }
            }
        }
        u=false;
        done_print();
    }
    //換位後有做動作有反應
    else{
        if(!u){
            step--;
            Block* yoo=static_cast<Block*>(mainLayout->itemAtPosition(10,0)->widget());
            yoo->count(step);
        }
        u=true;
        memset(check,false,sizeof(check));
        print();
    }
}

void Game::set_map(){
    int k;
    srand(time(NULL));
    //0的位置由上面的號碼替代
    for(int i=9;i>=0;i--){
        for(int j=0;j<10;j++){
            if(game_map[i][j]==0){
                score++;
                for(k=i-1;k>=0;k--){
                    if(game_map[k][j]!=0){
                        game_map[i][j]=game_map[k][j];
                        game_map[k][j]=0;
                        break;
                    }
                }
            }
        }
    }
    //補0
    for(int i=9;i>=0;i--){
        for(int j=0;j<10;j++){
            if(game_map[i][j]==0)
                game_map[i][j]=rand()%4+1;
        }
    }
    game_process();
}

void Game::game_over()
{
    QLabel *image = new QLabel();
    int star;
    if(score<=700)
        star=0;
    else if(score>700 && score<=1500)
        star=1;
    else if(score>1500 && score<=2200)
        star=2;
    else
        star=3;
    image->setPixmap(QPixmap(":/images/images/star_"+QString("%1").arg(star)+".png"));
    image->show();
}
