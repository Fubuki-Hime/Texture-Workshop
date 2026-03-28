#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <cctype>
#include <algorithm>
#include <Geode/utils/file.hpp>

using namespace geode::prelude;

class TWSFilters: public Popup {
public:
    static TWSFilters* create() {
        auto popup = new TWSFilters;
        if (popup->init()) {
            popup->autorelease();
            return popup;
        }
        delete popup;
        return nullptr;
    }

protected:
    bool init();

    void onToggle(CCObject* sender);
    void onClose();

    bool oldFeaturedValue;
    bool oldDownloadsValue;
    bool oldRecentValue;

    ListenerHandle imListeningToTheFilters;

    CCMenuItemToggler* featuredToggle;
    CCMenuItemToggler* downloadsToggle;
    CCMenuItemToggler* recentToggle;
};