#ifndef _TARJAN_H_
#define _TARJAN_H_

#include "Graph.h"
#include <stack>


class Graph;

class Tarjan
{
public:
	Tarjan(Graph *graph);
	~Tarjan();

	void DFS(Graph *graph, int v);
	int numSCC(void);
	bool isConnected(int v, int w);
	int sccGroup(int v);

	void subGraph(Graph* sGraph);

private:
	bool *m_visited;		// indica se o nó já foi visitado
	int *m_sccGroupID;		// indica o grupo em que está o nodo[i]
	int *m_low;				// low[v] = low number of v
	int m_pre;              // pre-order number counter
	int m_numSCC;           // numero de grupos SCC
	std::stack<int>	m_stack;

	int m_numNodes;		// numero de nós do grafo de entrada
	int m_numEdges;		// numero de veertices do grafo de entrada

	Graph *m_graph;
};


#endif	// _TARJAN_H_