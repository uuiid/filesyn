﻿//
// Created by teXiao on 2020/10/14.
//
#include <core_doQt.h>
#include "assDepModel.h"
#include <boost/numeric/conversion/cast.hpp>
DOODLE_NAMESPACE_S
assDepModel::assDepModel(QObject *parent)
:QAbstractListModel(parent)
,p_class_ptr_list_(){
  init();
}
int assDepModel::rowCount(const QModelIndex &parent) const {
  return boost::numeric_cast<int>(p_class_ptr_list_.size());
}
QVariant assDepModel::data(const QModelIndex &index, int role) const {
  if(!index.isValid()) return QVariant();
  if(index.row() >= p_class_ptr_list_.size()) return QVariant();

  if(role == Qt::DisplayRole || role == Qt::EditRole){
    return p_class_ptr_list_[index.row()]->getAssDepQ();
  } else if (role == Qt::UserRole){
    return QVariant::fromValue(p_class_ptr_list_[index.row()]);
  } else{
    return QVariant();
  }
}
void assDepModel::init() {
  clear();
  auto tmp_List  = doCore::assdepartment::getAll();
  beginInsertRows(QModelIndex(),0,boost::numeric_cast<int>(tmp_List.size())-1);
  p_class_ptr_list_ = tmp_List;
  endInsertRows();
}
void assDepModel::clear() {
  if(!p_class_ptr_list_.empty()) {
    beginResetModel();
    p_class_ptr_list_.clear();
    endResetModel();
  }
  doCore::coreDataManager::get().setAssDepPtr(nullptr);
}
assDepModel::~assDepModel() = default;


DOODLE_NAMESPACE_E