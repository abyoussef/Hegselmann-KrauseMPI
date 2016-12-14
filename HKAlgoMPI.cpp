#include <iostream>
// bibliotheque permettant l'utilsation de fonctions input/output dans le terminal
#include <fstream>
// bibliotheque permettant lecture et ecriture dans des fichiers (file input/ouput)
#include <cmath>
// bibliotheque permettant utilisation de fonctions mathematiques (type sqrt)
#include <vector>
// bibliotheque permettant utilisation de la structure de vecteur
#include <cstdlib>
// bibliotheque permettant utilisation des generateurs de nombres aleatoires (type rand)
#include <sstream>
//bibliotheque dans laquelle se trouve des fonctions de conversion qu'on utilisera

#include "Agent.h"
#include "mpi.h"

using namespace std;




//initialiser la matrice is Nearest Neighbours N*N 
short** initNearestNeighbours(int N){
	short** isNN = new short*[N];
	for(int i = 0; i< N; ++i){
		isNN[i] = new short[N];
		for(int j = 0; j<N; ++j){
			isNN[i][j]=0;
		}
	}
	return isNN;
}


// initialiser la matrice des agents N * d
double ** initAgents(int N, int d){
	double ** agents = new double*[N];
	for(int i = 0; i<N; ++i){
		agents[i]= new double[d];
		for(int j = 0; j<d; ++j){
			agents[i][j]=0;
		}
	}
	return agents;
}
//delete la matrice is Nearest Neighbours N*N 
void deleteIsNN(short ** isNN, int N){
  for(int i=0; i<N; i++) {
	delete[] isNN[i];
  }
  delete[] isNN;
}

// delete la matrice des agents N * d
void deleteAgents(double ** agents, int N){
  for(int i=0; i<N; i++) {
	delete[] agents[i];
  }
  delete[] agents;
}


//Transformation d'un fichier de donnees vers un vecteur<Donnee>
std::vector<Agent> lireFichierAgents(const char* nomFichier){
  ifstream fichier(nomFichier);
  if(fichier.fail()){
    cerr << "Imposible d'ouvrir le fichier " << nomFichier << endl;
    exit(-1);
  }

  // Count the number of lines
  std::string line;
  std::getline(fichier, line);
  int dim = std::count(line.begin(), line.end(), ',');
  fichier.seekg(0);

  vector<Agent> agents;
  while(true){
    double* attributs = new double[dim];
    for(int i = 0; i < dim; i++){
      fichier >> attributs[i];
      fichier.ignore();
    }

    if(fichier.eof()){ break; }
    agents.push_back(Agent(dim,attributs));
  }

  if(agents.size() == 0){
    cerr << "Aucun agent charge" << endl;
    exit(-1);
  }

  return agents;
}


//L'algorithme HK sous MPI
int main(int argc, char *argv[]){

  if (argc != 4){
    std::cerr << "Usage: HKAlgoMPI.exe <fichier d'entree> <seuil> <nombre d'iteration>" << std::endl;
    exit(-1);
  }
  

  //Initialisation MPI
  MPI_Init(&argc, &argv);
  int p;
  int r;
  MPI_Comm_size(MPI_COMM_WORLD, &p);
  MPI_Comm_rank(MPI_COMM_WORLD, &r);
  
  int iteration = atoi(argv[3]);
  int k = atoi(argv[2]);
  srand(time(NULL));

  double t1,t2;
  t1 = clock();
  
  //lire les agents
  std::vector<Agent> agents = lireFichierAgents(argv[1]);
 
  if(r==0){
	  writeAgents(agents,"debut.dat");
  }
  
  
  std::vector< std::vector<Agent> > list;

  // N   le nombre d'agents
  int N = agents.size();
  // d la dimension des agents
  int d = agents[0].getD(); 
  
//  //write the matrix
//  if(r==0){
//	  for(int i = 0; i< N; ++i){  
//		  for(int j = 0; j<agents[i].getD();++j){
//			  cout<<agents[i].getAttribut()[j]<<" ";
//		  }
//		  cout<<endl;
//	  }
//  }
//  
  //fixation du seuil par rapport au diametre de X. seuil = diametre /k
  double seuil =0.0;
  if(r==0){
	  for(int i(0); i< N-1;++i){
		  for( int j(i+1); j< N; ++j){
			  if( agents[i].distanceE(agents[j]) > seuil ){
				  seuil = agents[i].distanceE(agents[j]);
			  }
		  }
	  }
	  seuil =(double) seuil / max(1,k);
  }

  //Broadcast la valeur du seuil depuis 0 vers toutes les autres machines
  MPI_Bcast(&seuil, 1, MPI::DOUBLE, 0, MPI_COMM_WORLD);
  

//commencer l'iteration des evolutions	  
  for(int l(0); l<iteration;++l){
	  //Stocke les nouveaux agents
	  double** newAgents_global = initAgents(N,d);
	  double** newAgents_local = initAgents(N,d);
	
	
	  // local, la solution locale de chaque processus
	  short** isNN_local = initNearestNeighbours(N) ;
	  short** isNN_global = initNearestNeighbours(N) ;
	  
	  int index_min = (int)floor(r*(N/(double)p));
	  float index_max = (r + 1)*(N/(double)p) - 1;
	  
	  //Partie 1 : Determination des voisins en parallele
	  for(int i = index_min; i <= index_max; i++){
		  isNN_local[i][i] = 1;
		  for( int j = index_min + 1; j<N; ++j){
			  if( agents[i].distanceE(agents[j]) <= seuil ){
				  isNN_local[i][j] = 1; isNN_local[j][i]=1;
			  }
		  }
	  }
	  //Partage des informations
	  for(int i = 0; i<N;++i){
		  MPI_Allreduce( isNN_local[i], isNN_global[i] , N , MPI::SHORT , MPI::SUM , MPI_COMM_WORLD);
	  }
	  

	  
	  //decomment to see the matrix isNN_global
	//  if(r==0){
	//	  for(int i = 0; i< N; ++i){  
	//		  for(int j = 0; j<N;++j){
	//			  cout<<isNN_global[i][j];
	//		  }
	//		  cout<<endl;
	//	  }
	//  }
	  
	  //delete isNN_local
	  deleteIsNN(isNN_local,N);
	  //------------------------------------------- Test fait jusqu'ici
	  
	  
//	  // test pour verifier la fonction barycentre de Agent.h et reussi
//	  vector<int> indexes;
//	  for(int i = 0; i< 5; ++i ){
//		  indexes.push_back(i);
//	  }
//	  Agent a = Agent(d, barycentre(agents,indexes));
	  
	  
	  //Partie 2 : calcul des barycentres en parallele
	  for(int i = index_min; i <= index_max; i++){
		  vector<int> indexes;
		  for(int j = 0; j<N;++j){
			  if(isNN_global[i][j] == 1){
				  indexes.push_back(j);
			  }
		  }
		  newAgents_local[i]= barycentre(agents,indexes); // Divergence a ce niveau pour certain processeur malgre le bon fonctionnement de la fonction barycentre
	  }
	  
	  	 
	  deleteIsNN(isNN_global,N);
	  
	  // partage des informations
	  for(int i = 0; i<N;++i){
		  MPI_Allreduce( newAgents_local[i], newAgents_global[i] , d , MPI::DOUBLE , MPI::SUM , MPI_COMM_WORLD);
	  }
	  
	  
	  deleteAgents(newAgents_local, N);
	  
	  //initialisation des nouveaux gents 
	  for(int i=0; i<N;++i){
		  agents[i] = Agent(d, newAgents_global[i]);
	  }
	  
	  deleteAgents(newAgents_global, N);
	  
	  // Retranscription des agents a l'etape l
	  if(r==0){
		  stringstream ss;
		  ss<<"Fin"<<l<<".dat";
		  writeAgents(agents,ss.str());
	  }
  }
  
    t2 = clock();
    std::cout << "Temps = " << (t2-t1)/float(CLOCKS_PER_SEC) << " secondes" << std::endl;
  
  
  

  MPI_Finalize();

  return 0;
}
