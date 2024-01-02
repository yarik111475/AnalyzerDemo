#ifndef ANALYZEREDITDIALOG_H
#define ANALYZEREDITDIALOG_H

#include <QDialog>

class QLineEdit;
class QComboBox;
class AnalyzerStorage;
typedef std::tuple<QString,QString,QString,QString> ViewsItem;

class AnalyzerEditDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerStorage& analyzerStorage_;
    QComboBox* typesComboBoxPtr_ {nullptr};
    QLineEdit* nameLineEditPtr_  {nullptr};
public:
    explicit AnalyzerEditDialog(AnalyzerStorage& analyzerStorage,const ViewsItem& viewsItem,QWidget* parent=nullptr);
    virtual ~AnalyzerEditDialog()=default;
};

#endif // ANALYZEREDITDIALOG_H
