#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <map>
#include <algorithm>

#define INFILE "/home/jack/Desktop/exp1/PB20061338_柯志伟_exp1/astar/input/input5.txt"
#define INTMAX ((1<<31)-1)

typedef struct Point {
public:
    Point(int x, int y, bool locked) : x(x), y(y), locked(locked) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
public:
    int x;
    int y;
    bool locked; 
} Point;

typedef enum Direction {
    None,
    LUp,
    LDown,
    RUp,
    RDown
} Direction;

typedef struct Action {
public:
    Action() = default;
    Action(int x, int y, Direction direction): x(x), y(y), direction(direction) {}
    friend std::ostream& operator<<(std::ostream& os, const Action action) {
        os << "(" 
           << action.x << ", "
           << action.y << ", ";
        switch(action.direction) {
            case LUp:
                os << "LUp";
                break;
            case LDown:
                os << "LDown";
                break;
            case RUp:
                os << "RUp";
                break;
            case RDown:
                os << "RDown";
                break;
        }
        os << ")" << std::endl;
        return os;
    }
public:
    int x;
    int y;
    Direction direction;
} Action;

class State {
public:
    State(const std::vector<Point>& plates) : lock_plates(plates), prev(nullptr), action(0, 0, None), steps(0), f(0) {}
    
    State* get_next_state(Action action); 

    // 启发值函数
    int h() { return lock_plates.size()/3; } 

    bool success() { return lock_plates.empty(); }
public:
    int steps;  // 从初始状态到该状态的步数(每步代价为1)
    int cost;
    int f;
    std::vector<Point> lock_plates; // 类似稀疏矩阵的做法,lock_plates中只保存lock的位置
    struct State *prev;
    Action action; // 转移到这个状态时执行的action
};

class StateComparator {
public:
    bool operator()(const State* s1, const State* s2) const {
        return s1->cost > s2->cost;
    }
};

State* State::get_next_state(Action action) {
    Point point1(action.x, action.y, true);
    Point *point2, *point3;
    State *new_state_ptr = new State(lock_plates);
    new_state_ptr->prev = this;
    new_state_ptr->steps = steps + 1;
    new_state_ptr->action = action;
    auto it = std::find(new_state_ptr->lock_plates.begin(), new_state_ptr->lock_plates.end(), point1);
    if(it != new_state_ptr->lock_plates.end()) {
        new_state_ptr->lock_plates.erase(it);
    } else {
        new_state_ptr->lock_plates.push_back(Point(action.x, action.y, true));
    }
    switch(action.direction) {
        case LUp:  
            point2 = new Point(action.x-1, action.y, true);
            point3 = new Point(action.x, action.y-1, true);
            break;
        case LDown:
            point2 = new Point(action.x, action.y-1, true);
            point3 = new Point(action.x+1, action.y, true);
            break;
        case RUp:
            point2 = new Point(action.x-1, action.y, true);
            point3 = new Point(action.x, action.y+1, true);
            break;
        case RDown:
            point2 = new Point(action.x+1, action.y, true);
            point3 = new Point(action.x, action.y+1, true);
            break;
        default:
            break;
    }
    it = std::find(new_state_ptr->lock_plates.begin(), new_state_ptr->lock_plates.end(), *point2);
    if(it != new_state_ptr->lock_plates.end()) {
        new_state_ptr->lock_plates.erase(it);
    } else {
        new_state_ptr->lock_plates.push_back(*point2);
    }
    it = std::find(new_state_ptr->lock_plates.begin(), new_state_ptr->lock_plates.end(), *point3);
    if(it != new_state_ptr->lock_plates.end()) {
        new_state_ptr->lock_plates.erase(it);
    } else {
        new_state_ptr->lock_plates.push_back(*point3);
    }
    new_state_ptr->cost = new_state_ptr->steps + new_state_ptr->h();
    return new_state_ptr;
}

class Agent {
public:
    
    Agent(State *state, int n);
    ~Agent() = default;

    // 扩展边界 frontier, 由于行动决定了下一步的状态, 使用行动的集合代表 frontier
    void extend(State* state);

    // // 进行下一步
    // void next();

    bool RBFS(State* state, int flimit);

    State* current() { return current_state; }

    // 判断是否到达全0状态
    bool success() { return current_state->success(); } 


private:
    int N;
    State *current_state;
    std::vector<Action>actions;
    std::priority_queue<State*, std::vector<State*>, StateComparator> frontiers;
};

void Agent::extend(State* state) {
    frontiers.push(state);
}

// /**
//  * 直接使用A*算法
// */
// void Agent::next() {
//     current_state = frontiers.top();
//     frontiers.pop();
//     for(auto &action: actions) {
//         State* state = current_state->get_next_state(action);
//         frontiers.push(state);
//     }
// }

inline int max(int a, int b) {
    return a > b ? a : b;
}

inline int min(int a, int b) {
    return a < b ? a : b;
}

bool Agent::RBFS(State* state, int flimit) {
    current_state = state;
    if(success()) return true;
    std::vector<State*> vec;
    for(auto &action: actions) {
        State* st = current_state->get_next_state(action);
        vec.push_back(st);
    }
    if(vec.empty()) {
        state->f = INTMAX; 
        return false;
    }
    for(auto &st:vec) {
        st->f = max(st->cost, state->f);
    }
    while(true) {
        State* best;
        State* alternative;
        for(auto &st:vec) {
            if(best == nullptr) {
                best = st;
            } else if(alternative == nullptr) {
                if(st->f < best->f) {
                    alternative = best;
                    best = st;
                } else {
                    alternative = st;
                }
            } else {
                if(st->f < best->f) {
                    alternative = best;
                    best = st;
                } else if(st->f < alternative->f) {
                    alternative = st;
                }
            }
        }
        if(best->f > flimit) {
            current_state->f = best->f;
            return false;
        }
        bool success = RBFS(best, min(flimit, alternative->f));
        if(success) return true;
        else current_state = state;
    }
}



Agent::Agent(State *state, int n): current_state(state), N(n) {
    // 初始化所有可能的行动,针对每个状态可能的行动是不变的
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            if(i == 0 && j == 0) {
                actions.push_back(Action(i, j, RDown));
                continue;
            } else if(i == 0 && j == N-1) {
                actions.push_back(Action(i, j, LDown));
                continue;
            } else if(i == N - 1 && j == 0) {
                actions.push_back(Action(i, j, RUp));
                continue;
            } else if(i == N - 1 && j == N-1) {
                actions.push_back(Action(i, j, LUp));
                continue;
            } else if(i == 0) {
                actions.push_back(Action(i, j, LDown));
                actions.push_back(Action(i, j, RDown));
                continue;
            } else if(i == N-1) {
                actions.push_back(Action(i, j, LUp));
                actions.push_back(Action(i, j, RUp));
                continue;
            } else if(j == 0) {
                actions.push_back(Action(i, j, RUp));
                actions.push_back(Action(i, j, RDown));
                continue;
            } else if(j == N-1) {
                actions.push_back(Action(i, j, LUp));
                actions.push_back(Action(i, j, LDown));
                continue;
            } else {
                actions.push_back(Action(i, j, RUp));
                actions.push_back(Action(i, j, RDown));
                actions.push_back(Action(i, j, LUp));
                actions.push_back(Action(i, j, LDown));
            }
            
        }
    }
    frontiers.push(state);
}

int main() {
    int N;
    std::ifstream fin(INFILE);
    std::vector<Point> init_states;
    Agent *agent;
    if(!fin.is_open()) {
        std::cerr << "Failed to open file " << INFILE << std::endl;
        return -1;
    }
    // 解析输入文件并初始化agent
    fin >> N;
    int val;
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < N; j++) {
            fin >> val;
            if(val == true) {
                init_states.push_back(Point(i, j, true));
            } 
        }
    }
    agent = new Agent(new State(init_states), N);
    agent->current()->cost = agent->current()->h();
    agent->current()->f = agent->current()->cost;
    agent->RBFS(agent->current(), INTMAX);
    

    // 打印路径
    for(auto iter = agent->current(); iter != NULL; iter = iter->prev) {
        std::cout << iter->action;
    }
}



