#ifndef TEXTBOXSYSTEM_H
#define TEXTBOXSYSTEM_H

#include "System.h"
#include <sstream>

class TextboxSystem : public System{
    public:
        TextboxSystem();
        ~TextboxSystem();

        void update();
        void onNotify(Message m, Entity* e = nullptr);

    private:
        void onTextUpdated(Entity* e);

};

#endif // TEXTBOXSYSTEM_H
