#include "AnalyzerDialog.h"
#include "analyzers/AnalyzerLoader.h"
#include "analyzers/AnalyzerSettings.h"

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
#include <algorithm>

void AnalyzerDialog::resetSettings(const QString &text)
{
    const auto analyzersMap {analyzerSettings_.getAnalyzers()};
    const auto found {std::find_if(analyzersMap.begin(),analyzersMap.end(),[&](const std::pair<QString,QJsonObject>& pair){
            const auto name {pair.second.value("name").toString()};
            return name==analyzersComboBoxPtr_->currentText();
        })};

    if(found!=analyzersMap.end()){
        const auto key {found->second.value("type").toString()};

    }
}

AnalyzerDialog::AnalyzerDialog(QWidget *parent)
    : QDialog{parent},
      analyzerLoader_{AnalyzerLoader::instance()},
      analyzerSettings_{AnalyzerSettings::instance()},
      analyzersComboBoxPtr_{new QComboBox},
      analyzersAddButtonPtr_{new QPushButton(QObject::tr("..."))},
      matrixLineEditPtr_{new QLineEdit},
      matrixAddButtonPtr_{new QPushButton(QObject::tr("..."))},
      settingsVBoxLayoutPtr_{new QVBoxLayout}
{
    QGridLayout* analyzersGridLayoutPtr {new QGridLayout};
    analyzersGridLayoutPtr->addWidget(new QLabel(QObject::tr("Analyzer:")),0,0);
    analyzersGridLayoutPtr->addWidget(analyzersComboBoxPtr_,1,0,1,3);
    analyzersGridLayoutPtr->addWidget(analyzersAddButtonPtr_,1,4,1,1);

    analyzersGridLayoutPtr->addWidget(new QLabel(QObject::tr("Response matrix:")),2,0);
    analyzersGridLayoutPtr->addWidget(matrixLineEditPtr_,3,0,1,3);
    analyzersGridLayoutPtr->addWidget(matrixAddButtonPtr_,3,4,1,1);

    QGroupBox* analyzersGroupBoxPtr {new QGroupBox(QObject::tr("Analyzers"))};
    analyzersGroupBoxPtr->setLayout(analyzersGridLayoutPtr);

    QGroupBox* settingsGroupBoxPtr {new QGroupBox(QObject::tr("Settings"))};
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

    const auto analyzersMap {analyzerSettings_.getAnalyzers()};
    std::for_each(analyzersMap.begin(),analyzersMap.end(),[=](const std::pair<QString,QJsonObject>& pair){
        const QString analyzerName {pair.second.value("name").toString()};
        analyzersComboBoxPtr_->addItem(analyzerName);
    });

    resetSettings(analyzersComboBoxPtr_->currentText());
    QObject::connect(analyzersComboBoxPtr_,QOverload<const QString &>::of(&QComboBox::activated),
                     [=](const QString& text){
        resetSettings(text);
    });
}




