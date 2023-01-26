#include <iostream>
#include <fstream>
#include <cassert>
#include <vector>
#include <map>


using namespace std;

#define red 0
#define black 1

struct Interval {
    int low;
    int high;
    Interval() : low(0), high(0) {}
    Interval(int low,int high) : low(low),high(high) {}
    
    bool overlap(Interval interval) {
            return (this->low <= interval.high && interval.low <= this->high);
    }
};


struct interval_tree_node {
    int color;
    int key;
    int max;
    Interval interval;
    interval_tree_node* par;
    interval_tree_node* lchird;
    interval_tree_node* rchird;

    interval_tree_node(int k, int high, int low, int max, int col, interval_tree_node* p=nullptr, interval_tree_node* lch=nullptr, interval_tree_node* rch=nullptr ) :
        key(k), par(p), lchird(lch), rchird(rch), color(col),interval(low, high),max(max){}

    static void free_node_iterate(interval_tree_node* nodeptr);
};

class INTTree {

    public:
        INTTree() {
            root = nil;
        }

        ~INTTree() {
            rbdestroy();
        }

        static interval_tree_node* nil;

    public:
        void rbtransplant(interval_tree_node* u, interval_tree_node* v);
        interval_tree_node* rbsearch(int key);
        void rbleftRotate(interval_tree_node* x);
        void rbrightRotate(interval_tree_node* x);
        void rbinsert(interval_tree_node* nodeptr);
        void rbinsert_fixup(interval_tree_node* x);
        void rbdelete(interval_tree_node* nodeptr);
        void rbdelete_fixup(interval_tree_node* x);
        void rbdestroy();
        interval_tree_node* rbmin(interval_tree_node* nodeptr);
        interval_tree_node* get_root() { return root; }

        bool rb_tree_check();

        void interval_insert(interval_tree_node* nodeptr) { rbinsert(nodeptr); }
        void interval_delete(interval_tree_node* nodeptr) { rbdelete(nodeptr); }
        interval_tree_node* interval_search(Interval interval);

        void interval_fixup(interval_tree_node* x);  

        void inorder_visit(interval_tree_node* node);

        void inttree_inorder_visit() {
            fout.open(fileout_path, ios::app);  
            inorder_visit(this->root); 
            fout.close();
        }

        void init_fout(string fpath) { fileout_path = fpath; }

        bool inttree_check();   
    private:
        interval_tree_node* root;  
        string fileout_path; 
        ofstream fout;
};

interval_tree_node* INTTree::nil = nil = new interval_tree_node(0, 0, 0, 0, black);


void INTTree::rbtransplant(interval_tree_node* u, interval_tree_node* v) {
    if(u->par == nil) 
        this->root = v;
    else if(u == u->par->lchird) 
        u->par->lchird = v;
    else 
        u->par->rchird = v;
    v->par = u->par;    
}

interval_tree_node* INTTree::rbsearch(int key) {
    interval_tree_node* prev = this->root;

    while(prev != nil) {
        if(prev->key == key)
            return prev;
        else if(prev->key > key)
            prev = prev->lchird;
        else 
            prev = prev->rchird;
    }
}

void INTTree::rbleftRotate(interval_tree_node* x) {
    interval_tree_node* y = x->rchird;
    x->rchird = y->lchird;
    if (y->lchird != nil)
        y->lchird->par = x;
    y->par = x->par;

    if(x->par == nil)
        this->root = y;
    else if(x == x->par->lchird)
        x->par->lchird = y;
    else 
        x->par->rchird = y;
    y->lchird = x;
    x->par = y;

    // fixup interval max
    interval_fixup(x);
    interval_fixup(y);
    
}

void INTTree::rbrightRotate(interval_tree_node* x) {
    interval_tree_node* y = x->lchird;
    x->lchird = y->rchird;
    if (y->rchird != nil)
        y->rchird->par = x;
    y->par = x->par;

    if(x->par == nil)
        this->root = y;
    else if(x == x->par->rchird)
        x->par->rchird = y;
    else 
        x->par->lchird = y;
    y->rchird = x;
    x->par = y;

    // fixup interval max
    interval_fixup(y);
    interval_fixup(x);
}

void INTTree::rbinsert(interval_tree_node* nodeptr){
    interval_tree_node* y = nil;
    interval_tree_node* x = this->root;

    while(x != nil) {
        y = x;
        if(nodeptr->key < x->key) 
            x = x->lchird;
        else 
            x = x->rchird;
    }
    nodeptr->par = y;

    if(y == nil)
        this->root = nodeptr;
    else if(nodeptr->key < y->key) 
        y->lchird = nodeptr;
    else 
        y->rchird = nodeptr;

    nodeptr->lchird = nil;
    nodeptr->rchird = nil;
    nodeptr->color = red;

    interval_tree_node * z = y;

    while(z != nil) {
        interval_fixup(z);
        z = z->par;
    }
    rbinsert_fixup(nodeptr);
}

void INTTree::rbinsert_fixup(interval_tree_node* x){
    interval_tree_node* y;
    while(x->par->color == red) {
        if(x->par == x->par->par->lchird) {
            y = x->par->par->rchird;
            if(y->color == red) {
                x->par->color = black;
                y->color = black;
                x->par->par->color = red;
                x = x->par->par;
            }
            else {
                if(x == x->par->rchird) {
                    x = x->par;
                    rbleftRotate(x);
                }
                x->par->color = black;
                x->par->par->color = red;
                rbrightRotate(x->par->par);
            }
        }
        else {
            y = x->par->par->lchird;
            if(y->color == red) {
                x->par->color = black;
                y->color = black;
                x->par->par->color = red;
                x = x->par->par;
            }
            else {
                if(x == x->par->lchird) {
                    x = x->par;
                    rbrightRotate(x);
                }
                x->par->color = black;
                x->par->par->color = red;
                rbleftRotate(x->par->par);
            }   
        }
    }
    this->root->color = black;
}

void INTTree::rbdelete(interval_tree_node* nodeptr) {
    interval_tree_node* y = nodeptr;
    interval_tree_node* x;
    interval_tree_node* z;
    int y_ori_color = y->color;
    if(nodeptr->lchird == nil) {
        x = nodeptr->rchird;
        rbtransplant(nodeptr, nodeptr->rchird);
        z = nodeptr->par;
        while(z != nil) {
            interval_fixup(z);
            z = z->par;
        }
    }
    else if(nodeptr->rchird == nil) {
        x = nodeptr->lchird;
        rbtransplant(nodeptr, nodeptr->lchird);
        z = nodeptr->par;
        while(z != nil) {
            interval_fixup(z);
            z = z->par;
        }
    }
    else {
        y = rbmin(nodeptr->rchird);
        y_ori_color = y->color;
        x = y->rchird;
        if(y->par == nodeptr) 
            x->par = y;
        else {
            rbtransplant(y, y->rchird);
            y->rchird = nodeptr->rchird;
            y->rchird->par = y;
            interval_fixup(y->rchird);
        }
        rbtransplant(nodeptr, y);
        y->lchird = nodeptr->lchird;
        y->lchird->par = y;
        y->color = nodeptr->color;
        z = y;
        while(z != nil) {
            interval_fixup(z);
            z = z->par;
        }
    }

    if(y_ori_color == black)
        rbdelete_fixup(x);
}

void INTTree::rbdelete_fixup(interval_tree_node* x){
    interval_tree_node* w;
    while(x != this->root && x->color == black) {
        if(x == x->par->lchird) {
            w = x->par->rchird;
            if(w->color == red) {
                w->color = black;
                x->par->color = red;
                rbleftRotate(x->par);
                w = x->par->rchird;
            }
            if(w->lchird->color == black && w->rchird->color == black) {
                w->color = red;
                x = x->par;
            }
            else {
                if(w->rchird->color == black) {
                    w->lchird->color = black;
                    w->color = red;
                    rbrightRotate(w);
                    w = x->par->rchird;
                }
                w->color = x->par->color;
                x->par->color = black;
                w->rchird->color = black;
                rbleftRotate(x->par);
                x = this->root;
            }       
        }
        else {
            w = x->par->lchird;
            if(w->color == red) {
                w->color = black;
                x->par->color = red;
                rbrightRotate(x->par);
                w = x->par->lchird;
            }
            if(w->rchird->color == black && w->lchird->color == black) {
                w->color = red;
                x = x->par;
            }
            else {
                if(w->lchird->color == black) {
                    w->rchird->color = black;
                    w->color = red;
                    rbleftRotate(w);
                    w = x->par->lchird;
                }
                w->color = x->par->color;
                x->par->color = black;
                w->lchird->color = black;
                rbrightRotate(x->par);
                x = this->root;
            } 
        }
    }
    x->color = black;
}

bool INTTree::inttree_check() {
    bool check = true;
    // 红黑树性质
        // 1. 每个节点或是红色,或是黑色: 通过创建节点保证
        // 2. 根节点是黑色
    if (this->root->color != black)
            check = false;
        // 3. 每个叶节点是黑色
        // 4. 如果一个节点是红色,那么两个子节点也是黑色
        // 5. 该节点到所有后代节点的黑高度相同
        // 6. 该树是一颗二叉树
    // 区间树性质
        // 7. node.max = max(node.int.high, node.lch.max, node.rch.max)
        
    // 以上几条使用深度优先遍历检测
    interval_tree_node* prev;
    std::vector<interval_tree_node*> leafs;
    std::vector<interval_tree_node*> stack;
    std::map<interval_tree_node*, int> black_height;
    std::map<interval_tree_node*, bool> visited;

    black_height[nil] = 0;

    stack.push_back(this->root);

    while(! stack.empty()) {
        prev = stack.back();

        if(prev->lchird != nil && prev->rchird != nil) {
            // 检查满足二叉树性质
            if(! (prev->lchird->key <= prev->key && prev->rchird->key >= prev->key))
                check = false;
            // 检查满足区间树的max域性质 
            int max = prev->interval.high;
            if(prev->lchird->max > max)
                max = prev->lchird->max;
            if(prev->rchird->max > max)
                max = prev->rchird->max;
            if(max != prev->max)
                check = false;    
            //  更新黑高度
            if(prev->color == black)
                black_height[prev] = black_height[prev->par] + 1;
            else
                black_height[prev] = black_height[prev->par];
            // 如果一个节点是黑色,那么两个子节点也是黑色 
            if(prev->color == red && (prev->lchird->color == red || prev->rchird->color == red))
                check = false;
            if(visited.find(prev->lchird) != visited.end() && visited[prev->lchird] == false || visited.find(prev->lchird) == visited.end()) {
                stack.push_back(prev->lchird);
            }    
            else if(visited.find(prev->rchird) != visited.end() && visited[prev->rchird] == false || visited.find(prev->rchird) == visited.end()) {
                stack.push_back(prev->rchird);
            }
            else {
                visited[prev] = true;
                stack.pop_back();
            }
        }
        else if(prev->lchird != nil) {
            // 检查满足二叉树性质
            if(! (prev->lchird->key <= prev->key))
                check = false;
            // 检查满足区间树的max域性质 
            int max = prev->interval.high;
            if(prev->lchird->max > max)
                max = prev->lchird->max;
            if(max != prev->max)
                check = false;    
            //  更新黑高度
            if(prev->color == black)
                black_height[prev] = black_height[prev->par] + 1;
            else
                black_height[prev] = black_height[prev->par];
            // 如果一个节点是黑色,那么两个子节点也是黑色 
            if(prev->color == red && prev->lchird->color == red)
                check = false;
            if(visited.find(prev->lchird) != visited.end() && visited[prev->lchird] == false || visited.find(prev->lchird) == visited.end()) {
                stack.push_back(prev->lchird);
            }    
            else {
                visited[prev] = true;
                stack.pop_back();
            }
        }
        else if(prev->rchird != nil) {
            // 检查满足二叉树性质
            if(! (prev->rchird->key >= prev->key))
                check = false;
            // 检查满足区间树的max域性质 
            int max = prev->interval.high;
            if(prev->rchird->max > max)
                max = prev->rchird->max;
            if(max != prev->max)
                check = false;    
            //  更新黑高度
            if(prev->color == black)
                black_height[prev] = black_height[prev->par] + 1;
            else
                black_height[prev] = black_height[prev->par];
            // 如果一个节点是黑色,那么两个子节点也是黑色 
            if(prev->color == red && prev->rchird->color == red)
                check = false;
            if(visited.find(prev->rchird) != visited.end() && visited[prev->rchird] == false || visited.find(prev->rchird) == visited.end()) {
                stack.push_back(prev->rchird);
            }
            else {
                visited[prev] = true;
                stack.pop_back();
            }
        }
        else {
            // 此时为叶子节点
            // 检查满足二叉树性质
            // 检查满足区间树的max域性质 
            if(prev->max != prev->interval.high)   
                check = false;
            //  更新黑高度
            if(prev->color == black)
                black_height[prev] = black_height[prev->par] + 1;
            else
                black_height[prev] = black_height[prev->par];
            // 如果一个节点是黑色,那么两个子节点也是黑色 
            visited[prev] = true;
            stack.pop_back();
            leafs.push_back(prev);
        }
    }

    // 检查所有叶子的黑高度
    int bh = black_height[*(leafs.begin())];
    for(auto &node: leafs) {
        if(bh != black_height[node])
            check = false;
    }

    return check;
}


void INTTree::inorder_visit(interval_tree_node* node) {
    if(node->lchird != nil) 
        inorder_visit(node->lchird);
    fout << node->interval.low << "\t" << node->interval.high << "\t" << node->max << std::endl;
    if(node->rchird != nil)
        inorder_visit(node->rchird);
}

void INTTree::rbdestroy() {
    if(this->root != nil)
        interval_tree_node::free_node_iterate(this->root);
    this->root = nil;
}

void interval_tree_node::free_node_iterate(interval_tree_node* nodeptr) {
    if(nodeptr->lchird != INTTree::nil)
        free_node_iterate(nodeptr->lchird);
    if(nodeptr->rchird != INTTree::nil)
        free_node_iterate(nodeptr->rchird);
    delete nodeptr;
}

interval_tree_node* INTTree::rbmin(interval_tree_node* nodeptr) {
    assert(nodeptr != nil);
    while(nodeptr->lchird != nil) {
        nodeptr = nodeptr->lchird;
    }

    return nodeptr;
}

interval_tree_node* INTTree::interval_search(Interval interval) {
    interval_tree_node* x = this->root;
    while(x != nil && !(x->interval.overlap(interval))) {
        if(x->lchird != nil && x->lchird->max >= interval.low) 
            x = x->lchird;
        else 
            x = x->rchird;
    }
    return x;
}   

void INTTree::interval_fixup(interval_tree_node* x) {
    int max_num = x->interval.high;

    if(x->lchird != nil && x->rchird != nil) {
        if(x->lchird->max > max_num)
            max_num = x->lchird->max;
        if(x->rchird->max > max_num)
            max_num = x->rchird->max;
    }
    else if(x->lchird != nil) {
        if(x->lchird->max > max_num)
            max_num = x->lchird->max;
    }
    else if(x->rchird != nil) {
        if(x->rchird->max > max_num)
            max_num = x->rchird->max;
    }
    x->max = max_num;
}


int main() {
    ifstream fin;
    fin.open("E:\\Savefiles\\Labs\\Algorithm\\lab3\\ex1\\input\\input.txt", ios::in);

    // 读入数据并创建区间树
    Interval interval;
    INTTree inttree;

    while(! fin.eof()) {
        fin >> interval.low;
        fin >> interval.high;
        interval_tree_node* new_node = new interval_tree_node(interval.low, interval.high, interval.low, interval.high, red);
        inttree.interval_insert(new_node);
    }

    bool check = inttree.inttree_check();
    std::cout << "Interval Tree Check(true or false): " << check << std::endl;

    string fout_path("E:\\Savefiles\\Labs\\Algorithm\\lab3\\ex1\\output\\inorder.txt");
    inttree.init_fout(fout_path);
    inttree.inttree_inorder_visit();


    interval_tree_node* tmp;
    tmp = inttree.rbsearch(8);
    inttree.interval_delete(tmp);

    fout_path = "E:\\Savefiles\\Labs\\Algorithm\\lab3\\ex1\\output\\delete_data.txt";
    ofstream fout(fout_path, ios::app);
    fout << 8 << std::endl;
    fout.close();
    inttree.init_fout(fout_path);
    inttree.inttree_inorder_visit();

    check = inttree.inttree_check();

    std::cout << "Interval Tree Check(true or false): " << check << std::endl;

    fout_path = "E:\\Savefiles\\Labs\\Algorithm\\lab3\\ex1\\output\\search.txt";
    fout.open(fout_path, ios::app);
    interval.low = 37;
    interval.high = 40;
    tmp = inttree.interval_search(interval);
    if (tmp == INTTree::nil) 
        fout << "Null" << std::endl;
    else 
        fout << tmp->interval.low << "\t" << tmp->interval.high << std::endl;
    

    interval.low = 16;
    interval.high = 19;
    tmp = inttree.interval_search(interval);
    if (tmp == INTTree::nil) 
        fout << "Null" << std::endl;
    else 
        fout << tmp->interval.low << "\t" << tmp->interval.high << std::endl;


    interval.low = 26;
    interval.high = 26;
    tmp = inttree.interval_search(interval);
    if (tmp == INTTree::nil) 
        fout << "Null" << std::endl;
    else 
        fout << tmp->interval.low << "\t" << tmp->interval.high << std::endl;

    fout.close();
}