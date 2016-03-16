#include<iomanip>
#include<iostream>
#include<cstdio>
#include<time.h>
#include<cstdlib>
using namespace std;
int Block_Size;//���С
int Page;//ҳ��
int Block_Num;//����
int Total;//�߼���ַ�ܸ���
int Pos[8][8],Pos1[8][8];//λʾͼ
int Logical_Adress;//�߼���ַ
int P;//ҳ��
int W;//ҳ�ڵ�ַ
struct node//ҳ��
{
    int Block;//���
    bool Flag=0;//״̬λ
};
struct node F_Page[100];//FIFO��ҳ��
struct node L_Page[100];//LRU��ҳ��
char F_Stack[100];//FIFO��ջ
char L_Stack[100];//LRU��ջ
int OPT_lack;
int F_lack;
int L_lack;//ȱҳ��
int OPT_Pg[100];//�����洢ҳ�ŵĳ��ִ���
int OPT_Length=0;//�洢ҳ�ŵĸ���
struct no{
    char Block;
    bool Flag=0;
};
struct no OPT_Stack[100];
void INIT()//��ʼ��λʾͼ
{
    //��ʼ��λʾͼ
    srand(time(0));
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            Pos[i][j]=rand()%2;
            Pos1[i][j]=Pos[i][j];
        }
    //ȱҳ����ʼ��
    L_lack=0;
    F_lack=0;
    OPT_lack=0;
}
void Display_Pos(int pos[8][8])
{
    cout<<"λʾͼ��"<<endl;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
            cout<<pos[i][j]<<" ";
        cout<<endl;
    }

}
void Display_Stack(char x[])
{
    cout<<"ջ�����";
    //cout<<" Block_Num = "<<Block_Num<<endl;
    for(int i=0; i<Block_Num; i++)
        cout<<(char)x[i]<<" ";
}
void Display_Page(struct node x[])
{
    cout<<"\nҳ��\n"<<endl;
    cout<<"ҳ��"<<setw(8)<<"���"<<setw(9)<<"״̬λ"<<endl;
    for(int i=0; i<Page; i++)
        cout<<i<<setw(8)<<x[i].Block<<setw(8)<<x[i].Flag<<endl;
}
void FIFO()
{
    cout<<"-------------------FIFO-----------------------"<<endl;
    if(!F_Page[P].Flag) //״̬λΪ0��ȱҳ
    {
        F_lack++;
        if(F_lack>Block_Num) //��Ҫ�û�
        {
            F_Page[P].Block=F_Page[F_Stack[0]-'0'].Block;
            F_Page[P].Flag=1;
            F_Page[F_Stack[0]-'0'].Flag=0;
        }
        else
        {
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(!Pos[i][j])
                    {
                        F_Page[P].Block=i*8+j;//��ҳ����д�Ͽ��
                        F_Page[P].Flag=1;
                        Pos[i][j]=1;//λʾͼ��1
                        break;
                    }
                }
                if(F_Page[P].Flag==1)
                    break;
            }
        }
        int k;
        for(k=0; k<Block_Num-1; k++)
            F_Stack[k]=F_Stack[k+1];
        F_Stack[k]=P+'0';
    }
    Display_Stack(F_Stack);
    cout<<endl;
    Display_Page(F_Page);
    cout<<endl;
    Display_Pos(Pos);
}
void LRU()
{
    cout<<"---------------------LRU---------------------"<<endl;
    if(!L_Page[P].Flag) //״̬λΪ0��ȱҳ
    {
        L_lack++;
        if(L_lack>Block_Num) //��Ҫ�û�
        {
            L_Page[P].Block=L_Page[L_Stack[0]-'0'].Block;
            L_Page[P].Flag=1;
            L_Page[L_Stack[0]-'0'].Flag=0;
        }
        else
        {
            for(int i=0; i<8; i++)
            {
                for(int j=0; j<8; j++)
                {
                    if(!Pos1[i][j])
                    {
                        L_Page[P].Block=i*8+j;//��ҳ����д�Ͽ��
                        L_Page[P].Flag=1;
                        Pos1[i][j]=1;//λʾͼ��1
                        break;
                    }
                }
                if(L_Page[P].Flag==1)
                    break;
            }
        }
        int k;
        for(k=0; k<Block_Num-1; k++)
            L_Stack[k]=L_Stack[k+1];
        L_Stack[k]=P+'0';
    }
    else  //��ջ����ҳ�ö�
    {
        for(int i=0; i<Block_Num; i++)
        {
            if(L_Stack[i]-'0'==P)
            {
                for(int j=i; j<Block_Num-1; j++)
                    L_Stack[j]=L_Stack[j+1];
                L_Stack[Block_Num-1]=P+'0';
                break;
            }
        }
    }
    Display_Stack(L_Stack);
    cout<<endl;
    Display_Page(L_Page);
    cout<<endl;
    Display_Pos(Pos1);
}
bool IS_Stack(int k)//�жϵ�ǰҳ�Ƿ���ջ��
{
    for(int i=0;i<Block_Num;i++)
        if(OPT_Stack[i].Block==k+'0')
            return 1;
    return 0;
}
void OPT()
{
    cout<<"-------------------------OPT-----------------------------"<<endl;
   int i=0;
   while(i<OPT_Length){
    if(i<Block_Num){//ȱҳ���û�
        OPT_lack++;
        int k=0;
        for(k=0;k<Block_Num-1;k++)
        {
            OPT_Stack[k].Block=OPT_Stack[k+1].Block;
            //OPT_Stack[k].Flag=OPT_Stack[k+1].Flag;
        }
        OPT_Stack[k].Block=OPT_Pg[i]+'0';//ҳ��ջ
        //OPT_Stack[k].Flag=1;
    }else{
        if(!IS_Stack(OPT_Pg[i])){//ȱҳ�û�
            OPT_lack++;
            int q=0;
            for(int k=i+1;k<OPT_Length;k++)//�ҵ�Block_Num-1��ջ�е�Ԫ��,������־λ��1
            {
                if(q==Block_Num-1)
                    break;
                for(int m=0;m<Block_Num;m++){
                    if(OPT_Pg[k]==OPT_Stack[m].Block-'0'){
                        OPT_Stack[m].Flag=1;
                        q++;
                        break;
                    }
                }
            }
            for(int k=0;k<Block_Num;k++){//�ҵ��û�ҳ�����û�
                if(OPT_Stack[k].Flag==0){
                    OPT_Stack[k].Block=OPT_Pg[i]+'0';
                    break;
                }
            }
            for(int k=0;k<Block_Num;k++)//��ԭ��־λ
                OPT_Stack[k].Flag=0;
        }

    }
    i++;
    for(int k=0;k<Block_Num;k++)
        cout<<OPT_Stack[k].Block<<" ";
    cout<<endl;
   }
}
void OPE()
{
    cout<<"�����߼���ַ��";
    scanf("%x",&Logical_Adress);
    P=Logical_Adress/Block_Size;
    W=Logical_Adress%Block_Size;
    cout<<" Logical_Adress = "<<Logical_Adress<<endl;
    cout<<" P = "<<P<<" W = "<<W<<endl;
    if(P<Page)//��ֹ��ַԽ��
    {
        OPT_Pg[OPT_Length++]=P;
        FIFO();
        LRU();
    }
    else
    {
        cout<<"�����ַԽ�磡���������룺"<<endl;
        OPE();
    }

}
void Function()
{
    INIT();
    Display_Pos(Pos);
    cout<<"������С��������ҳ����Ҫ������߼���ַ������";
    cin>>Block_Size>>Block_Num>>Page>>Total;
    int k=Total;
    Block_Size*=1024;
    //��ʼ��ջ
    for(int i=0; i<Block_Num; i++)
    {
        F_Stack[i]='*';
        L_Stack[i]='*';
        OPT_Stack[i].Block='*';
    }
    while(Total)
    {
        OPE();
        Total--;
    }
    OPT();
    cout<<"\nFIFOȱҳ��:"<<F_lack<<"/"<<k<<endl;
    cout<<"LRUȱҳ��:"<<L_lack<<"/"<<k<<endl;
    cout<<"OPTȱҳ��:"<<OPT_lack<<"/"<<k<<endl;

}
int main()
{
    Function();
    return 0;
}
