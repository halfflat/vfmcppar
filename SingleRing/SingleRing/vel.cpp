// Calculate velocities of points on each ring
// Adapted from CalcVelMaster.m by Paul Walmsley

#include "vel.h"
#include "mesh.h"
#include <iostream>

using namespace std;

vector <vector <double> > Velocity(vector <vector <double> > Ring1, vector <vector <double> > Ring2){
	// Length of filaments in each ring
	//FilLength1 = MeshLengths(Ring1); FilLength2 = MeshLengths(Ring2);
	vector <vector <double> > SPrime = CalcSPrime(Ring1, MeshLengths(Ring1));
	return SPrime;

}

// Calculate s' from coefficients from Baggaley & Barenghi JLT 2012 166:3-20
vector <vector <double> > CalcSPrime(vector <vector <double> > Ring, vector <double> L){
	
	vector <double> A, B, C, D, E, blank;
	blank.push_back(0.0); blank.push_back(0.0); blank.push_back(0.0);
	vector <vector <double> > SPrime;

	// Funky for loop to generate correct indices for orderered array of lengths
	// Produces (98,99,0,1,2) -> (97,98,99,0,1) for N=100.
	int j,k,l,m;
	for(int i=0;i<Ring.size();i++){
		A.push_back(0.0); B.push_back(0.0); C.push_back(0.0); 
		D.push_back(0.0); E.push_back(0.0);
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=Ring.size()+1;}
		if(j-2==-2){j=Ring.size();}
		if(k-1==-1){k=Ring.size();}
		if(m+1==Ring.size()){m=-1;}
		if(m+2==Ring.size()){m=-2;}
		//cout << "(" << j-2 << ", " << k-1 << ", " << i << ", " << l+1 << ", " << m+2 << ")\n";

		A[i] = L[i]*L[l+1]*L[l+1]+L[i]*L[l+1]*L[m+2];
		A[i] = A[i] / (L[k-1]*(L[k-1]+L[i])*(L[k-1]+L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]));
		
		B[i] = -L[k-1]*L[l+1]*L[l+1]-L[i]*L[l+1]*L[l+1]-L[k-1]*L[l+1]*L[m+2];
		B[i] = B[i] / L[k-1]*L[i]*(L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]);

		D[i] = L[k-1]*L[i]*L[l+1]+L[i]*L[i]*L[l+1]+L[k-1]*L[i]*L[m+2]+L[i]*L[i]*L[m+2];
		D[i] = D[i] / L[l+1]*L[m+2]*(L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]);

		E[i] = -L[l+1]*L[i]*L[i]-L[k-1]*L[i]*L[l+1];
		E[i] = E[i] / L[m+2]*(L[l+1]+L[m+2])*(L[i]+L[i+1]+L[i+2])*(L[k-1]+L[i]+L[l+1]+L[m+2]);

		C[i] = -(A[i] + B[i] + D[i] + E[i]);		
	}
	// Calculate s' using FindFlag to handle disordered array and PBCs
	for(int p=0;p<Ring.size();p++){
		SPrime.push_back(blank);
		for(int q=0;q<3;q++){
			SPrime[p][q] += A[p]*FindFlag(Ring,p-2)[q];
			SPrime[p][q] += B[p]*FindFlag(Ring,p-1)[q];
			SPrime[p][q] += C[p]*FindFlag(Ring,p)[q];
			SPrime[p][q] += D[p]*FindFlag(Ring,p+1)[q];
			SPrime[p][q] += E[p]*FindFlag(Ring,p+2)[q];
		}
	}
	return SPrime;
}

// Return the position of a given flag value from array of positions
vector <double> FindFlag(vector <vector <double> > Ring, int f){
	vector <vector <double> >::iterator begin, current, end, wanted;
	begin = Ring.begin(); end = Ring.end();
	if(f < 0){f = Ring.size()+f;}
	if(f == Ring.size()){f = Ring.size()-f;}
	if(f > Ring.size()){f = Ring.size()-f+1;}
	for(current=begin;current!=end;current++){
		if(int((*current)[3]+0.1)==f){wanted = current; current = end-1;}
	}
	return (*wanted);
}


vector <vector <double> > CalcS2Prime(vector <vector <double> > Ring, vector <double> L){
	
	vector <double> A2, B2, C2, D2, E2, blank;
	blank.push_back(0.0); blank.push_back(0.0); blank.push_back(0.0);
	vector <vector <double> > S2Prime;

	// Funky for loop to generate correct indices for orderered array of lengths
	// Produces (98,99,0,1,2) -> (97,98,99,0,1) for N=100.
	int j,k,l,m;
	for(int i=0;i<Ring.size();i++){
		A2.push_back(0.0); B2.push_back(0.0); C2.push_back(0.0); 
		D2.push_back(0.0); E2.push_back(0.0);
		j = i; k = i; l = i; m = i;
		if(j-2==-1){j=Ring.size()+1;}
		if(j-2==-2){j=Ring.size();}
		if(k-1==-1){k=Ring.size();}
		if(l+1==100){l=-1;}
		if(m+1==100){m=-1;}
		if(m+2==100){m=-2;}
		//cout << "(" << j-2 << ", " << k-1 << ", " << i << ", " << l+1 << ", " << m+2 << ")\n";

		A2[i] = 2*(-2*L[i]*L[l+1]+L[l+1]*L[l+1]+L[l+1]*L[l+1]-L[i]*L[m+2]+L[l+1]*L[m+2]);
		A2[i] = A2[i] / (L[k-1]*(L[k-1]+L[i])*(L[k-1]+L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]));
		
		B2[i] = 2*(2*L[k-1]+2*L[i]*L[l+1]-L[l+1]*L[l+1]+L[k-1]*L[m+2]+L[i]*L[i+2]-L[l+1]*L[m+2]);
		B2[i] = B2[i] / L[k-1]*L[i]*(L[i]+L[l+1])*(L[i]+L[l+1]+L[m+2]);

		D2[i] = 2*(-L[k-1]*L[i]-L[i]*L[i]+L[k-1]*L[l+1]+2*L[i]*L[l+1]+L[k-1]*L[m+2]+2*L[i]*L[m+2]);
		D2[i] = D2[i] / L[l+1]*L[m+2]*(L[i]+L[l+1])*(L[k-1]+L[i]+L[l+1]);

		E2[i] = 2*(L[k-1]*L[i]+L[i]*L[i]-L[k-1]*L[l+1]-2*L[i]*L[l+1]);
		E2[i] = E2[i] / L[m+2]*(L[l+1]+L[m+2])*(L[i]+L[i+1]+L[i+2])*(L[k-1]+L[i]+L[l+1]+L[m+2]);

		C2[i] = -(A2[i] + B2[i] + D2[i] + E2[i]);		
	}
	// Calculate s'' using FindFlag to handle disordered array and PBCs
	for(int p=0;p<Ring.size();p++){
		S2Prime.push_back(blank);
		for(int q=0;q<3;q++){
			S2Prime[p][q] += A2[p]*FindFlag(Ring,p-2)[q];
			S2Prime[p][q] += B2[p]*FindFlag(Ring,p-1)[q];
			S2Prime[p][q] += C2[p]*FindFlag(Ring,p)[q];
			S2Prime[p][q] += D2[p]*FindFlag(Ring,p+1)[q];
			S2Prime[p][q] += E2[p]*FindFlag(Ring,p+2)[q];
		}
	}
	return S2Prime;
}

