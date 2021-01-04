﻿//
// Created by teXiao on 2020/10/14.
//
#include "shotTypeModel.h"

#include <memory>

#include < core_Cpp.h>

#include <boost/numeric/conversion/cast.hpp>
DOODLE_NAMESPACE_S
shotTypeModel::shotTypeModel(QObject *parent) : QAbstractListModel(parent) {
  shotType::insertChanged.connect(boost::bind(&shotTypeModel::reInit, this));
}

int shotTypeModel::rowCount(const QModelIndex &parent) const {
  return boost::numeric_cast<int>(p_type_ptr_list_.size());
}

QVariant shotTypeModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();
  if (index.row() >= p_type_ptr_list_.size()) return QVariant();
  auto var = QVariant();

  switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole: {
      var = p_type_ptr_list_[index.row()]->getTypeQ();
    } break;
    case Qt::UserRole:
      var = QVariant::fromValue(p_type_ptr_list_[index.row()]);
      break;
    default:
      break;
  }
  return var;
}

shotTypePtr shotTypeModel::daraRow(const QModelIndex &index) const {
  if (!index.isValid()) return nullptr;
  return p_type_ptr_list_[index.row()];
}
QVariant shotTypeModel::headerData(int section, Qt::Orientation Orientation, int role) const {
  return QAbstractItemModel::headerData(section, Orientation, role);
}
Qt::ItemFlags shotTypeModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::ItemIsEnabled;

  if (p_type_ptr_list_[index.row()]->isInsert())
    return QAbstractListModel::flags(index);
  else
    return Qt::ItemIsEnabled | Qt::ItemIsEditable | QAbstractListModel::flags(index);
}

bool shotTypeModel::setData(const QModelIndex &index, const QVariant &value, int role) {
  if (index.isValid() && role == Qt::EditRole) {
    bool isHas = false;
    for (const auto &item : p_type_ptr_list_) {
      if (item->isInsert() || value.toString() == item->getTypeQ()) {
        isHas = true;
        break;
      }
    }

    if (!isHas) {
      p_type_ptr_list_[index.row()]->setType(value.toString());
      p_type_ptr_list_[index.row()]->insert();
    } else {
      return false;
    }
  }
  return true;
}
bool shotTypeModel::insertRows(int position, int rows, const QModelIndex &index) {
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    p_type_ptr_list_.insert(p_type_ptr_list_.begin() + position,
                            std::make_shared<shotType>());
  }
  endInsertRows();
  return true;
}
bool shotTypeModel::removeRows(int position, int rows, const QModelIndex &index) {
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    p_type_ptr_list_.erase(p_type_ptr_list_.begin() + position);
  }
  endRemoveRows();
  return true;
}
void shotTypeModel::init() {
  clear();
  auto tmp_fileTypeList = shotType::getAll();
  if (tmp_fileTypeList.empty()) return;
  beginInsertRows(QModelIndex(), 0, boost::numeric_cast<int>(tmp_fileTypeList.size()) - 1);
  p_type_ptr_list_ = tmp_fileTypeList;
  endInsertRows();
}
void shotTypeModel::clear() {
  beginResetModel();
  p_type_ptr_list_.clear();
  endResetModel();
}
void shotTypeModel::reInit() {
  shotTypePtrList tmp_fileTypeList{};
  for (auto &tmp : shotType::Instances()) {
    if (tmp)
      tmp_fileTypeList.push_back(tmp->shared_from_this());
  }

  if (tmp_fileTypeList.empty()) return;
  beginInsertRows(QModelIndex(), 0, boost::numeric_cast<int>(tmp_fileTypeList.size()) - 1);
  p_type_ptr_list_ = tmp_fileTypeList;
  endInsertRows();
}

DOODLE_NAMESPACE_E