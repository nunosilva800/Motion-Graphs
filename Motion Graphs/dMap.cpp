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


int dMap::getMinimuns(int level, std::vector<int> m1, std::vector<int> m2){
	int nPts = 0;

	if(level >= this->nRelations) return 0;

	for(int i = 0 ; i < this->relations[level][0]->getNPointClouds() ; i++){
		for(int j = 0 ; j < this->relations[level][1]->getNPointClouds() ; j++){
			if(this->differenceMap[level][i][j] < this->threshold){
				if((this->relations[level][0]->getNPointClouds() - i) <= this->nSteps &&
				   (this->relations[level][1]->getNPointClouds() - j) <= this->nSteps ){
					   bool ok = true;
					   for(int l = 0 ; l < this->nSteps && ok ; l++){
							for(int c = 0 ; c < this->nSteps && ok; c++){
								if(this->differenceMap[level][l+i][c+j] >= this->threshold) ok = false;
							}
					   }

					   if(ok){
						   m1.push_back(i);
						   m2.push_back(j);
						   nPts++;
					   }
				}
			}
		}
	}

	return nPts;
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
	float x0 = 0,z0 = 0,teta = 0;
	double error = 0;

	this->calculateTransformation(s1,s2,&teta,&x0,&z0);

	s2->translate(x0,0,z0);
	s2->rotate(1,teta);
	
	for(int i = 0 ; i < NPOINTS ; i++){
		error += s1->getPoint(i)->getWeight() * pow((double)(this->difference(s1->getPoint(i),s2->getPoint(i))),2);
	}

	return (float)error;
}

void dMap::calculateTransformation(PointCloud *s1, PointCloud *s2, float *teta, float *x0, float *z0){
	float x1_ = 0, x2_ = 0, z1_ = 0, z2_ = 0, weights = 0;
	int i = 0;
	float p1 = 0, p2 = 0, p3 = 0, p4 = 0;

	this->calculateSums(s1,s2,&x1_,&x2_,&z1_,&z2_,&weights);

	p1 = this->calculateTeta1(s1,s2);
	p2 = this->calculateTeta2(x1_,x2_,z1_,z2_,weights);
	p3 = this->calculateTeta3(s1,s2);
	p4 = this->calculateTeta4(x1_,x2_,z1_,z2_,weights);

	*teta = atan( ((p1 - p2) / (p3 - p4)));
	
	*x0 = (float)((1.0/weights) * ( (x1_ - (x2_ * cos(*teta))) - (z2_ * sin(*teta)) ));

	*z0 = (float)((1.0/weights) * ( (z1_ + (x2_ * sin(*teta))) - (z2_ * cos(*teta)) ));
}


void dMap::calculateSums(PointCloud *s1, PointCloud *s2, float *x1_, float *x2_, float *z1_, float *z2_, float *weights){
	int i;

	for(i = 0 ; i < NPOINTS ; i++){
		*x1_ += s1->getPoint(i)->getWeight() * s1->getPoint(i)->getX();
	}

	for(i = 0 ; i < NPOINTS ; i++){
		*z1_ += s1->getPoint(i)->getWeight() * s1->getPoint(i)->getZ();
	}

	for(i = 0 ; i < NPOINTS ; i++){
		*x2_ += s2->getPoint(i)->getWeight() * s2->getPoint(i)->getX();
	}

	for(i = 0 ; i < NPOINTS ; i++){
		*z2_ += s2->getPoint(i)->getWeight() * s2->getPoint(i)->getZ();
	}

	for(i = 0 ; i < NPOINTS ; i++){
		*weights += s1->getPoint(i)->getWeight();
	}
}


float dMap::calculateTeta1(PointCloud *s1, PointCloud *s2){
	float result = 0;

	for(int i = 0 ; i < NPOINTS ; i++){
		result += s1->getPoint(i)->getWeight() * (
													(s1->getPoint(i)->getX() * s2->getPoint(i)->getX()) +
													(s1->getPoint(i)->getZ() * s2->getPoint(i)->getZ())
												 );
	}

	return result;
}

float dMap::calculateTeta2(float x1_, float x2_, float z1_, float z2_, float weights){
	return (float)((1.0/weights) * ((x1_ * z2_) - (x2_ * z1_)));
}

float dMap::calculateTeta3(PointCloud *s1, PointCloud *s2){
	float result = 0;

	for(int i = 0 ; i < NPOINTS ; i++){
		result += s1->getPoint(i)->getWeight() * (
													(s1->getPoint(i)->getX() * s2->getPoint(i)->getZ()) -
													(s2->getPoint(i)->getX() * s1->getPoint(i)->getZ())
												 );
	}

	return result;
}

float dMap::calculateTeta4(float x1_, float x2_, float z1_, float z2_, float weights){
	return (float)((1.0/weights) * ((x1_ * x2_) - (z1_ * z2_)));
}
