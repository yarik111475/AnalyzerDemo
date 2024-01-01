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
    explicit AnalyzerViewDialog(AnalyzerModel* analyzerModelPtr, AnalyzerStorage& analyzerStorage,QWidget* parent=nullptr);
    virtual ~AnalyzerViewDialog()=default;

private slots:
    void addSlot();
    void editSlot();
    void removeSlot();
    void saveSlot();

signals:
    void editSignal(int selectedRow);
    void removeSignal(int selectedRow);
    void addSignal(const QString& analyzerType,const QString& analyzerName);
};

#endif // ANALYZERADDDIALOG_H
