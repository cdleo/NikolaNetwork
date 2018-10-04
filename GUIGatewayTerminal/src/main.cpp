/*
 * File:   main.cpp
 * Author: cleo
 *
 * Created on October 14, 2014, 12:46 PM
 */

#include <QApplication>
#include <iostream>
#include <stdlib.h>
#include <GUIGatewayTerminal.h>

int main(int argc, char *argv[]) {
    
    QApplication app(argc, argv);
    
    std::string logName("GUIGatewayTerminal");
    LOG->initialize( logName );  
    LOG->INFO("***********************************************");
    LOG->INFO("********** Inicio GUIGatewayTerminal **********");
    LOG->INFO("***********************************************");    
    
    GUIGatewayTerminal* terminal;
    if (argc == 2) {
        terminal = new GUIGatewayTerminal(argv[1]);
    } else if (argc == 1) {
        terminal = new GUIGatewayTerminal();
    } else {
        std::cout << "Invalid parameters! Usage: ./GUIGatewayTerminal <routeToConfigFile>" << std::endl;
        exit(0);
    }
    
    app.setStartDragDistance(10);       //10pixels
    app.setStartDragTime(1000);         //1s
    
    terminal->show();
    int result = app.exec();
    
    delete terminal;
    
    LOG->INFO("GUIGatewayTerminal exitting OK!");
    
    return result;
}
