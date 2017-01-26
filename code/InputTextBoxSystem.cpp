#include "InputTextBoxSystem.h"

InputTextBoxSystem::InputTextBoxSystem() {
    addRequirement(Component::INPUT_TEXT_BOX);
    addSubscription(TEXT_ENTERED);
    addSubscription(ACTIVATE_INPUT_TEXT_BOX);
    addSubscription(DEACTIVATE_INPUT_TEXT_BOX);
    addSubscription(MOUSE_BUTTON_PRESSED);
    eActive = nullptr;
}

InputTextBoxSystem::~InputTextBoxSystem() {
    //dtor
}

void InputTextBoxSystem::update() {
    if (eManager->isDead(eActive)) {
        eActive = nullptr;
        return;
    }

    if (eActive->get<CInputTextBox>()->blinkTimer.getElapsedTime().asSeconds() >= eActive->get<CInputTextBox>()->blinkRate) {
        eActive->get<CInputTextBox>()->blinkOn = opposite(eActive->get<CInputTextBox>()->blinkOn);
        eActive->get<CInputTextBox>()->blinkTimer.restart();
    }

}

void InputTextBoxSystem::onTextEntered(Entity* e) {
    if(eActive != nullptr) {
        string prvContent = eActive->get<CInputTextBox>()->content;
        char c = e->get<CTextInput>()->value;
        if (eActive->get<CInputTextBox>()->numbersOnly) {
            if ((c >= 48 && c <= 57) || c == 8) {
                if (c == 8) {
                    if (!eActive->get<CInputTextBox>()->content.empty()) {
                        int newSize = eActive->get<CInputTextBox>()->content.size()-1;
                        eActive->get<CInputTextBox>()->content.resize(newSize);
                        eActive->get<CInputTextBox>()->charCount--;
                    }
                } else if (c == 32) {
                    c = 255;
                    if (eActive->get<CInputTextBox>()->charCount < eActive->get<CInputTextBox>()->charMax) {
                        eActive->get<CInputTextBox>()->content.push_back(c);
                        eActive->get<CInputTextBox>()->charCount++;
                    }
                } else {
                    if (eActive->get<CInputTextBox>()->charCount < eActive->get<CInputTextBox>()->charMax) {
                        eActive->get<CInputTextBox>()->content.push_back(c);
                        eActive->get<CInputTextBox>()->charCount++;
                    }
                }
            }
        } else {
            if (c == 8) {
                if (!eActive->get<CInputTextBox>()->content.empty()) {
                    int newSize = eActive->get<CInputTextBox>()->content.size()-1;
                    eActive->get<CInputTextBox>()->content.resize(newSize);
                    eActive->get<CInputTextBox>()->charCount--;
                }
            } else if(c == 13) {
                if (!eActive->get<CInputTextBox>()->ignoreEnter) {
                    eActive->get<CInputTextBox>()->content.push_back('\n');
                }
            } else if (c == 9) {
                if (!eActive->get<CInputTextBox>()->ignoreTab) {
                    eActive->get<CInputTextBox>()->content.push_back(c);
                }
            } else if (c == 32) {
                c = 255;
                if (eActive->get<CInputTextBox>()->charCount < eActive->get<CInputTextBox>()->charMax) {
                    eActive->get<CInputTextBox>()->content.push_back(c);
                    eActive->get<CInputTextBox>()->charCount++;
                }
            } else if (c != 27) {
                if (eActive->get<CInputTextBox>()->charCount < eActive->get<CInputTextBox>()->charMax) {
                    eActive->get<CInputTextBox>()->content.push_back(c);
                    eActive->get<CInputTextBox>()->charCount++;
                }
            }
        }
        eActive->get<CInputTextBox>()->blinkOn = true;
        if (eActive->get<CInputTextBox>()->content != prvContent) {
            notify(eActive->get<CInputTextBox>()->msgOnUpdate, eActive);
        }
    }
}

void InputTextBoxSystem::onActivateInputTextBox(Entity* e) {
    eActive = e;
    eActive->get<CInputTextBox>()->state = CInputTextBox::ACTIVE;
    eActive->get<CInputTextBox>()->blinkOn = true;
}

void InputTextBoxSystem::onDeactivateInputTextBox(Entity* e) {
    eActive->get<CInputTextBox>()->blinkOn = false;
    eActive->get<CInputTextBox>()->state = CInputTextBox::INACTIVE;
    eActive = nullptr;
}

void InputTextBoxSystem::onMouseButtonPressed(Entity* e) {
    if(eActive != nullptr) {
        double x = eActive->get<CPosition>()->x;
        double y = eActive->get<CPosition>()->y;
        double w = eActive->get<CButtonHitbox>()->width;
        double h = eActive->get<CButtonHitbox>()->height;

        sf::FloatRect hitbox(x - w/2, y - h/2, w, h);
        sf::Vector2i mousePosition(sf::Mouse::getPosition(*window).x, sf::Mouse::getPosition(*window).y);
        sf::Vector2f mp = window->mapPixelToCoords(mousePosition);

        if (!hitbox.contains(mp)) onDeactivateInputTextBox(eActive);
    }
}







