#include "deploy.h"
#include <stdio.h>
#include <sstream>
#include <string>
#include <queue>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iostream>
using namespace std;
//你要完成的功能总入口
/*
* 更新：1.增加一个Graph的对象用来保存图的信息
		2.1在服务器直连的情况下沿着边来移动服务器以产生初始种群
		2.2在度最大的节点上布置服务器
		2.3在需求量最大的几个直连节点处放置服务器
 		3.判断服务器数量是否大于消费节点个数，若大于，则直接返回一个较大的数，以便在下一轮扔掉，减少计算量
*/


/*
* GA:遗传算法	（未找到最优值）
* Violence:暴力求解
* cheat:使用了别人的解，用于验证路径的搜索
* run_default:服务器设置在直连位置处的求解


* 接口使用
* 函数定义：
* string Function(char * topo[MAX_EDGE_NUM], int line_num){
	Graph tgraph(topo,line_num);//构造图
	//Todo 生成服务器的算法
	// Your code
	vector<int>vectDec{xx,xx,xx,...};//服务器放置的位置 例：{1,3,5,44};
	Bulletin tmp_bulletin = tgraph.run(vectDec);	//调用run()得到路径以及费用
	//tmp_bulletin包括{bulletin_path(路径),bulletin_path_capacity(路径对应的带宽),
	//bulletin_path_consumptionNodeID(路径所服务的消费节点),
	//num_VideoServer(服务器数量),bulletin_price(方案的价格)}
	return format_result(tmp_bulletin);//将路径返回
}
*/

//函数声明
string GA(char * topo[MAX_EDGE_NUM], int line_num);
string Violence(char * topo[MAX_EDGE_NUM], int line_num);
string cheat(char * topo[MAX_EDGE_NUM], int line_num);
string run_default(char * topo[MAX_EDGE_NUM], int line_num);
string run_break(char * topo[MAX_EDGE_NUM], int line_num);

vector<int> run_delete(char * topo[MAX_EDGE_NUM],int line_num);
vector<int> find_max_bandwidth(vector<vertex> & Vertex_Table,int n);
void Aberrance(vector<vector<int >> &,double);	//变异
bool mating(vector<vector<int>> &,vector<vector<int>> &,vector<int> &,int);//交叉
void move(vector<vertex> &, vector<int> &);	//沿着边移动
string format_result(Bulletin &bulletin);
string NumberToString(int Number);
int random(double start, double end);
vector<int> random(double, double,unsigned int, bool);

vector<int> transBin2Dec(vector<int> & Bin_vect);
vector<int> transDec2Bin(vector<int> & Dec_vect ,int size);

double get_fitness(int price, int minFval);
int get_loc(vector<double> &acc_fitness,double proba);
void swap(vector<int> &vect, int i, int j);

void deploy_server(char * topo[MAX_EDGE_NUM], int line_num,char * filename)
{

	clock_t start ,end;
	start = clock();
	//Graph mGraph(topo,line_num);
	//mGraph.printGraph();
	//string topo_string = run_delete(topo,line_num);	//服务器放置在直连位置,依次删除需求最小的服务器布置位置
	//string topo_string = run_default(topo,line_num);	//服务器放置在直连位置
	string topo_string = GA(topo,line_num);	//遗传算法
	//string topo_string = Violence(topo,line_num);	//暴力搜寻
	//string topo_string = cheat(topo,line_num);	//服务器放置在制定位置
	//string topo_string = run_break(topo,line_num);

	// 需要输出的内容
	const char * topo_file = topo_string.c_str();
	//char * topo_file = (char *)"17\n\n0 8 0 20\n21 8 0 20\n9 11 1 13\n21 22 2 20\n23 22 2 8\n1 3 3 11\n24 3 3 17\n27 3 3 26\n24 3 3 10\n18 17 4 11\n1 19 5 26\n1 16 6 15\n15 13 7 13\n4 5 8 18\n2 25 9 15\n0 7 10 10\n23 24 11 23";

	// 直接调用输出文件的方法输出到指定文件中(ps请注意格式的正确性，如果有解，第一行只有一个数据；第二行为空；第三行开始才是具体的数据，数据之间用一个空格分隔开)
	write_result(topo_file, filename);
	end = clock();
	cout<<"总时间："<<double((end-start)/CLOCKS_PER_SEC)<<"s ="<<
	double((end-start)/(CLOCKS_PER_SEC+0.0)*1000.0)<<"ms"<<endl;
}

vector<int> run_delete(char * topo[MAX_EDGE_NUM],int line_num)
{
    Graph tgraph(topo,line_num);

    auto Consumption = tgraph.get_Consumption();
    auto Vertex_Table = tgraph.get_vertex();
    //Bulletin tmp_bulletin = tgraph.run(Consumption);
    //cout<<"Before   bulletin.bulletin_price:"<<tmp_bulletin.bulletin_price<<endl;
    sort(Consumption.begin(),Consumption.end(),[Vertex_Table](const int &a, const int &b){
    return Vertex_Table[a].sumConsumptionBandWidth<Vertex_Table[b].sumConsumptionBandWidth;});

    //Graph tgraph(topo,line_num);
    Bulletin final_bulletin = tgraph.run(Consumption);
    auto price = final_bulletin.bulletin_price ;//直连总成本
    //cout<<"After   bulletin.bulletin_price:"<<final_bulletin.bulletin_price<<endl;
	auto markConsumption = Consumption;
    for(auto iter=Consumption.begin();iter!=Consumption.end();++iter)
    {
        Graph tgraph1(topo,line_num);
        //cout<<"BeforeErase  "<<Consumption[1]<<' '<<Vertex_Table[Consumption[1]].sumConsumptionBandWidth<<endl;
        Consumption.erase(Consumption.begin());
        //cout<<"AfterErase  "<<Consumption[1]<<' '<<Vertex_Table[Consumption[1]].sumConsumptionBandWidth<<endl;
        auto tmp_bulletin = tgraph1.run(Consumption);
        if(tmp_bulletin.bulletin_price==INF)break;
        if(tmp_bulletin.bulletin_price<price)
        {
            price = tmp_bulletin.bulletin_price;
            final_bulletin = tmp_bulletin;
			markConsumption = Consumption;
        }
        //cout<<"bulletin.bulletin_price:"<<tmp_bulletin.bulletin_price<<"   present_price"<<price<<endl;
    }

    return markConsumption;
}

//vector<int> run_delete(char * topo[MAX_EDGE_NUM],int line_num)

string run_default(char * topo[MAX_EDGE_NUM], int line_num)
{
	Graph tgraph(topo,line_num);
	Bulletin tmp_bulletin = tgraph.run_default();
	cout<<"bulletin.bulletin_price:"<<tmp_bulletin.bulletin_price<<endl;
	return format_result(tmp_bulletin);
}

string cheat(char * topo[MAX_EDGE_NUM], int line_num)
{
	Graph tgraph(topo,line_num);
	//vector<int>vectDec{7,13,15,22,37,38,43};	//case0 2340:2042
	//vector<int>vectDec{10,22,26,29,35};		//case3 2700:2111
	//vector<int>vectDec{20,26,22,48,15,12,37};	//case4 2160:1967
	//vector<int>vectDec{0,3,22};	//case00
	vector<int>vectDec{0,1,24};	//case00
	//vector<int> vectDec{0,2,4,6,17,33,44,66,88,120};
	Bulletin tmp_bulletin = tgraph.run(vectDec);
	cout<<"bulletin.bulletin_price:"<<tmp_bulletin.bulletin_price<<endl;
	return format_result(tmp_bulletin);
}

string Violence(char * topo[MAX_EDGE_NUM], int line_num)
{
	Graph tgraph(topo,line_num);
	int num_Network_Node = tgraph.get_num_Network_Node();
	Bulletin bulletin = tgraph.run_default();//直连情况下的解
	for (int i = 0;i<num_Network_Node;i++){
		cout<<"i:"<<i<<endl;
		for(int j = i+1;j<num_Network_Node;j++){
			//cout<<"j:"<<j<<endl;
			for(int k = j+1;k<num_Network_Node;k++)
			{
				//cout<<"k:"<<k<<endl;
				Graph tgraph(topo,line_num);
				vector<int>vectDec{i,j,k};
				Bulletin tmp_bulletin = tgraph.run(vectDec);
				//cout<<"price:"<<tmp_bulletin.bulletin_price<<endl;
				if (tmp_bulletin.bulletin_price<bulletin.bulletin_price)
				{
					bulletin = tmp_bulletin;
				}
			}
		}
	}
	cout<<"bulletin.bulletin_price:"<<bulletin.bulletin_price<<endl;
	return format_result(bulletin);
}


string GA(char * topo[MAX_EDGE_NUM], int line_num)
{
	clock_t beg;
	beg = clock();
	Graph mGraph(topo,line_num);//存储图的信息
	auto Vertex_Table = mGraph.get_vertex();	//取出图的路径
	auto Consumption = mGraph.get_Consumption();	//直连消费节点的网络节点

	Graph igraph(topo,line_num);//用于产生初始解
	//算法初始化
	int num_Network_Node = igraph.get_num_Network_Node();

	int num_server_minus = 0;	//在去掉一部分直连的基础上再降低的服务器的个数
	//根据网络节点个数来设置种群规模
	int Population_Size = 150;//种群规模
	if(num_Network_Node>600){
		Population_Size =30;
		num_server_minus = 40;
	}	
	if(num_Network_Node>200&&num_Network_Node<400)
	{
		Population_Size = 40;
		num_server_minus = 40;
	}

	int iter_num = 200;	//迭代次数

	double Aberrance_Ratio = 0.2;	//变异率

	vector<vector<int> > Population(Population_Size, vector<int>(num_Network_Node));
	vector<int> fval(Population_Size);
	Bulletin bulletin = igraph.run_default();//直连情况下的解
	int minFval = bulletin.bulletin_price;	//minFval为整体最小值
	int default_fval = minFval;

	vector<int> init_pos_Dec = Consumption;
	vector<int> deleted_pos = run_delete(topo,line_num);
	vector<int> maxDegree_pos;
	int num_residue = deleted_pos.size();
	cout<<"NumOfSer......."<<num_residue<<endl;
	auto maxDegree_map = mGraph.find_max_degree(num_residue - num_server_minus);
	for(auto m = maxDegree_map.begin();m != maxDegree_map.end();m++)
	{
		maxDegree_pos.push_back(m->first);
	}
	vector<int> maxBandwidth_pos = find_max_bandwidth(Vertex_Table,num_residue-num_server_minus);

	//初始种
	//从带宽输出最大位置处往外移动
	for (auto i = 0; i <Population_Size/3; ++i)
	{
		Population[i] = transDec2Bin(maxBandwidth_pos,num_Network_Node);
		srand(unsigned(clock()));
		if(double(rand()/(RAND_MAX+1.0))<0.9)
			move(Vertex_Table,Population[i]);
	}

	//从度最大位置处随机移动
	for (auto i = Population_Size/3; i <Population_Size/3*2; ++i)
	{
		Population[i] = transDec2Bin(maxDegree_pos,num_Network_Node);
		srand(unsigned(clock()));
		if(double(rand()/(RAND_MAX+1.0))<0.9)
			move(Vertex_Table,Population[i]);
	}
	//从需求量最高的消费节点处随机移动
	for (auto i = Population_Size/3*2; i <Population_Size; ++i)
	{
		Population[i] = transDec2Bin(deleted_pos,num_Network_Node);
		srand(unsigned(clock()));
		if(double(rand()/(RAND_MAX+1.0))<0.9)
			move(Vertex_Table,Population[i]);
	}

	int times = 0;

	//退火过程中的参数设定
	vector<int> extend_prcie;
	vector<vector<int>> extend_population;
	vector<int> person_flag;
	while((times++)<iter_num){
		//cout<<"times------------------:"<<times<<"\n";
		int min_this_round = INF;
		extend_prcie.clear();
		extend_population.clear();
		person_flag.clear();

		//使用退火算法扩展种群规模
		extend_population = Population;
		for(int i = 0; i<Population_Size;i++)
		{
			auto new_person = Population[i];
			move(Vertex_Table,new_person);
			extend_population.push_back(new_person);
		}
		//计算新种群目标值
		for(int i = 0;i<Population_Size*2;i++){
			Graph tgraph(topo,line_num);
			auto vectDec = transBin2Dec(extend_population[i]);
			if(int(vectDec.size())>num_residue){
				extend_prcie.push_back(INF);

			}
			else{
				Bulletin tmp_bulletin = tgraph.run(vectDec);
				extend_prcie.push_back(tmp_bulletin.bulletin_price);
				if (tmp_bulletin.bulletin_price<bulletin.bulletin_price)
				{
					bulletin = tmp_bulletin;
					minFval = tmp_bulletin.bulletin_price;
				}
				if(tmp_bulletin.bulletin_price<min_this_round)
				{
					min_this_round = tmp_bulletin.bulletin_price;
				}
			}
			person_flag.push_back(i);
		}
		//取其中最好的Population_size个个体
		//从小往大排列
		sort(person_flag.begin(),person_flag.end(),[extend_prcie](const int &a, const int &b){
    return extend_prcie[a]<extend_prcie[b];});
		for(int i = 0;i<Population_Size;i++)
		{
			Population[i] = extend_population[person_flag[i]];
			fval[i] = extend_prcie[person_flag[i]];
		}
		//cout<<"本轮最好->:"<<min_this_round<<endl;
		//更新种群
		vector<vector<int> > new_Population;
		if(mating(Population,new_Population,fval,minFval))
			Population = new_Population;
		Aberrance(Population, Aberrance_Ratio);
		//cout<<"bulletin.bulletin_price:"<<bulletin.bulletin_price<<endl;
		if(((clock()-beg)/CLOCKS_PER_SEC)>80)
			break;
	}
	cout<<bulletin.bulletin_price<<"/"<<default_fval<<endl;
	return format_result(bulletin);
}
    /* 找到输出最大的n个节点
    */

    vector<int> find_max_bandwidth(vector<vertex> & Vertex_Table,int n)
    {
        vector<int> result_table;
        vector<vertex> bandTable = Vertex_Table;
        sort(bandTable.begin(),bandTable.end(),[](const vertex &a, const vertex &b){
            return a.sumBandWidth>b.sumBandWidth;});
        vector<vertex>::iterator it=bandTable.begin();
        for(int i=0; i<n; ++i)
        {
            result_table.push_back(it->networkNodeID);
            ++it;
        }
        return result_table;
    }

/*
* 变异
* Population:种群
* 变异率
*/
void Aberrance(vector<vector<int>> &Population, double ratio)
{
	unsigned int Population_Size = Population.size();
	unsigned int num_Network_Node = Population[0].size();
	unsigned int num_Aberrance = Population_Size*ratio;
	vector<int>  Aberrance_flag = random(0,Population_Size,num_Aberrance,false);
	for(unsigned int i= 0;i<Aberrance_flag.size();i++)
	{
		auto choosed = Aberrance_flag[i];
		auto flag = random(0,num_Network_Node,3,false);
		for(auto f = flag.begin();f!=flag.end();++f)
			Population[choosed][*f] = 1 - Population[choosed][*f];
	}
}

/*
* 交配
*
*/
bool mating(vector<vector<int>> &Population,vector<vector<int>> &new_Population,vector<int> &prices,int minFval)
{
	vector<double> fitness;
	vector<double> acc_fitness;
	double sum_fitness = 0.0;
	//计算适应度
	for(unsigned int i = 0;i<prices.size();i++)
	{
		fitness.push_back(get_fitness(prices[i], minFval));
		sum_fitness += fitness[i];
		acc_fitness.push_back(sum_fitness);
	}
	if(sum_fitness == 0)
	{
		return false;
	}
	//归一化适应度
	for(unsigned int i = 0;i<prices.size();i++)
	{
		fitness[i]/=sum_fitness;
		acc_fitness[i]/=sum_fitness;
	}

	for(unsigned int i= 0;i<Population.size();i+=2)
	{
		double proba1 = double(rand()/(RAND_MAX+1.0));
		double proba2 = double(rand()/(RAND_MAX+1.0));
		int loc1 = get_loc(acc_fitness,proba1);
		int loc2 = get_loc(acc_fitness,proba2);

		new_Population.push_back(Population[loc1]);
		new_Population.push_back(Population[loc2]);

		//选段交配
		/*
		vector<int> section = random(0,Population[0].size(),2,true);
		if(section[0]>section[1])
		{
			swap(section,0,1);
		}
		for(auto k = section[0];k<=section[1];k++)
		{
			new_Population[i][k] = Population[loc2][k];
			new_Population[i+1][k] = Population[loc1][k];
		}
		*/

		//选点交配
		int num_mating_points = random(0,Population[0].size());
		auto matingPoints = random(0,Population[0].size(),num_mating_points,false);
		for (auto iter = matingPoints.begin(); iter != matingPoints.end(); ++iter)
		{
			new_Population[i][*iter] = Population[loc2][*iter];
			new_Population[i+1][*iter] = Population[loc1][*iter];
		}

	}
	return true;

}

void move(vector<vertex> &Vertex_Table, vector<int> &single)
{
	auto single_Dec = transBin2Dec(single);
	auto size = single_Dec.size();
	auto pos = single_Dec[random(0,size)];
	auto flag = random(0,Vertex_Table[pos].degree);
	auto tmp = Vertex_Table[pos].adj;
	if(tmp == nullptr)
		return;
	while(flag--&&tmp->link!=nullptr)
	{
		tmp = tmp->link;
	}
	auto to_pos = tmp->dest;
	single[pos] = 0;
	single[to_pos] = 1;
}

void swap(vector<int> &vect, int i, int j)
{
	auto tmp = vect[i];
	vect[i] = vect[j];
	vect[j] = tmp;
}
/*
* 按概率取得位置
*/
int get_loc(vector<double> &acc_fitness,double proba)
{
	for(unsigned int i =0 ;i<acc_fitness.size();i++)
	{
		if(proba<acc_fitness[i])
			return i;
	}
	return -1;
}
/*
* 计算适应度
*/
double get_fitness(int price, int minFval)
{
	return exp(-(price - minFval)/minFval);
}

/*
* 转换为输出的格式
*/
string format_result(Bulletin &bulletin){
	string ret_string = "";
	string tmp_string = "";
	ret_string = NumberToString(bulletin.num_VideoServer);
	ret_string+="\n";
	int tmpNode = 0;
	int i = 0;
	auto new_path = bulletin.bulletin_path;
	for(auto it = new_path.begin();it!=new_path.end();it++){
		ret_string+="\n";
		while(it->size()>1){
			tmpNode = it->front();
			it->pop();
			tmp_string = NumberToString(tmpNode);
			ret_string+=tmp_string;ret_string+=" ";
		}
		tmp_string = NumberToString(bulletin.bulletin_path_consumptionNodeID[i]);
		ret_string+=tmp_string;ret_string+=" ";
		tmp_string = NumberToString(bulletin.bulletin_path_capacity[i]);
		ret_string+=tmp_string;
		i++;
	}
	return ret_string;
}

/*
* 将数字转化为字符串
*/
string NumberToString(int Number){
	ostringstream ss;
	ss << Number;
	return ss.str();
}


/*
* 产生范围内的随机整数
*/
int random(double start, double end)
{
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

/*
* 产生范围内的n个随机整数,duplication表示是否重复
*/
vector<int> random(double start, double end,unsigned int n, bool duplication = false)
{
	vector<int> ret(0);
	int num = -1;

	if(duplication){
		for(unsigned int i =0;i<n;i++)
		{
			num = start+(end-start)*rand()/(RAND_MAX + 1.0);
			ret.push_back(num);
		}
	}else{
		if(n>end-start)
			return ret;
		map<int, int> tmap;
		while(ret.size()<n){
			num = start+(end-start)*rand()/(RAND_MAX + 1.0);
			if(tmap[num]==0){
				tmap[num]=1;
				ret.push_back(num);
			}
		}
	}
	return ret;
}

/*
* 将二进制布站转化为十进制
*/
vector<int> transBin2Dec(vector<int> & Bin_vect)
{
	vector<int > ret;
	for (unsigned int i  = 0; i < Bin_vect.size(); ++i)
	{
		if(Bin_vect[i]==1)
			ret.push_back(i);
	}
	return ret;
}
/*
* 将十进制布站转化为二进制
*/
vector<int> transDec2Bin(vector<int> & Dec_vect ,int size)
{
	vector<int> ret(size,0);
	for (unsigned int i = 0; i < Dec_vect.size(); ++i)
	{
		ret[Dec_vect[i]] = 1;
	}
	return ret;
}
