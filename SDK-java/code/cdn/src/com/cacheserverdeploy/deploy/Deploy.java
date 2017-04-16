package com.cacheserverdeploy.deploy;


//------------lwd----------------
import java.util.*;

//
class AdjacentTableNodeInfo implements Cloneable{
    public int networkNodeID;
    public int degree;
    public int sumAllLinkBandWidth;
    public boolean isIsolatedNode;
    public boolean isVideoServer;
    public boolean isConnectedConsumptionNode;
    public int consumptionNodeID;
    public int sumConsumptionBandWidth;
    public LinkedList<NetworkLinkInfo> adjacentNetworkLinkArray;
    public ArrayList<Integer> adjacentNetworkNodeArray;

    //Constructor Function, to Initialize
    public AdjacentTableNodeInfo(){
        this.networkNodeID = -1;
        this.degree = -1;
        this.sumAllLinkBandWidth = 0;
        this.isIsolatedNode = false;
        this.isVideoServer = false;
        this.isConnectedConsumptionNode = false;
        this.consumptionNodeID = -1;
        this.sumConsumptionBandWidth = -1;
        this.adjacentNetworkLinkArray = new LinkedList<NetworkLinkInfo>();
        this.adjacentNetworkNodeArray = new ArrayList<Integer>();
    }
    public AdjacentTableNodeInfo(int networkNodeID){
        this.networkNodeID = networkNodeID;
        this.degree = -1;
        this.sumAllLinkBandWidth = 0;
        this.isIsolatedNode = false;
        this.isVideoServer = false;
        this.isConnectedConsumptionNode = false;
        this.consumptionNodeID = -1;
        this.sumConsumptionBandWidth = -1;
        this.adjacentNetworkLinkArray = new LinkedList<NetworkLinkInfo>();
        this.adjacentNetworkNodeArray = new ArrayList<Integer>();
    }

    @Override
    public AdjacentTableNodeInfo clone(){
        AdjacentTableNodeInfo clone = null;
        try{
            clone = (AdjacentTableNodeInfo)super.clone();
            clone.adjacentNetworkLinkArray = new LinkedList<NetworkLinkInfo>();
            for (NetworkLinkInfo n : this.adjacentNetworkLinkArray){
                clone.adjacentNetworkLinkArray.add(n.clone());
            }
            clone.adjacentNetworkNodeArray = new ArrayList<Integer>();
            for(int i: this.adjacentNetworkNodeArray){
                clone.adjacentNetworkNodeArray.add(i);
            }
        }catch (CloneNotSupportedException e){
            throw new RuntimeException(e);
        }

        return clone;
    }
    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.networkNodeID).append("%")
                .append(this.degree).append("%")
                .append(this.sumAllLinkBandWidth).append("%")
                .append(this.isIsolatedNode).append("%")
                .append(this.isVideoServer).append("%")
                .append(this.isConnectedConsumptionNode).append("%")
                .append(this.consumptionNodeID).append("%")
                .append(this.sumConsumptionBandWidth).append("%");
        for (NetworkLinkInfo n : this.adjacentNetworkLinkArray){
            strBuilder.append(n.toString()).append("#");
        }
        for (int i: adjacentNetworkNodeArray){
            strBuilder.append(i).append("@");
        }
        return strBuilder.toString();
    }
}

class SumAllLink implements Comparable<SumAllLink>{
    public int networkNodeID;
    public int degree;
    public int sumAllLinkBandWidth;

    public SumAllLink(){
        this.networkNodeID = -1;
        this.degree = -1;
        this.sumAllLinkBandWidth = 0;
    }
    public SumAllLink(int networkNodeID, int degree, int sumAllLinkBandWidth){
        this.networkNodeID = networkNodeID;
        this.degree = degree;
        this.sumAllLinkBandWidth = sumAllLinkBandWidth;
    }
    @Override
    public int compareTo(SumAllLink s){
        if (s.sumAllLinkBandWidth == this.sumAllLinkBandWidth){
            return s.degree - this.degree;//From big to small
        }else{
            return s.sumAllLinkBandWidth - this.sumAllLinkBandWidth;//From big to small
        }
    }
    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.networkNodeID).append(' ')
                .append(this.degree).append(' ')
                .append(this.sumAllLinkBandWidth);
        return strBuilder.toString();
    }
}
class NetworkLinkInfo implements Comparable<NetworkLinkInfo>,Cloneable{
    public int beginNetworkNodeID;
    public int endNetworkNodeID;
    public int sumLinkBandWidth;
    public int perLeaseCharge;

    //Constructor Function, to Initialize
    public NetworkLinkInfo(){
        this.beginNetworkNodeID = -1;
        this.endNetworkNodeID = -1;
        this.sumLinkBandWidth = -1;
        this.perLeaseCharge = -1;
    }
    public NetworkLinkInfo(int beginNetworkNodeID,int endNetworkNodeID,int sumLinkBandWidth,int perLeaseCharge){
        this.beginNetworkNodeID = beginNetworkNodeID;
        this.endNetworkNodeID = endNetworkNodeID;
        this.sumLinkBandWidth = sumLinkBandWidth;
        this.perLeaseCharge = perLeaseCharge;
    }
    @Override
    public NetworkLinkInfo clone(){
        NetworkLinkInfo clone = null;
        try{
            clone = (NetworkLinkInfo)super.clone();
        }catch (CloneNotSupportedException e){
            throw new RuntimeException(e);
        }

        return clone;
    }
    @Override
    public int compareTo(NetworkLinkInfo n){
//        if (this.sumLinkBandWidth == n.sumLinkBandWidth){
//            return this.perLeaseCharge - n.perLeaseCharge;
//        }else {
//            return n.sumLinkBandWidth - this.sumLinkBandWidth;
//        }
        if (this.perLeaseCharge == n.perLeaseCharge){
            return n.sumLinkBandWidth - this.sumLinkBandWidth;
        }else {
            return this.perLeaseCharge - n.perLeaseCharge;
        }
    }
    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.beginNetworkNodeID).append(' ')
                .append(this.endNetworkNodeID).append(' ')
                .append(this.sumLinkBandWidth).append(' ')
                .append(this.perLeaseCharge);
        return  strBuilder.toString();
    }
}
class PathUnitSimple{
    public int consumptionNodeID;
    public int occupationBandWidth;
    public NetworkLinkInfo networkLinkInfo;

    public PathUnitSimple(){
        this.consumptionNodeID = -1;
        this.occupationBandWidth = -1;
        this.networkLinkInfo = new NetworkLinkInfo();
    }
    public PathUnitSimple(int consumptionNodeID, int occupationBandWidth, NetworkLinkInfo networkLinkInfo){
        this.consumptionNodeID = consumptionNodeID;
        this.occupationBandWidth = occupationBandWidth;
        this.networkLinkInfo = networkLinkInfo;
    }

    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.consumptionNodeID).append(' ')
                .append(this.occupationBandWidth).append(' ')
                .append(this.networkLinkInfo.toString());
        return strBuilder.toString();
    }
}
class PathUnit{
    public PathUnitSimple pUS;
    public boolean isConnectedVideoServer;
    public Stack<PathUnitSimple> pathUnitSimpleStack;

    public PathUnit(){
        this.pUS = new PathUnitSimple();
        this.isConnectedVideoServer = false;
        this.pathUnitSimpleStack = new Stack<PathUnitSimple>();
    }
    public PathUnit(PathUnitSimple pUS){
        this.pUS = pUS;
        this.isConnectedVideoServer = false;
        this.pathUnitSimpleStack = new Stack<PathUnitSimple>();
    }

    public String getPathRoad(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.pUS.networkLinkInfo.endNetworkNodeID).append(' ')
                .append(this.pUS.networkLinkInfo.beginNetworkNodeID).append(' ');
        Stack<PathUnitSimple> pathUnitSimpleStackTemp = this.pathUnitSimpleStack;
        while (!pathUnitSimpleStackTemp.empty()){
            PathUnitSimple pUS = pathUnitSimpleStackTemp.pop();
            strBuilder.append(pUS.networkLinkInfo.beginNetworkNodeID).append(' ');
        }
        strBuilder.append(this.pUS.consumptionNodeID).append(' ').append(this.pUS.occupationBandWidth);
        return strBuilder.toString();
    }

    public int getPathCost(){
        int sumPerLeaseCharge = this.pUS.networkLinkInfo.perLeaseCharge;
        Stack<PathUnitSimple> pathUnitSimpleStackTemp = this.pathUnitSimpleStack;
        while (!pathUnitSimpleStackTemp.empty()){
            PathUnitSimple pUS = pathUnitSimpleStackTemp.pop();
            sumPerLeaseCharge += pUS.networkLinkInfo.perLeaseCharge;
        }
        return this.pUS.occupationBandWidth*sumPerLeaseCharge;
    }
    //Unused for now
    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.pUS.toString()).append(' ')
                .append(this.isConnectedVideoServer);
        return strBuilder.toString();
    }

}
class NodePath{
    public AdjacentTableNodeInfo aTNode;
    public int predecessorNetworkNodeID;
    public PathUnit pU;

    //public int occupationBandWidth;
    //public int sumConsumptionBandWidth;

    public NodePath(){
        this.aTNode = new AdjacentTableNodeInfo();
        this.predecessorNetworkNodeID = -1;
        this.pU = new PathUnit();
        //this.occupationBandWidth = -1;
    }
    //Unused for now
    @Override
    public String toString(){
        return this.aTNode.toString() + " $ " + this.pU.toString();
    }
}

/*class Path{
    public int consumptionNodeID;
    public Stack<Integer> networkNodeStack;
    public int occupationBandWidth;
    
    public Path(){
		this.consumptionNodeID = -1;
		this.networkNodeStack = new Stack<Integer>();
		this.occupationBandWidth = -1;
	}	
	
	public Path(int consumptionNodeID){
		this.consumptionNodeID = consumptionNodeID;
		this.networkNodeStack = new Stack<Integer>();
		this.occupationBandWidth = -1;
	}	
	@Override
	public String toString(){
		StringBuilder strBuilder = new StringBuilder();
		for (int i: this.networkNodeStack){
			strBuilder.append(i).append(" ");
		}
		strBuilder.append(this.consumptionNodeID).append(" ")
		          .append(this.occupationBandWidth);
		return strBuilder.toString();
	}
}*/
//
class ConsumptionNodeInfo implements Comparable<ConsumptionNodeInfo>{
    public int consumptionNodeID;
    public int conNetworkNodeID;
    public int sumConsumptionBandWidth;
    public ArrayList<String> strPathSimpleArray;
    public int costPathSimple;
    //public int nodeLevel;
    //public stack<AdjacentTableNodeInfo> pathNodeStack;


    //Constructor Function, to Initialize
    public ConsumptionNodeInfo(){
        this.consumptionNodeID = -1;
        this.conNetworkNodeID = -1;
        this.sumConsumptionBandWidth = -1;
        this.strPathSimpleArray = new ArrayList<String>();
        this.costPathSimple = -1;
        //this.nodeLevel = 1;
    }
    public ConsumptionNodeInfo(int consumptionNodeID){
        this.consumptionNodeID = consumptionNodeID;
        this.conNetworkNodeID = -1;
        this.sumConsumptionBandWidth = -1;
        this.strPathSimpleArray = new ArrayList<String>();
        this.costPathSimple = -1;
        //this.nodeLevel = 1;
    }
    public ConsumptionNodeInfo(int consumptionNodeID,int conNetworkNodeID,int sumConsumptionBandWidth){
        this.consumptionNodeID = consumptionNodeID;
        this.conNetworkNodeID = conNetworkNodeID;
        this.sumConsumptionBandWidth = sumConsumptionBandWidth;
        this.strPathSimpleArray = new ArrayList<String>();
        this.costPathSimple = -1;
    }

    public String getInitPath(){
        return this.conNetworkNodeID + " " +this.consumptionNodeID +" "+this.sumConsumptionBandWidth;
    }
    @Override
    public int compareTo(ConsumptionNodeInfo c){
        return c.sumConsumptionBandWidth - this.sumConsumptionBandWidth;
        //return this.sumConsumptionBandWidth - c.sumConsumptionBandWidth;
    }
    @Override
    public String toString(){
        StringBuilder strBuilder = new StringBuilder();
        strBuilder.append(this.consumptionNodeID).append(' ')
                .append(this.conNetworkNodeID).append(' ')
                .append(this.sumConsumptionBandWidth).append(' ');
        //.append(this.nodeLevel);
        return strBuilder.toString();
    }
}

//class ShortestPathUnit{
//    public int beginNetworkNodeID;
//    public int endNetworkNodeID;
//    public Stack<NetworkLinkInfo> stackPathUnion;
//    public Stack<Integer> stackNodes;
//
//    public ShortestPathUnit(){
//        this.beginNetworkNodeID = -1;
//        this.endNetworkNodeID = -1;
//        this.stackPathUnion = new Stack<NetworkLinkInfo>();
//        this.stackNodes = new Stack<Integer>();
//    }
//
//    @Override
//    public String toString(){
//        return this.beginNetworkNodeID + " "+ this.endNetworkNodeID;
//    }
//}
public class Deploy
{
    public static final int MAX_SUM_NETWORKNODES = 1000;
    public static final int MAX_DEGREE_OF_NETWORKNODE = 20;
    public static final int MAX_SUM_NETWORKLINKS = 10000;
    public static final int MAX_SUM_CONSUMPTION_NODES = 500;
    public static final int INF = Integer.MAX_VALUE;


    public static int sumNetworkNodes;
    public static int sumNetworkLinks;
    public static int sumConsumptionNodes;

    public static int costPerVideoServer;
    public static int[][] adjacentMatrixPerLeaseCharge;
    public static int[][] adjacentMatrixBandWidth;
    public static ArrayList<AdjacentTableNodeInfo> adjacentTable;
    public static String[] resultBase;
    public static int costBase;
    public static int sumVideoServerBase;

    /**
     * 你需要完成的入口
     * <功能详细描述>
     * @param graphContent 用例信息文件
     * @return [参数说明] 输出结果信息
     * @see [类、类#方法、类#成员]
     */
    public static String[] deployServer(String[] graphContent)
    {
        /**do your work here**/
        //-----lwd-----
        //1. Getting Sum of NetworkNodes,NetworkLinks,ConsumptionBandWidth
        String[] strSumsArray = graphContent[0].split(" ");
        sumNetworkNodes = Integer.parseInt(strSumsArray[0]);
        sumNetworkLinks = Integer.parseInt(strSumsArray[1]);
        sumConsumptionNodes = Integer.parseInt(strSumsArray[2]);
        sumVideoServerBase = sumConsumptionNodes/5;

        resultBase = new String[sumConsumptionNodes+2];
        resultBase[0] = String.valueOf(sumConsumptionNodes);
        resultBase[1] = "";

        //2. Getting Cost of Per Video Server
        costPerVideoServer = Integer.parseInt(graphContent[2]);

        costBase = costPerVideoServer * sumConsumptionNodes;

        //3. Building AdjacentTable Object
        adjacentTable = new ArrayList<AdjacentTableNodeInfo>();
        //Initializing AdjacentTable Object,O(n)
        for(int i=0;i<sumNetworkNodes;i++){
            AdjacentTableNodeInfo a = new AdjacentTableNodeInfo(i);
            adjacentTable.add(a);
        }
        //NetworkLinkInfo[][] adjacentMatrix = new NetworkLinkInfo[sumNetworkNodes][sumNetworkNodes];

        //4. Getting EdgeSet Array building AdjacentTable
        String[] strNetworkLinkArray;
        for (int i=4;i<sumNetworkLinks+4;i++){
            strNetworkLinkArray = graphContent[i].split(" ");
            //RightNetworkLink Object
            NetworkLinkInfo nRight = new NetworkLinkInfo();
            //ReverseNetworkLink Object
            NetworkLinkInfo nReverse = new NetworkLinkInfo();
            nReverse.endNetworkNodeID = nRight.beginNetworkNodeID = Integer.parseInt(strNetworkLinkArray[0]);
            nReverse.beginNetworkNodeID = nRight.endNetworkNodeID = Integer.parseInt(strNetworkLinkArray[1]);
            nReverse.sumLinkBandWidth = nRight.sumLinkBandWidth = Integer.parseInt(strNetworkLinkArray[2]);
            nReverse.perLeaseCharge = nRight.perLeaseCharge = Integer.parseInt(strNetworkLinkArray[3]);
            //Building AdjacentTable OK, double of edgeSet array
            adjacentTable.get(nRight.beginNetworkNodeID).adjacentNetworkLinkArray.add(nRight);
            //adjacentTable.get(nRight.beginNetworkNodeID).adjacentNetworkNodeArray.add(nRight.endNetworkNodeID);
            adjacentTable.get(nRight.beginNetworkNodeID).sumAllLinkBandWidth += nRight.sumLinkBandWidth;
            adjacentTable.get(nReverse.beginNetworkNodeID).adjacentNetworkLinkArray.add(nReverse);
            //adjacentTable.get(nReverse.beginNetworkNodeID).adjacentNetworkNodeArray.add(nReverse.endNetworkNodeID);
            adjacentTable.get(nReverse.beginNetworkNodeID).sumAllLinkBandWidth += nReverse.sumLinkBandWidth;

        }

        //5. Getting ConsumptionNode Array and building AdjacentTable
        ArrayList<ConsumptionNodeInfo> consumptionNodeArray = new ArrayList<ConsumptionNodeInfo>();
        String[] strConsumptionNodeArray;
        for (int i=sumNetworkLinks+5;i<sumNetworkLinks+5+sumConsumptionNodes;i++){
            strConsumptionNodeArray = graphContent[i].split(" ");
            //Building ConsumptionNOdeInfo Object
            ConsumptionNodeInfo c = new ConsumptionNodeInfo();
            c.consumptionNodeID = Integer.parseInt(strConsumptionNodeArray[0]);
            c.conNetworkNodeID = Integer.parseInt(strConsumptionNodeArray[1]);
            c.sumConsumptionBandWidth = Integer.parseInt(strConsumptionNodeArray[2]);
            resultBase[i-sumNetworkLinks-3] = c.getInitPath();
            //Adding to this.consumptionNodeArray
            consumptionNodeArray.add(c);
            //Modifying AdjacentTable
            adjacentTable.get(c.conNetworkNodeID).isConnectedConsumptionNode = true;
            adjacentTable.get(c.conNetworkNodeID).consumptionNodeID = c.consumptionNodeID;
            adjacentTable.get(c.conNetworkNodeID).sumConsumptionBandWidth = c.sumConsumptionBandWidth;
        }

        //6. K-shell: Removing Isolated Node Recursively to Simplify Network Topology
        removeIsolatedNode(adjacentTable);

        //7. Sorting adjacentNetworkLinkArray of adjacentTable Nodes by perLeaseCharge and sumConsumptionBandWidth
        ArrayList<SumAllLink> sumALArray = new ArrayList<SumAllLink>();
        for (AdjacentTableNodeInfo a : adjacentTable){
            Collections.sort(a.adjacentNetworkLinkArray);

            SumAllLink s = new SumAllLink(a.networkNodeID, a.degree, a.sumAllLinkBandWidth);
            sumALArray.add(s);
            //System.out.println(a.toString());
            //System.out.println(s.toString());
        }
        Collections.sort(sumALArray);

        //Setting VideoServer 1
//        for (int i=0;i<sumVideoServerBase;i++){
//            adjacentTable.get(sumALArray.get(i).networkNodeID).isVideoServer = true;
//        }
        //for(SumAllLink s : sumALArray){
        //System.out.println(s.toString());
        //}

        //System.out.println(sumNetworkNodes + "," + sumNetworkLinks + "," + sumConsumptionNodes);
        //System.out.println(costPerVideoServer);
        //8. Setting VideoServer 2: 简化图结构
        ArrayList<AdjacentTableNodeInfo> adjacentTableTemp = new ArrayList<AdjacentTableNodeInfo>(adjacentTable.size());
        for(AdjacentTableNodeInfo a:adjacentTable){
            adjacentTableTemp.add(a.clone());
        }

        int[] videoServerArray = setVideoServer(adjacentTableTemp);
        for (int i: videoServerArray){
            adjacentTable.get(i).isVideoServer = true;
            //System.out.println(i);
        }
//        //Output Test
//
        for (AdjacentTableNodeInfo a : adjacentTable){
            for (NetworkLinkInfo n: a.adjacentNetworkLinkArray){
                if (!adjacentTable.get(n.endNetworkNodeID).isVideoServer&&!adjacentTable.get(n.endNetworkNodeID).isConnectedConsumptionNode){
                    a.adjacentNetworkNodeArray.add(n.endNetworkNodeID);
                }
            }
            //System.out.println(a.toString());
        }

        Collections.sort(consumptionNodeArray);

        //Choosing Video Server


//        adjacentMatrixPerLeaseCharge = new int[sumNetworkNodes][sumNetworkNodes];
//        adjacentMatrixBandWidth = new int[sumNetworkNodes][sumNetworkNodes];
//        for (int i=0;i<sumNetworkNodes;i++){
//            for (int j=0;j<sumNetworkNodes;j++){
//                if (i == j){
//                    adjacentMatrixPerLeaseCharge[i][j] = 0;
//                    adjacentMatrixBandWidth[i][j] = 0;
//                }else{
//                    adjacentMatrixPerLeaseCharge[i][j] = INF;
//                    adjacentMatrixBandWidth[i][j] = INF;
//                }
//            }
//        }
//        for (AdjacentTableNodeInfo a : adjacentTable){
//            for (NetworkLinkInfo n: a.adjacentNetworkLinkArray){
//                adjacentMatrixPerLeaseCharge[n.beginNetworkNodeID][n.endNetworkNodeID] = n.perLeaseCharge;
//                adjacentMatrixBandWidth[n.beginNetworkNodeID][n.endNetworkNodeID] = n.sumLinkBandWidth;
//            }
//            //System.out.println(a.toString());
//        }

        //int[][] dist = new int[sumNetworkNodes][sumNetworkNodes];
        //int[][] path = new int[sumNetworkNodes][sumNetworkNodes];

        //ShortestPathUnit[][] shortestPathUnits = new ShortestPathUnit[sumNetworkNodes][sumNetworkNodes];

        //ArrayList<Integer>[][] road = new ArrayList<Integer>[sumNetworkNodes][sumNetworkNodes];

        //floyd(path,dist,shortestPathUnits);
//        for (int i=0;i<sumNetworkNodes;i++){
//            for (int j=0;j<sumNetworkNodes;j++){
//                System.out.print(adjacentMatrixPerLeaseCharge[i][j]+" ");
//            }
//            System.out.println();
//        }
//        for (int i=0;i<sumNetworkNodes;i++){
//            for (int j=0;j<sumNetworkNodes;j++){
//                System.out.print(adjacentMatrixBandWidth[i][j]+" ");
//            }
//            System.out.println();
//        }

        /*VideoServer Combining Method*/
        //初始状态，服务器放在消费节点相连的网络节点，即放在门口
//        for (AdjacentTableNodeInfo a: adjacentTable){
//            if (a.isConnectedConsumptionNode){
//                a.isVideoServer = true;
//            }
//        }
//        for (ConsumptionNodeInfo c: consumptionNodeArray){
//            AdjacentTableNodeInfo a = adjacentTable.get(c.conNetworkNodeID);
//            System.out.println(c.toString());
//            System.out.println(a.toString());
//            //向前跳一步
//            if (a.sumAllLinkBandWidth >= c.sumConsumptionBandWidth){
//                a.isVideoServer = false;
//                int sumBWTemp = 0;
//                for (NetworkLinkInfo n: a.adjacentNetworkLinkArray){
//
//                    sumBWTemp += n.sumLinkBandWidth;
//                    if (sumBWTemp >= c.sumConsumptionBandWidth){
//                        int occupationBWTemp = n.sumLinkBandWidth - (sumBWTemp - c.sumConsumptionBandWidth);
//                        c.costPathSimple += occupationBWTemp * n.perLeaseCharge;
//                        if (c.costPathSimple >= costPerVideoServer){
//                            a.isVideoServer = true;
//                            c.costPathSimple = 0;
//                            c.strPathSimpleArray.add(c.getInitPath());
//                            //break;
//                        }else{
//                            AdjacentTableNodeInfo aTTemp = adjacentTable.get(n.endNetworkNodeID);
//                            aTTemp.isVideoServer = true;
//                            String strPathTemp = n.endNetworkNodeID + " " + n.beginNetworkNodeID + " "+ c.consumptionNodeID + " "+ occupationBWTemp;
//                            c.strPathSimpleArray.add(strPathTemp);
//                        }
//                        break;
//
//                    }else{
//                        int occupationBWTemp = n.sumLinkBandWidth;
//                        c.costPathSimple += occupationBWTemp * n.perLeaseCharge;
//                        if (c.costPathSimple >= costPerVideoServer){
//                            a.isVideoServer = true;
//                            c.costPathSimple = 0;
//                            c.strPathSimpleArray.add(c.getInitPath());
//                            break;
//                        }else{
//                            AdjacentTableNodeInfo aTTemp = adjacentTable.get(n.endNetworkNodeID);
//                            aTTemp.isVideoServer = true;
//                            String strPathTemp = n.endNetworkNodeID + " " + n.beginNetworkNodeID + " "+ c.consumptionNodeID + " "+ occupationBWTemp;
//                            c.strPathSimpleArray.add(strPathTemp);
//                        }
//                    }
//                }
//
//
//            }
//        }
        //=========================Stop=========================




        //-----------------------------------老的方法------------------------------------------
        //9.
        //ArrayList<Path> pathArray = new ArrayList<Path>();
        //ArrayList<String> strPathArray = new ArrayList<String>();
        //int sumCostAllPaths = 0;

        //使用栈结构保存各个阶段的邻接表
        Stack<ArrayList<AdjacentTableNodeInfo>> stackAdjacentTable = new Stack<ArrayList<AdjacentTableNodeInfo>>();
        stackAdjacentTable.push(adjacentTableTemp);
        for (ConsumptionNodeInfo c : consumptionNodeArray){
            ArrayList<AdjacentTableNodeInfo> adjacentTableTempC = new ArrayList<AdjacentTableNodeInfo>(adjacentTable.size());
            ArrayList<AdjacentTableNodeInfo> adjacentTablePeek = stackAdjacentTable.peek();
            for(AdjacentTableNodeInfo a:adjacentTablePeek){
                adjacentTableTempC.add(a.clone());
            }
            //stackAdjacentTable.push(adjacentTableTemp);
            //int sumConsumptionBandWidthTemp = c.sumConsumptionBandWidth;
            //System.out.println(c.toString());
            boolean[] isVisitArray = new boolean[sumNetworkNodes];//Default value is false
            AdjacentTableNodeInfo a = adjacentTableTempC.get(c.conNetworkNodeID);
            //System.out.println(a.toString());
            //Extreme Case
            if (a.isVideoServer){
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.append(a.networkNodeID).append(' ').append(c.consumptionNodeID).append(' ').append(c.sumConsumptionBandWidth);
                //strPathArray.add(strBuilder.toString());
            } else if (a.sumAllLinkBandWidth < c.sumConsumptionBandWidth){
                a.isVideoServer = true;
                StringBuilder strBuilder = new StringBuilder();
                strBuilder.append(a.networkNodeID).append(' ').append(c.consumptionNodeID).append(' ').append(c.sumConsumptionBandWidth);
                //strPathArray.add(strBuilder.toString());
            } else {
                //Queue for BFS
                ArrayDeque<NodePath> nodePathQueue = new ArrayDeque<NodePath>();
                NodePath nPRoot = new NodePath();
                nPRoot.aTNode = a;
                //nPRoot.pU = ;
                nPRoot.pU.pUS.occupationBandWidth = c.sumConsumptionBandWidth;
                nodePathQueue.push(nPRoot);
                isVisitArray[a.networkNodeID] = true;
                //The logic is not perfect and come on
                while (!nodePathQueue.isEmpty()){
                    NodePath nPPoll = nodePathQueue.pollLast();
                    AdjacentTableNodeInfo aTTemp = adjacentTableTempC.get(nPPoll.aTNode.networkNodeID);
                    int sumLinkBandWidthTemp = 0;
                    int flag = 0;
                    ArrayList<NetworkLinkInfo> nVideoServerArray = new ArrayList<NetworkLinkInfo>();

                    //New Logic
//                    for (NetworkLinkInfo n : aTTemp.adjacentNetworkLinkArray){
//                        AdjacentTableNodeInfo endNode = adjacentTable.get(n.endNetworkNodeID);
//                        if(endNode.isVideoServer){
//                            nVideoServerArray.add(n);
//                        }
//                    }
                    //if (!nVideoServerArray.isEmpty()){
                        //for (NetworkLinkInfo n: nVideoServerArray){

                        //}
                    //}
                    for (NetworkLinkInfo n : aTTemp.adjacentNetworkLinkArray){
                        // && !adjacentTable.get(n.endNetworkNodeID).isConnectedConsumptionNode
                        if (n.endNetworkNodeID != nPPoll.predecessorNetworkNodeID && n.sumLinkBandWidth > 0&&!isVisitArray[n.endNetworkNodeID]){
                            AdjacentTableNodeInfo endNode = adjacentTableTempC.get(n.endNetworkNodeID);
                            //if (endNode.isVideoServer){

                            //}
                            int sumLeftLinkBandWidth = 0;
                            if (!endNode.isVideoServer){
                                for (NetworkLinkInfo nEnd : endNode.adjacentNetworkLinkArray){
                                    if (nEnd.endNetworkNodeID != aTTemp.networkNodeID && !aTTemp.adjacentNetworkNodeArray.contains(nEnd.endNetworkNodeID)){
                                        sumLeftLinkBandWidth += nEnd.sumLinkBandWidth;
                                    }
                                    if (sumLeftLinkBandWidth > nPPoll.pU.pUS.occupationBandWidth){
                                        break;
                                    }
                                }
                            }

                            sumLinkBandWidthTemp += n.sumLinkBandWidth;
                            //int flag = 0;
                            if (sumLinkBandWidthTemp >= nPPoll.pU.pUS.occupationBandWidth){
                                //isVisitArray[endNode.networkNodeID] = true;
                                int occupationBandWidthTemp = n.sumLinkBandWidth - (sumLinkBandWidthTemp - nPPoll.pU.pUS.occupationBandWidth);
                                PathUnitSimple pUSTemp = new PathUnitSimple(c.consumptionNodeID,occupationBandWidthTemp,n);
                                PathUnit pUTemp = new PathUnit(pUSTemp);
                                if (nPPoll.pU.pUS.consumptionNodeID != -1){
                                    for (PathUnitSimple p: nPPoll.pU.pathUnitSimpleStack){
                                        pUTemp.pathUnitSimpleStack.push(p);
                                    }
                                    pUTemp.pathUnitSimpleStack.push(nPPoll.pU.pUS);
                                }
                                if (endNode.isVideoServer){
                                    n.sumLinkBandWidth = sumLinkBandWidthTemp - nPPoll.pU.pUS.occupationBandWidth;
                                    pUTemp.isConnectedVideoServer = true;
                                    c.strPathSimpleArray.add(pUTemp.getPathRoad());
                                    //System.out.println(pUTemp.getPathRoad());
                                    c.costPathSimple += pUTemp.getPathCost();
                                    //break;
                                    flag = 1;
                                    break;
                                }else {
                                    //Dead Node, Can not walk
                                    if (occupationBandWidthTemp > sumLeftLinkBandWidth){
                                        sumLinkBandWidthTemp -= n.sumLinkBandWidth;
                                    }
                                     else {
                                        //PathUnit pUTemp = new PathUnit(c.consumptionNodeID,n.beginNetworkNodeID,n.endNetworkNodeID,occupationBandWidthTemp,n.perLeaseCharge);
                                        flag = 1;
                                        //Left bandwidth of this networkLink
                                        n.sumLinkBandWidth = sumLinkBandWidthTemp - nPPoll.pU.pUS.occupationBandWidth;

                                        NodePath nPTemp = new NodePath();
                                        nPTemp.aTNode = endNode;
                                        nPTemp.predecessorNetworkNodeID = nPPoll.aTNode.networkNodeID;
                                        nPTemp.pU = pUTemp;
                                        //nPTemp.occupationBandWidth = pUTemp.occupationBandWidth;
                                        nodePathQueue.add(nPTemp);
                                        isVisitArray[endNode.networkNodeID] = true;
                                        break;
                                    }
                                }
                                //break;
                            } else {
                                //isVisitArray[endNode.networkNodeID] = true;
                                int occupationBandWidthTemp = n.sumLinkBandWidth;
                                PathUnitSimple pUSTemp = new PathUnitSimple(c.consumptionNodeID,occupationBandWidthTemp,n);
                                PathUnit pUTemp = new PathUnit(pUSTemp);
                                //pUTemp.occupationBandWidth = occupationBandWidthTemp;
                                if (nPPoll.pU.pUS.consumptionNodeID != -1){
                                    for (PathUnitSimple p: nPPoll.pU.pathUnitSimpleStack){
                                        pUTemp.pathUnitSimpleStack.push(p);
                                    }
                                    pUTemp.pathUnitSimpleStack.push(nPPoll.pU.pUS);
                                }
                                if (endNode.isVideoServer){
                                    n.sumLinkBandWidth = 0;
                                    c.strPathSimpleArray.add(pUTemp.getPathRoad());
                                    //System.out.println(pUTemp.getPathRoad());
                                    c.costPathSimple += pUTemp.getPathCost();
                                }

                                 else {
                                    //Dead Node, Can not walk
                                    if (occupationBandWidthTemp > sumLeftLinkBandWidth){
                                        sumLinkBandWidthTemp -= n.sumLinkBandWidth;
                                    }
                                     else {
                                        n.sumLinkBandWidth = 0;
                                        NodePath nPTemp = new NodePath();
                                        nPTemp.aTNode = endNode;
                                        nPTemp.predecessorNetworkNodeID = nPPoll.aTNode.networkNodeID;
                                        nPTemp.pU = pUTemp;
                                        //nPTemp.occupationBandWidth = pUTemp.occupationBandWidth;
                                        nodePathQueue.add(nPTemp);
                                        isVisitArray[endNode.networkNodeID] = true;
                                    }
                                }
                            }

                        }
                    }
                    if (flag == 0 || c.costPathSimple >= costPerVideoServer){
                        aTTemp.isVideoServer = true;
                        StringBuilder strBuilder = new StringBuilder();
                        strBuilder.append(c.conNetworkNodeID).append(' ').append(c.consumptionNodeID).append(' ').append(c.sumConsumptionBandWidth);
                        c.strPathSimpleArray.clear();
                        c.strPathSimpleArray.add(strBuilder.toString());
                        c.costPathSimple = 0;

                        break;
                    }else{
                        stackAdjacentTable.push(adjacentTableTempC);//更新邻接表操作
                    }
                    //System.out.println(a.toString());
                    //System.out.println(c.toString());
                }
            }
        }
        //---------------------------老的方法-----------------------------------------------------
        //Printing adjacentTable for Testing
        //for(AdjacentTableNodeInfo a : adjacentTable){
        //System.out.println(a.toString());
        //}


        //for (String str : strPathArray){
        //System.out.println(str);
        //}
        int sumCostAllPaths = 0;
        ArrayList<String> strPathArray = new ArrayList<String>();
        TreeSet<Integer> tSet = new TreeSet<Integer>();
        for (ConsumptionNodeInfo c : consumptionNodeArray){
            sumCostAllPaths += c .costPathSimple;
            for (String str : c.strPathSimpleArray){
                strPathArray.add(str);
                String[] strArray = str.split(" ");
                int node = Integer.parseInt(strArray[0]);
                tSet.add(node);
                //System.out.println(str);
            }
        }
        sumCostAllPaths += tSet.size()*costPerVideoServer;
        //System.out.println(strPathArray.size());

        //System.out.println(sumCostAllPaths);
        //System.out.println(costPerVideoServer * sumConsumptionNodes);

        String[] resultTemp = new String[strPathArray.size() + 2];
        resultTemp[0] = String.valueOf(sumCostAllPaths);
        resultTemp[1] = "";
        for (int i=0;i<strPathArray.size();i++){
            resultTemp[i+2] = strPathArray.get(i);
        }

        if (costBase <= sumCostAllPaths){
            //System.out.println("BASE");
            return resultBase;
        }else{
            return resultTemp;
        }


        //return result;

        //return new String[]{"17","\r\n","0 8 0 20"};
    }

    //Question: How to save paths?

    //Function: Removing Isolated Node Recursively to Simplify Network Topology
    public static void removeIsolatedNode(ArrayList<AdjacentTableNodeInfo> adjacentTable){
        while(true){
            //Flag of Controlling the end of recursive operation
            boolean flag = true;
            for(AdjacentTableNodeInfo a : adjacentTable){
                //Calculating degrees of all network nodes
                a.degree = a.adjacentNetworkLinkArray.size();
                //Processing IsolateNode
                if (1 == a.degree && !a.isConnectedConsumptionNode){
                    flag = false;
                    a.isIsolatedNode = true;
                    a.sumAllLinkBandWidth = 0;
                    //Modifying Adjacent Table
                    int beginNetworkNodeIDTemp = a.adjacentNetworkLinkArray.get(0).endNetworkNodeID;
                    int endNetworkNodeIDTemp = a.adjacentNetworkLinkArray.get(0).beginNetworkNodeID;
                    NetworkLinkInfo nIsolated = a.adjacentNetworkLinkArray.remove(0);
                    AdjacentTableNodeInfo aTemp = adjacentTable.get(beginNetworkNodeIDTemp);
                    Integer i = endNetworkNodeIDTemp;
                    //aTemp.adjacentNetworkNodeArray.remove(i);
                    Iterator<NetworkLinkInfo> iteratorNLI = aTemp.adjacentNetworkLinkArray.iterator();
                    while(iteratorNLI.hasNext()){
                        NetworkLinkInfo n = iteratorNLI.next();
                        if (n.endNetworkNodeID == endNetworkNodeIDTemp){
                            //Removing IsolateNode NetworkLink
                            iteratorNLI.remove();//Avoiding Exception: java.util.ConcurrentModificationException
                            //Update Operation
                            aTemp.degree --;
                            aTemp.sumAllLinkBandWidth -= n.sumLinkBandWidth;

                        }
                    }
                }
                //System.out.println(a.toString());
            }
            if(flag){
                break;//Terminating recursive operation
            }
        }
    }

    public static int[] setVideoServer(ArrayList<AdjacentTableNodeInfo> adjacentTable){
        //int videoNum = sumConsumptionNodes/5;
        int[] videoServerNodeID = new int[sumVideoServerBase];
        for (int i=0;i<sumVideoServerBase;i++){
            ArrayList<SumAllLink> sumAllLinks = getSumAllLinkArrayFromAdjacentTable(adjacentTable);
            SumAllLink sumAllLinkMax = sumAllLinks.get(0);
            videoServerNodeID[i] = sumAllLinkMax.networkNodeID;
            AdjacentTableNodeInfo aMax = adjacentTable.get(sumAllLinkMax.networkNodeID);
            Iterator<NetworkLinkInfo> iteratorMax = aMax.adjacentNetworkLinkArray.iterator();
            while(iteratorMax.hasNext()){
                //for (NetworkLinkInfo n : aMax.adjacentNetworkLinkArray){
                NetworkLinkInfo n = iteratorMax.next();
                AdjacentTableNodeInfo aTEnd = adjacentTable.get(n.endNetworkNodeID);
                Iterator<NetworkLinkInfo> iterator = aTEnd.adjacentNetworkLinkArray.iterator();
                while(iterator.hasNext()){
                    NetworkLinkInfo nTemp = iterator.next();
                    if (nTemp.endNetworkNodeID == n.beginNetworkNodeID){
                        iterator.remove();
                        aTEnd.degree --;
                        aTEnd.sumAllLinkBandWidth -= nTemp.sumLinkBandWidth;
                    }
                }
                iteratorMax.remove();
                aMax.degree--;
                aMax.sumAllLinkBandWidth -= n.sumLinkBandWidth;
            }
        }
        return videoServerNodeID;

    }
    public static ArrayList<SumAllLink> getSumAllLinkArrayFromAdjacentTable(ArrayList<AdjacentTableNodeInfo> adjacentTable){
        ArrayList<SumAllLink> sumAllLinks = new ArrayList<SumAllLink>();
        for (AdjacentTableNodeInfo a : adjacentTable){
            SumAllLink s = new SumAllLink(a.networkNodeID, a.degree, a.sumAllLinkBandWidth);
            sumAllLinks.add(s);
        }
        Collections.sort(sumAllLinks);
        return sumAllLinks;
    }

//    public static void floyd(int[][] path, int[][] dist,ShortestPathUnit[][] shortestPathUnits) {
//
//        // 初始化
//        for (int i = 0; i < sumNetworkNodes; i++) {
//            for (int j = 0; j < sumNetworkNodes; j++) {
//                dist[i][j] = adjacentMatrixPerLeaseCharge[i][j];  // "顶点i"到"顶点j"的路径长度为"i到j的权值"。
//                path[i][j] = j;                // "顶点i"到"顶点j"的最短路径是经过顶点j。
//                shortestPathUnits[i][j] = new ShortestPathUnit();
//            }
//        }
//
//        // 计算最短路径
//        for (int k = 0; k < sumNetworkNodes; k++) {
//            for (int i = 0; i < sumNetworkNodes; i++) {
//                for (int j = 0; j < sumNetworkNodes; j++) {
//
//                    // 如果经过下标为k顶点路径比原两点间路径更短，则更新dist[i][j]和path[i][j]
//                    int tmp = (dist[i][k]==INF || dist[k][j]==INF) ? INF : (dist[i][k] + dist[k][j]);
//                    if (dist[i][j] > tmp) {
//                        // "i到j最短路径"对应的值设，为更小的一个(即经过k)
//                        dist[i][j] = tmp;
//                        // "i到j最短路径"对应的路径，经过k
//                        path[i][j] = path[i][k];
//                        shortestPathUnits[i][j].beginNetworkNodeID = i;
//                        shortestPathUnits[i][j].endNetworkNodeID = j;
//                        shortestPathUnits[i][j].stackNodes.push(k);
//
//                        //shortestPathUnits[i][j].stackPathUnion.push(k);
//                    }
//                }
//            }
//        }
//
//        // 打印floyd最短路径的结果
//        System.out.printf("floyd: \n");
//        for (int i = 0; i < sumNetworkNodes; i++) {
//            for (int j = 0; j < sumNetworkNodes; j++) {
//                System.out.printf("%2d  ", dist[i][j]);
//                //shortestPathUnits[i][j].stackNodes.toString();
//                System.out.print("[ ");
//                for (int k : shortestPathUnits[i][j].stackNodes){
//                    System.out.print(k + " ");
//                }
//                System.out.print("]");
//            }
//            System.out.printf("\n");
//        }
//    }

}

