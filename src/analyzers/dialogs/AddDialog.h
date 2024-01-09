#ifndef ANALYZERNEWDIALOG_H
#define ANALYZERNEWDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

namespace analyzer
{
    class Storage;
    class AddDialog : public QDialog
    {
        Q_OBJECT
    private:
        Storage& analyzerStorage_;
        QComboBox* typesComboBoxPtr_ {nullptr};
        QLineEdit* nameLineEditPtr_  {nullptr};
    public:
        explicit AddDialog(Storage& analyzerStorage,QWidget* parent=nullptr);
        virtual ~AddDialog()=default;
    signals:
        void addSignal(const QString& analyzerType,const QString& analyzerName);
    };
}


#endif // ANALYZERNEWDIALOG_H
