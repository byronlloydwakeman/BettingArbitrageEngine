//
// Created by 44785 on 02/08/2026.
//

#include "orderBook.hpp"

void OrderBook::update(const OddsUpdate& update) {
    {
        std::unique_lock lock(mtx_order_book_);
        order_book_[update.event_id][update.outcome].insert_or_assign(update.bookmaker_id, update);
    }
}

std::vector<OddsUpdate> OrderBook::getOddsFor(
    const std::string& event_id,
    const std::string& outcome
) const {
    std::unique_lock lock(mtx_order_book_);

    std::vector<OddsUpdate> result;

    auto event_id_key = order_book_.find(event_id);
    if (event_id_key == order_book_.end()) {
        return result;
    }

    auto outcome_key = event_id_key->second.find(outcome);
    if (outcome_key == event_id_key->second.end()) {
        return result;
    }

    for (const auto& [bookmaker_id, updateOdds] : outcome_key->second) {
        result.emplace_back(
            event_id,
            outcome,
            bookmaker_id,
            updateOdds.odds,
            updateOdds.timestamp
        );
    }

    return result;
}

void OrderBook::removeOddsFor(
    const std::string& event_id,
    double odds,
    const std::string& outcome,
    const std::string& bookmaker
) {
    std::unique_lock lock(mtx_order_book_);

    auto event_id_key = order_book_.find(event_id);
    if (event_id_key == order_book_.end()) {
        return;
    }

    auto outcome_key = event_id_key->second.find(outcome);
    if (outcome_key == event_id_key->second.end()) {
        return;
    }

    auto bookmaker_key = outcome_key->second.find(bookmaker);
    if (bookmaker_key == outcome_key->second.end()) {
        return;
    }

    // Only remove if the odds still match what was used to detect the arb, otherwise we could
    // be discarding a newer price update that arrived after the arb was found.
    if (bookmaker_key->second.odds == odds) {
        outcome_key->second.erase(bookmaker_key);
    }
}
