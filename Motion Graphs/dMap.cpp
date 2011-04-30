#include "dMap.h"

/*
 * Contructors and Destructors
 */
dMap::dMap(){

}

dMap::dMap(int nMotions){
	this->relations = (Motion**)malloc(sizeof(Motion*) * nMotions * nMotions);
	this->nRelations = nMotions * (nMotions - 1);
}

dMap::~dMap(){

}


/* 
 * Metodos
 */
void dMap::duplicateSpace(){

}


void dMap::constructMap(Motion **motions, int nMotion){

}

/**
 * Comparação entre duas animações. Compara frame a frame o esqueleto presente em cada uma delas.
 */
float **dMap::compareMotions(Motion *m1, Motion m2){

}

float dMap::compareFrames(PointCloud *s1, PointCloud *s2){

}