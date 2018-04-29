#include<bits/stdc++.h>
#include<stdio.h>
#include<fstream>
#include<iostream>
using namespace std;

#define NULL_VALUE -999999
#define INF 999999
#define MAX 1000
#define WHITE 1
#define GREY 2
#define BLACK 3

int x_i[MAX],y_i[MAX];
clock_t start,stop;


class Queue
{
    int queueInitSize ;
    int queueMaxSize;
    int * data;
    int length;
    int front;
    int rear;
public:
    Queue();
    ~Queue();
    void enqueue(int item); //insert item in the queue
    int dequeue(); //returns the item according to FIFO
    bool empty(); //return true if Queue is empty
};

Queue::Queue()
{
    queueInitSize = 2 ;
    queueMaxSize = queueInitSize;
    data = new int[queueMaxSize] ; //allocate initial memory
    length = 0 ;
    front = 0;
    rear = 0;
}


void Queue::enqueue(int item)
{
	if (length == queueMaxSize)
	{
		int * tempData ;
		//allocate new memory space for tempList
		queueMaxSize = 2 * queueMaxSize ;
		tempData = new int[queueMaxSize] ;
		int i, j;
		j = 0;
		for( i = rear; i < length ; i++ )
		{
			tempData[j++] = data[i] ; //copy items from rear
		}
		for( i = 0; i < rear ; i++ )
		{
			tempData[j++] = data[i] ; //copy items before rear
		}
		rear = 0 ;
		front = length ;
		delete[] data ; //free the memory allocated before
		data = tempData ; //make list to point to new memory
	}

	data[front] = item ; //store new item
	front = (front + 1) % queueMaxSize ;
	length++ ;
}


bool Queue::empty()
{
	if(length == 0) return true ;
	else return false ;
}


int Queue::dequeue()
{
	if(length == 0) return NULL_VALUE ;
	int item = data[rear] ;
	rear = (rear + 1) % queueMaxSize ;
	length-- ;
	return item ;
}


Queue::~Queue()
{
    if(data) delete[] data; //deallocate memory
    data = 0; //set to NULL
}

//****************Queue class ends here************************

//****************Dynamic ArrayList class based************************
class ArrayList
{
	double * list;
	int length ;
	int listMaxSize ;
	int listInitSize ;
public:
	ArrayList() ;
	~ArrayList() ;
	int searchItem(double item) ;
    void insertItem(double item) ;
	void removeItem(double item) ;
	void removeItemAt(int item);
	double getItem(int position) ;
	int getLength();
	bool empty();
	void printList();
} ;


ArrayList::ArrayList()
{
	listInitSize = 2 ;
	listMaxSize = listInitSize ;
	list = new double[listMaxSize] ;
	length = 0 ;
}

void ArrayList::insertItem(double newitem)
{
	double * tempList ;
	if (length == listMaxSize)
	{
		//allocate new memory space for tempList
		listMaxSize = 2 * listMaxSize ;
		tempList = new double[listMaxSize] ;
		int i;
        for( i = 0; i < length ; i++ )
        {
            tempList[i] = list[i] ; //copy all items from list to tempList
        }
        delete[] list ; //free the memory allocated before
        list = tempList ; //make list to point to new memory
	};

	list[length] = newitem ; //store new item
	length++ ;
}

int ArrayList::searchItem(double item)
{
	int i = 0;
	for (i = 0; i < length; i++)
	{
		if( list[i] == item ) return i;
	}
	return NULL_VALUE;
}

void ArrayList::removeItemAt(int position) //do not preserve order of items
{
	if ( position < 0 || position >= length ) return ; //nothing to remove
	list[position] = list[length-1] ;
	length-- ;
}


void ArrayList::removeItem(double item)
{
	int position;
	position = searchItem(item) ;
	if ( position == NULL_VALUE ) return ; //nothing to remove
	removeItemAt(position) ;
}


double ArrayList::getItem(int position)
{
	if(position < 0 || position >= length) return NULL_VALUE ;
	//cout << "Return : " << list[position] << endl;
	return list[position] ;
}

int ArrayList::getLength()
{
	return length ;
}

bool ArrayList::empty()
{
    if(length==0)return true;
    else return false;
}

void ArrayList::printList()
{
    int i;
    for(i=0;i<length;i++)
        printf("%d ", list[i]);
    printf("Current size: %d, current length: %d\n", listMaxSize, length);
}

ArrayList::~ArrayList()
{
    if(list) delete [] list;
    list = 0 ;
}

//******************ArrayList class ends here*************************



//******************Graph class starts here**************************
class Graph
{
	int nVertices, nEdges ;
	bool directed ;
	ArrayList  * adjList ;
	ArrayList *weight;
	int *parent;
	bool *visited;
	int *key;
	bool *MST;

public:
    double tsp_cost,walk_cost;
    vector<int> tsp_path;
	Graph(bool dir = false);
	~Graph();
	void setnVertices(int n);
	void addEdge(int u, int v);
	void removeEdge(int u, int v, int w);
	bool isEdge(int u, int v, int w);
    int getDegree(int u);
    void printAdjVertices(int u);
    bool hasCommonAdjacent(int u, int v);
    void printGraph();
	void Prim();
	void printMST();
	int minKey();
	bool isLeaf(int node);
    int leftChild(int node);
    int rightSibling(int node);
    void preorder(int root);
	//void printMST();
	void getTSPlength();
	void getWalklength();
	void TSP();
};


Graph::Graph(bool dir)
{
	nVertices = 0 ;
	nEdges = 0 ;
	adjList = 0 ;
	weight = 0;
	directed = dir ; //set direction of the graph
	visited = new bool[nVertices];
	parent = new int [nVertices];
	key = new int [nVertices];
	MST = new bool [nVertices];
}

void Graph::setnVertices(int n)
{
	this->nVertices = n ;
	this->tsp_cost = DBL_MAX;
	if(adjList!=0) delete[] adjList ; //delete previous list
	adjList = new ArrayList[nVertices] ;
	if(weight!=0) delete[] weight;
	weight = new ArrayList [nVertices];

	for(int i=0;i<nVertices;i++) visited[i] = false;
}

void Graph::addEdge(int u, int v)
{
    double c;
    if(u<0 || v<0 || u>=nVertices || v>=nVertices) return; //vertex out of range
    this->nEdges++ ;
	adjList[u].insertItem(v) ;
	//cout << "(" << x_i[u] << "," << y_i[u] <<") (" << x_i[v] << "," << y_i[v] << ")"<< endl;
	c = sqrt(pow(x_i[u]-x_i[v],2) + pow(y_i[u]-y_i[v],2));
	//cout << "c: " << c << endl;
	weight[u].insertItem(c);
	if(!directed){
        adjList[v].insertItem(u) ;
        weight[v].insertItem(c) ;
        //cout << weight[v].getItem(0) << " ";
	}
}

void Graph::removeEdge(int u, int v,int w)
{
    //write this function
    int pos;
    if(isEdge(u,v,w)==1)nEdges--;

    adjList[u].removeItem(v);
    pos=adjList[u].searchItem(v);
    weight[u].removeItemAt(pos);

    if(!directed){

        adjList[v].removeItem(u);
        weight[v].removeItemAt(pos);

    }
}

bool Graph::isEdge(int u, int v, int w)
{
    //returns true if (u,v) is an edge, otherwise should return false
    for(int i=0;i<adjList[u].getLength() ;i++){
        if(adjList[u].getItem(i)==v && weight[u].getItem(i)==w) return true;
    }
    return false;
}

int Graph::getDegree(int u)
{
    //returns the degree of vertex u
    return adjList[u].getLength();
}

void Graph::printAdjVertices(int u)
{
    //prints all adjacent vertices of a vertex u
    for(int i=0;i<adjList[u].getLength();i++){
        printf("%d ",adjList[u].getItem(i));
    }
    printf("\n");
}

bool Graph::hasCommonAdjacent(int u, int v)
{
    //returns true if vertices u and v have common adjacent vertices
    for(int i=0;i<adjList[i].getLength();i++){
        for(int j=0;j<adjList[j].getLength();j++){
            if(adjList[u].getItem(i)==adjList[v].getItem(j)) return true;
        }
    }
    return false;

}



void Graph::printGraph()
{
    printf("\nNumber of vertices: %d, Number of edges: %d\n", nVertices, nEdges);
    for(int i=0;i<nVertices;i++)
    {
        printf("%d th vertice: ", i);
        for(int j=0; j<adjList[i].getLength();j++)
        {
            //printf(" %d: %0.1f ", adjList[i].getItem(j),weight[i].getItem(j));
            cout << adjList[i].getItem(j) << ": " << weight[i].getItem(j) << " ";
        }
        printf("\n");
    }
}

Graph::~Graph()
{
    //write your destructor here
    if(adjList!=0) delete[] adjList;
    adjList = 0;
    if(weight!=0) delete[] weight;
    weight = 0;
    if(parent) delete[] parent;
    parent = 0;
    if(key) delete[] key;
    key = 0;
}



int Graph::minKey()
{
    int min = INT_MAX, min_index;

    for (int v = 0; v < nVertices; v++){
        if (MST[v] == false && key[v] < min){
            min = key[v];
            min_index = v;
        }
    }

   return min_index;
}


void Graph::printMST()
{
   printf("Edge   Weight\n");
   for (int i = 1; i < nVertices; i++)
      printf("%d - %d  \n", parent[i], i);
}

void Graph::Prim()
{

    for(int i=0;i<nVertices;i++){
        key[i] = INT_MAX;
        MST[i] = false;
    }

    key[0] = 0;
    parent[0] = -1;

    for(int i=0;i<nVertices-1;i++){
        int u = minKey();
        MST[u] = true;

        for(int j=0;j<weight[u].getLength();j++){
            int v = (int) adjList[u].getItem(j);

            if(MST[v]==false && weight[u].getItem(j)<key[v]){
                parent[v] = u;
                key[v] = weight[u].getItem(j);
            }
        }
    }
    //printMST();
}

bool Graph::isLeaf(int node)
{
    for(int i=0;i<nVertices;i++){
        if(parent[i]==node) return false;
    }

    return true;
}

int Graph::leftChild(int node)
{
    for(int i=0;i<nVertices;i++){
        if(parent[i]==node) return i;
    }

    return -2;
}

int Graph::rightSibling(int node)
{
    for(int i=node+1;i<nVertices;i++){
        if(parent[i]==parent[node]) return i;
    }

    return -2;
}

void Graph::preorder(int root)
{
    //for(int i=0;i<nVertices;i++) cout << parent[i] << " ";
    //cout << endl;

    tsp_path.push_back(root);

    if(!isLeaf(root)){
        int u = leftChild(root);

        while(u>0){
            preorder(u);
            //cout << u << endl;
            u = rightSibling(u);
            //cout << "right: " << u << endl;
        }
    }
}

void Graph::getTSPlength()
{
    tsp_cost = 0;

    for(int i=0;i<tsp_path.size()-1;i++){
        int u = tsp_path[i+1];
        int idx = adjList[tsp_path[i]].searchItem(u);
        tsp_cost+=weight[i].getItem(idx);
        //cout << tsp_path[i] <<" , "<< u << ":" << tsp_cost << endl;
    }
    int id = adjList[tsp_path[tsp_path.size()-1]].searchItem(0);
    tsp_cost+=weight[tsp_path[tsp_path.size()-1]].getItem(id);

    //cout << tsp_cost << endl;
}

void Graph::getWalklength()
{
    walk_cost = 0;

    for(int i=1;i<nVertices;i++){
        int u = parent[i];
        int idx = adjList[i].searchItem(u);
        walk_cost+=2*weight[i].getItem(idx);
        //cout << walk_cost << endl;
    }
}

void Graph::TSP()
{
    Prim();
    preorder(0);
    getTSPlength();
    getWalklength();
}


//**********************Graph class ends here******************************


//******main function to test your code*************************

int main(void)
{
    int n,x,y;
    int it=0;
    Graph g;
    ifstream input;
    input.open("1305073_input_8.txt");
    //input.open("input.txt");

    start = clock();
    input >> n;
    g.setnVertices(n);

    while(input >> x >> y){
        x_i[it] = x;
        y_i[it] = y;
        it++;
    }

    for(int i=0;i<n;i++){
        for(int j=i+1;j<n;j++){
            g.addEdge(i,j);
        }
    }

    while(1)
    {
        printf("1. TSP.    2.Prim  ");
        printf("3.Print Graph   4. Exit.\n");

        int ch;
        scanf("%d",&ch);
        if(ch==1)
        {
            //cout << start << endl;
            g.TSP();
            //cout << stop << endl;

            cout << "TSP Cost: " << g.tsp_cost << endl;
            cout << "Walk size: " << g.walk_cost << endl;
            cout << "TSP Path: ";

            for(int i=0;i<g.tsp_path.size();i++) cout << g.tsp_path[i] << "-> " ;
            cout << 0 << endl;

            stop = clock();
            double et = (stop - start);
            //cout << et << endl;
            //ofstream output;
            //output.open("output3.csv",std::ios_base::app | std::ios_base::out);
            //output << "Input size, Execution Time" << endl;
            //output << n << "," << et << endl;

        }
        else if(ch==2)
        {
            g.Prim();
        }
        else if(ch==3)
        {
            g.printGraph();
        }
        else if(ch==4)
        {
            break;
        }
    }

}



