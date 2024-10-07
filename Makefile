all: branch_misprediction btb_test

branch_misprediction: branch_misprediction.cpp
	g++ -O2 -march=native -std=c++11 -o branch_misprediction branch_misprediction.cpp

btb_test: btb_test.cpp
	g++ -O2 -march=native -std=c++11 -o btb_test btb_test.cpp
