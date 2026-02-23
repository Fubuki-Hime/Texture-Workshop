#pragma once

#include <Geode/Geode.hpp>
#include <Geode/utils/web.hpp>
#include <Geode/loader/Event.hpp>
#include <Geode/ui/TextInput.hpp>
#include <cctype>
#include <algorithm>
#include <Geode/utils/file.hpp>

#include "../../tps/headers/TWSPack.hpp"

using namespace geode::prelude;

class TWSPackCell : public CCLayerColor {
    public:

        TWSPack* texturePack;

        // texture pack info, hey guys its me uproxide this is my awesome comment
        geode::LazySprite* icon;
        CCLabelBMFont* nameLabel;
        CCLabelBMFont* versionLabel;
        CCLabelBMFont* downloadCount;

        static TWSPackCell* create(TWSPack* tp, bool other) {
            TWSPackCell* pRet = new TWSPackCell();
            if (pRet && pRet->init(tp, other)) {
                pRet->autorelease();
                return pRet;
            } else {
                delete pRet;
                return nullptr;
            }
        }

        ~TWSPackCell()
        {
            texturePack = nullptr;
        }
    
    protected:

        bool init(TWSPack* tp, bool other);
};