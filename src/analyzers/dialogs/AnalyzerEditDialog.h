#ifndef ANALYZEREDITDIALOG_H
#define ANALYZEREDITDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class AnalyzerStorage;
using ViewsItem=std::tuple<QString,QString,QString,QString>;

class AnalyzerEditDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerStorage& analyzerStorage_;
    QComboBox* typesComboBoxPtr_ {nullptr};
    QLineEdit* nameLineEditPtr_  {nullptr};
    ViewsItem viewsItem_;
public:
    explicit AnalyzerEditDialog(AnalyzerStorage& analyzerStorage,const ViewsItem& viewsItem,QWidget* parent=nullptr);
    virtual ~AnalyzerEditDialog()=default;
signals:
    void editSignal(const QString& analyzerId,const QString& analyzerName,const QString& analyzerType);
};

#endif // ANALYZEREDITDIALOG_H
