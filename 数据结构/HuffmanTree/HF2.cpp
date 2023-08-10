#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

typedef struct CLNode{
	unsigned long ch;                         //4个字节
	unsigned long weight;
	struct CLNode* next;
}CLNode,*CL;

typedef struct HTNode{
    unsigned long ch;                   //4个字节
    unsigned long weight;
	unsigned long par, lch, rch;
	char* code;
}HTNode;
unsigned long ValLen(unsigned long flength,unsigned short size);
unsigned short LCM(unsigned short m,unsigned short n);

bool FindInCL(CL CList,unsigned long ch );
unsigned long  LenCL(CL CList);

void compress(const char* FileIn,const char* FileOut,unsigned short size)
{
    //读取文件信息：文件长度，文件出现的字符及其出现次数
	//逐字节读取
    assert((size>=1)&&(size<=8));
	FILE* fpin = fopen(FileIn, "rb");
    assert(fpin!=NULL);
	unsigned char ch,chl;             
    unsigned short lth;            //ch中有效bit数
    unsigned long buf;             //定义bit缓存区，最多存储32bit
	unsigned long flength,ValidLength;
    unsigned long rlength;
    unsigned short remaindLen; 
    
    unsigned long i,j,k;

    CL CList = (CL)malloc(sizeof(CLNode));    //带头节点的字符链表
	CList->next = NULL;
	CLNode* pre = CList;

    fseek(fpin,0,SEEK_END);
    flength = ftell(fpin);
    ValidLength = ValLen(flength,size);    //定义有效字节数
    remaindLen = flength - ValidLength;      //定义剩余字节数


    //--------------------------------------------------------------------------------------------->
    fseek(fpin,0,SEEK_SET); 
    rlength=0;
    fread(&ch,1,1,fpin);
    assert(!feof(fpin));
    rlength++;
    lth=8;
    while(rlength!=ValidLength)                   //一次循环读入一个编码单元
    {      
        buf = 0;
        for(i=0;i<size;i++)
        {
            if(lth==0)
            {
                fread(&ch,1,1,fpin);
                rlength++;
                lth = 8;                
            }
            buf = (buf<<4)+(ch>>4);
            ch = ch<<4;
            lth -= 4;            
        }
        if(!FindInCL(CList, buf))
        {
            CLNode* node = (CLNode*)malloc(sizeof(CLNode));
			node->ch = buf;
			node->weight = 0;
			node->next = NULL;
			pre-> next = node;
			pre = node;                        //接入字符链表
        }
        // printf("读入一个编码单元\n");

    }

    // printf("%d",LenCL(CList));


    //---------------------------------------------------------------------------------->
    fseek(fpin,0,SEEK_SET);
    rlength = 0;
    fread(&ch,1,1,fpin);
    assert(!feof(fpin));
    rlength++;
    lth=8;

    while(rlength!=ValidLength)                   //一次循环读入一个编码单元
    {    
        buf = 0;
        for(i=0;i<size;i++)
        {
            if(lth==0)
            {
                fread(&ch,1,1,fpin);
                rlength++;
                lth = 8;                
            }
            buf = (buf<<4)+(ch>>4);
            ch = ch<<4;
            lth -= 4;            
        }
        pre = CList->next;
        while(pre)
        {
            if(pre->ch == buf)
            {
                pre->weight++;
                break;
            }
            else pre = pre->next;
        }
        assert(pre!=NULL);      //所有编码单元都存入表中
    }
    
    // printf("编码单元权重已写入表中\n");

    
    
    
    
    //-------------------------------------------------------------------------------------------->   
    //根据创建的字符表及权重创建Huffman树
	long n = LenCL(CList);
	long m = 2 * n - 1;
	HTNode* HTree = (HTNode*)malloc(sizeof(HTNode) * (m + 1)); //带头结点的Huffman节点数组
	pre = CList->next;
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
		k=0;
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
    // FILE* f=fopen("2.txt","w");
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
        //  fprintf(f,"%s\n",HUT);     
        (HTree[i]).code = (char*)malloc(sizeof(char)*(n-end+1));
        strcpy((HTree[i]).code,&(code[end]));
    }
    // fclose(f);

    // printf("Huffman树及其Huffman编码构建完成!\n");


    //--------------------------------------------------------------------------------------->

    //读取文件并编码
    fseek(fpin,0,SEEK_SET);
    rlength=0;
    printf("读取待压缩文件：%s，共有字节：%d个,有效压缩字节：%d个\n",FileIn,flength,ValidLength);
    FILE* fpout = fopen(FileOut,"wb");
    assert(fpout!=NULL);
    fseek(fpout,12,SEEK_SET);  //预留位置，写入文件头
    printf("正在压缩...\n");
   
    fread(&ch,1,1,fpin);
    assert(!feof(fpin));
    rlength++;
    lth=8;
    char buf1[512];                                  //定义缓存区
    buf1[0] = '\0';
    unsigned long wlength=12;
    unsigned long LenBuf; 
    while(rlength!=ValidLength)                   //一次循环读入一个编码单元
    {    
        buf = 0;
        for(i=0;i<size;i++)
        {
            if(lth==0)
            {
                fread(&ch,1,1,fpin);
                rlength++;
                lth = 8;                
            }
            buf = (buf<<4)+(ch>>4);
            ch = ch<<4;
            lth -= 4;            
        }
        for(i=1;i<=n;i++)
        {
            if(buf==(HTree[i]).ch) break;
        }                                            //获取编码单元对应的编码
        strcat(buf1,(HTree[i]).code);
        LenBuf = strlen(buf1);
        chl = 0;
        while(LenBuf>=8)
        {
            for(j=0;j<8;j++)
            {
                if(buf1[j] == '1') chl = (chl<<1)|1;
                else chl = chl<<1;
            }
            strcpy(buf1,buf1+8);
            fwrite(&chl,1,1,fpout);
            wlength++;
            LenBuf = strlen(buf1);
        }
    }

    if(LenBuf >0)      //最终剩余字符少于8个
    {
        strcat(buf1,"00000000");
        for(i=0;i<8;i++)
        {
            if(buf1[i] == '1') ch = (ch<<1)|1;
            else ch = ch<<1;
        }
        fwrite(&ch,1,1,fpout);
        wlength++;
    }

        //写入剩余字符
    if(remaindLen!=0)
    {
        unsigned short l = remaindLen;
        while(l)
        {
            fread(&ch,1,1,fpin);
            fwrite(&ch,1,1,fpout);
            l--;
            wlength++;
        } 
    }


    //------------------------------------------------------------------------------------>

    // 写入文件头与编码信息
    fseek(fpout,0,SEEK_SET);
    assert(sizeof(flength) == 4);
    fwrite(&ValidLength,4,1,fpout);
    assert(sizeof(wlength) == 4);
    fwrite(&wlength,4,1,fpout);
    assert(sizeof(remaindLen)==2);
    fwrite(&remaindLen,2,1,fpout);
    assert(sizeof(size)==2);
    fwrite(&size,2,1,fpout);
    fseek(fpout,wlength,SEEK_SET);
    
    assert(sizeof(n)==4);
    fwrite(&n,4,1,fpout);
    wlength+=4;
    unsigned long LenCode;
    HTNode Node;
    for(i=1;i<=n;i++)
    {
        Node = HTree[i];
        fwrite(&((HTree[i]).ch),4,1,fpout);
        wlength+=4;
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

void uncompress(const char* FileIn,const char* FileOut)
{
    //读取压缩文件，并根据压缩信息构建Huffman树
    unsigned long flength;
    unsigned short remaindLen,size;

    unsigned long len1,len2;
    unsigned char ch,chl;

    unsigned long m,n;
    unsigned long LenCode;  
    unsigned long buf;
    unsigned short lth;        

    char buf1[512],buf2[512];

   

    FILE* fpin = fopen(FileIn,"rb");
    assert(fpin!=NULL);
    fseek(fpin,0,SEEK_END);
    flength = ftell(fpin);
    fseek(fpin,0,SEEK_SET);
    printf("读取压缩文件:%s,共有字节%d个\n",FileIn,flength);
    printf("正在解压...\n");
    fread(&len1,sizeof(unsigned long),1,fpin);
    fread(&len2,sizeof(unsigned long),1,fpin);
    fread(&remaindLen,sizeof(unsigned short),1,fpin);
    fread(&size,sizeof(unsigned short),1,fpin);
    assert((size>=1)&&(size<=8));
    fseek(fpin,len2,SEEK_SET);
    fread(&n,sizeof(unsigned long),1,fpin);
    m = 2*n-1;
    HTNode* HTree = (HTNode*)malloc(sizeof(HTNode) * (m + 1)); //带头结点的Huffman节点数组
    
    unsigned long i,j,k,l;
    // FILE* f = fopen("2.txt","w");
    for(i=1;i<=n;i++)
    {
        fread(&((HTree[i]).ch),4,1,fpin);
        fread(&ch,1,1,fpin);
        LenCode = (long)ch;
        assert(LenCode<256);//确保后面设立的缓存区不溢出
        (HTree[i]).weight = LenCode;
        (HTree[i]).code =(char*)malloc(sizeof(char)*(LenCode+1));
        (HTree[i]).code[0] = 0;
        if(LenCode%8>0) k=LenCode/8+1;
        else k = LenCode/8;
        for(j=0;j<k;j++)
        {
            fread(&ch,1,1,fpin);
            _itoa(ch,buf1,2);
            for(l=8;l>strlen(buf1);l--)
            {
                strcat((HTree[i]).code,"0");
            }
            strcat((HTree[i]).code,buf1);
        }
        (HTree[i]).code[LenCode] = 0;
        //  fprintf(f,"%s\n",(HTree[i]).code);
    }
    // fclose(f);

    // printf("重构Huffman树成功!\n");

    //解压缩文件
    FILE* fpout = fopen(FileOut,"wb");
    assert(fpout!=NULL);
    unsigned long wlength=0;
    fseek(fpin,12,SEEK_SET);
    buf2[0]=0;
    chl = 0;
    lth = 0;
    while(true)
    {
        while((strlen(buf2) < 256)&&(wlength!=len1))   //确保缓存区不溢出,同时至少读入一个原字符的编码
        {
            fread(&ch,1,1,fpin);
            _itoa(ch,buf1,2);
            for(l=8;l>strlen(buf1);l--)
            {
                strcat(buf2,"0");
            }
            strcat(buf2,buf1);
        }
        for(i=1;i<n;i++)
        {
            if(memcmp((HTree[i]).code, buf2, (HTree[i]).weight) == 0) break;
        }
        strcpy(buf2,buf2+(HTree[i]).weight);
        buf = (HTree[i]).ch;
        for(i=0;i<size;i++)
        {
            chl = (chl<<4)+(buf>>(4*(size-i-1)));
            // printf(" %d",(buf>>(4*(size-i-1))));
            buf = buf<<((8-size+i+1)*4);
            // printf(" %d",buf);
            buf = buf>>((8-size+i+1)*4);
            // printf(" %d\n",buf);
            lth+=4;
            if(lth==8)
            {
                fwrite(&chl,1,1,fpout);
                wlength++;
                lth = 0;
                chl = 0;
                if(wlength==len1) break;                  //to look for
            }
            // if(wlength==299990) 
            //     printf("pause");
            if(i==(size-1)) buf = 0;
        }
        if(wlength==len1) break;
    }
   
    while(wlength!=(len1+remaindLen))
    {
        fread(&ch,1,1,fpin);
        fwrite(&ch,1,1,fpout);
        wlength++;
    }
    fclose(fpin);
    fclose(fpout);
    printf("解压后文件为:%s,共有字节:%d个\n",FileOut,wlength);
}



int main(int argc,char** argv) 
{
	// const char* FileIn = "MiKu.png";
	// const char* FileOut = "test.zip";
	// const char* FileInR = "test.zip";
	// const char* FileOutR = "MiKuR.png";
    printf("请选择是要解压缩还是压缩:1代表解压,0代表压缩\n");
    unsigned short choice;
    char FileIn[40];
    char FileOut[40];
    scanf("%d",&choice);
    if(choice==1)
    {
        printf("请输入要解压的文件名:\n");
        scanf("%s",FileIn);
        printf("请输入要输出的文件名:\n");
        scanf("%s",FileOut);
        uncompress(FileIn,FileOut);
    }
    else if(choice==0)
    {
        printf("请输入要压缩的文件名:\n");
        scanf("%s",FileIn);
        printf("请输入要压缩的最小单元(以4bit为单位),范围1-8\n");
        unsigned short size;
        scanf("%d",&size);
        printf("请输入要输出的文件名:\n");
        scanf("%s",FileOut);
        compress(FileIn,FileOut,size);
    };
	return 0;
}



bool FindInCL(CL CList,unsigned long buf )
{
    CLNode* pre = CList->next;
    while(pre)
    {
        if(pre->ch == buf) return true;
        pre = pre->next;
    }
    return false;
}
unsigned long  LenCL(CL CList)
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

unsigned long ValLen(unsigned long flength,unsigned short size)
{
    unsigned short m,n,l;
    unsigned long ValidLength=0;
    m = 8;
    unsigned long fbitsLen = flength*8;
    n = 4*size;

    l = LCM(m,n); 
    while(ValidLength<=fbitsLen) ValidLength+=l;
    return (ValidLength-l)/8;

}

unsigned short LCM(unsigned short m,unsigned short n)
{
    if(m==n) return m;
    else if(m>n) return LCM(m-n,n);
    else return LCM(n-m,m);
}






//全局变量不初始化
//文件读写问题,fwrite,fprintf,fread,fread,"rb","r","w","wb"的区别
//数据类型，溢出问题，类型转化问题
//程序适应性问题,assert的使用
//移位运算的优先级
//移位的骚操作
//变量重复利用导致值的覆盖

