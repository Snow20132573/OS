#include <iostream>
#include<vector>
#include<cstdio>
#include<cstring>
#include<cstdlib>
#include<map>
#include<iomanip>
#include<ctime>
using namespace std;
struct inode0{
    int number;//充当0级索引的块号
    vector<int> number0;//0级索引
};
struct inode1{
    int number;//充当1级索引的块号
    vector<int> number1;//1级索引
};
struct inode2{
    vector<int> number;//充当2级索引的块号
    vector<int> number2;//2级索引
};
struct inode{
    struct inode0 i0;//0级索引
    struct inode1 i1;//1级索引
    struct inode2 i2;//2级索引
};
struct FCB{
    char name[8]="";//文件名
    char content[400]="";//文件内容
    int type;//类型：0--文件，1--目录
    int Size;//大小
    char datetime[64];//日期
    struct inode Index;//文件索引表
    //struct FCB father;//指向父目录
    //map<string,struct FCB> mp;//根据文件名找到对应FCB;
    vector<struct FCB> son;//目录中的目录和文件
};



struct Empty_Block{
    int number;//空闲块号
    int num;//空闲块个数
    vector<struct Empty_Block> next;//最后一个空闲块指向的下一个空闲块集合
};
vector<struct Empty_Block> Empty1;//成组链接空闲块
vector<struct Empty_Block> Empty2;//成组链接空闲块
vector<struct Empty_Block> Empty3;//成组链接空闲块
int Empty_Block_number=300;//记载内存中总的空闲块个数

struct FCB Root;//根目录
char current_directory[256]=".";//当前路径
/*************************输出当前空闲块********************************/
void Dis_Empty()
{
    //cout<<"***************输出当前空闲块*****************"<<endl;
    vector<struct Empty_Block> Empty=Empty1;
    vector<struct Empty_Block> Empty0;
    vector<struct Empty_Block>::iterator it;
    cout<<"空闲块："<<endl;
    int m=4;
    if(Empty_Block_number<=100)
        m=2;
    else if(Empty_Block_number>100&&Empty_Block_number<=200)
        m=3;
    for(int t=1;t<m;t++){
        cout<<endl;
        for(int i=0;i!=Empty.size();i++){
            if(i%10==0)
                cout<<endl;
            cout<<Empty[i].number<<" ";
        }
        cout<<endl;
        int x=Empty.size();
        Empty=Empty[x-1].next;
    }
    cout<<endl;
}
/**********************初始化空闲块***********************/
void INIT()
{
    struct Empty_Block p;
    p.number=100;
    for(int i=100;i<199;i++){
        p.number=i;
        Empty1.push_back(p);
    }
    for(int i=200;i<299;i++){
        p.number=i;
        Empty2.push_back(p);
    }
    for(int i=300;i<400;i++){
        p.number=i;
        p.num=100;
        Empty3.push_back(p);
    }
    p.number=299;
    p.num=100;
    p.next=Empty3;
    Empty2.push_back(p);
    p.number=199;
    p.num=100;
    p.next=Empty2;
    Empty1.push_back(p);
//    cout<<"E1.size = "<<Empty1.size()<<endl;
//    cout<<"E2.size = "<<Empty2.size()<<endl;
//    cout<<"E3.size = "<<Empty3.size()<<endl;
}
/***********************系统时间**********************************/
void Get_datetime(char* tmp)
{
    time_t t=time(0);
    strftime(tmp,64,"%Y/%m/%d %H%M%S",localtime(&t));
}
/********************链接到混合索引************************/
void Link_inode(struct FCB &f,vector<int> x)
{
    int i=0;
    cout<<"***************链接到混合索引******************"<<endl;
    while(!x.empty())
    {
       i++;
       if(i<=6){//0级索引
            if(i==1){
                //cout<<"f.Index.i0.number = "<<*x.begin()<<endl;
                f.Index.i0.number=*x.begin();
                x.erase(x.begin());
                //cout<<"f.Index.i0.number = "<<f.Index.i0.number<<endl;

            }else{
                //cout<<"f.Index.i0.number0 = "<<*x.begin()<<endl;
                f.Index.i0.number0.push_back(*x.begin());
                x.erase(x.begin());
            }
       }else if(i<=12){
            if(i==7){
                //cout<<"f.Index.i0.number = "<<*x.begin()<<endl;
                f.Index.i1.number=*x.begin();
                x.erase(x.begin());
            }else{
                //cout<<"f.Index.i0.number1 = "<<*x.begin()<<endl;
                f.Index.i1.number1.push_back(*x.begin());
                x.erase(x.begin());
            }
       }else{
            cout<<"******2级*******"<<endl;
            if(i>=13&&i<=18){
                //cout<<"f.Index.i0.number = "<<*x.begin()<<endl;
                f.Index.i2.number.push_back(*x.begin());
                x.erase(x.begin());
            }else{
                f.Index.i2.number2.push_back(*x.begin());
                x.erase(x.begin());
            }
       }
    }
    //cout<<"i = "<<i<<endl;
}
/*************************分配内存*********************************/
bool Allocation_Memory(struct FCB &f,int num)//为FCB  f分配num块内存
{
    //空闲内存块是否足够分配
    if(Empty_Block_number<num){
        cout<<"Empty_Block_number "<<Empty_Block_number<<" < "<<"num "<<num<<endl;
        return false;
    }
    cout<<"*******************分配内存************************"<<endl;
    vector<int> x;//记载拿到的空闲块号
    int x_num=num;
    Empty_Block_number-=num;
    while(x_num){//申请自身需要的空闲块
        x.push_back(Empty1[0].number);
        Empty1.back().num--;

        if(Empty1.size()==1){//判断是不是超级块的最后一块
                //cout<<"操作失败!"<<endl;
            vector<struct Empty_Block> emp=Empty1[0].next;
            Empty1.erase(Empty1.begin());
            for(int i=0;i<100;i++)
                Empty1.push_back(emp[i]);
        }else{
            Empty1.erase(Empty1.begin());
        }
        x_num--;
    }
//    cout<<"分配的vector<>x内存块"<<endl;
//    int i=x.size();
//    for(int k=0;k!=i;k++)
//        cout<<x[k]<<" ";
//    cout<<endl;
    //将占用的内存块链接到混合索引
    Link_inode(f,x);
    return true;
}
/***********************多级混合索引的变化******************************/
void Dis_Index(struct FCB f)
{
    cout<<"多级混合索引表：\n0级占用块："<<endl;
    cout<<f.Index.i0.number<<" ";
    int t=0;
    //cout<<"f.Index.i0.number0 = "<<endl;
    while(t!=f.Index.i0.number0.size()){
        cout<<f.Index.i0.number0[t]<<" ";
        t++;
    }
    cout<<endl;
    if(f.Size>6){
        cout<<"1级占用块："<<endl;
        cout<<f.Index.i1.number<<" ";
        t=0;
        while(t!=f.Index.i1.number1.size()){
            cout<<f.Index.i1.number1[t]<<" ";
            t++;
        }
    }
    cout<<endl;
    t=0;
    if(f.Size>12){
        //cout<<"2级占用块："<<endl;
        while(t!=f.Index.i2.number.size()){
            cout<<f.Index.i2.number[t]<<" ";
            t++;
        }
        t=0;
        while(t!=f.Index.i2.number2.size()){
            cout<<f.Index.i2.number2[t]<<" ";
            t++;
        }

    }
    cout<<endl;
    cout<<"Empty_Block_num "<<Empty_Block_number<<endl;
}
/*************************输出内存分配后的变化******************************/
void Dis_change(struct FCB f)
{
    cout<<"分配后的变化"<<endl;
    Dis_Empty();//空闲块变化
    Dis_Index(f);
}
/**********************创建虚拟磁盘文件***************************/
void Format()
{
    //为根目录分配内存
    cout<<"创建根目录"<<endl;
    //fcb.name=".";
    strcat(Root.name,".");
    //strcat(root_name,fcb.name);
    //strcat(current_directory,fcb.name);
    Root.Size=1;
    Root.type=1;
    //Root.father=Root;
    Get_datetime(Root.datetime);
    //Root=
    Allocation_Memory(Root,1);
    Dis_change(Root);
}
void help()
{
    cout<<"输入命令的格式如下："<<endl;
    cout<<"显示目录内容-----------dir"<<endl;
    cout<<"显示目录里特殊文件-----dir a*"<<endl;
    cout<<"创建目录---------------md name"<<endl;
    cout<<"删除目录---------------del root/user/j1"<<endl;
    cout<<"创建空文件-------------mk nodname"<<endl;
    cout<<"创建文件---------------mk nodname 10"<<endl;
    cout<<"删除文件---------------del nodname"<<endl;
    cout<<"进入上级目录-----------cd.."<<endl;
    cout<<"进入当前目录j1---------cd root/user/j1"<<endl;
    cout<<"编辑文件内容-----------edit j1"<<endl;
    cout<<"显示文件内容-----------cat j1"<<endl;
}

int split(char *str,char result[10][40],char chr)//把一个字符串分解为多个子串
{
	int i=0,j=0,k=0;
	while(str[i]==chr)
	{
		i++;
	}
	if(str[i]=='\0'){
        //cout<<"************1"<<endl;
		return 0;
	}
	if(str[i]=='\n'){
        //cout<<"************1"<<endl;
		return 1;
	}
	for(;str[i]!='\0';i++)
	{
        if(str[i]!=chr)
		{
			result[j][k++]=str[i];
		}
		else
		{
			result[j++][k]='\0';
			k=0;
		}
	}
	result[j][k]='\0';
	return j+1;
}
void Trim(char* s)//消除命令两边的空白串
{
    char *p=s;
	while(*p==' '||*p=='\t'||*p=='\r'||*p=='\n')
	{
		p++;
	}
	strcpy(s,p);
	int i=strlen(s)-1;
	while(s[i]==' '||s[i]=='\t'||s[i]=='\r'||s[i]=='\n')
	{
		i--;
	}
	s[i+1]='\0';
}
/***************更新父目录大小,并返回代表当前路径的FCB*******************/
struct FCB* Update_Father_Size(int k)
{
    char str[10][40];
    struct FCB *f=&Root;
    if(k)
        f->Size+=k;
    int len=split(current_directory,str,'/');

    for(int i=1;i<len;i++){
        for(int j=0;j<f->son.size();j++){
            if(strcmp(str[i],f->son[j].name)==0){
                if(k)
                    f->son[j].Size+=k;
                //cout<<"f.son[j].name = "<<f->son[j].name<<endl;
                f=&(f->son[j]);
                break;
            }
        }
    }
    return f;
}
/*********检查文件或目录在当前目录下是否存在**********/
bool IS_Exit(char* str,int k)
{
    struct FCB *f=Update_Father_Size(0);
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str)==0&&f->son[i].type==k){
            return true;//文件或目录已存在
        }
    }
    return false;
}
/**************创建FCB 0--目录 1--文件*************/
void Make_f(char str[10][40],int len,int k)
{
    bool bb=IS_Exit(str[1],k);
    if(bb){
        if(k==1)
            cout<<"输入目录已存在"<<endl;
        else
            cout<<"输入文件已存在"<<endl;
        return ;
    }
    struct FCB f;
    strcat(f.name,str[1]);//为FCB赋初值
    Get_datetime(f.datetime);
    f.type=k;
    f.Size=1;
    if(len==3){
        int x=atoi(str[2]);
        if(x>=1&&x<=5){
            x++;
        }else if(x>=6&&x<=10){
            x+=2;
        }else if(x>=11&&x<=15){
            x+=4;
        }else if(x>=16&&x<=20){
            x+=5;
        }else if(x>=21&&x<=25){
            x+=6;
        }else if(x>=26&&x<=30){
            x+=7;
        }else if(x>=31&&x<=35){
            x+=8;
        }else{
            cout<<"文件大小越界"<<endl;
            return;
        }
        f.Size=x;
    }
    bool b=Allocation_Memory(f,f.Size);//分配内存
    if(!b){
        cout<<"内存分配失败!\n";
        return;
    }else{
        struct FCB *R=Update_Father_Size(f.Size);//逐级更新父目录大小
        //cout<<"R = "<<R->name<<endl;
//        for(int i=0;i<R->son.size();i++){
//            if(strcmp(R->son[i],str[1])==0)
//        }
        R->son.push_back(f);//将f放在当前目录下
        if(f.type==1){//新建的目录要更新当前路径
            strcat(current_directory,"/");
            strcat(current_directory,f.name);
        }
    }
    Dis_change(f);
}
/********************树***********************/
void TREE(struct FCB f,bool *v,int v_L)
{
    int i;
    //文件操作
    for(i=0;i<v_L;i++){//输出|和空格
        if(v[i]){
            cout<<"|";
        }else{
            cout<<" ";
        }
    }
    if(v[i])
        cout<<"|";
    cout<<"_"<<f.name;//输出当前FCB的名字
    if(f.son.size()!=0)
        cout<<"_";
    cout<<endl;
    //目录操作
    if(f.type==1){//对于目录递归调用
        if(strcmp(f.name,".")==0)
            v_L+=strlen(f.name)+2;
        else
            v_L+=strlen(f.name)+3;
        v[v_L]=true;
        int i;
        for(i=0;i<f.son.size();i++){
            TREE(f.son[i],v,v_L);
        }
        return;
    }
}
void Dis_TREE()
{
    bool v[100];
    int v_L=0;
    for(int i=0;i<100;i++)
        v[i]=0;
    TREE(Root,v,v_L);
}
bool Is_right_form(char* s1,char* s2)//判断S1串是不是S2串的格式 *a*bc*,?a?b
{
    int i,j,k=0;
    if(s1[0]!=s2[0]&&s2[0]!='*'&&s2[0]!='?')
        return false;
    for(i=0;i<strlen(s2);i++){
        bool f=0;
        if(s2[i]=='*'){
            continue;
        }
        if(s2[i]=='?'){
            k++;
            continue;
        }
        for(j=k;j<strlen(s1);j++){
            if(s1[j]==s2[i]){
                f=1;
                k=j+1;
                break;
            }
        }
        if(!f){
            return false;
        }
    }
    if(i==strlen(s2)&&s2[i-1]!='*'&&k<strlen(s1))//s2扫完最后一个但是s1没扫完，并且s2最后一个不是*
        return false;
    if(i==strlen(s2)&&s2[i-1]=='?'&&k!=strlen(s1))
        return false;
    return true;
}
/**************列出当前目录内的文件和目录***********************/
void List_FCB(char *s,int k)//k==1--显示目录内容,k==2显示目录中特殊内容
{
   cout<<"当前目录内容："<<endl;
   struct FCB *f=Update_Father_Size(0);
   if(k==1){
       for(int i=0;i<f->son.size();i++){
            cout<<setw(12)<<f->son[i].name<<setw(12)<<f->Size<<setw(20)<<f->datetime<<endl;
       }
   }else{
       for(int i=0;i<f->son.size();i++){
            if(Is_right_form(f->son[i].name,s))
            cout<<setw(12)<<f->son[i].name<<setw(12)<<f->Size<<setw(20)<<f->datetime<<endl;
       }
   }
}
/******************编辑文件**********************/
void Edit(char str[10][40])
{
    struct FCB *f=Update_Father_Size(0);
    int flag=0;
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str[1])==0){
            cout<<"输入文件内容："<<endl;
            flag=1;
            gets(f->son[i].content);
            break;
        }
    }
    if(!flag){
        cout<<"该文件不存在"<<endl;
    }
}
void Cat(char str[10][40])
{
    struct FCB *f=Update_Father_Size(0);
    int flag=0;
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str[1])==0){
            flag=1;
            cout<<f->son[i].content<<endl;
            break;
        }
    }
    if(!flag){
        cout<<"该文件不存在"<<endl;
    }
}
/*****************回收内存*******************/
void FREE_Memory(vector<int> x)
{
    struct Empty_Block e;
    while(!x.empty()){
        e.number=*x.begin();
        x.erase(x.begin());
        if(Empty1.back().num==100){
            e.num=1;
            e.next=Empty1;
            vector<Empty_Block> E1;
            E1.push_back(e);
            Empty1=E1;
        }else{
            e.num=Empty1.back().num;
            e.next=Empty1.back().next;
            Empty1.push_back(e);
        }
    }
}
/******************删除文件或目录************************/
void Del(char str[10][40])
{
    cout<<"*******************回收内存************************"<<endl;
    if(strcmp(str[1],".")==0){
        cout<<"根目录不能被删除"<<endl;
        return;
    }
    struct FCB *f=Update_Father_Size(0);
    //cout<<"f.name = "<<f->name<<endl;
    int flag=0;
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str[1])==0){//找到要删除的文件或目录
            //cout<<"找到要删除的文件或目录"<<f->name<<" - > "<<f->son[i].name<<endl;
            flag=1;
            if((f->son[i].type==0)||(f->son[i].type==1&&f->son[i].son.size()==0)){//保证是空目录或文件

                vector<int> x;
                int f_Size=f->son[i].Size;
                for(int yy=0;yy<f_Size;yy++){//一共是Size块存入X中
                    //cout<<" j = "<<yy<<"   文件大小："<<f->son[i].Size<<endl;
                    if(yy==0){
                        //cout<<"第一个"<<endl;
                        x.push_back(f->son[i].Index.i0.number);
                    }else if(yy>=1&&yy<=5){
                        //cout<<"第2个"<<endl;
                        x.push_back(f->son[i].Index.i0.number0[yy-1]);
                    }else if(yy==6){
                        x.push_back(f->son[i].Index.i1.number);
                    }else if(yy>=7&&yy<=11){
                        x.push_back(f->son[i].Index.i1.number1[yy-7]);
                    }else if(yy>=12&&yy<=17){
                        x.push_back(f->son[i].Index.i2.number[yy-12]);
                    }else if(yy>=18&&yy<=42){
                        x.push_back(f->son[i].Index.i2.number2[yy-18]);
                    }else{
                        cout<<"Error!"<<endl;
                    }
                    //cout<<"循环"<<endl;
                }
                struct FCB *t=Update_Father_Size(0-f->son[i].Size);//更新父目录大小
                //移除FCB
                vector<struct FCB>::iterator it;
                for(it=f->son.begin();it!=f->son.end();it++){
                    if(strcmp((*it).name,str[1])==0){//找到了FCB的迭代器
                        break;
                    }
                }
                //cout<<"FCB.size = "<<f->son.size()<<endl;
                f->son.erase(it);
                //cout<<"删除元素后父目录含FCB个数 = "<<f->son.size()<<endl;
                //回收内存
                FREE_Memory(x);
                Empty_Block_number+=f->son[i].Size;
                Dis_Empty();
                cout<<"*******************回收内存成功************************"<<endl;
            }else{
                cout<<"非空"<<endl;
            }
            break;
        }
    }
    if(!flag){
        cout<<"********该文件不存在回收内存失败********"<<endl;
    }
}
/***************返回上级目录*****************/
void Cd_father()
{
    cout<<"开始进入上级目录"<<endl;
    char str[10][40];
    int len=split(current_directory,str,'/');
    strcpy(current_directory,str[0]);
    if(len>2)
        strcat(current_directory,"/");
    for(int i=1;i<len-1;i++){
        strcat(current_directory,str[i]);
        if(i!=len-2)
            strcat(current_directory,"/");
    }
    //cout<<"Cd    current_directory = "<<current_directory<<endl;
    cout<<"进入上级目录成功"<<endl;
}
void Cd_absolute_directory(char str[10][40])
{
    char s[10][40];
    int len=split(str[1],s,'/');
    if(s[0][0]!='.'){
        cout<<"路径没从根目录开始该路径错误！"<<endl;
        return ;
    }
    struct FCB *f=&Root;
    for(int i=1;i<len;i++){
        bool b=false;
        //cout<<"s[i] = "<<s[i]<<endl;
        for(int j=0;j<f->son.size();j++){
            //cout<<"f->son[j].name = "<<f->son[j].name<<endl;
            if(strcmp(s[i],f->son[j].name)==0){
                b=true;
                f=&(f->son[j]);
                break;
            }
        }
        if(!b){
            cout<<"该路径错误！"<<endl;
            return ;
        }
    }
    strcpy(current_directory,str[1]);
    cout<<"切换路径成功!"<<endl;
}
void Function()
{
    INIT();
    cout<<"成组链接空闲块："<<endl;
    Dis_Empty();
    cout<<"初始化虚拟磁盘文件(为根目录分配内存):"<<endl;
    Format();
    help();
    char cmd[400];
    char str[10][40];
    cout<<"输入命令："<<endl;
    while(1){
        gets(cmd);
        Trim(cmd);
        //cout<<"处理过的Cmd = "<<cmd<<endl;
        int len=split(cmd,str,' ');//记载命令的块数,以空格 为区分符
        if(strcmp(str[0],"md")==0&&len==2){
            Make_f(str,len,1);
        }else if(strcmp(str[0],"mk")==0&&(len==2||len==3)){
            Make_f(str,len,0);
        }else if(strcmp(str[0],"dir")==0&&(len==1||len==2)){
            List_FCB(str[1],len);
        }else if(strcmp(str[0],"edit")==0&&len==2){
            Edit(str);
        }else if(strcmp(str[0],"cat")==0&&len==2){
            Cat(str);
        }else if(strcmp(str[0],"del")==0&&len==2){
            Del(str);
        }else if(strcmp(str[0],"cd..")==0&&len==1){
            Cd_father();
        }else if(strcmp(str[0],"cd")==0&&len==2){
            Cd_absolute_directory(str);
        }else{
            cout<<"输入命令格式错误！"<<endl;
        }
        cout<<"current_directory = "<<current_directory<<endl;
        Dis_TREE();
        //cout<<"Root.son.size = "<<Root.son.size()<<endl;
        //cout<<"Root.name = "<<Root.name<<endl;
    }

}
int main()
{
    Function();
    return 0;
}
