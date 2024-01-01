#ifndef ANALYZERADDDIALOG_H
#define ANALYZERADDDIALOG_H

#include <QDialog>

class QTableView;
class AnalyzerModel;
class AnalyzerStorage;
class AnalyzerViewDialog : public QDialog
{
    Q_OBJECT
private:
    AnalyzerModel* analyzerModelPtr_;
    AnalyzerStorage& analyzerStorage_;
    QTableView* analyzersViewPtr_ {nullptr};
public:
    AnalyzerViewDialog(AnalyzerModel* analyzerModelPtr, AnalyzerStorage& analyzerStorage,QWidget* parent=nullptr);
    virtual ~AnalyzerViewDialog()=default;

private slots:
    void addSlot();
    void editSlot();
    void removeSlot();
    void saveSlot();
};

#endif // ANALYZERADDDIALOG_H
