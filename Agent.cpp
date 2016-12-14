#include <cstdlib>
#include <cmath>
#include "Agent.h"
#include <string>
#include <fstream> // bibliotheque permettant lecture et ecriture dans des fichiers (file input/ouput)
#include <iomanip>      // std::setw

using namespace std;

Agent::Agent(int d) {
	this->d= d;
	this->attributs = new double[d];
	for(int i = 0; i < d; i++)
	        this->attributs[i] = 0; //par defaut initialisation a zero
};

Agent::Agent(int d, double *attr){
	this->d = d;
	this->attributs = new double[d];
	for(int i = 0; i < d; i++)
		this->attributs[i] = attr[i];
}

//Destructeur
Agent::~Agent(){
	if(this->attributs != NULL)
		delete [] this->attributs;
}

//Constructeur de copie
Agent::Agent(const Agent &X){
    this->d = X.getD();
    this->attributs = new double[this->d];
    double *attr = X.getAttribut();
    for(int i = 0; i < this->d; i++)
        this->attributs[i] = attr[i];
};

//surcharge de l'operateur =
Agent& Agent::operator=(const Agent &X){
    if (&X != this){
        this->d = X.getD();
        if(this->attributs != NULL)
            delete[] this->attributs;
        this->attributs = new double [this->d];
        double *attr = X.getAttribut();
        for(int i = 0; i < this->d; i++)
            this->attributs[i] = attr[i];
    }
    return *this;
};


//distance euclidienne
double Agent::distanceE(const Agent& agent) const{
    if(agent.getD() == this->d){
        double dist = 0;
        double *attr = agent.getAttribut();
        for(int i = 0; i < this->d; i++)
            dist += (this->attributs[i] - attr[i])*(this->attributs[i] - attr[i]);
        return sqrt(dist);
    }
    else{
        std::cout << " ERROR (Agent.cpp) : data do not have the same dimensions ! " <<std::endl;
        exit(0);
    }
};

//retour de la dimension
int Agent::getD() const {
    return this->d;
};



double * Agent::getAttribut() const {
    return this->attributs;
}

// place a sur le i_eme attribut
void Agent::setAttribut(int i, double a){
    if(i < this->d && i >= 0)
        (this->attributs)[i] = a;
    else{
        std::cout<<"ERROR (Agent.cpp) : impossible d'assigner le tableau pour l'indice demande"<<std::endl;
        exit(0);
    }
};




ostream& operator<<(ostream& os, const Agent& A){
	double *attr = A.getAttribut();
	for(int i = 0; i < A.getD(); i++)
		os << attr[i] << '\t';
	os << std::endl;
	return os;
};

double* barycentre(std::vector<Agent> &agents, std::vector<int> indexes){
	int d = agents[0].getD();
	double* res = new double[d];
	for(int j = 0; j< d;++j){
		res[j] = 0;
		for(int i = 0; i < indexes.size(); ++i){
			res[j] += agents[indexes[i]].getAttribut()[j];
		}
		res[j] = res[j] /(double)indexes.size();
		
	}
	return res;
}

void writeAgents(std::vector<Agent> &agents, const char* myFileName){
	ofstream myFile;
	std::string ss = myFileName;
	myFile.open( ss );
	for(int i(0); i<agents.size() ; ++i){
		for(int j(0); j<agents[i].getD();++j){
			myFile<<setw(20)<<agents[i].getAttribut()[j];
		}
		myFile<<endl;
	}
	myFile.close();
}

void writeAgents(std::vector<Agent> &agents, string myFileName){
	ofstream myFile;

	myFile.open( myFileName );
	for(int i(0); i<agents.size() ; ++i){
		for(int j(0); j<agents[i].getD();++j){
			myFile<<setw(20)<<agents[i].getAttribut()[j];
		}
		myFile<<endl;
	}
	myFile.close();
}


