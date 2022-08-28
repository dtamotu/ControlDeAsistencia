#include <iostream>
#include <opencv2/opencv.hpp>
#include "program.h"
using namespace cv;
using namespace std;

void menu(){
    cout << " **********************************************" << endl;
    cout << " ******************** MENU ********************" << endl;
    cout << " **********************************************" << endl;
    cout << " 1 - MOSTRAR IMAGEN" << endl;
    cout << " 2 - CAMBIAR IMAGEN" << endl;
    cout << " 3 - REDUCIR IMAGEN HORIZONTALMENTE" << endl;
    cout << " 4 - REDUCIR IMAGEN VERTICALMENTE" << endl;
    cout << " 5 - CONVERTIR A FORMATO SVG" << endl;
    cout << " 6 - SALIR" << endl;
}

int main(int, char**) {
    int option = 0; // Variable auxiliar para almacenar las opciones
    string direction;
    int value;
    cout << " -> INGRESE LA DIRECCION DE UNA IMAGEN: "; getline(cin,direction);
    Program program(direction);
    menu();
    do{
        cout << " **********************************************" << endl;
        cout << " -> INGRESE OPCION: "; cin >>option;cin.ignore();
            switch (option){
            case 1:
                cout << " -> SE ESTA MOSTRANDO LA IMAGEN\n";
                program.showImage();
                break;
            case 2:
                cout << " -> INGRESE LA NUEVA DIRECCION DE LA IMAGEN: "; getline(cin,direction);
                program.setImage(direction);
                break;
            case 3:
                cout << " -> INGRESE CUANTOS PIXELES DESEA REDUCIR HORIZONTALMENTE: "; cin>>value;cin.ignore();
                program.reduceH(value);
                break;
            case 4:
                cout<< " -> INGRESE CUANTOS PIXELES DESEA REDUCIR VERTICALMENTE: " ; cin>>value;cin.ignore();
                program.reduceV(value);
                break;
            case 5:
                //cout<<" -> INGRESE UN PIXEL DE LA FIGURA QUE DESEA CONVERTIR A SVG: \n";
                //cout<<" -> INGRESE EL VALOR EN X: "; cin>>value;
                //cout<<" -> INGRESE EL VALOR EN Y: "; cin>>value2;
                cout<<" -> INGRESE EL VALOR DEL UMBRAL(recomendado 20): "; cin>>value;
                program.convertToSVG(value);
                break;
            case 6:
                break;
            default:
                break;
        }
    } while (option!=6);
    return 0;
}
