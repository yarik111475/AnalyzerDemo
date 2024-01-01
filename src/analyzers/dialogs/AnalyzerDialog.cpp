#include "AnalyzerDialog.h"
#include "analyzers/IAnalyzer.h"
#include "analyzers/AnalyzerModel.h"
#include "analyzers/AnalyzerStorage.h"
#include "analyzers/dialogs/AnalyzerViewDialog.h"

#include <QUuid>
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

    QPushButton* saveButtonPtr {new QPushButton(QObject::tr("Save"))};
    QObject::connect(saveButtonPtr,&QPushButton::clicked,[&](){
        accept();
    });
    QPushButton* cancelButtonPtr {new QPushButton(QObject::tr("Cancel"))};
    QObject::connect(cancelButtonPtr,&QPushButton::clicked,[&](){
        reject();
    });

    QHBoxLayout* btnHBoxLayoutPtr {new QHBoxLayout};
    btnHBoxLayoutPtr->addStretch(5);
    btnHBoxLayoutPtr->addWidget(saveButtonPtr);
    btnHBoxLayoutPtr->addWidget(cancelButtonPtr);

    QVBoxLayout* mainVBoxLayoutPtr {new QVBoxLayout};
    mainVBoxLayoutPtr->addLayout(vboxLayoutPtr,5);
    mainVBoxLayoutPtr->addLayout(btnHBoxLayoutPtr,0);
    setLayout(mainVBoxLayoutPtr);

    QObject::connect(analyzersComboBoxPtr_,QOverload<int>::of(&QComboBox::activated),
                     [&](int index){
        resetSettingsWidget(analyzersComboBoxPtr_->currentIndex());
    });

    const ViewsContainer viewsContainer {analyzerStorage_.getAnalyzerViews()};
    QObject::connect(analyzerModelPtr_,&QAbstractItemModel::modelReset,
                     this,&AnalyzerDialog::modelResetSlot);
    analyzerModelPtr_->setViewsContainer(viewsContainer);

    std::for_each(viewsContainer.begin(),viewsContainer.end(),
                  [&](const std::tuple<QString,QString,QString,QString>& dataTuple){
        const QString analyzerId {std::get<3>(dataTuple)};
        const auto analyzerPtr {analyzerStorage_.getAnalyzerInstance(analyzerId)};
        if(analyzerPtr){
            stackedWidgetPtr_->addWidget(analyzerPtr->standardWidget());
        }
    });

    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}

void AnalyzerDialog::analyzersViewSlot()
{
    AnalyzerViewDialog viewDialog(analyzerModelPtr_,analyzerStorage_,this);
    QObject::connect(&viewDialog,&AnalyzerViewDialog::removeSignal,
                     [this](int selectedRow){
        const ViewsContainer containerBefore {analyzerStorage_.getAnalyzerViews()};
        const QString analyzerId {std::get<3>(containerBefore.at(selectedRow))};
        const bool removeOk {analyzerStorage_.removeAnalyzerInstance(analyzerId)};
        if(removeOk){
            QWidget* standardWidgetPtr {stackedWidgetPtr_->widget(selectedRow)};
            stackedWidgetPtr_->removeWidget(standardWidgetPtr);
            standardWidgetPtr->deleteLater();

            const ViewsContainer containerAfter {analyzerStorage_.getAnalyzerViews()};
            analyzerModelPtr_->setViewsContainer(containerAfter);
        }
    });
    QObject::connect(&viewDialog,&AnalyzerViewDialog::addSignal,
                     [this](const QString& analyzerType,const QString& analyzerName){
        const QUuid qUuid {QUuid::createUuid()};
        const QString analyzerId {qUuid.toString(QUuid::WithoutBraces)};
        const bool addOk {analyzerStorage_.addAnalyzerInstance(analyzerId,analyzerType,analyzerName)};
        if(addOk){
            const ViewsContainer containerAfter {analyzerStorage_.getAnalyzerViews()};
            analyzerModelPtr_->setViewsContainer(containerAfter);
            auto instancePtr {analyzerStorage_.getAnalyzerInstance(analyzerId)};
            QWidget* standardWidgetPtr {instancePtr->standardWidget()};
            stackedWidgetPtr_->addWidget(standardWidgetPtr);
        }
    });
    viewDialog.exec();
}

void AnalyzerDialog::matrixAddSlot()
{

}

void AnalyzerDialog::modelResetSlot()
{
    analyzersComboBoxPtr_->setModel(analyzerModelPtr_);
    analyzersComboBoxPtr_->setCurrentIndex(0);
    resetSettingsWidget(analyzersComboBoxPtr_->currentIndex());
}




