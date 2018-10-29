
g++ train.cpp -o train

./train $1 model_init.txt seq_model_01.txt model_01.txt
cat model_01.txt
./train $1 model_init.txt seq_model_02.txt model_02.txt
cat model_02.txt
./train $1 model_init.txt seq_model_03.txt model_03.txt
cat model_03.txt
./train $1 model_init.txt seq_model_04.txt model_04.txt
cat model_04.txt
./train $1 model_init.txt seq_model_05.txt model_05.txt
cat model_05.txt

g++ test.cpp -o test
./test modellist.txt testing_data1.txt result1.txt
./test modellist.txt testing_data2.txt result2.txt

g++ cal_acc.cpp -o cal_acc
./cal_acc result1.txt testing_answer.txt acc.txt

cat acc.txt
