#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<string>
#include<strstream>
using namespace std;
struct PCB{
    string Name;
    int start;
    int Size;
    struct PCB *next;
}PCB;
struct Memory{
    int start;
    int Size;
    struct Memory *pre;
    struct Memory *next;
}Memory;
struct Memory *Memory_Head=NULL,*Memory_Tail=NULL;//内存的头尾指针
struct PCB *Head=NULL,*Tail=NULL;//占用的PCB块头尾指针
struct PCB *Ready_Head=NULL,*Ready_Tail=NULL;//就绪进程的头尾指针
struct PCB *Running_Head=NULL;//执行进程的头指针
struct PCB *Block_Head=NULL,*Block_Tail=NULL;//阻塞进程的头尾指针
char* File="log.txt";//日志文件名

void InsertSort()
{
    struct Memory *p,*q,*first,*t;
    if(Memory_Head==Memory_Tail)
        return;
    first=Memory_Head->next;//要扫描的剩余节点头
    Memory_Head->next=NULL;//第一个节点默认有序
    while(first){//剩余节点没到尾
        for(t=first,q=Memory_Head;(q)&&q->Size <= t->Size;p=q,q=q->next);//找到剩余节点first的位置
        first= first->next;//指针后移
        if(q==Memory_Head){
            Memory_Head=t;
            Memory_Head->pre=t;
        }else{
            p->next=t;
            t->pre=p;
        }
        t->next=q;
    }
    Memory_Tail=t;
}

void Display_P(struct PCB *P_Head)//输出PCB链表
{
    struct PCB *p=P_Head;
    char *t;
    FILE* fp=fopen(File,"a");
    if(P_Head==Ready_Head){
        t="Ready Program:";
        fputs(t,fp);
    }
    else if(P_Head==Running_Head)
        fputs("Running Program:",fp);
    else
        fputs("Block Program:",fp);
    while(p)
    {
        cout<<" <"<<p->Name<<","<<p->Size<<"> ";
        fputs(" <",fp);
        fputs(p->Name.c_str(),fp);
        fputs(",",fp);
        strstream ss;
        string s;
        ss<<p->Size;
        ss>>s;
        fputs(s.c_str(),fp);
        fputs("> ",fp);
        p=p->next;
    }
    fputs("\n",fp);
    fclose(fp);
    cout<<endl;
}
void Display_M(struct Memory *P_Head)//输出内存状态
{
    struct Memory *p=P_Head;
    FILE *fp=fopen(File,"a");
    //char* t;
    fputs("Memory:",fp);
    if(Memory_Head==NULL){
        cout<<endl;
        fputs("\n",fp);
        fclose(fp);
        return ;
    }
    cout<<" <"<<p->start<<" , "<<p->Size<<"> ";
    fputs(" <",fp);

    strstream ss,tt;
    string s,t;
    ss<<p->start;
    ss>>s;
    fputs(s.c_str(),fp);
    fputs(",",fp);
    tt<<p->Size;
    tt>>t;
    fputs(t.c_str(),fp);
    fputs("> ",fp);
    p=p->next;
    while(p)
    {
        cout<<" <"<<p->start<<" , "<<p->Size<<"> ";
        fputs(" <",fp);
        strstream sss;
        string st;
        sss<<p->start;
        sss>>st;
        fputs(st.c_str(),fp);
        fputs(",",fp);
        strstream mm;
        string m;
        mm<<p->Size;
        mm>>m;
        fputs(m.c_str(),fp);
        fputs("> ",fp);
        p=p->next;
    }
    fputs("\n",fp);
    fclose(fp);
    cout<<endl;
}
void Display()//输出进程状态
{
    //FILE *fp=fopen(File,"a");
    cout<<"Ready Program:";
    Display_P(Ready_Head);
    cout<<"Running Program:";
    Display_P(Running_Head);
    //system("pause");
    cout<<"Block Program:";
    Display_P(Block_Head);
    cout<<"Memory:";
    Display_M(Memory_Head);
    cout<<"\n"<<endl;
    //写入日志文件

}
bool Is_Program(string name)//判断进程是否已经创建
{
    struct PCB *p;
    p=Ready_Head;
    while(p){
        if(p->Name==name)
            return true;
        p=p->next;
    }
    p=Running_Head;
    while(p){
        if(p->Name==name)
            return true;
        p=p->next;
    }
    p=Block_Head;
    while(p){
        if(p->Name==name)
            return true;
        p=p->next;
    }
    return false;
}
void Create()
{
    struct PCB *p=new struct PCB();
    struct PCB *q;
    struct Memory *m,*n;
    char* t;
    FILE *fp=fopen(File,"a");
    if(!Memory_Head)
    {
        cout<<"No Memory Left!"<<endl;
        fputs("No Memory Left!\n",fp);
        return ;
    }
    cout<<"input Program Name:";
    t="input Program Name:";
    fputs(t,fp);
    cin>>p->Name;
    fputs(p->Name.c_str(),fp);
    //查找进程是否存在
    bool Is=Is_Program(p->Name);
    if(Is){//进程存在，重新输入
        cout<<"Program exist"<<endl;
        fputs("Program exist!\n",fp);
        fclose(fp);
        Create();
        return;
    }
    cout<<"input size:";
    fputs("input size:",fp);
    cin>>p->Size;
    strstream ss;
    string s;
    ss<<p->Size;
    ss>>s;
    //sprintf(t,"%d",p->Size);
    fputs(s.c_str(),fp);
    //分配内存
    n=Memory_Head;
    while(n&&n->Size < p->Size)//找到第一个比P的空间大于等于的内存块
        n=n->next;
    if(n){
        //cout<<"n->start:"<<n->start<<"  n->Size:"<<n->Size<<endl;
        //将P放在内存N里,判断会不会产生系统碎片
        p->start = n->start;//记录进程占用内存起点
        if(n->Size - p->Size <=2){//剩余内存较小，将内存全部给出
            p->Size = n->Size;
            //删除这个内存
            if(n->pre!=Memory_Head){//前面存在内存块就要将前个内存块的next域指向下一个
                n->pre->next = n->next;
                if(n->next){
                    n->next->pre = n->pre->next;
                }
            }else{
                Memory_Head=n->next;//前面没有内存块要改头指针
                //cout<<"Memory_Head==NULL"<<endl;
                if(Memory_Head)
                {
                    Memory_Head->pre=Memory_Head;
                    //cout<<"Memory_Head!=NULL"<<endl;
                }

            }
            delete n;
        }else{
            //cout<<"改变N的起始位置和大小"<<endl;
            n->start = n->start + p->Size;
            n->Size = n->Size - p->Size;
        }
    }else{//没有合适的内存
        cout<<"Out of Memory!"<<endl;
        fputs("Out of Memory!\n",fp);
        fclose(fp);
        return;
    }
    //输出占用内存和剩余内存
    //cout<<"Memory of use:"<<" <"<<p->start<<" , "<<p->Size<<"> \n"<<"Memory of left：";
    //Display_M(Memory_Head);
    p->next=NULL;
    if(Running_Head==NULL){//没有正在执行的进程，创建的进程直接放在执行列表
        Running_Head=p;
    }else{
        if(Ready_Head==NULL){//没有就绪的进程
            Ready_Head=p;
            Ready_Tail=p;
        }else{
            Ready_Tail->next=p;
            Ready_Tail=p;
        }
    }
    cout<<"Program "<<p->Name<<" Create Successful!\n"<<endl;
    fputs("Program ",fp);
    fputs(p->Name.c_str(),fp);
    fputs(" Create Successful!\n",fp);
    fclose(fp);
}
void Time_over()//时间片到
{
    struct PCB *p;
    if(Running_Head){
        if(Ready_Head){//如果没有就绪态进程，就不用执行任何操作了
            //将执行进程回到就绪态
            p=Running_Head;
            Ready_Tail->next=p;
            Ready_Tail=p;
            //将第一个就绪态进程执行
            Running_Head=Ready_Head;
            Ready_Head=Ready_Head->next;
            //cout<<"***********"<<endl;
            Running_Head->next = NULL;
        }
    }else{
        FILE *fp=fopen(File,"a");
        cout<<"There is no Program performed!"<<endl;
        fputs("There is no Program performed!\n",fp);
        fclose(fp);
    }
}
void Program_Block()//进程阻塞
{
    //判断是否有正在执行的进程
    if(Running_Head){
        //判断是否有阻塞的进程
        //cout<<"有正在执行的进程"<<endl;
        if(Block_Head){
            Block_Tail->next=Running_Head;
        }else{
            Block_Head=Running_Head;
        }
        Block_Tail=Running_Head;
        //自动调度，要查看是否有就绪进程
        //cout<<"自动调度"<<endl;
        if(Ready_Head){
            Running_Head=Ready_Head;
            Ready_Head=Ready_Head->next;
            Running_Head->next=NULL;
        }else{
            Running_Head=NULL;
        }
    }else{
        FILE *fp=fopen(File,"a");
        cout<<"There is no Program performed!！"<<endl;
        fputs("There is no Program performed!！\n",fp);
        fclose(fp);

    }
}
void Help()
{
    FILE *fp=fopen(File,"a");
    cout<<"Free scheduling"<<endl;
    cout<<"C/c-----Create Program"<<endl;
    cout<<"T/t-----Time Over"<<endl;
    cout<<"B/b-----Block Program"<<endl;
    cout<<"W/w-----Wake up Program"<<endl;
    cout<<"D/d-----Displsy Program State"<<endl;
    cout<<"E/e-----Over Program\n"<<endl;
    fputs("Free scheduling\nC/c-----Create Program\nT/t-----Time Over\nB/b-----Block Program\nW/w-----Wake up Program\nD/d-----Displsy Program State\nE/e-----Over Program\n",fp);
    fclose(fp);
}
void Program_Over()//终止进程,将正在执行态的进程终止，释放内存，并将就绪态进程放入执行态
{
    struct PCB *p=Running_Head;
    struct Memory *m=Memory_Head;
    struct Memory *p1=NULL,*p2=NULL;//检测上空下空

    if(Running_Head){//有正在执行的进程
        //释放内存
        //检测上空
        while(m){
            if(m->start + m->Size ==p->start){
                p1=m;
                break;
            }
            m=m->next;
        }
        m=Memory_Head;
        while(m){
            if(m->start == p->Size + p->start){
                p2=m;
                break;
            }
            m=m->next;
        }
        if(!p1&&!p2){//上占下占,直接释放内存
            m = new struct Memory();
            m->start = p->start;
            m->Size = p->Size;
            if(!Memory_Head){
                m->pre=m;
                Memory_Head=m;
                Memory_Tail=m;
                Memory_Tail->next=NULL;
            }else{
                m->pre = Memory_Tail;
                Memory_Tail->next = m;
                Memory_Tail = m;
                Memory_Tail->next = NULL;
            }

//            if(!Memory_Head)
//                Memory_Head=Memory_Tail;
        }else if(p1&&!p2){//上空下占，修改上的size
            p1->Size += p->Size;
        }else if(!p1&&p2){//上占下空,修改下的start和size
            p2->start -= p->Size;
            p2->Size += p->Size;
        }else{//上空下空
            cout<<"p1->size:"<<p1->Size<<endl;
            cout<<"p2->size:"<<p2->Size<<endl;
            p1->Size = p1->Size +p2->Size+ p->Size;
            /*
            if(p1==Memory_Head)
                cout<<"P1==Memory_Head"<<endl;
            cout<<"p1->Size: "<<p1->Size<<endl;
            */
            p1->next=p2->next;
            if(p2->next)
                p2->next->pre = p1;
            else
                Memory_Tail=p1;
            delete p2;
        }

        delete p;
        //cout<<"paixu"<<endl;
        InsertSort();
        //cout<<"paixuwancheng"<<endl;
        //将就绪态进程执行
        if(Ready_Head){
           Running_Head=Ready_Head;
           Ready_Head=Ready_Head->next;
           Running_Head->next = NULL;
        }else{
            Running_Head=NULL;
            //cout<<"&&&&&&&&&&&&&&77\n";
        }
    }else{
        FILE *fp=fopen(File,"a");
        fputs("There is no Program performed！\n",fp);
        fclose(fp);
        cout<<"There is no Program performed！\n"<<endl;
    }
}
void Program_Wake()//进程唤醒，将阻塞态的进程放入就绪态
{
    struct PCB *p;
    if(Block_Head){//有阻塞进程
        p=Block_Head;
        Block_Head=Block_Head->next;
        p->next = NULL;
        if(!Ready_Head){//没有就绪态进程
            if(!Running_Head){//没有执行态进程，直接将唤醒进程放入执行态
                Running_Head = p;
            }else{
                Ready_Head=p;
                Ready_Tail=p;
            }
        }else{//插到就绪态队列尾部
            Ready_Tail->next = p;
            Ready_Tail = p;
        }
    }else{//没有阻塞态的进程
        FILE *fp=fopen(File,"a");
        fputs("There is no Program Blocked！\n",fp);
        fclose(fp);
        cout<<"There is no program Blocked\n"<<endl;
    }
}
void Function()
{
    struct Memory *M=new struct Memory();
    char Op;
    FILE *fp;
    fp=fopen(File,"w");
    cout<<"Please input two number instead of the begin and the size of the system:"<<endl;
    fputs("Please input two number instead of the begin and the size of the system:",fp);
    cin>>M->start>>M->Size;
    strstream ss;
    string s;
    ss<<M->start;
    ss>>s;
    fputs(s.c_str(),fp);
    fputc(' ',fp);
    ss<<M->Size;
    ss>>s;
    fputs(s.c_str(),fp);
    fputs("\n",fp);
    fclose(fp);
    Memory_Head=M;
    Memory_Tail=M;
    Memory_Head->pre=Memory_Head;
    Memory_Tail->next=NULL;
    if(M->start&&M->Size)
    {
        while(1){
            cout<<"Input Operation:(C/c,T/t,B/b,E/e,W/w,H/h,D/d,Q/q)"<<endl;
            cin>>Op;
            fp=fopen(File,"a");
            fputc(Op,fp);
            fputc('\n',fp);
            fclose(fp);
            //cout<<"!!!!!!!!!!!!!!"<<endl;
            //cout<<"OP = "<<Op<<endl;
            switch(Op)
            {
            case 'c':
            case 'C':
                Create();//创建进程
                break;
            case 'T':
            case 't':
                Time_over();//时间片到
                break;
            case 'B':
            case 'b':
                Program_Block();//阻塞进程
                break;
            case 'E':
            case 'e':
                Program_Over();//终止进程
                break;
            case 'W':
            case 'w':
                Program_Wake();//进程唤醒
                break;
            case 'H':
            case 'h':
                Help();//帮助
                break;
            case 'D':
            case 'd':
                Display();//显示进程状态
                break;
            case 'Q':
            case 'q':
                exit(0);
                break;
            default:
                cout<<"Error!\n";
                fp=fopen(File,"a");
                fputs("Error!\n",fp);
                fclose(fp);
            }
        }

    }else{
        fp=fopen(File,"a");
        fputs("Illegal input form,please input again！\n",fp);
        fclose(fp);
        cout<<"Illegal input form,please input again！"<<endl;
        Function();
    }
}
int main()
{
    Function();
    return 0;
}
