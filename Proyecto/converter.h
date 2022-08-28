#ifndef __CONVERTER_H__
#define __CONVERTER_H__

#include "image_matrix.h"
#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>
#include <utility>
#include <string>
#include "pixel.h"

class Marker{
    public:
    int passed;//para marcar aquellos que pasan el BFS
    int contour;//para marcar el contorno
    int marked;//para marca contorno pero por recorrido
    Marker(){
        passed=0;
        contour=0;
        marked=0;
    }
};

class Converter{
    private:
    std::vector<std::vector<Marker>> marked_matrix;
    Image_Matrix matrix;
    std::pair<int,int> findContour();
    std::pair<int,int> findNextPath(std::pair<int,int>);
    public:
    Converter();
    void setImage(Image_Matrix&);
    void BFS(int,int,int);
    void printContour();
    std::string convertToSVG();
    void DFS(std::pair<int,int>);
    
};


double differenceEnergy(Pixel x,Pixel y){
    int distance=0;
    distance+=(pow(x.B-y.B,2));
    distance+=(pow(x.G-y.G,2));
    distance+=(pow(x.R-y.R,2));
    distance=sqrt(distance);
    return distance;
}

Converter::Converter(){
}

void Converter::setImage(Image_Matrix& matrix){
    this->matrix=matrix;
    marked_matrix.clear();
    for(int i=0;i<matrix.getRows();i++){
        marked_matrix.push_back({});
        for(int j=0;j<matrix.getCols();j++){
            marked_matrix[i].push_back(Marker());
        }
    }
}

void Converter::BFS(int i,int j,int umbral){
    //std::cout<<i<<" - "<<j<<std::endl;
    std::queue<std::pair<int,int>> queue;
    marked_matrix[i][j].passed=true;
    queue.push(std::pair<int,int>(i,j));
    std::pair<int,int> helper;
    while(!queue.empty()){
        helper=queue.front();
        queue.pop();
        if(helper.first==0 || helper.second==0 || helper.first==marked_matrix.size()-1 || helper.second==marked_matrix[0].size()-1){

        }else{
            if(marked_matrix[helper.first+1][helper.second].passed==false && differenceEnergy(matrix.at(helper.first,helper.second),matrix.at(helper.first+1,helper.second))<umbral){
                marked_matrix[helper.first+1][helper.second].passed=true;
                queue.push(std::pair<int,int>(helper.first+1,helper.second));
            }
            if(marked_matrix[helper.first-1][helper.second].passed==false && differenceEnergy(matrix.at(helper.first,helper.second),matrix.at(helper.first-1,helper.second))<umbral){
                marked_matrix[helper.first-1][helper.second].passed=true;
                queue.push(std::pair<int,int>(helper.first-1,helper.second));
            }
            if(marked_matrix[helper.first][helper.second+1].passed==false && differenceEnergy(matrix.at(helper.first,helper.second),matrix.at(helper.first,helper.second+1))<umbral){
                marked_matrix[helper.first][helper.second+1].passed=true;
                queue.push(std::pair<int,int>(helper.first,helper.second+1));
            }
            if(marked_matrix[helper.first][helper.second-1].passed==false && differenceEnergy(matrix.at(helper.first,helper.second),matrix.at(helper.first,helper.second-1))<umbral){
                marked_matrix[helper.first][helper.second-1].passed=true;
                queue.push(std::pair<int,int>(helper.first,helper.second-1));
            }
        }
    }
}

void Converter::printContour(){
    cv::Mat image(marked_matrix.size(),marked_matrix[0].size(),matrix.getType());
    for(int i=0;i<marked_matrix.size();i++){
        for(int j=0;j<marked_matrix[0].size();j++){
            if(marked_matrix[i][j].passed==true){
                image.at<cv::Vec3b>(i,j)=cv::Vec3b(matrix.getB(i,j),matrix.getG(i,j),matrix.getR(i,j));
            }else{
                image.at<cv::Vec3b>(i,j)=cv::Vec3b(255,255,0);
            }
            if(marked_matrix[i][j].contour==true){
                image.at<cv::Vec3b>(i,j)=cv::Vec3b(0,255,255);
            }
        }
    }
    cv::imshow("Imagen con contorno",image);
    cv::waitKey(0);
}

std::pair<int,int> Converter::findContour(){
    std::pair<int,int> lastContour;
    for(int i=1;i<marked_matrix.size()-1;i++){
        for(int j=1;j<marked_matrix[i].size()-1;j++){
            if(marked_matrix[i][j].passed==true){
                if(marked_matrix[i+1][j].passed==false ||marked_matrix[i-1][j].passed==false||marked_matrix[i][j+1].passed==false||marked_matrix[i][j-1].passed==false){
                    marked_matrix[i][j].contour=true;
                    lastContour=std::pair<int,int>(i,j);
                }
            }
        }
    }
    printContour();
    return lastContour;
}

std::pair<int,int> Converter::findNextPath(std::pair<int,int> init){
    std::queue<std::pair<int,int>> queue;
    queue.push(init);
    int i=0;
    while(i<12){
        std::pair<int,int> tmp=queue.front();
        queue.pop();
        if(marked_matrix[tmp.first+1][tmp.second].marked==false){
            if(marked_matrix[tmp.first+1][tmp.second].contour==true){
                return std::pair<int,int>(tmp.first+1,tmp.second);
            }else{
                queue.push(std::pair<int,int>(tmp.first+1,tmp.second));
            }
        }
        if(marked_matrix[tmp.first-1][tmp.second].marked==false){
            if(marked_matrix[tmp.first-1][tmp.second].contour==true){
                return std::pair<int,int>(tmp.first-1,tmp.second);
            }else{
                queue.push(std::pair<int,int>(tmp.first-1,tmp.second));
            }
        }
        if(marked_matrix[tmp.first][tmp.second+1].marked==false){
            if(marked_matrix[tmp.first][tmp.second+1].contour==true){
                return std::pair<int,int>(tmp.first,tmp.second+1);
            }else{
                queue.push(std::pair<int,int>(tmp.first,tmp.second+1));
            }
        }
        if(marked_matrix[tmp.first][tmp.second-1].marked==false){
            if(marked_matrix[tmp.first][tmp.second-1].contour==true){
                return std::pair<int,int>(tmp.first,tmp.second-1);
            }else{
                queue.push(std::pair<int,int>(tmp.first,tmp.second-1));
            }
        }
        i++;
    }
    return std::pair<int,int>(-1,-1);
    
}

std::string Converter::convertToSVG(){
    std::pair<int,int> lastContour=findContour();
    std::string svg="<svg height='"+std::to_string(marked_matrix.size()) +"' width='"+std::to_string(marked_matrix[0].size())+"'>\n<path class=\"imagen\" d='M"+std::to_string(lastContour.second)+" "+std::to_string(lastContour.first)+" ";
    marked_matrix[lastContour.first][lastContour.second].marked=true;
    std::pair<int,int> tmp=findNextPath(lastContour);
    while(tmp.first!=-1 && tmp.second!=-1){
        marked_matrix[tmp.first][tmp.second].marked=true;
        svg+="L"+std::to_string(tmp.second)+" "+std::to_string(tmp.first)+" ";
        tmp=findNextPath(tmp);
    }
    svg+="Z'/>\n</svg>";
    return svg;
}
#endif
