#ifndef _ID_H_
#define _ID_H_

namespace modelo {
/**
 * \brief Generador de id´s para toda entidad dentro del juego.
 */
class Id{
    private:
    int id = 0;

    public:
    Id();
    ~Id();
    int nuevo_id();
};

} // namespace modelo

#endif // _ID_H_