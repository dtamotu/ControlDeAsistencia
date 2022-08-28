
#ifndef __QUEUE_LABEL_H__
#define __QUEUE_LABEL_H__

class Queue_Label{
    public:
    int x;
    int y;
    float total_energy;
    Queue_Label(){
        this->x=0; this->y=0;
        this->total_energy=1000;     
    }
    Queue_Label(int x, int y,int total_energy){
        this->x=x;
        this->y=y;
        this->total_energy=total_energy;
    }
};

#endif
bool operator<(const Queue_Label& p1, const Queue_Label& p2){
    return p1.total_energy < p2.total_energy;
}