/**
        CSVsmith
------------------------
* - Name           CSVsmith
* - Description    This is a simple CSV file viewer
* - Author         Youssef Ahmed Mahmoud Elebiary
* - Version        1.0
*/



#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setFixedSize(800, 600);
    w.show();
    return a.exec();
}
