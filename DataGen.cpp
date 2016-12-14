#include <iostream> // bibliotheque permettant l'utilsation de fonctions input/output dans le terminal
#include <fstream> // bibliotheque permettant lecture et ecriture dans des fichiers (file input/ouput)
#include <cstdlib> // bibliotheque permettant utilisation des generateurs de nombres aleatoires (type rand)
#include <iomanip>      // std::setw

using namespace std;

double randab(int a, int b){
	return ( rand()/(double)RAND_MAX ) * (b-a) + a ;
}
int main(int argc, char *argv[]){
	if(argc != 5){
		std::cerr << "Usage: ./DataGen <int nombre de points> <int dimension> <int Min> <int Max>" << std::endl;
		exit(-1);
	}
	int dim = atoi(argv[2]);
	int N = atoi(argv[1]);
	int a = atoi(argv[3]);
	int b = atoi(argv[4]);
	ofstream myfile;
	myfile.open("DataGen.dat");
	for(int i(0); i<N;++i){
		for (int j(0); j<dim; ++j){
			myfile<<randab(a,b)<<",";
					
		}
		myfile<<endl;
	}
	
	
	myfile.close();
	
	
}