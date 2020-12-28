﻿//
// Created by teXiao on 2020/10/15.
//

#include "assClassModel.h"

#include <memory>
#include < core_Cpp.h>
#include <boost/numeric/conversion/cast.hpp>
DOODLE_NAMESPACE_S
assClassModel::assClassModel(QObject *parent)
    : QAbstractListModel(parent), p_ass_info_ptr_list_() {}
int assClassModel::rowCount(const QModelIndex &parent) const {
  return boost::numeric_cast<int>(p_ass_info_ptr_list_.size());
}
QVariant assClassModel::data(const QModelIndex &index, int role) const {
  if (!index.isValid()) return QVariant();
  if (index.row() >= p_ass_info_ptr_list_.size()) return QVariant();

  auto var = QVariant();
  switch (role) {
    case Qt::EditRole:
    case Qt::DisplayRole:
      var = DOTOS(p_ass_info_ptr_list_[index.row()]->getAssClass(true));
      break;
    case Qt::UserRole:
      var = QVariant::fromValue(p_ass_info_ptr_list_[index.row()]);
    default:
      break;
  }
  return var;
}
Qt::ItemFlags assClassModel::flags(const QModelIndex &index) const {
  if (!index.isValid()) return Qt::ItemIsEditable;
  if (index.row() >= p_ass_info_ptr_list_.size()) return Qt::ItemIsEditable;

  // if (p_ass_info_ptr_list_[index.row()]->isInsert())
  //  return QAbstractListModel::flags(index);
  // else
  return Qt::ItemIsEditable | Qt::ItemIsEnabled |
         QAbstractListModel::flags(index);
}
bool assClassModel::setData(const QModelIndex &index, const QVariant &value,
                            int role) {
  if (!index.isValid()) return false;
  bool is_has = false;
  for (auto &&item : p_ass_info_ptr_list_) {
    if (value.toString().toStdString() == item->getAssClass(true) ||
        value.toString().toStdString() == item->getAssClass()) {
      is_has = true;
      break;
    }
  }
  auto ass = p_ass_info_ptr_list_[index.row()];
  if (!is_has) {
    if (ass->getAssClass(true) != value.toString().toStdString()) {
      ass->setAssClass(value.toString().toStdString());
      ass->setAssDep(coreDataManager::get().getAssDepPtr());

      if (ass->isInsert()) {
        ass->updateSQL();
      } else {
        ass->insert();
      }
      dataChanged(index, index, {role});
      return true;
    }
  }
  return false;
}
bool assClassModel::insertRows(int position, int rows,
                               const QModelIndex &index) {
  beginInsertRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    p_ass_info_ptr_list_.insert(p_ass_info_ptr_list_.begin() + position,
                                std::make_shared<assClass>());
    p_ass_info_ptr_list_[position]->setAssDep(
        coreDataManager::get().getAssDepPtr());
  }
  endInsertRows();
  return true;
}
bool assClassModel::removeRows(int position, int rows,
                               const QModelIndex &index) {
  beginRemoveRows(QModelIndex(), position, position + rows - 1);
  for (int row = 0; row < rows; ++row) {
    auto assClass = p_ass_info_ptr_list_[position];
    if (assClass) {
      assClass->deleteSQL();
      p_ass_info_ptr_list_.erase(p_ass_info_ptr_list_.begin() + position);
    }
  }
  endRemoveRows();
  return true;
}
void assClassModel::init() {
  clear();
  auto tmp_list =
      assClass::getAll(coreDataManager::get().getAssDepPtr());
  if (!tmp_list.empty()) {
    beginInsertRows(QModelIndex(), 0,
                    boost::numeric_cast<int>(tmp_list.size()) - 1);
    p_ass_info_ptr_list_ = tmp_list;
    endInsertRows();
  }
}
void assClassModel::clear() {
  if (p_ass_info_ptr_list_.empty()) return;
  beginResetModel();
  p_ass_info_ptr_list_.clear();
  endResetModel();
  coreDataManager::get().setAssClassPtr(nullptr);
}
assClassModel::~assClassModel() = default;

DOODLE_NAMESPACE_E