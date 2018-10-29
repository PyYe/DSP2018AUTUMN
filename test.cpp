#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include "hmm.h"

using namespace std;

double Viterbi(HMM *hmm, int m, string seq)
{
	int seq_len = seq.length();
	double delta[MAX_SEQ][MAX_STATE]={};
	//Initialization      
    	for( int i = 0 ; i < hmm->state_num ; i++ )
    	{
        	delta[0][i] = hmm->initial[i] * hmm->observation[seq[0]-'A'][i];
    	}
	// recursion
	for ( int t = 1 ; t < seq_len ; t++ )
	{
		for ( int j = 0 ; j < hmm->state_num ; j++ )
		{
			//calculate max
			double max_delta = -1;
			double tmp;
			for ( int i = 0 ; i < hmm->state_num ; i++ )
			{
				tmp = delta[t-1][i] * hmm->transition[i][j];
				if (tmp > max_delta)
					max_delta = tmp;
			}
			//calculate delta
			delta[t][j] = max_delta * hmm->observation[seq[t]-'A'][j];
		}
	}

    // termination
	double max_P = -1;
	for ( int i = 0 ; i < hmm->state_num ; i++ )
	{
		
		if (delta[seq_len-1][i] > max_P)
			max_P = delta[seq_len-1][i];
	}
	return max_P;
	
}

int main(int argc, char *argv[])
{
	string modellist_path = argv[1];
	string testdata_path = argv[2];
	string result_path = argv[3];
	
	
	//read models in modellist.txt to hmms 
	int model_count = 0;
	ifstream model_in(modellist_path);
	string modelStr;
    	while(getline(model_in, modelStr)){
		model_count += 1; 
    	}
    	model_in.close();

	static int models_num = model_count;
	
	HMM hmms[models_num];
	load_models(modellist_path.c_str(), hmms, models_num);
	
	//read testdata.txt to seqs 
	vector<string> seqs;
    	string inputStr;
	ifstream in(testdata_path);
    	while(getline(in, inputStr)){
        	seqs.push_back(inputStr);
    	}
    	in.close();
	
	// write result to result_path
	
	FILE *fp = fopen(result_path.c_str(), "wb");
	
	for(auto it_i=seqs.begin() ; it_i!=seqs.end() ; ++it_i){
        
		string seq = *it_i;		
		// for each example,
		double max_P; //Viterbi output
		double MAX_mp = -1; //record max probability of models
        	int MAX_model = -1;// record model which has max probability
        	
		for(int m = 0 ; m < models_num ; m++){
            	// go through 5 models
			max_P = Viterbi(&hmms[m], m, seq);
            
			if(max_P > MAX_mp){
                MAX_mp = max_P;
                MAX_model = m;
            }
        }
        // print the result to the specified file
        fprintf(fp, "model_0%d.txt %e\n", MAX_model + 1, MAX_mp);
    }
	
	
	
	fclose(fp);
	
	return 0;
}
