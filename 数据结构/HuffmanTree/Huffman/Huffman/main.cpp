#include "huffman.h"

int main()
{
	Huffman h("logo.zip", "logo_s.jpg",6,2);
	h.decompress();
}