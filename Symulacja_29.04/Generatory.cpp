#include <iostream>
#include <cstdio>
#include <math.h>
#include <stdio.h>
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

    Generator gieniek_1(1);
    Generator gieniek_2(2);
    // int min = 18;
    // int max = 20;

    // float result = gieniek.rand(5660)*(max-min)+min;

    UniformDistribution uni(1000,30000,500);
    ExponentialDistribution ex(5);

    float f = 0;
    for (int i=0;i<1000;i++){
        f += ex.get(2);
    }

    f = f/1000;

    return 0;
}

