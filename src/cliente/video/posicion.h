#ifndef _POSICION_H_
#define _POSICION_H_

namespace cliente {

class Posicion {
public:
    Posicion();
    
    Posicion(int x_, int y_);

    bool operator==(const Posicion& otro) const;

    int x;
    int y;
};

} // namespace cliente

#endif // _POSICION_H_
