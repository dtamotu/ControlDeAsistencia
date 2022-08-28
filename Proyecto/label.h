#ifndef __LABEL_H__
#define __LABEL_H__


class Label{
    public:
    //float energy;
    float total_energy;
    int predecessorX;
    int predecessorY;
    bool marked;
    Label(){
        total_energy=0;
        predecessorX=-1;
        predecessorY=-1;
        marked=false;
        
    }
    Label(int energy){
        total_energy=0;
        predecessorX=0;
        predecessorY=0;
        marked=false;
    }
};


#endif