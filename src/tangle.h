#ifndef GUARD_TANGLE_H
#define GUARD_TANGLE_H

#include "filament.h"
#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

class Tangle{
public:
	/* member data */
	vector <Filament*> 	mTangle;
	int mN_f;			// number of timesteps between saves
	int mN_slow;
	double mDr;			// spatial resolution
	/* member functions */
	Tangle(){};
	Tangle(Filament* Ring1, Filament* Ring2){
		mTangle.push_back(Ring1);
		mTangle.push_back(Ring2);
		mN_f = 10000;

	}
	~Tangle(){};
	void CalcVelocityNL_OF();
	void LoopKill();
	void Reconnection();
	void SelfReconnect(int P, int Q, int k, int l);
	void Reconnect(int P, int Q, int k, int l);
	void CalcVelocity();
	void FromFile(); 	// mostly for debug purposes
	void PropagatePos(double & dt); 
	void SaveState();
	bool MeshAdjust();
};

#endif