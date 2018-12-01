#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "File.h"
#include "Vocab.h"
#include "Ngram.h"
#include <string.h>
#include <float.h>
#include <map>
#include <string>
#include <fstream>
#include <vector>
#include <typeinfo>
using namespace std;
class Information 
{
	public:
		string word;
		double prob;
		int bestindex;
};

double getBigramProb(const char *w1, const char *w2, Vocab& voc, Ngram& bg)
{
	VocabIndex wid1 = voc.getIndex(w1);
	VocabIndex wid2 = voc.getIndex(w2);
    	// cout << wid1 << " " << wid2 << endl;

    	if(wid1 == Vocab_None)  //OOV
        	wid1 = voc.getIndex(Vocab_Unknown);
    	if(wid2 == Vocab_None)  //OOV
        	wid2 = voc.getIndex(Vocab_Unknown);

    	VocabIndex context[] = { wid1, Vocab_None };
    	return bg.wordProb( wid2, context);
}

int main(int argc, char *argv[])
{
	int order = atoi(argv[8]);
	Vocab voc;
	Ngram bg(voc, order);
	// read "testdata/xx.txt"
	/*
	 ㄏ 視   ㄒ 聞   開 ㄔ   ㄧ 喜   李   四   端   金   素   梅   明   搭   檔   雙   主   播
	 華 ㄕ   ㄒ 聞   將 在 ㄇ 天   年   第 ㄧ 天   推 ㄔ   ㄩ 旦   雙   主   播
	 由   王 ㄆ   主   播   李   四   端   與   剛   ㄔ 爐   ㄉ 新   科   立 ㄨ   高   金   素   梅   ㄧ 同   播   報   ㄒ 聞
	 ㄔ 試   啼 ㄕ   的   播   報   ㄋ 容
	 就 ㄕ   高   金   素   梅   自 ㄐ   明 天 上 ㄨ   應   總 統 ㄈ   ㄓ 邀
	 參 ㄐ   ㄩ 旦   ㄕ 旗 典 禮   唱   ㄍ 歌   的 ㄒ 聞
	 華 ㄕ   ㄐ 天 召 開   董 ㄕ 會
	 總 經 ㄌ   周   蓉   ㄕ 與   ㄐ 營   ㄊ 隊   展 ㄒ   上 ㄖ   ㄧ 年   多 ㄉ   成 績 ㄉ
	 在   節 ㄇ   ㄐ 營   組 ㄓ   重 ㄓ   ㄕ 都   有 ㄒ   氣 ㄒ
	 在 會 ㄓ   ㄉ 事   郭   力   昕   提 ㄢ   討 ㄌ   與   台 ㄕ   ㄘ 略   聯 ㄇ 的   可 ㄒ 性
	 董 ㄕ 會   ㄐ 定   以   ㄎ 源 節 流
	 ...
	*/
	//cout << "Open TestFile" << argv[2] << endl;
	ifstream TestFile(argv[2], ifstream::in);
   	string testStr;
    	vector<string> testContent;
    	while(getline(TestFile, testStr)){
		//cout << "testStr : " << testStr << endl;
		testContent.push_back(testStr);
	}
    	TestFile.close();
	//cout << "Test File was read " <<endl;
	// read "ZhuYin-Big5.map"
	/*
	...	
	於 於
	旺 旺
	昔 昔
	易 易
	昌 昌
	昆 昆
	ㄤ 昂 盎 骯 卬 醠
	昂 昂
	明 明
	...
	*/                                                                                                       
	ifstream MapFile(argv[4], ifstream::in);
    	string mapStr;
    	map<string, vector<Information> > mapContent;
    	while(getline(MapFile, mapStr)){
		int firstword = 1;
        	string key = mapStr.substr(0,2);
		vector<Information> chineseVector;
		size_t pos = 0;
		//cout << "pos:" << pos << endl;
		string deli = " ";
		while( (pos = mapStr.find(deli)) != std::string::npos ){
			//cout << "fw:" << firstword << endl;
			string chinese = mapStr.substr(0, pos);
			//cout << "chinese:" << chinese << endl;
			if (firstword==1){
				//cout << "firstword:"<<firstword<<endl;
				firstword=0;
			}
			//cout << "erased" << endl;
			else if (chinese.length()==2 && firstword!=1){
				//cout << "chinese:" << chinese << endl;
				//if (firstword < 0 )cout << "firstword:" << firstword << endl;
				firstword -= 1;
				//cout << ""<<<<
				Information chineseInformation;
				chineseInformation.word = chinese;
				chineseInformation.prob = 0.0;//getBigramProb("", chinese.c_str(), voc, bg);
				//cout << "cp:" << chineseInformation.prob;
				chineseInformation.bestindex = 0;
				chineseVector.push_back(chineseInformation);
			}
			else {};
			mapStr.erase(0, pos+deli.length());

		}
		mapContent[key] = chineseVector;
    	}
    	MapFile.close();
	//cout << "MapFile was read" << endl;

	// read "bigram.lm"
	File BigramFile(argv[6], "r");
    	bg.read(BigramFile);
    	BigramFile.close();
	//cout << "BigramFile was read" << endl;

	// Viterbi
	vector<vector<Information>> forward;
	//cout << "vector<vector<Information>> forward" << endl;
	vector<string> revisedText;
	vector<string>::iterator revised_iter;
	for (vector<string>::iterator test_Str=testContent.begin() ; test_Str!=testContent.end() ; test_Str++){
		//cout << " AAA " << endl;
		revisedText.clear();
		cout << "<s> ";
		forward.clear();
		// Viterbi forward
		int len_test_Str = (*test_Str).length();
		//cout << "len_test_Str:" << len_test_Str << endl;
		for (int i=0 ; i<len_test_Str ; i++){
			//cout << "i:" << i << endl;
			//cout << "*test_Str[i].size" << sizeof((*test_Str)[i]) << endl;
			if ((*test_Str)[i] != ' '){
				string word_temp = (*test_Str).substr(i, 2);
				//cout << "word_temp:" << word_temp << endl;
				//vector<string> possiList; 
				//cout << "possiList begin" << endl;
				//cout << typeid(mapContent.find(word_temp)->second).name() << endl;
				//vector<string>::iterator ii=mapContent.find(word_temp)->second.begin();
				//cout<< (*ii) << endl;
				//possiList.assign( mapContent.find(word_temp)->second.begin(), mapContent.find(word_temp)->second.end() );
				//cout << "possiList" << endl;
				forward.push_back(mapContent.find(word_temp)->second);
				i++;
			}
		}
		//cout << "Starting calulate prob" << endl;

		for (vector<Information>::iterator iter = forward.begin()->begin(); iter!=forward.begin()->end();iter++){
			iter->prob = getBigramProb("", iter->word.c_str(), voc,bg);
		}

		for (vector<vector<Information>>::iterator iter_forw=forward.begin()+1 ; iter_forw!=forward.end() ; iter_forw++){
			for (vector<Information>::iterator iter_now=iter_forw->begin() ; iter_now!=iter_forw->end() ; iter_now++){
				int index = 0;
				int best_index;
				double best_prob = -DBL_MAX;
				double prob_temp;
				
				for (vector<Information>::iterator iter_prev=(iter_forw-1)->begin() ; iter_prev!=(iter_forw-1)->end() ; iter_prev++){
					//cout << "enter iter_prev" << endl;
					//cout << "iter_prev->prob" << iter_prev->prob << endl;
					prob_temp = iter_prev->prob + getBigramProb(iter_prev->word.c_str(), iter_now->word.c_str(), voc, bg);
					//cout << "prob_temp:" << prob_temp <<endl;
					if (prob_temp>best_prob){
						//cout << "best_prob update" << prob_temp <<endl;
						best_prob = prob_temp;
						best_index = index;
					}
					index += 1;
				}
				iter_now->prob = best_prob;
				iter_now->bestindex = best_index;
				//cout << best_prob << endl;
			}
		}
		//cout << "best_prob:" <<endl;
		// backward and cout
		int index_temp = 0;
		int init = 1;
		for(vector<vector<Information> >::iterator iter_back=forward.end()-1; iter_back!=forward.begin()-1; iter_back--){
			if (init==1){
				double prob_temp = -DBL_MAX;
				int index_temp_temp = index_temp;
				for (vector<Information>::iterator iter_last=iter_back->begin() ; iter_last!=iter_back->end() ; iter_last++){
					if (iter_last->prob > prob_temp){
						index_temp = index_temp_temp;
						prob_temp = iter_last->prob;
					}
					index_temp_temp += 1;
				}
				init=0;
			}
			
			
			revised_iter = revisedText.begin();
			revisedText.insert(revised_iter," ");
			revised_iter = revisedText.begin();
			revisedText.insert(revised_iter, (*iter_back)[index_temp].word );
			
			//cout << (*iter_back)[index_temp].word << " ";
			index_temp = (*iter_back)[index_temp].bestindex;
		}
		revised_iter = revisedText.begin();
		for (vector<string>::iterator revised_iter=revisedText.begin() ; revised_iter!=revisedText.end(); revised_iter++){
			cout << (*revised_iter) ;
		}
		cout << "</s>" << endl;
		
	} 
	return 0;
}
