#include "defines.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <ctime>

#define Infty   (2147483647 >> 2)
#define Vertex_num Vertex_num1
#define Input_file_path input_scale_1_1_file
#define Output_file_path output_scale_1_1_file
#define Code_flag code_flag_1_1
#define Vertex_num_expand (Vertex_num+1)     // 为johnson算法预留一个顶点

using namespace std;

typedef struct node {
    int id;
    struct node* next;
} node,*vertex_list;

int Graph_weights[Vertex_num_expand][Vertex_num_expand];
vertex_list Graph_edges[Vertex_num_expand];
int Graph_pi[Vertex_num_expand];    // 顶点的前驱, 供bellman_ford使用
int Graph_d[Vertex_num_expand];     // 目前的最短距离, 供bellman_ford使用
int Graph_D[Vertex_num_expand][Vertex_num_expand]; // dijkstra使用
int Graph_PI[Vertex_num_expand][Vertex_num_expand]; // dijkstra使用
int Heap_nodes[Vertex_num+1]; // 使用堆实现优先队列, 0号元素对应优先队列的长度
int Heap_size;


/*堆相关算法*/

inline int parent(int i) {
    return i / 2;
}

inline int left(int i) {
    return 2*i;
}

inline int right(int i) {
    return 2*i+1;
}

void min_heapify(int* heap, int i, int* d) {
    int l = left(i);
    int r = right(i);
    int smallest;
    int temp;
    if(l <= Heap_size && d[heap[l]] < d[heap[i]])
        smallest = l;
    else
        smallest = i;
    if(r <= Heap_size && d[heap[r]] < d[heap[smallest]])
        smallest = r;
    if(smallest != i) {
        temp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = temp;
        min_heapify(heap, smallest, d);
    }
}


void build_min_heap(int* heap, int* d) {
    Heap_size = heap[0];
    for(int i=heap[0]/2; i >= 1; i--) {
        min_heapify(heap, i, d);
    }
}

//void heap_sort(int* heap) {
//    build_min_heap(heap);
//    int temp;
//    for(int i=heap[0]; i>=2; i--) {
//        temp = heap[1];
//        heap[1] = heap[i];
//        heap[i] = temp;
//        Heap_size = Heap_size-1;
//        min_heapify(heap, 1);
//    }
//}

//int heap_minimum(int* heap) {
//    return heap[1];
//}

int heap_extract_min(int* heap, int* d) {
    if(Heap_size < 1) {
        cout << "heap overflow";
        return -1;
    }
    int min = heap[1];
    heap[1] = heap[Heap_size];
    Heap_size = Heap_size-1;
    min_heapify(heap, 1, d);
    return min;
}

void heap_decrease_key(int* heap, int i, int key, int* d) {
    if(key > d[heap[i]]) {
        cout << "new key is bigger than current key";
        return ;
    }
    d[heap[i]] = key;
    int temp;
    while(i>1 && d[heap[parent(i)]] > d[heap[i]]) {
        temp = heap[i];
        heap[i] = heap[parent(i)];
        heap[parent(i)] = temp;
        i = parent(i);
    }
}

//void min_heap_insert(int* heap, int key, int* d) {
//    Heap_size = Heap_size+1;
//    d[heap[Heap_size]] = Infty;
//    heap_decrease_key(heap, Heap_size, key, d);
//}


/*Bellman ford算法*/

void initialize_single_source(int s, int* d, int* pi) {
    for(int i=0; i< Vertex_num_expand ; i++) {
        d[i] = Infty;
        pi[i] = -1;
    }
    d[s] = 0;
}

void relax(int u, int v, int* d, int* pi) {
    if(d[v] > d[u] + Graph_weights[u][v]) {
        d[v] = d[u] + Graph_weights[u][v];
        pi[v] = u;
    }
}

void relax(int u, int v, int*d, int* heap, int* pi) {
    if(d[v] > d[u] + Graph_weights[u][v]) {
        for(int i=0; i< Heap_size; i++) {
            if(heap[i] == v) {
                heap_decrease_key(heap, i, d[u] + Graph_weights[u][v], d);
            }
        }
        pi[v] = u;
    }
}


// 对扩展的图执行bellman_ford操作, s = Vertex_num
// 传入源s, 以及l,r供检测到负环时,函数向外传递检测到负环的边的左右顶点序号
bool bellman_ford(int s, int *l, int* r) {
    initialize_single_source(s, Graph_d, Graph_pi);
    node* node_ptr;
    for(int i=1; i<= Vertex_num_expand-1;i++) {
        for(int j=0; j<Vertex_num_expand ; j++) {
            node_ptr = Graph_edges[j];
            while(node_ptr) {
                relax(j, node_ptr->id, Graph_d, Graph_pi);
                node_ptr = node_ptr->next;
            }
        }
    }
    for(int i=0; i<Vertex_num_expand ; i++) {
        node_ptr = Graph_edges[i];
        while(node_ptr) {
            if(Graph_d[node_ptr->id] > Graph_d[i]+Graph_weights[i][node_ptr->id]) {
                *l = i;
                *r = node_ptr->id;
                return false;
            }
            node_ptr = node_ptr->next;
        }
    }
    return true;
}

// 传入源s, 储存当前各顶点的最小距离的数组d,储存前趋顶点数组pi
void dijkstra(int s, int* d, int* pi) {
    initialize_single_source(s, d, pi);
    vector<int> S;
    Heap_nodes[0] = Vertex_num;
    for(int i=0; i< Vertex_num; i++) {
        Heap_nodes[i+1] = i;
    }
    build_min_heap(Heap_nodes, d);
    node* node_ptr;
    while(Heap_size != 0) {
        int u = heap_extract_min(Heap_nodes, d);
        S.push_back(u);
        node_ptr = Graph_edges[u];
        while(node_ptr) {
            relax(u, node_ptr->id, d, Heap_nodes, pi);
            node_ptr = node_ptr->next;
        }
    }
}

void johnson() {
    ofstream  ftime;
    ftime.open(output_time_file, ios::app);
    int l,r;
    bool success = false;
    double time_start, time_end, time_cost;
    time_start = clock();
    // 扩展图
    for(int i=0; i< Vertex_num; i++) {
        node* new_node = (node*) malloc(sizeof(node));
        new_node->id = i;
        Graph_weights[Vertex_num][i] = 0;
        if(! Graph_edges[Vertex_num]) {
            new_node->next = nullptr;
            Graph_edges[Vertex_num] = new_node;
        }
        else {
            node* list_head = Graph_edges[Vertex_num];
            new_node->next = list_head->next;
            list_head->next = new_node;
        }
    }
    time_end = clock();
    time_cost = time_end - time_start;
    // 如果有负环就删除负环中的一条边
    while(!success) {
        success = bellman_ford(Vertex_num, &l, &r);
        if(!success) {

            node* node_ptr = Graph_edges[l];
            node* node_prev = Graph_edges[l];

            cout << "the input graph contains a negative-weight cycle, try delete the edge ( " << l << ", " << r << " )" << endl;
            while(node_ptr) {
                if(node_ptr->id == r) {
                    if(node_ptr == Graph_edges[l]) {
                        Graph_edges[l] = node_ptr->next;
                    }
                    else {
                        node_prev->next = node_ptr->next;
                    }
                    free(node_ptr);
                    break;
                }
                node_ptr = node_ptr->next;
            }
        }
    }

    time_start = clock();
    for(int i=0; i<Vertex_num; i++) {
        node* node_ptr = Graph_edges[i];
        while(node_ptr) {
            Graph_weights[i][node_ptr->id] += Graph_d[i] - Graph_d[node_ptr->id];
            node_ptr = node_ptr->next;
        }
    }

    for(int i=0; i< Vertex_num; i++) {
        dijkstra(i, Graph_D[i], Graph_PI[i]);
        for(int j=0; j<Vertex_num;j++) {
            if(Graph_D[i][j] != Infty) {
                Graph_D[i][j] += Graph_d[j] - Graph_d[i];
            }
        }
    }
    time_end = clock();
    time_cost += time_end - time_start;

    time_cost = time_cost / CLOCKS_PER_SEC;

    cout << "Time costs " << time_cost <<" s";
    ftime << time_cost << "s" << endl;

}


int main() {
    ifstream fin;
    ofstream fout;
    fin.open(Input_file_path, ios::in);
    fout.open(Output_file_path, ios::out);
    int edge_l;
    int edge_r;
    int edge_weight;
    while(! fin.eof()) {
        fin >> edge_l >> edge_r >> edge_weight;
        node* new_node = (node*) malloc(sizeof(node));
        new_node->id = edge_r;
        Graph_weights[edge_l][edge_r] = edge_weight;
        if(! Graph_edges[edge_l]) {
            new_node->next = nullptr;
            Graph_edges[edge_l] = new_node;
        }
        else {
            node* list_head = Graph_edges[edge_l];
            new_node->next = list_head->next;
            list_head->next = new_node;
        }
    }

    cout << Code_flag << endl;

    johnson();

    for(int i=0; i<Vertex_num; i++) {
        for(int j=0; j<Vertex_num; j++) {
            if(Graph_D[i][j] < Infty) {
                fout << "( ";
                int prev = j;
                vector<int> stack;
                while(prev != -1) {
                    stack.push_back(prev);
                    prev = Graph_PI[i][prev];
                }
                while(! stack.empty()) {
                    fout << stack.back() << ",";
                    stack.pop_back();
                }
                fout << " " << Graph_D[i][j] << " )";
                fout << endl;
            }
            else {
                fout << "Infty" << endl;
            }
        }
    }
}