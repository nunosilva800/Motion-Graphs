#include "stdafx.h"
#include "Graph.h"

/*
 * Contructors and Destructors
 */

Graph::Graph(){
	this->entity = NULL;
	this->indexes = NULL;
	this->nNodes = 0;
}

Graph::Graph(Ogre::Entity *e){
	this->entity = e;
	this->indexes = NULL;
	this->nNodes = 0;
}

Graph::~Graph(){

}

/*
 * Methods
 */

/**
 * Adiciona o novo nodo ao grafo e atribui-lhe um ID (incremental)
 */
int Graph::addNode(gNode *node){
	node->setID(this->nNodes);
	this->nodes.push_back(node);
	this->nNodes++;
	return node->getID();
}

/**
 * Verifica se aquele nodo já está introduzido no grafo
 * @return NULL caso não exista ou retorna o próprio nodo
 *
gNode* Graph::existNode(gNode *node){
	for(int i = 0 ; i < this->nNodes ; i++){
		if(node->getID() == this->nodes[i].getID()) return this->getNode(i);
	}
	
	return NULL;
}

/**
 * retorna o nodo na posicao pretendida ou nulo caso essa posicao nao exista
 */
gNode *Graph::getNode(int node){
	if(node >= this->nNodes || node < 0) return NULL;
	else return this->nodes.at(node);
}


void Graph::constructGraph(Ninja motions, int nMotions, float threshold, int nCoincidents){
	if(nMotions == 0) return;

	this->initIndexes(motions,nMotions);
	
	sNinja::iterator it;
	int i,j,nRelations = 0;

	for(it = motions->begin() , i = 0 ; it != motions->end() ; it++, i++){
		gNode *n1 = new gNode();
		
		gNode *n2 = new gNode();
		
		Edge *e = new Edge(n2,it->first);
		n1->addEdge(e);

		int lastPos = it->second->getNPointClouds()-1;
		this->indexes[i][0] = this->addNode(n1);
		this->indexes[i][lastPos] = this->addNode(n2);
	}
	
	this->map = new dMap(nMotions);
	this->map->setNSteps(nCoincidents);
	this->map->setThreshold(threshold);

	this->map->constructMap(motions, nMotions);

	for(i = 0 ; i < this->map->getNRelations() ; i++){
		std::string m1 = *this->map->relations[i][0];
		std::string m2 = *this->map->relations[i][1];

		int index1 = -1, index2 = -1;

		for(it = motions->begin(), j = 0 ; it != motions->end() ; it++, j++){
			if(it->first.compare(m1) == 0) index1 = j;
			if(it->first.compare(m2) == 0) index2 = j;
		}

		if(index1 != -1 && index2 != -1){

			std::vector<int> min1,min2;

			nRelations = this->map->getMinimuns(i,&min1,&min2);

			for(j = 0 ; j < nRelations ; j++){
				if(this->indexes[i][min1[j]] < 0 || this->indexes[i][min1[j]] > this->nNodes){
					gNode *n = new gNode();
					this->indexes[i][min1[j]] = this->addNode(n);
				}

				if(this->indexes[i][min2[j]] < 0 || this->indexes[i][min2[j]] > this->nNodes){
					gNode *n = new gNode();
					this->indexes[i][min2[j]] = this->addNode(n);
				}

				this->createTransition(m1,this->indexes[i][min1[j]],min1[j],m2,this->indexes[i][min2[j]],min2[j],
										j,this->map->getNSteps(),motions->at(m1)->getNPointClouds(),motions->at(m2)->getNPointClouds());
			}
		}
	}
	
	int k;
	FILE *f;
	if((f = fopen("indexes.txt","w")) == NULL) return;
	
	for(it = motions->begin(), i = 0 ; it != motions->end() ; it++, i++){
		int sep = 1;
		for(j = 0 ; j < it->second->getNPointClouds() ; j++){
			fprintf(f,"%d, ",this->indexes[i][j]);
			if(this->indexes[i][j] != -1){
				bool done = false;
				for(k = j + 1 ; k < it->second->getNPointClouds() && !done ; k++){
					if(this->indexes[i][k] != -1){
						this->splitAnimation(it->first,sep,this->indexes[i][j],j,this->indexes[i][k],k,it->second->getNPointClouds());
						sep++;
						done = true;
					}
				}
			}
		}
		fprintf(f,"\n");
	}
	fclose(f);
}


void Graph::createAnimation(std::string newName, std::string originaAnimation,
							int frame1, int frame2, int totalFrames){
	Ogre::Animation *animation = this->entity->getSkeleton()->getAnimation(originaAnimation);

	
    float length = animation->getLength();
    float tInit = ((float)((float)frame1 * length)) / ((float)totalFrames);
    float tEnd = ((float)((float)frame2 * length)) / ((float)totalFrames);
    float newDuration = tEnd - tInit;
	float timeStep = newDuration / ((float)(frame2 - frame1));
	

    Ogre::Animation *newAnimation = this->entity->getSkeleton()->createAnimation(newName,newDuration);

	Ogre::Animation::NodeTrackIterator trackIterator = animation->getNodeTrackIterator();

	while(trackIterator.hasMoreElements()){ 
		Ogre::NodeAnimationTrack* track = trackIterator.getNext();

		Ogre::NodeAnimationTrack* newTrack = newAnimation->createNodeTrack(track->getHandle(), track->getAssociatedNode());

		short numFrames = frame2 - frame1;

		for(float keyFrameTime = 0.0; keyFrameTime < newDuration; keyFrameTime += timeStep){
			Ogre::TransformKeyFrame* transformedKFrame = track->createNodeKeyFrame(keyFrameTime);
			
			track->getInterpolatedKeyFrame(keyFrameTime,transformedKFrame);

			Ogre::Real newTime = newDuration - (transformedKFrame->getTime() - tInit);

			Ogre::TransformKeyFrame* newKFrame = newTrack->createNodeKeyFrame(newTime);

			newKFrame->setScale(transformedKFrame->getScale());
			newKFrame->setRotation(transformedKFrame->getRotation());
			newKFrame->setTranslate(transformedKFrame->getTranslate());

		}
    }
}

/**
 * Frame1 < Frame2
 */
void Graph::splitAnimation(std::string name, int separation,
							int node1, int frame1,
							int node2, int frame2, int totalFrames){

    std::stringstream ss;
    ss << name << "_" << separation;
    std::string label = ss.str();

    this->getNode(node1)->addEdge(new Edge(this->getNode(node2),label));

	this->createAnimation(label,name,frame1,frame2,totalFrames);

}


/*
		e1					   e3		e4
 1-------------->2			1----->5------->2
					---->		    \ e7
								     \
 3-------------->4			3-------->6---->4
		e2						e5		e6
*/
void Graph::createTransition(std::string m1, int node1, int frame1,
							 std::string m2, int node2, int frame2,
							 int transiction,int range, int totalFrames1, int totalFrames2){

    std::stringstream ss;
    ss << m1 << "_" ;
    ss << m2 << "_" << transiction;
    std::string newName = ss.str();

    this->getNode(node1)->addEdge(new Edge(this->getNode(node2),newName));
    
	this->createAnimation("aux_blend_animation_1",m1,frame1,frame1 + range,totalFrames1);
	this->createAnimation("aux_blend_animation_2",m2,frame2 - range,frame2,totalFrames2);
		

	Ogre::Animation *animation1 = this->entity->getSkeleton()->getAnimation("aux_blend_animation_1");
	Ogre::Animation *animation2 = this->entity->getSkeleton()->getAnimation("aux_blend_animation_2");

	float length = (animation1->getLength() > animation2->getLength()) ? animation2->getLength() : animation1->getLength();
	float timeStep = length / ((float)(range));	

    Ogre::Animation *newAnimation = this->entity->getSkeleton()->createAnimation(newName,length);

	Ogre::Animation::NodeTrackIterator trackIterator1 = animation1->getNodeTrackIterator();
	Ogre::Animation::NodeTrackIterator trackIterator2 = animation2->getNodeTrackIterator();

	while(trackIterator1.hasMoreElements() && trackIterator2.hasMoreElements()){ 
		Ogre::NodeAnimationTrack* track1 = trackIterator1.getNext();
		Ogre::NodeAnimationTrack* track2 = trackIterator2.getNext();

		//DUBIDAS Handle e AssociatedNode de quem? 1 ou 2?
		Ogre::NodeAnimationTrack* newTrack = newAnimation->createNodeTrack(track1->getHandle(), track1->getAssociatedNode());
		float weight = 1.0;
		int i = 0;
		for(float keyFrameTime = 0.0; keyFrameTime < length; keyFrameTime += timeStep , weight -= (float)(1.0/(float)range)){
			Ogre::TransformKeyFrame* keyFrame1 = track1->createNodeKeyFrame(keyFrameTime);
			Ogre::TransformKeyFrame* keyFrame2 = track2->createNodeKeyFrame(keyFrameTime);
			
			track1->getInterpolatedKeyFrame(keyFrameTime,keyFrame1);
			track2->getInterpolatedKeyFrame(keyFrameTime,keyFrame2);

				//Ogre::NodeAnimationTrack* auxTrack = newAnimation->createNodeTrack(track1->getHandle() + 1 , track1->getAssociatedNode());

				//Ogre::TransformKeyFrame* newKFrame1 = auxTrack->createNodeKeyFrame(0.0);
				//Ogre::TransformKeyFrame* newKFrame2 = auxTrack->createNodeKeyFrame(1.0);
			

				//newKFrame1->setScale(keyFrame1->getScale());
				//newKFrame1->setRotation(keyFrame1->getRotation());
				//newKFrame1->setTranslate(keyFrame1->getTranslate());

				////FALTA APLICAR TRANSLACAO
				//newKFrame2->setScale(keyFrame2->getScale());
				//newKFrame2->setRotation(keyFrame2->getRotation());
				//newKFrame2->setTranslate(keyFrame2->getTranslate());

			Ogre::Real newTime = length - keyFrameTime;

				//Ogre::TransformKeyFrame* keyFrameInt = track1->createNodeKeyFrame(newTime);
				//auxTrack->getInterpolatedKeyFrame(weight,keyFrameInt);

			Ogre::TransformKeyFrame* newKFrame = newTrack->createNodeKeyFrame(newTime);

			newKFrame->setScale(keyFrame1->getScale());
			
				PointCloud *p1 = this->map->motions->at(m1)->getPointCloud(frame1);
				PointCloud *p2 = this->map->motions->at(m1)->getPointCloud(frame1);
				float x = 0 , y = 0 , teta = 0;
				this->map->calculateTransformation(p1,p2,&teta,&x,&y);
				
				float alpha = 2.0 * powf(((float)(i+1)/(float)(frame2-frame1)),3) - 3.0* powf(((float)(i+1)/(float)(frame2-frame1)),2) + 1.0;
				i++;
				Ogre::Quaternion rotation1 = keyFrame1->getRotation();
				Ogre::Quaternion rotation2 = keyFrame2->getRotation();
				Ogre::Quaternion rotation = Ogre::Quaternion::Slerp(alpha,rotation1,rotation2);
				Ogre::Vector3 translation = keyFrame1->getTranslate();

				translation.x += x;
				translation.y += y;


			newKFrame->setRotation(rotation);
			
			newKFrame->setTranslate(translation);

				//track1->removeAllKeyFrames();
				//newAnimation->destroyNodeTrack(track1->getHandle() + 1);
		}
    }



	this->entity->getSkeleton()->removeAnimation("aux_blend_animation_1");
	this->entity->getSkeleton()->removeAnimation("aux_blend_animation_2");
	

    //get animations to start blending
    //Ogre::Animation *animation1_ptr = this->entity->getSkeleton()->getAnimation(m1);
    //Ogre::Animation *animation2_ptr = this->entity->getSkeleton()->getAnimation(m2);

    ////apply 2D transformation to second motion (is this the best way to do it?)
    ////no... but its the only one we got (NOT DONE)
    //Ogre::Animation * animation2_trans2d = animation2_ptr->clone(m2);
    //
    //for(int i=0;i < animation2_ptr->getNumNodeTracks(); i++)
    //{
    //    for(int j = 0; j <= animation2_ptr->getNodeTrack(i)->getNumKeyFrames(); j++)
    //    {
    //        //get point clouds from original animation in the key frame
    //        PointCloud * p1 = new PointCloud();
    //        PointCloud * p2 = new PointCloud();

    //        //TODO: animation2_ptr->getNodeTrack(i)->getnode
    //    }

    //}

    ////create a new raw animation
    //Ogre::Animation *newAnimation = animation1_ptr->clone(newName); //use m1 or m2 i guess
    //newAnimation->destroyAllTracks();

    ////blend anim1 with anim2_trans2d and output the result to newAnimation
    ////should we be careful about the animation times ?

    //for(int i=0;i < animation1_ptr->getNumNodeTracks(); i++)
    //{
    //    newAnimation->createNodeTrack(i,animation1_ptr->getNodeTrack(i)->getAssociatedNode());

    //    //iterate through each keyframe in animation1 and blend it with animation2
    //    for(int j = frame1, aux = 0; j <= frame2; j++, aux++)
    //    {
    //        //create new nodetrack in new animation
    //        animation1_ptr->getNodeTrack(i)->createNodeKeyFrame(
    //            animation1_ptr->getNodeTrack(i)->getNodeKeyFrame(j)->getTime() - 
    //            animation1_ptr->getNodeTrack(i)->getNodeKeyFrame(frame1)->getTime());

    //        //get rotations and slerp them to new animatons
    //        Ogre::Quaternion quat1 = animation1_ptr->getNodeTrack(i)->getNodeKeyFrame(j)->getRotation();
    //        Ogre::Quaternion quat2 = animation2_trans2d->getNodeTrack(i)->getNodeKeyFrame(j)->getRotation();

    //        float alpha = 2 * powf(((i+1)/(frame2-frame1)),3) - 3* powf(((i+1)/(frame2-frame1)),2) + 1;
    //        newAnimation->getNodeTrack(i)->getNodeKeyFrame(aux)->setRotation(Ogre::Quaternion::Slerp(alpha,quat1,quat2)); //what is this first value? weight? alpha?

    //        //get root position and interpolate it linearly
    //        float ix,iy,iz; // where is root ?!?! need to find it...
    //        int root = 0;

    //        Ogre::Vector3 vec1 = animation1_ptr->getNodeTrack(root)->createNodeKeyFrame(j)->getTranslate();
    //        Ogre::Vector3 vec2 = animation2_trans2d->getNodeTrack(root)->createNodeKeyFrame(j)->getTranslate();

    //        ix = vec1.x + (j/(frame2-frame1)) *(vec2.x - vec1.x);
    //        iy = vec1.y + (j/(frame2-frame1)) *(vec2.y - vec1.y);
    //        iz = vec1.z + (j/(frame2-frame1)) *(vec2.z - vec1.z);

    //        newAnimation->getNodeTrack(root)->getNodeKeyFrame(j)->setTranslate(Ogre::Vector3(ix,iy,iz));

    //    }
    //}


    ////TODO criar animação com o newName entre frame1 de m1 e frame2 de m2
    ///*
    //true,this->motionGraph->getMotion(
    //&motions->at(map.relations[i][0]),&motions->at(map.relations[i][0]),
    //pts1[j],pts2[j]-nCoincidents,pts1[j] + nCoincidents,pts2[j])));
    //*/

}


void Graph::initIndexes(Ninja motions, int nMotions){
	int maxFrames = -1;

	sNinja::iterator it;

	for(it = motions->begin() ; it != motions->end() ; it++){
		if(it->second->getNPointClouds() > maxFrames)
			maxFrames = it->second->getNPointClouds();
	}
	maxFrames *= 1.5;
	this->indexes = (int**)malloc(sizeof(int*) * nMotions);
	
	for(int i = 0 ; i < nMotions ; i++){
		this->indexes[i] = (int*)malloc(sizeof(int) * maxFrames);
	}

	for(int i = 0 ; i < nMotions ; i++){
		for(int j = 0 ; j < maxFrames ; j++){
			this->indexes[i][j] = -1;
		}
	}
}

void Graph::printGraph(char *path){
	FILE *fp = NULL;
	FILE *fp2 = NULL;
	Edge* eAux;
	gNode* gAux;
	if((fp = fopen(path,"w")) == NULL) return ;
	if((fp2 = fopen("cenas.txt","w")) == NULL) return ;

 	fprintf(fp,"digraph G {\n");
	fprintf(fp,"subgraph cluster0{\n");
	fprintf(fp,"style=filled;\n");
	fprintf(fp,"color= red;\n");
	fprintf(fp,"node [ fillcolor =white color = black  style=filled  shape = hexagon] ; \n");
	int ac = 0;
	for(int i = 0 ; i < this->nNodes ; i++){
		ac += this->getNode(i)->getNEdges();
	}

	fprintf(fp2,"%d\n%d\n",this->nNodes,ac);

	for(int i = 0 ; i < this->nNodes ; i++){
		gAux = this->getNode(i);
		if(gAux){
			for(int j = 0 ; j < gAux->getNEdges() ; j++){
				eAux = gAux->getEdge(j);
				if(eAux){
					int id = gAux->getID();
					int idDest = 0;
					gNode *gDest = eAux->getDestionation();
					std::string anim = eAux->getLabel();
					const char *name = NULL;
					
					if(id < 0){
						id = -101010101;
					}
					if(!gDest){
						idDest = -101010101;
					}
					else{
						try{
						idDest = gDest->getID();
						}
						catch (exception &e){
							idDest = -1;
						}
						if(idDest < 0){
							idDest = -101010101;
						}
					}
					if(anim.size() > 0){
						name = anim.data();
					}
					else name = "NONAMENONAMENONAMENONAME";
					fprintf(fp2,"%d %d\n",gAux->getID(),gDest->getID());
					fprintf(fp,"\t\"%d\" -> \"%d\" [label = \"%s\"];\n",id,idDest,name);
																		//gAux->getID(),
																		//(eAux->getDestionation()) ? eAux->getDestionation()->getID() : -10101010101,
																		//eAux->getLabel().data());
																		//this->nodes[i].getID(),
																		//this->nodes[i].getEdge(j)->getDestionation()->getID(),
																		//this->nodes[i].getEdge(j)->getLabel().data());
				}
			}
		}
	}
	fprintf(fp,"}}\n");

	fclose(fp);
	fclose(fp2);
}



bool Graph::removeNode(int i)
{
	nodes.erase( nodes.begin() + i);
	nNodes--;

	return true;
}

bool Graph::changeNode(int i, gNode *node)
{
	nodes[i] = node;

	return true;
}