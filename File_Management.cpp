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
    int number;//�䵱0�������Ŀ��
    vector<int> number0;//0������
};
struct inode1{
    int number;//�䵱1�������Ŀ��
    vector<int> number1;//1������
};
struct inode2{
    vector<int> number;//�䵱2�������Ŀ��
    vector<int> number2;//2������
};
struct inode{
    struct inode0 i0;//0������
    struct inode1 i1;//1������
    struct inode2 i2;//2������
};
struct FCB{
    char name[8]="";//�ļ���
    char content[400]="";//�ļ�����
    int type;//���ͣ�0--�ļ���1--Ŀ¼
    int Size;//��С
    char datetime[64];//����
    struct inode Index;//�ļ�������
    //struct FCB father;//ָ��Ŀ¼
    //map<string,struct FCB> mp;//�����ļ����ҵ���ӦFCB;
    vector<struct FCB> son;//Ŀ¼�е�Ŀ¼���ļ�
};



struct Empty_Block{
    int number;//���п��
    int num;//���п����
    vector<struct Empty_Block> next;//���һ�����п�ָ�����һ�����п鼯��
};
vector<struct Empty_Block> Empty1;//�������ӿ��п�
vector<struct Empty_Block> Empty2;//�������ӿ��п�
vector<struct Empty_Block> Empty3;//�������ӿ��п�
int Empty_Block_number=300;//�����ڴ����ܵĿ��п����

struct FCB Root;//��Ŀ¼
char current_directory[256]=".";//��ǰ·��
/*************************�����ǰ���п�********************************/
void Dis_Empty()
{
    //cout<<"***************�����ǰ���п�*****************"<<endl;
    vector<struct Empty_Block> Empty=Empty1;
    vector<struct Empty_Block> Empty0;
    vector<struct Empty_Block>::iterator it;
    cout<<"���п飺"<<endl;
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
/**********************��ʼ�����п�***********************/
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
/***********************ϵͳʱ��**********************************/
void Get_datetime(char* tmp)
{
    time_t t=time(0);
    strftime(tmp,64,"%Y/%m/%d %H%M%S",localtime(&t));
}
/********************���ӵ��������************************/
void Link_inode(struct FCB &f,vector<int> x)
{
    int i=0;
    cout<<"***************���ӵ��������******************"<<endl;
    while(!x.empty())
    {
       i++;
       if(i<=6){//0������
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
            cout<<"******2��*******"<<endl;
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
/*************************�����ڴ�*********************************/
bool Allocation_Memory(struct FCB &f,int num)//ΪFCB  f����num���ڴ�
{
    //�����ڴ���Ƿ��㹻����
    if(Empty_Block_number<num){
        cout<<"Empty_Block_number "<<Empty_Block_number<<" < "<<"num "<<num<<endl;
        return false;
    }
    cout<<"*******************�����ڴ�************************"<<endl;
    vector<int> x;//�����õ��Ŀ��п��
    int x_num=num;
    Empty_Block_number-=num;
    while(x_num){//����������Ҫ�Ŀ��п�
        x.push_back(Empty1[0].number);
        Empty1.back().num--;

        if(Empty1.size()==1){//�ж��ǲ��ǳ���������һ��
                //cout<<"����ʧ��!"<<endl;
            vector<struct Empty_Block> emp=Empty1[0].next;
            Empty1.erase(Empty1.begin());
            for(int i=0;i<100;i++)
                Empty1.push_back(emp[i]);
        }else{
            Empty1.erase(Empty1.begin());
        }
        x_num--;
    }
//    cout<<"�����vector<>x�ڴ��"<<endl;
//    int i=x.size();
//    for(int k=0;k!=i;k++)
//        cout<<x[k]<<" ";
//    cout<<endl;
    //��ռ�õ��ڴ�����ӵ��������
    Link_inode(f,x);
    return true;
}
/***********************�༶��������ı仯******************************/
void Dis_Index(struct FCB f)
{
    cout<<"�༶���������\n0��ռ�ÿ飺"<<endl;
    cout<<f.Index.i0.number<<" ";
    int t=0;
    //cout<<"f.Index.i0.number0 = "<<endl;
    while(t!=f.Index.i0.number0.size()){
        cout<<f.Index.i0.number0[t]<<" ";
        t++;
    }
    cout<<endl;
    if(f.Size>6){
        cout<<"1��ռ�ÿ飺"<<endl;
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
        //cout<<"2��ռ�ÿ飺"<<endl;
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
/*************************����ڴ�����ı仯******************************/
void Dis_change(struct FCB f)
{
    cout<<"�����ı仯"<<endl;
    Dis_Empty();//���п�仯
    Dis_Index(f);
}
/**********************������������ļ�***************************/
void Format()
{
    //Ϊ��Ŀ¼�����ڴ�
    cout<<"������Ŀ¼"<<endl;
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
    cout<<"��������ĸ�ʽ���£�"<<endl;
    cout<<"��ʾĿ¼����-----------dir"<<endl;
    cout<<"��ʾĿ¼�������ļ�-----dir a*"<<endl;
    cout<<"����Ŀ¼---------------md name"<<endl;
    cout<<"ɾ��Ŀ¼---------------del root/user/j1"<<endl;
    cout<<"�������ļ�-------------mk nodname"<<endl;
    cout<<"�����ļ�---------------mk nodname 10"<<endl;
    cout<<"ɾ���ļ�---------------del nodname"<<endl;
    cout<<"�����ϼ�Ŀ¼-----------cd.."<<endl;
    cout<<"���뵱ǰĿ¼j1---------cd root/user/j1"<<endl;
    cout<<"�༭�ļ�����-----------edit j1"<<endl;
    cout<<"��ʾ�ļ�����-----------cat j1"<<endl;
}

int split(char *str,char result[10][40],char chr)//��һ���ַ����ֽ�Ϊ����Ӵ�
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
void Trim(char* s)//�����������ߵĿհ״�
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
/***************���¸�Ŀ¼��С,�����ش���ǰ·����FCB*******************/
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
/*********����ļ���Ŀ¼�ڵ�ǰĿ¼���Ƿ����**********/
bool IS_Exit(char* str,int k)
{
    struct FCB *f=Update_Father_Size(0);
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str)==0&&f->son[i].type==k){
            return true;//�ļ���Ŀ¼�Ѵ���
        }
    }
    return false;
}
/**************����FCB 0--Ŀ¼ 1--�ļ�*************/
void Make_f(char str[10][40],int len,int k)
{
    bool bb=IS_Exit(str[1],k);
    if(bb){
        if(k==1)
            cout<<"����Ŀ¼�Ѵ���"<<endl;
        else
            cout<<"�����ļ��Ѵ���"<<endl;
        return ;
    }
    struct FCB f;
    strcat(f.name,str[1]);//ΪFCB����ֵ
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
            cout<<"�ļ���СԽ��"<<endl;
            return;
        }
        f.Size=x;
    }
    bool b=Allocation_Memory(f,f.Size);//�����ڴ�
    if(!b){
        cout<<"�ڴ����ʧ��!\n";
        return;
    }else{
        struct FCB *R=Update_Father_Size(f.Size);//�𼶸��¸�Ŀ¼��С
        //cout<<"R = "<<R->name<<endl;
//        for(int i=0;i<R->son.size();i++){
//            if(strcmp(R->son[i],str[1])==0)
//        }
        R->son.push_back(f);//��f���ڵ�ǰĿ¼��
        if(f.type==1){//�½���Ŀ¼Ҫ���µ�ǰ·��
            strcat(current_directory,"/");
            strcat(current_directory,f.name);
        }
    }
    Dis_change(f);
}
/********************��***********************/
void TREE(struct FCB f,bool *v,int v_L)
{
    int i;
    //�ļ�����
    for(i=0;i<v_L;i++){//���|�Ϳո�
        if(v[i]){
            cout<<"|";
        }else{
            cout<<" ";
        }
    }
    if(v[i])
        cout<<"|";
    cout<<"_"<<f.name;//�����ǰFCB������
    if(f.son.size()!=0)
        cout<<"_";
    cout<<endl;
    //Ŀ¼����
    if(f.type==1){//����Ŀ¼�ݹ����
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
bool Is_right_form(char* s1,char* s2)//�ж�S1���ǲ���S2���ĸ�ʽ *a*bc*,?a?b
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
    if(i==strlen(s2)&&s2[i-1]!='*'&&k<strlen(s1))//s2ɨ�����һ������s1ûɨ�꣬����s2���һ������*
        return false;
    if(i==strlen(s2)&&s2[i-1]=='?'&&k!=strlen(s1))
        return false;
    return true;
}
/**************�г���ǰĿ¼�ڵ��ļ���Ŀ¼***********************/
void List_FCB(char *s,int k)//k==1--��ʾĿ¼����,k==2��ʾĿ¼����������
{
   cout<<"��ǰĿ¼���ݣ�"<<endl;
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
/******************�༭�ļ�**********************/
void Edit(char str[10][40])
{
    struct FCB *f=Update_Father_Size(0);
    int flag=0;
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str[1])==0){
            cout<<"�����ļ����ݣ�"<<endl;
            flag=1;
            gets(f->son[i].content);
            break;
        }
    }
    if(!flag){
        cout<<"���ļ�������"<<endl;
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
        cout<<"���ļ�������"<<endl;
    }
}
/*****************�����ڴ�*******************/
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
/******************ɾ���ļ���Ŀ¼************************/
void Del(char str[10][40])
{
    cout<<"*******************�����ڴ�************************"<<endl;
    if(strcmp(str[1],".")==0){
        cout<<"��Ŀ¼���ܱ�ɾ��"<<endl;
        return;
    }
    struct FCB *f=Update_Father_Size(0);
    //cout<<"f.name = "<<f->name<<endl;
    int flag=0;
    for(int i=0;i<f->son.size();i++){
        if(strcmp(f->son[i].name,str[1])==0){//�ҵ�Ҫɾ�����ļ���Ŀ¼
            //cout<<"�ҵ�Ҫɾ�����ļ���Ŀ¼"<<f->name<<" - > "<<f->son[i].name<<endl;
            flag=1;
            if((f->son[i].type==0)||(f->son[i].type==1&&f->son[i].son.size()==0)){//��֤�ǿ�Ŀ¼���ļ�

                vector<int> x;
                int f_Size=f->son[i].Size;
                for(int yy=0;yy<f_Size;yy++){//һ����Size�����X��
                    //cout<<" j = "<<yy<<"   �ļ���С��"<<f->son[i].Size<<endl;
                    if(yy==0){
                        //cout<<"��һ��"<<endl;
                        x.push_back(f->son[i].Index.i0.number);
                    }else if(yy>=1&&yy<=5){
                        //cout<<"��2��"<<endl;
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
                    //cout<<"ѭ��"<<endl;
                }
                struct FCB *t=Update_Father_Size(0-f->son[i].Size);//���¸�Ŀ¼��С
                //�Ƴ�FCB
                vector<struct FCB>::iterator it;
                for(it=f->son.begin();it!=f->son.end();it++){
                    if(strcmp((*it).name,str[1])==0){//�ҵ���FCB�ĵ�����
                        break;
                    }
                }
                //cout<<"FCB.size = "<<f->son.size()<<endl;
                f->son.erase(it);
                //cout<<"ɾ��Ԫ�غ�Ŀ¼��FCB���� = "<<f->son.size()<<endl;
                //�����ڴ�
                FREE_Memory(x);
                Empty_Block_number+=f->son[i].Size;
                Dis_Empty();
                cout<<"*******************�����ڴ�ɹ�************************"<<endl;
            }else{
                cout<<"�ǿ�"<<endl;
            }
            break;
        }
    }
    if(!flag){
        cout<<"********���ļ������ڻ����ڴ�ʧ��********"<<endl;
    }
}
/***************�����ϼ�Ŀ¼*****************/
void Cd_father()
{
    cout<<"��ʼ�����ϼ�Ŀ¼"<<endl;
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
    cout<<"�����ϼ�Ŀ¼�ɹ�"<<endl;
}
void Cd_absolute_directory(char str[10][40])
{
    char s[10][40];
    int len=split(str[1],s,'/');
    if(s[0][0]!='.'){
        cout<<"·��û�Ӹ�Ŀ¼��ʼ��·������"<<endl;
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
            cout<<"��·������"<<endl;
            return ;
        }
    }
    strcpy(current_directory,str[1]);
    cout<<"�л�·���ɹ�!"<<endl;
}
void Function()
{
    INIT();
    cout<<"�������ӿ��п飺"<<endl;
    Dis_Empty();
    cout<<"��ʼ����������ļ�(Ϊ��Ŀ¼�����ڴ�):"<<endl;
    Format();
    help();
    char cmd[400];
    char str[10][40];
    cout<<"�������"<<endl;
    while(1){
        gets(cmd);
        Trim(cmd);
        //cout<<"�������Cmd = "<<cmd<<endl;
        int len=split(cmd,str,' ');//��������Ŀ���,�Կո� Ϊ���ַ�
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
            cout<<"���������ʽ����"<<endl;
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
