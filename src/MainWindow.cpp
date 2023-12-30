#include "MainWindow.h"
#include "analyzers/AnalyzerLoader.h"
#include "analyzers/AnalyzerSettings.h"
#include "analyzers/dialogs/AnalyzerDialog.h"

#include <QMenu>
#include <QAction>
#include <QMenuBar>
#include <QMdiArea>

void MainWindow::makeMenu()
{
    QMenu* fileMenuPtr {new QMenu(QObject::tr("File"))};
    fileMenuPtr->addAction(QObject::tr("Open"));
    fileMenuPtr->addAction(QObject::tr("Save"));
    fileMenuPtr->addSeparator();
    fileMenuPtr->addAction(QObject::tr("Quit"));

    QMenu* analyzersMenuPtr {new QMenu(QObject::tr("Analyzers"))};
    analyzersMenuPtr->addAction(QObject::tr("Settings"),[&](){
        AnalyzerDialog dialog {this};
        const auto result {dialog.exec()};
        if(result==QDialog::Accepted){
        }
    });

    menuBar()->addMenu(fileMenuPtr);
    menuBar()->addMenu(analyzersMenuPtr);
}

MainWindow::MainWindow(std::shared_ptr<QSettings> appSettingsPtr, QWidget *parent)
    : QMainWindow(parent),
      mdiAreaPtr_{new QMdiArea},
      analyzerLoader_{AnalyzerLoader::instance()},
      analyzerSettings_{AnalyzerSettings::instance()},
      appSettingsPtr_{appSettingsPtr}
{
    makeMenu();
    setCentralWidget(mdiAreaPtr_);
    resize(800,600);
}
