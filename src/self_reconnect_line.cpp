/* 'Smart' line self-reconnection algorithm that chooses whether to use Type I/II in Baggaley, JLT 168:18-30 (2012).
	depending on line length following reconnection. */

#include "tangle.h"
#include "point.h"
#include <fstream>
#include <sstream>

using namespace std;

void Tangle::SelfReconnectLine(int P, int Q, int k, int l){
	/* ensure l follows k */
	Point* pTest = mTangle[P]->mPoints[k]->mNext;
	bool l_after_k(false);

	while(pTest->mNext->mFlagDummy==0){
		if(pTest == mTangle[Q]->mPoints[l]){
			l_after_k = true;
		}
		pTest = pTest->mNext;
	}
	if(l_after_k == false){
		int swap(l); l = k; k = swap;
	}
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tattempting line reconnection" << endl;
	/* create new ring in tangle */
	mTangle.push_back(new Ring());
	/* separate new wring */
	double mindist2(123456);
	int dist_flag(0);
/*	double dist_i_1(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_i_1 += mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_1<mindist2){mindist2=dist_i_1; dist_flag = 1;}
	
	double dist_i_2(mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_i_2 += mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_i_2<mindist2){mindist2=dist_i_2; dist_flag = 2;}*/
	
	double dist_ii_1(mTangle[P]->mPoints[k]->mNext->Disp2(mTangle[Q]->mPoints[l]->mPrev));
	dist_ii_1 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_1<mindist2){mindist2=dist_ii_1; dist_flag = 3;}
	
	double dist_ii_2(mTangle[P]->mPoints[k]->mPrev->Disp2(mTangle[Q]->mPoints[l]->mNext));
	dist_ii_2 += mTangle[P]->mPoints[k]->Disp2(mTangle[Q]->mPoints[l]);
	if(dist_ii_2<mindist2){mindist2=dist_ii_2; dist_flag = 4;}
	int i(0);
	Point* pStart;
	Point* pNew;
	switch(dist_flag){
		case 1: { // temperamental - hopefully rarely called.
			int swap(0); swap = l; l = k; k = swap;
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 1 line reconnection" << endl;
    		/* iterate over filament to find index of knext as it will be lost in reassignment */
    		int k_prev_cache(123456);
    		for(int j(0); j<mTangle[P]->mN; j++){
    			if(mTangle[P]->mPoints[j] == mTangle[P]->mPoints[k]->mNext){
    				k_prev_cache = j;
    			}
    		}
    		mTangle[P]->mPoints[k]->mPrev = mTangle[Q]->mPoints[l]->mPrev;
    		mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k];  
			mTangle[Q]->mPoints[l]->mPrev = mTangle[P]->mPoints[k_prev_cache];
			mTangle[P]->mPoints[k_prev_cache]->mNext = mTangle[Q]->mPoints[l];			
			pNew = mTangle[P]->mPoints[k]->mNext;
			pStart = pNew;
	    	i = 0;
    		do{
		        mTangle.back()->mPoints.push_back(new Point(pNew));
        		mTangle.back()->mN++;
        		pNew = pNew->mNext;
        		i++;
    		}while(pNew!=pStart);
    		break;
    	}
		case 2: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 2 line reconnection" << endl;
			/* iterate over filament to find index of knext as it will be lost in reassignment */
			int k_next_cache(123456);
			for(int j(0); j<mTangle[P]->mN; j++){
				if(mTangle[P]->mPoints[j] == mTangle[P]->mPoints[k]->mNext){
					k_next_cache = j;
				}
			}
			mTangle[P]->mPoints[k]->mNext = mTangle[Q]->mPoints[l]->mNext;
			mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k];  
			mTangle[Q]->mPoints[l]->mNext = mTangle[P]->mPoints[k_next_cache];
			mTangle[P]->mPoints[k_next_cache]->mPrev = mTangle[Q]->mPoints[l];			
			pNew = mTangle[Q]->mPoints[l];
			pStart = pNew;
			i = 0;
			do{
				mTangle.back()->mPoints.push_back(new Point(pNew));
				mTangle.back()->mN++;
				pNew = pNew->mNext;
				i++;
			}while(pNew!=pStart);
			break;
		}
		case 3: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 3 line reconnection" << endl;
			mTangle[P]->mPoints[k]->mNext->mPrev = mTangle[Q]->mPoints[l]->mPrev;  
			mTangle[Q]->mPoints[l]->mPrev->mNext = mTangle[P]->mPoints[k]->mNext;
			pNew = mTangle[P]->mPoints[k]->mNext;
			pStart = pNew;
			/* reassign pointers to separate new ring */
			mTangle[P]->mPoints[k]->mNext = mTangle[Q]->mPoints[l];
			mTangle[Q]->mPoints[l]->mPrev = mTangle[P]->mPoints[k];		
			i = 0;
			do{
				mTangle.back()->mPoints.push_back(new Point(pNew));
				mTangle.back()->mN++;
				pNew = pNew->mNext;
				i++;
			}while(pNew!=pStart);
			break;
		}
		case 4: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tcase 4 line reconnection" << endl;
			mTangle[P]->mPoints[k]->mPrev->mNext = mTangle[Q]->mPoints[l]->mNext;  
			mTangle[Q]->mPoints[l]->mNext->mPrev = mTangle[P]->mPoints[k]->mPrev;
			pNew = mTangle[P]->mPoints[k];
			pStart = pNew;
			/* reassign pointers to separate new ring */
			mTangle[P]->mPoints[k]->mPrev = mTangle[Q]->mPoints[l];
			mTangle[Q]->mPoints[l]->mNext = mTangle[P]->mPoints[k];
			i = 0;
			do{
				mTangle.back()->mPoints.push_back(new Point(pNew));
				mTangle.back()->mN++;
				pNew = pNew->mNext;
				i++;
			}while(pNew!=pStart);
			break;
		}
		default: {
			mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tOdd kind of reconnection encountered - need to reconsider input" << endl;
		}
	}
	/* count points on new ring and assign pointers */
	int N_new = mTangle.back()->mN;
	for(int d(1); d<N_new-1; d++){
		mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
		mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mPoints[N_new-1];
	mTangle.back()->mPoints[N_new-1]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mPoints[N_new-1]->mPrev = mTangle.back()->mPoints[N_new-2];
	mTangle.back()->CalcMeshLengths();
	/* add new line of remaining points */
	mTangle.push_back(new String());
	int N_rem = mTangle[P]->mN - N_new;
	pNew = mTangle[P]->mPoints[0];
	/* push back position and velocities of new points to tangle */
	int v(0);
	do{
		mTangle.back()->mPoints.push_back(new Point(pNew));
		mTangle.back()->mN++;
		pNew = pNew->mNext;
		v++;
	}while(v!=N_rem);
	/* assign pointers and calculate dummies' positions*/
	for(int d(1); d!=N_rem-1; d++){
	mTangle.back()->mPoints[d]->mPrev = mTangle.back()->mPoints[d-1];
	mTangle.back()->mPoints[d]->mNext = mTangle.back()->mPoints[d+1];
	}
	for (int i=0; i != 4; i++){
		mTangle.back()->mDummies.push_back(new Point()); // note that default constructor labels as not a dummy!
	}
	mTangle.back()->mPoints[N_rem-1]->mPrev = mTangle.back()->mPoints[N_rem-2];
	mTangle.back()->mPoints[N_rem-1]->mNext = mTangle.back()->mDummies[0];
	mTangle.back()->mDummies[0]->mPrev = mTangle.back()->mPoints[N_rem-1];
	mTangle.back()->mDummies[0]->mNext = mTangle.back()->mDummies[1];
	mTangle.back()->mDummies[1]->mPrev = mTangle.back()->mDummies[0];
	mTangle.back()->mPoints[0]->mNext = mTangle.back()->mPoints[1];
	mTangle.back()->mPoints[0]->mPrev = mTangle.back()->mDummies[2];
	mTangle.back()->mDummies[2]->mNext = mTangle.back()->mPoints[0];
	mTangle.back()->mDummies[2]->mPrev = mTangle.back()->mDummies[3];
	mTangle.back()->mDummies[3]->mNext = mTangle.back()->mDummies[2];
	mTangle.back()->CalcMeshLengths();
	mTangle.back()->CalcDummy();
	/* label dummies for further mesh calculations */
	for (int i=0; i != 4; i++){
		mTangle.back()->mDummies[i]->mFlagDummy = 1;
	}
	mTangle.back()->CalcMeshLengths();
	mTangle.back()->mPoints[0]->mFlagFilled = 5;// starting point should remain stationary
	for(unsigned int q(0); q<mTangle[P]->mPoints.size(); q++){
		delete mTangle[P]->mPoints[q];
	}
	mTangle.erase(mTangle.begin()+P);
	mN_recon++;
	mLog << StringTime() << "\t" << setw(10) << mStep << ":\t\tsuccessful line reconnection" << endl;
}