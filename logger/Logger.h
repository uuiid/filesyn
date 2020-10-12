﻿/*
 * @Author: your name
 * @Date: 2020-10-10 13:18:58
 * @LastEditTime: 2020-10-10 13:48:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \Doodle\logger\Logger.h
 */
#pragma once
#include <QDebug>

#if defined(LOGGER_LIBRARY)
#  define LOGGER_EXPORT Q_DECL_EXPORT
#else
#  define LOGGER_EXPORT Q_DECL_IMPORT
#endif


namespace Logger
{

#define DOODLE_LOG_DEBUG qDebug() << __FILE__ << __FUNCTION__ << __LINE__
#define DOODLE_LOG_INFO qInfo() << __FILE__ << __FUNCTION__ << __LINE__
#define DOODLE_LOG_WARN qWarning() << __FILE__ << __FUNCTION__ << __LINE__
#define DOODLE_LOG_CRIT qCritical() << __FILE__ << __FUNCTION__ << __LINE__

void LOGGER_EXPORT doodle_initLog(const QString &logPath = QStringLiteral("Log"),
                                  int logMaxCount = 1024, bool async = true);

} // namespace Logger
