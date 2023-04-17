#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

typedef struct CLNode{
	unsigned char ch;
	long weight;
	struct CLNode* next;
}CLNode,*CL;

typedef struct HTNode{
	long weight;
    unsigned char ch;
	long par, lch, rch;
	char* code;
}HTNode;

bool FindInCL(CL CList,unsigned char ch );
long  LenCL(CL CList);


void compress(const char* FileIn, const char* FileOut)
{
	//读取文件信息：文件长度，文件出现的字符及其出现次数
	//逐字节读取
	FILE* fpin = fopen(FileIn, "rb");
    assert(fpin!=NULL);
	unsigned char ch;
	long flength;
	CL CList = (CL)malloc(sizeof(CLNode));    //带头节点的字符链表
	CList->next = NULL;
	CLNode* pre = CList;

	//创建字符表
	fread(&ch, 1, 1, fpin);
	while (!feof(fpin))
	{
		if (!FindInCL(CList, ch))
		{
			CLNode* node = (CLNode*)malloc(sizeof(CLNode));
			node->ch = ch;
			node->weight = 0;
			node->next = NULL;
			pre-> next = node;
			pre = node;                        //接入字符链表
		}
		fread(&ch, 1, 1, fpin);
	}

	//多次扫描文件获得各字符的权重
	pre = CList->next;
	while (pre)
	{
		fseek(fpin, 0, SEEK_SET);
		fread(&ch, 1, 1, fpin);
		while (!feof(fpin))
		{
			if (ch == pre->ch) pre->weight++;
			fread(&ch, 1, 1, fpin);
		}
		pre = pre->next;
	}

	//根据创建的字符表及权重创建Huffman树
	long n = LenCL(CList);
	long m = 2 * n - 1;
	HTNode* HTree = (HTNode*)malloc(sizeof(HTNode) * (m + 1)); //带头结点的Huffman节点数组
	pre = CList->next;
    long i,j,k; 
	for (i = 1; i <= n; i++,pre=pre->next)
	{
		(HTree[i]).weight = pre->weight;
		(HTree[i]).code = NULL;
        (HTree[i]).ch = pre->ch;
		(HTree[i]).lch = 0;
		(HTree[i]).rch = 0;
		(HTree[i]).par = 0;
	}
	while (i <= m)
	{
		(HTree[i]).weight = 0;
		(HTree[i]).code = NULL;
        (HTree[i]).ch = '\0';
		(HTree[i]).lch = 0;
		(HTree[i]).rch = 0;
		(HTree[i]).par = 0;                      //初始化Huffman节点数组
        i++;
	}
	for (i = n + 1; i <= m; i++)
	{
		long min = 99999999;
		long k=0;
		for (j = 1; j <= i - 1; j++)
		{
			if (((HTree[j]).weight < min) && ((HTree[j]).par == 0))
			{
				min = (HTree[j]).weight;
				k = j;
			}
		}
		assert(k != 0);                           //确保min初值足够大
		(HTree[i]).weight = (HTree[k]).weight;
		(HTree[i]).lch = k;
		(HTree[k]).par = i;
		min = 99999999;
		k = 0;
		for (j = 1; j <= i - 1; j++)
		{
			if (((HTree[j]).weight < min)&&((HTree[j]).par==0))
			{
				min = (HTree[j]).weight;
				k = j;
			}
		}
		assert(k != 0);                           //确保min初值足够大
		(HTree[i]).weight += (HTree[k]).weight;
		(HTree[i]).rch = k;
		(HTree[k]).par = i;

	}
    //确定各字符对应的Huffman编码
    // FILE* f=fopen("1.txt","w");
    for(i=1;i<=n;i++)
    {
        long end;
        char* code = (char*)malloc(sizeof(char)*(n+1));
        code[n] = '\0';
        j = (HTree[i]).par;
        k = i;
        end = n;
        while(j)
        {
            end--;
            if((HTree[j]).lch == k) code[end] = '0';
            else code[end] = '1';
            k = j;
            j = (HTree[j]).par;
        }
        assert(j==0) ;   
        // char* HUT =  (char*)malloc(sizeof(char)*(n-end+1));
        // strcpy(HUT,&(code[end]));    
        // fprintf(f,"%s\n",HUT); 
        (HTree[i]).code = (char*)malloc(sizeof(char)*(n-end+1));
        strcpy((HTree[i]).code,&(code[end]));
    }
    //  fclose(f);

    //读取文件并编码
    fseek(fpin,0,SEEK_END);   //将读文件指针置于文件开头
    flength = ftell(fpin);
    fseek(fpin,0,SEEK_SET);
    FILE* fpout = fopen(FileOut,"wb");
    assert(fpout!=NULL);
    fseek(fpout,8,SEEK_SET);  //预留位置，写入文件头


    printf("读取将要压缩的文件：%s,共有字节%d个\n",FileIn,flength);
    printf("正在压缩...\n");

    char buf[512];                 //设定一个缓存区，存储要写入文件的内容
    buf[0] = '\0';
    long rlength=0,wlength=8;
    long LenBuf;
    while(rlength!=flength)
    {
        fread(&ch,1,1,fpin);
        rlength++;
        for(i=1;i<=n;i++)
        {
            if(ch==(HTree[i]).ch) break;
        }
        assert((i>=1)&&(i<=n));
        strcat(buf,(HTree[i]).code);
        LenBuf = strlen(buf);
        ch = 0;
        while(LenBuf>=8)
        {
            for(j=0;j<8;j++)
            {
                if(buf[j] == '1') ch = (ch<<1)|1;
                else ch = ch<<1;
            }
            strcpy(buf,buf+8);
            fwrite(&ch,1,1,fpout);
            wlength++;
            LenBuf = strlen(buf);
        }
    }
    if(LenBuf >0)      //最终剩余字符少于8个
    {
        strcat(buf,"00000000");
        for(i=0;i<8;i++)
        {
            if(buf[i] == '1') ch = (ch<<1)|1;
            else ch = ch<<1;
        }
        fwrite(&ch,1,1,fpout);
        wlength++;
    }

    // 写入文件头与编码信息
    fseek(fpout,0,SEEK_SET);
    assert(sizeof(flength) == 4);
    fwrite(&flength,4,1,fpout);
    assert(sizeof(wlength) == 4);
    fwrite(&wlength,4,1,fpout);
    fseek(fpout,wlength,SEEK_SET);
    assert(sizeof(n)==4);
    fwrite(&n,4,1,fpout);
    wlength+=4;
    HTNode Node;
    long LenCode;
    for(i=1;i<=n;i++)
    {
        Node = HTree[i];
        fwrite(&((HTree[i]).ch),1,1,fpout);
        wlength++;
        assert(strlen((HTree[i]).code)<256);
        ch = strlen((HTree[i]).code);
        fwrite(&ch,1,1,fpout);
        wlength++;
        LenCode = strlen((HTree[i]).code);
        if(LenCode%8 != 0)
        {
            for(j=LenCode%8;j<8;j++) strcat((HTree[i]).code,"0");
        }
        while((HTree[i]).code[0] != 0)
        {
            ch = 0;
            for(j=0;j<8;j++)
            {
                if((HTree[i]).code[j]=='1') ch = (ch<<1)|1;
                else ch = ch<<1;
            }
            strcpy((HTree[i]).code,(HTree[i]).code+8);
            fwrite(&ch,1,1,fpout);
            wlength++;
        }
        HTree[i] = Node;
    }
    fclose(fpin);
    fclose(fpout);
    printf("压缩后文件为:%s,共写入字节%d个\n",FileOut,wlength);

}

void uncompress(const char*InFile,const char*OutFile)
{

    //读取压缩文件，并根据压缩信息构建Huffman树
    long flength,len1,len2;
    long n,m;
    unsigned char ch;
    long LenCode;
    char buf[512];             
    char buf2[512];         //缓存区    
    
    FILE* fpin = fopen(InFile,"rb");
    assert(fpin!=NULL);
    fseek(fpin,0,SEEK_END);
    flength = ftell(fpin);
    fseek(fpin,0,SEEK_SET);
    printf("读取压缩文件:%s,共有字节%d个\n",InFile,flength);
    printf("正在解压...\n");
    fread(&len1,sizeof(long),1,fpin);
    fread(&len2,sizeof(long),1,fpin);
    fseek(fpin,len2,SEEK_SET);
    fread(&n,sizeof(long),1,fpin);
    m = 2*n-1;
    HTNode* HTree = (HTNode*)malloc(sizeof(HTNode) * (m + 1)); //带头结点的Huffman节点数组
    long i,j,k,l;
    // FILE* f = fopen("1.txt","w");
    for(i=1;i<=n;i++)
    {
        fread(&((HTree[i]).ch),1,1,fpin);
        fread(&ch,1,1,fpin);
        LenCode = (long)ch;
        assert(LenCode<256);  //确保后面设立的缓存区不溢出
        (HTree[i]).weight = LenCode;
        (HTree[i]).code =(char*)malloc(sizeof(char)*(LenCode+1));
        (HTree[i]).code[0] = 0;
        if(LenCode%8>0) k=LenCode/8+1;
        else k = LenCode/8;
        for(j=0;j<k;j++)
        {
            fread(&ch,1,1,fpin);
            _itoa(ch,buf,2);
            for(l=8;l>strlen(buf);l--)
            {
                strcat((HTree[i]).code,"0");
            }
            strcat((HTree[i]).code,buf);
        }
        (HTree[i]).code[LenCode] = 0;
        // fprintf(f,"%s\n",(HTree[i]).code);
    }
    // fclose(f);

    //解压缩文件
    FILE* fpout = fopen(OutFile,"wb");
    assert(fpout!=NULL);
    long wlength;
    fseek(fpin,8,SEEK_SET);
    buf2[0] = 0;
    while(true)
    {
        while((strlen(buf2) < 256)&&(wlength!=len1))   //确保缓存区不溢出,同时至少读入一个原字符的编码
        {
            fread(&ch,1,1,fpin);
            _itoa(ch,buf,2);
            for(l=8;l>strlen(buf);l--)
            {
                strcat(buf2,"0");
            }
            strcat(buf2,buf);  
        }
        for(i=1;i<n;i++)
        {
            if(memcmp((HTree[i]).code, buf2, (HTree[i]).weight) == 0) break;
        }  
        strcpy(buf2,buf2+(HTree[i]).weight);
        ch = (HTree[i]).ch;
        fwrite(&ch,1,1,fpout);
        wlength++;
        // if(wlength==299990) 
        //         printf("pause");
        
        if(wlength == len1) break;
    }
    fclose(fpin);
    fclose(fpout);
    printf("解压后文件为%s,共有字节%d个\n",OutFile,wlength);
}

bool FindInCL(CL CList,unsigned char ch )
{
    CLNode* pre = CList->next;
    while(pre)
    {
        if(pre->ch == ch) return true;
        pre = pre->next;
    }
    return false;
}
long  LenCL(CL CList)
{
    CLNode* pre = CList->next;
    long len=0;
    while(pre)
    {
        len++;
        pre = pre->next;
    }
    return len;
}

int main(int argc,char** argv) {
	const char* FileIn = "logo.jpg";
	const char* FileOut = "logo.zip";
	const char* FileInR = "logo.zip";
	const char* FileOutR = "logo_r.jpg";
	compress(FileIn, FileOut);
	uncompress(FileInR, FileOutR);
	return 0;
}


//全局变量不初始化
//文件读写问题,fwrite,fprintf,fread,fread,"rb","r","w","wb"的区别
//数据类型，溢出问题，类型转化问题
//程序适应性问题,assert的使用