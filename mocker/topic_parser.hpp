#pragma once
#include <hv/json.hpp>
#include <ranges>
#include <string_view>
#include <vector>

#include "message_generated.h"

struct Subject {
    Messages::Payload quote_type;
    std::vector<std::string> symbols;
};

inline Messages::Payload parse_quote_type(std::string_view str) {
    if (str == "order") {
        return Messages::Payload::OrderData;
    } else if (str == "trade") {
        return Messages::Payload::TradeData;
    } else if (str == "tick") {
        return Messages::Payload::TickData;
    } else if (str == "k1min") {
        return Messages::Payload::K1min;
    } else if (str == "k1d") {
        return Messages::Payload::K1d;
    } else if (str == "k1w") {
        return Messages::Payload::K1w;
    } else if (str == "k1mon") {
        return Messages::Payload::K1mon;
    } else {
        return Messages::Payload::ErrorData;
    }
}

inline std::vector<Subject> parse_expr(std::string_view str) {
    // {"tick":"600022|600000", "k1d":"000001|300116", "order":"000001|300116"}
    auto j = nlohmann::json::parse(str);
    std::vector<Subject> subjects;
    subjects.reserve(j.size());
    for (auto&& [k, v] : j.items()) {
        Subject s{};
        s.quote_type = parse_quote_type(k);
        auto range = v.get<std::string_view>() | std::views::split('|') | std::views::transform([](auto&& rng) { return std::string_view(rng.begin(), rng.end()); });
        for (auto&& e : range) {
            s.symbols.emplace_back(e);
        }
        subjects.emplace_back(s);
    }
    return subjects;
}