#include "RenderSystem.h"

RenderSystem::RenderSystem(){
    addRequirement(Component::DRAW);
    addRequirement(Component::POSITION);
    eLists.resize(CDraw::N_TAGS);
}

RenderSystem::~RenderSystem(){
    //dtor
}

void RenderSystem::start(EntitiesManager* eManager, sf::RenderWindow* window, double targetUPS){
    this->eManager = eManager;
    this->window = window;
    this->targetUPS = targetUPS;
/*
    wWindow = window->getView().getSize().x;
    hWindow = window->getView().getSize().y;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;
*/
    wWindow = 1280;
    hWindow = 720;
    cxWindow = wWindow/2;
    cyWindow = hWindow/2;

    eLists.resize(CDraw::N_TAGS);
}

void RenderSystem::updateEntities(){
    std::list<Entity*>::iterator it;
    for (it = eManager->modifiedEntities.begin(); it != eManager->modifiedEntities.end(); it++){
        entities.remove(*it);
        if (hasRequirements(*it)){
            entities.push_back(*it);
        }
    }
    for (it = eManager->addedEntities.begin(); it != eManager->addedEntities.end(); it++){
        if (hasRequirements(*it)){
            entities.push_back(*it);
        }
    }
    for (it = eManager->removedEntities.begin(); it != eManager->removedEntities.end(); it++){
        entities.remove(*it);
    }
    for (unsigned int i = 0; i < eLists.size(); i++){
        eLists[i].clear();
    }
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        CDraw* cDraw = (*i)->get<CDraw>();
        eLists[cDraw->tag].push_back(*i);
        if (cDraw->tag == CDraw::WORLD) eWorld.push_back(*i);
    }
}

bool orderUnits(Entity* a, Entity* b){
    /*
    if (a->has(Component::UNIT) && b->has(Component::UNIT)){
        if (a->get<CUnit>()->dead && !b->get<CUnit>()->dead){
            return true;
        }else if (b->get<CUnit>()->dead && !a->get<CUnit>()->dead){
            return false;
        }else{
            if ((a->get<CPosition>()->y) < (b->get<CPosition>()->y)){
                return true;
            }else if ((a->get<CPosition>()->y) > (b->get<CPosition>()->y)){
                return false;
            }else{
                if ((a->get<CPosition>()->x) < (b->get<CPosition>()->x)){
                    return true;
                }
            }
        }
    }else{

        if ((a->get<CPosition>()->y) < (b->get<CPosition>()->y)){
            return true;
        }else if ((a->get<CPosition>()->y) > (b->get<CPosition>()->y)){
            return false;
        }else{
            if ((a->get<CPosition>()->x) < (b->get<CPosition>()->x)){
                return true;
            }
        }
    }
    */
    double ha = 0;
    double hb = 0;
    if (a->has(Component::DIMENSIONS)){
        ha = a->get<CDimensions>()->height;
    }else if (a->has(Component::ANIMATION)){
        ha = a->get<CAnimation>()->sprite.getLocalBounds().height;
    }else if (a->has(Component::TEXTURE)){
        ha = Assets::getTexture(a->get<CTexture>()->file)->getSize().y;
    }

    if (b->has(Component::DIMENSIONS)){
        hb = b->get<CDimensions>()->height;
    }else if (b->has(Component::ANIMATION)){
        hb = b->get<CAnimation>()->sprite.getLocalBounds().height;
    }else if (b->has(Component::TEXTURE)){
        hb = Assets::getTexture(b->get<CTexture>()->file)->getSize().y;
    }

    double xa = a->get<CPosition>()->x;
    double xb = b->get<CPosition>()->x;
    double ya = a->get<CPosition>()->y + ha/2;
    double yb = b->get<CPosition>()->y + hb/2;

    if ((ya) < (yb)){
        return true;
    }else if ((ya) > (yb)){
        return false;
    }else{
        if ((a->get<CPosition>()->x) < (b->get<CPosition>()->x)){
            return true;
        }
    }
}

void RenderSystem::update(){
    std::list<Entity*>::iterator it;
    window->clear(sf::Color::Black);
    eLists[CDraw::WORLD].sort(orderUnits);
    eLists[CDraw::WORLD_0].sort(orderUnits);
    eLists[CDraw::WORLD_1].sort(orderUnits);
    eLists[CDraw::WORLD_2].sort(orderUnits);
    eLists[CDraw::WORLD_3].sort(orderUnits);
    for (unsigned int i = 0; i < eLists.size(); i++){
        for (it = eLists[i].begin(); it != eLists[i].end(); it++){
            Entity* e = *it;
            if (eManager->isDead(e)) continue;
            if (!e->get<CDraw>()->isHidden){
                draw(e);
            }
        }
    }
    //ImGui::Render();
    window->display();
}

CDraw::Tag RenderSystem::getTag(EntityList& eList){
    if (eList.empty()) return CDraw::NONE;
    return eList.front()->get<CDraw>()->tag;
}

void RenderSystem::draw(Entity* e){
    if (e->has(Component::NINE_PATCH)) drawNinePatch(e);

    //printf("Tag %d ", e->get<CDraw>()->tag);
    CPosition* cPosition = e->get<CPosition>();
    if (e->has(Component::BAR_HUD)){
        CBarHUD* cBar = e->get<CBarHUD>();
        sf::RectangleShape rect;
        rect.setSize(sf::Vector2f(cBar->width, cBar->height));
        rect.setFillColor(cBar->fillColor);
        rect.setOutlineColor(cBar->outlineColor);
        rect.setOutlineThickness(cBar->outlineThickness);
        if      (cBar->grow == CBarHUD::LEFT) rect.setOrigin(cBar->width, cBar->height/2);
        else if (cBar->grow == CBarHUD::RIGHT) rect.setOrigin(0, cBar->height/2);
        rect.setPosition(cPosition->x, cPosition->y);
        window->draw(rect);
    }

    if (e->has(Component::SIMPLE_BUTTON)){
        e->get<CSimpleButton>()->shape.setOrigin(e->get<CSimpleButton>()->shape.getSize().x/2, e->get<CSimpleButton>()->shape.getSize().y/2);
        e->get<CSimpleButton>()->shape.setPosition(cPosition->x, cPosition->y);
        window->draw(e->get<CSimpleButton>()->shape);
    }

    if (e->has(Component::UNIT_HIGHLIGHT)){
        CUnitHighlight::State state = e->get<CUnitHighlight>()->state;
        sf::Sprite sprite(*Assets::getTexture("unit-highlight.png"));
        sprite.setPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + e->get<CUnitHighlight>()->yOff);
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        sprite.setColor(e->get<CUnitHighlight>()->colors[state]);
        window->draw(sprite);
    }
    if (e->has(Component::UNIT_HIGHLIGHT_2) && e->get<CUnitHighlight2>()->state == CUnitHighlight2::ON){
        sf::Sprite sprite(*Assets::getTexture("unit-highlight.png"));
        sprite.setPosition(e->get<CPosition>()->x, e->get<CPosition>()->y + 16);
        sprite.setOrigin(sprite.getLocalBounds().width/2, sprite.getLocalBounds().height/2);
        sprite.setColor(e->get<CUnitHighlight2>()->color);
        window->draw(sprite);
    }

    if (e->has(Component::TEXTURE)){
        CTexture* cTexture = e->get<CTexture>();
        if (!cTexture->file.empty()){
            if (hasBegining(cTexture->file, "9p-")){
                drawNinePatch(e);
            }else{
                sf::Sprite sprite(*Assets::getTexture(cTexture->file));
                sf::FloatRect rect = sprite.getLocalBounds();
                sprite.setOrigin(rect.width/2, rect.height/2);
                if (e->has(Component::CURSOR)) sprite.setOrigin(0,0);
                sprite.setPosition(cPosition->x, cPosition->y);
                if (e->has(Component::DIMENSIONS)){
                    CDimensions* cDim = e->get<CDimensions>();
                    sf::FloatRect rect = sprite.getLocalBounds();
                    double wScale = cDim->width/rect.width;
                    double hScale = cDim->height/rect.height;
                    sprite.setScale(wScale, hScale);
                }
                if (e->has(Component::SPIN_BUTTON_VALUE) && e->has(Component::UNIT)){
                    //sprite.setColor(Assets::colors[e->get<CUnit>()->id]);
                }
                if (e->has(Component::DISPLAYER) && e->has(Component::UNIT)){
                    //sprite.setColor(Assets::colors[e->get<CUnit>()->id]);
                }
                if (e->has(Component::OPTION_CELL) && e->get<COptionCell>()->value <= 99){
                    sprite.setColor(CUnit::Map[e->get<COptionCell>()->value].color);
                }
                if (e->has(Component::ROTATION)){
                    sprite.setRotation(e->get<CRotation>()->angle);
                }
                sf::Color c = sprite.getColor();
                c.a = e->get<CDraw>()->alpha;
                sprite.setColor(c);
                sprite.scale((cTexture->hFlip ? -1:1), (cTexture->vFlip ? -1:1));
                window->draw(sprite);
            }
        }
    }else if (e->has(Component::ANIMATION)){
        sf::Sprite sprite = e->get<CAnimation>()->sprite;
        sf::FloatRect rect = sprite.getLocalBounds();
        sprite.setOrigin(rect.width/2, rect.height/2);
        sprite.setPosition(cPosition->x, cPosition->y);
        if (e->has(Component::DIMENSIONS)){
            CDimensions* cDim = e->get<CDimensions>();
            sf::FloatRect rect = sprite.getLocalBounds();
            double wScale = cDim->width/rect.width;
            double hScale = cDim->height/rect.height;
            sprite.setScale(wScale, hScale);
        }
        sprite.scale((e->get<CAnimation>()->hFlip ? -1:1), (e->get<CAnimation>()->vFlip ? -1:1));
        sprite.setColor(sf::Color(255,255,255,e->get<CDraw>()->alpha));
        if (e->has(Component::UNIT)){
            sf::Color c = e->get<CUnit>()->color;
            c.a = e->get<CDraw>()->alpha;
            //sprite.setColor(c);
        }
        if (e->has(Component::ROTATION)){
            sprite.setRotation(e->get<CRotation>()->angle);
        }
        window->draw(sprite);
    }

    if (e->has(Component::CIRCLE_SHAPE)){
        sf::Color color = e->get<CCircleShape>()->shape.getFillColor();
        color.a = e->get<CDraw>()->alpha;
        e->get<CCircleShape>()->shape.setFillColor(color);
        e->get<CCircleShape>()->shape.setOrigin(e->get<CCircleShape>()->shape.getRadius(), e->get<CCircleShape>()->shape.getRadius());
        e->get<CCircleShape>()->shape.setPosition(e->get<CPosition>()->x, e->get<CPosition>()->y);
        window->draw(e->get<CCircleShape>()->shape);
    }
    if (e->has(Component::RECT_SHAPE)){
        sf::Color color = e->get<CRectShape>()->shape.getFillColor();
        color.a = e->get<CDraw>()->alpha;
        sf::FloatRect rect = e->get<CRectShape>()->shape.getLocalBounds();
        sf::Vector2f size = sf::Vector2f(e->get<CRectShape>()->shape.getSize().x, e->get<CRectShape>()->shape.getSize().y);
        e->get<CRectShape>()->shape.setFillColor(color);
        //e->get<CRectShape>()->shape.setOrigin(rect.width/2, rect.height/2);
        e->get<CRectShape>()->shape.setOrigin(size.x/2, size.y/2);


        sf::Vector2f position(e->get<CPosition>()->x, e->get<CPosition>()->y);
        if (e->has(Component::BAR_DISPLAYER)){
            switch(e->get<CBarDisplayer>()->growDirection){
                case CBarDisplayer::LEFT: position.x -= size.x/2.f; break;
                case CBarDisplayer::RIGHT: position.x += size.x/2.f; break;
                case CBarDisplayer::TOP: position.y -= size.y/2.f; break;
                case CBarDisplayer::DOWN: position.y += size.y/2.f; break;
            }
        }

        if (e->has(Component::ROTATION)){
            e->get<CRectShape>()->shape.setRotation(e->get<CRotation>()->angle);
        }
        sf::Color c = e->get<CRectShape>()->shape.getFillColor();
        c.a = e->get<CDraw>()->alpha;
        e->get<CRectShape>()->shape.setFillColor(c);

        e->get<CRectShape>()->shape.setPosition(position);
        window->draw(e->get<CRectShape>()->shape);
    }
    if (e->has(Component::ARMY_POLYGON)){
        window->draw(e->get<CArmyPolygon>()->shape);
    }

    if (e->has(Component::TEXT_BOX)){
        CTextBox* cText = e->get<CTextBox>();
        for(unsigned int i = 0; i < cText->lines.size(); i++){
            sf::FloatRect dim = cText->lines[i].getLocalBounds();
            cText->lines[i].setOrigin(dim.width/2, dim.height/2);
            cText->lines[i].setPosition(cPosition->x, cPosition->y - cText->height/2 + i*cText->size);
            window->draw(cText->lines[i]);
        }
    }
    if (e->has(Component::TEXT_BOX_2)){
        double x = cPosition->x + e->get<CTextbox2>()->xOff;
        double y = cPosition->y + e->get<CTextbox2>()->yOff;
        double w = e->get<CTextbox2>()->content.getLocalBounds().width;
        double h = e->get<CTextbox2>()->content.getLocalBounds().height;
        h += e->get<CTextbox2>()->content.getFont()->getLineSpacing(e->get<CTextbox2>()->content.getCharacterSize())/4;
        if (e->get<CTextbox2>()->align == CTextbox2::CENTRALIZED){
            e->get<CTextbox2>()->content.setOrigin(w/2, h/2);
        }else{
            e->get<CTextbox2>()->content.setOrigin(0, 0);
        }

        sf::Color color = e->get<CTextbox2>()->content.getColor();
        color.a = e->get<CDraw>()->alpha;
        e->get<CTextbox2>()->content.setFillColor(color);

        e->get<CTextbox2>()->content.setPosition(x, y);
        window->draw(e->get<CTextbox2>()->content);
    }

    if (e->has(Component::INPUT_TEXT_BOX) && e->has(Component::TEXT_BOX_2)){
        if (e->get<CInputTextBox>()->blinkOn){
            sf::RectangleShape shape;
            string txt = e->get<CTextbox2>()->content.getString();
            sf::Text txtEnd = e->get<CTextbox2>()->content;
            int indEnd = max(0, (int)txt.size()-1);
            txtEnd.setString(txt[indEnd]);
            double h = e->get<CTextbox2>()->content.getCharacterSize();
            double x = e->get<CTextbox2>()->content.findCharacterPos(indEnd).x + txtEnd.getLocalBounds().width;
            double y = e->get<CTextbox2>()->content.getPosition().y - h/2;
            shape.setFillColor(sf::Color::Black);
            shape.setSize(sf::Vector2f(1, h));
            shape.setPosition(x, y);
            window->draw(shape);
        }
    }

}

void RenderSystem::drawNinePatch(Entity* e){
    if (!e->has(Component::TEXTURE) || !e->has(Component::DIMENSIONS)) return;
    const NinePatch& patch = Assets::getNinePatch(e->get<CTexture>()->file);
    double w = max(e->get<CDimensions>()->width, patch.wMin);
    double h = max(e->get<CDimensions>()->height, patch.hMin);
    double x = e->get<CPosition>()->x;
    double y = e->get<CPosition>()->y;
    vector< vector<sf::Sprite> > parts = patch.parts;

    double wTarget, hTarget;
    double wScale, hScale;
    double xTarget, yTarget;

    ///PART 0,0
    parts[0][0].setOrigin(parts[0][0].getTextureRect().width/2.f, parts[0][0].getTextureRect().height/2.f);
    parts[0][0].setPosition(x - w/2.f + parts[0][0].getTextureRect().width/2.f,
                            y - h/2.f + parts[0][0].getTextureRect().height/2.f);


    ///PART 0,1
    wTarget = w - parts[0][0].getTextureRect().width - parts[0][2].getTextureRect().width;
    hTarget = parts[0][1].getTextureRect().height;
    wScale = wTarget/parts[0][1].getTextureRect().width;
    hScale = 1.f;
    xTarget = x;
    yTarget = y - h/2.f + parts[0][1].getTextureRect().height/2.f;
    parts[0][1].setOrigin(parts[0][1].getTextureRect().width/2.f, parts[0][1].getTextureRect().height/2.f);
    parts[0][1].setPosition(xTarget, yTarget);
    parts[0][1].setScale(wScale, hScale);

    ///PART 0,2
    parts[0][2].setOrigin(parts[0][2].getTextureRect().width/2.f, parts[0][2].getTextureRect().height/2.f);
    parts[0][2].setPosition(x + w/2.f - parts[0][2].getTextureRect().width/2.f,
                            y - h/2.f + parts[0][2].getTextureRect().height/2.f);
    ///PART 1,0
    wTarget = parts[1][0].getLocalBounds().width;
    hTarget = h - parts[0][0].getLocalBounds().height - parts[2][0].getLocalBounds().height;
    wScale = 1.f;
    hScale = hTarget/parts[1][0].getLocalBounds().height;
    xTarget = x - w/2 + parts[1][0].getLocalBounds().width/2.f;
    yTarget = y - h/2 + parts[0][0].getLocalBounds().height + hTarget/2.f;
    parts[1][0].setOrigin(parts[1][0].getLocalBounds().width/2, parts[1][0].getLocalBounds().height/2);
    parts[1][0].setPosition(xTarget, yTarget);
    parts[1][0].setScale(wScale, hScale);

    ///PART 1,1
    wTarget = w - parts[1][0].getLocalBounds().width - parts[1][2].getLocalBounds().width;
    hTarget = h - parts[0][1].getLocalBounds().height - parts[2][1].getLocalBounds().height;
    wScale = wTarget/parts[1][1].getLocalBounds().width;
    hScale = hTarget/parts[1][1].getLocalBounds().height;
    xTarget = x;
    yTarget = y - h/2.f + parts[0][1].getLocalBounds().height + hTarget/2.f;
    parts[1][1].setOrigin(parts[1][1].getLocalBounds().width/2, parts[1][1].getLocalBounds().height/2);
    parts[1][1].setPosition(xTarget, yTarget);
    parts[1][1].setScale(wScale, hScale);


    ///PART 1,2
    wTarget = parts[1][2].getLocalBounds().width;
    hTarget = h - parts[0][2].getLocalBounds().height - parts[2][2].getLocalBounds().height;
    wScale = wTarget/parts[1][2].getLocalBounds().width;
    hScale = hTarget/parts[1][2].getLocalBounds().height;
    xTarget = x + w/2 - parts[1][2].getLocalBounds().width/2;
    yTarget = y - h/2.f + parts[0][2].getLocalBounds().height + hTarget/2.f;
    parts[1][2].setOrigin(parts[1][2].getLocalBounds().width/2, parts[1][2].getLocalBounds().height/2);
    parts[1][2].setPosition(xTarget, yTarget);
    parts[1][2].setScale(wScale, hScale);

    ///PART 2,0
    parts[2][0].setOrigin(parts[2][0].getLocalBounds().width/2, parts[2][0].getLocalBounds().height/2);
    parts[2][0].setPosition(x - w/2 + parts[2][0].getLocalBounds().width/2,
                            y + h/2 - parts[2][0].getLocalBounds().height/2);

    ///PART 2,1
    wTarget = w - parts[2][0].getLocalBounds().width - parts[2][2].getLocalBounds().width;
    hTarget = parts[2][1].getLocalBounds().height;
    wScale = wTarget/parts[2][1].getLocalBounds().width;
    hScale = 1.f;
    xTarget = x;
    yTarget = y + h/2 - parts[2][1].getLocalBounds().height/2;
    parts[2][1].setOrigin(parts[2][1].getLocalBounds().width/2, parts[2][1].getLocalBounds().height/2);
    parts[2][1].setPosition(xTarget, yTarget);
    parts[2][1].setScale(wScale, hScale);

    ///PART 2,2
    parts[2][2].setOrigin(parts[2][2].getLocalBounds().width/2, parts[2][2].getLocalBounds().height/2);
    parts[2][2].setPosition(x + w/2 - parts[2][2].getLocalBounds().width/2,
                            y + h/2 - parts[2][2].getLocalBounds().height/2);

    for (unsigned i = 0; i < 3; ++i){
        for (unsigned j = 0; j < 3; ++j){
            window->draw(parts[i][j]);
        }
    }


}











