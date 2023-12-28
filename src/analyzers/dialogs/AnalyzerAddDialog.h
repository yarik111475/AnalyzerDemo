#ifndef ANALYZERADDDIALOG_H
#define ANALYZERADDDIALOG_H

#include <QDialog>

class AnalyzerAddDialog : public QDialog
{
    Q_OBJECT
public:
    AnalyzerAddDialog(QWidget* parent=nullptr);
    virtual ~AnalyzerAddDialog()=default;
};

#endif // ANALYZERADDDIALOG_H
