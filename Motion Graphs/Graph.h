#ifndef _GRAPH
#define _GRAPH

#include "gNode.h"
#include "dMap.h"
#include <sstream>

class MotionGraph;

class Graph{
	public:
		Graph();
		Graph(MotionGraph *m);
		~Graph();

		void constructGraph(Ninja motions, int nMotions, float threshold, int nCoincidents);

		int addNode(gNode *node);
		//void addEdge(gNode);
		gNode* existNode(gNode *node);

		int getNNodes(){return this->nNodes;}
		gNode* getNode(int node);

	private:
		MotionGraph *motionGraph;
		int nNodes;
		std::vector<gNode> nodes;


		void splitAnimation(std::string name, int separation,
							int node1, int frame1,
							int node2, int frame2);
		void createTransition(std::string m1, int node1, int frame1,
							  std::string m2, int node2, int frame2,
							  int transiction,int range);

		int **indexes;
		
		void initIndexes(Ninja motions, int nMotions);
};


#endif