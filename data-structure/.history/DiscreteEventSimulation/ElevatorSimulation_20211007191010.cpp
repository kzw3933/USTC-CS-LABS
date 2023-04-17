//定义必要的数据类型
typedef enum Floor {Floor0,Floor1,Floor2,Floor3,Floor4} Floor;
typedef enum State {GoingUp,GoingDown,Idle} State;
typedef struct Elevator{
    Floor floor;
    int D1;
    int D2;
    int D3;
    State state;
}Elevator;
typedef 
