#include "huffman.h"

unsigned long Huffman::get_vaild_length()
{
	unsigned long f_bits_len = 8 * in_flength;
	unsigned short m = 8;
	unsigned short n = 4 * bits_size;
	unsigned long valid_length = 0;
	unsigned short l = LCM(m,n);
	while (valid_length <= f_bits_len) valid_length += l;
	return (valid_length-l) / 8;
}
unsigned short Huffman::LCM(unsigned short m,unsigned short n)
{
	if (m == n) return m;
	else if (m > n) return LCM(m - n, n);
	else return LCM(n - m, m);
}

unsigned char Huffman::get_bits_length(unsigned char forks)
{
	assert((forks >= 2) && (forks <= 16));
	if (forks <= 2)
			return 1;
	else if (forks <= 4)
			return 2;
	else if (forks <= 8)
			return 3;
	else return 4;
}



void Huffman::create_node_array()
{
	char in_char;
	unsigned char u_char=0;
	unsigned long id=0;
	unsigned long freq=0;
	unsigned long r_length=0;
	bool char_read_in = true;
	Node_num = 0;
	in_file.seekg(0, ios::end);
	in_flength = (unsigned long)in_file.tellg();
	compress_flength = get_vaild_length();
	assert(compress_flength);
	in_file.seekg(0, ios::beg);
	while (r_length!=compress_flength)
	{
		for (unsigned char i = 0; i < bits_size; i++)
		{
			if (char_read_in)
			{
				in_file.read(&in_char, 1);
				u_char = (unsigned char)in_char;
				r_length++;
				char_read_in = false;
			}
			else
				char_read_in = true;
			id = (id << 4) + (u_char >> 4);
			u_char = (u_char << 4);
		}
		bool exist = (find(node_array.begin(), node_array.end(),id)!=node_array.end());
		if (exist)
		{
			id = 0;
			continue;
		}
		else
		{
			unsigned long* node = new unsigned long();
			*node = id;
			node_array.push_back(*node);
			Node_num++;
			table_weights[id] = 0;
			id = 0;
		}	
	}
	in_file.seekg(0, ios::beg);
	r_length = 0;
	char_read_in = true;
	while (r_length != compress_flength)
	{
		for (unsigned char i = 0; i < bits_size; i++)
		{
			if (char_read_in)
			{
				in_file.read(&in_char, 1);
				u_char = (unsigned char)in_char;
				r_length++;
				char_read_in = false;
			}
			else
				char_read_in = true;
			id = (id << 4) + (u_char >> 4);
			u_char = (u_char << 4);
		}
		table_weights[id]++;
		id = 0;		
	}
}

void Huffman::create_pq()
{
	create_node_array();
	vector<unsigned long>::iterator iter = node_array.begin();
	for (; iter != node_array.end(); iter++)
	{
		Node_ptr node = new Huffman_node();
		node->id = *iter;
		node->freq = table_weights[*iter];
		node->code = "";
		for (int j = 0; j < 16; j++)
			node->child[j] = NULL;
		node->parent = NULL;
		pq.push(node);
	}
	unsigned long increment = 0;
	while (((Node_num + increment - 1) % (forks - 1)) != 0)
	{		
		Node_ptr node = new Huffman_node();
		node->id = unsigned long (-1);
		node->freq = 0;
		node->code = "";
		for (int j = 0; j < 16; j++)
			node->child[j] = NULL;
		node->parent = NULL;
		pq.push(node);
		increment++;
	}
	Node_num += increment;
}

void Huffman::create_huffman_tree()
 {
	root = NULL;
	Node_ptr PTR[16];
	unsigned char i=0;
	while (!pq.empty())
	{
		PTR[0] = pq.top();
		pq.pop();
		i=1;
		if (pq.empty())
		{
			root = PTR[0];
			break;
		}
		while (i < forks)
		{
			PTR[i] = pq.top();
			pq.pop();
			i++;
		}
		Node_ptr new_node = new Huffman_node();
		new_node->freq = 0;
		for (i = 0; i < forks; i++)
		{
			new_node->freq += (PTR[i])->freq;
			new_node->id = (unsigned long)-1;
			new_node->child[i] = PTR[i];
			(PTR[i])->parent = new_node;
		}
		pq.push(new_node);
	}
}

void Huffman::calculate_huffman_codes()
{
	assert(root);
	for (unsigned char i = 0; i < forks; i++)
	{
		if (root->child[i])
			create_map_table(root->child[i], i );
	}
}

void Huffman::create_map_table(const Node_ptr node, unsigned char child_serial_number)
{
	char str1[5],str2[5];
	str1[0] = 0;
	str2[0] = 0;
	unsigned char bits_length = get_bits_length(forks);
	_itoa(child_serial_number, str1, 2);
	for (unsigned char i = (unsigned char)strlen(str1); i < bits_length; i++)
		strcat(str2, "0");
	strcat(str2, str1);
	node->code = node->parent->code +str2 ;
	if (node->child[0] == NULL)
		table_strings[node->id] = node->code;
	else {
		for (unsigned char i = 0; i < forks; i++)
		{
			if (node->child[i] == NULL) break;
			create_map_table(node->child[i], i);
		}
	}
}
void Huffman::do_compress()
{
	unsigned long r_length, w_length;
	char in_char;
	unsigned char i, j;
	bool char_read_in = true;
	unsigned long id=0;
	unsigned char u_char,out_char=0;
	map<unsigned long, string>::iterator table_it;
	string buf;     //定义一个缓存区
	unsigned char buf_len=buf.size();
	in_file.seekg(0, ios::beg);
	r_length = 0;
	//预留文件头，写入原文件长度，有效编码长度，存入编码起始位置，编码单元大小，树的叉树
	out_file.seekp(18, ios::beg);  
	w_length = 18;
	//存入原文件的编码信息
	while (r_length != compress_flength)
	{
		while (buf_len<64)
		{
			for (i = 0; i < bits_size; i++)
			{
				if (char_read_in)
				{
					in_file.read(&in_char, 1);
					u_char = (unsigned char)in_char;
					r_length++;
					char_read_in = false;
				}
				else
					char_read_in = true;
				id = (id << 4) + (u_char >> 4);
				u_char = (u_char << 4);
			}
			table_it = table_strings.find(id);
			assert(table_it != table_strings.end());
			buf += table_it->second;
			buf_len = buf.size();
			id = 0;
			if (r_length == compress_flength)
				break;
		}
		for (i = 0; (i+8) <= buf_len ; i+=8)
		{
			out_char = 0;
			for (j = 0; j < 8; j++)
			{
				u_char = 1;
				if (buf[i + j] == '1')
					out_char += u_char << (7 - j);				
			}
			out_file.write((char*)&out_char, 1);
			w_length++;
		}
		buf = buf.substr(i);	
		buf_len = buf.size();
	}
	//处理buf不足的情况
	if (buf_len > 0 && buf_len < 8)
	{
		buf += "00000000";
		out_char = 0;
		for (j = 0; j < 8; j++)
		{
			u_char = 1;
			if (buf[j] == '1')
				out_char += u_char << (7 - j);
		}
		out_file.write((char*)&out_char, 1);
		w_length++;
	}
	//写入文件头
	out_file.seekp(0, ios::beg);
	out_file.write((char*)&in_flength, 4);
	out_file.write((char*)&compress_flength, 4);
	out_file.write((char*)&w_length, 4);              //剩余字符起始位置
	out_file.write((char*)&bits_size, 1);
	out_file.write((char*)&forks, 1);
	out_file.seekp(w_length, ios::beg);
	//处理剩余字符
	while (compress_flength < in_flength)
	{
		in_file.read(&in_char, 1);
		r_length++;
		out_file.write(&in_char, 1);
		w_length++;
		if (r_length == in_flength)
			break;
	 }
	 
	//存入编码信息
	out_file.seekp(14, ios::beg);
	out_file.write((char*)&w_length, 4);
	out_file.seekp(w_length, ios::beg);
	out_file.write((char*)&Node_num, 4);
	w_length += 4;
	table_it = table_strings.begin();
	while (table_it != table_strings.end())
	{
		out_file.write((char*)&(table_it->first), 4);
		unsigned char code_len = (table_it->second).size();
		out_file.write((char*)&(code_len), 1);
		out_file.write((char*)(table_it->second.c_str()), code_len);
		w_length += (4+1+code_len);
		table_it++;
	}
	
}

void Huffman::show_huffman_tree()
{
	Node_ptr node = root;
	assert(root);
	show_huffman_node(root,1);

}

void Huffman::show_huffman_node(Node_ptr node,unsigned long level)
{
	printf("%d-->%d\n",level,node->id);
	
	for (unsigned char i = 0; i < 16; i++) {
		if (node->child[i])
			show_huffman_node(node->child[i],level+1);
		else
			break;
	}
}

Huffman::Huffman(string in_file_name, string out_file_name,unsigned char Forks,unsigned char BITs)
{
	in_file.open(in_file_name.c_str(), ios::in | ios::binary);
	assert(in_file);
	out_file.open(out_file_name.c_str(), ios::out | ios::binary);
	assert(out_file);
	forks = Forks;
	bits_size = BITs;
	compress_flength=0;
	in_flength=0;
	Node_num=0;
	root = NULL;
	bits_length = 0;
	code_saved_address = 0;
	remaind_chars_saved_address = 0;
}




unsigned char Huffman::get_child_serial_number(string str)
{
	unsigned char str_len = str.size();
	unsigned char child_serial_num=0;
	unsigned u_c = 1;
	for (unsigned char i = 0; i < str_len; i++)
	{
		unsigned char u_c = 1;
		if (str[i] == '1')
			child_serial_num += u_c << (str_len - i-1);
	}
	return child_serial_num;
}

void Huffman::rebuid_huffman_tree()
{
	unsigned long id;
	unsigned char code_len;
	unsigned char bits_length;
	char buf[256];
	root = new Huffman_node();
	for (unsigned char l = 0; l < 16; l++)
		root->child[l] = NULL;	
	root->parent = NULL;
	Node_ptr node, tmp, new_node;
	string str;
	in_file.seekg(0, ios::beg);
	in_file.read((char*) & in_flength, 4);
	in_file.read((char*)&compress_flength, 4);
	in_file.read((char*)&remaind_chars_saved_address, 4);
	in_file.read((char*)&bits_size, 1);
	in_file.read((char*)&forks, 1);
	in_file.read((char*)&code_saved_address, 4);
	in_file.seekg(code_saved_address, ios::beg);
	in_file.read((char*)&Node_num, 4);
	bits_length = get_bits_length(forks);
	for (unsigned long i = 0; i < Node_num; i++)
	{
		in_file.read((char*)&id, 4);
		in_file.read((char*)&code_len, 1);
		in_file.read((char*)buf, code_len);
		buf[code_len] = 0;
		str += buf;
		node = root;
		unsigned char iter_num = code_len / bits_length;
		for (unsigned char j = 0; j < iter_num; j++)
		{
			unsigned child_serial_number = get_child_serial_number(str.substr(0,bits_length));
			tmp = node->child[child_serial_number];
			if (tmp == NULL)
			{
				new_node = new Huffman_node();
				for (unsigned char l = 0; l < 16; l++)
					new_node->child[l] = NULL;
				new_node->parent = node;
				if (j == iter_num - 1)
				{
					new_node->id = id;
					new_node->code = buf;
				}
				node->child[child_serial_number] = new_node;
				tmp = new_node;
			}
			node = tmp;
			str = str.substr(bits_length);
		}
	}
}


void Huffman::decode_huffman()
{
	unsigned long w_length=0;
	in_file.seekg(18, ios::beg);
	out_file.seekp(0, ios::beg);
	char in_char;
	unsigned char u_char;
	char buf[10];
	char buf1[10];
	unsigned long id;
	bool char_write_out=false;
	Node_ptr node;
	unsigned char bits_length = get_bits_length(forks);
	string str;
	unsigned char str_len = str.size();
	while (w_length != compress_flength)
	{
		while (str_len < 32)
		{
			in_file.read(&in_char, 1);
			u_char = (unsigned char)in_char;
			buf1[0] = 0;
			_itoa(u_char, buf, 2);
			for (unsigned char i = 0; i < 8 - strlen(buf); i++)
				strcat(buf1, "0");
			strcat(buf1, buf);
			str += buf1;
			str_len = str.size();
			
		}
		node = root;
		while(true)
		{
			string sub_str = str.substr(0, bits_length);
			unsigned char child_serial_number = get_child_serial_number(sub_str);
			node = node->child[child_serial_number];
			str = str.substr(bits_length);
			str_len = str.size();
			if (node->child[0] == NULL)
			{
				id = node->id;
				break;
			}
			
		}
		//将id写入文件
		u_char = 0;
		for (unsigned char i = 0; i < bits_size; i++)
		{
			u_char = (u_char << 4) + (id >> (4 * (bits_size - i - 1)));
			id = id << ((8 - bits_size + i + 1) * 4);
			id = id >> ((8 - bits_size + i + 1) * 4);
			if (char_write_out)
			{
				out_file.write((char*)&u_char, 1);
				u_char = 0;
				w_length++;
				char_write_out = false;
				if (w_length == compress_flength)
					break;
			}
			else
				char_write_out = true;

		}

	}
	if(compress_flength < in_flength)
	{
		in_file.seekg(remaind_chars_saved_address, ios::beg);
		while (true)
		{
			in_file.read((char*)&in_char, 1);
			out_file.write((char*)in_char, 1);
			w_length++;
			if (w_length == in_flength)
				break;
		}		
	}
}

Huffman::~Huffman()
{
	in_file.close();
	out_file.close();
}

void Huffman::compress()
{
	create_pq();
	create_huffman_tree();
	calculate_huffman_codes();
	do_compress();
	show_huffman_tree();
}




void Huffman::decompress()
{
	rebuid_huffman_tree();
	decode_huffman();
}