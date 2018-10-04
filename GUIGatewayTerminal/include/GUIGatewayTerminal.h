#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGroupBox>
#include <QLabel>

#include <GUI/GlobalDefinitions.h>
#include <Parameters.h>
#include <Log.h>

namespace Ui {
class GUIGatewayTerminal;
}

class GUIGatewayTerminal : public QMainWindow
{
public:
    explicit GUIGatewayTerminal(std::string routeToConfigFile = "", QWidget *parent = 0);
    virtual ~GUIGatewayTerminal();
    
private:
    void loadParameters(std::string routeToConfigFile);
    
    bool addPredefinedServices();
    
    bool    on_;
};

#endif // MAINWINDOW_H
