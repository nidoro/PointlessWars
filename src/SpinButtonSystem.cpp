#include "SpinButtonSystem.h"

SpinButtonSystem::SpinButtonSystem(){
    addRequirement(Component::SPIN_BUTTON);

    addSubscription(CREATE_SPIN_BUTTONS);
    //addSubscription(CHANGE_SPIN_VALUE);
    addSubscription(INCREASE_SPIN_BUTTON_VALUE);
    addSubscription(DECREASE_SPIN_BUTTON_VALUE);

    styles.resize(CSpinButton::N_STYLES);

    styles[CSpinButton::STYLE_1].align = SpinButtonStyle::VERTICAL;
    styles[CSpinButton::STYLE_1].distance = 106;
    styles[CSpinButton::STYLE_1].width = 32;
    styles[CSpinButton::STYLE_1].height = 32;

    styles[CSpinButton::STYLE_1].btIncreaseDef = "button-more-01-default.png";
    styles[CSpinButton::STYLE_1].btIncreaseHov = "button-more-01-hovered.png";
    styles[CSpinButton::STYLE_1].btIncreaseAct = "button-more-01-hovered.png";

    styles[CSpinButton::STYLE_1].btDecreaseDef = "button-less-01-default.png";
    styles[CSpinButton::STYLE_1].btDecreaseHov = "button-less-01-hovered.png";
    styles[CSpinButton::STYLE_1].btDecreaseAct = "button-less-01-hovered.png";

    styles[CSpinButton::STYLE_2].align = SpinButtonStyle::HORIZONTAL;
    styles[CSpinButton::STYLE_2].distance = 86;
    styles[CSpinButton::STYLE_2].width = 32;
    styles[CSpinButton::STYLE_2].height = 32;

    styles[CSpinButton::STYLE_2].btDecreaseDef = "button-left-less-default.png";
    styles[CSpinButton::STYLE_2].btDecreaseHov = "button-left-less-hovered.png";
    styles[CSpinButton::STYLE_2].btDecreaseAct = "button-left-less-hovered.png";

    styles[CSpinButton::STYLE_2].btIncreaseDef = "button-right-more-default.png";
    styles[CSpinButton::STYLE_2].btIncreaseHov = "button-right-more-hovered.png";
    styles[CSpinButton::STYLE_2].btIncreaseAct = "button-right-more-hovered.png";
}

SpinButtonSystem::~SpinButtonSystem(){

}

void SpinButtonSystem::update(){
    for(EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        /*
        if (eManager->isDead(e)) continue;
        if (eManager->isDead(e->get<CSpinButton>()->eValue)){
            e->get<CSpinButton>()->eValue = nullptr;
            eManager->removeEntity(e);
        }else if (e->get<CSpinButton>()->eValue->has(Component::SPIN_BUTTON_VALUE)){
            e->get<CSpinButton>()->max = e->get<CSpinButton>()->eValue->get<CSpinButtonValue>()->max;
            e->get<CSpinButton>()->min = e->get<CSpinButton>()->eValue->get<CSpinButtonValue>()->min;
        }
        */
    }
}

void SpinButtonSystem::onCreateSpinButtons(Entity* e){
    if (!e->has(Component::SPIN_BUTTON)){
        printf("CSpinButtonValue expected!\n");
        return;
    }

    double x = e->get<CPosition>()->x;
    double y = e->get<CPosition>()->y;

    SpinButtonStyle style = styles[e->get<CSpinButton>()->style];

    Entity* eIncrease = eManager->createEntity();
    Entity* eDecrease = eManager->createEntity();

    double dx = style.align == SpinButtonStyle::VERTICAL ? 0 : style.distance/2;
    double dy = style.align == SpinButtonStyle::HORIZONTAL ? 0 : style.distance/2;

    eIncrease->add(new CPosition(x + dx, y - dy));
    eIncrease->add(new CButtonState());
    eIncrease->add(new CButtonHitbox(style.width, style.height));
    eIncrease->add(new CButtonTrigger(INCREASE_SPIN_BUTTON_VALUE));
    eIncrease->add(new CDimensions(style.width, style.height));
    eIncrease->add(new CTexture(style.btIncreaseDef));
    eIncrease->add(new CDefaultTexture(style.btIncreaseDef));
    eIncrease->add(new CHoverTexture(style.btIncreaseHov));
    eIncrease->add(new CActiveTexture(style.btIncreaseAct));
    eIncrease->add(new CDraw(CDraw::GUI2));
    eIncrease->add(new CActor());
    eIncrease->add(new CVelocity());
    eIncrease->addObservedEntity("SpinButton", e);

    eDecrease->add(new CPosition(x - dx, y + dy));
    eDecrease->add(new CButtonState());
    eDecrease->add(new CButtonHitbox(style.width, style.height));
    eDecrease->add(new CButtonTrigger(DECREASE_SPIN_BUTTON_VALUE));
    eDecrease->add(new CDimensions(style.width, style.height));
    eDecrease->add(new CTexture(style.btDecreaseDef));
    eDecrease->add(new CDefaultTexture(style.btDecreaseDef));
    eDecrease->add(new CHoverTexture(style.btDecreaseHov));
    eDecrease->add(new CActiveTexture(style.btDecreaseAct));
    eDecrease->add(new CDraw(CDraw::GUI2));
    eDecrease->add(new CActor());
    eDecrease->add(new CVelocity());
    eDecrease->addObservedEntity("SpinButton", e);

    e->attachEmployee(eIncrease);
    e->attachEmployee(eDecrease);

    e->addObservedEntity("SpinButtonIncrease", eIncrease);
    e->addObservedEntity("SpinButtonDecrease", eDecrease);
}

void SpinButtonSystem::onChangeSpinValue(Entity* e){
    /*
    Entity* eValue = e->get<CSpinButton>()->eValue;
    int value = eValue->get<CSpinButtonValue>()->value;
    value += e->get<CSpinButton>()->step;
    value = min(value, eValue->get<CSpinButtonValue>()->max);
    value = max(value, eValue->get<CSpinButtonValue>()->min);
    eValue->get<CSpinButtonValue>()->value = value;
    //printf("Value = %d\n", value);
    */
}

void SpinButtonSystem::onIncreaseSpinButtonValue(Entity* e){
    Entity* eButton = e->getObservedEntity("SpinButton");
    int value = eButton->get<CSpinButton>()->value;
    value += eButton->get<CSpinButton>()->step;
    value = min(value, eButton->get<CSpinButton>()->max);
    value = max(value, eButton->get<CSpinButton>()->min);
    eButton->get<CSpinButton>()->value = value;
}
void SpinButtonSystem::onDecreaseSpinButtonValue(Entity* e){
    Entity* eButton = e->getObservedEntity("SpinButton");
    int value = eButton->get<CSpinButton>()->value;
    value -= eButton->get<CSpinButton>()->step;
    value = min(value, eButton->get<CSpinButton>()->max);
    value = max(value, eButton->get<CSpinButton>()->min);
    eButton->get<CSpinButton>()->value = value;
}









