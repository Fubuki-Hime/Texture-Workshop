#include "./headers/TWSPackInfo.hpp"

bool TWSPackInfo::init(TWSPack* tp)
{
    if (!Popup::init(335.f, 231.f, "TWS_Box.png"_spr))
        return false;

    int reloadIconTries = 0;

    float scale = CCDirector::sharedDirector()->getContentScaleFactor()/4;

    LazySprite* icon = LazySprite::create({100, 100});
    icon->setLoadCallback(
        [this, &reloadIconTries, tp, icon](Result<> result) {
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

            if (tp->featured) {
                auto featuredSpr = CCSprite::createWithSpriteFrameName("TWS_Featured.png"_spr);
                featuredSpr->setScale(0.6);
                featuredSpr->setPosition({ 45, 189.5 });
                this->m_mainLayer->addChild(featuredSpr);
            }
        }
    );
    this->m_mainLayer->addChild(icon);
    icon->loadFromUrl(tp->IconURL, geode::LazySprite::Format::kFmtPng);
    icon->setScale(0.6 * scale);
    icon->setPosition({ 45, 189.5 });
    icon->setZOrder(1);

    auto line = CCSprite::createWithSpriteFrameName("floorLine_001.png");
    this->m_mainLayer->addChild(line);
    line->setPosition(this->m_mainLayer->getContentSize() / 2);
    line->setPositionY(line->getPositionY() + 40);
    line->setScale(0.675);

    std::string fullDesc = fmt::format("# {}\n{}", tp->TPName, tp->TPDescription);

    auto desc = MDTextArea::create(fullDesc, ccp(300, 135));
    this->m_mainLayer->addChild(desc);
    desc->setPosition(line->getPosition());
    desc->setPositionY(desc->getPositionY() - 75);

    auto nameLabel = CCLabelBMFont::create(tp->TPName.c_str(), "bigFont.fnt");
    nameLabel->setScale(0.5);
    nameLabel->setAnchorPoint(ccp(0, 0.5f));
    nameLabel->setPosition({76.5, 206.5}); // OH BOY I LOVE HARDCODING!
    nameLabel->setZOrder(1);
    this->m_mainLayer->addChild(nameLabel);

    auto creatorLabel = CCLabelBMFont::create(fmt::format("By: {}", tp->TPCreator).c_str(), "goldFont.fnt");
    creatorLabel->setScale(0.45);
    creatorLabel->setAnchorPoint(ccp(0, 0.5f));
    creatorLabel->setPosition({76.5, 194.5}); // OH BOY I LOVE HARDCODING!
    creatorLabel->setZOrder(1);
    this->m_mainLayer->addChild(creatorLabel);

    return true;
}