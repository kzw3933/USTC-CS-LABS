
#include <iostream>
#include<vector>
#include <chrono>
#include"z3++.h"


using namespace z3;
using namespace std;

void solve_nqueen_smt(int n) {
    auto start = std::chrono::high_resolution_clock::now();
    cout << "The num of queen is " << n << endl;
    context ctx;
    solver slv(ctx);
    vector<expr> queen_cols;
    for (int i = 0; i < n; i++) {
        queen_cols.push_back(ctx.int_const(("queen_" + to_string(i)).c_str()));
    }

    expr_vector temp_expr_vec(ctx);
    for (int i = 0; i < n; i++) {
        slv.add((queen_cols[i] >= 1 && queen_cols[i] <= n));
        temp_expr_vec.push_back(queen_cols[i]);
    }
    slv.add(distinct(temp_expr_vec));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < i; j++) {
            slv.add((queen_cols[j] != (queen_cols[i] + i-j)) && (queen_cols[j] != (queen_cols[i] + j-i)));
        }
    }
    auto res = slv.check();
    cout << res << endl;
    switch (res) {
    case unsat:
        cout << "The problem is unsat" << endl;
        break;
    case sat: {
        model m = slv.get_model();
        for (int i = 0; i < n; i++) {
            cout << m.eval(ctx.int_const(("queen_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;
    }
            break;
    case unknown:
        cout << "The problem is unknown" << endl;
        break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";


}


void solve_nqueen_puresat(int n) {
    auto start = std::chrono::high_resolution_clock::now();
    cout << "The num of queen is " << n << endl;
    context ctx;
    solver slv(ctx);
    vector<vector<expr>> queens;

    
    for (int i = 0; i < n; i++) {
        vector<expr> temp;
        for (int j = 0; j < n; j++) {
            temp.push_back(ctx.bool_const(("queen_" + to_string(i) + "_" + to_string(j)).c_str()));
        }
        queens.push_back(temp);     
    }
    vector<expr> rows, cols;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // 增加行列约束: 至少一个为真
            if (j == 0) rows.push_back(queens[i][0]);
            else rows[i] = rows[i] || queens[i][j];
            if (i == 0) cols.push_back(queens[0][j]);
            else cols[j] = cols[j] || queens[i][j];
            if (i == n - 1) slv.add(cols[j]);
        }
        slv.add(rows[i]);
    }
    // 增加行列约束: 至多一个为真
    expr temp_expr_1(ctx);
    for (int i = 0; i < n; i++) {
        for (int j = 1; j < n; j++) {
            for (int k = 0; k < j; k++) {
                if (i == 0 && j == 1 && k == 0) temp_expr_1 = !queens[0][1] || !queens[0][0];
                else temp_expr_1 = (temp_expr_1) && (!queens[i][j] || !queens[i][k]);
            }
        }
    }
    slv.add(temp_expr_1);

    for (int j = 0; j < n; j++) {
        for (int i = 1; i < n; i++) {
            for (int k = 0; k < i; k++) {
                if (j == 0 && i == 1 && k == 0) temp_expr_1 = !queens[1][0] || !queens[0][0];
                else temp_expr_1 = (temp_expr_1) && (!queens[i][j] || !queens[k][j]);
            }
        }
    }
    slv.add(temp_expr_1);
    // 增加对角约束
    expr temp_expr_2(ctx);

    for (int i = 1; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bool init = false;
            for (int k = i - 1; k >= 0 && (j + k - i) >= 0; k--) {
                if (k == i - 1) temp_expr_1 = !queens[k][j+k-i] || !queens[i][j];
                else temp_expr_1 = (temp_expr_1) && (!queens[k][j + k - i] || !queens[i][j]);
                init = true;
            }
            for (int k = i + 1; k < n && (i + j - k) >= 0 ; k++) {
                if (!init) {
                    temp_expr_1 = (!queens[k][i + j - k] || !queens[i][j]);
                }
                else {
                    temp_expr_1 = (temp_expr_1) && (!queens[k][i + j - k] || !queens[i][j]);
                }          
            }
            if (i == 1 && j == 0) temp_expr_2 = temp_expr_1;
            else temp_expr_2 = (temp_expr_2) && (temp_expr_1);
        }
    }


    slv.add(temp_expr_2);
    
    auto res = slv.check();
    cout << res << endl;
    switch (res) {
        case unsat:
            cout << "The problem is unsat" << endl;
            break;
        case sat: {
            model m = slv.get_model();
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    cout << m.eval(ctx.bool_const(("queen_" + to_string(i) + "_" + to_string(j)).c_str())) << "\t" ;
                }
                cout << endl;
            }
        }        
            break;
        case unknown:
            cout << "The problem is unknown" << endl;
            break;
    } 
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}


void get_num_base2(int num, vector<int>& num_vec) {
    if (num == 0) num_vec.push_back(0);
    else {
        while (num != 0) {
            num_vec.push_back(num % 2);
            num = num / 2;
        }
    }
    return;
}



void solve_add(int a, int b) {
    context ctx;
    solver slv(ctx);

    vector<int> a_vec, b_vec;
    get_num_base2(a, a_vec);
    get_num_base2(b, b_vec);
    int max_digit_num = a_vec.size() > b_vec.size() ? a_vec.size() : b_vec.size();
    
    vector<expr> a_bools, b_bools, c_bools, d_bools;
    for (int i = 0; i < max_digit_num; i++) {
        a_bools.push_back(ctx.bool_const(("a_" + to_string(i)).c_str()));
        b_bools.push_back(ctx.bool_const(("b_" + to_string(i)).c_str()));
        c_bools.push_back(ctx.bool_const(("c_" + to_string(i)).c_str()));
        d_bools.push_back(ctx.bool_const(("d_" + to_string(i)).c_str()));
    }
    c_bools.push_back(ctx.bool_const(("c_" + to_string(c_bools.size())).c_str()));

    expr temp(ctx);
    for (int i = 0; i < max_digit_num; i++) {
        if (i == 0) {
            temp = (a_vec[0] == 0) ? (!a_bools[0]) : (a_bools[0]);
            temp = (b_vec[0] == 0) ? (temp && (!b_bools[0])) : (temp && (b_bools[0]));
        }
        else {
            if (i > a_vec.size() - 1) {
                temp = (temp) && (!a_bools[i]);
            }
            else {
                if (a_vec[i] == 0) {
                    temp = (temp) && (!a_bools[i]);
                }
                else {
                    temp = (temp) && (a_bools[i]);
                }
            }
            if (i > b_vec.size() - 1) {
                temp = (temp) && (!b_bools[i]);
            }
            else {
                if (b_vec[i] == 0) {
                    temp = (temp) && (!b_bools[i]);
                }
                else {
                    temp = (temp) && (b_bools[i]);
                }
            }
        }
        slv.add(temp);
    }
    slv.add(!c_bools[0]);
    for (int i = 0; i < max_digit_num; i++) {
        slv.add(d_bools[i] == (a_bools[i] == (b_bools[i] == c_bools[i])));
        if (i != max_digit_num - 1) {
            slv.add(c_bools[i + 1] == ((a_bools[i] && b_bools[i]) || (a_bools[i] && c_bools[i]) || (b_bools[i] && c_bools[i])));
        }
        else {
            slv.add(!c_bools[i + 1]);
        }     
    }

    auto res = slv.check();
    cout << res << endl;
    switch (res) {
    case unsat:
        cout << "The problem is unsat" << endl;
        break;
    case sat: {
        model m = slv.get_model();
        cout << "a is: " << a << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("a_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;
        cout << "b is: " << b << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("b_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;
        cout << "sum is: " << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("d_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;
    }
            break;
    case unknown:
        cout << "The problem is unknown" << endl;
        break;
    }
}


void solve_sub(int a, int b) {
    context ctx;
    solver slv(ctx);

    vector<int> a_vec, b_vec;
    get_num_base2(a, a_vec);
    get_num_base2(b, b_vec);


    int max_digit_num = a_vec.size() > b_vec.size() ? a_vec.size() : b_vec.size();

    vector<expr> a_bools, b_bools, c_bools, d_bools;
    for (int i = 0; i < max_digit_num; i++) {
        a_bools.push_back(ctx.bool_const(("a_" + to_string(i)).c_str()));
        b_bools.push_back(ctx.bool_const(("b_" + to_string(i)).c_str()));
        c_bools.push_back(ctx.bool_const(("c_" + to_string(i)).c_str()));
        d_bools.push_back(ctx.bool_const(("d_" + to_string(i)).c_str()));
    }
    c_bools.push_back(ctx.bool_const(("c_" + to_string(c_bools.size())).c_str()));

    expr temp(ctx);
    for (int i = 0; i < max_digit_num; i++) {
        if (i == 0) {
            temp = (a_vec[0] == 0) ? (!a_bools[0]) : (a_bools[0]);
            temp = (b_vec[0] == 1) ? (temp && (!b_bools[0])) : (temp && (b_bools[0]));
        }
        else {
            if (i > a_vec.size() - 1) {
                temp = (temp) && (!a_bools[i]);
            }
            else {
                if (a_vec[i] == 0) {
                    temp = (temp) && (!a_bools[i]);
                }
                else {
                    temp = (temp) && (a_bools[i]);
                }
            }
            if (i > b_vec.size() - 1) {
                temp = (temp) && (b_bools[i]);
            }
            else {
                if (b_vec[i] == 1) {
                    temp = (temp) && (!b_bools[i]);
                }
                else {
                    temp = (temp) && (b_bools[i]);
                }
            }
        }
        slv.add(temp);
    }
    slv.add(c_bools[0]);

    for (int i = 0; i < max_digit_num; i++) {
        slv.add(d_bools[i] == (a_bools[i] == (b_bools[i] == c_bools[i])));
        if (i != max_digit_num - 1) {
            slv.add(c_bools[i + 1] == ((a_bools[i] && b_bools[i]) || (a_bools[i] && c_bools[i]) || (b_bools[i] && c_bools[i])));
        }
        else {
            slv.add(!c_bools[i + 1]);
        }
    }
    auto start = std::chrono::high_resolution_clock::now();
    auto res = slv.check();
    cout << res << endl;
    switch (res) {
    case unsat:
        cout << "The problem is unsat" << endl;
        break;
    case sat: {
        model m = slv.get_model();
        cout << "a is: " << a << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("a_" + to_string(i)).c_str())) <<"\t";
        }
        cout << endl;
        cout << "b is: " << b << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("b_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;
        cout << "sum is: " << "\t";
        for (int i = max_digit_num - 1; i >= 0; i--) {
            cout << m.eval(ctx.bool_const(("d_" + to_string(i)).c_str())) << "\t";
        }
        cout << endl;

    }
            break;
    case unknown:
        cout << "The problem is unknown" << endl;
        break;
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Elapsed time: " << elapsed.count() << " s\n";
}



int main() {

    bool exit = false;
    int choice;
    int n;
    int a, b;
    while (!exit) {
        cout << "1: 使用纯SAT求解N皇后问题" << endl;
        cout << "2: 使用SMT求解N皇后问题" << endl;
        cout << "3: 使用SAT求解两数相加问题" << endl;
        cout << "4: 使用SAT求解两数相减问题" << endl;
        cout << "5: 退出" << endl;
        cout << "请输入选择: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "请输入问题规模(N): ";
            cin >> n;
            solve_nqueen_puresat(n);
            cout << endl;
            break;
        case 2:
            cout << "请输入问题规模(N): ";
            cin >> n;
            solve_nqueen_smt(n);
            cout << endl;
            break;
        case 3:
            cout << "请输入两个数字(a b): ";
            cin >> a >> b;
            solve_add(a, b);
            cout << endl;
            break;
        case 4:
            cout << "请输入两个数字(a b): ";
            cin >> a >> b;
            solve_sub(a, b);
            cout << endl;
            break;
        case 5:
            exit = true;
            break;
        default:
            break;
        }
        
    }
    Z3_finalize_memory();
    return 0;
}