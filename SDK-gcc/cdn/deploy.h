#pragma once
#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "lib_io.h"
#include <vector>
#include <stack>
#include <queue>
#include <map>
#include <string>
#include <unordered_map>
#include <sstream>
#include <ctime>
#include <iostream>
void deploy_server(char * graph[MAX_EDGE_NUM], int edge_num, char * filename);

#define INF        (~(0x1<<31))

//存放当前最好的结果
struct Bulletin
{
	std::vector<std::queue<int>> bulletin_path;	//当前服务器方案下的存放路径
	std::vector<int> bulletin_path_capacity; 	//路径对应的带宽
	std::vector<int> bulletin_path_consumptionNodeID;//路径对应的消费节点id
	int num_VideoServer;
	int bulletin_price;
	Bulletin(std::vector<std::queue<int>> bp,std::vector<int> bpc,std::vector<int> bpcon, int nVS,int bprice):
	bulletin_path(bp),bulletin_path_capacity(bpc),bulletin_path_consumptionNodeID(bpcon),
	num_VideoServer(nVS),bulletin_price(bprice){}
};

/*
* 网络节点，用来以邻接表的形式记录边
*/
struct node {
	int dest;	//节点标号
	int Bandwidth;	//带宽
	int LeastFee;	//租赁费用
	struct node * link;	//下一个邻接的点
	node(int d, int bw, int lf) :dest(d), Bandwidth(bw), LeastFee(lf), link(nullptr){}

};

/*
* 用于求最短路径时候的顶点
*/
struct Dij_node{
	friend class Graph;
	int dest;	//节点标号
	int LeastFee;	//租赁费用
	Dij_node(int d, int lf):dest(d),LeastFee(lf){}
	friend bool operator < (const Dij_node & node1,const Dij_node & node2)//重载小于号操作，用于对节点根据租赁费用排序
	{
		return node1.LeastFee>node2.LeastFee;
	}
};

/*
* 顶点结构
 */
struct vertex
{
	int networkNodeID;	//顶点标号
	int degree;	//该顶点的度
	int sumBandWidth;	//顶点所连边的带宽总和
	bool isIsolatedNode;	//是否为孤立的点
	bool isVideoServer;	//是否是服务器节点
	bool isConnectedConsumptionNode;	//是否连接消费节点
	int consumptionNodeID;	//消费节点id，默认-1
	int sumConsumptionBandWidth;	//所连接消费节点的带宽需求
	struct node *adj;	//与其相邻的节点
	vertex(int id) :networkNodeID(id), degree(0), sumBandWidth(0),isIsolatedNode(false),
	isVideoServer(false), isConnectedConsumptionNode(false),
	consumptionNodeID(-1), sumConsumptionBandWidth(-1), adj(nullptr){}
};



/*
* 图的结构
*/
class Graph
{
private:
	int num_Network_Node;	//网络节点的数目
	int num_edge;			//图中边的数目
	int num_Consum_Node;	//消费节点数目
	int num_VideoServer;	//服务器数目
	int price_Server;	//服务器价格
	int SourceID;	//源点id
	int SinkID;		//汇点id
	std::vector<vertex> Vertex_Table;	//存放所有的网络节点
	std::vector<int> Consumption;	//每个消费节点所直连的网络节点id
	std::vector<int> Server;	//服务器节点id
	std::vector<std::stack<int>> default_path;	//存放当前成本最低的路径
	std::vector<std::queue<int>> new_path;	//当前服务器方案下的存放路径
	std::vector<int> new_path_price;	//当前服务器方案下每条路径对应的租赁成本
	std::vector<int> path_capacity;	//当前服务器方案下的每条路径对应的带宽
	std::vector<int> path_consumptionNodeID;	//路径对应的消费节点id
	int sum_price;		//总成本
	int sum_demanded;	//总需求
	std::vector<int> dPath;	//Dijkstra求出的路径矩阵
	std::vector<int> dPrice;	//Dijstra求出的最短路径（价格）

public:
	Graph(char * topo[MAX_EDGE_NUM], int line_num){
		int numnetworknode,numedge,numConsumNode,priceServer;
		std::stringstream ss;
		ss<<topo[0];
		ss>>numnetworknode>>numedge>>numConsumNode;
		ss<<topo[2];
		ss>>priceServer;
		num_Network_Node=numnetworknode;
		num_edge=numedge;
		num_Consum_Node=numConsumNode;
		price_Server=priceServer;
		num_VideoServer = 0;
		SourceID = -1;
		SinkID = -1;
		sum_demanded = 0;
		for (int i = 0; i < num_Network_Node; ++i)
		{
			Vertex_Table.push_back(vertex(i));
		}
		Consumption = std::vector<int> (num_Consum_Node);
		dPath = std::vector<int>(num_Network_Node+2);
		dPrice = std::vector<int>(num_Network_Node+2,INF);
		int cc = 3;
		int a,b,c,d;
		while(++cc<=num_edge + 3)
		{
			ss<<topo[cc];
			ss>>a>>b>>c>>d;
			insertEdge(a,b,c,d);
		}
		while(++cc<line_num)
		{
			ss<<topo[cc];
			ss>>a>>b>>c;
			addConsumtion(a,b,c);
		}
		//Removing Isolated Node Recursively to Simplify Network Topology(lwd)
		removeIsolateNode();
	}
	~Graph(){
		Vertex_Table.clear();
		Consumption.clear();
		Server.clear();
		default_path.clear();
		new_path.clear();
		new_path_price.clear();
		path_capacity.clear();
		path_consumptionNodeID.clear();
		dPath.clear();
		dPrice.clear();
	}
	int get_num_Network_Node(){return num_Network_Node;}
	int get_num_Consum_Node(){return num_Consum_Node;}

	std::vector<vertex> get_vertex(){return Vertex_Table;}
	std::vector<int> get_Consumption(){return Consumption;}	//每个消费节点所直连的网络节点id

	//Removing Isolated Node Recursively to Simplify Network Topology(lwd)
	void removeIsolateNode(){
		while (1){
			int flag  = 0;
			int size = Vertex_Table.size();
			for(int i=0;i<size;i++){
				if (1 == Vertex_Table[i].degree && !Vertex_Table[i].isConnectedConsumptionNode){
					flag = 1;
					Vertex_Table[i].isIsolatedNode = true;
					Vertex_Table[i].degree--;
					Vertex_Table[i].sumBandWidth = 0;
					struct node * adjTemp = Vertex_Table[i].adj;
					int destTemp = adjTemp->dest;
					delete Vertex_Table[i].adj;
					Vertex_Table[i].adj = NULL;
					struct node * adjDest = Vertex_Table[destTemp].adj;
					struct node * linkNow = adjDest;

					while (linkNow->link != nullptr){
						if (linkNow->link->dest == Vertex_Table[i].networkNodeID){
							if (linkNow->link->link !=nullptr){
								struct node * linkDelete = linkNow->link;
								linkNow->link = linkNow->link->link;
								delete linkDelete;
								linkDelete = NULL;
							}else{
								delete linkNow->link;
								linkNow->link = NULL;
							}
							Vertex_Table[destTemp].degree--;
							Vertex_Table[destTemp].sumBandWidth -= linkNow->Bandwidth;
							break;
						}
						linkNow->link = linkNow->link->link;
					}
				}
			}
			if (flag == 0){
				break;
			}
		}
	}
	/*
	* 计算总成本
	*/
	void calculate_sum_price(){
		sum_price = num_VideoServer * price_Server;
		for (auto iter = new_path_price.begin(); iter != new_path_price.end(); ++iter)
		{
			sum_price += *iter;
		}
	}

	/*
	* 插入一条连接
	* fn&tn:起点&终点 bandwidth:路径所能支持最大带宽，leastfee:带宽租赁费用 pre Gbps
	*/
	void insertEdge(int fn, int tn, int bandwidth, int leastfee){
		//初始化两个节点
		node *beg = new node(fn, bandwidth, leastfee);
		node *end = new node(tn, bandwidth, leastfee);
		//更新两顶点的度以及总带宽
		Vertex_Table[fn].degree++;
		Vertex_Table[fn].sumBandWidth += bandwidth;
		Vertex_Table[tn].degree++;
		Vertex_Table[tn].sumBandWidth += bandwidth;
		//以fn为起点的邻接点
		node *tmpnode = Vertex_Table[fn].adj;
		if (tmpnode){//已存在连接点，添加end节点到邻接表中，并将其余邻接点放入end后
			end->link = Vertex_Table[fn].adj;
			Vertex_Table[fn].adj = end;
		}
		else{//否则直接将end添加到邻接表
			Vertex_Table[fn].adj = end;
		}
		//与上同理
		tmpnode = Vertex_Table[tn].adj;
		if (tmpnode){
			beg->link = Vertex_Table[tn].adj;
			Vertex_Table[tn].adj = beg;
		}
		else{
			Vertex_Table[tn].adj = beg;
		}
	}
	/*
	* 添加消费节点
	* cid 消费节点id， vertid：与消费点相连的顶点，消费带宽
	*/
	void addConsumtion(int cid, int vertid, int CBandWidth)
	{
		sum_demanded += CBandWidth;
		Vertex_Table[vertid].isConnectedConsumptionNode = true;
		Vertex_Table[vertid].consumptionNodeID  = cid;
		Vertex_Table[vertid].sumConsumptionBandWidth = CBandWidth;
		Consumption[cid] = vertid;
	}

	/*
	* 打印图
	*/
	void printGraph(){
		std::cout<<"-------------------\n";
		std::cout<<"网络节点数: "<<num_Network_Node<<"\n";
		std::cout<<"消费节点数："<<num_Consum_Node<<"\n";
		std::cout<<"边的数目："<<num_edge<<"\n\n";
		std::cout<<"消费节点：-----------------------------------"<<"\n";
		std::cout<<"消费节点(带宽需求)：网络节点->[邻接点-带宽-租赁费用]\n";
		for (int i = 0; i < num_Consum_Node; ++i)
		{
			int nid = Consumption[i];
			node *tmp = Vertex_Table[nid].adj;
			std::cout<<"消费节点 "<<i<<"("<<Vertex_Table[nid].sumConsumptionBandWidth<<")"<<" : "<<nid<<" -> ";
			while(tmp!=nullptr)
			{
				std::cout<<"["<<tmp->dest<<"-"<<tmp->Bandwidth<<"-"<<tmp->LeastFee<<"],";
				tmp = tmp->link;
			}
			std::cout<<"\n";
		}
		std::cout<<"\n网络节点：-----------------------------------"<<"\n";
		std::cout<<"网络节点->[邻接点-带宽-租赁费用]\n";
		for (int i = 0; i < num_Network_Node; ++i){
			std::cout<<"网络节点:"<<i<<"  度"<<Vertex_Table[i].degree<<
			"  所连链路总带宽"<<Vertex_Table[i].sumBandWidth<<"\n";
			node *tmp = Vertex_Table[i].adj;
			while(tmp!=nullptr)
			{
				std::cout<<"["<<tmp->dest<<"-"<<tmp->Bandwidth<<"-"<<tmp->LeastFee<<"],";
				tmp = tmp->link;
			}
			std::cout<<"\n";
		}
	}

	/*
	* 放置服务器
	* n：放置服务器个数 n=-1代表将服务器全部放置在直连的位置
	*/
	void set_server(int n = -1)
	{

		if(n == -1){
			for (auto iter = Consumption.begin(); iter != Consumption.end(); ++iter)
			{
				Vertex_Table[*iter].isVideoServer = true;
				Server.push_back(*iter);
				num_VideoServer++;
			}
		}else{
			auto m = find_max_degree(n);
			auto map_it = m.begin();
			while(map_it!=m.end())
			{
				if(!Vertex_Table[map_it->first].isVideoServer){
					Vertex_Table[map_it->first].isVideoServer = true;
					Server.push_back(map_it->first);
					num_VideoServer++;
				}
				++map_it;
			}
		}

	}

	void set_server(std::vector<int> vect)
	{
		for (auto iter = vect.begin(); iter != vect.end(); ++iter)
		{
			Vertex_Table[*iter].isVideoServer = true;
			Server.push_back(*iter);
			num_VideoServer ++;
		}
	}
	/*
	* 找到度最大的n个网络节点
	*/
	std::unordered_map<int,int> find_max_degree(int n){
		//1.采用堆排序 O(log(N))
		//ToDo
		//2.暴力搜寻 O(M * n)
		std::unordered_map<int,int> ret_map;
		int max;
		int flag;
		for (int i = 0; i < n; ++i)
		{
			max = Vertex_Table[0].degree;
			flag = 0;
			for (int j = 0; j < num_Network_Node; ++j)
			{
				if(Vertex_Table[j].degree>max)
				{
					max = Vertex_Table[j].degree;
					flag = j;
				}
			}
			ret_map[flag] = max;
			Vertex_Table[flag].degree = 0;
		}

		auto map_it = ret_map.begin();
		while(map_it!=ret_map.end())
		{
			Vertex_Table[map_it->first].degree = map_it->second;
			//std::cout<<map_it->first<<":"<<map_it->second<<"\n";
			++map_it;
		}

		return ret_map;
	}
    /*
    * 找到输出最大的n个节点
    */
    /*
    std::unordered_map<int,int> find_max_bandwidth(int n){
		//1.采用堆排序 O(log(N))
		//ToDo
		//2.暴力搜寻 O(M * n)
		std::unordered_map<int,int> ret_map;
		int max;
		int flag;
		for (int i = 0; i < n; ++i)
		{
			max = Vertex_Table[0].sumBandWidth;
			flag = 0;
			for (int j = 0; j < num_Network_Node; ++j)
			{
				if(Vertex_Table[j].sumBandWidth>max)
				{
					max = Vertex_Table[j].sumBandWidth;
					flag = j;
				}
			}
			ret_map[flag] = max;
			Vertex_Table[flag].sumBandWidth = 0;
		}
		return ret_map;
	}*/
    /*
    std::vector<int> find_max_bandwidth(int n)
    {
        std::vector<int> result_table;
        std::vector<vertex> bandTable = Vertex_Table;
        sort(bandTable.begin(),bandTable.end(),[Vertex_Table](const int &a, const int &b){
            return Vertex_Table[a].sumBandWidth>Vertex_Table[b].sumBandWidth;});
        std::vector<vertex>::iterator it=bandTable.begin();
        for(int i=0; i<n; ++i)
        {
            result_table[i] = *it->networkNodeID;
            ++it;
        }
        return result_table;
    }
    */
	/*
	* 找到当前链路中的带宽短板
	*/
	int get_BandWidth_bottleneck(std::queue<int> &tmp_queue)
	{
		auto new_q = tmp_queue;
		int i = new_q.front(), j = 0,bottleneck = INF;int weight = 0;
		new_q.pop();
		while(!new_q.empty())
		{
			j = new_q.front();new_q.pop();
			weight = get_BandWidth_weight(i,j);
			if(weight<bottleneck)
			{
				bottleneck = weight;
			}
			i = j;
		}
		return bottleneck;
	}
	/*
	* 找到短板链路，设置为前路径的承受能力，并将所有路径减去，min（消费点需求，承受能力）
	* std::queue<int> &s:路径栈；int cc：路径服务的消费节点直连的网络节点
	*/
	void update_path(std::queue<int> &tmp_queue,int cc)
	{
		int bottleneck = get_BandWidth_bottleneck(tmp_queue);
		//将所有路径减去min（消费点需求，承受能力）
		int capacity = (Vertex_Table[cc].sumConsumptionBandWidth<bottleneck)?Vertex_Table[cc].sumConsumptionBandWidth:bottleneck;
		//更新带宽
		auto new_queue = tmp_queue;
		int i = tmp_queue.front(),j = -1;
		tmp_queue.pop();
		while(!tmp_queue.empty())
		{
			j = tmp_queue.front();
			tmp_queue.pop();
			//更新路径中各连接的带宽
			reduce_BandWidth_weight(i,j,capacity);
			i = j;
		}
		Vertex_Table[cc].sumConsumptionBandWidth -= capacity;

	}

	/*
	* 优先队列迪杰斯特拉算法
	*/
	void Dijkstra_dq(int v0)
	{
		Dij_node first(-1,-1),next(-1,-1),temp(-1,-1);
		std::priority_queue<Dij_node> pq;
		node * arc;
		bool *final = new bool[num_Network_Node];
		for (int v = 0;v<num_Network_Node;v++)
		{
			dPrice[v] = INF;
			dPath[v] = v0;
			final[v] = false;
		}
		dPrice[v0] = 0;
		temp = Dij_node(v0,dPrice[v0]);
		pq.push(temp);
		while(!pq.empty())
		{
			first = pq.top();
			pq.pop();
			final[first.dest] = true;
			if(first.dest == num_Network_Node-1)
				break;
			for(arc = Vertex_Table[first.dest].adj;arc!=nullptr;arc = arc->link)
			{
				if(final[arc->dest])
					continue;
				next = Dij_node(arc->dest,first.LeastFee+arc->LeastFee);
				if(next.LeastFee<dPrice[next.dest])
				{
					dPrice[next.dest] = next.LeastFee;
					dPath[next.dest] = first.dest;
					pq.push(next);
				}
			}
		}
		delete arc;
		delete final;
		arc = nullptr;
		final = nullptr;

	}


	/*
	* 原始迪杰斯特拉算法
	* 求节点v0到其余节点的最短距离
	*/
	void Dijkstra(int v0)
	{
		int min = INF,v = 0,w = 0,k = 0;
		//初始化路径以及权重数组
		int *final = new int[num_Network_Node];
		for(v = 0;v<num_Network_Node;v++)
		{
			final[v] = 0;
			dPath[v] = v0;
			dPrice[v] = get_LeastFee_weight(v0,v);
		}
		node *tmp = Vertex_Table[v0].adj;
		while(tmp!=nullptr)
		{
			dPrice[tmp->dest] = tmp->LeastFee;
			tmp = tmp->link;
		}
		dPrice[v0] = 0;
		final[v0] = 1;
		for(v = 0;v<num_Network_Node;v++)
		{
			if(v == v0)
				continue;
			//找到从V0开始的最短路径节点
			min = INF;
			for(w = 0;w<num_Network_Node;w++)
			{
				if(!final[w]&&dPrice[w]<min)
				{
					min = dPrice[w];
					k = w;
				}
			}
			final[k] = 1;
			if (k == num_Network_Node-1)
				break;
			for(w = 0;w<num_Network_Node;w++)
			{
				int wei = get_LeastFee_weight(k,w);
				if(!final[w]&&min+wei<dPrice[w])
				{
					dPrice[w] = min+wei;
					dPath[w] = k;
				}
			}
		}
		delete []final;
	}

	/*
	* 获取两节点所连接路径的租赁单价
	 */
	int get_LeastFee_weight(int beg, int end)
	{
		if (beg == end)	return 0;
		node *tmp = Vertex_Table[beg].adj;
		while(tmp!=nullptr)
		{
			if(tmp->dest == end)
				return tmp->LeastFee;
			tmp = tmp->link;
		}
		return INF;
	}
	/*
	* 获取两节点所连接路径的带宽
	 */
	int get_BandWidth_weight(int beg, int end)
	{
		if (beg == end)
			std::cout<<"--------------------;\n";
		if (beg == end)	return 0;
		node *tmp = Vertex_Table[beg].adj;
		while(tmp!=nullptr)
		{
			if(tmp->dest == end)
				return tmp->Bandwidth;
			tmp = tmp->link;
		}
		return INF;
	}

	/*
	* 将两节点所连接路径的带宽减去subtrahend
	 */
	void reduce_BandWidth_weight(int beg, int end, int subtrahend)
	{
		node *parent = Vertex_Table[beg].adj;
		node *tmp = parent->link;
		if(parent->dest == end){
			parent->Bandwidth -= subtrahend;
			if (parent->Bandwidth == 0)
			{
				Vertex_Table[beg].degree --;
				Vertex_Table[end].degree --;
				parent->link = nullptr;
				Vertex_Table[beg].adj = tmp;
				delete parent;
			}
			return;
		}
		while(tmp!=nullptr)
		{
			if (tmp->dest == end)
			{
				tmp->Bandwidth -=subtrahend;
				if(tmp->Bandwidth == 0)
				{
					Vertex_Table[beg].degree --;
					Vertex_Table[end].degree --;
					parent->link = tmp->link;
					tmp->link = nullptr;
					delete tmp;

				}
				return;
			}
			parent = tmp;
			tmp = tmp->link;
		}
	}

	/*
	* MSMS_Dijkstra
	* 多源多汇最小费用最大流算法
	* Multiple Source Multiple Sink
	* 使用Dijkstra算法求单点到单点最短路径
	* 复杂度降到O(n^2);
	*/

	bool MSMS_Dijkstra()
	{
		//在图中加入两个节点，一个为源点，连接到各服务器上，一个为汇点，连接到各消费节点
		SourceID = num_Network_Node;
		Vertex_Table.push_back(vertex(num_Network_Node++));
		SinkID = num_Network_Node;
		Vertex_Table.push_back(vertex(num_Network_Node++));

		//将两个节点加入到图中
		//add Source point
		for (auto iter = Server.begin(); iter != Server.end(); ++iter)
		{
			node *end = new node(*iter, INF, 0);
			node *tmp = Vertex_Table[SourceID].adj;
			if(tmp == nullptr)
			{
				Vertex_Table[SourceID].adj = end;
			}else{
				Vertex_Table[SourceID].adj = end;
				end->link = tmp;
			}
		}
		//add Sink point
		for (auto iter = Consumption.begin(); iter != Consumption.end(); ++iter)
		{
			node *end = new node(SinkID, Vertex_Table[*iter].sumConsumptionBandWidth, 0);
			node *tmp = Vertex_Table[*iter].adj;
			if (tmp == nullptr)
			{
				Vertex_Table[*iter].adj = end;
			}else{
				Vertex_Table[*iter].adj = end;
				end->link = tmp;
			}
		}


		while (sum_demanded!=0)
		{
			int cc = -1;
			//Dijkstra(num_Network_Node-2);
			Dijkstra_dq(num_Network_Node-2);
			//找出当前最优路径
			std::stack<int> tmp_stack;
			tmp_stack.push(SinkID);
			int k = dPath[SinkID];
			cc = k;
			while(k!=SourceID)
			{
				tmp_stack.push(k);
				k = dPath[k];
			}
			std::queue<int> tmp_queue;
			while(!tmp_stack.empty()){
				tmp_queue.push(tmp_stack.top());
				tmp_stack.pop();
			}
			int min_price = dPrice[SinkID];
			if(min_price == INF)
				return false;

			int neck = get_BandWidth_bottleneck(tmp_queue);	//瓶颈带宽
			int capacity = (Vertex_Table[cc].sumConsumptionBandWidth<neck)?Vertex_Table[cc].sumConsumptionBandWidth:neck;
			//std::cout<<"当前路径可支撑最大流量带宽："<<capacity<<std::endl;

			path_capacity.push_back(capacity);
			path_consumptionNodeID.push_back(Vertex_Table[cc].consumptionNodeID);
			sum_demanded -= capacity;
			//std::cout<<"sum_demanded:"<<sum_demanded<<std::endl;
			new_path.push_back(tmp_queue);//将路径压入到临时路径vector中
			new_path_price.push_back(min_price*capacity);//将价格压入临时价格vector中
			//更新图
			update_path(tmp_queue,cc);
		}
		return true;
	}
	/*
	* 运行算法
	*/
	Bulletin run_default(){
		int num_server = -1;	//设置服务器个数
		set_server(num_server);	//设置服务器
		MSMS_Dijkstra();
		calculate_sum_price();
		Bulletin bulletin(new_path,path_capacity,path_consumptionNodeID,num_VideoServer,sum_price);
		return bulletin;
	}

	Bulletin run(std::vector<int> &server_vect){

		set_server(server_vect);	//设置服务器
		bool find = MSMS_Dijkstra();
		calculate_sum_price();
		Bulletin bulletin(new_path,path_capacity,path_consumptionNodeID,num_VideoServer,sum_price);
		if(!find)
			bulletin.bulletin_price = INF;
		return bulletin;
	}

};

#endif
