#ifndef ANALYZEREDITDIALOG_H
#define ANALYZEREDITDIALOG_H

#include <QDialog>

class AnalyzerEditDialog : public QDialog
{
    Q_OBJECT
public:
    explicit AnalyzerEditDialog(QWidget* parent=nullptr);
    virtual ~AnalyzerEditDialog()=default;
};

#endif // ANALYZEREDITDIALOG_H
