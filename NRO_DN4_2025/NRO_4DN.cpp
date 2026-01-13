#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <chrono>
#include <omp.h>
#include <ctime>
#include <unistd.h>

using namespace std;

int main() {

  // inicializiramo matriko A
  vector<vector<double>> A;

  // inicializiramo vektor b;
  vector<double> b;
  

  std::string filename = "datoteka_A_b.txt";

  // preberemo datoteko
  std::ifstream infile;
  infile.open(filename);

  if (!infile.is_open()) {
      std::cerr << "Napaka: Datoteke '" << filename << "' ni mogoce odpreti!" << std::endl;
      std::cerr << "Preverite, ali ste v terminalu v pravi mapi (uporabite ukaz 'cd')." << std::endl;
      return 1;
  }

  // preberemo prvo vrstico, v kateri imamo podano velikost matrike A 
  std::string string_first_line;
  std::getline(infile, string_first_line);
  
  std::replace(string_first_line.begin(), string_first_line.end(), '=', ' ');

  std::istringstream iss(string_first_line);
  std::string nepomemben_del1; 
  std::string nepomemben_del2; 
  int n; 

  iss >> nepomemben_del1;
  iss >> nepomemben_del2;
  iss >> n;

  std::cout << "Velikost matrike A: " << n << "x" << n << std::endl;; 

  // Branje matrike A
  for (int iiA=0; iiA < n; iiA++)
    {
      std::string line;
      std::getline(infile, line);
      std::replace(line.begin(), line.end(), ';', ' ');

      std::istringstream iss_column(line);
      vector<double> row;

      for (int column=0; column<n; column++)
	{
	  double element_a = 0;
	  iss_column >> element_a;
	  row.push_back(element_a);
	}
      A.push_back(row);
    }

  std::string empty_line;
  std::getline(infile, empty_line);

  // Branje vektorja b
  std::string string_line_b;
  std::getline(infile, string_line_b);

  std::replace(string_line_b.begin(), string_line_b.end(), '>', ' ');
  std::istringstream iss_b(string_line_b);
  int n_b; 

  iss_b >> nepomemben_del1;
  iss_b >> nepomemben_del2;
  iss_b >> n_b;

  std::cout << "Velikost vektorja b: " << n_b << std::endl;; 

  for (int iib=0; iib<n_b; iib++)
    {
      std::string line_b_element;
      std::getline(infile, line_b_element);
      std::istringstream iss_b_element(line_b_element);
      
      double b_element=0;
      iss_b_element >> b_element;

      b.push_back(b_element);
    }

  // Inicializacija T
  vector<double> T;
  for (int iiT=0; iiT<n_b; iiT++)
    {
      T.push_back(100);
    }

  auto start_time = std::chrono::high_resolution_clock::now();

  // ===================================================================================
  // IMPLEMENTACIJA GS METODE IN OPENMP ANALIZA
  // ===================================================================================
  
  /* ANALIZA PARALELIZACIJE (OpenMP):
     
     Gauss-Seidelove (GS) metode v svoji osnovni obliki ni mogoče preprosto paralelizirati 
     z uporabo #pragma omp parallel for na notranji zanki.
     
     Razlog: ODVISNOST PODATKOV (Loop-Carried Dependency).
     Pri izračunu T[i] v trenutni iteraciji (k+1) potrebujemo:
     - T[0]...T[i-1], ki so že izračunani v TEJ (k+1) iteraciji.
     - T[i+1]...T[n], ki so iz PREJŠNJE (k) iteracije.
     
     Če bi uporabili paralelne niti, bi ena nit poskušala brati T[i-1], medtem ko ga druga
     nit še ni izračunala. To vodi v "Race Condition" in napačne rezultate (ali pa bi 
     metoda degenerirala v Jacobijevo metodo, če bi brali samo stare vrednosti).
     
     Zato se spodnja koda izvaja serijsko za zagotavljanje pravilne konvergence.
  */

  int max_iterations = 2000;

  for (int ii=0; ii<max_iterations; ii++)
    {
      for (int i = 0; i < n; i++) {
          double sigma = 0.0;
          
          
          for (int j = 0; j < n; j++) {
              if (j != i) {
                  sigma += A[i][j] * T[j];
              }
          }
          
          
          if (A[i][i] != 0.0) {
              T[i] = (b[i] - sigma) / A[i][i];
          }
      }
    } 

  auto end_time = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> time_duration = end_time - start_time;
  
  
  double max_T = -1e9; 
  
 
  for (int iiT=0; iiT<n_b; iiT++)
    {
      
      
      if (T[iiT] > max_T){
	    max_T = T[iiT];
      }
    }

  std::cout << "Serial Time: " << time_duration.count() << " seconds" << std::endl;
  cout << "Max. temperature: " << max_T << " degree C." << endl;
  
  return 0;
}