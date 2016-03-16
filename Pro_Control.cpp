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
struct Memory *Memory_Head=NULL,*Memory_Tail=NULL;//�ڴ��ͷβָ��
struct PCB *Head=NULL,*Tail=NULL;//ռ�õ�PCB��ͷβָ��
struct PCB *Ready_Head=NULL,*Ready_Tail=NULL;//�������̵�ͷβָ��
struct PCB *Running_Head=NULL;//ִ�н��̵�ͷָ��
struct PCB *Block_Head=NULL,*Block_Tail=NULL;//�������̵�ͷβָ��
char* File="log.txt";//��־�ļ���

void InsertSort()
{
    struct Memory *p,*q,*first,*t;
    if(Memory_Head==Memory_Tail)
        return;
    first=Memory_Head->next;//Ҫɨ���ʣ��ڵ�ͷ
    Memory_Head->next=NULL;//��һ���ڵ�Ĭ������
    while(first){//ʣ��ڵ�û��β
        for(t=first,q=Memory_Head;(q)&&q->Size <= t->Size;p=q,q=q->next);//�ҵ�ʣ��ڵ�first��λ��
        first= first->next;//ָ�����
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

void Display_P(struct PCB *P_Head)//���PCB����
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
void Display_M(struct Memory *P_Head)//����ڴ�״̬
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
void Display()//�������״̬
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
    //д����־�ļ�

}
bool Is_Program(string name)//�жϽ����Ƿ��Ѿ�����
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
    //���ҽ����Ƿ����
    bool Is=Is_Program(p->Name);
    if(Is){//���̴��ڣ���������
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
    //�����ڴ�
    n=Memory_Head;
    while(n&&n->Size < p->Size)//�ҵ���һ����P�Ŀռ���ڵ��ڵ��ڴ��
        n=n->next;
    if(n){
        //cout<<"n->start:"<<n->start<<"  n->Size:"<<n->Size<<endl;
        //��P�����ڴ�N��,�жϻ᲻�����ϵͳ��Ƭ
        p->start = n->start;//��¼����ռ���ڴ����
        if(n->Size - p->Size <=2){//ʣ���ڴ��С�����ڴ�ȫ������
            p->Size = n->Size;
            //ɾ������ڴ�
            if(n->pre!=Memory_Head){//ǰ������ڴ���Ҫ��ǰ���ڴ���next��ָ����һ��
                n->pre->next = n->next;
                if(n->next){
                    n->next->pre = n->pre->next;
                }
            }else{
                Memory_Head=n->next;//ǰ��û���ڴ��Ҫ��ͷָ��
                //cout<<"Memory_Head==NULL"<<endl;
                if(Memory_Head)
                {
                    Memory_Head->pre=Memory_Head;
                    //cout<<"Memory_Head!=NULL"<<endl;
                }

            }
            delete n;
        }else{
            //cout<<"�ı�N����ʼλ�úʹ�С"<<endl;
            n->start = n->start + p->Size;
            n->Size = n->Size - p->Size;
        }
    }else{//û�к��ʵ��ڴ�
        cout<<"Out of Memory!"<<endl;
        fputs("Out of Memory!\n",fp);
        fclose(fp);
        return;
    }
    //���ռ���ڴ��ʣ���ڴ�
    //cout<<"Memory of use:"<<" <"<<p->start<<" , "<<p->Size<<"> \n"<<"Memory of left��";
    //Display_M(Memory_Head);
    p->next=NULL;
    if(Running_Head==NULL){//û������ִ�еĽ��̣������Ľ���ֱ�ӷ���ִ���б�
        Running_Head=p;
    }else{
        if(Ready_Head==NULL){//û�о����Ľ���
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
void Time_over()//ʱ��Ƭ��
{
    struct PCB *p;
    if(Running_Head){
        if(Ready_Head){//���û�о���̬���̣��Ͳ���ִ���κβ�����
            //��ִ�н��̻ص�����̬
            p=Running_Head;
            Ready_Tail->next=p;
            Ready_Tail=p;
            //����һ������̬����ִ��
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
void Program_Block()//��������
{
    //�ж��Ƿ�������ִ�еĽ���
    if(Running_Head){
        //�ж��Ƿ��������Ľ���
        //cout<<"������ִ�еĽ���"<<endl;
        if(Block_Head){
            Block_Tail->next=Running_Head;
        }else{
            Block_Head=Running_Head;
        }
        Block_Tail=Running_Head;
        //�Զ����ȣ�Ҫ�鿴�Ƿ��о�������
        //cout<<"�Զ�����"<<endl;
        if(Ready_Head){
            Running_Head=Ready_Head;
            Ready_Head=Ready_Head->next;
            Running_Head->next=NULL;
        }else{
            Running_Head=NULL;
        }
    }else{
        FILE *fp=fopen(File,"a");
        cout<<"There is no Program performed!��"<<endl;
        fputs("There is no Program performed!��\n",fp);
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
void Program_Over()//��ֹ����,������ִ��̬�Ľ�����ֹ���ͷ��ڴ棬��������̬���̷���ִ��̬
{
    struct PCB *p=Running_Head;
    struct Memory *m=Memory_Head;
    struct Memory *p1=NULL,*p2=NULL;//����Ͽ��¿�

    if(Running_Head){//������ִ�еĽ���
        //�ͷ��ڴ�
        //����Ͽ�
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
        if(!p1&&!p2){//��ռ��ռ,ֱ���ͷ��ڴ�
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
        }else if(p1&&!p2){//�Ͽ���ռ���޸��ϵ�size
            p1->Size += p->Size;
        }else if(!p1&&p2){//��ռ�¿�,�޸��µ�start��size
            p2->start -= p->Size;
            p2->Size += p->Size;
        }else{//�Ͽ��¿�
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
        //������̬����ִ��
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
        fputs("There is no Program performed��\n",fp);
        fclose(fp);
        cout<<"There is no Program performed��\n"<<endl;
    }
}
void Program_Wake()//���̻��ѣ�������̬�Ľ��̷������̬
{
    struct PCB *p;
    if(Block_Head){//����������
        p=Block_Head;
        Block_Head=Block_Head->next;
        p->next = NULL;
        if(!Ready_Head){//û�о���̬����
            if(!Running_Head){//û��ִ��̬���̣�ֱ�ӽ����ѽ��̷���ִ��̬
                Running_Head = p;
            }else{
                Ready_Head=p;
                Ready_Tail=p;
            }
        }else{//�嵽����̬����β��
            Ready_Tail->next = p;
            Ready_Tail = p;
        }
    }else{//û������̬�Ľ���
        FILE *fp=fopen(File,"a");
        fputs("There is no Program Blocked��\n",fp);
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
                Create();//��������
                break;
            case 'T':
            case 't':
                Time_over();//ʱ��Ƭ��
                break;
            case 'B':
            case 'b':
                Program_Block();//��������
                break;
            case 'E':
            case 'e':
                Program_Over();//��ֹ����
                break;
            case 'W':
            case 'w':
                Program_Wake();//���̻���
                break;
            case 'H':
            case 'h':
                Help();//����
                break;
            case 'D':
            case 'd':
                Display();//��ʾ����״̬
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
        fputs("Illegal input form,please input again��\n",fp);
        fclose(fp);
        cout<<"Illegal input form,please input again��"<<endl;
        Function();
    }
}
int main()
{
    Function();
    return 0;
}
