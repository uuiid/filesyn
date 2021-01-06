/*
 * @Author: your name
 * @Date: 2020-09-28 14:13:33
 * @LastEditTime: 2020-12-01 14:36:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\doodle_GUI\doodle_global.h
 */
#pragma once

#include <core_global.h>

#include <QtCore/qglobal.h>
#include <QWidget>
#include <QHBoxLayout>
#include <QComboBox>
#include <QSpinBox>

// class QHBoxLayout;
// class QComboBox;

DOODLE_NAMESPACE_S
enum class filterState {
  useFilter,
  notFilter,
  showAll,
};

class shotEpsListModel;
class episodesintDelegate;
class shotEpsListWidget;

class shotListModel;
class shotIntEnumDelegate;
class shotListWidget;

class shotClassModel;
class shotClassWidget;

class shotTypeModel;
class shotTypeWidget;

class shotTableModel;
class shotTableWidget;

class assDepModel;
class assDepWidget;

class assClassModel;
class assClassWidget;
class assSortfilterModel;

class assTypeModel;
class assTypeWidget;

class assTableModel;
class assTableWidght;

class settingWidget;

class shotWidget;
class assWidght;
class mainWindows;
DOODLE_NAMESPACE_E