#include "jerarquia.cpp"
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QValueAxis>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <QBarSet>
#include <QBarSeries>
QT_CHARTS_USE_NAMESPACE
using namespace std;
int main(int argc, char *argv[]){

    QApplication a(argc, argv);
    //Clase para graficar
    srand(time(nullptr));
    int gn=0;
    cout<<"Ingresa el numero de generaciones: ";
    cin>>gn;
    pJerarquia r(16,4,gn,2,5); //(int miembros,int genes,int generaciones,int pcruza,int mutar)
    for(int i=0;i<r.generaciones;i++){
        r.creaIndividuos();
        r.calcAptitud();
        r.calcProbabilidad();
        r.creaJerarquia();
        r.valEsp();

        if(i==0 || i==(r.generaciones-1)){
            r.TablaJerarquia(i+1);
            r.Tabla1(i+1);
        }
        r.ruleta();
        r.cruza();
        if(i==0 || i==(r.generaciones-1))
            r.Tabla2(i+1);
        r.Tabla3(i+1);
        r.maxmin();
        r.reasignar();
    }
    cout<<endl<<"Archivos generados correctamente con los nombres: Tabla1.txt,TablaJerarquia.txt, Tabla2.txt, Tabla3.txt y Tabla4.txt";
    QLineSeries *series = new QLineSeries();
    QLineSeries *series2 = new QLineSeries();
    cout<<endl<<"Maximos"<<endl<<endl;
    for(int i=1;i<=r.generaciones;i++){
        cout<<r.getMayores(i-1)<<endl;
        series->append(i,r.getMayores(i-1));
        series2->append(i,r.getMenores(i-1));
    }
    cout<<endl<<"Minimos"<<endl<<endl;
    for(int i=0;i<r.generaciones;i++)
        cout<<r.getMenores(i)<<endl;

    //Para mostrar los datos necesitamos una instancia de QChart
    QChart *chart = new QChart();
    QChartView *chartView = new QChartView(chart);
    chart->setTitle("Práctica 5: Selección por jerarquía");

    chart->legend()->show(); //muestra la linea del color cuando ponemos show para saber cual se está referenciando

    series->setName("Máximos");
    series2->setName("Mínimos");

    chartView->chart()->addSeries(series);
    chartView->chart()->addSeries(series2);

    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(1, r.generaciones);
    axisX->setTickCount(10);
    chartView->chart()->setAxisX(axisX, series);

    QValueAxis *axisX2 = new QValueAxis;
    axisX2->setRange(1, r.generaciones);
    axisX2->setTickCount(10);
    chartView->chart()->setAxisX(axisX2, series2);

    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 1);
    axisY->setTickCount(20);
    chartView->chart()->setAxisY(axisY, series);



    QValueAxis *axisY2 = new QValueAxis;
    axisY2->setRange(0, 1);
    axisY2->setTickCount(20);
    chartView->chart()->setAxisY(axisY2,series2);

    //Agregamos a un chartview la instancia de qchart

    chartView->setRenderHint(QPainter::Antialiasing);
    //Para mostrarlo en ventana
    QMainWindow window;
    window.setCentralWidget(chartView);
    window.resize(600, 400);
    window.show();
    return a.exec();
}
