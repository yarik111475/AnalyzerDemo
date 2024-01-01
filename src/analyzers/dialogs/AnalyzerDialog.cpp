#include "AnalyzerDialog.h"
#include "analyzers/IAnalyzer.h"
#include "analyzers/AnalyzerModel.h"
#include "analyzers/AnalyzerStorage.h"
#include "analyzers/dialogs/AnalyzerViewDialog.h"

#include <QFrame>
#include <QLabel>
#include <QLibrary>
#include <QLineEdit>
#include <QGroupBox>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QStackedWidget>
#include <QApplication>
#include <algorithm>

#include <QDebug>

void AnalyzerDialog::resetSettingsWidget(int index)
{
    stackedWidgetPtr_->setCurrentIndex(index);
}

AnalyzerDialog::AnalyzerDialog(QWidget *parent)
    : QDialog{parent},
      analyzerModelPtr_{new AnalyzerModel},
      analyzerStorage_{AnalyzerStorage::instance()},
      analyzersComboBoxPtr_{new QComboBox},
      analyzersViewButtonPtr_{new QPushButton(QObject::tr("..."))},
      matrixLineEditPtr_{new QLineEdit},
      matrixAddButtonPtr_{new QPushButton(QObject::tr("..."))},
      settingsVBoxLayoutPtr_{new QVBoxLayout},
      stackedWidgetPtr_{new QStackedWidget}
{
    QGridLayout* analyzersGridLayoutPtr {new QGridLayout};
    analyzersGridLayoutPtr->addWidget(new QLabel(QObject::tr("Analyzer:")),0,0);
    analyzersGridLayoutPtr->addWidget(analyzersComboBoxPtr_,1,0,1,3);
    analyzersGridLayoutPtr->addWidget(analyzersViewButtonPtr_,1,4,1,1);
    QObject::connect(analyzersViewButtonPtr_,&QPushButton::clicked,
                     this,&AnalyzerDialog::analyzersViewSlot);

    analyzersGridLayoutPtr->addWidget(new QLabel(QObject::tr("Response matrix:")),2,0);
    analyzersGridLayoutPtr->addWidget(matrixLineEditPtr_,3,0,1,3);
    analyzersGridLayoutPtr->addWidget(matrixAddButtonPtr_,3,4,1,1);
    QObject::connect(matrixAddButtonPtr_,&QPushButton::clicked,
                     this,&AnalyzerDialog::matrixAddSlot);

    QGroupBox* analyzersGroupBoxPtr {new QGroupBox(QObject::tr("Analyzers"))};
    analyzersGroupBoxPtr->setLayout(analyzersGridLayoutPtr);

    QGroupBox* settingsGroupBoxPtr {new QGroupBox(QObject::tr("Settings"))};
    settingsVBoxLayoutPtr_->addWidget(stackedWidgetPtr_);
    settingsGroupBoxPtr->setLayout(settingsVBoxLayoutPtr_);

    QVBoxLayout* vboxLayoutPtr {new QVBoxLayout};
    vboxLayoutPtr->addWidget(analyzersGroupBoxPtr,0);
    vboxLayoutPtr->addWidget(settingsGroupBoxPtr,5);

    QPushButton* acceptButtonPtr {new QPushButton(QObject::tr("Ok"))};
    QObject::connect(acceptButtonPtr,&QPushButton::clicked,[&](){
        accept();
    });
    QPushButton* rejectButtonPtr {new QPushButton(QObject::tr("Cancel"))};
    QObject::connect(rejectButtonPtr,&QPushButton::clicked,[&](){
        reject();
    });

    QHBoxLayout* hboxLayoutPtr {new QHBoxLayout};
    hboxLayoutPtr->addStretch(5);
    hboxLayoutPtr->addWidget(acceptButtonPtr);
    hboxLayoutPtr->addWidget(rejectButtonPtr);

    QVBoxLayout* mainVBoxLayoutPtr {new QVBoxLayout};
    mainVBoxLayoutPtr->addLayout(vboxLayoutPtr,5);
    mainVBoxLayoutPtr->addLayout(hboxLayoutPtr,0);
    setLayout(mainVBoxLayoutPtr);

    QObject::connect(analyzersComboBoxPtr_,QOverload<int>::of(&QComboBox::activated),[&](int index){
        resetSettingsWidget(analyzersComboBoxPtr_->currentIndex());
    });

    const ViewsContainer viewsContainer {analyzerStorage_.getAnalyzerViews()};
    std::for_each(viewsContainer.begin(),viewsContainer.end(),[&](const std::tuple<QString,QString,QString,QString>& dataTuple){
        const QString analyzerId {std::get<3>(dataTuple)};
        const auto analyzerPtr {analyzerStorage_.getAnalyzerInstance(analyzerId)};
        if(analyzerPtr){
           stackedWidgetPtr_->addWidget(analyzerPtr->standardWidget());
        }
    });
    analyzerModelPtr_->setViewsContainer(viewsContainer);
    analyzersComboBoxPtr_->setModel(analyzerModelPtr_);
    resetSettingsWidget(analyzersComboBoxPtr_->currentIndex());

    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}

void AnalyzerDialog::analyzersViewSlot()
{
    AnalyzerViewDialog viewDialog(analyzerModelPtr_,analyzerStorage_);
    viewDialog.exec();
}

void AnalyzerDialog::matrixAddSlot()
{

}




