#include <bits/stdc++.h>
#include <stdio.h>
#include <stdlib.h>
#include "windows.h"
#include <time.h>
#include "conio.h"
using namespace std;

int getrandom(int a,int b)
{
    return rand() % (b - a + 1) + a;
}

struct ok
{
    int x,y;
};

ok head;
int dir[5][2] = {{ 0, 0},{ 0,-1},{ 0, 1},{-1, 0},{ 1, 0}};
int length = 3;
int len = 32,hig = 20;
int direction;
int room[100][100];
int hf = 1;

void go(int x,int y)
{
    COORD wok;
    wok.X = x * 2;
    wok.Y = y;
    HANDLE a = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(a,wok);
}

void food()
{
    int x = getrandom(1,len),y = getrandom(1,hig);
    while(room[y][x] != 0)
    {
        x = getrandom(1,len);
        y = getrandom(1,hig);
    }
    room[y][x] = -2;
    go(x,y);printf("●");
    hf = 1;
}

void make_obstacle()
{
    for(int i = 0; i <= len + 1; i++)
    {
        room[0][i] = -1;
        go(i,0);printf("■");
        room[hig+1][i] = -1;
        go(i,hig + 1);printf("■");
    }
    for(int i = 0; i <= hig+1;i++)
    {
        room[i][0] = -1;
        go(0,i);printf("■");
        room[i][len+1] = -1;
        go(len + 1,i);printf("■");
    }
}

void move()
{
    head.x += dir[direction][0];int x = head.x;
    head.y += dir[direction][1];int y = head.y;
    if(room[y][x] != 0 && room[y][x] != -2)
        return ;
    if(room[y][x] == -2)
    {
        length++;
        hf = 0;
    }
    for(int i = 1; i <= len; i++)
        for(int j = 1; j <= hig; j++)
        {
            if(room[j][i] >= 1 && room[y][x] != -2)
            {
                room[j][i]--;
                if(room[j][i] == 0)
                {
                    go(i,j);
                    printf(" ");
                }
            }
        }
    
    room[head.y][head.x] = length;

    go(head.x,head.y);printf("□");
}

int main()
{
    srand(time(0));

    CONSOLE_CURSOR_INFO cursor_info = {1,0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);

    make_obstacle();
    food();

    direction = -3;
    head.x = len/2 + 1;
    head.y = hig/2 + 1;
    room[head.y][head.x] = length;
    go(head.x,head.y);printf("□");

    while(true)
    {
        int dirBefore = direction;//储存未更改的方向
        int key = -1;
        for(int i = 1;i <= 10;i++)//分多个时间点检测方向,而不是只有一个时间点,这样可以提升操作手感
        {
            if(kbhit() && key == -1)//如果有按键按下且未曾读取按键（使同一时间单位内按下多次时不覆盖前一次按键，而是等到下一时间单位读取并做出相应反馈，方便实现掉头等操作）
            {
                key = getch();//读取
                if(key == 224)//读取方向键时需读取两次,第一次必定是224,所以判断读入的是方向键时则再读取一次
                {
                    key = getch();

                    switch(key)
                    {
                        case 72: direction = 1;break;//上
                        case 80: direction = 2;break;//下
                        case 75: direction = 3;break;//左
                        case 77: direction = 4;break;//右
                    }
                }
            }
            Sleep(5);
        }
        if(dirBefore != direction && ( dirBefore + direction == 3 || dirBefore + direction == 7))
            direction = dirBefore;

        move();

        if(room[head.y][head.x] != length)
        {
            go(len/2-2,hig/2 + 1);printf("game over");
            while(1)
            {
                if(GetAsyncKeyState(VK_LBUTTON) & 0x8000)
                {
                    break;
                }
                Sleep(30); // 添加适当的延时
            }
            break;
        }

        if(hf == 0) food();

    }
    return 0;
}