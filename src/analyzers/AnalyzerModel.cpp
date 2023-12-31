#include "AnalyzerModel.h"

AnalyzerModel::AnalyzerModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int AnalyzerModel::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return 0;
    }
    return viewsContainer_.size();
}

int AnalyzerModel::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return 0;
    }
    return columns_;
}

QVariant AnalyzerModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid()){
        return QVariant{};
    }
    if(index.row() >= viewsContainer_.size()){
        return QVariant{};
    }
    if(role==Qt::DisplayRole){
        const int row {index.row()};
        const int column {index.column()};
        const auto dataTuple {viewsContainer_.at(row)};
        switch(column){
        case 0:
            return std::get<2>(dataTuple);//name
            break;
        case 1:
           return std::get<1>(dataTuple);//type
            break;
        case 2:
            return std::get<0>(dataTuple);//id
            break;
        default:
            return QVariant{};
        }
    }
    return QVariant{};
}

void AnalyzerModel::setViewsContainer(const ViewsContainer &viewsContainer)
{
    beginResetModel();
    viewsContainer_.assign(viewsContainer.begin(),viewsContainer.end());
    endResetModel();
}
