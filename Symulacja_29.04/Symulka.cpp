#include <chrono>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <algorithm>
#include <vector>
using namespace std;


class BaseStation{

    public:
        int _ID;
        int _RBlocks;
        int _H;
        int _L;
        int _DisconnectedUsers;
        int _DisconnectedUsersTemp;
        float _UserPerSecond;
        bool _Is_full;
        bool _Is_asleep;
        bool _ConnectionPassed;
        bool _overloading;
        bool _canGoToSleep;
        BaseStation* Neighbour_1;
        BaseStation* Neighbour_2;
        list<float> ResourceBlocks;

        BaseStation(int id, 
                    int rblocks, 
                    float user_per_second,
                    float _h = 0.8,
                    float _l = 0.2,
                    bool is_full = false, 
                    bool is_asleep = false,
                    bool connection_passed = false,
                    bool can_go_to_sleep = false)
            {
            _ID = id;
            _RBlocks = rblocks;
            _H = rblocks*_h;
            _L = rblocks*_l;
            _UserPerSecond = user_per_second;
            _Is_full = is_full;
            _Is_asleep = is_asleep;
            _ConnectionPassed = connection_passed;
            _canGoToSleep = can_go_to_sleep;
            _DisconnectedUsers = 0;
            _DisconnectedUsersTemp = 0;
            
        }

        void addNeighbours(BaseStation* neighbour_1, BaseStation* neighbour_2){
            Neighbour_1 = neighbour_1;
            Neighbour_2 = neighbour_2;
        }

        bool connect(int u, bool firstUsage = true){
            _ConnectionPassed = false;

            if (_Is_full == false && _Is_asleep == false){
                ResourceBlocks.push_back(u);
                if(ResourceBlocks.size()==_RBlocks){
                    _Is_full=true;
                }else if(ResourceBlocks.size()>_H){
                    _overloading = true;
                    // TODO BUDZENIE INNYCH
                }else if(ResourceBlocks.size()>_L && _canGoToSleep == false){
                    _canGoToSleep = true;
                }
                return true;
            }else{
                _ConnectionPassed = true;
                bool connected_status = connectToNeighbour(Neighbour_1,u);

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

            if (ResourceBlocks.empty()){
                //cout << "Resources empty" << endl;
                return 0;
            }
            else{

                list<float>::iterator it;
                for (it = ResourceBlocks.begin(); it != ResourceBlocks.end(); ++it){
                    
                    if(*it < next_hop || *it == next_hop){
                        *it = 0;
                    }else{
                        *it -= next_hop;
                    }
                }

                int rb_size = disconnect();

                if(_canGoToSleep == true && rb_size<_L){
                    this->sleepWell();
                }

                return rb_size;

            }
                
        }

        void sleepWell(){
            if (ResourceBlocks.empty()){
                this->_Is_asleep=true;
                this->_canGoToSleep = false;
                return;
            }
            else{
                
                list<float>::iterator it;
                int rb_size = ResourceBlocks.size();
                int i = 0;

                int neighbour_1_space = 0;
                int neighbour_2_space = 0;

                if (Neighbour_1->_Is_asleep == false && Neighbour_1->_overloading == false){
                    neighbour_1_space = Neighbour_1->_H - Neighbour_1->ResourceBlocks.size();
                }
                
                if (Neighbour_2->_Is_asleep == false && Neighbour_2->_overloading == false){
                    neighbour_2_space = Neighbour_2->_H - Neighbour_2->ResourceBlocks.size();
                }

                if (neighbour_1_space <= 0 && neighbour_2_space <= 0){
                    return;
                }else if( rb_size <= (neighbour_1_space + neighbour_2_space)){

                    for (it = ResourceBlocks.begin(); it != ResourceBlocks.end(); ++it){
                    
                        if(i < (rb_size/2) && neighbour_1_space>0){
                            neighbour_1_space -=1;
                            Neighbour_1->connect(*it);
                        }else if(neighbour_2_space>0){
                            neighbour_2_space -=1;
                            Neighbour_2->connect(*it);
                        }

                        ++i;
                    
                    }
                    
                    this->_Is_asleep=true;
                    this->ResourceBlocks = {};
                    this->_canGoToSleep = false;


                }

                return;

                

            }

        }

        int generateUser(float time_ms){
            float lambda = getLambda(time_ms);
            if(lambda != 0){
                float next_user_arrival = 1000/lambda;
                return (int)next_user_arrival;
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
        
    
        NetworkSimulation(int id){
            _ID = id;
            
        }
    
        int runMainLoop(BaseStation* bs_1, BaseStation* bs_2, BaseStation* bs_3){
            float time_ms = 0.0;
            int next_user_arrival = 0;
            bool connected;
            int mi_ms;
            int disconnected = 0;
            int user_1 = bs_1->generateUser(time_ms);
            int user_2 = bs_2->generateUser(time_ms);
            int user_3 = bs_3->generateUser(time_ms);

            // float time_h = (time_ms / 3.6) * 0.001 *     0.001;
            // while (time_ms<=86400000)
            int x = 86400000;
            while (time_ms<=x)
            {

                next_user_arrival = min({user_1,user_2,user_3});
                user_1 -= next_user_arrival;
                user_2 -= next_user_arrival;
                user_3 -= next_user_arrival;

                // 4. Skok do nastepnego uzytkownika    
                time_ms += next_user_arrival;
                // cout<< time_ms/1000000 << " " << mi_ms << " " << bs_1->ResourceBlocks.size() << " " << bs_2->ResourceBlocks.size() << " " << bs_3->ResourceBlocks.size() << endl;

                // 1. Wyczyszczenie uzytkownikow - redukcja blokow
                bs_1->reduceRB(next_user_arrival);
                bs_2->reduceRB(next_user_arrival);
                bs_3->reduceRB(next_user_arrival);

                // 2. Wczytywanie użytkowników

                // TODO generator mi
                srand(time(0));
                int max_mi_ms=30;
                int min_mi_ms=1;
                mi_ms = rand() % (max_mi_ms-min_mi_ms+1) + min_mi_ms;
                mi_ms *= 1000;
                // TODOend

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

    NetworkSimulation net_1(1);
    BaseStation bs_1(1, rb, user_per_second);
    BaseStation bs_2(2, rb, user_per_second);
    BaseStation bs_3(3, rb, user_per_second);

    bs_1.addNeighbours(&bs_2, &bs_3);
    bs_2.addNeighbours(&bs_3, &bs_1);
    bs_3.addNeighbours(&bs_1, &bs_2);

    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    int disc = net_1.runMainLoop(&bs_1,&bs_2,&bs_3);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    cout << ">> /'.runMainLoop/' Time difference = " << chrono::duration_cast<chrono::seconds>(end - begin).count() << "[s]" << endl;
    disc = bs_1._DisconnectedUsers+bs_2._DisconnectedUsers+bs_3._DisconnectedUsers;
    printf("disconnected users: %d\n", disc);


    // Badanie SleepWell
    // bs_1.connect(5);
    // bs_1.connect(10);
    // bs_1.connect(150);
    // bs_1.connect(200);
    // bs_1.connect(250);
    
    // bs_2._Is_full=true;
    // bs_3._Is_full=true;

    // bs_1.reduceRB(5);
    // bs_1.reduceRB(10);



    return 0;
}
// cd 