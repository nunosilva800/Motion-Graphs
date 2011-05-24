#include "stdafx.h"
#include "Tarjan.h"
/*
 * Contructors and Destructors
 */

Tarjan::Tarjan(Graph *graph)
{
	m_pre = 0;
	m_numSCC = 0;

	m_graph = graph;
	m_numNodes = graph->getNNodes();

	m_visited = new bool[graph->getNNodes()];
	m_sccGroupID = new int[graph->getNNodes()];
	m_low = new int[graph->getNNodes()];

	for (int i = 0; i < graph->getNNodes(); i++) 
	{	m_visited[i] = false;
		m_sccGroupID[i] = 0;
		m_low[i] = 0;
	}

	for (int i = 0; i < graph->getNNodes(); i++) 
	{	if (!m_visited[i]) DFS(graph, i);
	}
}

Tarjan::~Tarjan(){

}

/*
 * Methods
 */

void Tarjan::DFS(Graph *graph, int v)
{
	m_visited[v] = true;
	m_low[v] = m_pre++;
	int min = m_low[v];

	// Coloca na pilha
	m_stack.push(v);

	// Pesquisa no grafo, percorre os vertices do nó v
	int auxID;
	for (int n = 0; n < graph->getNode(v)->getNEdges(); n++)
	{
		auxID = graph->getNode(v)->getEdge(n)->getDestionation()->getID();
		if (!m_visited[auxID]) DFS(graph, auxID);

		// Verifica se já foi visitado
		if (m_low[auxID] < min) min = m_low[auxID];
	}

	// já está na pilha e no mesmo grupo SCC
	if (min < m_low[v])
	{	m_low[v] = min; 
		return; 
	}

	// retira os elementos da pilha e adiciona-os a um grupo SCC
	int n = 0;
	do {
		n = m_stack.top(); 
		m_stack.pop();
		m_sccGroupID[n] = m_numSCC;
		m_low[n] = graph->getNNodes();
	} while (n != v);

	m_numSCC++;
}

int Tarjan::numSCC(void)
{
	return m_numSCC;
}

bool Tarjan::isConnected(int v, int w)
{
	return (m_sccGroupID[v] == m_sccGroupID[w]);
}

int Tarjan::sccGroup(int v)
{
	return m_sccGroupID[v];
}

void removeGraphNode(int index, Graph *gh)
{
	// Pesquisa referencias (vertices) que apontam para index e remove
	gNode *aux;
	for (int k=0; k<gh->getNNodes(); k++)
	{
		for (int j=0; j<gh->getNode(k)->getNEdges(); j++)
		{
			if (gh->getNode(k)->getEdge(j)->getDestionation()->getID() == index)
			{
				aux = gh->getNode(k);
				aux->removeEdge(j);
				gh->changeNode(k, aux);
			}
		}
	}

}

void Tarjan::subGraph(Graph* sGraph)
{
	// Calcula o numero de elementos de cada SCC
	int *nSCC = new int[this->numSCC()];
	for( int i=0; i<this->numSCC(); i++)
		nSCC[i] = 0;

	for( int i=0; i<m_numNodes; i++)
		nSCC[m_sccGroupID[i]]++;


	// Escolhe o maior grupo SCC
	int index = 0;
	int val = 0;
	for( int i=0; i<this->numSCC(); i++)
	{	
		if(val < nSCC[i])
		{	val = nSCC[i];
			index = i;
		}
	}

	cout << "\nMaior SCC group: " << index << " Total: " << val << "\n\n";

	Graph *graph = new Graph();	
	gNode aux;

		// Copia apenas os nós do maior grupo SCC
		for (int i = 0; i < m_numNodes; i++) 
		{	
			aux.setID( i );
			if (m_sccGroupID[i] == index)
				graph->addNode( m_graph->getNode(i) );
			else
				graph->addNode( &aux );
		}

		// Remove os death end's (referencias)
		for (int i = m_numNodes-1; i >= 0; i--) 
		{
			if(graph->getNode(i)->getNEdges() == 0)
			{
				removeGraphNode(i, graph);		
			}
		}

		*sGraph = *graph;
}