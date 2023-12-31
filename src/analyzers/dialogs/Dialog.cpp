#include "Dialog.h"
#include "analyzers/IAnalyzer.h"
#include "analyzers/Model.h"
#include "analyzers/Storage.h"
#include "analyzers/dialogs/ViewDialog.h"
#include "../Defines_p.h"

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

void analyzer::Dialog::resetSettingsWidget(int index)
{
    stackedWidgetPtr_->setCurrentIndex(index);
}

analyzer::Dialog::Dialog(QWidget *parent)
    : QDialog{parent},
      analyzerModelPtr_{new analyzer::Model},
      analyzerStorage_{analyzer::Storage::instance()},
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
                     this,&analyzer::Dialog::analyzersViewSlot);

    analyzersGridLayoutPtr->addWidget(new QLabel(QObject::tr("Response matrix:")),2,0);
    analyzersGridLayoutPtr->addWidget(matrixLineEditPtr_,3,0,1,3);
    analyzersGridLayoutPtr->addWidget(matrixAddButtonPtr_,3,4,1,1);
    QObject::connect(matrixAddButtonPtr_,&QPushButton::clicked,
                     this,&analyzer::Dialog::matrixAddSlot);

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
        QString lastError {};
        analyzerStorage_.saveInstances(lastError);
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

    const analyzer::ViewsContainer viewsContainer {analyzerStorage_.getViews()};
    QObject::connect(analyzerModelPtr_,&QAbstractItemModel::modelReset,
                     this,&analyzer::Dialog::modelResetSlot);
    analyzerModelPtr_->setViewsContainer(viewsContainer);

    std::for_each(viewsContainer.begin(),viewsContainer.end(),
                  [&](const std::tuple<QString,QString,QString,QString>& dataTuple){
        const QString analyzerId {std::get<analyzer::TupleFields::Id>(dataTuple)};
        const auto analyzerPtr {analyzerStorage_.getInstance(analyzerId)};
        if(analyzerPtr){
            stackedWidgetPtr_->addWidget(analyzerPtr->standardWidget());
        }
    });

    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}

void analyzer::Dialog::analyzersViewSlot()
{
    analyzer::ViewDialog viewDialog(analyzerModelPtr_,analyzerStorage_,this);
    QObject::connect(&viewDialog,&analyzer::ViewDialog::removeSignal,
                     [this](int selectedRow){
        const analyzer::ViewsContainer containerBefore {analyzerStorage_.getViews()};
        const QString analyzerId {std::get<analyzer::TupleFields::Id>(containerBefore.at(selectedRow))};
        const bool removeOk {analyzerStorage_.removeInstance(analyzerId)};
        if(removeOk){
            QWidget* standardWidgetPtr {stackedWidgetPtr_->widget(selectedRow)};
            if(standardWidgetPtr){
                stackedWidgetPtr_->removeWidget(standardWidgetPtr);
                standardWidgetPtr->deleteLater();
            }
            const analyzer::ViewsContainer containerAfter {analyzerStorage_.getViews()};
            analyzerModelPtr_->setViewsContainer(containerAfter);
        }
    });
    QObject::connect(&viewDialog,&analyzer::ViewDialog::addSignal,
                     [this](const QString& analyzerType,const QString& analyzerName){
        const QUuid qUuid {QUuid::createUuid()};
        const QString analyzerId {qUuid.toString(QUuid::WithoutBraces)};
        const bool addOk {analyzerStorage_.addInstance(analyzerId,analyzerType,analyzerName)};
        if(addOk){ 
            const auto instancePtr {analyzerStorage_.getInstance(analyzerId)};
            if(instancePtr){
                QWidget* standardWidgetPtr {instancePtr->standardWidget()};
                stackedWidgetPtr_->addWidget(standardWidgetPtr);
            }
            const analyzer::ViewsContainer container {analyzerStorage_.getViews()};
            analyzerModelPtr_->setViewsContainer(container);
        }
    });
    QObject::connect(&viewDialog,&analyzer::ViewDialog::editSignal,
                     [this](const QString& analyzerId,const QString& analyzerType,const QString& analyzerName,int selectedRow){
        const bool editOk {analyzerStorage_.editInstance(analyzerId,analyzerType,analyzerName)};
        if(editOk){
            QWidget* standardWidgetPtr {stackedWidgetPtr_->widget(selectedRow)};
            if(standardWidgetPtr){
                stackedWidgetPtr_->removeWidget(standardWidgetPtr);
                standardWidgetPtr->deleteLater();
            }
            const auto instancePtr {analyzerStorage_.getInstance(analyzerId)};
            if(instancePtr){
                QWidget* standardWidgetPtr {instancePtr->standardWidget()};
                stackedWidgetPtr_->addWidget(standardWidgetPtr);
            }
            const analyzer::ViewsContainer container {analyzerStorage_.getViews()};
            analyzerModelPtr_->setViewsContainer(container);
        }
    });
    viewDialog.exec();
}

void analyzer::Dialog::matrixAddSlot()
{

}

void analyzer::Dialog::modelResetSlot()
{
    analyzersComboBoxPtr_->setModel(analyzerModelPtr_);
    analyzersComboBoxPtr_->setCurrentIndex(0);
    resetSettingsWidget(0);
}




