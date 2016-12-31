#include "FormationEditor.h"

FormationEditor::FormationEditor(){
    addSubscription(CREATE_SCREEN);
    addSubscription(LOAD_FORMATION);
    addSubscription(SAVE_FORMATION);
    addSubscription(BT_FORMATION_SAVE);
    addSubscription(DELETE_FORMATION);
    addSubscription(CLEAR_FORMATION_EDITOR);
    eLoad = nullptr;
    eSave = nullptr;
    active = false;
}

FormationEditor::~FormationEditor(){
    //dtor
}

void FormationEditor::update(){
    if (!active) return;
    const int col = 15;
    const int row = 23;
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if (positions[i][j]->get<CCheckBox>()->on){
                if (order[i][j]->get<CButtonState>()->state == CButtonState::LOCKED){
                    order[i][j]->get<CButtonState>()->state = CButtonState::NON_ACTIVE;
                }
                units[i][j]->get<CDraw>()->isHidden = false;
            }else{
                if (order[i][j]->get<CInputTextBox>()->state == CInputTextBox::ACTIVE){
                    notify(DEACTIVATE_INPUT_TEXT_BOX);
                }
                order[i][j]->get<CInputTextBox>()->content = "";
                order[i][j]->get<CInputTextBox>()->charCount = 0;
                order[i][j]->get<CButtonState>()->state = CButtonState::LOCKED;
                units[i][j]->get<CDraw>()->isHidden = true;
            }
        }
    }
}

void FormationEditor::onLoadFormation(Entity* e){
    string nameFormation = eLoad->get<CDropList>()->value;
    if (nameFormation.empty()) return;
    CFormation F;
    for(list<CFormation>::iterator i = formations.begin(); i != formations.end(); i++){
        if (i->name == nameFormation){
            F = *i;
            break;
        }
    }

    clearPositions();

    const int col = 15;
    const int row = 23;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            if (F.positions[i][col-1-j] > 0){
                notify(TOGGLE_CHECK_BOX, positions[i][j]);
                order[i][j]->get<CInputTextBox>()->content = int2str(F.positions[i][col-1-j]);
                order[i][j]->get<CInputTextBox>()->charCount = int2str(F.positions[i][col-1-j]).size();
            }
        }
    }
}

void FormationEditor::onDeleteFormation(Entity* e){
    string nameFormation = eLoad->get<CDropList>()->value;

    tinyxml2::XMLDocument doc;
    string path = "../rsc-0.1/formations.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Error!\n");
    }
    tinyxml2::XMLNode* node = doc.FirstChildElement(nameFormation.c_str());
    tinyxml2::XMLElement* element;
    tinyxml2::XMLElement* elPositions;
    if (node == nullptr){
        return;
    }else{
        doc.DeleteNode(node);
    }

    doc.SaveFile(path.c_str());

    updateLoadOptions();
    eLoad->get<CDropList>()->value = "";
}

void FormationEditor::onSaveFormation(Entity* e){
    string name = eSave->get<CInputTextBox>()->content;

    tinyxml2::XMLDocument doc;
    string path = "../rsc-0.1/formations.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Error!\n");
    }
    tinyxml2::XMLNode* node = doc.FirstChildElement(name.c_str());
    tinyxml2::XMLElement* element;
    tinyxml2::XMLElement* elPositions;
    if (node == nullptr){
        element = doc.NewElement(name.c_str());
        elPositions = doc.NewElement("Positions");
        doc.InsertEndChild(element);
        element->InsertEndChild(elPositions);
    }else{
        element = node->ToElement();
        elPositions = element->FirstChildElement("Positions");
    }

    const int col = 15;
    const int row = 23;

    std::stringstream ss;
    ss << '\n';
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            int value = 0;
            if (positions[i][col-1-j]->get<CCheckBox>()->on){
                value = 1;
                if (str2int(order[i][col-1-j]->get<CInputTextBox>()->content) > 0){
                    value = str2int(order[i][col-1-j]->get<CInputTextBox>()->content);
                }
            }
            ss << value << " ";
        }
        ss << '\n';
    }

    elPositions->SetText(ss.str().c_str());

    doc.SaveFile(path.c_str());
    notify(CLOSE_PANEL, eSave);

    updateLoadOptions();
}
void FormationEditor::onBtFormationSave(Entity* e){
    notify(LOCK_ALL_BUTTONS);
    createSaveInputTextBox();
}

void FormationEditor::createSaveInputTextBox(){
    double width = wWindow;
    double height = hWindow;
    double xc = cxWindow;
    double yc = cyWindow;

    Entity* eSavePanel = eManager->createEntity();
    eSavePanel->add(new CPosition(cxWindow, cyWindow));
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
    label.setString("Name:");
    eLabel1->add(new CTextbox2(label, 0, 0));
    eLabel1->add(new CPosition(xOri + 0.045*wPanel, yOri + hPanel/2 - cSize/2));
    eLabel1->add(new CDraw(CDraw::GUI1));

    ///INPUT TEXT BOX
    eSave = eManager->createEntity();
    eSave->add(new CTexture("tooltip-01.png"));
    eSave->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel/2, yOri + hPanel/2));
    eSave->add(new CDimensions(0.6*wPanel, 0.7*hPanel));
    eSave->add(new CDraw(CDraw::GUI1));
    eSave->add(new CButtonHitbox(0.6*wPanel, 0.7*hPanel));
    eSave->add(new CButtonState());
    eSave->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
    eSave->add(new CDefaultTexture("tooltip-01.png"));
    eSave->add(new CDisplayer(CDisplayer::INPUT_TEXT, eSave));
    //eSave->add(new CInputTextBox(true, CInputTextBox::ACTIVE, 10));
    eSave->add(new CParentPanel(eSavePanel));

    txt.setFont(Assets::getFont(Assets::getPrimaryFont()));
    txt.setCharacterSize(18);
    txt.setFillColor(sf::Color::Black);

    eSave->add(new CTextbox2(txt, -0.55*wPanel/2, -cSize/2));

    ///CONFIRM BUTTON
    Entity* eOk = eManager->createEntity();
    eOk->add(new CTexture());
    eOk->add(new CPosition(xOri + 0.05*wPanel + label.getLocalBounds().width + 0.6*wPanel + 0.06*wPanel/2, yOri + hPanel/2));
    eOk->add(new CDimensions(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonHitbox(0.7*hPanel, 0.7*hPanel));
    eOk->add(new CButtonState());
    eOk->add(new CButtonTrigger(SAVE_FORMATION, CButtonTrigger::ON_RELEASE, sf::Keyboard::Return));
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

    notify(ACTIVATE_INPUT_TEXT_BOX, eSave);

    notify(BEGIN_PANEL, eSavePanel);
    notify(ATTACH_ENTITY_TO_PANEL, eLabel1);
    notify(ATTACH_ENTITY_TO_PANEL, eSave);
    notify(ATTACH_ENTITY_TO_PANEL, eOk);
    notify(ATTACH_ENTITY_TO_PANEL, eCancel);
    notify(END_PANEL);
}

void FormationEditor::onCreateScreen(Entity* e){
    switch(e->get<CScreen>()->id){
        case CScreen::FORMATION_EDITOR: eManager->clearSystem(); create(); notify(NEW_SCREEN); break;
        default: break;
    }
}

void FormationEditor::clearPositions(){
    const int col = 15;
    const int row = 23;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            positions[i][j]->get<CCheckBox>()->on = false;
            positions[i][j]->get<CTexture>()->file = positions[i][j]->get<CCheckBox>()->offTexture;
        }
    }
}

void FormationEditor::onClearFormationEditor(Entity* e){
    clearPositions();
}

void FormationEditor::create(){
    Entity* eBack = eManager->createEntity();
    eBack->add(new CPosition(cxWindow, cyWindow));
    eBack->add(new CTexture("background-woods-01.png"));
    eBack->add(new CDraw(CDraw::BACKGROUND));

    Entity* eObj;

    createGrid();
    createUnits();
    createMenu();
    updateLoadOptions();

    active = true;
}

void FormationEditor::createMenu(){
    const int col = 15;
    const int row = 23;

    double ux = 26;
    double uy = 26;
    double w = (col-1)*ux;
    double h = (row-1)*uy;
    double wButton = 90;
    double hButton = 30;
    double x0 = cxWindow + w + wButton/2;
    double y0 = cyWindow - h/2 + hButton/2;
    double x = cxWindow + w/2 - 1*wButton;
    double y = cyWindow - h/2 - hButton/2 - uy/2;

    eLoad = eManager->createEntity();
    eLoad->add(new CPosition(x, y));
    eLoad->add(new CDraw(CDraw::GUI1));
    eLoad->add(new CDimensions(wButton, hButton));
    eLoad->add(new CButtonHitbox(wButton, hButton));
    eLoad->add(new CButtonState());
    eLoad->add(new CButtonTrigger(EXPAND_DROP_LIST));
    eLoad->add(new CDropList(list<string>()));
    eLoad->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 12, sf::Color::Black));
    eLoad->add(new CRectShape(wButton, hButton, sf::Color::White, 1, sf::Color::Black));
    eLoad->add(new CDisplayer(CDisplayer::DROP_LIST_VALUE, eLoad));

    x = cxWindow + w/2;
    Entity* eObj;
    eObj = createButton(Assets::getString("LABEL-LOAD"), wButton, hButton, x, y, LOAD_FORMATION);

    x = cxWindow + w/2 + 1*wButton;
    eObj = createButton(Assets::getString("LABEL-DELETE"), wButton, hButton, x, y, DELETE_FORMATION);

    x = cxWindow + w/2 - 0.5*wButton;
    y = cyWindow + h/2 + hButton/2 + uy/2;
    eObj = createButton(Assets::getString("LABEL-CLEAR"), wButton, hButton, x, y, CLEAR_FORMATION_EDITOR);

    x = cxWindow + w/2 + 0.5*wButton;
    eObj = createButton(Assets::getString("LABEL-SAVE"), wButton, hButton, x, y, BT_FORMATION_SAVE);
}

void FormationEditor::clearGrid(){
    positions.clear();
    order.clear();

    const int col = 15;
    const int row = 23;
    positions = vector< vector<Entity*> >(row, vector<Entity*>(col));
    order = vector< vector<Entity*> >(row, vector<Entity*>(col));
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            positions[i][j] = nullptr;
            order[i][j] = nullptr;
        }
    }
}

void FormationEditor::createUnits(){
    double uxFormation = 16;
    double uyFormation = 13;
    double hFormation = 23*uyFormation;
    double wFormation = 15*uyFormation;
    const int col = 15;
    const int row = 23;

    double x0 = cxWindow/2 + wFormation;
    double y0 = cyWindow - hFormation/2;

    double x = x0;
    double y = y0;

    CUnit::ID id = 1;

    units.clear();
    units = vector< vector<Entity*> >(row, vector<Entity*>(col));
    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            Entity* eUnit = eManager->createEntity();
            eUnit->add(new CUnit(CUnit::Map[id]));
            eUnit->add(new CAnimation(false, CUnit::Map[id].aIdle));
            eUnit->add(new CDraw(CDraw::WORLD));
            eUnit->add(new CPosition(x, y));
            eUnit->get<CDraw>()->isHidden = true;
            units[i][col-1-j] = eUnit;
            x -= uxFormation;
        }
        y += uyFormation;
        x = x0;
    }
}

void FormationEditor::createGrid(){
    clearGrid();

    const int col = 15;
    const int row = 23;

    double x0 = cxWindow;
    double y0 = cyWindow;
    double ux = 26;
    double uy = 26;
    double w = (col-1)*ux;
    double h = (row-1)*uy;
    double x = x0;
    double y = y0;

    Entity* eObj;

    ///PANEL
    eObj = eManager->createEntity();
    eObj->add(new CPosition(cxWindow + w/2, cyWindow));
    eObj->add(new CTexture("back-formation-grid.png"));
    eObj->add(new CDraw(CDraw::GUI));

    for(int i = 0; i < col; i++){
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI1));
        eObj->add(new CRectShape(1, h, sf::Color::White));
        x += ux;
    }

    x0 = cxWindow + w/2;
    y0 = cyWindow - h/2;
    x = x0;
    y = y0;

    for(int i = 0; i < row; i++){
        eObj = eManager->createEntity();
        eObj->add(new CPosition(x, y));
        eObj->add(new CDraw(CDraw::GUI1));
        eObj->add(new CRectShape(w, 1, sf::Color::White));
        y += uy;
    }

    x0 = cxWindow;
    y0 = cyWindow - h/2;
    x = x0;
    y = y0;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            eObj = eManager->createEntity();
            eObj->add(new CPosition(x, y));
            eObj->add(new CDraw(CDraw::GUI2));
            eObj->add(new CTexture("alpha.png"));
            eObj->add(new CCheckBox("blank-circle.png", "alpha.png", false));
            eObj->add(new CButtonHitbox(ux, uy));
            eObj->add(new CDimensions(ux, uy));
            eObj->add(new CButtonState());
            eObj->add(new CButtonTrigger(TOGGLE_CHECK_BOX));
            x += ux;
            positions[i][j] = eObj;
        }
        y += uy;
        x = x0;
    }

    x0 = cxWindow;
    y0 = cyWindow - h/2;
    x = x0;
    y = y0;

    for(int i = 0; i < row; i++){
        for(int j = 0; j < col; j++){
            eObj = eManager->createEntity();
            eObj->add(new CPosition(x, y));
            eObj->add(new CDraw(CDraw::GUI3));
            eObj->add(new CTexture("alpha.png"));
            eObj->add(new CButtonHitbox(ux, uy));
            eObj->add(new CDimensions(ux, uy));
            eObj->add(new CButtonState(CButtonState::LOCKED));
            eObj->add(new CButtonTrigger(ACTIVATE_INPUT_TEXT_BOX));
            //eObj->add(new CInputTextBox(true, CInputTextBox::INACTIVE, 2, true));
            eObj->add(new CTextbox2("", Assets::getFont(Assets::getPrimaryFont()), 9, sf::Color::Black));
            eObj->add(new CDisplayer(CDisplayer::INPUT_TEXT, eObj));
            eObj->get<CButtonTrigger>()->mouseButton = sf::Mouse::Right;
            x += ux;
            order[i][j] = eObj;
        }
        y += uy;
        x = x0;
    }
}

void FormationEditor::updateLoadOptions(){
    formations.clear();
    const int col = 15;
    const int row = 23;

    tinyxml2::XMLDocument doc;
    string path = "../rsc-0.1/formations.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Could not open formations.xml!\n");
    }
    tinyxml2::XMLElement* element = doc.FirstChildElement();

    for (element; element != nullptr; element = element->NextSiblingElement()){
        CFormation formation;
        formation.name = element->Value();
        string strPos = element->FirstChildElement("Positions")->GetText();
        std::stringstream ss(strPos);
        for(int i = 0; i < row; i++){
            for(int j = 0; j < col; j++){
                ss >> formation.positions[i][j];
            }
        }
        formations.push_back(formation);
    }

    eLoad->get<CDropList>()->values.clear();
    for(list<CFormation>::iterator i = formations.begin(); i != formations.end(); i++){
        eLoad->get<CDropList>()->values.push_back(i->name);
    }
}

Entity* FormationEditor::createButton(string label, double w, double h, double x, double y, Message m){
    Entity* e = eManager->createEntity();
    e->add(new CTexture());
    e->add(new CPosition(x, y));
    e->add(new CDimensions(w, h));
    e->add(new CButtonHitbox(w, h));
    e->add(new CButtonState());
    e->add(new CButtonTrigger(m));
    e->add(new CDefaultTexture("tooltip-02.png"));
    e->add(new CHoverTexture("tooltip-02.png"));
    e->add(new CActiveTexture("tooltip-02.png"));
    e->add(new CDraw(CDraw::GUI1));
    e->add(new CTextbox2(label, Assets::getFont(Assets::getPrimaryFont()), 18, sf::Color::White));
    e->add(new CButtonSounds("click2.wav", "rollover2.wav"));
    e->add(new CSound());
    return e;
}



