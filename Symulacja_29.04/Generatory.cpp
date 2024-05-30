#include <iostream>
#include <cstdio>
#include <math.h>
#include <stdio.h>
#include <fstream>
using namespace std;

class Generator{
    
    public:
    float M;
    float A;
    float Q;
    float R;
    int Seed;

    Generator(int seed,float m = 2147483647.0, float a = 16807.0, float q = 127773.0, float r = 2836.0 ){
        this->M = m;
        this->A = a;
        this->Q = q;
        this->R = r;
        this->Seed = seed;
    }

    float rand(){
        float h = floor(this->Seed/this->Q);
        this->Seed = this->A * (this->Seed - this->Q * h) - this->R * h;
        if(this->Seed < 0){
            this->Seed += this->M;
        }

        return this->Seed/this->M;
    }


};

class UniformDistribution: private Generator{

    public:
    int Min;
    int Max;
    
    UniformDistribution(int min, int max, int seed): Generator{seed}{
        this->Min = min;
        this->Max = max;
    }

    float get(){
        return Generator::rand() * (this->Max - this->Min) + this->Min;
    }

};

class ExponentialDistribution: public Generator{

    public:
    ExponentialDistribution(int seed): Generator{seed}{};

    float get(float lamb){
        float k = Generator::rand();
        return -(1000/lamb)*log(k);
    }
};


int main(){

    float seed = 5;
    // float seed = 100;
    // float seed = 1500;
    int min_mi = 1000;
    int max_mi = 30000;

    UniformDistribution gen_uni(min_mi, max_mi, seed);
    ExponentialDistribution gen_exp_1(seed);
    ExponentialDistribution gen_exp_2(seed+22);
    ExponentialDistribution gen_exp_3(seed+33);

    
    
    ofstream MyFile("wykresy/exponential_1_seed_5.txt");

    int res = (int)gen_exp_1.get(1);
    for(int i=0; i<1000; i++){
        MyFile << res;
        MyFile << "\n";
        res = (int)gen_exp_1.get(1);
    }
    
    MyFile.close();
    
    ofstream MyFile_2("wykresy/exponential_2_seed_5.txt");

    res = (int)gen_exp_2.get(1);
    for(int i=0; i<1000; i++){
        MyFile_2 << res;
        MyFile_2 << "\n";
        res = (int)gen_exp_2.get(1);
    }
    
    MyFile_2.close();

    ofstream MyFile_3("wykresy/exponential_3_seed_5.txt");

    res = (int)gen_exp_3.get(1);
    for(int i=0; i<1000; i++){
        MyFile_3 << res;
        MyFile_3 << "\n";
        res = (int)gen_exp_3.get(1);
    }
    
    MyFile_3.close();


    return 0;
}

