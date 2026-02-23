using namespace geode::prelude;

#include "headers/TWSLayer.hpp"
#include "headers/TWSPackCell.hpp"
#include "../tps/headers/TWSPack.hpp"
#include "../header/boobs.hpp"

#include <cctype>
#include <algorithm>
#include <matjson.hpp>
#include <Geode/ui/GeodeUI.hpp>
#include <Geode/loader/Event.hpp>

TWSLayer* TWSLayer::create() {
    auto ret = new TWSLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
};

CCScene* TWSLayer::scene() {
    auto layer = TWSLayer::create();
    auto scene = CCScene::create();
    scene->addChild(layer);
    return scene;
}

bool TWSLayer::init() {
    if(!CCLayer::init())
        return false;

    setID("TWSLayer");

    auto director = CCDirector::sharedDirector();
    auto winSize = director->getWinSize();

    get = this;

    CCSprite* backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    CCMenuItemSpriteExtra* backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(TWSLayer::onClose));
    backBtn->setID("back-button");

    CCMenu* backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->addChild(backBtn);
    addChild(backMenu, 1);

    backMenu->setPosition(ccp(director->getScreenLeft() + 25.f, director->getScreenTop() - 22.f));
    setKeyboardEnabled(true);
    setKeypadEnabled(true);

    m_background = CCLayerGradient::create({181, 69, 20, 255}, {48, 15, 104, 255}, {1, -1});
    m_background->setID("background");
    m_background->setAnchorPoint({ 0.f, 0.f });
    m_background->setContentSize(CCDirector::get()->getWinSize());
    addChild(m_background, -3);

    auto spriteTexture = CCSprite::create("TWS_Cubes.png"_spr);
    spriteTexture->setAnchorPoint({0, 0});

    ccTexParams params = {GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT};
    spriteTexture->getTexture()->setTexParameters(&params);

    auto rect = spriteTexture->getTextureRect();
    rect.size = rect.size * (CCPoint(winSize) / CCPoint(spriteTexture->getScaledContentSize()) * 2);
    rect.origin = CCPoint{0, 0};
    spriteTexture->setTextureRect(rect);
    spriteTexture->setRotation(18);
    spriteTexture->setOpacity(100);
    spriteTexture->setPosition({-winSize.width/2, 0});
    spriteTexture->setID("background-overlay");
    spriteTexture->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA });
    spriteTexture->runAction(CCRepeatForever::create(CCSequence::create(
        CCMoveBy::create(5, {16, 49}),
        CCMoveBy::create(0, {-16, -49}),
        nullptr
    )));

    addChild(spriteTexture, -2);

    auto bg = cocos2d::extension::CCScale9Sprite::create("square02_small.png");
    bg->setID("content-background");
    this->addChild(bg);

    bg->setPosition(winSize / 2);
    bg->setContentHeight(252);
    bg->setContentWidth(380);
    bg->setOpacity(135);
    bg->setPositionY(bg->getPositionY() - 15);

    outline = CCSprite::createWithSpriteFrameName("TWS_Outline.png"_spr);
    outline->setID("outline");
    this->addChild(outline);
    outline->setPosition(winSize / 2);
    outline->setScale(1.2);
    outline->setZOrder(1);

    auto pagesMenu = CCMenu::create();
    pagesMenu->setID("pages-menu");
    outline->addChild(pagesMenu, 1);
    pagesMenu->setPosition(0, 0);

    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
    auto nextPage = CCMenuItemSpriteExtra::create(
        nextPageSpr,
        this,
        menu_selector(TWSLayer::onNextPage)
    );
    pagesMenu->addChild(nextPage);

    auto prevPage = CCMenuItemSpriteExtra::create(
        nextPageSpr,
        this,
        menu_selector(TWSLayer::onPrevPage)
    );
    pagesMenu->addChild(prevPage);
    prevPage->setPositionX(100);

    buttonMenu = CCMenu::create();
    buttonMenu->setID("button-menu");
    addChild(buttonMenu, 1);
    
    buttonMenu->setPosition(0, 0);
    buttonMenu->setContentSize(winSize);

    auto discordSprite = CCSprite::createWithSpriteFrameName("gj_discordIcon_001.png");
    auto discordButton = CCMenuItemSpriteExtra::create(
        discordSprite,
        this,
        menu_selector(TWSLayer::onDiscord)
    );
    discordButton->setID("discord-button");
    buttonMenu->addChild(discordButton);
    discordButton->setPosition(ccp(director->getScreenRight() - 20, director->getScreenBottom() + 20));

    auto supportSprite = CCSprite::createWithSpriteFrameName("geode.loader/gift.png");
    auto supportButton = CCMenuItemSpriteExtra::create(
        supportSprite,
        this,
        menu_selector(TWSLayer::onSupport)
    );
    supportButton->setID("support-button");
    buttonMenu->addChild(supportButton);
    supportButton->setPosition(ccp(director->getScreenRight() - 20, director->getScreenBottom() + 55));

    auto creditsSprite = CCSprite::createWithSpriteFrameName("TWS_Credits.png"_spr);
    creditsSprite->setScale(0.7);
    auto creditsButton = CCMenuItemSpriteExtra::create(
        creditsSprite,
        this,
        menu_selector(TWSLayer::onCredits)
    );
    creditsButton->setID("credits-button");
    buttonMenu->addChild(creditsButton);
    creditsButton->setPosition(ccp(director->getScreenRight() - 20, director->getScreenBottom() + 90));

    auto refreshSpr = CCSprite::createWithSpriteFrameName("TWS_RefreshButton.png"_spr);
    refreshSpr->setScale(0.8);
    refreshButton = CCMenuItemSpriteExtra::create(
        refreshSpr,
        this,
        menu_selector(TWSLayer::onRefresh)
    );
    refreshButton->setID("refresh-button");
    buttonMenu->addChild(refreshButton);
    refreshButton->setPosition(ccp(director->getScreenLeft() + 25, director->getScreenBottom() + 65));
    refreshButton->setVisible(false);

    auto filesSpr = CCSprite::createWithSpriteFrameName("TWS_FileButton.png"_spr);
    filesSpr->setScale(0.8);
    auto filesBtn = CCMenuItemSpriteExtra::create(
        filesSpr,
        this,
        menu_selector(TWSLayer::onPacksFolder)
    );
    filesBtn->setID("files-button");
    buttonMenu->addChild(filesBtn);
    filesBtn->setPosition(ccp(director->getScreenLeft() + 25, director->getScreenBottom() + 25));

    getTexturePacks();

    scroll = ScrollLayer::create(ccp(313, 180));
    scroll->setAnchorPoint(ccp(0, 0));
    scroll->ignoreAnchorPointForPosition(false);
    float basePosY = 143.5;
    scroll->setZOrder(-1);
    scroll->setPosition(8, 8);

    /*TWSPackCell* testTp1 = TWSPackCell::create(TWSPack::create("Test TP", "https://goon.com", "https://textureworkshop.xyz/api/v1/tws/getLogo/131.png", "test tp for thing", "Roxi", "1.0.0", "2.208", false, 1680), false);
    TWSPackCell* testTp2 = TWSPackCell::create(TWSPack::create("Test TP 2", "https://goon.com", "https://textureworkshop.xyz/api/v1/tws/getLogo/9.png", "test tp for thing", "Roxi", "1.0.0", "2.208", true, 10593), true);
    scroll->m_contentLayer->addChild(testTp1);
    scroll->m_contentLayer->addChild(testTp2);
    testTp1->setPosition(0, basePosY);
    testTp2->setPosition(0, basePosY - 35);*/

    outline->addChild(scroll);

    return true;
}

void TWSLayer::getTexturePacks() {
    auto req = geode::utils::web::WebRequest();

    req.onProgress([](geode::utils::web::WebProgress const& progress) {
        log::debug("Progress: ", progress.downloadProgress());
    });

    m_getTPslistener.spawn( 
        req.get(fmt::format("https://textureworkshop.xyz/api/v2/tws/getTPs?page={}", boobs::page)),
        [this](geode::utils::web::WebResponse value) {
            //log::debug("Response: {}", value.code());
            //log::debug("Body: {}", value.string().unwrap());
            if (value.string().unwrap() == "{}") {
                log::info("theres nothing on this page, trying previous page...");
                boobs::page -= 1;
                getTexturePacks();
            } else {
                pageJson = value.json().unwrap();
                log::debug("Page JSON: {}", pageJson.dump());
                setupTPCells();
            }
        }
    );
}

void TWSLayer::setupTPCells() {
    int i = 0;
    scroll->m_contentLayer->setAnchorPoint(ccp(0,1));

    scroll->m_contentLayer->removeAllChildren();

    for (auto& value : pageJson) {
        auto tpObject = value;

        bool featured = false;
        if (tpObject["packFeature"].asInt().unwrap() == 1) {
            featured = true;
        } else {
            featured = false;
        }

        TWSPack* tp = TWSPack::create(
            tpObject["packName"].asString().unwrap(),
            tpObject["downloadLink"].asString().unwrap(),
            tpObject["packLogo"].asString().unwrap(),
            tpObject["packDescription"].asString().unwrap(),
            tpObject["packCreator"].asString().unwrap(),
            tpObject["packVersion"].asString().unwrap(),
            tpObject["gdVersion"].asString().unwrap(),
            featured,
            tpObject["packDownloads"].asInt().unwrap()
        );

        tps.push_back(tp);

        stupid = !stupid;

        auto tpCell = TWSPackCell::create(tp, stupid);
        scroll->m_contentLayer->addChild(tpCell);
        tpCell->setPosition(0, 312 - (35 * i));
        float height = std::max<float>(scroll->getContentSize().height, 35 * scroll->m_contentLayer->getChildrenCount());
        scroll->m_contentLayer->setContentSize(ccp(scroll->m_contentLayer->getContentSize().width, height));
        i++;
    }

    scroll->moveToTop();
}

void TWSLayer::onClose(CCObject*) {
    auto mainMenu = MenuLayer::scene(false);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, mainMenu));
}

void TWSLayer::onDiscord(CCObject*) {
    utils::web::openLinkInBrowser("https://discord.gg/pUGPY9hQ22");
}

void TWSLayer::onSupport(CCObject*) {
    geode::createQuickPopup(
        "Support Me",
        "This mod tragically costs money to host and maintain cause storage is EXPENSIVE! GOD!\nHowever, if you enjoy TWS and want to support its development, consider supporting us on patreon for some cool perks!",
        "No Thanks", "Ok",
        [](auto, bool btn2) {
            if (btn2) {
                utils::web::openLinkInBrowser("https://patreon.com/uproxide");
            }
        }
    );
}

void TWSLayer::onCredits(CCObject*) {
    FLAlertLayer::create(
        "Credits",
        "<cg>Uproxide</c> - Main Developer\n<co>ShineUA and Alphalaneous</c> - Pull Requests :3\n<cr>TheSillyDoggo</c> - Help with Searching\n<cr>M336</c> - Serverside Code\n<cl>Brift</c> - Main Sprite Creator\n<cl>Dremsk1y</c> - Sprites\n<cp>Riley</c> - she spenda the money (hosting)",
        "Ok"
    )->show();
}

void TWSLayer::onRefresh(CCObject*) {
    // TODO
}

void TWSLayer::onPacksFolder(CCObject*) {
    utils::file::openFolder(Loader::get()->getInstalledMod("geode.texture-loader")->getConfigDir());
}

// prev/next page of texture packs
void TWSLayer::onPrevPage(CCObject*) {
    boobs::page -= 1;
    getTexturePacks();
}

void TWSLayer::onNextPage(CCObject*) {
    boobs::page += 1;
    getTexturePacks();
}

void TWSLayer::keyBackClicked() {
    onClose(nullptr);
}

void TWSLayer::textChanged(CCTextInputNode*) {
    // TODO
}

TWSLayer::~TWSLayer()
{
    get = nullptr;
}