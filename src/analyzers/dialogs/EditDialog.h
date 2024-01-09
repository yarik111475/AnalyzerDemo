#ifndef ANALYZEREDITDIALOG_H
#define ANALYZEREDITDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;

namespace analyzer
{
    class Storage;
    using ViewsItem=std::tuple<QString,QString,QString,QString>;

    class EditDialog : public QDialog
    {
        Q_OBJECT
    private:
        Storage& analyzerStorage_;
        QComboBox* typesComboBoxPtr_ {nullptr};
        QLineEdit* nameLineEditPtr_  {nullptr};
        ViewsItem viewsItem_;
        int selectedRow_ {};
    public:
        explicit EditDialog(Storage& analyzerStorage,const ViewsItem& viewsItem,int selectedRow,QWidget* parent=nullptr);
        virtual ~EditDialog()=default;
    signals:
        void editSignal(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName,int selectedRow);
    };
}



#endif // ANALYZEREDITDIALOG_H
