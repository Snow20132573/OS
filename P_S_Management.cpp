#include<iomanip>
#include<iostream>
#include<cstdio>
#include<time.h>
#include<cstdlib>
using namespace std;
int Block_Size;//块大小
int Page;//页数
int Block_Num;//块数
int Total;//逻辑地址总个数
int Pos[8][8],Pos1[8][8];//位示图
int Logical_Adress;//逻辑地址
int P;//页号
int W;//页内地址
struct node//页表
{
    int Block;//块号
    bool Flag=0;//状态位
};
struct node F_Page[100];//FIFO的页表
struct node L_Page[100];//LRU的页表
char F_Stack[100];//FIFO的栈
char L_Stack[100];//LRU的栈
int OPT_lack;
int F_lack;
int L_lack;//缺页数
int OPT_Pg[100];//用来存储页号的出现次序
int OPT_Length=0;//存储页号的个数
struct no{
    char Block;
    bool Flag=0;
};
struct no OPT_Stack[100];
void INIT()//初始化位示图
{
    //初始化位示图
    srand(time(0));
    for(int i=0; i<8; i++)
        for(int j=0; j<8; j++)
        {
            Pos[i][j]=rand()%2;
            Pos1[i][j]=Pos[i][j];
        }
    //缺页数初始化
    L_lack=0;
    F_lack=0;
    OPT_lack=0;
}
void Display_Pos(int pos[8][8])
{
    cout<<"位示图："<<endl;
    for(int i=0; i<8; i++)
    {
        for(int j=0; j<8; j++)
            cout<<pos[i][j]<<" ";
        cout<<endl;
    }

}
void Display_Stack(char x[])
{
    cout<<"栈情况：";
    //cout<<" Block_Num = "<<Block_Num<<endl;
    for(int i=0; i<Block_Num; i++)
        cout<<(char)x[i]<<" ";
}
void Display_Page(struct node x[])
{
    cout<<"\n页表：\n"<<endl;
    cout<<"页号"<<setw(8)<<"块号"<<setw(9)<<"状态位"<<endl;
    for(int i=0; i<Page; i++)
        cout<<i<<setw(8)<<x[i].Block<<setw(8)<<x[i].Flag<<endl;
}
void FIFO()
{
    cout<<"-------------------FIFO-----------------------"<<endl;
    if(!F_Page[P].Flag) //状态位为0，缺页
    {
        F_lack++;
        if(F_lack>Block_Num) //需要置换
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
                        F_Page[P].Block=i*8+j;//在页表中写上块号
                        F_Page[P].Flag=1;
                        Pos[i][j]=1;//位示图置1
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
    if(!L_Page[P].Flag) //状态位为0，缺页
    {
        L_lack++;
        if(L_lack>Block_Num) //需要置换
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
                        L_Page[P].Block=i*8+j;//在页表中写上块号
                        L_Page[P].Flag=1;
                        Pos1[i][j]=1;//位示图置1
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
    else  //将栈命中页置顶
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
bool IS_Stack(int k)//判断当前页是否在栈内
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
    if(i<Block_Num){//缺页不置换
        OPT_lack++;
        int k=0;
        for(k=0;k<Block_Num-1;k++)
        {
            OPT_Stack[k].Block=OPT_Stack[k+1].Block;
            //OPT_Stack[k].Flag=OPT_Stack[k+1].Flag;
        }
        OPT_Stack[k].Block=OPT_Pg[i]+'0';//页入栈
        //OPT_Stack[k].Flag=1;
    }else{
        if(!IS_Stack(OPT_Pg[i])){//缺页置换
            OPT_lack++;
            int q=0;
            for(int k=i+1;k<OPT_Length;k++)//找到Block_Num-1个栈中的元素,并将标志位置1
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
            for(int k=0;k<Block_Num;k++){//找到置换页进行置换
                if(OPT_Stack[k].Flag==0){
                    OPT_Stack[k].Block=OPT_Pg[i]+'0';
                    break;
                }
            }
            for(int k=0;k<Block_Num;k++)//还原标志位
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
    cout<<"输入逻辑地址：";
    scanf("%x",&Logical_Adress);
    P=Logical_Adress/Block_Size;
    W=Logical_Adress%Block_Size;
    cout<<" Logical_Adress = "<<Logical_Adress<<endl;
    cout<<" P = "<<P<<" W = "<<W<<endl;
    if(P<Page)//防止地址越界
    {
        OPT_Pg[OPT_Length++]=P;
        FIFO();
        LRU();
    }
    else
    {
        cout<<"输入地址越界！请重新输入："<<endl;
        OPE();
    }

}
void Function()
{
    INIT();
    Display_Pos(Pos);
    cout<<"输入块大小，块数，页数，要输入的逻辑地址个数：";
    cin>>Block_Size>>Block_Num>>Page>>Total;
    int k=Total;
    Block_Size*=1024;
    //初始化栈
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
    cout<<"\nFIFO缺页率:"<<F_lack<<"/"<<k<<endl;
    cout<<"LRU缺页率:"<<L_lack<<"/"<<k<<endl;
    cout<<"OPT缺页率:"<<OPT_lack<<"/"<<k<<endl;

}
int main()
{
    Function();
    return 0;
}
