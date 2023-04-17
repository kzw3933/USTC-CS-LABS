#pragma once
#include<string>
#include<fstream>
#include<map>
#include<queue>
#include<assert.h>
using namespace std;

struct Huffman_node
{
	unsigned long id;
	string code;
	unsigned long freq;
	Huffman_node* child[16];
	Huffman_node* parent;
};

typedef Huffman_node* Node_ptr;

class Huffman
{
private:
	Node_ptr root;
	vector<unsigned long> node_array;
	unsigned char forks;        //几叉树
	unsigned char bits_size;  //编码单元大小(以4bits为单位)
	unsigned long compress_flength;
	unsigned long code_saved_address;
	unsigned char bits_length;
	unsigned long remaind_chars_saved_address;
	unsigned long in_flength;
	unsigned long Node_num;
	fstream  in_file, out_file;
	map<unsigned long, string> table_strings;
	map<unsigned long, unsigned long>table_weights;
	class Compare
	{
	public:
		bool operator()(const Node_ptr& c1, const Node_ptr& c2) const
		{
			return (*c1).freq > (*c2).freq;
		}
	};
	priority_queue< Node_ptr, vector<Node_ptr>, Compare > pq;

	//计算文件的有效压缩长度
	unsigned long get_vaild_length();

	//辅助计算文件的有效压缩长度
	unsigned short LCM(unsigned short m,unsigned short n);

	//辅助计算编码长度
	unsigned char get_bits_length(unsigned char forks);

	// 从输入文件中获取编码单元及其频度
	void create_node_array(); 

	// 根据构造好的编码单元数组构造优先队列
	void create_pq();

	// 根据构造好的Huffman树建立Huffman映射表
	void create_map_table(const Node_ptr node, unsigned char child_serial_number);

	// 构造Huffman树
	void create_huffman_tree();

	// 计算Huffman编码
	void calculate_huffman_codes();

	// 开始压缩过程
	void do_compress();

	//展示huffman树
	void show_huffman_tree();

	//展示huffman节点
	void show_huffman_node(Node_ptr node,unsigned long level);

	// 从huffman编码文件中重建huffman树
	void rebuid_huffman_tree();

	//辅助重建Huffman树
	unsigned char get_child_serial_number(string str);

	// 根据重建好的huffman树解码文件
	void decode_huffman();

public:
	// 根据输入和输出流初始化对象
	Huffman(string in_file_name, string out_file_name, unsigned char FORKs, unsigned char BITs);

	// 析构函数
	~Huffman();

	// 压缩文件
	void compress();

	// 解压文件
	void decompress();

};
