#include "ViewDialog.h"
#include "analyzers/Model.h"
#include "analyzers/Storage.h"
#include "analyzers/dialogs/AddDialog.h"
#include "analyzers/dialogs/EditDialog.h"

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QTableView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QItemSelectionModel>
#include <QApplication>

analyzer::ViewDialog::ViewDialog(analyzer::Model *analyzerModelPtr, analyzer::Storage &analyzerStorage, QWidget *parent)
    :QDialog{parent},
      analyzerModelPtr_{analyzerModelPtr},
      analyzerStorage_{analyzerStorage},
      analyzersViewPtr_{new QTableView}
{
    QVBoxLayout* viewVBoxLayoutPtr {new QVBoxLayout};
    viewVBoxLayoutPtr->addWidget(analyzersViewPtr_);
    analyzersViewPtr_->setModel(analyzerModelPtr);

    QPushButton* addButtonPtr {new QPushButton(QObject::tr("Add"))};
    QObject::connect(addButtonPtr,&QPushButton::clicked,
                     this,&analyzer::ViewDialog::addSlot);
    QPushButton* editButtonPtr {new QPushButton(QObject::tr("Edit"))};
    QObject::connect(editButtonPtr,&QPushButton::clicked,
                     this,&analyzer::ViewDialog::editSlot);
    QPushButton* removeButtonPtr {new QPushButton(QObject::tr("Remove"))};
    QObject::connect(removeButtonPtr,&QPushButton::clicked,
                     this,&analyzer::ViewDialog::removeSlot);
    QPushButton* closeButtonPtr {new QPushButton(QObject::tr("Close"))};
    QObject::connect(closeButtonPtr,&QPushButton::clicked,
                     [this](){
        accept();
    });

    QHBoxLayout* btnHBoxLayoutPtr {new QHBoxLayout};
    btnHBoxLayoutPtr->addStretch(5);
    btnHBoxLayoutPtr->addWidget(addButtonPtr);
    btnHBoxLayoutPtr->addWidget(editButtonPtr);
    btnHBoxLayoutPtr->addWidget(removeButtonPtr);
    btnHBoxLayoutPtr->addWidget(closeButtonPtr);

    QFrame* btnFramePtr {new QFrame};
    btnFramePtr->setLayout(btnHBoxLayoutPtr);

    QVBoxLayout* mainVBoxLayoutPtr {new QVBoxLayout};
    mainVBoxLayoutPtr->addLayout(viewVBoxLayoutPtr,5);
    mainVBoxLayoutPtr->addWidget(btnFramePtr,0);
    setLayout(mainVBoxLayoutPtr);

    analyzersViewPtr_->setSelectionMode(QAbstractItemView::SingleSelection);
    analyzersViewPtr_->setSelectionBehavior(QAbstractItemView::SelectRows);
    analyzersViewPtr_->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    setWindowFlags(Qt::Dialog|Qt::WindowCloseButtonHint);
    setWindowTitle(qApp->applicationName());
}

void analyzer::ViewDialog::addSlot()
{
    analyzer::AddDialog addDialog {analyzerStorage_,this};
    QObject::connect(&addDialog,&analyzer::AddDialog::addSignal,
                     this,&analyzer::ViewDialog::addSignal);
    addDialog.exec();
}

void analyzer::ViewDialog::editSlot()
{
    const auto selectionModel {analyzersViewPtr_->selectionModel()->selectedRows()};
    if(!selectionModel.isEmpty()){
        const int selectedRow {selectionModel.at(0).row()};
        const ViewsItem viewsItem {analyzerModelPtr_->getViewsItem(selectedRow)};
        analyzer::EditDialog editDialog {analyzerStorage_,viewsItem,selectedRow,this};
        QObject::connect(&editDialog,&analyzer::EditDialog::editSignal,
                         this,&analyzer::ViewDialog::editSignal);
        editDialog.exec();
    }
}

void analyzer::ViewDialog::removeSlot()
{
    const auto selectionList {analyzersViewPtr_->selectionModel()->selectedRows()};
    if(!selectionList.isEmpty()){
        const int selectedRow {selectionList.at(0).row()};
        emit removeSignal(selectedRow);
    }
}
