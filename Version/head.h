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
extern int ALLORD;   //总结单数
extern int TIME;     //记录时间
extern int MONEY;    //成本为1000
extern int RIDERNUM; //雇佣骑手数量
extern int ACHORD;   //订单完成数
extern int OVERORD;  //订单超时数
extern int RUN;      //判断能否继续经营，破产或吊销执照均不能继续营业；
extern int ENDTIME;  //统计最后一个订单的发单时间
extern int TOTORD;   //统计样例中订单总数
extern int mouseFLAG, count;
extern int FLAG;

//定义所用子函数
riderNode addOrd(riderNode rider, orderList HeadOrd); //将订单添加到骑手的订单队列中

int Manhattan(int ordx, int ordy, int gestx, int gesty); //计算距离

int Direction(int ordx, int ordy, int riderx, int ridery); //判断必经点与骑手的方向关系

int sameDir(int curDir, int keyDir); //判断两个方向是否相同

riderNode upDateRider(riderNode rider); //更新骑手数据

riderNode upDateOrd(riderNode rider); //更新订单数据

void hireRider(); //把雇佣的骑手加进空闲骑手结构体数组里

orderList input(); //输入样例

orderList CreatOrdList(orderList headPtr1); //创建待处理订单的链表

void perSend(orderList HeadOrd); //对待命骑手做先知处理

void send(orderList HeadOrd); //将订单派送给骑手

int plan(riderNode rider); //规划骑手路径

void Broke(); //破产结算

int Dir_Path(int keyDir, int keyPosX, int keyPosY, int keyNextPosX, int keyNextPosY, riderNode rider); //根据方向规划下一步方向; //根据方向规划下一步方向

int isArrive(int ordx, int ordy, int riderx, int ridery); //判断是否到达关键点

mapNode BFS(riderNode rider); //对骑手位置三个位置范围内进行广搜，寻找关键点

mapNode search(int posx, int posy, int num2, int curPosX, int curPosY, int dir, int dis, int no); //对当前扫描点进行检查

int reason(int posCurx, int posCury, int posNextx, int posNexty); //计算更改路径是否合理

riderNode execute(int path, riderNode rider); //执行已规划路径

void output(); //输出结果

void picture(); //图文输出结果

void move(int x, int y, int a, int b, int condition);

unsigned __stdcall CreatOrd(void *pArguments);

unsigned __stdcall printflist(void *pArguments);

unsigned __stdcall pro(void *pArguments);

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