#ifndef OBSERVADOR_H
#define OBSERVADOR_H
#include <string>

class Observador {
    public:
        virtual ~Observador() {}
        virtual void en_notificacion(std::string& id_label) = 0;
};

#endif // OBSERVADOR_H