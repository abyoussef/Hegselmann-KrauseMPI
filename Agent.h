#ifndef AGENT_H
#define AGENT_H

#include <iostream>
#include <string>
#include <vector>

class Agent {

    public:
		Agent(int d); //constructeur1
		Agent(int d, double * attr);//constructeur2
		~Agent(); // Destructeur
        
        // Distance Euclidienne entre 2 donnees
        double distanceE(const Agent& agent) const;
        
        Agent(const Agent &X);//Constructeur de copie
        Agent &operator=(const Agent &X);
        
        int getD() const; // La dimension de l'agent 
        double * getAttribut() const; // Renvoie les attributs
        void setAttribut(int i, double a); //permet la modification du i_eme attribut
        
        

    private:
        int d;
        double * attributs;
};

std::ostream& operator<<(std::ostream& os, const Agent& A);


double* barycentre(std::vector<Agent> &agents, std::vector<int> indexes);

void writeAgents(std::vector<Agent> &agents, const char* myFileName);

void writeAgents(std::vector<Agent> &agents, std::string myFileName);
#endif // AGENT_H
