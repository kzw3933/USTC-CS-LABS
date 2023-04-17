//定义必要的数据类型
typedef enum Floor {Floor0,Floor1,Floor2,Floor3,Floor4} Floor;
typedef enum State {GoingUp,GoingDown,Idle} State;
typedef struct Elevator{
    Floor floor;          //电梯的当前位置
    int D1;                 //值为非0时，有人在进入或离开电梯
    int D2;                 //如果电梯在某层等候300t以上
    int D3;                    //值为非0时，电梯门开着但没人上电梯
    State state;            //电梯的状态
}Elevator;
typedef struct Passenger{
    Floor infloor;
    Floor outfloor;
    int GiveupTime;
    int InterTime;    //距下一个人到达的时间间隔
}Passenger;
typedef struct Buildings{
    int CallUp[5];          //分别对应每层楼的上升按钮和下降按钮是否被按亮
    int CallDown[5];
}Buildings;

