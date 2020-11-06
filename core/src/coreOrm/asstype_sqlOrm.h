// generated by sqlpp11-ddl2cpp 009-asstype.sql asstype_sqlOrm doodle
#ifndef DOODLE_ASSTYPE_SQLORM_H
#define DOODLE_ASSTYPE_SQLORM_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace doodle
{
  namespace Asstype_
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
      using _traits = sqlpp::make_traits<sqlpp::smallint, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update, sqlpp::tag::can_be_null>;
    };
    struct AssType
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "ass_type";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T assType;
            T& operator()() { return assType; }
            const T& operator()() const { return assType; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::varchar, sqlpp::tag::can_be_null>;
    };
    struct AssClassId
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "assClass_id";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T assClassId;
            T& operator()() { return assClassId; }
            const T& operator()() const { return assClassId; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::smallint, sqlpp::tag::can_be_null>;
    };
  } // namespace Asstype_

  struct Asstype: sqlpp::table_t<Asstype,
               Asstype_::Id,
               Asstype_::AssType,
               Asstype_::AssClassId>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "asstype";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T asstype;
        T& operator()() { return asstype; }
        const T& operator()() const { return asstype; }
      };
    };
  };
} // namespace doodle
#endif
