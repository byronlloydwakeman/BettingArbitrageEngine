//
// Created by 44785 on 22/08/2026.
//

#ifndef BETTINGARBITRAGEENGINE_FOOTBALLOUTCOME_HPP
#define BETTINGARBITRAGEENGINE_FOOTBALLOUTCOME_HPP

#include <unordered_map>

enum class FootballOutcome {
    HomeWin,
    Draw,
    AwayWin
};

inline std::string toString(FootballOutcome o) {
    switch (o) {
        case FootballOutcome::HomeWin: return "HomeWin";
        case FootballOutcome::Draw:    return "Draw";
        case FootballOutcome::AwayWin: return "AwayWin";
    }
    return "Unknown";
}

inline std::optional<FootballOutcome> toEnum(std::string o) {
    std::unordered_map<std::string, FootballOutcome> table = {
        {"HomeWin", FootballOutcome::HomeWin},
        {"Draw", FootballOutcome::Draw},
        {"AwayWin", FootballOutcome::AwayWin}
    };

    if (table.contains(o)) {
        return table[o];
    }

    return std::nullopt;
}

#endif //BETTINGARBITRAGEENGINE_FOOTBALLOUTCOME_HPP
