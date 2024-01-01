#include "AnalyzerViewDialog.h"
#include "analyzers/AnalyzerModel.h"

#include <QFrame>
#include <QString>
#include <QStringList>
#include <QTableView>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QApplication>

AnalyzerViewDialog::AnalyzerViewDialog(AnalyzerModel *analyzerModelPtr, AnalyzerStorage &analyzerStorage, QWidget *parent)
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
                     this,&AnalyzerViewDialog::addSlot);
    QPushButton* editButtonPtr {new QPushButton(QObject::tr("Edit"))};
    QObject::connect(editButtonPtr,&QPushButton::clicked,
                     this,&AnalyzerViewDialog::editSlot);
    QPushButton* removeButtonPtr {new QPushButton(QObject::tr("Remove"))};
    QObject::connect(removeButtonPtr,&QPushButton::clicked,
                     this,&AnalyzerViewDialog::removeSlot);
    QPushButton* saveButtonPtr {new QPushButton(QObject::tr("Save"))};
    QObject::connect(saveButtonPtr,&QPushButton::clicked,
                     this,&AnalyzerViewDialog::saveSlot);
    QPushButton* cancelButtonPtr {new QPushButton(QObject::tr("Cancel"))};
    QObject::connect(cancelButtonPtr,&QPushButton::clicked,
                     [this](){
        reject();
    });

    QHBoxLayout* btnHBoxLayoutPtr {new QHBoxLayout};
    btnHBoxLayoutPtr->addWidget(addButtonPtr);
    btnHBoxLayoutPtr->addWidget(editButtonPtr);
    btnHBoxLayoutPtr->addWidget(removeButtonPtr);
    btnHBoxLayoutPtr->addWidget(saveButtonPtr);
    btnHBoxLayoutPtr->addWidget(cancelButtonPtr);

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

void AnalyzerViewDialog::addSlot()
{

}

void AnalyzerViewDialog::editSlot()
{

}

void AnalyzerViewDialog::removeSlot()
{

}

void AnalyzerViewDialog::saveSlot()
{

}
