#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <algorithm>

#define INFILE "/home/jack/Desktop/exp1/PB20061338_柯志伟_exp1/csp/input/input8.txt"

typedef struct Var {
    int d;
    int s;
    std::vector<int> vals;
} Var;


class CSP {
public:
    CSP(int n, int d, int s, std::vector<bool> requests): 
        N(n), D(d), S(s), score(0), best_shifts(nullptr), requests(requests), shifts(d*s, -1){}
    
    ~CSP() = default;

    int get_score();

    void schedule();

    bool constraint_propagation();

    Var* get_min_constraints_var();

    bool success() { return var_stack.size() == D * S && constraint_validation(); }

    void init_vars();
    int count_work_shifts(int index);
    void print();

    bool constraint_validation();


private:
    int N; // 宿管阿姨数量
    int D; // 值班天数
    int S; // 每⽇轮班次数
    int score; // 当前排班表得分
    std::vector<int> *best_shifts; // 目前得分最好的排班表
    std::vector<bool> requests; // 轮班请求
    std::vector<Var> vars; // 变量表
    std::vector<int> shifts; // (d,s)是否已经被排班
    std::stack<Var*> var_stack; // 存放当前对变量赋值走过的路径
};

int CSP::get_score() {
    int sc = 0;
    for(int i = 0; i < D*S; i++) {
        int n = shifts[i];
        if(requests[n*D*S+i]) sc++;
    }
    return sc;
}

Var* CSP::get_min_constraints_var() {
    Var *res = nullptr;
    int min = N;
    for(int i = 0; i < D * S; i++) {
        if((vars[i].vals.size()< min || res == nullptr) && shifts[i] < 0) {
            min = vars[i].vals.size();
            res = &vars[i];
        }
    }
    return res;
}

void CSP::init_vars() {
    std::vector<int> vals(N, 0);
    for(int i = 0; i < N; i++) {
        vals[i] = i;
    }
    for(int i = 0; i < D; i++) {
        for(int j = 0; j < S; j++) {
            vars.push_back(Var{i, j, vals});
        } 
    }
}
int CSP::count_work_shifts(int index){
    int count = 0;
    for(int i = 0; i < D; i++) {
        for(int j = 0; j < S; j++) {
            if(shifts[i*S+j] == index) {
                count++;
            }
        }
    }
    return count;
}

bool CSP::constraint_propagation() {
    std::vector<int> vals(N, 0);
    for(int i = 0; i < N; i++) {
        vals[i] = i;
    }
    for(int i = 0; i < D; i++) {
        for(int j = 0; j < S; j++) {
            if(shifts[i*S+j] >= 0) continue;
            vars[i*S+j].vals = vals;
            // 同一个宿管阿姨不能连续工作两个班次
            int left = i*S+j-1;
            int right = i*S+j+1;
            if(left >= 0 && shifts[left] >= 0) {
                auto iter = std::find(vars[i*S+j].vals.begin(), vars[i*S+j].vals.end(), shifts[left]);
                if(iter != vars[i*S+j].vals.end()) {
                    vars[i*S+j].vals.erase(iter);
                }
            }
            if(right < D*S && shifts[right] > 0) {
                auto iter = std::find(vars[i*S+j].vals.begin(), vars[i*S+j].vals.end(), shifts[right]);
                if(iter != vars[i*S+j].vals.end()) {
                    vars[i*S+j].vals.erase(iter);
                }
            }
            if(vars[i*S+j].vals.size() == 0) return false;
        }
    }
    // 公平性
    int lack = 0;
    for(int i = 0; i < N; i++) {
        if(count_work_shifts(i) < D*S/N) {
            lack += (D*S/N - count_work_shifts(i)); 
        }
    }
    if(lack > D*S - var_stack.size()) return false;
    return true;
}

void CSP::print() {
    if(best_shifts == nullptr) {
        std::cout << "no solution" << std::endl;
        return;
    }
    for(int i = 0; i < D; i++) {
        for(int j = 0; j < S; j++) {
            std::cout << "("
                 << i << ", "
                 << j << ", "
                 << (*best_shifts)[i*S+j] 
                 << ")" << "\t";
        }
        std::cout << std::endl;
    }
}

/**
 * 验证当前的解决方案满足所有约束
*/
bool CSP::constraint_validation() {
    // 同一个宿管阿姨不能连续工作两个班次
    for(int i = 0; i < D; i++) {
        for(int j = 0; j < S; j++) {
            int index = i*S + j;
            int right = index+1;
            if(right < D*S) {
                if(shifts[index] == shifts[right]) return false;
            }
        }
    }
    // 公平性
    for(int i = 0; i < N; i++) {
        if(count_work_shifts(i) < D*S/N) {
            return false;
        }
    }
    return true;
}

// /**
//  * 递归实现CSP问题的变量集变量逐个求解
// */
// void CSP::schedule() {
//     Var* var = get_min_constraints_var();
//     num++;
//     int index = var->d * S + var->s;
//     for(auto val : var->vals) {
//         shifts[index] = val;
//         if(success()) {
//             if(best_shifts == nullptr) {
//                 best_shifts = new std::vector<int>(shifts);
//                 score = get_score();
//             }  else if(get_score() > score) {
//                 delete best_shifts; 
//                 best_shifts = new std::vector<int>(shifts);
//                 score = get_score();
//             }
//             continue;
//         }
//         if(!constraint_propagation()) {
//             continue;
//         }
//         schedule();
//     }
//     num--;
//     shifts[index] = -1;
// }

void CSP::schedule() {
    // 将第一个var入栈
    var_stack.push(&vars[0]);
    while(!var_stack.empty()) {
        Var *cur_var = var_stack.top();
        int index = cur_var->d * S + cur_var->s;
        if(cur_var->vals.empty()) {
            var_stack.pop();
            shifts[index] = -1;
            continue;
        }
        shifts[index] = cur_var->vals.back();
        cur_var->vals.pop_back();
        if(success()) {
            if(best_shifts == nullptr) {
                best_shifts = new std::vector<int>(shifts);
                score = get_score();
            }  else if(get_score() > score) {
                delete best_shifts; 
                best_shifts = new std::vector<int>(shifts);
                score = get_score();
            }
            std::cout << "The score is: " << score << std::endl;
        } else {
            if(!constraint_propagation()) continue;
            Var* next_var = get_min_constraints_var();
            var_stack.push(next_var);
        }
    }
}

int main() {
    int N, D, S;
    char comma;
    std::ifstream fin(INFILE);
    CSP* csp;
    fin >> N >> comma >> D >> comma >> S;
    int a = 0;
    int tmp;
    std::vector<bool> requests(N*D*S, false);
    for(int i = 0; i < N; i++) {
        for(int j = 0; j < D; j++) {
            for(int k = 0; k < S; k++) {      
                fin >> tmp;
                if(k < S-1) fin >> comma;
                if(tmp > 0) requests[i*D*S+j*S+k] = true;
            }
        }
    }
    csp = new CSP(N, D, S, requests);
    csp->init_vars();
    csp->schedule();
    csp->print();
}