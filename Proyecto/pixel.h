#ifndef __PIXEL_H__
#define __PIXEL_H__

class Pixel{
    public:
    int x,y;
    int B,G,R;
    Pixel(){
        x=0;y=0;
        B=0;G=0;R=0;
    }
    Pixel(int x,int y,int B,int G,int R){
        this->x=x;this->y=y;this->B=B;this->G=G;this->R=R;
    }
};

#endif