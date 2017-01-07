#include <iostream>
#include<string.h>
#include <vector>
#include <fstream>
#define maxValue 100000000.0 //最大浮点数
#define max 100
using namespace std;
char filename1[50]="/Users/apple/Desktop/Topology)";
fstream fin;
ofstream fout;


struct Edge//边结点定义
{
    int dest;//下一个路由代号
    int metric;//度量
    struct Edge *next;
    Edge():next(NULL){};
    Edge(int dest):dest(dest),next(NULL){}//构造函数
};


struct Vertex//路由结点
{
    int route;//路由号
    int network;//连接网络号
    struct Edge *head;//指向边结点链表头
};

struct Routing_Table//路由表
{
    int network;//网络号
    int metric;//度量
    int route;//路由号
};

class Graphlnk
{
private:
    int maxVertexs;//允许路由器数目
    int numVertexs;//当前路由器数目
    Vertex NodeTable[max];//顶点表
    
public:
    Graphlnk(int sz)
    {
        maxVertexs=sz;
        numVertexs=0;
        for(int i=0;i<maxVertexs;++i)
        {
            NodeTable[i].head=NULL;
        }
    }
    int getNodeDest(int route)//得到路由器对应代号
    {
        for(int i=0;i<numVertexs;++i)
        {
            if(NodeTable[i].route==route)
            {
                return i;
            }
        }
        return -1;
    }
    int getName(int i)//取代号代表的路由器
    {
        return NodeTable[i].route;
    }
    int NumberOfVertices()//返回当前路由器数目
    {
        return numVertexs;
    }
    int getName_Net(int network)//取网络号关联的路由器
    {
        for(int i=0;i<numVertexs;++i)
        {
            if(NodeTable[i].network==network)
            {
                return i;
            }
        }
        return -1;
    }
    int getMetric_Vehicle(int v1,int v2);//返回两点间度量
    bool insertVertex(int route);//插入新路由器
    bool deleteVertex(int route);//删除此路由器
    bool insertEdge(int v1,int v2,int mertic);//插入新边
    bool deleteEdge(int v1,int v2);//删除边
    bool Cheat(int route);//cheat是否已存此路由器
    void Foutdata(int v1);
};

bool Graphlnk::Cheat(int route)
{
    for(int i=0;i<numVertexs;++i)
    {
        if(NodeTable[i].route==route)
        {
            return false;//已有，不需要新加入
        }
    }
    return true;//需要加入此路由器
}

int Graphlnk::getMetric_Vehicle(int v1,int v2)
{
    if(v1!=-1&&v2!=-1)
    {
        Edge *p=NodeTable[v1].head;
        while(p!=NULL)
        {
            if(p->dest==v2)
            {
                return p->metric;
            }
            p=p->next;
        }
    }
    return maxValue;
}




bool Graphlnk::insertVertex(int route)//添加路由器
{
    if(numVertexs==maxVertexs)
    {
        cerr<<"满，不能插入"<<endl;
        return false;
    }
    NodeTable[numVertexs].route=route;
    numVertexs++;
    return true;
}

bool Graphlnk::deleteEdge(int v1,int v2)//删边
{
    if(v1!=-1&&v2!=-1)
    {
        Edge *p=NodeTable[v1].head;
        Edge *q=NULL;
        Edge *s=p;
        while(p!=NULL&&p->dest!=v2)
        {
            q=p;
            p=p->next;
        }
        if(p!=NULL)
        {
            if(p==s)
            {
                NodeTable[v1].head=p->next;
            }
            else
            {
                q->next=p->next;
            }
            delete p;
        }
        else
        {
            return false;
        }
        p=NodeTable[v2].head;
        q=NULL;
        s=p;
        while(p->dest!=v1)
        {
            q=p;
            p=p->next;
        }
        if(p==s)
        {
            NodeTable[v2].head=p->next;
        }
        else
        {
            q->next=p->next;
        }
        delete p;
        return true;
    }
    return false;
}


bool Graphlnk::deleteVertex(int route)//删除城市
{
    int v=getNodeDest(route);//得到城市代号
    if(numVertexs==1||v<0||v>maxVertexs)
    {
        return false;
    }
    Edge *p,*s,*t;
    int k;
    while(NodeTable[v].head!=NULL)
    {
        p=NodeTable[v].head;//找到删除的顶点的边链表
        k=p->dest;
        s=NodeTable[k].head;//开始删除邻接顶点的边链表上的v点
        t=NULL;
        while(s!=NULL&&s->dest!=v)
        {
            t=s;
            s=s->next;
        }
        if(s!=NULL)
        {
            if(t==NULL)
            {
                NodeTable[k].head=s->next;
            }
            else
            {
                t->next=s->next;
                delete s;
            }
        }
        NodeTable[v].head=p->next;
        delete p;
    }
    numVertexs--;
    NodeTable[v].route=NodeTable[numVertexs].route;//填补
    NodeTable[v].head=NodeTable[numVertexs].head;
    p=NodeTable[v].head;
    while(p!=NULL)
    {
        s=NodeTable[p->dest].head;
        while(s!=NULL)
        {
            if(s->dest==numVertexs)
            {
                s->dest=v;
                break;
            }
            else
            {
                s=s->next;
            }
        }
        p=p->next;
    }
    return true;
}


bool Graphlnk::insertEdge(int v1,int v2,int metric)//添边
{
    if(v1>=0&&v1<numVertexs&&v2>=0&&v2<numVertexs)
    {
        Edge *p=NodeTable[v1].head;
        Edge *q;
        while(p!=NULL&&p->dest!=v2)
        {
            p=p->next;
        }
        if(p!=NULL)
        {
            cerr<<"已存在此路线"<<endl;
            return false;
        }
        p=new Edge;
        p->dest=v2;
        p->metric=metric;
        p->next=NodeTable[v1].head;
        NodeTable[v1].head=p;
        q=new Edge;
        q->dest=v1;
        q->metric=metric;
        q->next=NodeTable[v2].head;
        NodeTable[v2].head=p;
        return true;
    }
    return false;
}





void Shortest_Path(Graphlnk &G,int v,int *dist,int *path)
{
    int n=G.NumberOfVertices();//得路由器数目
    bool *S=new bool[n];
    int i,j,k,w,min,u;
    for(i=0;i<n;++i)//初始化
    {
        dist[i]=G.getMetric_Vehicle(v,i);//
        //cout<<"直达到"<<i<<"花费"<<dist[i]<<endl;//****
        S[i]=false;
        if(i!=v&&dist[i]<maxValue)
        {
            path[i]=v;//直达路径
            //cout<<i<<"的前面是"<<v<<endl;//***
        }
        else
        {
            path[i]=-1;//不能直达
        }
    }
    S[v]=true;//本身
    dist[v]=0;//本身
    path[v]=v;
    for(i=0;i<n-1;++i)
    {
        min=maxValue;
        u=v;
        for(j=0;j<n;++j)
        {
            if(S[j]==false&&dist[j]<min)
            {
                u=j;
                min=dist[j];
            }
        }
        S[u]=true;
        for(k=0;k<n;++k)//比较更新
        {
            w=G.getMetric_Vehicle(u,k);
            if(S[k]==false&&w<maxValue&&dist[u]+w<dist[k])
            {
                dist[k]=dist[u]+w;
                //cout<<"此时的u"<<u<<"  此时的w"<<w<<endl;
                //cout<<"更新到"<<k<<"花费"<<dist[k]<<endl;//****
                path[k]=u;
                //cout<<"更新   "<<i<<"的前面是"<<v<<endl;//***
            }
        }
    }
}





void Out(Graphlnk G,int *path,int *dist,int v1,int v2,int &metric,int &route)//输出最短路径第一邻接路由表
{
    metric=dist[v2];
    if(v1==v2)
    {
        route=0;
    }
    else
    {
        int a,temp=v2;
        int i=temp;
        while(path[temp]!=v1&&path[temp]!=-1)
        {
            i=temp;
            a=path[temp];
            temp=a;
        }
        route=G.getName(i);
    }
}


/*void FinData(Graphlnk &G,char *filename)
{
    fin.open(filename);
    if(!fin)
    {
        cout<<"读取数据失败"<<endl;
        exit(1);
    }
    while(fin.peek()!=EOF)
    {
        int route1,route2;
        fin>>route1;
        fin>>route2;
        if(G.Cheat(route1)==true)
        {
            G.insertVertex(route1);
        }
        if(G.Cheat(route2)==true)
        {
            G.insertVertex(route2);
        }
        int metric;
        //cout<<"耗时/秒钟-->";
        fin>>b.H;
        fin>>b.M;
        fin>>b.S;
        c_time=getSeconds(b);
        //cout<<"耗费/元-->";
        fin>>c_cost;
        //cout<<"时刻表"<<endl;
        int m,n;
        m=G.getNodeDest(v1);
        n=G.getNodeDest(v2);
        G.insertEdge(m,n,c_cost,c_time);
        char mark;
        Time a;
        do
        {
            fin>>a.H;
            fin>>a.M;
            fin>>a.S;
            G.insertStart_time(a,m,n);
            fin>>mark;
        }while(mark!='.');
    }
    fin.close();
}*/

void Graphlnk::Foutdata(int v1)
{
    int dist[max],path[max];
    int metric,route;
    Shortest_Path(*this,v1,dist,path);
    remove("/Users/apple/Desktop/bbc");
    fout.open("/Users/apple/Desktop/bbc");
    if(!fout)
    {
        cout<<"保存失败"<<endl;
        exit(1);
    }
    int n=NumberOfVertices();
    fout<<"目的网络\t距离\t下一跳路由表\n";
    for(int i=0;i<n;++i)
    {
        if(path[i]!=-1)
        {
            fout<<NodeTable[i].network<<"\t";
            Out(*this,path,dist,v1,i,metric,route);
            fout<<metric<<"\t";
            fout<<route<<endl;
        }
    }
    
    fout.close();
}



