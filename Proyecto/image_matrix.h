#ifndef __IMAGE_MATRIX_H__
#define __IMAGE_MATRIX_H__

#include <vector>
#include <opencv2/opencv.hpp>
#include "pixel.h"
class Image_Matrix{
    private:
    std::vector<std::vector<Pixel>> matrix;
    cv::Mat image;
    int type;
    public:
    Image_Matrix();
    Image_Matrix(cv::Mat&);
    Pixel& at(int,int); 
    int getB(int,int);
    int getG(int,int);
    int getR(int,int);
    int getRows();
    int getCols();
    int getType();
    float calculateEnergy(int,int);
    void erase(int);
    void erase(int,int);
    void  convertToMat();
    cv::Mat convertToMatReturn();
};

Image_Matrix::Image_Matrix(){
    
}

Image_Matrix::Image_Matrix(cv::Mat &image){
    this->type=image.type();
    this->image=image;
    int rows=image.rows;
    int cols=image.cols;
    //inicializacion de la matriz
    for(int i=0;i<image.rows;i++){
        matrix.push_back({});
        for(int j=0;j<image.cols;j++){
            matrix[i].push_back(Pixel(i,j,image.at<cv::Vec3b>(i,j)[0],image.at<cv::Vec3b>(i,j)[1],image.at<cv::Vec3b>(i,j)[2]));
        }
    }
}

Pixel& Image_Matrix::at(int i ,int j){
    return matrix[i][j];
}

int Image_Matrix::getB(int i, int j){
    return image.at<cv::Vec3b>(i,j)[0];
}
int Image_Matrix::getG(int i, int j){
    return image.at<cv::Vec3b>(i,j)[1];
}
int Image_Matrix::getR(int i, int j){
    return image.at<cv::Vec3b>(i,j)[2];
}
int Image_Matrix::getRows(){
    return matrix.size();
    //return image.rows;
}
int Image_Matrix::getCols(){
    return matrix[0].size();
    //return image.cols;
}
int Image_Matrix::getType(){
    return type;
}
float Image_Matrix::calculateEnergy(int i,int j){
    if(i==0 || j==0 || i==getRows()-1 || j==getCols()-1){
        return 1000;
    }
    int differB=matrix[i+1][j].B-matrix[i-1][j].B;
    int differG=matrix[i+1][j].G-matrix[i-1][j].G;
    int differR=matrix[i+1][j].R-matrix[i-1][j].R;
    double gradienteX=pow(differB,2)+pow(differG,2)+pow(differR,2);
    differB=matrix[i][j+1].B-matrix[i][j-1].B;
    differG=matrix[i][j+1].G-matrix[i][j-1].G;
    differR=matrix[i][j+1].R-matrix[i][j-1].R;
    double gradienteY=pow(matrix[i+1][j].B-matrix[i-1][j].B,2);
    return sqrt(gradienteX+gradienteY);
}
void Image_Matrix::erase(int i){
    matrix.erase(matrix.begin()+i);
}
void Image_Matrix::erase(int i,int j){
    matrix[i].erase(matrix[i].begin()+j);
}
void Image_Matrix::convertToMat(){
    image=cv::Mat(matrix.size(),matrix[0].size(),image.type());
    for(int i=0;i<matrix.size();i++){
        for(int j=0;j<matrix[0].size();j++){
            image.at<cv::Vec3b>(i, j)[0] = matrix[i][j].B;
            image.at<cv::Vec3b>(i, j)[1] = matrix[i][j].G;
            image.at<cv::Vec3b>(i, j)[2] = matrix[i][j].R;
        }
    }
}
cv::Mat Image_Matrix::convertToMatReturn(){
    cv::Mat newImage(matrix.size(),matrix[0].size(),image.type());
    for(int i=0;i<matrix.size();i++){
        for(int j=0;j<matrix[0].size();j++){
            newImage.at<cv::Vec3b>(i, j)[0] = matrix[i][j].B;
            newImage.at<cv::Vec3b>(i, j)[1] = matrix[i][j].G;
            newImage.at<cv::Vec3b>(i, j)[2] = matrix[i][j].R;
        }
    }
    return newImage;
}
#endif