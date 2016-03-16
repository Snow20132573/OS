#include<iostream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<map>
using namespace std;
/*******************定义结构体**************************/

struct Node{
    string name;//名称
    string progress;//占用该节点的进程
    vector<string> waitinglist;//等待该节点的进程
    string IO;//设备类型
    struct Node *next;//下一个节点
    struct Node *parent;//父节点
};
/*******************定义全局变量**************************/
struct Node *DCTs;//设备队列头节点
struct Node *COCTs;//控制器队列头节点
struct Node *CHCTs;//通道队列头节点
/*******************输出设备的链接图**************************/
void Dis(struct Node* p)
{
    cout<<p->name<<" -> "<<p->parent->name<<" -> "<<p->parent->parent->name<<endl;
}
void Display(struct Node* head)
{
    struct Node* p=head->next;
    while(p){
        Dis(p);
        p=p->next;
    }
}
/*******************初始化**************************/
void INIT()
{
    struct Node *ch1,*ch2,*co1,*co2,*co3,*key,*mouse,*printer,*display;
    /********两个通道********/
    CHCTs=(struct Node*)new struct Node();
    ch1=(struct Node*)new struct Node();
    ch1->name="ch1";
    CHCTs->next=ch1;
    ch2=(struct Node*)new struct Node();
    ch2->name="ch2";
    ch1->next=ch2;
    ch2->next=NULL;
    /********三个控制器********/
    COCTs=(struct Node*)new struct Node();
    co1=(struct Node*)new struct Node();
    co1->name="co1";
    co1->parent=ch1;
    COCTs->next=co1;
    co2=(struct Node*)new struct Node();
    co2->name="co2";
    co2->parent=ch1;
    co1->next=co2;
    co3=(struct Node*)new struct Node();
    co3->name="co3";
    co3->parent=ch2;
    co2->next=co3;
    co3->next=NULL;
    /********四个设备********/
    DCTs=(struct Node*)new struct Node();
    key=(struct Node*)new struct Node();
    key->name="key";
    key->IO="I";
    key->parent=co1;
    DCTs->next=key;
    mouse=(struct Node*)new struct Node();
    mouse->name="mouse";
    mouse->IO="I";
    mouse->parent=co2;
    key->next=mouse;
    printer=(struct Node*)new struct Node();
    printer->name="printer";
    printer->IO="O";
    printer->parent=co3;
    mouse->next=printer;
    display=(struct Node*)new struct Node();
    display->name="display";
    display->IO="O";
    display->parent=co1;
    printer->next=display;
    display->next-NULL;
}
/*************************判断是不是链表中，若是，通过m返回节点*****************************/
bool IS_inNode(string name,struct Node* head,struct Node* &m)
{
    struct Node* q=head->next;
    while(q){
        if(q->name==name){
            m=q;
            return true;
        }
        q=q->next;
    }
    return false;
}
/*******************显示节点名称***********************/
void Disp(struct Node* head)
{
    struct Node* p=head->next;
    while(p){
        cout<<p->name<<" ";
        p=p->next;
    }
    cout<<endl;
}
void ADD_CH(struct Node* p)//为通道P添加控制器
{
    string name;
    struct Node* q;
    cout<<"\n现有通道：\n";
    Disp(CHCTs);
    cout<<"输入选择的通道：";
    cin>>name;
    bool bo = IS_inNode(name,CHCTs,q);
    if(!bo){
        cout<<"输入错误，请重新输入！"<<endl;
        ADD_CH(p);
        return;
    }else{
        p->parent=q;
    }
}
void ADD_co(struct Node* p)//为设备P添加控制器
{
    struct Node* q;
    cout<<"\n现有控制器：\n";
    Disp(COCTs);
    cout<<endl;
    string name;
    cout<<"输入设备连接到的控制器名字，若没在现有控制器中，直接输入新的控制器名：";
    cin>>name;
    bool bo = IS_inNode(name,COCTs,q);
    if(!bo){//控制器不存在，添加新的控制器
        q=(struct Node*)new struct Node();
        q->name=name;
        p->parent=q;//设备p指向控制器q
        //将控制器加入控制器队列
        p=COCTs->next;
        while(p->next){
            p=p->next;
        }
        p->next=q;
        q->next=NULL;
        ADD_CH(q);
    }else{//控制器如若存在，只需将设备指向控制器
        p->parent=q;
    }
}
void ADD()
{
    struct Node* p=(struct Node*)new struct Node();
    cout<<"输入添加的设备名称：";
    cin>>p->name;
    //将设备添加到设备队列
    struct Node* q=DCTs->next;
    while(q->next){
        q=q->next;
    }
    q->next=p;
    p->next=NULL;
    cout<<"输入所添加的设备类型：(I/O)";
    cin>>p->IO;
    ADD_co(p);
    cout<<"\n增加设备的相关信息：";
    Dis(p);
}
/*********************节点的进程占用情况*******************************/
void display1(struct Node* head)
{
    struct Node* p=head->next;
    while(p){
        cout<<p->name<<":执行进程："<<p->progress<<"\n    等待进程：";
        for(vector<string>::iterator it=p->waitinglist.begin();it!=p->waitinglist.end();it++){
            cout<<*it<<" ";
        }
        p=p->next;
        cout<<endl;
    }
    cout<<endl;
}
void display2()
{
    display1(CHCTs);
    display1(COCTs);
    display1(DCTs);
}
//判断该设备类型的是否有空闲设备,没有就返回该类型设备的第一个
struct Node* IS_emp(string s)
{
    struct Node* p=DCTs->next;
    while(p){
        if(p->IO==s&&p->progress.size()==0){
            return p;
        }
        p=p->next;
    }
    p=DCTs->next;
    while(p){
        if(p->IO==s){
            return p;
        }
        p=p->next;
    }
}
/******************申请设备**********************/
void APPLY()
{
    string pname,s;
    struct Node* p;
    cout<<"输入设备类型或名称：";
    cin>>s;
    cout<<"输入进程名：";
    cin>>pname;
    //设备独立性
    if(s=="I"||s=="O"){
        p=IS_emp(s);
    }else{
        p=DCTs->next;
        while(p){
            if(p->name==s){
                break;
            }
            p=p->next;
        }
    }
    if(p->progress.size()){//设备不空闲，进程等待
        p->waitinglist.push_back(pname);
    }else{//设备空闲占用设备
        p->progress=pname;
        if(p->parent->progress.size()){//控制器不空闲，进程等待
            p->parent->waitinglist.push_back(pname);
        }else{//控制器空闲占用控制器
            p->parent->progress=pname;
            if(p->parent->parent->progress.size()){//通道不空闲，进程等待
                p->parent->parent->waitinglist.push_back(pname);
            }else{//通道空闲占用通道
                p->parent->parent->progress=pname;
            }
        }
    }
    display2();
}
/***********************回收设备******************************/
void RELEASE()
{
    string s;
    cout<<"输入回收的设备：";
    cin>>s;
    //找到设备d
    struct Node* d=DCTs->next;
    while(d){
        if(d->name==s){
            break;
        }
        d=d->next;
    }
    if(d->progress==d->parent->parent->progress){//设备可回收，CH，CO，D占用进程都是p
        if(!d->waitinglist.empty()){//设备的等待队列不空，为进程分配设备，控制器，通道
            d->progress=*d->waitinglist.begin();
            d->waitinglist.erase(d->waitinglist.begin());

        }else{
            d->progress="";
        }
        //分配控制器
        d->parent->waitinglist.push_back(d->progress);
        d->parent->progress=*d->parent->waitinglist.begin();
        d->parent->waitinglist.erase(d->parent->waitinglist.begin());
        //分配通道
        d->parent->parent->waitinglist.push_back(d->parent->progress);
        d->parent->parent->progress=*d->parent->parent->waitinglist.begin();
        d->parent->parent->waitinglist.erase(d->parent->parent->waitinglist.begin());
        cout<<"回收成功！"<<endl;
        display2();
    }
}
/**********************删除设备***********************/
void DEL()
{
    string s;
    cout<<"输入删除的设备名称：";
    cin>>s;
    struct Node* p=DCTs->next;
    while(p){
        if(p->name==s)
            break;
        p=p->next;
    }
    //cout<<"***********************2"<<"p->name: "<<p->name<<endl;
    if(p->progress.length()){//设备不空闲
        //cout<<"***********************3"<<endl;
        if(p->progress==p->parent->progress){//进程占用控制器，将等待队列占用控制器
            if(!p->parent->waitinglist.empty()){//为第一个等待进程分配CO,CH
                p->parent->progress=*p->parent->waitinglist.begin();
                p->parent->waitinglist.erase(p->parent->waitinglist.begin());
                p->parent->parent->waitinglist.push_back(p->parent->progress);
            }else{
                p->parent->progress="";
            }
            if(p->parent->parent->progress==p->progress){//进程占用通道，将第一个等待队列进程占用通道
                if(!p->parent->parent->waitinglist.empty()){//为第一个等待进程分配CH
                    p->parent->parent->progress=*p->parent->parent->waitinglist.begin();
                    p->parent->parent->waitinglist.erase(p->parent->parent->waitinglist.begin());
                }else{
                    p->parent->parent->progress="";
                }
            }else{//在CH等待队列中删进程
                vector<string>::iterator it;
                for(it=p->parent->parent->waitinglist.begin();it!=p->parent->parent->waitinglist.end();it++){
                    if(*it==p->progress)
                        break;
                }
                p->parent->parent->waitinglist.erase(it);
            }
        }else{//在CO等待队列中删进程
            vector<string>::iterator it;
            for(it=p->parent->waitinglist.begin();it!=p->parent->waitinglist.end();it++){
                if(*it==p->progress)
                    break;
            }
            p->parent->waitinglist.erase(it);
        }
    }
    //删除设备P前连接链表
    struct Node* q=DCTs;
    while(q->next!=p)
        q=q->next;
    q->next=p->next;
    //删除与设备唯一相连的CO
    q=DCTs->next;
    int i=0;
    while(q->parent==p->parent)
        i++;
    if(i==1){
        //删除控制器CO
        struct Node* q=COCTs;
        while(q->next!=p->parent)
            q=q->next;
        q->next=q->next->next;
        free(p->parent);
    }
    free(p);
    cout<<"显示链表："<<endl;
    Display(DCTs);
    cout<<"显示进程执行情况："<<endl;
    display2();
}
void OPE()
{
    char OP;
    cout<<"\n\输入操作：";
    cin>>OP;
    cout<<"OP = "<<OP<<endl;
    switch (OP){
        case 'A':
            ADD();
            break;
        case 'D':
            DEL();
            break;
        case 'S':
            APPLY();
            break;
        case 'R':
            RELEASE();
            break;
        case 'E':
            exit(0);
            break;
        default:
            cout<<"输入有误，重新输入！\n";
            break;
    }
    OPE();

}
void Function()
{
    INIT();
    cout<<"表单链表："<<endl;
    Display(DCTs);
    OPE();
}
int main()
{
    Function();
    return 0;
}
