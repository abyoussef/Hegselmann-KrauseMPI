# Hegselmann-KrauseMPI
_Implementation of HK opinion dynamics model with MPI._ 

To excecute this algorithm, 
### Step 1 : Compilation of the data generation code
    g++ DataGen.cpp -o DataGen
### Step 2 : Generate random data with N points in dimension dim and between min and max
    ./DataGen   N=1000      dim=2    min=0   max=100 
### Step 3 : Compilation of the HK Algorithm code in MPI 
    mpic++ Agent.cpp HKAlgoMPI.cpp -o HKAlgoMPI.exe 
### Step 4 : Excecution with 6 processors, a threshold of (Diameter of the Dataset)/20 and 15 iterations on the DataGen.dat dataset
    mpirun -np  6  HKAlgoMPI.exe   DataGen.dat    20    15 
### Step 5 : Generate the GIF animation    
    gnuplot plot.plt  

## Animation 2D :
![animate2D](https://github.com/abyoussef/Hegselmann-KrauseMPI/blob/master/animate2D.gif)

## Animation 3D : 
![animate3D](https://github.com/abyoussef/Hegselmann-KrauseMPI/blob/master/animate3D.gif)
