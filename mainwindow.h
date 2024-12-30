#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr); // Constructor de la ventana principal

private slots:
    // Métodos para cargar y guardar datos
    void cargar_datos_por_defecto(); // Cargar datos desde el archivo Data.txt
    void guardar_datos(); // Guardar datos en un archivo
    void cargar_datos(); // Cargar datos desde un archivo

    // Métodos para agregar, eliminar y editar elementos
    void agregar_pedido(); // Agregar un nuevo pedido
    void eliminar_pedido(); // Eliminar un pedido seleccionado
    void editar_pedido(); // Editar un pedido seleccionado
    void agregar_cliente(); // Agregar un nuevo cliente
    void eliminar_cliente(); // Eliminar un cliente seleccionado
    void editar_cliente(); // Editar un cliente seleccionado
    void agregar_producto(); // Agregar un nuevo producto
    void eliminar_producto(); // Eliminar un producto seleccionado
    void editar_producto(); // Editar un producto seleccionado

    // Método para filtrar elementos en las listas
    void filtrar_elementos(const QString &texto); // Filtrar elementos según el texto ingresado

private:
    // Tablas para mostrar pedidos, clientes y productos
    QTableWidget *tabla_pedidos; // Tabla de pedidos
    QTableWidget *tabla_clientes; // Tabla de clientes
    QTableWidget *tabla_productos; // Tabla de productos
};

#endif // MAINWINDOW_H
