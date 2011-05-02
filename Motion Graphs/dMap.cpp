#include "dMap.h"

/*
 * Contructors and Destructors
 */
dMap::dMap(){

}

dMap::dMap(int nMotions){
	this->differenceMap = (float***)malloc(sizeof(float**) * nMotions * nMotions);
	this->relations = (Motion***)malloc(sizeof(Motion**) * nMotions * nMotions);
	this->maxRelations = nMotions * nMotions;
	this->nRelations = 0;
}



dMap::~dMap(){

}


/* 
 * Metodos
 */
void dMap::duplicateSpace(){
	int nRel = this->nRelations;
	int maxRel = this->maxRelations * 2;
	float ***difMap = (float***)malloc(sizeof(float**) * maxRel);
	Motion ***rel = (Motion***)malloc(sizeof(Motion**) * maxRel);

	for(int i = 0 ; i < nRel ; i++){
		rel[i] = (Motion**)malloc(sizeof(Motion*) * 2);
		rel[i][0] = this->relations[i][0];
		rel[i][1] = this->relations[i][1];
		
		float **map = (float**)malloc(sizeof(float*) * rel[i][0]->getNPointClouds());

		for(int j = 0 ; j < rel[i][0]->getNPointClouds() ; j++){
			map[j] = (float*)malloc(sizeof(float) * rel[i][1]->getNPointClouds());
		}

		for(int c = 0 ; c < rel[i][0]->getNPointClouds() ; c++){
			for(int l = 0 ; l < rel[i][1]->getNPointClouds() ; l++){
				map[l][c] = this->differenceMap[i][l][c];
			}
		}
	}
	
}


void dMap::constructMap(Motion **motions, int nMotions){
	for(int i = 0 ; i < nMotions -1 ; i++){
		for(int j = i + 1 ; j < nMotions; j++){
			if(i != j) this->compareMotions(motions[i], motions[j]);
		}
	}
}

/**
 * Comparação entre duas animações. Compara frame a frame o esqueleto presente em cada uma delas.
 */
void dMap::compareMotions(Motion *m1, Motion *m2){
	float **map;

	if(this->nRelations > (int)((float)this->maxRelations * 0.75)) this->duplicateSpace();

	this->relations[this->nRelations] = (Motion**)malloc(sizeof(Motion*) * 2);
	this->relations[this->nRelations][0] = m1;
	this->relations[this->nRelations][1] = m2;

	map = (float**)malloc(sizeof(float*) * m1->getNPointClouds());

	for(int i = 0 ; i < m1->getNPointClouds() ; i++){
		map[i] = (float*)malloc(sizeof(float) * m2->getNPointClouds());
	}
	
	for(int i = 0 ; i < m1->getNPointClouds() ; i++){
		for(int j = 0 ; j < m2->getNPointClouds() ; j++){
			//TODO compareFrames(i,j) == compareFrames(j,i) ?
			map[i][j] = this->compareFrames(m1->getPointCloud(i), m2->getPointCloud(j));
		}
	}

	this->differenceMap[this->nRelations] = map;

	this->nRelations++;
}

float dMap::compareFrames(PointCloud *s1, PointCloud *s2){
	//TODO numero de pontos entre PointClouds é sempre o mesmo?
	for(int i = 0 ; i < s1->getNPoints() ; i++){

	}
}