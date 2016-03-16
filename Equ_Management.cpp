#include<iostream>
#include<cstdlib>
#include<cstring>
#include<vector>
#include<map>
using namespace std;
/*******************����ṹ��**************************/

struct Node{
    string name;//����
    string progress;//ռ�øýڵ�Ľ���
    vector<string> waitinglist;//�ȴ��ýڵ�Ľ���
    string IO;//�豸����
    struct Node *next;//��һ���ڵ�
    struct Node *parent;//���ڵ�
};
/*******************����ȫ�ֱ���**************************/
struct Node *DCTs;//�豸����ͷ�ڵ�
struct Node *COCTs;//����������ͷ�ڵ�
struct Node *CHCTs;//ͨ������ͷ�ڵ�
/*******************����豸������ͼ**************************/
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
/*******************��ʼ��**************************/
void INIT()
{
    struct Node *ch1,*ch2,*co1,*co2,*co3,*key,*mouse,*printer,*display;
    /********����ͨ��********/
    CHCTs=(struct Node*)new struct Node();
    ch1=(struct Node*)new struct Node();
    ch1->name="ch1";
    CHCTs->next=ch1;
    ch2=(struct Node*)new struct Node();
    ch2->name="ch2";
    ch1->next=ch2;
    ch2->next=NULL;
    /********����������********/
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
    /********�ĸ��豸********/
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
/*************************�ж��ǲ��������У����ǣ�ͨ��m���ؽڵ�*****************************/
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
/*******************��ʾ�ڵ�����***********************/
void Disp(struct Node* head)
{
    struct Node* p=head->next;
    while(p){
        cout<<p->name<<" ";
        p=p->next;
    }
    cout<<endl;
}
void ADD_CH(struct Node* p)//Ϊͨ��P��ӿ�����
{
    string name;
    struct Node* q;
    cout<<"\n����ͨ����\n";
    Disp(CHCTs);
    cout<<"����ѡ���ͨ����";
    cin>>name;
    bool bo = IS_inNode(name,CHCTs,q);
    if(!bo){
        cout<<"����������������룡"<<endl;
        ADD_CH(p);
        return;
    }else{
        p->parent=q;
    }
}
void ADD_co(struct Node* p)//Ϊ�豸P��ӿ�����
{
    struct Node* q;
    cout<<"\n���п�������\n";
    Disp(COCTs);
    cout<<endl;
    string name;
    cout<<"�����豸���ӵ��Ŀ��������֣���û�����п������У�ֱ�������µĿ���������";
    cin>>name;
    bool bo = IS_inNode(name,COCTs,q);
    if(!bo){//�����������ڣ�����µĿ�����
        q=(struct Node*)new struct Node();
        q->name=name;
        p->parent=q;//�豸pָ�������q
        //���������������������
        p=COCTs->next;
        while(p->next){
            p=p->next;
        }
        p->next=q;
        q->next=NULL;
        ADD_CH(q);
    }else{//�������������ڣ�ֻ�轫�豸ָ�������
        p->parent=q;
    }
}
void ADD()
{
    struct Node* p=(struct Node*)new struct Node();
    cout<<"������ӵ��豸���ƣ�";
    cin>>p->name;
    //���豸��ӵ��豸����
    struct Node* q=DCTs->next;
    while(q->next){
        q=q->next;
    }
    q->next=p;
    p->next=NULL;
    cout<<"��������ӵ��豸���ͣ�(I/O)";
    cin>>p->IO;
    ADD_co(p);
    cout<<"\n�����豸�������Ϣ��";
    Dis(p);
}
/*********************�ڵ�Ľ���ռ�����*******************************/
void display1(struct Node* head)
{
    struct Node* p=head->next;
    while(p){
        cout<<p->name<<":ִ�н��̣�"<<p->progress<<"\n    �ȴ����̣�";
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
//�жϸ��豸���͵��Ƿ��п����豸,û�оͷ��ظ������豸�ĵ�һ��
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
/******************�����豸**********************/
void APPLY()
{
    string pname,s;
    struct Node* p;
    cout<<"�����豸���ͻ����ƣ�";
    cin>>s;
    cout<<"�����������";
    cin>>pname;
    //�豸������
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
    if(p->progress.size()){//�豸�����У����̵ȴ�
        p->waitinglist.push_back(pname);
    }else{//�豸����ռ���豸
        p->progress=pname;
        if(p->parent->progress.size()){//�����������У����̵ȴ�
            p->parent->waitinglist.push_back(pname);
        }else{//����������ռ�ÿ�����
            p->parent->progress=pname;
            if(p->parent->parent->progress.size()){//ͨ�������У����̵ȴ�
                p->parent->parent->waitinglist.push_back(pname);
            }else{//ͨ������ռ��ͨ��
                p->parent->parent->progress=pname;
            }
        }
    }
    display2();
}
/***********************�����豸******************************/
void RELEASE()
{
    string s;
    cout<<"������յ��豸��";
    cin>>s;
    //�ҵ��豸d
    struct Node* d=DCTs->next;
    while(d){
        if(d->name==s){
            break;
        }
        d=d->next;
    }
    if(d->progress==d->parent->parent->progress){//�豸�ɻ��գ�CH��CO��Dռ�ý��̶���p
        if(!d->waitinglist.empty()){//�豸�ĵȴ����в��գ�Ϊ���̷����豸����������ͨ��
            d->progress=*d->waitinglist.begin();
            d->waitinglist.erase(d->waitinglist.begin());

        }else{
            d->progress="";
        }
        //���������
        d->parent->waitinglist.push_back(d->progress);
        d->parent->progress=*d->parent->waitinglist.begin();
        d->parent->waitinglist.erase(d->parent->waitinglist.begin());
        //����ͨ��
        d->parent->parent->waitinglist.push_back(d->parent->progress);
        d->parent->parent->progress=*d->parent->parent->waitinglist.begin();
        d->parent->parent->waitinglist.erase(d->parent->parent->waitinglist.begin());
        cout<<"���ճɹ���"<<endl;
        display2();
    }
}
/**********************ɾ���豸***********************/
void DEL()
{
    string s;
    cout<<"����ɾ�����豸���ƣ�";
    cin>>s;
    struct Node* p=DCTs->next;
    while(p){
        if(p->name==s)
            break;
        p=p->next;
    }
    //cout<<"***********************2"<<"p->name: "<<p->name<<endl;
    if(p->progress.length()){//�豸������
        //cout<<"***********************3"<<endl;
        if(p->progress==p->parent->progress){//����ռ�ÿ����������ȴ�����ռ�ÿ�����
            if(!p->parent->waitinglist.empty()){//Ϊ��һ���ȴ����̷���CO,CH
                p->parent->progress=*p->parent->waitinglist.begin();
                p->parent->waitinglist.erase(p->parent->waitinglist.begin());
                p->parent->parent->waitinglist.push_back(p->parent->progress);
            }else{
                p->parent->progress="";
            }
            if(p->parent->parent->progress==p->progress){//����ռ��ͨ��������һ���ȴ����н���ռ��ͨ��
                if(!p->parent->parent->waitinglist.empty()){//Ϊ��һ���ȴ����̷���CH
                    p->parent->parent->progress=*p->parent->parent->waitinglist.begin();
                    p->parent->parent->waitinglist.erase(p->parent->parent->waitinglist.begin());
                }else{
                    p->parent->parent->progress="";
                }
            }else{//��CH�ȴ�������ɾ����
                vector<string>::iterator it;
                for(it=p->parent->parent->waitinglist.begin();it!=p->parent->parent->waitinglist.end();it++){
                    if(*it==p->progress)
                        break;
                }
                p->parent->parent->waitinglist.erase(it);
            }
        }else{//��CO�ȴ�������ɾ����
            vector<string>::iterator it;
            for(it=p->parent->waitinglist.begin();it!=p->parent->waitinglist.end();it++){
                if(*it==p->progress)
                    break;
            }
            p->parent->waitinglist.erase(it);
        }
    }
    //ɾ���豸Pǰ��������
    struct Node* q=DCTs;
    while(q->next!=p)
        q=q->next;
    q->next=p->next;
    //ɾ�����豸Ψһ������CO
    q=DCTs->next;
    int i=0;
    while(q->parent==p->parent)
        i++;
    if(i==1){
        //ɾ��������CO
        struct Node* q=COCTs;
        while(q->next!=p->parent)
            q=q->next;
        q->next=q->next->next;
        free(p->parent);
    }
    free(p);
    cout<<"��ʾ����"<<endl;
    Display(DCTs);
    cout<<"��ʾ����ִ�������"<<endl;
    display2();
}
void OPE()
{
    char OP;
    cout<<"\n\���������";
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
            cout<<"���������������룡\n";
            break;
    }
    OPE();

}
void Function()
{
    INIT();
    cout<<"������"<<endl;
    Display(DCTs);
    OPE();
}
int main()
{
    Function();
    return 0;
}
