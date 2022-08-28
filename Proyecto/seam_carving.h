#ifndef __SEAM_CARVING_H__
#define __SEAM_CARVING_H__
#include <opencv2/opencv.hpp>
#include <vector>
#include <queue>
#include <utility>
#include "image_matrix.h"
#include "label.h"
#include "queue_label.h"

class Seam_Carving{
    private:
    int rows;
    int cols;

    std::vector<std::vector<Label>> label_matrix;
    std::vector<std::vector<float>> energy_matrix;
    int way=0;
    public:
    Seam_Carving();
    Seam_Carving(Image_Matrix &);
    void setImage(Image_Matrix&);
    Queue_Label makeHorizontalDijkstra();
    Queue_Label makeVerticalDijkstra();
    void eliminatePath(Queue_Label,Image_Matrix&);
    void cleanLabel_Matrix();
    void reduceHorizontal(Image_Matrix&,int);
    void reduceVertical(Image_Matrix&,int);
};

Seam_Carving::Seam_Carving(){
    this->rows = 0;
    this->cols = 0;
}

Seam_Carving::Seam_Carving(Image_Matrix &matrix){
    this->rows = matrix.getRows();
    this->cols = matrix.getCols();
    for(int i=0;i<matrix.getRows();i++){
        label_matrix.push_back({});
        for(int j=0;j<matrix.getCols();j++){
            label_matrix[i].push_back(Label(matrix.calculateEnergy(i,j)));
        }
    }
}

void Seam_Carving::setImage(Image_Matrix &matrix){
    label_matrix.clear();
    energy_matrix.clear();
    for(int i=0;i<matrix.getRows();i++){
        energy_matrix.push_back({});
        label_matrix.push_back({});
        for(int j=0;j<matrix.getCols();j++){
            energy_matrix[i].push_back(matrix.calculateEnergy(i,j));
            label_matrix[i].push_back(Label());
        }
    }
}

Queue_Label Seam_Carving::makeHorizontalDijkstra(){
    this->way=1;
    std::priority_queue<Queue_Label> queue;
    for(int i=0;i<energy_matrix.size();i++){
        queue.push(Queue_Label(i,0,label_matrix[i][0].total_energy));
    }
    while(!queue.empty()){
        Queue_Label label=queue.top();
        queue.pop();
        label_matrix[label.x][label.y].marked=true;
        if(label.y == energy_matrix[0].size()-1){
            return label;
        }
        //Realizando la conexion a los tres nodos cercanos
        //Nodo 1
        label_matrix[label.x][label.y+1].predecessorX=label.x;
        label_matrix[label.x][label.y+1].predecessorY=label.y;
        label_matrix[label.x][label.y+1].total_energy+=energy_matrix[label.x][label.y];
        queue.push(Queue_Label(label.x,label.y+1,label_matrix[label.x][label.y+1].total_energy));
        //Nodo 2
        if(label.x-1>=0 && label_matrix[label.x-1][label.y+1].total_energy==0){
            label_matrix[label.x-1][label.y+1].predecessorX=label.x;
            label_matrix[label.x-1][label.y+1].predecessorY=label.y;
            label_matrix[label.x-1][label.y+1].total_energy+=energy_matrix[label.x][label.y];
            queue.push(Queue_Label(label.x-1,label.y+1,label_matrix[label.x-1][label.y+1].total_energy));
        }
        //Nodo 3
        if(label.x+1<energy_matrix.size() && label_matrix[label.x+1][label.y+1].total_energy==0){
            label_matrix[label.x+1][label.y+1].predecessorX=label.x;
            label_matrix[label.x+1][label.y+1].predecessorY=label.y;
            label_matrix[label.x+1][label.y+1].total_energy+=energy_matrix[label.x][label.y];
            queue.push(Queue_Label(label.x+1,label.y+1,label_matrix[label.x+1][label.y+1].total_energy));
        }
    }
}

Queue_Label Seam_Carving::makeVerticalDijkstra(){
    this->way=0;
    std::priority_queue<Queue_Label> queue;
    for(int i=0;i<energy_matrix[0].size();i++){
        queue.push(Queue_Label(0,i,label_matrix[0][i].total_energy));
    }
    while(!queue.empty()){
        Queue_Label label=queue.top();
        queue.pop();
        label_matrix[label.x][label.y].marked=true;
        if(label.x == energy_matrix.size()-1){
            return label;
        }
        //Realizando la conexion a los tres nodos cercanos
        //Nodo 1
        label_matrix[label.x+1][label.y].predecessorX=label.x;
        label_matrix[label.x+1][label.y].predecessorY=label.y;
        label_matrix[label.x+1][label.y].total_energy+=energy_matrix[label.x][label.y];
        queue.push(Queue_Label(label.x+1,label.y,label_matrix[label.x+1][label.y].total_energy));
        //Nodo 2
        if(label.y-1>=0 && label_matrix[label.x-1][label.y+1].total_energy==0){
            label_matrix[label.x+1][label.y-1].predecessorX=label.x;
            label_matrix[label.x+1][label.y-1].predecessorY=label.y;
            label_matrix[label.x+1][label.y-1].total_energy+=energy_matrix[label.x][label.y];
            queue.push(Queue_Label(label.x+1,label.y-1,label_matrix[label.x+1][label.y-1].total_energy));
        }
        //Nodo 3
        if(label.y+1<energy_matrix[0].size() && label_matrix[label.x+1][label.y+1].total_energy==0){
            label_matrix[label.x+1][label.y+1].predecessorX=label.x;
            label_matrix[label.x+1][label.y+1].predecessorY=label.y;
            label_matrix[label.x+1][label.y+1].total_energy+=energy_matrix[label.x][label.y];
            queue.push(Queue_Label(label.x+1,label.y+1,label_matrix[label.x+1][label.y+1].total_energy));
        }
    }
}

void Seam_Carving::eliminatePath(Queue_Label q_label,Image_Matrix& matrix){
    std::pair<int,int> tmp(q_label.x,q_label.y);
    Label label;
    std::vector<std::pair<int,int>> path;
    do{
        //eliminacion horizontal es diferente de vertical
        //Eliminación Horizontal
        if(way==1){
            for(int i=tmp.first;i<energy_matrix.size()-2;i++){
                energy_matrix[i][tmp.second]=energy_matrix[i+1][tmp.second];
                matrix.at(i,tmp.second)=matrix.at(i+1,tmp.second);
            }
            path.push_back(std::pair<int,int>(tmp.first,tmp.second));
            cols--;
        }else{//Eliminacion Vertical
            matrix.erase(tmp.first,tmp.second);
            energy_matrix[tmp.first].erase(energy_matrix[tmp.first].begin()+tmp.second);
            rows--;
        }
        
        //actualizar nodo
        label=label_matrix[tmp.first][tmp.second];
        tmp=std::pair<int,int>(label.predecessorX,label.predecessorY);
    }while(tmp.first!=-1 && tmp.second!=-1);

    if(way==1){
        matrix.erase(matrix.getRows()-1);
        energy_matrix.erase(energy_matrix.begin()+energy_matrix.size()-1);
    }

    //actualizar la energia
    if(way==1){
        for(int i=0;i<path.size();i++){
            energy_matrix[path[i].first][path[i].second]=matrix.calculateEnergy(path[i].first,path[i].second);
            if(path[i].first-1>=0){
                energy_matrix[path[i].first-1][path[i].second]=matrix.calculateEnergy(path[i].first-1,path[i].second);
            }
        }
    }else{
        for(int i=0;i<path.size();i++){
            energy_matrix[path[i].first][path[i].second]=matrix.calculateEnergy(path[i].first,path[i].second);
            if(path[i].second-1>=0){
                energy_matrix[path[i].first][path[i].second-1]=matrix.calculateEnergy(path[i].first-1,path[i].second-1);
            }
        }
    }
}

void Seam_Carving::cleanLabel_Matrix(){
    label_matrix.clear();
    for(int i=0;i<energy_matrix.size();i++){
        label_matrix.push_back({});
        for(int j=0;j<energy_matrix[i].size();j++){
            label_matrix[i].push_back(Label());
        }
    }
}

void Seam_Carving::reduceHorizontal(Image_Matrix& matrix,int n){
    for(int i=0;i<n;i++){
        Queue_Label label=makeHorizontalDijkstra();
        eliminatePath(label,matrix);
    }
}

void Seam_Carving::reduceVertical(Image_Matrix& matrix,int n){
    for(int i=0;i<n;i++){
        Queue_Label label=makeVerticalDijkstra();
        eliminatePath(label,matrix);
    }
}

#endif