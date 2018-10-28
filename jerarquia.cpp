#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <fstream>
#include <string>
#include <iomanip>
#define PI 3.14159265
using namespace std;


class pJerarquia{
    private:

        float probabilidad[16];
        float suma;
        float max;
        float valesp[16];
        int individuosB[16][4];
        int individuosE[16];
        int individuosBC[16][4];
        int individuosBD[16][4];
        float aptitud[16];
        int padres[16];
        int contador;
        int miembros;
        int genes;
        int pcruza;
        int mutar;
    public:
        pJerarquia(int,int,int,int,int);
        int generaciones;
        float mayores[100];
        float menores[100];
        void creaIndividuos();
        void convBinario(int,int);
        void calcAptitud();
        float fitness(float);
        void calcProbabilidad();
        void creaJerarquia();
        void valEsp();
        void ruleta();
        void cruza();
        void mutacion();
        void maxmin();
        int convEntero(int);
        float calcProbabilidadD(int);
        void reasignar();
        void getmaxmin();
        void Tabla1(int);
        void TablaJerarquia(int);
        void Tabla2(int);
        void Tabla3(int);
        float getSuma(float*);
        int getMaxI(int*);
        int getMinI(int*);
        float getMaxF(float*);
        float getMinF(float*);
        float getMayores(int);
        float getMenores(int);

};

pJerarquia::pJerarquia(int imiembros,int igenes,int igeneraciones,int ipcruza,int imutar){
    miembros = imiembros;
    genes = igenes;
    generaciones = igeneraciones;
    pcruza = ipcruza;
    mutar = imutar;
    contador=0;
}

void pJerarquia::creaIndividuos(){
    int limite=0,random;
    int individuos[16];
    for(int i=0;i<genes;i++){
        limite += pow(2,i);
    }
    for(int i=0;i<miembros;i++){
        random =  rand() % limite + 1;//numero para el individuo
        individuosE[i]= random; //Individuos enteros es el "x"
        convBinario(random,i);
    }
}

void pJerarquia::convBinario(int numero,int valori){
    int dividendo, resto, divisor = 2,cont = 1,j=genes-1;
    for(int i=0;i<genes;i++){
        individuosB[valori][i] = 0; //Binario de los enteros
    }
    dividendo = numero;
    while(dividendo >= divisor){
    resto = dividendo % 2;
    if(resto == 1)
        individuosB[valori][j] = 1;
    else
       individuosB[valori][j] = 0;
       dividendo = dividendo/divisor;
       cont++;
       j--;
    }
    if(dividendo == 1)
       individuosB[valori][j] = 1;
    else
       individuosB[valori][j] = 0;
}

void pJerarquia::calcAptitud(){
    suma = 0;
    for(int i=0;i<miembros;i++){
        aptitud[i] = fitness(float(individuosE[i]));
        suma += aptitud[i];
    }
}
float pJerarquia::fitness(float individuo){
    float res=0;
    res= float(individuo-5)/float(2+sin(individuo*PI/180));
    if(res<0)
        return res*-1;
    else
        return res;
}
void pJerarquia::calcProbabilidad(){
    for(int i=0;i<miembros;i++)
        probabilidad[i] = aptitud[i]/(float)suma;
}

void pJerarquia::creaJerarquia(){
    float tempap=0,temppr=0;
    int tempe=0,tempb[genes];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<(miembros-1);j++){
            if(aptitud[j] > aptitud[j+1]){
                //con esto movemos la aptitud
                tempap = aptitud[j];
                aptitud[j] = aptitud[j+1];
                aptitud[j+1] = tempap;
                //con esto movemos la probabilidad
                temppr = probabilidad[j];
                probabilidad[j] = probabilidad[j+1];
                probabilidad[j+1] = temppr;
                //con esto movemos los enteros
                tempe = individuosE[j];
                individuosE[j] = individuosE[j+1];
                individuosE[j+1] = tempe;
                //con esto movemos los binarios
                for(int k=0;k<genes;k++){
                    tempb[k] = individuosB[j][k];
                }
                for(int k=0;k<genes;k++){
                    individuosB[j][k] = individuosB[j+1][k];
                }
                for(int k=0;k<genes;k++){
                    individuosB[j+1][k] = tempb[k];
                }
            }
        }
    }
}

void pJerarquia::valEsp(){
    float max=1.1,min = 2-max,N=miembros;
    for(int i=0;i<miembros;i++){
        valesp[i] = min-(max-min)*(i/(float)(N-1));
    }
}


void pJerarquia::ruleta(){
    float sum=0,flecha;
    float limites[17];
    limites[0] = 0;
    for(int i=1;i<miembros;i++){
        sum += valesp[i];
        limites[i] = sum;
    }
    for(int j=0;j<miembros;j++){
        flecha=(float)rand()/((float)RAND_MAX/sum);
        for(int i=0;i<miembros;i++){
            if(limites[i]<flecha && flecha<=limites[i+1]){
                padres[j] = i;
            }
        }
    }
}

void pJerarquia::cruza(){
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            individuosBC[i][j] = individuosB[padres[i]][j];
        }
    }
    for(int i=0;i<miembros-1;i++){
        for(int k=0;k<pcruza;k++){
            individuosBD[i][k] = individuosBC[i][k];
            individuosBD[i+1][k] = individuosBC[i+1][k]; //Individuos binarios descendencia
        }
        for(int j=pcruza;j<genes;j++){
            individuosBD[i][j] = individuosBC[i+1][j];
            individuosBD[i+1][j] = individuosBC[i][j];
        }
        i++;
    }
}

void pJerarquia::mutacion(){
    int imutar,gmutar;

    for(int i=0;i<mutar;i++){
        imutar = rand() % (miembros-1) + 1;
        for(int j=0;j<genes;j++){
            if(individuosBD[imutar][j] == 0){
                individuosBD[imutar][j] = 1;
                break;
            }
        }

    }
}
void pJerarquia::maxmin(){
    int tmax,tmin,temp;
    tmax = convEntero(0);
    tmin = convEntero(0);
    for(int i=1;i<miembros;i++){
        temp = convEntero(i);
        if(temp > tmax){
            tmax = 	temp;
        }
        if(temp < tmin){
            tmin = temp;
        }
    }
    mayores[contador] = calcProbabilidadD(tmax);
    menores[contador] = calcProbabilidadD(tmin);
    contador++;
}

int pJerarquia::convEntero(int conv){
    int regresa = 0,exponente=0;
    for(int i=genes-1;i>=0;i--){
        if(individuosBD[conv][i] == 1){
            regresa += pow(2,exponente);
        }
        exponente++;
    }
    return regresa;
}


float pJerarquia::calcProbabilidadD(int num){
    int sum=0;
    for(int i=0;i<miembros;i++){
        sum += convEntero(i);
    }
    return num/(float)sum;
}

void pJerarquia::reasignar(){
    //individuosB,individuosE,individuosBD
    //individuosBD -> individuosB;
    //individuosBD -> convEntero ->individuosE;
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            individuosB[i][j] = individuosBD[i][j];
        }
        individuosE[i] = convEntero(i);
    }
}

void pJerarquia::getmaxmin(){
    for(int i=0;i<generaciones;i++){
        cout << "max: " << mayores[i] << endl;
        cout << "min: " << menores[i] << endl;
        cout << endl;
    }
}


void pJerarquia::Tabla1(int generacion){
    ofstream archivo;
    archivo.open("Tabla1.txt",ios::app);
    archivo << generacion << "a generacion | Tabla 1" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "No. | Poblacion Inicial | Valor x | Aptitud |Probabilidad"<<endl;
    archivo <<"___________________________________________________________"<<endl;
    string binario[miembros];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            if(individuosB[i][j]==0)
                binario[i]+='0';
            else
                binario[i]+='1';
        }
    }
    for(int i = 0; i < miembros;i++){
        archivo<<(i+1)<<setw(4)<< "|"<<setw(12)<< binario[i]<< setw(8)<<"|"<< setw(5)<<individuosE[i]<<setw(5)<<"|"<<setw(7)<<aptitud[i]<<setw(3) <<"|"<< setw(5)<<probabilidad[i]<<endl;
    }
    float promedio= 1/ (float)miembros;
    archivo <<"___________________________________________________________"<<endl;
    archivo<<"Suma"<<setw(32)<<"|"<<setw(8)<<suma<<setw(2)<<"|"<<"1"<<endl;
    archivo<<"Promedio"<<setw(28)<<"|"<<setw(8)<<(suma/miembros)<<setw(2)<<"|"<<promedio<<endl;
    archivo<<"Max"<<setw(33)<<"|"<<setw(8)<<getMaxF(aptitud)<<setw(2)<<"|"<<getMaxF(probabilidad)<<endl;
    archivo<<"Min"<<setw(33)<<"|"<<setw(8)<<getMinF(aptitud)<<setw(2)<<"|"<<getMinF(probabilidad)<<endl;
    archivo<<endl<<endl;
    archivo.close();
}

void pJerarquia::TablaJerarquia(int generacion){
    ofstream archivo;
    archivo.open("TablaJerarquia.txt",ios::app);
    archivo << generacion << "a generacion | Tabla de Jerarquía" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "Jerarquías |   Aptitud   | Valesp"<<endl;
    archivo <<"___________________________________________________________"<<endl;
    for(int i = 0; i < miembros;i++){
        archivo<<(i+1)<<setw(12)<< "|"<<aptitud[i]<<setw(5)<<"|"<< valesp[i]<<endl;
    }
    archivo <<"___________________________________________________________"<<endl;
    archivo<<"Suma"<<setw(32)<<"|"<<miembros<<endl;

    archivo<<endl<<endl;
    archivo.close();
}


void pJerarquia::Tabla2(int generacion){ //Se usan las variables individuosBC e individuosBD
    ofstream archivo;
    archivo.open("Tabla2.txt",ios::app);
    archivo << generacion << "a generacion | Tabla 2" << endl;
    archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
    archivo << "No. |    Cruza    |Pto. de cruza| Descendencia | Valor x | Aptitud "<<endl;
    archivo << "______________________________________________________________________"<<endl;
    string binarioC[miembros], binarioD[miembros];
    for(int i=0;i<miembros;i++){
        for(int j=0;j<genes;j++){
            if(individuosBC[i][j]==0)
                binarioC[i]+='0';
            else
                binarioC[i]+='1';

            if(individuosBD[i][j]==0)
                binarioD[i]+='0';
            else
                binarioD[i]+='1';

        }
    }
    float binD[miembros];
    for(int i = 0; i < miembros;i++){
        cout<<binarioC[i]<<endl;
        archivo<<(i+1)<<setw(4)<< "|"<<setw(10)<< binarioC[i]<< setw(4)<<"|"<<setw(6) <<pcruza<<setw(7)<<" |"<<setw(10)<<binarioD[i]<<setw(6)<<"|"<<setw(5)<<convEntero(i)<<setw(5)<<"|"<<setw(5)<<(convEntero(i)*convEntero(i))<<endl; //SE TIENE QUE OBTENER EL VALOR
        binD[i]=convEntero(i)*convEntero(i);
    }
    float promedio= getSuma(binD)/ (float)miembros;
    archivo << "______________________________________________________________________"<<endl;
    archivo<<"Suma"<<setw(55)<<"|"<<setw(8)<<getSuma(binD)<<endl;
    archivo<<"Promedio"<<setw(51)<<"|"<<setw(8)<<promedio<<endl;
    archivo<<"Max"<<setw(56)<<"|"<<setw(8)<<getMaxF(binD)<<endl;
    archivo<<"Min"<<setw(56)<<"|"<<setw(8)<<getMinF(binD)<<endl;
    archivo<<endl<<endl;
    archivo.close();
}



void pJerarquia::Tabla3(int generacion){ //Se tiene que mutar aqui porque el arreglo individuosBD se modifica directamente
    if(generacion==1 || generacion==generaciones){
        ofstream archivo;
        archivo.open("Tabla3.txt",ios::app);
        archivo << generacion << "a generacion | Tabla 3" << endl;
        archivo << "Aptitud F(x)=abs|(x-5)/(2+Sen(x))|"<<endl;
        archivo << "No. | Descendencia | Mutacion | Valor x | Aptitud "<<endl;
        archivo << "____________________________________________________"<<endl;
        string binarioD[miembros], binarioDM[miembros];
        for(int i=0;i<miembros;i++){
            for(int j=0;j<genes;j++){
                if(individuosBD[i][j]==0)
                    binarioD[i]+='0';
                else
                    binarioD[i]+='1';

            }
        }
        mutacion();
        for(int i=0;i<miembros;i++){
            for(int j=0;j<genes;j++){
                if(individuosBD[i][j]==0)
                    binarioDM[i]+='0';
                else
                    binarioDM[i]+='1';

            }
        }

        float binD[miembros];
        for(int i = 0; i < miembros;i++){
            archivo<<(i+1)<<setw(4)<< "|"<<setw(10)<< binarioD[i]<< setw(4)<<"|"<<setw(6)<<binarioDM[i]<<setw(6)<<"|"<<setw(5)<<convEntero(i)<<setw(5)<<"|"<<setw(5)<<(convEntero(i)*convEntero(i))<<endl; //SE TIENE QUE OBTENER EL VALOR
            binD[i]=convEntero(i)*convEntero(i);
        }


        float promedio= getSuma(binD)/ (float)miembros;
        archivo << "____________________________________________________"<<endl;
        archivo<<"Suma"<<setw(38)<<"|"<<setw(8)<<getSuma(binD)<<endl;
        archivo<<"Promedio"<<setw(34)<<"|"<<setw(8)<<promedio<<endl;
        archivo<<"Max"<<setw(39)<<"|"<<setw(8)<<getMaxF(binD)<<endl;
        archivo<<"Min"<<setw(39)<<"|"<<setw(8)<<getMinF(binD)<<endl;
        archivo<<endl<<endl;
        archivo.close();
    }else{
        mutacion();
    }
}

float pJerarquia::getSuma(float *aux){
    float suma=0;
    for(int i=0;i<miembros;i++)
        suma+=aux[i];
    return suma;
}

int pJerarquia::getMaxI(int *aux){
    int max=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] > max)
            max = aux[i];
    return max;
}

int pJerarquia::getMinI(int *aux){
    int min=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] < min)
            min = aux[i];
    return min;
}

float pJerarquia::getMaxF(float *aux){
    float max=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] > max)
            max = aux[i];
    return max;
}

float pJerarquia::getMinF(float *aux){
    float min=aux[0];
    for(int i=0;i<miembros;i++)
        if(aux[i] < min)
            min = aux[i];
    return min;
}

float pJerarquia::getMayores(int indice){
    return mayores[indice];
}
float pJerarquia::getMenores(int indice){
    return menores[indice];
}
