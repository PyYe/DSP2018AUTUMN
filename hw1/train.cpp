#include "hmm.h"
#include <vector>
#include <string>
#include <numeric>	
#include <iostream>
#include <fstream>
using namespace std; 

typedef struct{

	double alpha[MAX_SEQ][MAX_STATE];			
	double beta[MAX_SEQ][MAX_STATE];	
	double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];
	double gamma[MAX_SEQ][MAX_STATE];
} PARA;

/*
static void cal_alpha(HMM *hmm, PARA *para, string seq);
static void cal_beta(HMM *hmm, PARA *para, string seq);
static void cal_epilson(HMM *hmm, PARA *para, string seq);
static void cal_gamma(HMM *hmm, PARA *para, string seq);
static void train(HMM *hmm, PARA *para, vector<string> seqs);
*/

static void cal_alpha(HMM *hmm, PARA *para, string seq)
{
	int seq_len = seq.length();
	//cout << hmm->state_num << endl;
	//cout << seq_len <<endl;
	//Initialization
	for( int i = 0 ; i < hmm->state_num ; i++ )
	{
		para->alpha[0][i] = hmm->initial[i] * hmm->observation[seq[0]-'A'][i];
		//cout << para->alpha[0][i]<<" initial "<<hmm->initial[i] << " obser "<< hmm->observation[seq[0]-'A'][i]<<endl;
	}
	
	//Induction
	//double sum;
	for( int t = 0 ; t < (seq_len-1) ; t++ )
	{
		for( int j = 0 ; j < hmm->state_num ; j++ )
		{
			double sum = 0;
			for(int i = 0 ; i < hmm->state_num ; i++)
			{
				sum += ( para->alpha[t][i] * hmm->transition[i][j] );
				//cout << " sum "<<sum<<" alpha "<<para->alpha[t][i]<<" t "<<t<<" i "<<i<<" trainsition "<<hmm->transition[i][j]<<endl;
			}				
			
			para->alpha[t+1][j] = sum * hmm->observation[seq[t+1] - 'A'][j]; 
			
			//cout<<" t+1 "<<t+1<<" j "<<j<<" "<<para->alpha[t+1][j]<<" seq[t]-'A' "<<seq[t]-'A'<<" observ "<<hmm->observation[seq[t]-'A'][j]<<" sum "<<sum<<endl;
		}
	}	
}
static void cal_beta(HMM *hmm, PARA *para, string seq)
{
	double sum;
	int seq_len = seq.length();
	//Initialization
	for(int i=0;i<hmm->state_num;i++)
	{
		para->beta[seq_len-1][i] = 1;
	}
	
	//Induction
	
	for( int t = seq_len-2 ; t >= 0 ; t-- )
	{
		for( int i = 0 ; i < hmm->state_num ; i++ )
		{
			sum = 0;
			for(int j = 0 ; j < hmm->state_num ; j++ )
			{
				sum += ( hmm->transition[i][j] * hmm->observation[seq[t+1]-'A'][j] * para->beta[t+1][j] );
			}				
			
			para->beta[t][i] = sum;
		}
	}
}
static void cal_epilson(HMM *hmm, PARA *para, string seq)//double epsilon[MAX_SEQ][MAX_STATE][MAX_STATE];
{
	double sum;
	int seq_len = seq.length();
	for( int t = 0 ; t < (seq_len-1) ; t++ )
	{
		sum = 0;
		// calculate sum
		for( int j = 0 ; j < hmm->state_num ; j++ )
		{
			for( int i = 0 ; i < hmm->state_num ; i++ )
			{
				sum += para->alpha[t][i] *  hmm->transition[i][j] * hmm->observation[seq[t+1]-'A'][j] * para->beta[t+1][j];
			}	
		}
		//calculate eplison
		for( int j = 0 ; j < hmm->state_num ; j++ )
		{
			for( int i = 0 ; i < hmm->state_num ; i++ )
			{
				para->epsilon[t][i][j] = para->alpha[t][i] *  hmm->transition[i][j] * hmm->observation[seq[t+1]-'A'][j] * para->beta[t+1][j] / sum;
			}	
		}
	}
}
static void cal_gamma(HMM *hmm, PARA *para, string seq)//double gamma[MAX_SEQ][MAX_STATE];
{
	double sum;
	int seq_len = seq.length();
	for( int t = 0 ; t < seq_len ; t++ )
	{
		sum = 0;
		for( int i = 0 ; i < hmm->state_num ; i++ )
		{
			sum += para->alpha[t][i] * para->beta[t][i];
		}
		
		for( int i = 0 ; i < hmm->state_num ; i++ )
		{
			para->gamma[t][i] = para->alpha[t][i] * para->beta[t][i] / sum;
			//if (i<5 && t<5)
			//	cout <<"t"<<t <<"i"<<i<<" "<<para->gamma[t][i]<<endl;
		}
	}
}
static void train(HMM *hmm, PARA *para, vector<string> seqs)
{
	int seq_length = seqs[0].length();
	double Pi_sum[MAX_STATE]={}; //(i)
	double A_D_sum[MAX_STATE]={}; //(i)
	double A_N_sum[MAX_STATE][MAX_STATE]={}; //(i)(j)
	double B_D_sum[MAX_STATE]={}; //(i)
	double B_N_sum[MAX_STATE][MAX_STATE]={}; //(i)(j)
        		
	//calculate
	//vector<int>::iterator it_i;
	for(auto it_i=seqs.begin() ; it_i!=seqs.end() ; ++it_i)
	{
		string seq = *it_i;		
		cal_alpha(hmm, para, seq);
		cal_beta(hmm, para, seq);
		cal_gamma(hmm, para, seq);
		cal_epilson(hmm, para, seq);
		
	
		for ( int i = 0; i < hmm->state_num; i++ ) {
				
			// sum for pi
			Pi_sum[i] += para->gamma[0][i];
				
			// sum for A
			for ( int t = 0; t < (seq_length-1) ; t++ ) {
				//A's Denominator sum
				A_D_sum[i] += para->gamma[t][i];
				//A's Numerator sum
				for ( int j = 0; j < hmm->state_num ; j++ )
					A_N_sum[i][j] += para->epsilon[t][i][j];
			}
	
			// sum for B
			for ( int t = 0; t < seq_length; t++ ) {
				B_D_sum[i] += para->gamma[t][i];
				B_N_sum[seq[t]- 'A'][i] += para->gamma[t][i];//[seq[t]-65] because Ot=Vt
			}
		}
		//break;
	}		
		
	//update lambda
		
	//pi
	for (int i = 0; i < hmm->state_num; i++) {
		hmm->initial[i] = Pi_sum[i] / seqs.size();
	}
	//A
	for (int i = 0; i < hmm->state_num; i++) {
		for (int j = 0; j < hmm->state_num; j++) {
			hmm->transition[i][j] = A_N_sum[i][j] / A_D_sum[i];
		}
	}
	//B
	for (int k = 0; k < hmm->state_num; k++) {
		for (int j = 0; j < hmm->state_num; j++) {
			hmm->observation[k][j] = B_N_sum[k][j] / B_D_sum[j];
			//cout << hmm->observation[k][j] <<" k "<<k<<" j "<<j<<" "<<B_N_sum[k][j]<<" "<<B_D_sum[j]<<endl;
		}
	}
}
int main(int argc, char *argv[])
{	
	int iters_num = stoi(string(argv[1]));
	string init_path = argv[2];
	string seq_path = argv[3];
	string out_path = argv[4];
	
	HMM hmm;
	loadHMM( &hmm, init_path.c_str());
	
/*
	for(int i=0; i <4;i++){
		for(int j=0;j<4;j++)
			//cout<<hmm.observation[i][j]<<" ";
		//cout<<endl;
	}
*/


	//read seq_model_XX.txt to seqs 
	vector<string> seqs;
        string inputStr;
	ifstream in(seq_path);
        while(getline(in, inputStr)){
		//cout<<inputStr<<endl;
        	seqs.push_back(inputStr);
    	}
    	in.close();
	
	//train(update lambda(pi,A,B))
	PARA para;
	for (int iter = 0 ; iter < iters_num ; iter++)//update lambda  [iters_num] times
	{
		train(&hmm, &para, seqs);
	}
	
	dumpHMM(open_or_die(out_path.c_str(), "w"), &hmm);
	
	/*cout << para.alpha[1][0]<<endl;
	cout << para.beta[1][0]<<endl;
	
	cout << para.gamma[4][2]<<endl;
	cout << para.epsilon[1][1][1]<<endl;*/
	dumpHMM(open_or_die(out_path.c_str(), "w"), &hmm);

	return 0;
}
