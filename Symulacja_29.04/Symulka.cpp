#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <vector>
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

class BaseStation{

    public:
        int _ID;
        int _RBlocks;
        int _H;
        int _L;
        int _DisconnectedUsers;
        int _DisconnectedUsersTemp;
        float _RunTime;
        float _SleepTime;
        float _Blocked;
        float _UserPerSecond;
        float _PowerConsumed;
        float _PowerRunning;
        float _PowerSleeping;
        bool _Is_full;
        bool _Is_asleep;
        bool _ConnectionPassed;
        bool _overloading;
        bool _canGoToSleep;
        bool _passed_to_neighbour_1;
        bool _passed_to_neighbour_2;
        BaseStation* Neighbour_1;
        BaseStation* Neighbour_2;
        ExponentialDistribution* gen_exponential;
        list<float> ResourceBlocks;

        BaseStation(int id, 
                    int rblocks, 
                    float user_per_second,
                    ExponentialDistribution* gen_exp,
                    float _h = 0.8,
                    float _l = 0.2,
                    bool is_full = false, 
                    bool is_asleep = false,
                    bool connection_passed = false,
                    bool can_go_to_sleep = false)
            {
            this->_ID = id;
            this->_RBlocks = rblocks;
            this->_H = rblocks*_h;
            this->_L = rblocks*_l;
            this->_UserPerSecond = user_per_second;
            this->_Is_full = is_full;
            this->_Is_asleep = is_asleep;
            this->_ConnectionPassed = connection_passed;
            this->_canGoToSleep = can_go_to_sleep;
            this->_DisconnectedUsers = 0;
            this->_DisconnectedUsersTemp = 0;
            this->_passed_to_neighbour_1 = false;
            this->_passed_to_neighbour_2 = false;
            this->_RunTime = 0.0;
            this->_SleepTime = 0.0;
            this->_Blocked = 0.0;
            this->_PowerConsumed = 0.0;
            this->_PowerRunning = 200;
            this->_PowerSleeping = 1;
            this->gen_exponential = gen_exp;
            
        }

        void addNeighbours(BaseStation* neighbour_1, BaseStation* neighbour_2){
            Neighbour_1 = neighbour_1;
            Neighbour_2 = neighbour_2;
        }

        void updateRunTime(float t){

            if(this->_Blocked > 0){
                if(t >= this->_Blocked){
                    this->_Blocked = 0;
                }else if(t < this->_Blocked){
                    this->_Blocked -= t;
                }
            }

            if(this->_Blocked==0){
                if(this->_Is_asleep == true){
                   this->_SleepTime += t;
                }else{
                    this->_RunTime += t;
                }

                this->calculateConsumedPower();
            }
            
        }

        void calculateConsumedPower(){
            float running_status_power = (this->_RunTime/1000) * _PowerRunning;
            float sleeping_status_power = (this->_SleepTime/1000) * _PowerSleeping;

            this->_PowerConsumed = (running_status_power + sleeping_status_power);
        }

        void changeStatus(){
            this->_PowerConsumed += 1000;

            if(this->_Is_asleep == true){
                this->_Is_asleep = false;
                 
            }else if(this->_Is_asleep == false){
                this->_Is_asleep = true;
                this->_Blocked = 50;
            }
            
            this->_canGoToSleep = false;

        }

        bool connect(int u, bool firstUsage = true){
            _ConnectionPassed = false;
            bool connected_status;

            if (_Is_full == false && _Is_asleep == false && this->_Blocked == 0){
                ResourceBlocks.push_back(u);
                if(ResourceBlocks.size()==_RBlocks){
                    _Is_full=true;
                }else if(ResourceBlocks.size()>_H){
                    _overloading = true;
                    
                    if(Neighbour_1->_Is_asleep == true){
                        this->wakeUpNeighbour(Neighbour_1);
                    }else if(Neighbour_2->_Is_asleep == true){
                        this->wakeUpNeighbour(Neighbour_2);
                    }

                }else if(ResourceBlocks.size()>_L && _canGoToSleep == false){
                    _canGoToSleep = true;
                }
                return true;
            }else{
                _ConnectionPassed = true;

                if(_passed_to_neighbour_1 == false){
                    connected_status = connectToNeighbour(Neighbour_1,u);

                    _passed_to_neighbour_1 = true;
                    _passed_to_neighbour_2 = false;

                }else if(_passed_to_neighbour_2 == false){

                    connected_status = connectToNeighbour(Neighbour_2,u);

                    _passed_to_neighbour_1 = false;
                    _passed_to_neighbour_2 = true;
                }
                

                if(connected_status == false && firstUsage == true){
                    _DisconnectedUsers += 1;
                }
                return connected_status;
            }
            
        }

        bool connectToNeighbour(BaseStation* neighbour ,int u){
            if(neighbour->_ConnectionPassed == false){
                return neighbour->connect(u, false);
            }
            return false;
            
        }

        int disconnect(){

            ResourceBlocks.remove(0);
            if(ResourceBlocks.size()<_RBlocks){_Is_full=false;}

            return ResourceBlocks.size();
        }

        int reduceRB(float next_hop){

            if(ResourceBlocks.empty()){
                //cout << "Resources empty" << endl;
                return 0;
            }
            else if(this->_Blocked == 0){

                list<float>::iterator it;
                for (it = ResourceBlocks.begin(); it != ResourceBlocks.end(); ++it){
                    
                    if(*it <= next_hop){
                        *it = 0;
                    }else{
                        *it -= next_hop;
                    }
                }

                int rb_size = disconnect();

                if(_canGoToSleep == true && rb_size<_L){
                    this->sleepWell();
                }else if(this->_overloading == true && rb_size <= _H){
                    this->_overloading = false;
                }

                return rb_size;

            }else{
                return 0;
            }
                
        }

        void sleepWell(){
            if (ResourceBlocks.empty()){
                this->changeStatus();
                return;
            }
            else{
                
                list<float>::iterator it;
                int rb_size = ResourceBlocks.size();
                int i = 0;

                int neighbour_1_space = 0;
                int neighbour_2_space = 0;

                if (Neighbour_1->_Is_asleep == false && Neighbour_1->_overloading == false && Neighbour_1->_Is_full == false){
                    neighbour_1_space = Neighbour_1->_H - Neighbour_1->ResourceBlocks.size();
                }
                
                if (Neighbour_2->_Is_asleep == false && Neighbour_2->_overloading == false && Neighbour_2->_Is_full == false){
                    neighbour_2_space = Neighbour_2->_H - Neighbour_2->ResourceBlocks.size();
                }

                if (neighbour_1_space <= 0 && neighbour_2_space <= 0){
                    return;
                }else if( rb_size <= (neighbour_1_space + neighbour_2_space)){

                    for (it = ResourceBlocks.begin(); it != ResourceBlocks.end(); ++it){
                        
                        if(i < (rb_size/2)){
                            if(neighbour_1_space>0){
                                neighbour_1_space -=1;
                                Neighbour_1->connect(*it);
                            }else if(neighbour_2_space>0){
                                neighbour_2_space -=1;
                                Neighbour_2->connect(*it);
                            }
                        }else{
                            if(neighbour_2_space>0){
                                neighbour_2_space -=1;
                                Neighbour_2->connect(*it);
                            }else if(neighbour_1_space>0){
                                neighbour_1_space -=1;
                                Neighbour_1->connect(*it);
                            }
                        }
                        

                        ++i;
                    
                    }
                    
                    
                    this->ResourceBlocks = {};
                    this->changeStatus();


                }

                return;

                

            }

        }

        void wakeUpNeighbour(BaseStation* neighbour){

            neighbour->changeStatus();

            int rb_size = this->ResourceBlocks.size();
            int i = 0;

            while(i<=(rb_size/2)){
                
                float user = ResourceBlocks.back();
                bool connected_status = neighbour->connect(user);
                if(connected_status==true){
                    ResourceBlocks.pop_back();
                }

                ++i;
            }
            neighbour->_Blocked = 50;
            

            return;
        }

        int generateUser(float time_ms){
            // float lambda = getLambda(time_ms);
            // if(lambda != 0){
            //     float next_user_arrival = 1000/lambda;
            //     return (int)next_user_arrival;
            // }else{
            //     return 0;
            // }

            float lambda = getLambda(time_ms);
            if(lambda != 0){
                int next_user_arrival = (int)gen_exponential->get(lambda);
                return next_user_arrival;
            }else{
                return 0;
            }

        }

        float getLambda(float time_ms){
            // lambda
                // 8h - 1/2 lb - 0-8
                // 6h - 3/4 lb - 8-14
                // 4h -  1  lb - 14-18
                // 6h - 3/4 lb - 18-24

            int day_ms = 86400000;
            float h_8 = day_ms/3;
            float h_8_6 = h_8+day_ms/4; // 6h = 21 600 000
            float h_8_6_4 = h_8_6+day_ms/6;

            if(time_ms < h_8){
                return _UserPerSecond/2;
            }else if(h_8 <= time_ms && time_ms < h_8_6){
                return _UserPerSecond*3/4;
            }else if(h_8_6 <= time_ms && time_ms < h_8_6_4){
                return _UserPerSecond;
            }else if(h_8_6_4 <= time_ms && time_ms < day_ms){
                return _UserPerSecond*3/4;
            }else{
                return 0;
            }
        }


};


class NetworkSimulation{

    public:
        int _ID;
        UniformDistribution* gen_uniform;
        
    
        NetworkSimulation(int id, UniformDistribution* gen_uni){
            this->_ID = id;
            this->gen_uniform = gen_uni;
        }
    
        int runMainLoop(BaseStation* bs_1, BaseStation* bs_2, BaseStation* bs_3){
            float time_ms = 0.0;
            int next_user_arrival = 0;
            bool connected;
            int mi_ms;
            int disconnected;
            int user_1 = bs_1->generateUser(time_ms);
            int user_2 = bs_2->generateUser(time_ms);
            int user_3 = bs_3->generateUser(time_ms);

            // float time_h = (time_ms / 3.6) * 0.001 *     0.001;
            // while (time_ms<=86400000)
            int day_ms = 86400000;
            while (time_ms<=day_ms)
            {

                next_user_arrival = min({user_1,user_2,user_3});
                user_1 -= next_user_arrival;
                user_2 -= next_user_arrival;
                user_3 -= next_user_arrival;

                // 4. Skok do nastepnego uzytkownika
                time_ms += next_user_arrival;
                bs_1->updateRunTime(next_user_arrival);
                bs_2->updateRunTime(next_user_arrival);
                bs_3->updateRunTime(next_user_arrival);

                // 1. Wyczyszczenie uzytkownikow - redukcja blokow
                bs_1->reduceRB(next_user_arrival);
                bs_2->reduceRB(next_user_arrival);
                bs_3->reduceRB(next_user_arrival);

                // 2. Wczytywanie użytkowników

                // prototypowy generator mi 1000 - 30000 ms
                // srand(time(0));
                // int max_mi_ms=30;
                // int min_mi_ms=1;
                // mi_ms = rand() % (max_mi_ms-min_mi_ms+1) + min_mi_ms;
                // mi_ms *= 1000;
                // generator end

                mi_ms = (int)gen_uniform->get();


                if(user_1 == 0){
                    bs_1->connect(mi_ms);
                }else if(user_2 == 0){
                    bs_2->connect(mi_ms);
                }else if(user_3 == 0){
                    bs_3->connect(mi_ms);
                }
                

                // 3. Generowanie użytkownika

                if(user_1 == 0){
                    user_1 = bs_1->generateUser(time_ms);
                }else if(user_2 == 0){
                    user_2 = bs_2->generateUser(time_ms);
                }else if(user_3 == 0){
                    user_3 = bs_3->generateUser(time_ms);
                }
                
                
            }
            
            disconnected = bs_1->_DisconnectedUsers + bs_2->_DisconnectedUsers + bs_3->_DisconnectedUsers;
            return disconnected;
        }

        int findMin(list<int> lst){
            list<int>::iterator it;

            int min = 0;
            for(it=lst.begin(); it!=lst.end(); ++it){
                if(*it != 0 && min > *it){
                    min = *it;
                }
            }

            return min;
        }

        void reduce(int reducer, list<int> lst){

            
            if (lst.empty()){
                //cout << "Resources empty" << endl;
                return;
            }
            else{

                list<int>::iterator it;
                for (it = lst.begin(); it != lst.end(); ++it){                    
                    *it -= reducer;
                }
                
                return;
            }
        }

        
  
};

int main() {

    int rb = 65;
    float user_per_second = (float)10;
    float seed = 1;
    int min_mi = 1000;
    int max_mi = 30000;

    UniformDistribution gen_uniform(min_mi, max_mi, seed);
    ExponentialDistribution bs_1_gen_exponential(seed);
    ExponentialDistribution bs_2_gen_exponential(seed+22);
    ExponentialDistribution bs_3_gen_exponential(seed+33);

    NetworkSimulation net_1(1, &gen_uniform);
    BaseStation bs_1(1, rb, 1, &bs_1_gen_exponential);
    BaseStation bs_2(2, rb, user_per_second, &bs_2_gen_exponential);
    BaseStation bs_3(3, rb, user_per_second, &bs_3_gen_exponential);

    bs_1.addNeighbours(&bs_2, &bs_3);
    bs_2.addNeighbours(&bs_3, &bs_1);
    bs_3.addNeighbours(&bs_1, &bs_2);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int disc = net_1.runMainLoop(&bs_1,&bs_2,&bs_3);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << ">> /'.runMainLoop/' Time difference = " << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
    printf("disconnected users: %d\n", disc);


    // Badanie 




    return 0;
}