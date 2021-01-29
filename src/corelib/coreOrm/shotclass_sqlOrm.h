// generated by sqlpp11-ddl2cpp 003-shotclass.sql shotclass_sqlOrm doodle
#ifndef DOODLE_SHOTCLASS_SQLORM_H
#define DOODLE_SHOTCLASS_SQLORM_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace doodle
{
  namespace Shotclass_
  {
    struct Id
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T id;
            T& operator()() { return id; }
            const T& operator()() const { return id; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::bigint, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update, sqlpp::tag::can_be_null>;
    };
    struct ShotClass
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "shot_class";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T shotClass;
            T& operator()() { return shotClass; }
            const T& operator()() const { return shotClass; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct ProjectId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "project_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T projectId;
            T& operator()() { return projectId; }
            const T& operator()() const { return projectId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::bigint, sqlpp::tag::can_be_null>;
    };
  } // namespace Shotclass_

  struct Shotclass: sqlpp::table_t<Shotclass,
               Shotclass_::Id,
               Shotclass_::ShotClass,
               Shotclass_::ProjectId>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "shotclass";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T shotclass;
        T& operator()() { return shotclass; }
        const T& operator()() const { return shotclass; }
      };
    };
  };
} // namespace doodle
#endif