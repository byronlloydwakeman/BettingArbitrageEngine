//
// Created by 44785 on 31/07/2026.
//

#ifndef BETTINGARBITRAGEENGINE_FEED_HPP
#define BETTINGARBITRAGEENGINE_FEED_HPP
#include <functional>

#include "event.hpp"


class IFeed {
    public:
        virtual void connect() = 0;
        virtual void setCallback(std::function<void(const OddsUpdate&)> callback) = 0;
        virtual void disconnect() = 0;
        virtual ~IFeed() = default;
};


#endif //BETTINGARBITRAGEENGINE_FEED_HPP
