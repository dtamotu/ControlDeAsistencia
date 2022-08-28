#ifndef __PROGRAM_H__
#define __PROGRAM_H__
#include <string>
#include <opencv2/opencv.hpp>

#include "seam_carving.h"
#include "image_matrix.h"
#include "converter.h"
class Program{
    private:
        std::string direction;
        cv::Mat image;
        Image_Matrix matrix; //esto se representa como si fuera un polo
        Seam_Carving seam_carving; //esto se representa como si fuera una maquina 
        Converter converter;

    public:
        Program();
        Program(std::string);
        void showImage();
        void setImage(std::string);
        void reduceH(int);
        void reduceV(int);
        void convertToSVG(int);
        void convertToSVG(int,int,int);
};

Program::Program(){
    direction = "";
}

Program::Program(std::string direction){
    //puedes colocar directamente la direccion en esta parte
    //setImage("D:/John/silueta_polo.png");
    setImage(direction);
}

void Program::showImage(){
    image=matrix.convertToMatReturn();
    if(image.empty()){
        std::cout<<"No hay imagen"<<std::endl;
    }else{
        cv::imshow("Imagen",image);
        cv::waitKey(0);
    }
}
void Program::setImage(std::string newDirection){
    this->direction = newDirection;
    image=cv::imread(direction);
    matrix=Image_Matrix(image);
    seam_carving.setImage(matrix);
}

void Program::reduceH(int value){
    seam_carving.reduceHorizontal(matrix,value);
}
void Program::reduceV(int value){
    seam_carving.reduceVertical(matrix,value);
}
void Program::convertToSVG(int posx,int posy,int umbral){
    converter.setImage(matrix);
    converter.BFS(posx,posy,umbral);
    converter.printContour();
    std::string svg= converter.convertToSVG();
    std::cout<<"Copia el siguiendo codigo a html"<<std::endl;
    std::cout<<svg<<std::endl;
}
void Program::convertToSVG(int umbral){
    converter.setImage(matrix);
    converter.BFS(matrix.getRows()/2,matrix.getCols()/2,umbral);
    std::string svg= converter.convertToSVG();
    std::cout<<"Copia el siguiendo codigo a html"<<std::endl;
    std::cout<<svg<<std::endl;
}
#endif