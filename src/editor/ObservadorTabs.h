#ifndef OBSERVADOR_TABS_H
#define OBSERVADOR_TABS_H
#include "Sprite.h"

class ObservadorTabs {
    public:
        virtual ~ObservadorTabs() {}
        virtual void en_click_terreno_tab(Sprite sprite_clickeado) = 0;
};

#endif // OBSERVADOR_TABS_H