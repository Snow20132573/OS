#include<cstdio>
#include<cstdlib>
#include <iostream>
#include<iomanip>
#include<cstring>
using namespace std;
int m,n;//m类资源，n个进程
int Max[100][100];//进程对资源的最大需求
int Available[100];//系统中可利用的资源
int Allocation[100][100];//已分配资源
int Need[100][100];//进程仍需的资源
char P_name[100][10];//进程的名字
bool Finish[100];//进程可被做
void Make()
{
    m=3;
    n=5;

    Available[1]=3;
    Available[2]=3;
    Available[3]=2;

    strcpy(P_name[1],"p0");
    strcpy(P_name[2],"p1");
    strcpy(P_name[3],"p2");
    strcpy(P_name[4],"p3");
    strcpy(P_name[5],"p4");
    Max[1][1]=7;
    Max[1][2]=5;
    Max[1][3]=3;

    Max[2][1]=3;
    Max[2][2]=2;
    Max[2][3]=2;

    Max[3][1]=9;
    Max[3][2]=0;
    Max[3][3]=2;

    Max[4][1]=2;
    Max[4][2]=2;
    Max[4][3]=2;

    Max[5][1]=4;
    Max[5][2]=3;
    Max[5][3]=3;

    Allocation[1][1]=0;
    Allocation[1][2]=1;
    Allocation[1][3]=0;

    Allocation[2][1]=2;
    Allocation[2][2]=0;
    Allocation[2][3]=0;

    Allocation[3][1]=3;
    Allocation[3][2]=0;
    Allocation[3][3]=2;

    Allocation[4][1]=2;
    Allocation[4][2]=1;
    Allocation[4][3]=1;

    Allocation[5][1]=0;
    Allocation[5][2]=0;
    Allocation[5][3]=2;

    Need[1][1]=7;
    Need[1][2]=4;
    Need[1][3]=3;

    Need[2][1]=1;
    Need[2][2]=2;
    Need[2][3]=2;

    Need[3][1]=6;
    Need[3][2]=0;
    Need[3][3]=0;

    Need[4][1]=0;
    Need[4][2]=1;
    Need[4][3]=1;

    Need[5][1]=4;
    Need[5][2]=3;
    Need[5][3]=1;
}
void Trim(char *s)
{
    char *p=s;
    while(*p==' '||*p=='\t'||*p=='\r'||*p=='\n')
        p++;
    strcpy(s,p);
    int i=strlen(s)-1;
    while(s[i]==' '||s[i]=='\t'||s[i]=='\r'||s[i]=='\n')
        i--;
    s[i+1]='\0';
}
int Split(char* s,char result[100][100],char chr)//将字符串S以chr为分隔符，结果存在result里面
{
    int i=0,j=0,k=0;
    while(s[i]==chr)
        i++;
    if(s[i]=='\0')
        return 0;
    for(;s[i]!='\0';i++){
        if(s[i]!=chr){
            result[j][k++]=s[i];
        }else{
            result[j++][k]='\0';
            k=0;
        }
    }
    return j+1;
}
void INIT_Source(char s[100][100])//初始化资源i-m
{
    for(int i=1;i<=m;i++)
        Available[i]=atoi(s[i-1]);
}
bool INIT_Program(char s[100][100],int k)//初始化第k个进程的名字，Max,Allocation,Need
{
    for(int i=0;i<2*m+1;i++){
        if(i==0){
            strcpy(P_name[k],s[i]);
        }else if(i>0&&i<=m){
            Max[k][i]=atoi(s[i]);
            //cout<<"M ["<<k<<","<<i<<"]"<<Max[k][i]<<endl;
        }else{
            Allocation[k][i-m]=atoi(s[i]);
            //cout<<"A ["<<k<<","<<i-m<<"]"<<Allocation[k][i-m]<<endl;
            if(Allocation[k][i-m]>Max[k][i-m]){
                //cout<<" > M["<<k<<","<<i-m<<"]"<<Max[k][i-m]<<endl;
                return false;
            }
            Need[k][i-m]=Max[k][i-m]-Allocation[k][i-m];
            Available[i-m]-=Allocation[k][i-m];
        }
    }
    return true;
}
bool Is_Safty(char Pro[100][100],int con)//安全性算法，检测算法是否安全,将安全序列放在Pro中
{
    int Work[100];
    int work[100][100];//对应于i进程的jwork
    int len=1;
    for(int i=1;i<=m;i++)
        Work[i]=Available[i];
    cout<<"输出Work集合："<<endl;
    for(int i=1;i<=m;i++)
        cout<<Work[i]<<endl;
    for(int i=1;i<=n;i++)//所有进程都没做
        Finish[i]=false;
    int t=n;
    while(t--){
        int i;
        //找到一个可执行的进程i
        bool IS=true;
        for(i=1;i<=n;i++){
            if(Finish[i]==false){
                bool all=true;//判断是不是所有的资源都满足
                for(int j=1;j<=m;j++){
                    if(Need[i][j] > Work[j]){
                        //cout<<"name "<<P_name[i]<<" Need  "<<Need[i][j]<<" > "<<"Work "<<Work[j]<<endl;
                        all=false;
                        break;
                    }
                }
                if(all==true){//即进程i的所有资源都满足
                    //cout<<"************1"<<endl;
                    IS=false;
                    strcpy(Pro[len],P_name[i]);
                    Finish[i]=true;
                    for(int j=1;j<=m;j++)
                        work[len][j]=Work[j];
                    len++;
                    for(int j=1;j<=m;j++)
                        Work[j]+=Allocation[i][j];
                    break;
                }
            }
        }
        if(IS){
            //cout<<"************2"<<endl;
            cout<<"Not Safty!Source not Enough!"<<endl;
            return false;
        }
    }
    cout<<setw(6)<<"Prog|"<<setw(13)<<"Max|"<<setw(13)<<"Allocation|"<<setw(13)<<"Need|"<<setw(13)<<"Sort|"<<setw(13)<<"Work|"<<setw(13)<<"Finished|"<<endl;
    for(int i=1;i<=n;i++){//n个进程
        cout<<setw(5)<<P_name[i]<<setw(1)<<"|";
        for(int j=1;j<=m;j++){//m类资源
            cout<<setw(4)<<Max[i][j];
        }
        cout<<setw(1)<<"|";
        for(int j=1;j<=m;j++){//m类资源
            cout<<setw(4)<<Allocation[i][j];
        }
        cout<<setw(1)<<"|";
        for(int j=1;j<=m;j++){//m类资源
            cout<<setw(4)<<Need[i][j];
        }
        cout<<setw(1)<<"|";
        cout<<setw(12)<<Pro[i]<<setw(1)<<"|";
        for(int j=1;j<=m;j++){//m类资源
            cout<<setw(4)<<work[i][j];
        }
        cout<<setw(1)<<"|";
        cout<<setw(12)<<Finish[i]<<endl;
    }
//    if(con==2){
//        for(int i=1;i<=n;i++)
//            Available[i]=Work[i];
//    }
    return true;
}
bool Request(char str[100][100])
{
    int i;
    for(i=1;i<=n;i++){
        if(strcmp(str[1],P_name[i])==0){//找到进程i
            break;
        }
    }
    for(int j=1;j<=m;j++){
        if(atoi(str[j+1]) > Need[i][j]){
            cout<<"Request > Need  Error! Request too much!"<<endl;
            return false;//不会为进程分配资源
        }
    }
    for(int j=1;j<=m;j++){
        if(atoi(str[j+1]) > Available[j]){
            cout<<"Request  >  Available   Source is not enough! wait"<<endl;
            return false;
        }
    }
    for(int j=1;j<=m;j++){
        Available[j]-=atoi(str[j+1]);
        Allocation[i][j]+=atoi(str[j+1]);
        Need[i][j]-=atoi(str[j+1]);
    }
    char Pro[100][100];
    bool b=Is_Safty(Pro,2);
    if(b){
         cout<<"Safty Queue :";
        for(int j=1;j<=n;j++){
            cout<<Pro[j];
            if(j!=n){
                cout<<" -> ";
            }
        }
        cout<<endl;
    }else{
        cout<<"It can't be finished!"<<endl;
        for(int j=1;j<=m;j++){
            Available[j]+=atoi(str[j+1]);
            Allocation[i][j]-=atoi(str[j+1]);
            Need[i][j]+=atoi(str[j+1]);
        }
    }
}
void Function()
{
    char s[100];
//    cout<<"Input Sources"<<endl;
//    gets(s);
//    Trim(s);
//    cout<<s<<endl;
//    char str[100][100];
//    m=Split(s,str,' ');//m类资源
//    INIT_Source(str);//初始化m类资源
//    cout<<"Input Program num：";
//    cin>>n;
//    cout<<"Pro Max Allocation"<<endl;
//        getchar();
//    for(int i=0;i<n;i++)
//    {
//        gets(s);
//        Trim(s);
//        cout<<"Program information:  "<<s<<endl;
//        int t=Split(s,str,' ');
//        bool b=INIT_Program(str,i+1);
//        if(!b){
//            cout<<"Allocation > Max ,Not illegal,Input again!"<<endl;
//            i--;
//        }
//    }
    Make();
    char Pro[100][100];
    char str[100][100];
    cout<<"Max"<<endl;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            cout<<Max[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Allocation"<<endl;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            cout<<Allocation[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Need"<<endl;
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            cout<<Need[i][j]<<" ";
        }
        cout<<endl;
    }
    bool b=Is_Safty(Pro,1);
    cout<<"第一次 b = "<<b<<endl;
    if(b){
        cout<<"Safty Queue :";
        for(int i=1;i<=n;i++){
            cout<<Pro[i];
            if(i!=n){
                cout<<" -> ";
            }
        }
        cout<<endl;
    }
    //cin>>s;
    //char str[100][100];
    //getchar();
//    strcpy(str[1],"p1");
//    strcpy(str[2],"1");
//    strcpy(str[3],"0");
//    strcpy(str[4],"2");
    while(1){

        gets(s);
        Trim(s);
        //cout<<"s = "<<s<<endl;
        Split(s,str,' ');
        cout<<"进Requsest"<<endl;
        Request(str);
        cout<<"出Requsest"<<endl;
//        strcpy(str[1],"p4");
//        strcpy(str[2],"3");
//        strcpy(str[3],"3");
//        strcpy(str[4],"0");
//        Request(str);
//        cout<<"出Requsest"<<endl;
    }
    cin>>s;
}
int main()
{
    while(1){
        Function();
    }
    return 0;
}
