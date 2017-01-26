#include "OptionBoxSystem.h"

OptionBoxSystem::OptionBoxSystem() {
    addRequirement(Component::OPTION_BOX);

    addSubscription(SELECT_OPTION);
    addSubscription(EXPAND_OPTION_BOX);
    addSubscription(RETRACT_OPTION_BOX);
}

OptionBoxSystem::~OptionBoxSystem() {
    //dtor
}

void OptionBoxSystem::update() {
    EntityListIt i, j;
    for (i = entities.begin(); i != entities.end(); i++) {
    }
}

void OptionBoxSystem::onExpandOptionBox(Entity* e) {
    COptionBox* box = e->get<COptionBox>();
    double x = e->get<CPosition>()->x + e->get<COptionBox>()->offset.x;
    double y = e->get<CPosition>()->y + e->get<COptionBox>()->offset.y;
    //CButtonHitbox::Shape shape = e->get<CButtonHitbox>()->shape;
    double w = e->get<CDimensions>()->width;
    double h = e->get<CDimensions>()->height;

    COptionBox::Data data = e->get<COptionBox>()->data;

    std::list<int>::iterator i;
    for (i = box->options.begin(); i != box->options.end(); i++) {
        Entity* c = eManager->createEntity();
        c->add(new CButtonState());
        c->add(new CPosition(x, y));
        c->add(new CButtonTrigger(SELECT_OPTION));
        c->add(new CTexture());
        c->add(new CDefaultTexture());
        c->add(new CHoverTexture());
        c->add(new CActiveTexture());
        c->add(new CDimensions(w, h));
        c->add(new CButtonHitbox(w, h));
        c->add(new CDraw(CDraw::GUI));
        c->add(new COptionCell(e, *i));
        if (data == COptionBox::ACTION) {
            c->add(new CTooltip(CTooltip::ACTION));
            setCellTooltip(c, data);
            if (*i <= 99) {
                c->add(new CHighlightTrigger(CUnitHighlight2::ATTACK, *i));
                c->get<CButtonState>()->gainedFocusMessage = HIGHLIGHT_UNITS;
                c->get<CButtonState>()->lostFocusMessage = HIGHLIGHT_UNITS_OFF;
            }
        }
        if (e->has(Component::OWNER)) {
            c->add(new COwner(e->get<COwner>()->e));
        }
        //c->add(new CTooltip("", "", 150, 0, 0, CTooltip::STYLE_2));
        setCellTexture(c, data);

        box->cells.push_back(c);
        if (box->direction == COptionBox::DOWN) y += box->distance;
        if (box->direction == COptionBox::UP) y -= box->distance;
    }
    box->state = COptionBox::CHOOSING;
    e->get<CButtonTrigger>()->setUniqueTrigger(RETRACT_OPTION_BOX);
}

void OptionBoxSystem::onRetractOptionBox(Entity* e) {
    COptionBox* box = e->get<COptionBox>();
    EntityListIt i;
    for(i = box->cells.begin(); i != box->cells.end(); i++) {
        if ((*i)->has(Component::TOOLTIP)) {
            notify(BUTTON_LOST_FOCUS, *i);
        }
        eManager->removeEntity(*i);
    }
    box->cells.clear();
    box->state = COptionBox::CHOSEN;
    e->get<CButtonTrigger>()->setUniqueTrigger(EXPAND_OPTION_BOX);
}

void OptionBoxSystem::onSelectOptionBox(Entity* e) {
    COptionCell* cell = e->get<COptionCell>();
    Entity* box = cell->optionBox;
    //EntityFactory::SelectOptionBoxValue(box, cell->value);

    box->get<COptionBox>()->selected = cell->value;

    onRetractOptionBox(box);
}

void OptionBoxSystem::setCellTexture(Entity* e, COptionBox::Data data) {
    if (data == COptionBox::ACTION) {
        CAction::ID id = e->get<COptionCell>()->value;
        e->get<CTexture>()->file = CAction::Map[id].btDefTexture;
        e->get<CDefaultTexture>()->file = CAction::Map[id].btDefTexture;
        e->get<CHoverTexture>()->file = CAction::Map[id].btHovTexture;
        e->get<CActiveTexture>()->file = CAction::Map[id].btActTexture;
    }
}

void OptionBoxSystem::setCellTooltip(Entity* e, COptionBox::Data data) {
    switch(data) {
    case COptionBox::ACTION:
        e->get<CTooltip>()->info = CTooltip::ACTION;
        break;
    default:
        break;
    }
}
