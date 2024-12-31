#include "mainwindow.h"
#include <QCoreApplication>
#include <QLabel>
#include <QVBoxLayout>
#include <QInputDialog>
#include <QPushButton>
#include <QTabWidget>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QTableWidget>
#include <QHeaderView>
#include <QDialog>
#include <QFormLayout>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QCompleter>
#include <QStringListModel>
#include <QToolBar>
#include <QIcon>
#include <QContextMenuEvent>

// Constructor de la clase MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Configuración de la ventana principal
    setWindowTitle("Stock Manager"); // Establecer el título de la ventana
    resize(600, 400); // Establecer el tamaño inicial de la ventana
    setWindowState(Qt::WindowMaximized); // Maximizar la ventana al iniciar

    // Crear el widget central y el layout principal
    QWidget *widget_central = new QWidget(this); // Crear un widget central
    setCentralWidget(widget_central); // Establecer el widget central
    QVBoxLayout *layout_vertical = new QVBoxLayout(widget_central); // Crear un layout vertical

    // Crear una barra de herramientas en la parte superior
    QToolBar *barra_herramientas = addToolBar("Main Toolbar"); // Crear una barra de herramientas
    barra_herramientas->setOrientation(Qt::Horizontal); // Hacer la barra horizontal
    barra_herramientas->setIconSize(QSize(16, 16)); // Reducir el tamaño de los íconos
    addToolBar(Qt::TopToolBarArea, barra_herramientas); // Mover la barra a la parte superior

    // Aplicar estilo para eliminar bordes y efecto 3D
    barra_herramientas->setStyleSheet(
        "QToolBar { border: none; background: transparent; }"
        "QToolButton { border: none; background: transparent; }"
    );

    // Agregar el ícono de guardado a la barra de herramientas
    QAction *accion_guardar = new QAction(QIcon("Save.png"), "Save", this); // Crear una acción para guardar
    barra_herramientas->addAction(accion_guardar); // Agregar la acción a la barra de herramientas
    connect(accion_guardar, &QAction::triggered, this, &MainWindow::guardar_datos); // Conectar la acción al método guardar_datos

    // Establecer el color de fondo del menú
    menuBar()->setStyleSheet("QMenuBar { background-color: #f0f0f0; }"); // Aplicar estilo al menú

    // Crear una barra de menú
    QMenuBar *barra_menu = this->menuBar(); // Obtener la barra de menú

    // Crear el menú "File"
    QMenu *menu_archivo = barra_menu->addMenu("File"); // Crear un menú "File"

    // Crear acciones para abrir y guardar
    QAction *accion_abrir = new QAction("Open File", this); // Crear una acción para abrir archivo
    QAction *accion_guardar_archivo = new QAction("Save File As", this); // Crear una acción para guardar archivo

    // Conectar las acciones a las funciones correspondientes
    connect(accion_abrir, &QAction::triggered, this, &MainWindow::cargar_datos); // Conectar la acción abrir al método cargar_datos
    connect(accion_guardar_archivo, &QAction::triggered, this, &MainWindow::guardar_datos_como); // Conectar la acción guardar al método guardar_datos_como

    // Agregar las acciones al menú "File"
    menu_archivo->addAction(accion_abrir); // Agregar la acción abrir al menú
    menu_archivo->addAction(accion_guardar_archivo); // Agregar la acción guardar al menú

    // Crear un layout horizontal para los botones y la barra de búsqueda
    QHBoxLayout *layout_horizontal = new QHBoxLayout(); // Crear un layout horizontal
    layout_vertical->addLayout(layout_horizontal); // Agregar el layout horizontal al layout vertical

    // Crear una barra de búsqueda
    QLineEdit *barra_busqueda = new QLineEdit(this); // Crear una barra de búsqueda
    barra_busqueda->setPlaceholderText("Search..."); // Establecer el texto de marcador de posición
    layout_horizontal->addWidget(barra_busqueda); // Agregar la barra de búsqueda al layout horizontal
    connect(barra_busqueda, &QLineEdit::textChanged, this, &MainWindow::filtrar_elementos); // Conectar el cambio de texto al método filtrar_elementos

    // Crear un QTabWidget para diferentes secciones (Orders, Products, Clients)
    QTabWidget *pestanas = new QTabWidget(this); // Crear un widget de pestañas
    layout_vertical->addWidget(pestanas); // Agregar el widget de pestañas al layout vertical

    // Sección de Pedidos
    QWidget *pestana_pedidos = new QWidget(this); // Crear una pestaña para pedidos
    QVBoxLayout *layout_pedidos = new QVBoxLayout(pestana_pedidos); // Crear un layout vertical para la pestaña de pedidos
    QLabel *etiqueta_pedidos = new QLabel("Current Orders", this); // Crear una etiqueta para la pestaña de pedidos
    layout_pedidos->addWidget(etiqueta_pedidos); // Agregar la etiqueta al layout de pedidos

    // Tabla de pedidos
    tabla_pedidos = new QTableWidget(this); // Crear una tabla para los pedidos
    tabla_pedidos->setColumnCount(3); // Establecer el número de columnas
    tabla_pedidos->setHorizontalHeaderLabels({"Customer", "Address", "Product"}); // Establecer las etiquetas de las columnas
    tabla_pedidos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_pedidos->setShowGrid(false); // Ocultar la cuadrícula
    tabla_pedidos->setContextMenuPolicy(Qt::CustomContextMenu); // Habilitar el menú contextual
    layout_pedidos->addWidget(tabla_pedidos); // Agregar la tabla al layout de pedidos

    // Conectar el evento de clic derecho al menú contextual
    connect(tabla_pedidos, &QTableWidget::customContextMenuRequested, this, &MainWindow::mostrar_menu_contextual); // Conectar el evento de menú contextual al método mostrar_menu_contextual

    // Crear un layout horizontal para los botones de pedidos
    QHBoxLayout *layout_botones_pedidos = new QHBoxLayout(); // Crear un layout horizontal para los botones
    QPushButton *boton_agregar_pedido = new QPushButton("Add Order", this); // Crear un botón para agregar pedidos
    QPushButton *boton_eliminar_pedido = new QPushButton("Delete Order", this); // Crear un botón para eliminar pedidos
    QPushButton *boton_editar_pedido = new QPushButton("Edit Order", this); // Crear un botón para editar pedidos
    QPushButton *boton_afectar = new QPushButton("Process", this); // Crear un botón para procesar pedidos
    layout_botones_pedidos->addWidget(boton_agregar_pedido); // Agregar el botón de agregar al layout
    layout_botones_pedidos->addWidget(boton_eliminar_pedido); // Agregar el botón de eliminar al layout
    layout_botones_pedidos->addWidget(boton_editar_pedido); // Agregar el botón de editar al layout
    layout_botones_pedidos->addWidget(boton_afectar); // Agregar el botón de procesar al layout
    layout_pedidos->addLayout(layout_botones_pedidos); // Agregar el layout de botones al layout de pedidos

    // Conectar los botones de pedidos a sus funciones
    connect(boton_agregar_pedido, &QPushButton::clicked, this, &MainWindow::agregar_pedido); // Conectar el botón de agregar al método agregar_pedido
    connect(boton_eliminar_pedido, &QPushButton::clicked, this, &MainWindow::eliminar_pedido); // Conectar el botón de eliminar al método eliminar_pedido
    connect(boton_editar_pedido, &QPushButton::clicked, this, &MainWindow::editar_pedido); // Conectar el botón de editar al método editar_pedido
    connect(boton_afectar, &QPushButton::clicked, this, &MainWindow::afectar_inventario); // Conectar el botón de procesar al método afectar_inventario

    // Sección de Productos
    QWidget *pestana_productos = new QWidget(this); // Crear una pestaña para productos
    QVBoxLayout *layout_productos = new QVBoxLayout(pestana_productos); // Crear un layout vertical para la pestaña de productos
    QLabel *etiqueta_productos = new QLabel("Available Products", this); // Crear una etiqueta para la pestaña de productos
    layout_productos->addWidget(etiqueta_productos); // Agregar la etiqueta al layout de productos

    // Tabla de productos
    tabla_productos = new QTableWidget(this); // Crear una tabla para los productos
    tabla_productos->setColumnCount(2); // Establecer el número de columnas
    tabla_productos->setHorizontalHeaderLabels({"Product", "Quantity"}); // Establecer las etiquetas de las columnas
    tabla_productos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_productos->setShowGrid(false); // Ocultar la cuadrícula
    layout_productos->addWidget(tabla_productos); // Agregar la tabla al layout de productos

    // Crear un layout horizontal para los botones de productos
    QHBoxLayout *layout_botones_productos = new QHBoxLayout(); // Crear un layout horizontal para los botones
    QPushButton *boton_agregar_producto = new QPushButton("Add Product", this); // Crear un botón para agregar productos
    QPushButton *boton_eliminar_producto = new QPushButton("Delete Product", this); // Crear un botón para eliminar productos
    QPushButton *boton_editar_producto = new QPushButton("Edit Product", this); // Crear un botón para editar productos
    layout_botones_productos->addWidget(boton_agregar_producto); // Agregar el botón de agregar al layout
    layout_botones_productos->addWidget(boton_eliminar_producto); // Agregar el botón de eliminar al layout
    layout_botones_productos->addWidget(boton_editar_producto); // Agregar el botón de editar al layout
    layout_productos->addLayout(layout_botones_productos); // Agregar el layout de botones al layout de productos

    // Conectar los botones de productos a sus funciones
    connect(boton_agregar_producto, &QPushButton::clicked, this, &MainWindow::agregar_producto); // Conectar el botón de agregar al método agregar_producto
    connect(boton_eliminar_producto, &QPushButton::clicked, this, &MainWindow::eliminar_producto); // Conectar el botón de eliminar al método eliminar_producto
    connect(boton_editar_producto, &QPushButton::clicked, this, &MainWindow::editar_producto); // Conectar el botón de editar al método editar_producto

    // Sección de Clientes
    QWidget *pestana_clientes = new QWidget(this); // Crear una pestaña para clientes
    QVBoxLayout *layout_clientes = new QVBoxLayout(pestana_clientes); // Crear un layout vertical para la pestaña de clientes
    QLabel *etiqueta_clientes = new QLabel("Registered Clients", this); // Crear una etiqueta para la pestaña de clientes
    layout_clientes->addWidget(etiqueta_clientes); // Agregar la etiqueta al layout de clientes

    // Tabla de clientes
    tabla_clientes = new QTableWidget(this); // Crear una tabla para los clientes
    tabla_clientes->setColumnCount(2); // Establecer el número de columnas
    tabla_clientes->setHorizontalHeaderLabels({"Name", "Address"}); // Establecer las etiquetas de las columnas
    tabla_clientes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_clientes->setShowGrid(false); // Ocultar la cuadrícula
    layout_clientes->addWidget(tabla_clientes); // Agregar la tabla al layout de clientes

    // Crear un layout horizontal para los botones de clientes
    QHBoxLayout *layout_botones_clientes = new QHBoxLayout(); // Crear un layout horizontal para los botones
    QPushButton *boton_agregar_cliente = new QPushButton("Add Client", this); // Crear un botón para agregar clientes
    QPushButton *boton_eliminar_cliente = new QPushButton("Delete Client", this); // Crear un botón para eliminar clientes
    QPushButton *boton_editar_cliente = new QPushButton("Edit Client", this); // Crear un botón para editar clientes
    layout_botones_clientes->addWidget(boton_agregar_cliente); // Agregar el botón de agregar al layout
    layout_botones_clientes->addWidget(boton_eliminar_cliente); // Agregar el botón de eliminar al layout
    layout_botones_clientes->addWidget(boton_editar_cliente); // Agregar el botón de editar al layout
    layout_clientes->addLayout(layout_botones_clientes); // Agregar el layout de botones al layout de clientes

    // Conectar los botones de clientes a sus funciones
    connect(boton_agregar_cliente, &QPushButton::clicked, this, &MainWindow::agregar_cliente); // Conectar el botón de agregar al método agregar_cliente
    connect(boton_eliminar_cliente, &QPushButton::clicked, this, &MainWindow::eliminar_cliente); // Conectar el botón de eliminar al método eliminar_cliente
    connect(boton_editar_cliente, &QPushButton::clicked, this, &MainWindow::editar_cliente); // Conectar el botón de editar al método editar_cliente

    // Agregar las pestañas en el orden correcto: Orders, Products, Clients
    pestanas->addTab(pestana_pedidos, "Orders"); // Agregar la pestaña de pedidos
    pestanas->addTab(pestana_productos, "Products"); // Agregar la pestaña de productos
    pestanas->addTab(pestana_clientes, "Customers"); // Agregar la pestaña de clientes

    // Cargar datos automáticamente al iniciar
    cargar_datos_por_defecto(); // Llamar al método para cargar datos por defecto
}

// Método para afectar el inventario
void MainWindow::afectar_inventario()
{
    for (int i = 0; i < tabla_pedidos->rowCount(); ++i) // Recorrer todas las filas de la tabla de pedidos
    {
        // Obtener el color de fondo de la primera celda de la fila
        QTableWidgetItem *item = tabla_pedidos->item(i, 0);

        if (item && item->background() == Qt::green) // Verificar si el fondo es verde
        {
            QString producto = tabla_pedidos->item(i, 2)->text(); // Obtener el nombre del producto
            for (int j = 0; j < tabla_productos->rowCount(); ++j) // Recorrer la tabla de productos
            {
                if (tabla_productos->item(j, 0)->text() == producto) // Buscar el producto en la tabla de productos
                {
                    int cantidad = tabla_productos->item(j, 1)->text().toInt();

                    if (cantidad > 0) // Verificar que la cantidad sea mayor que 0
                    {
                        cantidad -= 1; // Restar 1 a la cantidad
                        tabla_productos->setItem(j, 1, new QTableWidgetItem(QString::number(cantidad)));
                    }

                    break;
                }
            }
        }
    }

    tabla_pedidos->setRowCount(0); // Eliminar todas las órdenes, independientemente del color
}

// Método para mostrar el menú contextual en la tabla de pedidos
void MainWindow::mostrar_menu_contextual(const QPoint &pos)
{
    QTableWidgetItem *item = tabla_pedidos->itemAt(pos); // Obtener el ítem en la posición del clic

    if (item) // Verificar si hay un ítem válido
    {
        QMenu menu(this); // Crear un menú contextual
        QAction *accion_verde = menu.addAction("Green"); // Agregar opción "Green"
        QAction *accion_rojo = menu.addAction("Red"); // Agregar opción "Red"

        // Conectar las acciones a las funciones correspondientes
        connect(accion_verde, &QAction::triggered, this, [this, item]() { cambiar_color_fila(item->row(), Qt::green); });
        connect(accion_rojo, &QAction::triggered, this, [this, item]() { cambiar_color_fila(item->row(), Qt::red); });

        menu.exec(tabla_pedidos->viewport()->mapToGlobal(pos)); // Mostrar el menú en la posición del clic
    }
}

// Método para cambiar el color de fondo de una fila
void MainWindow::cambiar_color_fila(int fila, const QColor &color)
{
    for (int col = 0; col < tabla_pedidos->columnCount(); ++col) // Recorrer todas las columnas de la fila
    {
        QTableWidgetItem *item = tabla_pedidos->item(fila, col);

        if (item)
        {
            item->setBackground(color); // Cambiar el color de fondo de la celda
        }
    }
}

// Método para agregar un nuevo pedido
void MainWindow::agregar_pedido()
{
    // Crear una ventana de diálogo personalizada
    QDialog dialogo(this);
    dialogo.setWindowTitle("Add Order");

    // Crear barras de búsqueda para cliente y producto
    QLineEdit *buscar_cliente = new QLineEdit(&dialogo);
    buscar_cliente->setPlaceholderText(""); // Barra en blanco
    QLineEdit *buscar_producto = new QLineEdit(&dialogo);
    buscar_producto->setPlaceholderText(""); // Barra en blanco

    // Crear un QCompleter para clientes
    QStringList lista_clientes;

    for (int i = 0; i < tabla_clientes->rowCount(); ++i) // Recorrer la tabla de clientes
    {
        lista_clientes << tabla_clientes->item(i, 0)->text(); // Obtener nombres de clientes
    }

    QCompleter *completer_clientes = new QCompleter(lista_clientes, &dialogo);
    completer_clientes->setCaseSensitivity(Qt::CaseInsensitive); // No distinguir mayúsculas/minúsculas
    buscar_cliente->setCompleter(completer_clientes);

    // Crear un QCompleter para productos
    QStringList lista_productos;

    for (int i = 0; i < tabla_productos->rowCount(); ++i) // Recorrer la tabla de productos
    {
        lista_productos << tabla_productos->item(i, 0)->text(); // Obtener nombres de productos
    }

    QCompleter *completer_productos = new QCompleter(lista_productos, &dialogo);
    completer_productos->setCaseSensitivity(Qt::CaseInsensitive); // No distinguir mayúsculas/minúsculas
    buscar_producto->setCompleter(completer_productos);

    // Crear un layout para la ventana de diálogo
    QFormLayout formulario(&dialogo);
    formulario.addRow("Customer:", buscar_cliente);
    formulario.addRow("Product:", buscar_producto);

    // Crear botones de aceptar y cancelar
    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
    formulario.addRow(&botones);

    // Conectar los botones a las acciones correspondientes
    connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

    // Mostrar la ventana de diálogo y verificar si se aceptó
    if (dialogo.exec() == QDialog::Accepted)
    {
        // Obtener los valores ingresados
        QString cliente = buscar_cliente->text();
        QString producto = buscar_producto->text();

        // Verificar que los campos no estén vacíos
        if (!cliente.isEmpty() && !producto.isEmpty())
        {
            // Buscar la dirección del cliente
            QString direccion = "";

            for (int i = 0; i < tabla_clientes->rowCount(); ++i) // Recorrer la tabla de clientes
            {
                if (tabla_clientes->item(i, 0)->text() == cliente)
                {
                    direccion = tabla_clientes->item(i, 1)->text();

                    break;
                }
            }

            // Agregar una nueva fila a la tabla de pedidos
            int fila = tabla_pedidos->rowCount();
            tabla_pedidos->insertRow(fila);

            // Crear los ítems de la fila y establecer el fondo rojo
            QTableWidgetItem *item_cliente = new QTableWidgetItem(cliente);
            item_cliente->setBackground(Qt::red); // Fondo rojo
            QTableWidgetItem *item_direccion = new QTableWidgetItem(direccion);
            item_direccion->setBackground(Qt::red); // Fondo rojo
            QTableWidgetItem *item_producto = new QTableWidgetItem(producto);
            item_producto->setBackground(Qt::red); // Fondo rojo

            // Asignar los ítems a la tabla
            tabla_pedidos->setItem(fila, 0, item_cliente);
            tabla_pedidos->setItem(fila, 1, item_direccion);
            tabla_pedidos->setItem(fila, 2, item_producto);
        }
    }
}

// Método para guardar datos en un archivo
void MainWindow::guardar_datos()
{
    QString ruta_archivo = QCoreApplication::applicationDirPath() + "/Data.txt"; // Ruta del archivo Data.txt
    QFile archivo(ruta_archivo); // Crear un objeto QFile con la ruta del archivo

    // Abrir el archivo para escritura
    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream salida(&archivo); // Crear un flujo de texto para escribir en el archivo

        // Guardar la lista de clientes
        salida << "Customers\n"; // Escribir el encabezado de clientes
        for (int i = 0; i < tabla_clientes->rowCount(); ++i)
        {
            QString nombre = tabla_clientes->item(i, 0)->text();
            QString direccion = tabla_clientes->item(i, 1)->text();
            salida << nombre << " - " << direccion << "\n"; // Escribir cada cliente en el archivo
        }

        // Guardar la lista de productos
        salida << "Products\n"; // Escribir el encabezado de productos
        for (int i = 0; i < tabla_productos->rowCount(); ++i)
        {
            QString nombre = tabla_productos->item(i, 0)->text();
            QString cantidad = tabla_productos->item(i, 1)->text();
            salida << nombre << " - " << cantidad << "\n"; // Escribir cada producto en el archivo
        }

        // Guardar la lista de pedidos
        salida << "Orders\n"; // Escribir el encabezado de pedidos
        for (int i = 0; i < tabla_pedidos->rowCount(); ++i)
        {
            QString cliente = tabla_pedidos->item(i, 0)->text();
            QString direccion = tabla_pedidos->item(i, 1)->text();
            QString producto = tabla_pedidos->item(i, 2)->text();
            salida << cliente << " - " << direccion << " - " << producto << "\n"; // Escribir cada pedido en el archivo
        }

        archivo.close(); // Cerrar el archivo después de escribir
    }
    else
    {
        QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para escritura."); // Mostrar un mensaje de error si no se puede abrir el archivo
    }
}

// Método para guardar datos en un archivo con un nombre y ubicación específicos
void MainWindow::guardar_datos_como()
{
    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Save File", QCoreApplication::applicationDirPath(), "Text Files (*.txt)"); // Abrir diálogo para guardar archivo

    if (!nombre_archivo.isEmpty()) // Verificar que se haya elegido un nombre de archivo
    {
        QFile archivo(nombre_archivo); // Crear un objeto QFile con el nombre elegido

        if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) // Abrir el archivo para escritura
        {
            QTextStream salida(&archivo); // Crear un flujo de texto para escribir en el archivo

            // Guardar la lista de clientes
            salida << "Customers\n"; // Escribir el encabezado de clientes
            for (int i = 0; i < tabla_clientes->rowCount(); ++i)
            {
                QString nombre = tabla_clientes->item(i, 0)->text();
                QString direccion = tabla_clientes->item(i, 1)->text();
                salida << nombre << " - " << direccion << "\n"; // Escribir cada cliente en el archivo
            }

            // Guardar la lista de productos
            salida << "Products\n"; // Escribir el encabezado de productos
            for (int i = 0; i < tabla_productos->rowCount(); ++i)
            {
                QString nombre = tabla_productos->item(i, 0)->text();
                QString cantidad = tabla_productos->item(i, 1)->text();
                salida << nombre << " - " << cantidad << "\n"; // Escribir cada producto en el archivo
            }

            // Guardar la lista de pedidos
            salida << "Orders\n"; // Escribir el encabezado de pedidos
            for (int i = 0; i < tabla_pedidos->rowCount(); ++i)
            {
                QString cliente = tabla_pedidos->item(i, 0)->text();
                QString direccion = tabla_pedidos->item(i, 1)->text();
                QString producto = tabla_pedidos->item(i, 2)->text();
                salida << cliente << " - " << direccion << " - " << producto << "\n"; // Escribir cada pedido en el archivo
            }

            archivo.close(); // Cerrar el archivo después de escribir
        }
        else
        {
            QMessageBox::warning(this, "Error", "No se pudo abrir el archivo para escritura."); // Mostrar un mensaje de error si no se puede abrir el archivo
        }
    }
}

// Método para cargar datos desde un archivo
void MainWindow::cargar_datos()
{
    QString nombre_archivo = QFileDialog::getOpenFileName(this, "Open File", QCoreApplication::applicationDirPath(), "Text Files (*.txt)"); // Abrir diálogo para cargar archivo

    if (!nombre_archivo.isEmpty()) // Verificar que se haya elegido un nombre de archivo
    {
        QFile archivo(nombre_archivo); // Crear un objeto QFile con el nombre elegido

        if (archivo.open(QIODevice::ReadOnly | QIODevice::Text)) { // Abrir el archivo para lectura
            QTextStream entrada(&archivo); // Crear un flujo de texto para leer del archivo
            QString linea; // Variable para almacenar cada línea leída
            QString seccion_actual; // Variable para identificar la sección actual

            // Leer cada línea del archivo
            while (!entrada.atEnd()) // Mientras no se llegue al final del archivo
            {
                linea = entrada.readLine(); // Leer una línea

                // Determinar la sección actual (clients, products o orders)
                if (linea.startsWith("Customers"))
                {
                    seccion_actual = "Customers"; // Actualizar la sección a clients
                }
                else if (linea.startsWith("Products"))
                {
                    seccion_actual = "Products"; // Actualizar la sección a products
                }
                else if (linea.startsWith("Orders"))
                {
                    seccion_actual = "Orders"; // Actualizar la sección a orders
                }
                else
                {
                    // Agregar la línea a la lista correspondiente
                    if (seccion_actual == "Customers")
                    {
                        QStringList datos = linea.split(" - ");

                        if (datos.size() == 2)
                        {
                            int fila = tabla_clientes->rowCount();
                            tabla_clientes->insertRow(fila);
                            tabla_clientes->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                            tabla_clientes->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                        }
                    }
                    else if (seccion_actual == "Products")
                    {
                        QStringList datos = linea.split(" - ");

                        if (datos.size() == 2)
                        {
                            int fila = tabla_productos->rowCount();
                            tabla_productos->insertRow(fila);
                            tabla_productos->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                            tabla_productos->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                        }
                    }
                    else if (seccion_actual == "Orders")
                    {
                        QStringList datos = linea.split(" - ");

                        if (datos.size() == 3)
                        {
                            int fila = tabla_pedidos->rowCount();
                            tabla_pedidos->insertRow(fila);
                            tabla_pedidos->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                            tabla_pedidos->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                            tabla_pedidos->setItem(fila, 2, new QTableWidgetItem(datos[2]));

                            // Establecer el color de fondo de la fila en rojo
                            for (int col = 0; col < tabla_pedidos->columnCount(); ++col)
                            {
                                QTableWidgetItem *item = tabla_pedidos->item(fila, col);

                                if (item)
                                {
                                    item->setBackground(Qt::red); // Fondo rojo
                                }
                            }
                        }
                    }
                }
            }
            archivo.close(); // Cerrar el archivo después de leer
        }
    }
}

// Método para cargar datos por defecto desde el archivo Data.txt
void MainWindow::cargar_datos_por_defecto()
{
    QString ruta_archivo = QCoreApplication::applicationDirPath() + "/Data.txt"; // Ruta del archivo Data.txt
    QFile archivo(ruta_archivo); // Crear un objeto QFile con la ruta del archivo

    if (archivo.exists() && archivo.open(QIODevice::ReadOnly | QIODevice::Text)) // Verificar si el archivo existe y abrirlo
    {
        QTextStream entrada(&archivo); // Crear un flujo de texto para leer del archivo
        QString linea; // Variable para almacenar cada línea leída
        QString seccion_actual; // Variable para identificar la sección actual

        // Leer cada línea del archivo
        while (!entrada.atEnd())
        {
            linea = entrada.readLine(); // Leer una línea del archivo

            // Determinar la sección actual (clients, products o orders)
            if (linea.startsWith("Customers"))
            {
                seccion_actual = "Customers"; // Actualizar la sección a clients
            }
            else if (linea.startsWith("Products"))
            {
                seccion_actual = "Products"; // Actualizar la sección a products
            }
            else if (linea.startsWith("Orders"))
            {
                seccion_actual = "Orders"; // Actualizar la sección a orders
            }
            else
            {
                // Agregar la línea a la lista correspondiente según la sección actual
                if (seccion_actual == "Customers")
                {
                    QStringList datos = linea.split(" - ");

                    if (datos.size() == 2)
                    {
                        int fila = tabla_clientes->rowCount();
                        tabla_clientes->insertRow(fila);
                        tabla_clientes->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                        tabla_clientes->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                    }
                }
                else if (seccion_actual == "Products")
                {
                    QStringList datos = linea.split(" - ");

                    if (datos.size() == 2)
                    {
                        int fila = tabla_productos->rowCount();
                        tabla_productos->insertRow(fila);
                        tabla_productos->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                        tabla_productos->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                    }
                }
                else if (seccion_actual == "Orders")
                {
                    QStringList datos = linea.split(" - ");

                    if (datos.size() == 3)
                    {
                        int fila = tabla_pedidos->rowCount();
                        tabla_pedidos->insertRow(fila);
                        tabla_pedidos->setItem(fila, 0, new QTableWidgetItem(datos[0]));
                        tabla_pedidos->setItem(fila, 1, new QTableWidgetItem(datos[1]));
                        tabla_pedidos->setItem(fila, 2, new QTableWidgetItem(datos[2]));

                        // Establecer el color de fondo de la fila en rojo
                        for (int col = 0; col < tabla_pedidos->columnCount(); ++col)
                        {
                            QTableWidgetItem *item = tabla_pedidos->item(fila, col);

                            if (item)
                            {
                                item->setBackground(Qt::red); // Fondo rojo
                            }
                        }
                    }
                }
            }
        }
        archivo.close(); // Cerrar el archivo después de leer
    }
}

// Método para filtrar elementos en las listas según el texto ingresado
void MainWindow::filtrar_elementos(const QString &texto)
{
    // Filtrar elementos en la tabla de clientes
    for (int i = 0; i < tabla_clientes->rowCount(); ++i)
    {
        bool coincide = false;
        for (int j = 0; j < tabla_clientes->columnCount(); ++j)
        {
            if (tabla_clientes->item(i, j)->text().toLower().contains(texto.toLower()))
            {
                coincide = true;

                break;
            }
        }
        tabla_clientes->setRowHidden(i, !coincide);
    }

    // Filtrar elementos en la tabla de productos
    for (int i = 0; i < tabla_productos->rowCount(); ++i)
    {
        bool coincide = false;

        for (int j = 0; j < tabla_productos->columnCount(); ++j)
        {
            if (tabla_productos->item(i, j)->text().toLower().contains(texto.toLower()))
            {
                coincide = true;

                break;
            }
        }
        tabla_productos->setRowHidden(i, !coincide);
    }

    // Filtrar elementos en la tabla de pedidos
    for (int i = 0; i < tabla_pedidos->rowCount(); ++i)
    {
        bool coincide = false;

        for (int j = 0; j < tabla_pedidos->columnCount(); ++j)
        {
            if (tabla_pedidos->item(i, j)->text().toLower().contains(texto.toLower()))
            {
                coincide = true;

                break;
            }
        }
        tabla_pedidos->setRowHidden(i, !coincide);
    }
}

// Método para agregar un nuevo cliente
void MainWindow::agregar_cliente()
{
    QDialog dialogo(this);
    dialogo.setWindowTitle("Add Client");

    QLineEdit *campo_nombre = new QLineEdit(&dialogo);
    QLineEdit *campo_direccion = new QLineEdit(&dialogo);

    QFormLayout formulario(&dialogo);
    formulario.addRow("Name:", campo_nombre);
    formulario.addRow("Address:", campo_direccion);

    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
    formulario.addRow(&botones);

    connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

    if (dialogo.exec() == QDialog::Accepted)
    {
        QString nombre = campo_nombre->text();
        QString direccion = campo_direccion->text();

        if (!nombre.isEmpty() && !direccion.isEmpty())
        {
            int fila = tabla_clientes->rowCount();
            tabla_clientes->insertRow(fila);
            tabla_clientes->setItem(fila, 0, new QTableWidgetItem(nombre));
            tabla_clientes->setItem(fila, 1, new QTableWidgetItem(direccion));
        }
    }
}

// Método para eliminar un cliente seleccionado
void MainWindow::eliminar_cliente()
{
    int fila = tabla_clientes->currentRow();
    if (fila >= 0)
    {
        tabla_clientes->removeRow(fila);
    }
}

// Método para editar un cliente seleccionado
void MainWindow::editar_cliente()
{
    int fila = tabla_clientes->currentRow();

    if (fila >= 0)
    {
        QString nombre = tabla_clientes->item(fila, 0)->text();
        QString direccion = tabla_clientes->item(fila, 1)->text();

        QDialog dialogo(this);
        dialogo.setWindowTitle("Edit Client");

        QLineEdit *campo_nombre = new QLineEdit(&dialogo);
        campo_nombre->setText(nombre);
        QLineEdit *campo_direccion = new QLineEdit(&dialogo);
        campo_direccion->setText(direccion);

        QFormLayout formulario(&dialogo);
        formulario.addRow("Name:", campo_nombre);
        formulario.addRow("Address:", campo_direccion);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
        formulario.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

        if (dialogo.exec() == QDialog::Accepted)
        {
            tabla_clientes->setItem(fila, 0, new QTableWidgetItem(campo_nombre->text()));
            tabla_clientes->setItem(fila, 1, new QTableWidgetItem(campo_direccion->text()));
        }
    }
}

// Método para agregar un nuevo producto
void MainWindow::agregar_producto()
{
    QDialog dialogo(this);
    dialogo.setWindowTitle("Add Product");

    QLineEdit *campo_nombre = new QLineEdit(&dialogo);
    QLineEdit *campo_cantidad = new QLineEdit(&dialogo);

    QFormLayout formulario(&dialogo);
    formulario.addRow("Product Name:", campo_nombre);
    formulario.addRow("Quantity:", campo_cantidad);

    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
    formulario.addRow(&botones);

    connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

    if (dialogo.exec() == QDialog::Accepted)
    {
        QString nombre = campo_nombre->text();
        QString cantidad = campo_cantidad->text();

        if (!nombre.isEmpty() && !cantidad.isEmpty())
        {
            int fila = tabla_productos->rowCount();
            tabla_productos->insertRow(fila);
            tabla_productos->setItem(fila, 0, new QTableWidgetItem(nombre));
            tabla_productos->setItem(fila, 1, new QTableWidgetItem(cantidad));
        }
    }
}

// Método para eliminar un producto seleccionado
void MainWindow::eliminar_producto()
{
    int fila = tabla_productos->currentRow();

    if (fila >= 0)
    {
        tabla_productos->removeRow(fila);
    }
}

// Método para editar un producto seleccionado
void MainWindow::editar_producto()
{
    int fila = tabla_productos->currentRow();

    if (fila >= 0)
    {
        QString nombre = tabla_productos->item(fila, 0)->text();
        QString cantidad = tabla_productos->item(fila, 1)->text();

        QDialog dialogo(this);
        dialogo.setWindowTitle("Edit Product");

        QLineEdit *campo_nombre = new QLineEdit(&dialogo);
        campo_nombre->setText(nombre);
        QLineEdit *campo_cantidad = new QLineEdit(&dialogo);
        campo_cantidad->setText(cantidad);

        QFormLayout formulario(&dialogo);
        formulario.addRow("Product Name:", campo_nombre);
        formulario.addRow("Quantity:", campo_cantidad);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
        formulario.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

        if (dialogo.exec() == QDialog::Accepted)
        {
            tabla_productos->setItem(fila, 0, new QTableWidgetItem(campo_nombre->text()));
            tabla_productos->setItem(fila, 1, new QTableWidgetItem(campo_cantidad->text()));
        }
    }
}

// Método para eliminar un pedido seleccionado
void MainWindow::eliminar_pedido()
{
    int fila = tabla_pedidos->currentRow();

    if (fila >= 0)
    {
        tabla_pedidos->removeRow(fila);
    }
}

// Método para editar un pedido seleccionado
void MainWindow::editar_pedido()
{
    int fila = tabla_pedidos->currentRow();

    if (fila >= 0)
    {
        QString cliente = tabla_pedidos->item(fila, 0)->text();
        QString direccion = tabla_pedidos->item(fila, 1)->text();
        QString producto = tabla_pedidos->item(fila, 2)->text();

        QDialog dialogo(this);
        dialogo.setWindowTitle("Edit Order");

        QLineEdit *buscar_cliente = new QLineEdit(&dialogo);
        buscar_cliente->setText(cliente);
        buscar_cliente->setPlaceholderText("Search client...");
        QLineEdit *buscar_producto = new QLineEdit(&dialogo);
        buscar_producto->setText(producto);
        buscar_producto->setPlaceholderText("Search product...");

        QFormLayout formulario(&dialogo);
        formulario.addRow("Customer:", buscar_cliente);
        formulario.addRow("Product:", buscar_producto);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialogo);
        formulario.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialogo, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialogo, &QDialog::reject);

        if (dialogo.exec() == QDialog::Accepted)
        {
            QString nuevo_cliente = buscar_cliente->text();
            QString nuevo_producto = buscar_producto->text();

            // Buscar la dirección del cliente
            QString nueva_direccion = "";
            for (int i = 0; i < tabla_clientes->rowCount(); ++i)
            {
                if (tabla_clientes->item(i, 0)->text() == nuevo_cliente)
                {
                    nueva_direccion = tabla_clientes->item(i, 1)->text();

                    break;
                }
            }

            // Actualizar los valores en la tabla
            tabla_pedidos->setItem(fila, 0, new QTableWidgetItem(nuevo_cliente));
            tabla_pedidos->setItem(fila, 1, new QTableWidgetItem(nueva_direccion));
            tabla_pedidos->setItem(fila, 2, new QTableWidgetItem(nuevo_producto));
        }
    }
}
