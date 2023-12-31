#ifndef ANALYZERADDDIALOG_H
#define ANALYZERADDDIALOG_H

#include <QDialog>

class AnalyzerListDialog : public QDialog
{
    Q_OBJECT
public:
    AnalyzerListDialog(QWidget* parent=nullptr);
    virtual ~AnalyzerListDialog()=default;
};

#endif // ANALYZERADDDIALOG_H
