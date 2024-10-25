#pragma once
#include <hv/json.hpp>
#include <ranges>
#include <string_view>
#include <unordered_map>
#include <vector>

#include "message_generated.h"

using QuoteMap = std::unordered_map<Messages::Payload, std::vector<std::string>>;

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

inline QuoteMap parse_expr(std::string_view str) {
    // {"tick":"600022|600000", "k1d":"000001|300116", "order":"000001|300116"}
    auto j = nlohmann::json::parse(str);
    QuoteMap map{};
    for (auto&& [k, v] : j.items()) {
        auto quote_type = parse_quote_type(k);
        auto range = v.get<std::string_view>() | std::views::split('|') | std::views::transform([](auto&& rng) { return std::string_view(rng.begin(), rng.end()); });
        std::vector<std::string> symbols{range.begin(), range.end()};
        map[quote_type] = std::move(symbols);
    }
    return map;
}