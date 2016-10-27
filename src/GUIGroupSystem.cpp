#include "GUIGroupSystem.h"

void GUIGroupSystem::onCreateGUIGroup(Entity* e){
    if (e->get<CGUIGroup>()->groupType == "window" && e->get<CGUIGroup>()->groupID == "single-player"){
        createWindowSinglePlayer(e); 
    }else if (e->get<CGUIGroup>()->groupType == "window-page"){
     
    }
}

void GUIGroupSystem::createWindowSinglePlayer(Entity* e){
    // e has CGUIGroup
    e->addObservedEntity("page-first", eManager->createEntity());
    e->addObservedEntity("page-quick-match", eManager->createEntity());

    e->attachEmployee(e->getObservedEntity("page-first"));
    e->attachEmployee(e->getObservedEntity("page-quick-match"));

    Entity* eObj;

    // First page:
    double wButton = 180;
    double hButton = 40;
    double spcButton = 50;
    double x0 = cxWindow;
    double y0 = cyWindow - spcButton/2;
    double x,y;

    // Title
    eObj = eManager->createEntity();
    eObj->add(new CTextbox2());

    /*
    pages.clear();
    pages.resize(CInGameMenuPage::N_PAGES);
    

    Entity* eObj;

    double w = 180;
    double h = 40;
    double spacing = 50;
    double x0 = cxWindow;
    double y0 = cyWindow - spacing/2;
    double x = x0;
    double y = y0;

    ePanel = eManager->createEntity();
    ePanel->add(new CTexture("9p-grey-frame.png"));
    ePanel->add(new CDimensions(290, 300));
    ePanel->add(new CPosition(cxWindow, cyWindow));
    ePanel->add(new CDraw(CDraw::GUI_00)); ePanel->add(new CPanel()); 
    ///=====================
    /// FIRST PAGE
    ///=====================
    x0 = cxWindow;
    y0 = 250;
    x = x0;
    y = y0;
    /// BUTTON RESUME
    y += spacing;
    eObj = createRectButton(Assets::getString("BUTTON-RESUME"), 18, 40, x, y, sf::Color::White, sf::Color::White, 1, RESUME_GAME);
    eObj->add(new CInGameMenuPage(CInGameMenuPage::FIRST));
    eObj->get<CButtonTrigger>()->hotkey = sf::Keyboard::Escape;
    addToPage(eObj, CInGameMenuPage::FIRST);
    */
}









