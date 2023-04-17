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
	unsigned char forks;        //������
	unsigned char bits_size;  //���뵥Ԫ��С(��4bitsΪ��λ)
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

	//�����ļ�����Чѹ������
	unsigned long get_vaild_length();

	//���������ļ�����Чѹ������
	unsigned short LCM(unsigned short m,unsigned short n);

	//����������볤��
	unsigned char get_bits_length(unsigned char forks);

	// �������ļ��л�ȡ���뵥Ԫ����Ƶ��
	void create_node_array(); 

	// ���ݹ���õı��뵥Ԫ���鹹�����ȶ���
	void create_pq();

	// ���ݹ���õ�Huffman������Huffmanӳ���
	void create_map_table(const Node_ptr node, unsigned char child_serial_number);

	// ����Huffman��
	void create_huffman_tree();

	// ����Huffman����
	void calculate_huffman_codes();

	// ��ʼѹ������
	void do_compress();

	//չʾhuffman��
	void show_huffman_tree();

	//չʾhuffman�ڵ�
	void show_huffman_node(Node_ptr node,unsigned long level);

	// ��huffman�����ļ����ؽ�huffman��
	void rebuid_huffman_tree();

	//�����ؽ�Huffman��
	unsigned char get_child_serial_number(string str);

	// �����ؽ��õ�huffman�������ļ�
	void decode_huffman();

public:
	// ����������������ʼ������
	Huffman(string in_file_name, string out_file_name, unsigned char FORKs, unsigned char BITs);

	// ��������
	~Huffman();

	// ѹ���ļ�
	void compress();

	// ��ѹ�ļ�
	void decompress();

};
