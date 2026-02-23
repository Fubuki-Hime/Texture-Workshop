#include "./headers/TWSPackCell.hpp"

bool TWSPackCell::init(TWSPack* tp, bool other) {
    if (!CCLayerColor::init())
        return false;

    texturePack = tp;

    int reloadIconTries = 0;

    if (other) {
        this->setOpacity(100);
    } else {
        this->setOpacity(50); 
    }

    this->setContentSize(ccp(315, 35));
    this->setAnchorPoint(ccp(0, 1));
    
    // thank you geode lead developers for the awesome creation that is lazy sprite, i love you so much. mwah.
    icon = geode::LazySprite::create({100, 100});
    icon->setLoadCallback(
        [this, &reloadIconTries, tp](Result<> result) {
            if (!result.isOk()) 
            {
                if (reloadIconTries < 3) {
                    log::info("failed to load icon, please refresh this TWS page to try again."); 
                    // fallback
                    icon->initWithSpriteFrameName("TWS_Error.png"_spr);
                } else {
                    icon->loadFromUrl(tp->IconURL, geode::LazySprite::Format::kFmtPng);
                    reloadIconTries += 1;
                }
            }
        }
    );
    this->addChild(icon);
    icon->loadFromUrl(tp->IconURL, geode::LazySprite::Format::kFmtPng);
    icon->setScale(0.35);
    icon->setPosition({ 18, this->getContentSize().height / 2 });

    // name setup yay
    nameLabel = CCLabelBMFont::create(tp->TPName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.4);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition({35, 25}); // OH BOY I LOVE HARDCODING!
    this->addChild(nameLabel);

    versionLabel = CCLabelBMFont::create(tp->TPVersion.c_str(), "bigFont.fnt");
    versionLabel->setScale(0.2);
    versionLabel->setAnchorPoint(ccp(0, 1));
    versionLabel->setPosition({35, 17}); // OH BOY I LOVE HARDCODING!
    this->addChild(versionLabel);

    downloadCount = CCLabelBMFont::create(fmt::format("{}", tp->downloads).c_str(), "bigFont.fnt");
    downloadCount->setScale(0.2);
    downloadCount->setAnchorPoint(ccp(0, 1));
    downloadCount->setPosition({35 + versionLabel->getPosition().x + 5, 17}); // OH BOY I LOVE HARDCODING!
    this->addChild(downloadCount);

    return true;
};
