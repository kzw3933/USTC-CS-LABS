typedef struct {
    int Occurtime;
    int type;      //0代表客户到达，1代表客户离开       
}Event;    //定义事件类型
typedef struct EventNode{
    Event event;
    struct EventNode *next;    
} EventNode,*EventList;   //定义事件表节点
typedef struct {
    int ArrivalTime;   //到达时间
    int Duration;    //办理事务要花的时间
    int money;         //交易量
    int type;    //事务类型，0为存钱，1为取钱
}Customer;    //定义客户类型
typedef struct QNode{
    Customer customer;
    struct QNode *next;
} QNode,*QuenePtr;   //定义队列节点
typedef struct {
    QuenePtr front;
    QuenePtr rear;
}QueneList;   //定义队列类型
