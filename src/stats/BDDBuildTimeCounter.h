//
// Created by Maxim.Popov on 05.08.2022.
//

#ifndef CODE_BDDBUILDTIMECOUNTER_H
#define CODE_BDDBUILDTIMECOUNTER_H

#include <chrono>

namespace chrono = std::chrono;

class BDDBuildTimeCounter {
  public:
    static BDDBuildTimeCounter* getInstance() {
        if (timeCounter == nullptr) {
            timeCounter = new BDDBuildTimeCounter();
        }
        return timeCounter;
    }

    static void orderingStarted() {
        orderingStart = chrono::steady_clock::now();
    }

    static void orderingFinished() {
        if (orderingStart == chrono::steady_clock::time_point()) {
            orderingStart = chrono::steady_clock::now();
            orderingEnd = constructionStart;
        } else {
            orderingEnd = chrono::steady_clock::now();
        }
    }

    static void constructionStarted() {
        constructionStart = chrono::steady_clock::now();
    }

    static void constructionFinished() {
        if (constructionStart == chrono::steady_clock::time_point()) {
            constructionStart = chrono::steady_clock::now();
            constructionEnd = constructionStart;
        } else {
            constructionEnd = chrono::steady_clock::now();
        }
    }

    static int getOrderingTimeInMilliseconds() {
        if (orderingStart == chrono::steady_clock::time_point()) {
            return 0;
        }

        if (orderingEnd == chrono::steady_clock::time_point()) {
            return chrono::duration_cast<chrono::milliseconds>(
                       chrono::steady_clock::now() - orderingStart)
                .count();
        }

        return chrono::duration_cast<chrono::milliseconds>(
                   orderingEnd - orderingStart)
            .count();
    }

    static int getConstructionTimeInMilliseconds() {
        if (constructionStart == chrono::steady_clock::time_point()) {
            return 0;
        }

        if (constructionEnd == chrono::steady_clock::time_point()) {
            return chrono::duration_cast<chrono::milliseconds>(
                       chrono::steady_clock::now() - constructionStart)
                .count();
        }

        return chrono::duration_cast<chrono::milliseconds>(
                   constructionEnd - constructionStart)
            .count();
    }

    static int getTotalTimeInMilliseconds() {
        return getConstructionTimeInMilliseconds() + getOrderingTimeInMilliseconds();
    }

  protected:
    BDDBuildTimeCounter() {};
    inline static BDDBuildTimeCounter* timeCounter = nullptr;
    inline static chrono::steady_clock::time_point orderingStart = chrono::steady_clock::time_point();
    inline static chrono::steady_clock::time_point orderingEnd = chrono::steady_clock::time_point();
    inline static chrono::steady_clock::time_point constructionStart = chrono::steady_clock::time_point();
    inline static chrono::steady_clock::time_point constructionEnd = chrono::steady_clock::time_point();
};

#endif // CODE_BDDBUILDTIMECOUNTER_H
