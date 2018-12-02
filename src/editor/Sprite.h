#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include <QPixmap>

struct Sprite {
    std::string id;
    int tipo;
    QPixmap imagen;
};

#endif // SPRITE_H