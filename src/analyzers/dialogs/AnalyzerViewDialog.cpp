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
    QPushButton* editButtonPtr {new QPushButton(QObject::tr("Edit"))};
    QPushButton* removeButtonPtr {new QPushButton(QObject::tr("Remove"))};
    QPushButton* saveButtonPtr {new QPushButton(QObject::tr("Save"))};
    QPushButton* cancelButtonPtr {new QPushButton(QObject::tr("Cancel"))};

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
    setWindowTitle(QObject::tr("Analyzers"));
}
