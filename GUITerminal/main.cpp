/*
 * File:   main.cpp
 * Author: cleo
 *
 * Created on July 22, 2014, 2:37 PM
 */

#include <QApplication>

#include "mainwindow.h"

int main(int argc, char *argv[])
{
    //Q_INIT_RESOURCE(application);

    QApplication app(argc, argv);
    app.setOrganizationName("QtProject");
    app.setApplicationName("Application Example");
    MainWindow mainWin;
    mainWin.show();
    return app.exec();
}
