#include "TextboxSystem.h"

TextboxSystem::TextboxSystem() {
    addRequirement(Component::POSITION);
    addRequirement(Component::TEXT_BOX_2);
    addSubscription(TEXT_UPDATED);
}

TextboxSystem::~TextboxSystem() {

}

void TextboxSystem::update() {
    for(EntityListIt i = entities.begin(); i != entities.end(); i++) {
        Entity* e = *i;
        if (eManager->isDead(e)) continue;
        onTextUpdated(*i);
        if (e->has(Component::TYPING_EFFECT)) {
            if (e->get<CTypingEffect>()->fullText.empty()) {
                e->remove(Component::TYPING_EFFECT);
                eManager->addModified(e);
            } else if (e->get<CTypingEffect>()->clock.getElapsedTime().asSeconds() >= 1/e->get<CTypingEffect>()->rate) {
                int iNext = e->get<CTypingEffect>()->iNextChar;
                e->get<CTypingEffect>()->text.push_back(e->get<CTypingEffect>()->fullText[iNext]);
                e->get<CTextbox2>()->content.setString(e->get<CTypingEffect>()->text);

                e->get<CTypingEffect>()->clock.restart();
                e->get<CTypingEffect>()->iNextChar++;
                if ((unsigned) e->get<CTypingEffect>()->iNextChar >= e->get<CTypingEffect>()->fullText.size()) {
                    e->remove(Component::TYPING_EFFECT);
                    eManager->addModified(e);
                }

                Entity* eSound = eManager->createEntity();
                eSound->add(new CSound("typing-sound.ogg", CSound::REMOVE_ENTITY));
                notify(PLAY_SOUND, eSound);
            }
        }
    }
}

void TextboxSystem::onNotify(Message m, Entity* e) {
    switch(m) {
    case TEXT_UPDATED:
        onTextUpdated(e);
        break;
    default:
        break;
    }
}

void TextboxSystem::onTextUpdated(Entity* e) {
    double wMax = e->get<CTextbox2>()->wMax;
    double w = e->get<CTextbox2>()->content.getLocalBounds().width;
    if (w > wMax) {
        string text = e->get<CTextbox2>()->content.getString();
        std::stringstream ss(text);
        string word;
        vector<sf::Text> lines(1, e->get<CTextbox2>()->content);
        lines[0].setString("");
        int l = 0;
        while (ss >> word) {
            sf::Text newText = lines[l];
            newText.setString(lines[l].getString() + word);
            w = newText.getLocalBounds().width;
            if (w <= wMax) {
                newText.setString(newText.getString() + " ");
                lines[l] = newText;
            } else {
                lines.push_back(newText);
                l++;
                lines[l].setString(word + " ");
            }
        }
        string newString;
        for (unsigned int j = 0; j < lines.size()-1; j++) {
            newString += lines[j].getString() + "\n";
        }
        newString += lines.back().getString();
        e->get<CTextbox2>()->content.setString(newString);
    }
}

