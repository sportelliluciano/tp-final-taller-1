#ifndef _POSICION_H_
#define _POSICION_H_

namespace cliente {

class Posicion {
public:
    Posicion() : x(0), y(0) { }
    
    Posicion(int x_, int y_) : x(x_), y(y_) { }

    bool operator==(const Posicion& otro) const {
        return (x == otro.x) && (y == otro.y);
    }

    int x;
    int y;
};

} // namespace cliente

#endif // _POSICION_H_
