#include "AnalyzerAddDialog.h"
#include "analyzers/AnalyzerStorage.h"

#include <QLabel>
#include <QLineEdit>
#include <QComboBox>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QApplication>
#include <QMessageBox>
#include <algorithm>

AnalyzerAddDialog::AnalyzerAddDialog(AnalyzerStorage &analyzerStorage, QWidget *parent)
    :QDialog{parent},
      analyzerStorage_{analyzerStorage},
      typesComboBoxPtr_{new QComboBox},
      nameLineEditPtr_{new QLineEdit}
{
    const auto typesContainer {analyzerStorage_.getAnalyserTypes()};
    QStringList itemsList {};
    std::transform(typesContainer.begin(),typesContainer.end(),std::back_inserter(itemsList),
                   [](const QString& item){
        return item;
    });
    typesComboBoxPtr_->addItems(itemsList);
    nameLineEditPtr_->setPlaceholderText(QObject::tr("Enter analyzer name..."));

    QFormLayout* formLayoutPtr {new QFormLayout};
    formLayoutPtr->addRow(QObject::tr("Analyzer type:"),typesComboBoxPtr_);
    formLayoutPtr->addRow(QObject::tr("Analyzer name:"),nameLineEditPtr_);

    QPushButton* okButtonPtr {new QPushButton(QObject::tr("Ok"))};
    QObject::connect(okButtonPtr,&QPushButton::clicked,
                     [this](){
        if(nameLineEditPtr_->text().isEmpty()){
            QMessageBox::warning(this,QObject::tr("Warning"),QObject::tr("You must enter analyzer name!"));
            return;
        }
        const QString analyzerType {typesComboBoxPtr_->currentText()};
        const QString analyzerName {nameLineEditPtr_->text()};
        emit addSignal(analyzerType,analyzerName);
        accept();
    });
    QPushButton* cancelButtonPtr {new QPushButton(QObject::tr("Cancel"))};
    QObject::connect(cancelButtonPtr,&QPushButton::clicked,
                     [this](){
        reject();
    });

    QHBoxLayout* btnHBoxLayoutPtr {new QHBoxLayout};
    btnHBoxLayoutPtr->addStretch(5);
    btnHBoxLayoutPtr->addWidget(okButtonPtr);
    btnHBoxLayoutPtr->addWidget(cancelButtonPtr);

    QVBoxLayout* mainVBoxLayoutPtr {new QVBoxLayout};
    mainVBoxLayoutPtr->addLayout(formLayoutPtr,5);
    mainVBoxLayoutPtr->addLayout(btnHBoxLayoutPtr,0);

    setLayout(mainVBoxLayoutPtr);
    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}
