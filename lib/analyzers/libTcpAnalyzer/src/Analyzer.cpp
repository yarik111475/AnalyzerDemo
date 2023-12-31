#include "Analyzer.h"
#include <QLabel>
#include <QWidget>

QWidget *Analyzer::standardWidget()
{
    return new QLabel("TcpAnalyzer");
}

QWidget *Analyzer::extendedWidget()
{

}

void Analyzer::handle()
{

}
