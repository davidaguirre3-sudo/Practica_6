#include "ventanaprincipal.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QMessageBox>

VentanaPrincipal::VentanaPrincipal(QWidget* padre) : QMainWindow(padre) {
    juego = new Juego(this);
    juego->iniciar();

    vista = new QGraphicsView(this);
    vista->setScene(juego->getEscena());

    QDoubleSpinBox* spinAngulo = new QDoubleSpinBox(this);
    spinAngulo->setRange(0.0, 180.0);
    spinAngulo->setValue(45.0);
    spinAngulo->setSuffix(QString(" grados"));

    QDoubleSpinBox* spinVelocidad = new QDoubleSpinBox(this);
    spinVelocidad->setRange(1.0, 100.0);
    spinVelocidad->setValue(20.0);

    QPushButton* botonDisparar = new QPushButton(QString("Disparar"), this);

    QHBoxLayout* controles = new QHBoxLayout();
    controles->addWidget(new QLabel(QString("Angulo:"), this));
    controles->addWidget(spinAngulo);
    controles->addWidget(new QLabel(QString("Velocidad:"), this));
    controles->addWidget(spinVelocidad);
    controles->addWidget(botonDisparar);

    QVBoxLayout* principal = new QVBoxLayout();
    principal->addWidget(vista);
    principal->addLayout(controles);

    QWidget* contenedor = new QWidget(this);
    contenedor->setLayout(principal);
    setCentralWidget(contenedor);
    setWindowTitle(QString("Practica 5 - Actividad 2"));

    QObject::connect(botonDisparar, &QPushButton::clicked, this, [this, spinAngulo, spinVelocidad]() {
        onDisparar(spinAngulo->value(), spinVelocidad->value());
    });

    QObject::connect(juego, &Juego::turnoCambiado, this, &VentanaPrincipal::onTurnoCambiado);
    QObject::connect(juego, &Juego::juegoTerminado, this, &VentanaPrincipal::onJuegoTerminado);
}

VentanaPrincipal::~VentanaPrincipal() {

}

void VentanaPrincipal::onDisparar(double angulo, double velocidad) {
    juego->disparar(angulo, velocidad);
}

void VentanaPrincipal::onTurnoCambiado(int indiceJugadorActivo) {
    setWindowTitle(QString(indiceJugadorActivo == 0
                                ? "Practica 5 - turno: Jugador 1"
                                : "Practica 5 - turno: Jugador 2"));
}

void VentanaPrincipal::onJuegoTerminado(const std::string& nombreGanador) {
    QMessageBox::information(this, QString("Fin del juego"),
                              QString((nombreGanador + " gano!").c_str()));
}
