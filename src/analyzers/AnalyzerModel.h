#ifndef ANALYZERMODEL_H
#define ANALYZERMODEL_H

#include <QAbstractTableModel>
#include <QJsonObject>
#include <vector>
#include <tuple>

typedef std::tuple<QString,QString,QString,QString> ViewsItem;
typedef std::vector<std::tuple<QString,QString,QString,QString>> ViewsContainer;

class AnalyzerModel : public QAbstractTableModel
{
    Q_OBJECT
private:
    const int columns_ {4};
    ViewsContainer viewsContainer_ {};
public:
    explicit AnalyzerModel(QObject *parent = nullptr);
    virtual ~AnalyzerModel()=default;
    virtual int rowCount(const QModelIndex& parent)const override;
    virtual int columnCount(const QModelIndex& parent)const override;
    virtual QVariant data(const QModelIndex& index,int role)const override;
    virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    void setViewsContainer(const ViewsContainer& viewsContainer);
    ViewsItem getViewsItem(int row)const;
};

#endif // ANALYZERMODEL_H
