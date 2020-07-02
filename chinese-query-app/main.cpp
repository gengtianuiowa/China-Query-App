#include "mainwindow.h"
#include <QApplication>
#include <cspfile.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   MainWindow w;
    w.show();
//    CSpFile* FILE = new CSpFile;
//    if (FILE->LoadFile())
//        cout<<"Loading Complete!"<<endl;
    return a.exec();
}
