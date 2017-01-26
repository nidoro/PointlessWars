#ifndef REMOTEPLAYER_H
#define REMOTEPLAYER_H

#include "System.h"

class RemotePlayer : public System {
    public:
        RemotePlayer();
        ~RemotePlayer();

        void update();

    private:

        void onReceivePacket();

};

#endif // REMOTEPLAYER_H
