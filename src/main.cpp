#include <iostream>

#include "ingestion.hpp"
#include "mockFeed.hpp"
#include "orderBook.hpp"
#include "footballComputeQueue.hpp"

int main() {
    std::vector<std::unique_ptr<IFeed>> feeds;
    feeds.push_back(std::make_unique<MockFeed>("Bet365"));
    feeds.push_back(std::make_unique<MockFeed>("BetFred"));
    feeds.push_back(std::make_unique<MockFeed>("Ladbrooks"));

    std::mutex cout_mutex;

    IngestionQueue queue;
    OrderBook order_book;

    // Currently built to handle football odds
    FootballComputeQueue compute_queue;

    compute_queue.startConsuming(order_book);

    // Almost like updating the big nasdaq stock screen with all the prices available at a given time
    queue.startConsuming([&cout_mutex, &order_book, &compute_queue](const auto& feed) {
        std::lock_guard lock(cout_mutex);
        order_book.update(feed);

        // When the order book is updated, we then should perform a compute to find arb
        // But only with the event that has changed
        compute_queue.push({feed.event_id});
    });

    // Queue to handle high concurrency
    for (const auto& feed : feeds) {
        feed->setCallback([&queue](const OddsUpdate& update) {
            queue.push(update);
        });
    }

    // Manually start the mock feeds
    for (const auto& feed : feeds) {
        feed->connect();
    }

    // Actually go through the order_book at search for arb opportunities
    for (int i = 0; i < 500; i++ ) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (auto& feed: feeds) {
        feed->disconnect();
    }

    queue.stop();
    compute_queue.stop();
};
