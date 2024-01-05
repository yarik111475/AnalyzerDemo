#include "MainWindow.h"
#include "analyzers/AnalyzerStorage.h"
#include "analyzers/dialogs/AnalyzerDialog.h"
#include "analyzers/Defines_p.h"

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

    menuBar()->addMenu(fileMenuPtr);
    menuBar()->addMenu(analyzersMenuPtr_);
    updateAnalyzersActions();
}

void MainWindow::updateAnalyzersActions()
{
    analyzersMenuPtr_->clear();
    analyzersMenuPtr_->addAction(QObject::tr("Settings"),[&](){
        AnalyzerDialog dialog {this};
        QObject::connect(&dialog,&QDialog::finished,
                         [this](){
            updateAnalyzersActions();
        });
        dialog.exec();
    });

    const auto viewsContainer {analyzerStorage_.getViews()};
    std::for_each(viewsContainer.begin(),viewsContainer.end(),
                  [&](const std::tuple<QString,QString,QString,QString>& dataTuple){
        const auto analyzerName {std::get<TupleFields::Name>(dataTuple)};
        analyzersMenuPtr_->addAction(analyzerName);
    });
}

MainWindow::MainWindow(std::shared_ptr<QSettings> appSettingsPtr, QWidget *parent)
    : QMainWindow(parent),
      analyzersMenuPtr_{new QMenu(QObject::tr("Analyzers"))},
      mdiAreaPtr_{new QMdiArea},
      analyzerStorage_{AnalyzerStorage::instance()},
      appSettingsPtr_{appSettingsPtr}
{
    makeMenu();
    setCentralWidget(mdiAreaPtr_);
    resize(800,600);
}
