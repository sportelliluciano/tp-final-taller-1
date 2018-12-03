#ifndef _LANZADOR_JUGADOR_H_
#define _LANZADOR_JUGADOR_H_

#include <string>

#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QWidget>


class Jugador : public QFrame {
public:
    Jugador(QWidget* parent, const std::string& id_);

    void set_nombre(const std::string& nombre);
    void set_casa(const std::string& casa);
    void set_listo(bool listo);

private:
    QFrame frame_checkbox;
    QCheckBox checkbox;
    QWidget wx_vertical;
    QVBoxLayout layout_vertical;
    QLabel label_nombre;
    std::string id;
};

#endif // _LANZADOR_JUGADOR_H_