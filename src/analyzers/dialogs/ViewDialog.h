#ifndef ADDDIALOG_H
#define ADDDIALOG_H

#include <QDialog>

class QTableView;

namespace analyzer
{
class Model;
class Storage;

class ViewDialog : public QDialog
{
    Q_OBJECT
private:
    Model* analyzerModelPtr_;
    Storage& analyzerStorage_;
    QTableView* analyzersViewPtr_ {nullptr};
public:
    explicit ViewDialog(Model* analyzerModelPtr, Storage& analyzerStorage,QWidget* parent=nullptr);
    virtual ~ViewDialog()=default;

private slots:
    void addSlot();
    void editSlot();
    void removeSlot();

signals:
    void removeSignal(int selectedRow);
    void addSignal(const QString& analyzerType,const QString& analyzerName);
    void editSignal(const QString& analyzerId,const QString& analyzerType,const QString& analyzerName, int selectedRow);
};
}

#endif // ADDDIALOG_H
