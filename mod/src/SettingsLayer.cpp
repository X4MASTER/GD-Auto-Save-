#include "SettingsLayer.hpp"
#include "AutoSave.hpp"

AutoSaveSettingsLayer* AutoSaveSettingsLayer::create() {
    auto ret = new AutoSaveSettingsLayer();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool AutoSaveSettingsLayer::init() {
    if (!CCNode::init()) return false;
    buildUI();
    return true;
}

void AutoSaveSettingsLayer::buildUI() {
    auto win = CCDirector::sharedDirector()->getWinSize();
    auto title = CCLabelBMFont::create("Auto Save Settings", "bigFont.fnt");
    title->setPosition({win.width/2, win.height - 60});
    addChild(title);

    auto autoLabel = CCLabelBMFont::create("Enable Auto Save", "goldFont.fnt");
    autoLabel->setPosition({120, win.height - 120});
    addChild(autoLabel);

    auto toggleOn = CCLabelBMFont::create("Toggle", "goldFont.fnt");
    auto toggleItem = CCMenuItemLabel::create(toggleOn, this, menu_selector(AutoSaveSettingsLayer::onToggleAuto));
    toggleItem->setPosition({win.width - 120, win.height - 120});
    auto menu = CCMenu::create(toggleItem, nullptr);
    menu->setPosition({0,0});
    addChild(menu);

    auto saveNowBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_button_01.png"),
        this, menu_selector(AutoSaveSettingsLayer::onSaveNow)
    );
    auto lbl = CCLabelBMFont::create("Save Now", "goldFont.fnt");
    lbl->setPosition({saveNowBtn->getContentSize().width/2, saveNowBtn->getContentSize().height/2});
    saveNowBtn->addChild(lbl);
    saveNowBtn->setPosition({win.width/2, win.height - 200});
    auto menu2 = CCMenu::create(saveNowBtn, nullptr);
    menu2->setPosition({0,0});
    addChild(menu2);

    auto intervalLabel = CCLabelBMFont::create("Interval (min)", "goldFont.fnt");
    intervalLabel->setPosition({120, win.height - 160});
    addChild(intervalLabel);

    auto inc = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this, menu_selector(AutoSaveSettingsLayer::onIntervalChange)
    );
    inc->setTag(1);
    inc->setPosition({win.width - 80, win.height - 160});
    auto dec = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this, menu_selector(AutoSaveSettingsLayer::onIntervalChange)
    );
    dec->setTag(-1);
    dec->setPosition({win.width - 160, win.height - 160});
    auto menu3 = CCMenu::create(inc, dec, nullptr);
    menu3->setPosition({0,0});
    addChild(menu3);
}

void AutoSaveSettingsLayer::onSaveNow(CCObject*) {
    FLAlertLayer::create("Auto Save", "Trigger the game's save now? This will attempt to simulate the game's save button.", "Yes", "No",
        [](auto, auto){ bool ok = AutoSaveManager::instance().triggerManualSave(); if (ok) FLAlertLayer::create("Auto Save","Save invoked","OK")->show(); else FLAlertLayer::create("Auto Save","Could not find save control","OK")->show(); },
        nullptr)->show();
}

void AutoSaveSettingsLayer::onToggleAuto(CCObject*) {
    AutoSaveManager::instance().settings.autoEnabled = !AutoSaveManager::instance().settings.autoEnabled;
    if (AutoSaveManager::instance().settings.autoEnabled) {
        AutoSaveManager::instance().scheduleNext();
        FLAlertLayer::create("Auto Save", "Auto-trigger enabled. Protections active.", "OK")->show();
    } else {
        FLAlertLayer::create("Auto Save", "Auto-trigger disabled.", "OK")->show();
    }
}

void AutoSaveSettingsLayer::onIntervalChange(CCObject* sender) {
    auto item = static_cast<CCNode*>(sender);
    int delta = item->getTag();
    int cur = AutoSaveManager::instance().settings.intervalMinutes;
    cur += delta;
    if (cur < 10) cur = 10;
    if (cur > 60) cur = 60;
    AutoSaveManager::instance().settings.intervalMinutes = cur;
    FLAlertLayer::create("Auto Save", ("Interval set to " + std::to_string(cur) + " minutes").c_str(), "OK")->show();
}
