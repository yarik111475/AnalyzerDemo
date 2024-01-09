#include "Model.h"
#include "Defines_p.h"

analyzer::Model::Model(QObject *parent) : QAbstractTableModel(parent)
{
}

int analyzer::Model::rowCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return 0;
    }
    return viewsContainer_.size();
}

int analyzer::Model::columnCount(const QModelIndex &parent) const
{
    if(parent.isValid()){
        return 0;
    }
    return columns_;
}

QVariant analyzer::Model::data(const QModelIndex &index, int role) const
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
            return std::get<TupleFields::Name>(dataTuple);
        case 1:
           return std::get<TupleFields::Type>(dataTuple);
        case 2:
            return std::get<TupleFields::State>(dataTuple);
        case 3:
            return std::get<TupleFields::Id>(dataTuple);
        default:
            return QVariant{};
        }
    }
    return QVariant{};
}

QVariant analyzer::Model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(orientation==Qt::Horizontal && role==Qt::DisplayRole){
        switch (section){
        case 0:
            return QObject::tr("Name");
        case 1:
            return QObject::tr("Type");
        case 2:
            return QObject::tr("State");
        case 3:
            return QObject::tr("Id");
        default:
            return QVariant{};
        }
    }
    if(orientation==Qt::Vertical && role==Qt::DisplayRole){
        return QString::number(section);
    }
    return QVariant {};
}

void analyzer::Model::setViewsContainer(const ViewsContainer &viewsContainer)
{
    beginResetModel();
    viewsContainer_.assign(viewsContainer.begin(),viewsContainer.end());
    endResetModel();
}

analyzer::ViewsItem analyzer::Model::getViewsItem(int row) const
{
    if(row >= viewsContainer_.size()){
        return ViewsItem();
    }
    return viewsContainer_.at(row);
}
