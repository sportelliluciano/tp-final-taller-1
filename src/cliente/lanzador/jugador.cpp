#include "cliente/lanzador/jugador.h"

Jugador::Jugador(QWidget* parent, const std::string& id_)
: QFrame(parent), frame_checkbox(this), checkbox(&frame_checkbox), 
  wx_vertical(this), layout_vertical(&wx_vertical), label_nombre(&wx_vertical),
  id(id_)
{ 
    // Frame base
    setObjectName(QString(id.c_str()));
    setMinimumSize(QSize(100, 100));
    setMaximumSize(QSize(100, 100));
    
    // Frame para el checkbox
    frame_checkbox.setObjectName(QString((id + "frm_checkbox").c_str()));
    frame_checkbox.setGeometry(QRect(0, 64, 100, 34));
    frame_checkbox.setMinimumSize(QSize(100, 34));
    frame_checkbox.setMaximumSize(QSize(100, 34));
    frame_checkbox.setFrameShape(QFrame::NoFrame);
    frame_checkbox.setFrameShadow(QFrame::Raised);
    
    // Checkbox
    checkbox.setObjectName(QString((id + "checkbox").c_str()));
    checkbox.setEnabled(false);
    checkbox.setGeometry(QRect(34, 0, 32, 32));
    
    // Widget vertical
    wx_vertical.setObjectName(QString((id + "wx_vertical").c_str()));
    wx_vertical.setGeometry(QRect(0, 0, 100, 64));
    
    // Layout vertical
    layout_vertical.setSpacing(0);
    layout_vertical.setObjectName(QString((id + "layout_vertical").c_str()));
    layout_vertical.setContentsMargins(0, 0, 0, 0);

    // Nombre del jugador
    label_nombre.setObjectName(QString((id + "label_nombre").c_str()));
    label_nombre.setAlignment(Qt::AlignHCenter|Qt::AlignTop);
    label_nombre.setWordWrap(true);

    layout_vertical.addWidget(&label_nombre, 0, Qt::AlignTop);
}

void Jugador::set_nombre(const std::string& nombre) {
    label_nombre.setText(nombre.c_str());
}

void Jugador::set_casa(const std::string& casa) {
    setStyleSheet(QString(("QFrame#" + id + " {\n"
        " background: url(:/imgs/nuevos/" + casa + ".png);\n"
        " background-position: center;\n"
        "}").c_str())
    );
}

void Jugador::set_listo(bool listo) {
    checkbox.setChecked(listo);
}