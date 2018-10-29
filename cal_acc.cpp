#include <vector>
#include <string>
#include <numeric>	
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <sstream>
#include <string.h>
using namespace std;
 
//./cal_acc result1.txt testing_answer.txt acc.txt
int main(int argc, char *argv[])
{
	string result_path = argv[1];
	string answer_path = argv[2];
	string acc_path = argv[3];
	//============read result file===========================
	vector<string> results;
    	string inputResult;
	ifstream Rin(result_path);
	int sum = 0;
    	string line;
	//int sum = 0;
	while( getline(Rin, inputResult) )
	{
		//cout << inputResult << endl;
		stringstream ss(inputResult);
		while( getline(ss, inputResult, ' ') )
		{
			sum+=1;
			//cout << " sum "<<sum;
			double aa = sum % 2;
			//cout << " aa "<<aa;
			if( aa == 1.0 ){
				//cout << " inputResult " << inputResult ;
				results.push_back(inputResult);
			}
			//cout << " end " <<endl;
		}
		//cout <<" end "<< endl;
	}
    	Rin.close();
	//============read answer file===========================
	vector<string> answers;
    	string inputAnswer;
	ifstream Ain(answer_path);
   	while( getline(Ain, inputAnswer) )
        	answers.push_back(inputAnswer);
  
    	Ain.close();
	//========================================
	double T_num = 0;
	double F_num = 0;
	
	for(int i = 0 ; i < answers.size() ; ++i){
        	/*
		//split result 
		string s = results[i];
		char str[] = s;
		char *pch;
		pch = strtok (str," ");
		*/
		//compare result and answer  
		//cout << " A " << answers[i] << " R " << results[i]<<endl;
		if ( answers[i] == results[i] )
			T_num += 1;
		else
			F_num += 1;
	}
	cout << " T " << T_num << " F " << F_num << endl;
	double acc ;
	acc = T_num /( T_num + F_num );
	cout << " acc " << acc << endl;
	ofstream out(acc_path);
	out << acc ;	
	out.close();
}	
