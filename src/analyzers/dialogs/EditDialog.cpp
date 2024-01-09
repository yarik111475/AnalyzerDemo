#include "EditDialog.h"
#include "analyzers/Storage.h"
#include "../Defines_p.h"

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

analyzer::EditDialog::EditDialog(analyzer::Storage &analyzerStorage, const ViewsItem &viewsItem, int selectedRow, QWidget *parent)
    :QDialog{parent},
      analyzerStorage_{analyzerStorage},
      typesComboBoxPtr_{new QComboBox},
      nameLineEditPtr_{new QLineEdit},
      viewsItem_{viewsItem},
      selectedRow_{selectedRow}
{
    const auto typesContainer {analyzerStorage_.getTypes()};
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
        const QString analyzerId {std::get<TupleFields::Id>(viewsItem_)};
        const QString analyzerName {nameLineEditPtr_->text()};
        const QString analyzerType {typesComboBoxPtr_->currentText()};
        const QString analyzerState {std::get<TupleFields::State>(viewsItem_)};
        if(std::get<TupleFields::Name>(viewsItem_)!=analyzerName || std::get<TupleFields::Type>(viewsItem_)!=analyzerType){
            if(analyzerState=="Enabled"){
                QMessageBox::warning(this,QObject::tr("Warning"),QObject::tr("Analyzer is running.\nStop it before editing!"));
                return;
            }
            emit editSignal(analyzerId,analyzerType,analyzerName,selectedRow_);
        }
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

    const QString analyzerName {std::get<TupleFields::Name>(viewsItem)};
    const QString analyzerType {std::get<TupleFields::Type>(viewsItem)};

    typesComboBoxPtr_->setCurrentText(analyzerType);
    nameLineEditPtr_->setText(analyzerName);

    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}
