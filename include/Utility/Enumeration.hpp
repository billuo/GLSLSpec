#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>


/// Enumeration database
template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
struct EDB {
    static const std::vector<std::pair<Enum, std::string>> database;
};

/// Enumeration
template <typename Enum, typename = std::enable_if_t<std::is_enum_v<Enum>>>
struct E : public EDB<Enum> {
  public:
    using Base = EDB<Enum>;

    static std::string to_string(Enum e)
    {
        static const std::map<Enum, std::string> enum_string = aux_init_enum_string();
        auto it = enum_string.find(e);
        if (it != enum_string.end()) {
            return it->second;
        }
        return {};
    }

    static Enum to_enum(const std::string& s)
    {
        static const std::map<std::string, Enum> string_enum = aux_init_string_enum();
        auto it = string_enum.find(s);
        if (it != string_enum.end()) {
            return it->second;
        }
        return static_cast<Enum>(-1);
    }

    E(Enum e) : value(e)
    {}

    ~E() = default;

    auto to_string() const
    { return to_string(value); }

    friend std::ostream& operator<<(std::ostream& os, E rhs)
    { return os << rhs.to_string(); }

  private:
    Enum value;

    static auto aux_init_enum_string()
    {
        std::map<Enum, std::string> ret;
        for (auto&&[e, s] : Base::database) {
            ret[e] = s;
        }
        return ret;
    }

    static auto aux_init_string_enum()
    {
        std::map<std::string, Enum> ret;
        for (auto&&[e, s] : Base::database) {
            ret[s] = e;
        }
        return ret;
    }

};

#define DEFINE_ENUMERATION_DATABASE(type) template <> decltype(EDB<type>::database) EDB<type>::database =


