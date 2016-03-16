#include<cstdlib>
#include<cstdio>
#include <iostream>
#include<cstring>
#include<iomanip>
using namespace std;
struct PCB{
	char name[10];
	int Size;
	int arrival_time;	//到达时间
	int burst_time;		//服务时间
	int finished_time=0;	//结束运行时间
	int runned_time=0;		//已运行时间
	int T=0;            //周转时间
	float W=0;            //带权周转时间=T/运行时间
	bool Is_do=false;   //是否已经选为最短作业
	struct PCB*next=NULL;
};
struct PCB *All;//所有进程的信息
struct PCB* Find_R_Tail(struct PCB *head)//找到head队列的尾，如果为空返回null
{

    if(head->next==NULL){
        return head;
    }
    struct PCB *p=head->next;
    while(p->next){
        p=p->next;
    }
    return p;
}
void Link(struct PCB *head,struct PCB *p)//将进程P链接到head队列的尾
{
    struct PCB *Tail=Find_R_Tail(head);
    p->next=NULL;
    Tail->next=p;
}
void Init()//初始化进程
{
    All=new struct PCB();
    All->next=NULL;
    struct PCB *p=new struct PCB();
    strcpy(p->name,"A");
    p->arrival_time=0;
    p->burst_time=4;
    Link(All,p);

    p=new struct PCB();
    strcpy(p->name,"B");
    p->arrival_time=1;
    p->burst_time=3;
    Link(All,p);


    p=new struct PCB();
    strcpy(p->name,"C");
    p->arrival_time=2;
    p->burst_time=5;
    Link(All,p);


    p=new struct PCB();
    strcpy(p->name,"D");
    p->arrival_time=3;
    p->burst_time=2;
    Link(All,p);


    p=new struct PCB();
    strcpy(p->name,"E");
    p->arrival_time=4;
    p->burst_time=4;
    Link(All,p);

}
/**********************FCFS****************************/
void FCFS()
{

    struct PCB *ready=new struct PCB(),*blocked=new struct PCB(),*running=new struct PCB(),*finished=new struct PCB();
    int con=0;
    float t=0,w=0;
    struct PCB *q=All->next;
    //为就绪队列赋初值
//    while(q){
//        ready
//    }
    while(q){
        q->finished_time=con+q->burst_time;
        q->T=q->finished_time-q->arrival_time;
        t+=q->T;
        q->W = ((float)q->T /(float)q->burst_time);
        w+=q->W;
        con=q->finished_time;
        q=q->next;
    }
    t/=5;
    w/=(float)5;
    struct PCB *p=All->next;
    while(p){
        cout<<"P->name = "<<p->name<<"   p->arrival = "<<p->arrival_time<<"  p->burst = "<<p->burst_time;
        cout<<"  P->finish_time = "<<setw(3)<<p->finished_time<<"  p-T = "<<setw(3)<<p->T<<"  p->W = "<<p->W<<endl;
        p=p->next;
    }
    cout<<"t = "<<t<<"  w = "<<w<<endl;
}
void Dis_Program(struct PCB *head)
{
    struct PCB *p=head->next;
    while(p){
        cout<<"P->name = "<<p->name<<"   p->arrival = "<<p->arrival_time<<"  p->burst = "<<p->burst_time<<endl;
        p=p->next;
    }
    cout<<endl;
}
void SJF()
{
    int con=0;//当前进程运行到的时间
    float t=0,w=0;
    for(int i=0;i<5;i++){
        struct PCB *p=All->next;
        while(p->Is_do==true&&con>=p->arrival_time)
            p=p->next;
        struct PCB *q=All->next;
        while(q){//选出最短的作业
            if(con>=q->arrival_time&&q!=p&&q->Is_do==false&&q->burst_time < p->burst_time){
                p=q;
            }
            q=q->next;
        }
        p->finished_time = con + p->burst_time;
        p->T = p->finished_time - p->arrival_time;
        t+=p->T;
        p->W = (float)p->T/(float)p->burst_time;
        w+=p->W;
        con=p->finished_time;
        p->Is_do=true;
    }
    t/=5;
    w/=(float)5;
    struct PCB *p=All->next;
    while(p){
        cout<<"P->name = "<<p->name<<"   p->arrival = "<<p->arrival_time<<"  p->burst = "<<p->burst_time;
        cout<<"  P->finish_time = "<<setw(3)<<p->finished_time<<"  p-T = "<<setw(3)<<p->T<<"  p->W = "<<p->W<<endl;
        p=p->next;
    }
    cout<<"t = "<<t<<"  w = "<<w<<endl;
}
/*****************head 队列中删除 p 节点********************/
void Del_P(struct PCB *head,struct PCB *p)
{
    struct PCB *q=head;
    while(q){
        if(q->next==p){
            q->next=q->next->next;
            return;
        }
        q=q->next;
    }
    q=head->next;
    while(q){
        cout<<q->name<<" ";
        q=q->next;
    }
    cout<<endl;
}
/****************显示队列内容***********************/
void Dis_head(struct PCB *head)
{
    struct PCB *p=head->next;
    while(p){
        cout<<p->name<<" ";
        p=p->next;
    }
    cout<<endl;
}
void Dis_Queue(struct PCB *ready,struct PCB *running,struct PCB *block,struct PCB *finish)
{
    cout<<"ready: ";
    Dis_head(ready);
    cout<<"running: ";
    Dis_head(running);
    cout<<"block: ";
    Dis_head(block);
    cout<<"finish: ";
    Dis_head(finish);
    cout<<"*******************************"<<endl;
}
void loop(struct PCB *ready,struct PCB *running,struct PCB *block,struct PCB *finish,int q)
{
    cout<<"********************************"<<endl;
    system("pause");
    struct PCB *p,*pp;
    p=ready->next;
    while(p!=NULL){
        pp=p->next;
        Del_P(ready,p);
        Link(running,p);
        //cout<<"p->burst_time = "<<p->burst_time<<"  q = "<<q<<endl;
        p->burst_time-=q;
        Dis_Queue(ready,running,block,finish);
        if(p->burst_time <= 0){
            Link(finish,p);
        }else{
            Link(block,p);
        }
        running->next=NULL;
        Dis_Queue(ready,running,block,finish);
        p=pp;
        //cout<<"p->name = "<<p->name<<endl;
    }
    if(block->next!=NULL){
        //cout<<"block not null!"<<endl;
        ready->next=block->next;
//        p=ready->next;
//        while(p){
//            cout<<p->name<<" ";
//            p=p->next;
//        }
//        cout<<endl;
        block->next=NULL;
        loop(ready,running,block,finish,q);
    }
}
void Time_Run(int q)//时间片轮转
{
    struct PCB *p;
    struct PCB *pp;
    struct PCB *ready=new struct PCB();
    struct PCB *running=new struct PCB();
    struct PCB *block=new struct PCB();
    struct PCB *finished=new struct PCB();

    //为ready赋初值
    p=All->next;
    int Time_cont=0;
    int Max_time=0;
    while(p){
        pp=new struct PCB();
        strcpy(pp->name,p->name);
        pp->arrival_time=p->arrival_time;
        pp->burst_time=p->burst_time;
        Link(ready,pp);
        p=p->next;
    }
    loop(ready,running,block,finished,q);
}
int main()
{
    Init();
    Dis_Program(All);
    int x;
    //cin>>x;
    cout<<"FCFS："<<endl;
    FCFS();
    system("pause");
    system("cls");
    //cin>>x;
    cout<<"SJF:"<<endl;
    SJF();
    system("pause");
    system("cls");
    cout<<"RR:"<<endl;
    Time_Run(1);
    return 0;
}
