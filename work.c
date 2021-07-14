#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
//#include <graphics.h>
//#include "DataStruct.h"
// #include "foundation.c"
// #include "process.c"
#define RIdERCOAST 300                          //骑手的雇佣成本
#define PROFIT 10                               //每准时完成一单外卖的盈利
#define LOSS 50                                 //每超时完成一旦外卖的损失
#define DISTRIITIME 3                           //订单分配时间上限，超时则视为拒单；
#define PASTIME 30                              //外卖送达时间上限，超时则罚款；
#define FORBIDTIME 60                           //派送允许的最大时间，超时则吊销执照
#define POSX 8                                  //骑手起始横坐标
#define POSY 9                                  //骑手起始纵坐标
#define RIDERNUMMAX 200                         //骑手最大人数
#define SideLength 40                           //建筑边长
#define LengthMax 680                           //图形界面长度
const int BLOCKLEFT = 0;                        //区域1的左边界
const int BLOCKRIGHT = 16;                      //区域1的右边界
const int BLOCKUP = 0;                          //区域2的左边界
const int BLOCKDOWN = 16;                       //区域2的右边界
const int dir[9] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; //必经点与骑手的方向关系
//1为左，2为右，3为上，4为下，5为左上，6为左下，7为右上，8为右下，9为两点重合
const int dic[12][2] = {{-2, 0}, {2, 0}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}, {0, -2}, {0, 2}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}}; //标记骑手的移动方向
//骑手在状态0下：0为向左，1为向右，2为左上，3为右上，4为左下，5为右下
//骑手在状态1下：6为向上，7为向下，8为左上，9为右上，10为左下，11为右下
//// const int dic2[6][2] = {{0, -2}, {0, 2}, {-1, -1}, {1, -1}, {-1, 1}, {1, 1}};
//// 骑手在状态1下：0为向上，1为向下，2为左上，3为右上，4为左下，5为右下

//变量定义可以写在主函数里面
//int n=0;//订单序号，开始时为0，也为当前接单数
int ALLORD = 0;   //总结单数
int TIME = 0;     //记录时间
int MONEY = 1000; //成本为1000
int RIDERNUM = 0; //雇佣骑手数量
int ACHORD = 0;   //订单完成数
int OVERORD = 0;  //订单超时数
int RUN = 0;      //判断能否继续经营，破产或吊销执照均不能继续营业；
int ENDTIME;      //统计最后一个订单的发单时间
int TOTORD = 0;   //统计样例中订单总数
int h[20][20];    //用于画图

//全局数据结构定义
struct ordernode
{
    int no;          //订单序号
    int state;       //订单现处于的状态
    int startime;    //时间
    int LossTime;    //出现超时的时间
    int ord_geatDis; //计算餐厅到买家距离
    int ord_geatDir; //计算餐厅到买家方向
    int ordPos_X;    //餐馆横坐标
    int ordPos_Y;    //餐馆纵坐标
    int gestPos_X;   //买家横坐标
    int gestPos_Y;   //买家纵坐标
    int keyPosX;     //关键点横坐标
    int keyPosY;     //关键点纵坐标
    struct ordernode *next;
}; //创建一个关于餐馆订单的链表
typedef struct ordernode orderNode;
typedef orderNode *orderList;

struct ridernode
{
    int no;         //标记骑手序号
    int state;      //标记骑手状态
    int isGod;      //标记先知骑手
    int ridPosCurX; //骑手横坐标
    int ridPosCurY; //骑手纵坐标
    int curDir;     //当前骑手前往的方向
    int curDis;     //当前骑手距离关键点的距离
    int numOrd;     //骑手携带的订单数量
    int finishTime; //计算骑手完成当前订单所用时间
    int AchNum;
    int OverNum;
    int printfX[4];    //图形打印时输出的横坐标
    int printfY[4];    //图形打印时输出的纵坐标
    orderList HeadOrd; //标记骑手订单链表的首地址
    orderNode *next;   //与相应的订单相对应，与订单链表连系起来
    //int ridPosNextX;   //骑手下一步横坐标的移动方向
    //int ridPosNextY;   //骑手下一步纵坐标的移动方向
    int flag;
}; //创建一个骑手状态的结构体数组；
typedef struct ridernode riderNode;
riderNode rider[RIDERNUMMAX];

struct mapnode
{
    int x;
    int y;
    int flag;
};
typedef struct mapnode mapNode;
mapNode mapn[20][20];
mapNode node[100];


int main()
{
    int sleep = 2000;
    //创建图形界面
    // initgraph(LengthMax, LengthMax);
    // setbkcolor(WHITE);
    // for (int i = 0; i <= LengthMax; i += 2 * SideLength)
    // {
    //     for (int j = 0; j <= LengthMax; j += 2 * SideLength)
    //     {
    //         setcolor(GREEN);
    //         rectangle(j, i, j + SideLength, i + SideLength);
    //         setfillcolor(BLUE);
    //         floodfill(j + 1, i + 1, GREEN);
    //     }
    // }
    // setlocale(LC_ALL, ""); //修改程序语言环境以适应中文
    orderList HeadOrd1 = NULL;
    orderList HeadOrd2 = NULL;
    HeadOrd1 = (orderNode *)malloc(sizeof(orderNode));
    HeadOrd2 = (orderNode *)malloc(sizeof(orderNode));
    HeadOrd1 = input();
    HeadOrd2->next = NULL;
    // hireRider();//对单个骑手进行调试时使用
    for (TIME = 1; RUN == 0 && TIME <= ENDTIME && ACHORD < TOTORD; TIME++)
    {
        while (MONEY >= 350)
        {
            if (RIDERNUM <= RIDERNUMMAX)
                hireRider(); //rider range from 1;
        }
        perSend(HeadOrd1);
        HeadOrd2->next = CreatOrdList(HeadOrd1);
        if (HeadOrd2->next != NULL)
        {
            send(HeadOrd2);
        }
        for (int i = 0; i < RIDERNUM && RUN == 0; i++)
        {
            if (rider[i].numOrd != 0)
            {
                int path = plan(rider[i]);
                rider[i] = execute(path, rider[i]);
            }
        }
        //Sleep(sleep);
        output();
        // picture();
        //scanf("\n");
    }
    if (ACHORD < TOTORD)
    {
        RUN = 2;
        Broke();
        //output();
    }
    //getch();
    return 0;
}

orderList input()
{
    orderNode *headPtr = NULL;
    headPtr = (orderNode *)malloc(sizeof(orderNode));
    orderNode *lastPtr = NULL, *currentPtr = NULL;
    lastPtr = headPtr;
    int a, b, c, d, e, f;
    FILE *fp = fopen("C:\\VScode\\STRUCTURE\\project\\data.txt", "r");
    if (fp == NULL)
    {
        printf("error open!\n");
    }
    else
    {
        while (fscanf(fp, "%d %d %d %d %d %d", &a, &b, &c, &d, &e, &f) != EOF)
        {
            currentPtr = (orderNode *)malloc(sizeof(orderNode));
            currentPtr->no = a;
            currentPtr->startime = b;
            currentPtr->ordPos_X = c;
            currentPtr->ordPos_Y = d;
            currentPtr->gestPos_X = e;
            currentPtr->gestPos_Y = f;
            currentPtr->next = NULL;
            // currentPtr->keyPosX = currentPtr->ordPos_X;
            // currentPtr->keyPosY = currentPtr->ordPos_Y;
            // currentPtr->state
            lastPtr->next = currentPtr;
            lastPtr = currentPtr;
            ENDTIME = currentPtr->startime + FORBIDTIME; //规则允许的最大时间
            TOTORD++;
        }
    }
    lastPtr->next = NULL;
    fclose(fp);
    return headPtr;
}

void mouseSend() //进行鼠标输入
{
}

orderList CreatOrdList(orderList headPtr1) //该链表与定单链表交换节点
{
    orderNode *curOrd1 = headPtr1->next;
    orderNode *tempOrd = headPtr1;
    orderNode *headPtr2;
    headPtr2 = (orderNode *)malloc(sizeof(orderNode));
    headPtr2 = NULL;
    orderNode *curOrd2 = headPtr2;
    int flag = 0;
    int isCreat = 0;
    while (flag == 0 && curOrd1)
    {
        flag = 1;
        if (curOrd1->startime == TIME)
        {
            tempOrd->next = curOrd1->next;
            //curOrd2->next = curOrd1;
            if (headPtr2 == NULL)
            {
                headPtr2 = curOrd1;
            }
            else
            {
                curOrd2 = headPtr2;
                while (curOrd2->next)
                {
                    curOrd2 = curOrd2->next;
                }
                curOrd2->next = curOrd1;
            }
            // setfillcolor(YELLOW);
            //floodfill(curOrd1->ordPos_X*SideLength+1, curOrd1->ordPos_Y*SideLength+1, EGERGB(0xFF, 0xFF, 0xFF));
            //setfillcolor(GREEN);
            //floodfill(curOrd1->gestPos_X*SideLength+1, curOrd1->gestPos_Y*SideLength+1, EGERGB(0xFF, 0xFF, 0xFF));
            curOrd1->next = NULL;
            curOrd2 = curOrd1;
            curOrd1 = tempOrd->next;
            flag = 0;
            isCreat = 1;
        }
    }
    if (isCreat == 1)
        return headPtr2;
    else
    {
        return NULL;
    }
}

void send(orderList HeadOrd) //将订单派送给骑手
{
    orderNode *curOrd = HeadOrd->next;
    while (curOrd)
    {
        int minDis = 100;                  //寻找距离最短的骑手位置
        int minTime = 100000;              //寻找用时最短的骑手
        int minDisRider = -1;              //记录距离最短骑手的号码
        int sameDirRider = -1;             //记录相同方向的骑手
        int minTimeRider = -1;             //记录用时最短的骑手
        for (int i = 0; i < RIDERNUM; i++) //遍历骑手
        {
            int ordx = curOrd->ordPos_X;
            int ordy = curOrd->ordPos_Y;
            int riderx = rider[i].ridPosCurX;
            int ridery = rider[i].ridPosCurY;
            int curDir = Direction(ordx, ordy, riderx, ridery);
            int curDis = Manhattan(ordx, ordy, riderx, ridery);
            if (rider[i].numOrd == 0) //优先考虑待命骑手
            {
                if (minDis > curDis)
                {
                    minDis = curDis;
                    minDisRider = i;
                }
            }

            if (minTime > rider[i].finishTime)
            {
                minTime = rider[i].finishTime;
                minTimeRider = i;
            }

            if (curDir == rider[i].curDir && rider[i].numOrd <= 2) //如果顺路
            {
                sameDirRider = i;
            }
        }
        if (minDisRider != -1) //第一优先级
        {
            rider[minDisRider] = addOrd(rider[minDisRider], HeadOrd);
        }
        else if (minTimeRider != -1) //第二优先级
        {
            rider[minTimeRider] = addOrd(rider[minTimeRider], HeadOrd);
        }
        else if (sameDirRider != -1) //第三优先级
        {
            rider[sameDirRider] = addOrd(rider[sameDirRider], HeadOrd);
        }
        curOrd = HeadOrd->next;
    }
}

void perSend(orderList HeadOrd) //对待命骑手做先知处理
{
    orderNode *curOrd = HeadOrd->next;
    for (int i = 0; i < RIDERNUM && curOrd != NULL; i++)
    {
        if (rider[i].numOrd == 0)
        {
            rider[i].isGod = 1;
            rider[i] = addOrd(rider[i], HeadOrd);
            ALLORD--;
            curOrd = HeadOrd->next;
        }
    }
}

riderNode execute(int path, riderNode rider) //执行已规划路径
{
    int num = 0;
    int condition; //标记骑手属性:0为待命，1为正在派送，2为停靠餐厅，3为停靠食客，4为停靠餐客
    orderNode *curOrd = rider.HeadOrd->next;
    orderNode *tempOrd = rider.HeadOrd;
    if (rider.flag == 1)
    {
        // setcolor(YELLOW);
        // circle(8.5 * SideLength, 9.5 * SideLength, 0.49 * SideLength);
        // setfillcolor(YELLOW);
        // floodfill(8.5 * SideLength + 1, 9.5 * SideLength + 1, YELLOW);
        rider.flag = 0;
    }
    move(rider.ridPosCurX, rider.ridPosCurY, dic[path][0], dic[path][1], rider.state);
    rider.ridPosCurX += dic[path][0];
    rider.ridPosCurY += dic[path][1];
    rider.AchNum = -1;
    rider.OverNum = -1;
    rider.printfX[0] = 0;
    if (rider.isGod == 1 && curOrd->startime == TIME)
    {
        rider.isGod = 0;
        ALLORD++;
    }
    while (curOrd && RUN == 0)
    {
        if (TIME - curOrd->startime > FORBIDTIME) //实时进行破产分析
        {
            RUN = 2;
            Broke();
            wprintf(L"%d号骑手的编号%d单超时！\n", rider.no, curOrd->no);
            wprintf(L"该订单信息：派单时间：%d  订单状态：%d    餐厅地点:%d  %d  食客地点：%d    %d\n", curOrd->startime, curOrd->state, curOrd->ordPos_X, curOrd->ordPos_Y, curOrd->gestPos_X, curOrd->gestPos_Y);
        }
        else if (TIME - curOrd->startime == PASTIME + 1) //调试专用
        {
            wprintf(L"%d号骑手的编号%d单超时！\n", rider.no, curOrd->no);
            wprintf(L"该订单信息：派单时间：%d  订单状态：%d    餐厅地点:%d  %d  食客地点：%d    %d\n", curOrd->startime, curOrd->state, curOrd->ordPos_X, curOrd->ordPos_Y, curOrd->gestPos_X, curOrd->gestPos_Y);
        }
        if (TIME == curOrd->LossTime + 1)
        {
            OVERORD++;
            MONEY -= LOSS;
            rider.OverNum = curOrd->no;
            if (MONEY < 0)
            {
                RUN = 1;
                Broke();
            }
        }
        if (isArrive(curOrd->keyPosX, curOrd->keyPosY, rider.ridPosCurX, rider.ridPosCurY) == 1 && TIME >= curOrd->startime && rider.numOrd > 0)
        {
            num++;
            rider.printfX[num] = curOrd->keyPosX;
            rider.printfY[num] = curOrd->keyPosY;
            mapn[curOrd->keyPosX][curOrd->keyPosY].flag = 0;
            if (curOrd->state == 1 && curOrd->ord_geatDis != 0)
            {
                condition = 2;
                curOrd->state = 2;
                curOrd->keyPosX = curOrd->gestPos_X;
                curOrd->keyPosY = curOrd->gestPos_Y;
                mapn[curOrd->keyPosX][curOrd->keyPosY].flag = rider.no;
                curOrd = curOrd->next;
                tempOrd = tempOrd->next;
            }
            else //已完成订单或者是两者在同一位置//标记骑手属性:0为待命，1为正在派送，2为停靠餐厅，3为停靠食客，4为停靠餐客
            {
                rider.numOrd--;
                ACHORD++;
                rider.AchNum = curOrd->no;
                if (curOrd->state == 1)
                {
                    condition = 4;
                    mapn[curOrd->gestPos_X][curOrd->gestPos_Y].flag = 0;
                    //setfillcolor(YELLOW);
                    //floodfill(curOrd->ordPos_X*SideLength+1, curOrd->ordPos_Y*SideLength+1, EGERGB(0x0, 0x0, 0x0));
                }
                else if (curOrd->state == 2)
                {
                    condition = 3;
                    //setfillcolor(YELLOW);
                    //floodfill(curOrd->ordPos_X*SideLength+1, curOrd->ordPos_Y*SideLength+1, EGERGB(0x0, 0x0, 0x0));
                }
                if (TIME - curOrd->startime <= PASTIME)
                {
                    MONEY += PROFIT;
                }
                rider.isGod = 0;
                orderNode *FreeOrd = NULL;
                tempOrd->next = curOrd->next;
                FreeOrd = curOrd;
                curOrd = curOrd->next;
                free(FreeOrd);
            }
        }
        else
        {
            curOrd = curOrd->next;
            tempOrd = tempOrd->next;
        }
        if (num == 1)
            rider.printfX[3] = condition;
        else if (num == 2)
        {
            rider.printfY[3] = condition;
        }
    }
    rider.printfX[0] = num;
    rider = upDateRider(rider); //每次操作完成对骑手数据进行更新
    if (rider.HeadOrd->next == NULL)
    {
        rider.HeadOrd->next = rider.next;
    }
    return rider;
}

void output() //输出
{
    int AchNum = 0;
    int OverNum = 0;
    int Ach[RIDERNUMMAX] = {0};
    int Over[RIDERNUMMAX] = {0};
    FILE *fp;
    fp = fopen("C:\\VScode\\STRUCTURE\\project\\two_10_output5.txt", "a");
    if (RUN == 3)
    {
        wprintf(L"因为程序过程中出错，故跳出\n");
        fprintf(fp, "因为程序过程中出错，故跳出\n");
    }
    for (int i = 0; i < RIDERNUM; i++)
    {
        if (rider[i].OverNum != -1)
        {
            Over[OverNum] = rider[i].OverNum;
            OverNum++;
            rider[i].OverNum = -1;
        }
        if (rider[i].AchNum != -1)
        {
            Ach[AchNum] = rider[i].AchNum;
            AchNum++;
            rider[i].AchNum = -1; //该数据只使用一次
        }
    }
    wprintf(L"时间:%d\n", TIME);
    fprintf(fp, "时间：%d\n", TIME);
    wprintf(L"钱:%d\n", MONEY);
    fprintf(fp, "钱：%d\n", MONEY);
    wprintf(L"接单数:%d\n", ALLORD);
    fprintf(fp, "接单数：%d\n", ALLORD);
    wprintf(L"完成数：%d；结单：", ACHORD);
    fprintf(fp, "完成数：%d；结单：", ACHORD);
    if (AchNum == 0)
    {
        wprintf(L"；");
        fprintf(fp, "；");
    }
    else
    {
        printf("%d", Ach[0]);
        fprintf(fp, "%d", Ach[0]);
        for (int i = 1; i < AchNum; i++)
        {
            printf(" %d", Ach[i]);
            fprintf(fp, " %d", Ach[i]);
        }
        wprintf(L"；");
        fprintf(fp, "；");
    }
    printf("\n");
    fprintf(fp, "\n");
    wprintf(L"超时数%d；罚单：", OVERORD);
    fprintf(fp, "超时数：%d；罚单：", OVERORD);
    if (OverNum == 0)
    {
        wprintf(L"；");
        fprintf(fp, "；");
    }
    else
    {
        printf("%d", Over[0]);
        fprintf(fp, "%d", Over[0]);
        for (int i = 1; i < OverNum; i++)
        {
            printf(" %d", Over[i]);
            fprintf(fp, " %d", Over[i]);
        }
        wprintf(L"；");
        fprintf(fp, "；");
    }
    printf("\n");
    fprintf(fp, "\n");
    for (int i = 0; i < RIDERNUM; i++)
    {
        wprintf(L"骑手%d位置：%d，%d；停靠：", i, rider[i].ridPosCurX, rider[i].ridPosCurY);
        fprintf(fp, "骑手%d位置：%d,%d；停靠：", i, rider[i].ridPosCurX, rider[i].ridPosCurY);
        if (rider[i].printfX[0] != 0)
        {
            if (rider[i].printfX[3] == 2)
            {
                wprintf(L"餐馆 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
                fprintf(fp, "餐馆 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
            }
            else if (rider[i].printfX[3] == 3)
            {
                wprintf(L"食客 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
                fprintf(fp, "食客 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
            }
            else if (rider[i].printfX[3] == 4)
            {
                wprintf(L"餐客 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
                fprintf(fp, "餐客 %d %d", rider[i].printfX[1], rider[i].printfY[1]);
            }
            if (rider[i].printfX[0] == 2)
            {
                printf(" ");
                if (rider[i].printfY[3] == 2)
                {
                    wprintf(L" 餐馆 %d %d", rider[i].printfX[2], rider[i].printfY[2]);
                    fprintf(fp, " 餐馆 %d %d", rider[i].printfX[2], rider[i].printfY[2]);
                }
                else if (rider[i].printfY[3] == 3)
                {
                    wprintf(L" 食客 %d %d", rider[i].printfX[2], rider[i].printfY[2]);
                    fprintf(fp, " 食客 %d %d", rider[i].printfX[2], rider[i].printfY[2]);
                }
                else if (rider[i].printfY[3] == 4)
                {
                    wprintf(L" 餐客%d %d", rider[i].printfX[2], rider[i].printfY[2]);
                    fprintf(fp, " 餐客%d %d", rider[i].printfX[2], rider[i].printfY[2]);
                }
            }
            rider[i].printfX[0] = 0; //对于待命骑手不再输出停靠
        }
        wprintf(L"；\n");
        fprintf(fp, "；\n");
    }
    // if (ACHORD != TOTORD)
    // {
    //     printf("\n");
    //     fprintf(fp, "\n");
    // }
    fclose(fp);
}

void Broke() //破产结算
{
    if (RUN == 1)
    {
        wprintf(L"因为资金小于0，破产。\n");
    }
    else if (RUN == 2)
    {

        wprintf(L"因为订单派送超时，吊销营业执照。\n");
    }
    else if (RUN == 3)
    {
        wprintf(L"因为程序出现异常，退出程序\n");
    }
}

int isArrive(int ordx, int ordy, int riderx, int ridery) //判断是否到达关键点
{
    int flag = 0;
    if (ordx == riderx && abs(ordy - ridery) == 1)
    {
        flag = 1;
    }
    else if (ordy == ridery && abs(ordx - riderx) == 1)
    {
        flag = 1;
    }
    return flag;
}

int plan(riderNode rider) //规划骑手路径
{
    int path;
    mapNode keyNode; //获得BFS后求得的关键点坐标
    keyNode.flag = 0;
    int curDir;
    int keyNode_x;
    int keyNode_y;
    int keyNextPosX;
    int keyNextPosY;
    rider = upDateRider(rider); //在优化路径前对原始订单链表更新数据
    curDir = rider.curDir;
    keyNode = BFS(rider);
    if (keyNode.flag == 1)
    {
        curDir = Direction(keyNode.x, keyNode.y, rider.ridPosCurX, rider.ridPosCurY);
        keyNextPosX = rider.HeadOrd->next->keyPosX; //首订单的坐标地址
        keyNextPosY = rider.HeadOrd->next->keyPosY;
        path = Dir_Path(curDir, keyNode.x, keyNode.y, keyNextPosX, keyNextPosY, rider);
    }
    else if (keyNode.flag == 0) //按原始路径派送
    {
        curDir = rider.curDir;
        keyNode_x = rider.HeadOrd->next->keyPosX;
        keyNode_y = rider.HeadOrd->next->keyPosY;
        if (rider.HeadOrd->next->state == 1)
        {
            keyNextPosX = rider.HeadOrd->next->gestPos_X;
            keyNextPosY = rider.HeadOrd->next->gestPos_Y;
        }
        else if (rider.HeadOrd->next->state == 2 && rider.HeadOrd->next->next != NULL)
        {
            keyNextPosX = rider.HeadOrd->next->next->ordPos_X;
            keyNextPosY = rider.HeadOrd->next->next->ordPos_Y;
        }
        else if (rider.HeadOrd->next->next == NULL) //如果只有一组订单，让骑手返回起始位置
        {
            keyNextPosX = POSX;
            keyNextPosY = POSY;
        }
        path = Dir_Path(curDir, keyNode_x, keyNode_y, keyNextPosX, keyNextPosY, rider);
    }
    return path;
}

mapNode BFS(riderNode rider) ///*对骑手位置三个位置范围内进行广搜，寻找关键点*///有问题此处的curPosx与curPosy为骑手位置，不能用isArrive判断
{
    mapNode keyNode;
    keyNode.flag = 0;
    int state = 0;
    int ismate = 0;
    int posx = rider.ridPosCurX;
    int posy = rider.ridPosCurY;
    int dir = rider.curDir;
    int dis = rider.curDis;
    int num1 = 1, num2 = 1;
    node[num1].x = posx;
    node[num1].y = posy;
    while (num1 <= num2 && num2 <= 10 && ismate == 0) //版本1.0只对两步内的区域进行搜索
    {
        if (node[num1].x % 2 == 0 && node[num1].y % 2 == 1) //水平道路
        {
            state = 0;
        }
        else if (node[num1].x % 2 == 1 && node[num1].y % 2 == 0) //垂直道路
        {
            state = 1;
        }
        for (int k = 0; k < 6 && ismate == 0; k++)
        {
            int path = 6 * state + k;
            int curPosX = node[num1].x + dic[path][0]; //state的转换！
            int curPosY = node[num1].y + dic[path][1];
            int curDir = Direction(curPosX, curPosY, posx, posy);
            int curDis = Manhattan(posx, posy, curPosX, curPosY);
            if (sameDir(dir, curDir) == 1 && curPosX != rider.HeadOrd->next->keyPosX && curPosY != rider.HeadOrd->next->keyPosX) //所找关键点不能为订单链表的首订单
            {
                if (curDis <= 5 && curDis <= dis && curPosX <= BLOCKRIGHT && curPosX >= BLOCKLEFT && curPosY <= BLOCKDOWN && curPosY >= BLOCKUP) //只在一定距离内搜索//并且保障该点不会出界
                {
                    num2++;
                    keyNode = search(curPosX, curPosY, num2, posx, posy, dir, dis, rider.no);
                    ismate = keyNode.flag;
                }
            }
        }
        num1++;
    }
    return keyNode; //flag为0则未找到关键点
}

mapNode search(int posx, int posy, int num2, int curPosX, int curPosY, int dir, int dis, int no) //对当前扫描点进行检查
{
    mapNode keyNode;
    node[num2].x = posx;
    node[num2].y = posy;
    if (posx % 2 == 0 && posy % 2 == 1) //水平道路
    {
        if (mapn[posx][posy + 1].flag == no)
        {
            keyNode.x = posx;
            keyNode.y = posy + 1;
            keyNode.flag = 1;
        }
        else if (mapn[posx][posy - 1].flag == no)
        {
            keyNode.x = posx;
            keyNode.y = posy - 1;
            keyNode.flag = 1;
        }
        else
        {
            keyNode.flag = 0;
        }
    }
    else if (posx % 2 == 1 && posy % 2 == 0) //垂直道路
    {
        if (mapn[posx + 1][posy].flag == no)
        {
            keyNode.x = posx + 1;
            keyNode.y = posy;
            keyNode.flag = 1;
        }
        else if (mapn[posx - 1][posy].flag == no)
        {
            keyNode.x = posx - 1;
            keyNode.y = posy;
            keyNode.flag = 1;
        }
        else
        {
            keyNode.flag = 0;
        }
    }
    else
    {
        printf("WRONG POSISTION IN BFS FOR NO:%d RIDER!\n", no);
        RUN = 3;
    }
    return keyNode;
}

int Dir_Path(int keyDir, int keyPosX, int keyPosY, int keyNextPosX, int keyNextPosY, riderNode rider) //根据方向规划下一步方向//最核心函数
{
    int path;
    int state = rider.state;
    int riderX = rider.ridPosCurX;
    int riderY = rider.ridPosCurY;
    int DisX = abs(keyPosX - riderX);
    int DisY = abs(keyPosY - riderY);
    int dir = Direction(keyNextPosX, keyNextPosY, keyPosX, keyPosY);
    if (state == 0) //水平道路
    {
        switch (keyDir)
        {
        case (3):                    //3为上
            if (riderX == BLOCKLEFT) //当在左边界只能右上
            {
                path = 3;
            }
            else if (riderX == BLOCKRIGHT) //当在右边界只能左上
            {
                path = 2;
            }
            else
            {
                if (dir == 1 || dir == 5 || dir == 6)
                {
                    path = 2; //左上
                }
                else if (dir == 2 || dir == 7 || dir == 8)
                {
                    path = 3; //右上
                }
                else
                {
                    path = 2; //左上
                }
            }
            break;
        case (4): //4为下
            if (riderX == BLOCKLEFT)
            {
                path = 5; //当在左边界只能右下
            }
            else if (riderX == BLOCKRIGHT)
            {
                path = 4; //当在右边界只能左下
            }
            else
            {
                if (dir == 1 || dir == 5 || dir == 6)
                {
                    path = 4; //左下
                }
                else if (dir == 2 || dir == 7 || dir == 8)
                {
                    path = 5; //右下
                }
                else
                {
                    path = 4; //左下
                }
            }
            break;
        case (5): //5为左上
            if (DisX <= 2)
            {
                if (DisY == 1)
                    if (dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 2; //左上
                    }
                    else //if (dir == 1 || dir == 2 || dir == 6 || dir == 4 || dir == 8)
                    {
                        path = 0; //左
                    }
                else
                {
                    path = 2; //左上
                }
            }
            else if (DisX > 2)
            {
                path = 0; //左
            }
            break;
        case (6): //6为左下
            if (DisX <= 2)
            {
                if (DisY == 1)
                {
                    if (dir == 1 || dir == 2 || dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 0; //左
                    }
                    else //if ( dir == 6 || dir == 4 || dir == 8)
                    {
                        path = 4; //左下
                    }
                }
                else
                {
                    path = 4; //左下
                }
            }
            else if (DisX > 2)
            {
                path = 0; //左
            }
            break;
        case (7): //7为右上
            if (DisX <= 2)
            {
                if (DisY == 1)
                {
                    if (dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 3; //右上
                    }
                    else
                    {
                        path = 1; //右
                    }
                }
                else
                {
                    path = 3; //右上
                }
            }
            else if (DisX > 2)
            {
                path = 1; //右
            }
            break;
        case (8): //8为右下
            if (DisX <= 2)
            {
                if (DisY == 1)
                {
                    if (dir == 1 || dir == 2 || dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 1; //右
                    }
                    else
                    {
                        path = 5; //右下
                    }
                }
                else
                {
                    path = 5; //右下
                }
            }
            else if (DisX > 2)
            {
                path = 1; //右
            }
            break;
        default:
            break;
        }
    }
    else if (state == 1) //竖直道路
    {
        switch (keyDir)
        {
        case (1):                  //1为左
            if (riderY == BLOCKUP) //当在上边界只能左下
            {
                path = 4;
            }
            else if (riderY == BLOCKDOWN) //当在下边界只能左上
            {
                path = 2;
            }
            else
            {
                if (dir == 5 || dir == 3 || dir == 7)
                {
                    path = 2; //左上
                }
                else
                {
                    path = 4; //左下
                }
            }
            break;
        case (2):                  //2为右
            if (riderY == BLOCKUP) //当在上边界只能右下
            {
                path = 5;
            }
            else if (riderY == BLOCKDOWN) //当在下边界只能右上
            {
                path = 3;
            }
            else
            {
                if (dir == 5 || dir == 3 || dir == 7)
                {
                    path = 3; //右上
                }
                else
                {
                    path = 5; //右下
                }
            }
            break;
        case (5): //5为左上
            if (DisY <= 2)
            {
                if (DisX == 1)
                {
                    if (dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 0; //上
                    }
                    else
                    {
                        path = 2; //左上
                    }
                }
                else
                {
                    path = 2; //左上
                }
            }
            else if (DisY > 2)
            {
                path = 0; //上
            }
            break;
        case (6): //6为左下
            if (DisY == 2)
            {
                if (DisX == 1)
                {
                    if (dir == 6 || dir == 4 || dir == 8)
                    {
                        path = 1; //下
                    }
                    else
                    {
                        path = 4; //左下
                    }
                }
                else
                {
                    path = 4; //左下
                }
            }
            else if (DisY > 2)
            {
                path = 1; //下
            }
            break;
        case (7): //7为右上
            if (DisY <= 2)
            {

                if (DisX == 1)
                {
                    if (dir == 5 || dir == 3 || dir == 7)
                    {
                        path = 0; //上
                    }
                    else
                    {
                        path = 3; //右上
                    }
                }
                else
                {
                    path = 3; //右上
                }
            }
            else if (DisY > 2)
            {
                path = 0; //上
            }
            break;
        case (8): //8为右下
            if (DisY <= 2)
            {
                if (DisX == 1)
                {
                    if (dir == 6 || dir == 4 || dir == 8)
                    {
                        path = 1; //下
                    }
                    else
                    {
                        path = 5; //右下
                    }
                }
                else
                {
                    path = 5; //右下
                }
            }
            else if (DisY > 2)
            {
                path = 1; //右下
            }
            break;
        default:
            break;
        }
    }
    path += 6 * state;
    return path;
}

riderNode addOrd(riderNode Rider, orderList HeadOrd) //将订单添加到骑手的订单队列中//按照该订单的可剩余时间排序
{
    orderNode *curOrd1 = HeadOrd->next;
    orderNode *curOrd2 = Rider.HeadOrd->next;
    ALLORD++;
    //if (Rider.isGod == 0)
    Rider.numOrd++;
    curOrd1->state = 1;
    curOrd1->keyPosX = curOrd1->ordPos_X;
    curOrd1->keyPosY = curOrd1->ordPos_Y;
    curOrd1->LossTime = curOrd1->startime + PASTIME;
    curOrd1->ord_geatDir = Direction(curOrd1->gestPos_X, curOrd1->gestPos_Y, curOrd1->ordPos_X, curOrd1->ordPos_Y);
    curOrd1->ord_geatDis = Manhattan(curOrd1->gestPos_X, curOrd1->gestPos_Y, curOrd1->ordPos_X, curOrd1->ordPos_Y);
    mapn[curOrd1->ordPos_X][curOrd1->ordPos_Y].flag = Rider.no;
    if (curOrd2 == NULL) //对于目前没有订单的骑手
    {
        HeadOrd->next = curOrd1->next;
        Rider.HeadOrd->next = curOrd1;
        curOrd1->next = NULL;
        Rider.next = curOrd1;
    }
    else
    {
        Rider.next->next = curOrd1; //先将新加入订单插入队尾
        Rider.next = Rider.next->next;
        HeadOrd->next = curOrd1->next;
        curOrd1->next = NULL;
        curOrd1 = HeadOrd->next;
    }
    Rider = upDateRider(Rider); //如果添加新订单对该骑手数据进行更新
    return Rider;
}

int Manhattan(int ordx, int ordy, int riderx, int ridery) //计算距离
{
    return abs(ordx - riderx) + abs(ordy - ridery);
}

int Direction(int ordx, int ordy, int riderx, int ridery) //判断必经点与骑手的方向关系//以后一组为基准点
{
    if (ordx - riderx > 0) //右方向
    {
        if (ordy - ridery > 0) //右下方向
            return 8;
        else if (ordy - ridery == 0) //右方向
            return 2;
        else //右上方向
        {
            return 7;
        }
    }
    else if (ordx - riderx < 0) //左方向
    {
        if (ordy - ridery > 0) //左下方向
            return 6;
        else if (ordy - ridery == 0) //左方向
            return 1;
        else //左上方向
        {
            return 5;
        }
    }
    else if (ordx - riderx == 0) //在同一竖直道路上
    {
        if (ordy - ridery > 0) //下方向
            return 4;
        else if (ordy - ridery < 0) //上方向
        {
            return 3;
        }
        else //两点位置相同
        {
            return 0;
        }
    }
    return -1;
}

void hireRider() //把雇佣的骑手加进空闲骑手结构体数组里
{
    orderList HeadOrd = NULL;
    HeadOrd = (orderNode *)malloc(sizeof(orderNode));
    MONEY -= 300;
    rider[RIDERNUM].no = RIDERNUM;
    rider[RIDERNUM].state = 0; //初始骑手在水平道路上
    rider[RIDERNUM].isGod = 0;
    rider[RIDERNUM].AchNum = -1;
    rider[RIDERNUM].OverNum = -1;
    rider[RIDERNUM].ridPosCurX = POSX;
    rider[RIDERNUM].ridPosCurY = POSY;
    rider[RIDERNUM].finishTime = 0;
    rider[RIDERNUM].numOrd = 0;
    rider[RIDERNUM].flag = 1;
    rider[RIDERNUM].HeadOrd = HeadOrd;
    rider[RIDERNUM].next = rider[RIDERNUM].HeadOrd;
    rider[RIDERNUM].HeadOrd->next = NULL;
    RIDERNUM++;
}

riderNode upDateRider(riderNode rider) //更新骑手数据
{
    int riderx = rider.ridPosCurX;
    int ridery = rider.ridPosCurY;
    orderList HeadOrd = rider.HeadOrd;
    orderNode *curOrd;
    if (rider.numOrd != 0)
    {
        rider = upDateOrd(rider);
        orderList HeadOrd = rider.HeadOrd;
        orderNode *curOrd = HeadOrd->next;
        int ordx = curOrd->keyPosX;
        int ordy = curOrd->keyPosY;
        rider.curDir = Direction(ordx, ordy, riderx, ridery);
        rider.curDis = Manhattan(ordx, ordy, riderx, ridery);
    }
    else if (rider.numOrd == 0)
    {
        rider.next = rider.HeadOrd;
        rider.finishTime = 0;
        rider.curDir = 0;
        rider.curDis = 0;

    }                                       //更新骑手的订单状态
    if (riderx % 2 == 1 && ridery % 2 == 0) //处于竖直道路
    {
        rider.state = 1;
    }
    else if (ridery % 2 == 1 && riderx % 2 == 0) //处于水平道路
    {
        rider.state = 0;
    }
    else
    {
        printf("EORROR! WRONG POSITION!\n");
    } //更新骑手位置状态
    curOrd = HeadOrd;
    while (curOrd->next)
    {
        curOrd = curOrd->next;
    }
    rider.next = curOrd;
    return rider;
}

riderNode upDateOrd(riderNode rider) //更新订单数据
{
    orderList HeadOrd = rider.HeadOrd;
    orderNode *curOrd = HeadOrd->next;
    orderNode *tempOrd = HeadOrd;
    rider.finishTime = 0; //每次更新一次骑手用时
    if (curOrd->state == 1)
    {
        rider.finishTime += Manhattan(curOrd->ordPos_X, curOrd->ordPos_Y, rider.ridPosCurX, rider.ridPosCurY) / 2;
    }
    else if (curOrd->state == 2)
    {
        rider.finishTime += Manhattan(curOrd->gestPos_X, curOrd->gestPos_Y, rider.ridPosCurX, rider.ridPosCurY) / 2;
    }
    while (curOrd)
    {
        if (curOrd->state == 1)
        {
            curOrd->keyPosX = curOrd->ordPos_X;
            curOrd->keyPosY = curOrd->ordPos_Y;
            rider.finishTime += curOrd->ord_geatDis / 2;
        }
        else if (curOrd->state == 2)
        {
            curOrd->keyPosX = curOrd->gestPos_X;
            curOrd->keyPosY = curOrd->gestPos_Y;
        }
        if (curOrd->next != NULL)
        {
            rider.finishTime += Manhattan(curOrd->gestPos_X, curOrd->gestPos_Y, curOrd->next->keyPosX, curOrd->next->keyPosY) / 2;
        }
        tempOrd = curOrd->next;
        curOrd = curOrd->next;
    }
    return rider;
}

int sameDir(int curDir, int keyDir) //判断两个方向是否相同
{
    int flag = 0;
    if (curDir == 5 && (keyDir == 1 || keyDir == 3 || keyDir == 5))
    {
        flag = 1;
    }
    else if (curDir == 6 && (keyDir == 1 || keyDir == 4 || keyDir == 6))
    {
        flag = 1;
    }
    else if (curDir == 7 && (keyDir == 2 || keyDir == 3 || keyDir == 7))
    {
        flag = 1;
    }
    else if (curDir == 8 && (keyDir == 2 || keyDir == 4 || keyDir == 8))
    {
        flag = 1;
    }
    else if (curDir == 1 && keyDir == 1)
    {
        flag = 1;
    }
    else if (curDir == 2 && keyDir == 2)
    {
        flag = 1;
    }
    else if (curDir == 3 && keyDir == 3)
    {
        flag = 1;
    }
    else if (curDir == 4 && keyDir == 4)
    {
        flag = 1;
    }
    return flag;
}

// void move(int x, int y, int a, int b, int condition)
// {
//     //printf("%d %d\n",a,abs(a));
//     if (condition == 0)
//     {
//         if (y % 2 == 1)
//         {
//             x = (x + 0.5) * SideLength;
//             y = (y + 0.5) * SideLength;
//             a *= SideLength;
//             b *= SideLength;
//         }
//         else
//         {
//             x = (x + 0.5) * SideLength;
//             y = (y - 0.5) * SideLength;
//             a *= SideLength;
//             b *= SideLength;
//         }
//         if (a != 0)
//         {
//             for (int k = 0; k < abs(a); delay_fps(60))
//             {
//                 setfillcolor(WHITE);
//                 floodfill(x, y, WHITE);
//                 if (a < 0)
//                     x--;
//                 else if (a > 0)
//                     x++;
//                 //printf("%d %d\n",a,abs(a));
//                 setcolor(YELLOW);
//                 circle(x, y, 0.49 * SideLength);
//                 setfillcolor(YELLOW);
//                 floodfill(x, y, YELLOW);
//                 k++;
//             }
//         }
//         if (b != 0)
//         {
//             for (int k = 0; k < abs(b); delay_fps(60))
//             {
//                 setfillcolor(WHITE);
//                 floodfill(x, y, WHITE);
//                 if (b < 0)
//                     y--;
//                 else if (b > 0)
//                     y++;
//                 setcolor(YELLOW);
//                 circle(x, y, 0.49 * SideLength);
//                 setfillcolor(YELLOW);
//                 floodfill(x, y, YELLOW);
//                 k++;
//             }
//         }
//     }
//     else
//     {
//         if (x % 2 == 1)
//         {
//             x = (x + 0.5) * SideLength;
//             y = (y + 0.5) * SideLength;
//             a *= SideLength;
//             b *= SideLength;
//         }
//         else
//         {
//             x = (x - 0.5) * SideLength;
//             y = (y + 0.5) * SideLength;
//             a *= SideLength;
//             b *= SideLength;
//         }
//         if (b != 0)
//         {
//             for (int k = 0; k < abs(b); delay_fps(60))
//             {
//                 setfillcolor(WHITE);
//                 floodfill(x, y, WHITE);
//                 if (b < 0)
//                     y--;
//                 else if (b > 0)
//                     y++;
//                 setcolor(YELLOW);
//                 circle(x, y, 0.49 * SideLength);
//                 setfillcolor(YELLOW);
//                 floodfill(x, y, YELLOW);
//                 k++;
//             }
//         }
//         if (a != 0)
//         {
//             for (int k = 0; k < abs(a); delay_fps(60))
//             {
//                 setfillcolor(WHITE);
//                 floodfill(x, y, WHITE);
//                 if (a < 0)
//                     x--;
//                 else if (a > 0)
//                     x++;
//                 setcolor(YELLOW);
//                 circle(x, y, 0.49 * SideLength);
//                 setfillcolor(YELLOW);
//                 floodfill(x, y, YELLOW);
//                 k++;
//             }
//         }
//     }
// }
