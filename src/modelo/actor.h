#ifndef _ACTOR_H_
#define _ACTOR_H_

namespace modelo {

/**
 * \brief Actor del juego. 
 * 
 * Representa un actor del juego.
 */
class Actor { 
public:
    Actor(int pos_x, int pos_y);

    int x() const;
    int y() const;

    virtual int get_id() = 0;

    virtual ~Actor();

protected:
    int posicion_x, posicion_y;
    //Textura textura;
};

} // namespace modelo

#endif // _ACTOR_H_