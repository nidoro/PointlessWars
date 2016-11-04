#include "EditorSystem.h"

EditorSystem::EditorSystem(){
    addSubscription(INITIALIZE_EDITOR);
    addSubscription(SET_PREVIEW_BACKGROUND);
    addSubscription(ADD_OBJECT_TO_PREVIEW);
    addSubscription(DROP);
    addSubscription(BT_EDITOR_TEST);
    addSubscription(BT_EDITOR_LOAD);
    addSubscription(BT_EDITOR_CLEAR);
    addSubscription(BT_EDITOR_SAVE);
    addSubscription(LOAD_SCENARIO_PREVIEW);
    addSubscription(SAVE_SCENARIO);
    addSubscription(TEST_SCENARIO);
    addSubscription(END_SCENARIO_TEST);
    eLoadPanel = nullptr;
}

EditorSystem::~EditorSystem(){
    //dtor
}

void EditorSystem::onInitializeEditor(Entity* e){
    eManager->clearSystem();

    height = 0.8*hWindow;
    width = height*16/9;
    x0 = 10 + 0.10*hWindow*16/9 + 10;
    y0 = 10 + 0.065*hWindow + 10;
    xc = x0 + width/2;
    yc = y0 + height/2;
    xScale = width / wWindow;
    yScale = height / hWindow;

    eBack = eManager->createEntity();
    eBack->add(new CTexture("yellow.png"));
    eBack->add(new CDimensions(width, height));
    eBack->add(new CPosition(xc, yc));
    eBack->add(new CDraw());

    createBackgroundOptions();
    createObjectOptions();
    createMenuOptions();
    loadScenario("temp");
}

void EditorSystem::createBackgroundOptions(){
    list<string> backgrounds = Assets::getBackgrounds();

	double hMiniature = 0.10*hWindow;
	double wMiniature = hMiniature*16/9;

	EntityList cells;

	for(list<string>::iterator i = backgrounds.begin(); i != backgrounds.end(); i++){
        Entity* e = eManager->createEntity();
        e->add(new CTexture(*i));
        e->add(new CPosition());
        e->add(new CDraw());
        e->add(new CDimensions(wMiniature, hMiniature));
        e->add(new CButtonHitbox(wMiniature, hMiniature));
        e->add(new CButtonState());
        e->add(new CButtonTrigger(SET_PREVIEW_BACKGROUND));
        cells.push_back(e);
	}

    Entity* bgList = eManager->createEntity();
    bgList->add(new CScrollList(cells));
    bgList->get<CScrollList>()->orientation = CScrollList::VERTICAL;
    bgList->get<CScrollList>()->size = 6;
    bgList->get<CScrollList>()->spacing = 10;
    bgList->add(new CPosition(wMiniature/2 + 10, 0.065*hWindow + 10 + 32));
    notify(INITIALIZE_SCROLL_LIST, bgList);
}

void EditorSystem::createObjectOptions(){

	double hMiniature = 0.065*hWindow;
	double wMiniature = hMiniature;

	EntityList cells;

	for(map<string, CScenarioObject>::iterator i = CScenarioObject::Map.begin(); i != CScenarioObject::Map.end(); i++){
        CScenarioObject obj = i->second;
        Entity* e = eManager->createEntity();
        e->add(new CAnimation(false, obj.aDefault));
        e->add(new CPosition());
        e->add(new CDraw());
        e->add(new CDimensions(wMiniature, hMiniature));
        e->add(new CButtonHitbox(wMiniature, hMiniature));
        e->add(new CButtonState());
        e->add(new CButtonTrigger(ADD_OBJECT_TO_PREVIEW, CButtonTrigger::ON_PRESS));
        e->add(new CScenarioObject(obj));
        cells.push_back(e);
	}

    Entity* objList = eManager->createEntity();
    objList->add(new CScrollList(cells));
    objList->get<CScrollList>()->orientation = CScrollList::HORIZONTAL;
    objList->get<CScrollList>()->size = 16;
    objList->get<CScrollList>()->spacing = 10;
    objList->add(new CPosition(0.10*hWindow*16/9 + 10 + 10 + wMiniature/2, hMiniature/2 + 10));
    notify(INITIALIZE_SCROLL_LIST, objList);
}

void EditorSystem::onSetPreviewBackground(Entity* e){
    eBack->get<CTexture>()->file = e->get<CTexture>()->file;
}

void EditorSystem::createMenuOptions(){

    double w = 80;
    double h = 40;

    Entity* eBtLoad = eManager->createEntity();
    Entity* eBtSave = eManager->createEntity();
    Entity* eBtClear = eManager->createEntity();
    Entity* eBtTest = eManager->createEntity();

    /// BUTTON LABEL COMMON PROPERTIES
    sf::Text label;
    label.setFont(Assets::getFont(Assets::getPrimaryFont()));
    label.setCharacterSize(18);

    ///LOAD
    label.setString(Assets::getString("BUTTON-EDITOR-LOAD"));
    eBtLoad->add(new CTexture());
    eBtLoad->add(new CPosition(wWindow - 10 - w/2, y0 + h/2 + 0*(h + 10)));
    eBtLoad->add(new CDimensions(w, h));
    eBtLoad->add(new CButtonHitbox(w, h));
    eBtLoad->add(new CButtonState());
    eBtLoad->add(new CButtonTrigger(BT_EDITOR_LOAD));
    eBtLoad->add(new CDefaultTexture("bt-04-default.png"));
    eBtLoad->add(new CHoverTexture("bt-04-hovered.png"));
    eBtLoad->add(new CActiveTexture("bt-04-hovered.png"));
    eBtLoad->add(new CDraw(CDraw::GUI1));
    eBtLoad->add(new CTextbox2(label, 0, 0, CTextbox2::CENTRALIZED));
    eBtLoad->add(new CButtonSounds("click2.wav", "rollover2.wav"));

    ///SAVE
    label.setString(Assets::getString("BUTTON-EDITOR-SAVE"));
    eBtSave->add(new CTexture());
    eBtSave->add(new CPosition(wWindow - 10 - w/2, y0 + h/2 + 1*(h + 10)));
    eBtSave->add(new CDimensions(w, h));
    eBtSave->add(new CButtonHitbox(w, h));
    eBtSave->add(new CButtonState());
    eBtSave->add(new CButtonTrigger(BT_EDITOR_SAVE));
    eBtSave->add(new CDefaultTexture("bt-04-default.png"));
    eBtSave->add(new CHoverTexture("bt-04-hovered.png"));
    eBtSave->add(new CActiveTexture("bt-04-hovered.png"));
    eBtSave->add(new CDraw(CDraw::GUI1));
    eBtSave->add(new CTextbox2(label, 0, 0, CTextbox2::CENTRALIZED));
    eBtSave->add(new CButtonSounds("click2.wav", "rollover2.wav"));

    ///CLEAR
    label.setString(Assets::getString("BUTTON-EDITOR-CLEAR"));
    eBtClear->add(new CTexture());
    eBtClear->add(new CPosition(wWindow - 10 - w/2, y0 + h/2 + 2*(h + 10)));
    eBtClear->add(new CDimensions(w, h));
    eBtClear->add(new CButtonHitbox(w, h));
    eBtClear->add(new CButtonState());
    eBtClear->add(new CButtonTrigger(BT_EDITOR_CLEAR));
    eBtClear->add(new CDefaultTexture("bt-04-default.png"));
    eBtClear->add(new CHoverTexture("bt-04-hovered.png"));
    eBtClear->add(new CActiveTexture("bt-04-hovered.png"));
    eBtClear->add(new CDraw(CDraw::GUI1));
    eBtClear->add(new CTextbox2(label, 0, 0, CTextbox2::CENTRALIZED));
    eBtClear->add(new CButtonSounds("click2.wav", "rollover2.wav"));

    ///TEST
    label.setString(Assets::getString("BUTTON-EDITOR-TEST"));
    eBtTest->add(new CTexture());
    eBtTest->add(new CPosition(wWindow - 10 - w/2, y0 + h/2 + 3 *(h + 10)));
    eBtTest->add(new CDimensions(w, h));
    eBtTest->add(new CButtonHitbox(w, h));
    eBtTest->add(new CButtonState());
    eBtTest->add(new CButtonTrigger(BT_EDITOR_TEST));
    eBtTest->add(new CDefaultTexture("bt-04-default.png"));
    eBtTest->add(new CHoverTexture("bt-04-hovered.png"));
    eBtTest->add(new CActiveTexture("bt-04-hovered.png"));
    eBtTest->add(new CDraw(CDraw::GUI1));
    eBtTest->add(new CTextbox2(label, 0, 0, CTextbox2::CENTRALIZED));
    eBtTest->add(new CButtonSounds("click2.wav", "rollover2.wav"));
}

void EditorSystem::onAddObjectToPreview(Entity* e){
    Entity* eObj = eManager->createEntity();
    CScenarioObject obj = *e->get<CScenarioObject>();
    double w = Assets::getAnimation(obj.aDefault).wSprite*xScale;
    double h = Assets::getAnimation(obj.aDefault).hSprite*yScale;
    eObj->add(new CAnimation(false, obj.aDefault));
    eObj->add(new CPosition(cxWindow, cyWindow));
    eObj->add(new CDraw(CDraw::WORLD));
    eObj->add(new CDimensions(w, h));
    eObj->add(new CButtonHitbox(w, h));
    eObj->add(new CButtonState());
    eObj->add(new CButtonTrigger(DRAG, CButtonTrigger::ON_PRESS));
    eObj->add(new CScenarioObject(obj));
    eObj->add(new CDragDrop(true));
    notify(DRAG, eObj);
    objects.push_back(eObj);
}

void EditorSystem::onDrop(Entity* e){
    double ex0 = e->get<CPosition>()->x - e->get<CDimensions>()->width/2;
    double ey0 = e->get<CPosition>()->y - e->get<CDimensions>()->height/2;
    if (ex0+e->get<CDimensions>()->width < x0 || ex0 > x0+width || ey0+e->get<CDimensions>()->height < y0 || ey0 > y0+height){
        objects.remove(e);
        eManager->removeEntity(e);
    }
}

void EditorSystem::onBtEditorTest(Entity* e){
    Entity* eFade = eManager->createEntity();
    eFade->add(new CFadeTransition());
    eFade->get<CFadeTransition>()->color = sf::Color::Black;
    eFade->get<CFadeTransition>()->endMessage = TEST_SCENARIO;
    notify(START_FADE_OUT_TRANSITION, eFade);
}

void EditorSystem::onTestScenario(Entity* e){
    saveScenario("temp");
    clearPreview();
    eManager->clearSystem();
    Entity* eScenario = eManager->createEntity();
    eScenario->add(new CScenario("temp"));
    eScenario->add(new CButtonState());
    eScenario->add(new CButtonHitbox());
    eScenario->add(new CButtonTrigger(END_SCENARIO_TEST, CButtonTrigger::ON_RELEASE, sf::Keyboard::Escape));
    eScenario->add(new CPosition());
    notify(LOAD_SCENARIO, eScenario);
}

void EditorSystem::onBtEditorLoad(Entity* e){
    createLoadInputBox();
    //loadScenario("temp.dat");
}

void EditorSystem::createLoadInputBox(){
    eLoadPanel = eManager->createEntity();
    eLoadPanel->add(new CPosition(xc, yc));
    eLoadPanel->add(new CTexture("frame-600-200.png"));
    eLoadPanel->add(new CDimensions(0.6*width, 0.08*height));
    eLoadPanel->add(new CDraw(CDraw::GUI1));
    eLoadPanel->add(new CPanel());

    double wPanel = eLoadPanel->get<CDimensions>()->width;
    double hPanel = eLoadPanel->get<CDimensions>()->height;
    double xOri = xc - wPanel/2;
    double yOri = yc - hPanel/2;

    sf::Text label;
    sf::Text txt;

    int cSize = 18;
    label.setFont(Assets::getFont(Assets::getPrimaryFont()));
    label.setCharacterSize(cSize);
    label.setFillColor(sf::Color::White);

    ///LABEL
    Entity* eLabel1 = eManager->createEntity();
    label.setString("File name:");
    eLabel1->add(new CTextbox2(label, 0, 0));
    eLabel1->add(new CPosition(xOri + 0.045*wPanel, yOri + hPanel/2 - cSize/2));
    eLabel1->add(new CDraw(CDraw::GUI1));

    ///INPUT TEXT BOX
    eLoadInput = eManager->createEntity();
    eLoadInput->add(new CTexture("tooltip-01.png"));
    eLoadInput->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel/2, yOri + hPanel/2));
    eLoadInput->add(new CDimensions(0.6*wPanel, 0.7*hPanel));
    eLoadInput->add(new CDraw(CDraw::GUI1));
    eLoadInput->add(new CButtonHitbox(0.6*wPanel, 0.7*hPanel));
    eLoadInput->add(new CButtonState());
    eLoadInput->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    eLoadInput->add(new CDefaultTexture("tooltip-01.png"));
    eLoadInput->add(new CDisplayer(CDisplayer::INPUT_TEXT, eLoadInput));
    //eLoadInput->add(new CInputTextBox(true, CInputTextBox::ACTIVE, 25));

    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(18);
    txt.setFillColor(sf::Color::Black);

    eLoadInput->add(new CTextbox2(txt, -0.55*wPanel/2, -cSize/2));

    ///CONFIRM BUTTON
    Entity* eOk = eManager->createEntity();
    eOk->add(new CTexture());
    eOk->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel + 0.06*wPanel/2, yOri + hPanel/2));
    eOk->add(new CDimensions(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonHitbox(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonState());
    eOk->add(new CButtonTrigger(LOAD_SCENARIO_PREVIEW, CButtonTrigger::ON_RELEASE, sf::Keyboard::Return));
    eOk->add(new CDefaultTexture("ok-button.png"));
    eOk->add(new CHoverTexture("ok-button.png"));
    eOk->add(new CActiveTexture("ok-button.png"));
    eOk->add(new CDraw(CDraw::GUI1));
    eOk->add(new CButtonSounds("click2.wav", "rollover2.wav"));


    ///CANCEL BUTTON
    Entity* eCancel = eManager->createEntity();
    eCancel->add(new CTexture());
    eCancel->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel + 0.06*wPanel/2 + 0.06*wPanel, yOri + hPanel/2));
    eCancel->add(new CDimensions(0.7*hPanel, 0.7*hPanel));
    eCancel->add(new CButtonHitbox(0.7*hPanel, 0.7*hPanel));
    eCancel->add(new CButtonState());
    eCancel->add(new CButtonTrigger(CLOSE_PANEL, CButtonTrigger::ON_RELEASE, sf::Keyboard::Escape));
    eCancel->add(new CDefaultTexture("cancel-button.png"));
    eCancel->add(new CHoverTexture("cancel-button.png"));
    eCancel->add(new CActiveTexture("cancel-button.png"));
    eCancel->add(new CDraw(CDraw::GUI1));
    eCancel->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    eCancel->add(new CParentPanel(eLoadPanel));

    notify(ACTIVATE_INPUT_TEXT_BOX, eLoadInput);

    notify(BEGIN_PANEL, eLoadPanel);
    notify(ATTACH_ENTITY_TO_PANEL, eLabel1);
    notify(ATTACH_ENTITY_TO_PANEL, eLoadInput);
    notify(ATTACH_ENTITY_TO_PANEL, eOk);
    notify(ATTACH_ENTITY_TO_PANEL, eCancel);
    notify(END_PANEL);
}

void EditorSystem::saveScenario(string name){
    string path = "../rsc-0.1/scenarios/" + name + ".sce";
    FILE* file = fopen(path.c_str(), "w");
    fprintf(file, "%%Background\n");
    fprintf(file, "%s\n\n", eBack->get<CTexture>()->file.c_str());
    fprintf(file, "%%%20s %15s %15s %10s %10s %10s %10s\n", "OBJECT-NAME", "X-RELATIVE", "Y-RELATIVE", "X-SCALE", "Y-SCALE", "H-FLIP", "V-FLIP");

    for(EntityListIt i = objects.begin(); i != objects.end(); i++){
        Entity* e = *i;
        string objName = e->get<CScenarioObject>()->id;
        double x = (e->get<CPosition>()->x - x0)/width;
        double y = (e->get<CPosition>()->y - y0)/height;
        double xSc = e->get<CDimensions>()->width/(Assets::getAnimation(e->get<CScenarioObject>()->aDefault).wSprite*xScale);
        double ySc = e->get<CDimensions>()->height/(Assets::getAnimation(e->get<CScenarioObject>()->aDefault).hSprite*yScale);
        int hFlip = e->get<CAnimation>()->hFlip;
        int vFlip = e->get<CAnimation>()->vFlip;
        fprintf(file, "%21s %15.4f %15.4f %10.4f %10.4f %10d %10d\n", objName.c_str(), x, y, xSc, ySc, hFlip, vFlip);
    }

    fclose(file);
}

bool EditorSystem::loadScenario(string name){
    CScenario scenario;
    scenario.name = name;
    ifstream file(string(string("../rsc-0.1/scenarios/") + name + ".sce").c_str());
    if (!file.is_open()){
        printf("%s not found!\n", name.c_str());
        return false;
    }
    string line;
    while(!file.eof()){
        if (line.size() != 0 && line[0] != '%'){
            istringstream ss(line);
            if (scenario.background.empty()){
                ss >> scenario.background;
            }else{
                CScenarioObject object;
                int column = 0;
                while(++column <= 7){
                    switch(column){
                        case 1: ss >> object.id; break;
                        case 2: ss >> object.xRelative; break;
                        case 3: ss >> object.yRelative; break;
                        case 4: ss >> object.xScale; break;
                        case 5: ss >> object.yScale; break;
                        case 6: ss >> object.hFlip; break;
                        case 7: ss >> object.vFlip; break;
                        default: break;
                    }
                }
                scenario.objects.push_back(object);
            }
        }
        getline(file, line);
    }

    createPreview(scenario);
    return true;
}

void EditorSystem::createPreview(CScenario scenario){
    clearPreview();
    eBack->get<CTexture>()->file = scenario.background;
    for(list<CScenarioObject>::iterator i = scenario.objects.begin(); i != scenario.objects.end(); i++){
        Entity* eObj = eManager->createEntity();
        CScenarioObject object = *i;
        double w = CScenarioObject::Map[i->id].width*xScale*object.xScale;
        double h = CScenarioObject::Map[i->id].height*yScale*object.yScale;
        eObj->add(new CAnimation(object.hFlip, CScenarioObject::Map[i->id].aDefault, object.vFlip));
        eObj->add(new CDraw(CDraw::WORLD));
        eObj->add(new CPosition(x0 + i->xRelative*width, y0 + i->yRelative*height));
        eObj->add(new CDimensions(w, h));
        eObj->add(new CButtonHitbox(w, h));
        eObj->add(new CButtonState());
        eObj->add(new CButtonTrigger(DRAG, CButtonTrigger::ON_PRESS));
        eObj->add(new CScenarioObject(CScenarioObject::Map[i->id]));
        eObj->add(new CDragDrop(true));
        objects.push_back(eObj);
    }
}

void EditorSystem::clearPreview(){
    for(EntityListIt i = objects.begin(); i != objects.end(); i++){
        Entity* e = *i;
        eManager->removeEntity(e);
    }
    objects.clear();
}

void EditorSystem::onBtEditorSave(Entity* e){
    createSaveInputBox();
}

void EditorSystem::onLoadScenarioPreview(Entity*e){
    string name = eLoadInput->get<CTextbox2>()->content.getString();
    loadScenario(name);
    notify(REMOVE_PANEL, eLoadPanel);
}

void EditorSystem::onBtEditorClear(Entity* e){
    clearPreview();
}

void EditorSystem::onSaveScenario(Entity* e){
    string name = eSaveInput->get<CTextbox2>()->content.getString();
    saveScenario(name);
    notify(REMOVE_PANEL, eSavePanel);
}

void EditorSystem::createSaveInputBox(){

    eSavePanel = eManager->createEntity();
    eSavePanel->add(new CPosition(xc, yc));
    eSavePanel->add(new CTexture("frame-600-200.png"));
    eSavePanel->add(new CDimensions(0.6*width, 0.08*height));
    eSavePanel->add(new CDraw(CDraw::GUI1));
    eSavePanel->add(new CPanel());

    double wPanel = eSavePanel->get<CDimensions>()->width;
    double hPanel = eSavePanel->get<CDimensions>()->height;
    double xOri = xc - wPanel/2;
    double yOri = yc - hPanel/2;

    sf::Text label;
    sf::Text txt;

    int cSize = 18;
    label.setFont(Assets::getFont(Assets::getPrimaryFont()));
    label.setCharacterSize(cSize);
    label.setFillColor(sf::Color::White);

    ///LABEL
    Entity* eLabel1 = eManager->createEntity();
    label.setString("File name:");
    eLabel1->add(new CTextbox2(label, 0, 0));
    eLabel1->add(new CPosition(xOri + 0.045*wPanel, yOri + hPanel/2 - cSize/2));
    eLabel1->add(new CDraw(CDraw::GUI1));

    ///INPUT TEXT BOX
    eSaveInput = eManager->createEntity();
    eSaveInput->add(new CTexture("tooltip-01.png"));
    eSaveInput->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel/2, yOri + hPanel/2));
    eSaveInput->add(new CDimensions(0.6*wPanel, 0.7*hPanel));
    eSaveInput->add(new CDraw(CDraw::GUI1));
    eSaveInput->add(new CButtonHitbox(0.6*wPanel, 0.7*hPanel));
    eSaveInput->add(new CButtonState());
    eSaveInput->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    eSaveInput->add(new CDefaultTexture("tooltip-01.png"));
    eSaveInput->add(new CDisplayer(CDisplayer::INPUT_TEXT, eSaveInput));
    //eSaveInput->add(new CInputTextBox(true, CInputTextBox::ACTIVE, 25));

    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(18);
    txt.setFillColor(sf::Color::Black);

    eSaveInput->add(new CTextbox2(txt, -0.55*wPanel/2, -cSize/2));

    ///CONFIRM BUTTON
    Entity* eOk = eManager->createEntity();
    eOk->add(new CTexture());
    eOk->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel + 0.06*wPanel/2, yOri + hPanel/2));
    eOk->add(new CDimensions(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonHitbox(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonState());
    eOk->add(new CButtonTrigger(SAVE_SCENARIO, CButtonTrigger::ON_RELEASE, sf::Keyboard::Return));
    eOk->add(new CDefaultTexture("ok-button.png"));
    eOk->add(new CHoverTexture("ok-button.png"));
    eOk->add(new CActiveTexture("ok-button.png"));
    eOk->add(new CDraw(CDraw::GUI1));
    eOk->add(new CButtonSounds("click2.wav", "rollover2.wav"));


    ///CANCEL BUTTON
    Entity* eCancel = eManager->createEntity();
    eCancel->add(new CTexture());
    eCancel->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel + 0.06*wPanel/2 + 0.06*wPanel, yOri + hPanel/2));
    eCancel->add(new CDimensions(0.7*hPanel, 0.7*hPanel));
    eCancel->add(new CButtonHitbox(0.7*hPanel, 0.7*hPanel));
    eCancel->add(new CButtonState());
    eCancel->add(new CButtonTrigger(CLOSE_PANEL, CButtonTrigger::ON_RELEASE, sf::Keyboard::Escape));
    eCancel->add(new CDefaultTexture("cancel-button.png"));
    eCancel->add(new CHoverTexture("cancel-button.png"));
    eCancel->add(new CActiveTexture("cancel-button.png"));
    eCancel->add(new CDraw(CDraw::GUI1));
    eCancel->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    eCancel->add(new CParentPanel(eSavePanel));

    notify(ACTIVATE_INPUT_TEXT_BOX, eSaveInput);

    notify(BEGIN_PANEL, eSavePanel);
    notify(ATTACH_ENTITY_TO_PANEL, eLabel1);
    notify(ATTACH_ENTITY_TO_PANEL, eSaveInput);
    notify(ATTACH_ENTITY_TO_PANEL, eOk);
    notify(ATTACH_ENTITY_TO_PANEL, eCancel);
    notify(END_PANEL);
}

void EditorSystem::onEndScenarioTest(Entity* e){
    Entity* eFade = eManager->createEntity();
    eFade->add(new CFadeTransition());
    eFade->get<CFadeTransition>()->color = sf::Color::Black;
    eFade->get<CFadeTransition>()->endMessage = INITIALIZE_EDITOR;
    notify(START_FADE_OUT_TRANSITION, eFade);
}

