// generated by sqlpp11-ddl2cpp 000-pinyin.sql pinyin_sqlOrm doodle
#ifndef DOODLE_PINYIN_SQLORM_H
#define DOODLE_PINYIN_SQLORM_H

#include <sqlpp11/table.h>
#include <sqlpp11/data_types.h>
#include <sqlpp11/char_sequence.h>

namespace doodle
{
  namespace Pinyin_
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
      using _traits = sqlpp::make_traits<sqlpp::integer, sqlpp::tag::must_not_insert, sqlpp::tag::must_not_update, sqlpp::tag::can_be_null>;
    };
    struct Znch
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "znch";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T znch;
            T& operator()() { return znch; }
            const T& operator()() const { return znch; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
    struct En
    {
      struct _alias_t
      {
        static constexpr const char _literal[] =  "en";
        using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
        template<typename T>
        struct _member_t
          {
            T en;
            T& operator()() { return en; }
            const T& operator()() const { return en; }
          };
      };
      using _traits = sqlpp::make_traits<sqlpp::text, sqlpp::tag::can_be_null>;
    };
  } // namespace Pinyin_

  struct Pinyin: sqlpp::table_t<Pinyin,
               Pinyin_::Id,
               Pinyin_::Znch,
               Pinyin_::En>
  {
    struct _alias_t
    {
      static constexpr const char _literal[] =  "pinyin";
      using _name_t = sqlpp::make_char_sequence<sizeof(_literal), _literal>;
      template<typename T>
      struct _member_t
      {
        T pinyin;
        T& operator()() { return pinyin; }
        const T& operator()() const { return pinyin; }
      };
    };
  };
} // namespace doodle
#endif