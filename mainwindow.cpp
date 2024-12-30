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

// En el constructor de MainWindow
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    // Configuración de la ventana principal
    setWindowTitle("Stock Manager");
    resize(600, 400);
    setWindowState(Qt::WindowMaximized);

    // Crear el widget central y el layout principal
    QWidget *widget_central = new QWidget(this);
    setCentralWidget(widget_central);
    QVBoxLayout *layout_vertical = new QVBoxLayout(widget_central);

    // Crear una barra de herramientas en la parte superior
    QToolBar *toolbar = addToolBar("Main Toolbar");
    toolbar->setOrientation(Qt::Horizontal); // Hacer la barra horizontal
    toolbar->setIconSize(QSize(16, 16)); // Reducir el tamaño de los íconos
    addToolBar(Qt::TopToolBarArea, toolbar); // Mover la barra a la parte superior

    // Aplicar estilo para eliminar bordes y efecto 3D
        toolbar->setStyleSheet(
            "QToolBar { border: none; background: transparent; }"
            "QToolButton { border: none; background: transparent; }"
        );

    // Agregar el ícono de guardado a la barra de herramientas
    QAction *actionSave = new QAction(QIcon("Save.png"), "Save", this);
    toolbar->addAction(actionSave);
    connect(actionSave, &QAction::triggered, this, &MainWindow::guardar_datos);

    // Establecer el color de fondo del menú
    menuBar()->setStyleSheet("QMenuBar { background-color: #f0f0f0; }");

    // Crear una barra de menú
    QMenuBar *barra_menu = this->menuBar();

    // Crear el menú "Archivo"
    QMenu *menu_archivo = barra_menu->addMenu("File");

    // Crear acciones para abrir y guardar
    QAction *accion_abrir = new QAction("Open File", this);
    QAction *accion_guardar = new QAction("Save File As", this);

    // Conectar las acciones a las funciones correspondientes
    connect(accion_abrir, &QAction::triggered, this, &MainWindow::cargar_datos);
    connect(accion_guardar, &QAction::triggered, this, &MainWindow::guardar_datos);

    // Agregar las acciones al menú "Archivo"
    menu_archivo->addAction(accion_abrir);
    menu_archivo->addAction(accion_guardar);

    // Crear un layout horizontal para los botones y la barra de búsqueda
    QHBoxLayout *layout_horizontal = new QHBoxLayout();
    layout_vertical->addLayout(layout_horizontal);

    // Crear una barra de búsqueda
    QLineEdit *barra_busqueda = new QLineEdit(this);
    barra_busqueda->setPlaceholderText("Search...");
    layout_horizontal->addWidget(barra_busqueda);
    connect(barra_busqueda, &QLineEdit::textChanged, this, &MainWindow::filtrar_elementos);

    // Crear un QTabWidget para diferentes secciones (Pedidos, Clientes, Productos)
    QTabWidget *tab_widget = new QTabWidget(this);
    layout_vertical->addWidget(tab_widget);

    // Sección de Pedidos
    QWidget *tab_pedidos = new QWidget(this);
    QVBoxLayout *layout_pedidos = new QVBoxLayout(tab_pedidos);
    QLabel *etiqueta_pedidos = new QLabel("Current Orders", this);
    layout_pedidos->addWidget(etiqueta_pedidos);

    // Tabla de pedidos
    tabla_pedidos = new QTableWidget(this);
    tabla_pedidos->setColumnCount(3);
    tabla_pedidos->setHorizontalHeaderLabels({"Customer", "Address", "Product"});
    tabla_pedidos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla_pedidos->setShowGrid(false);
    tabla_pedidos->setContextMenuPolicy(Qt::CustomContextMenu);
    layout_pedidos->addWidget(tabla_pedidos);

    // Conectar el evento de clic derecho al menú contextual
    connect(tabla_pedidos, &QTableWidget::customContextMenuRequested, this, &MainWindow::mostrarMenuContextual);

    // Crear un layout horizontal para los botones de pedidos
    QHBoxLayout *layout_botones_pedidos = new QHBoxLayout();
    QPushButton *boton_agregar_pedido = new QPushButton("Add Order", this);
    QPushButton *boton_eliminar_pedido = new QPushButton("Delete Order", this);
    QPushButton *boton_editar_pedido = new QPushButton("Edit Order", this);
    QPushButton *boton_afectar = new QPushButton("Affect", this);
    layout_botones_pedidos->addWidget(boton_agregar_pedido);
    layout_botones_pedidos->addWidget(boton_eliminar_pedido);
    layout_botones_pedidos->addWidget(boton_editar_pedido);
    layout_botones_pedidos->addWidget(boton_afectar);
    layout_pedidos->addLayout(layout_botones_pedidos);

    // Conectar los botones de pedidos a sus funciones
    connect(boton_agregar_pedido, &QPushButton::clicked, this, &MainWindow::agregar_pedido);
    connect(boton_eliminar_pedido, &QPushButton::clicked, this, &MainWindow::eliminar_pedido);
    connect(boton_editar_pedido, &QPushButton::clicked, this, &MainWindow::editar_pedido);
    connect(boton_afectar, &QPushButton::clicked, this, &MainWindow::afectar_inventario);

    // Sección de Productos
    QWidget *tab_productos = new QWidget(this);
    QVBoxLayout *layout_productos = new QVBoxLayout(tab_productos);
    QLabel *etiqueta_productos = new QLabel("Available Products", this);
    layout_productos->addWidget(etiqueta_productos);

    // Tabla de productos
    tabla_productos = new QTableWidget(this);
    tabla_productos->setColumnCount(2);
    tabla_productos->setHorizontalHeaderLabels({"Product", "Quantity"});
    tabla_productos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla_productos->setShowGrid(false);
    layout_productos->addWidget(tabla_productos);

    // Crear un layout horizontal para los botones de productos
    QHBoxLayout *layout_botones_productos = new QHBoxLayout();
    QPushButton *boton_agregar_producto = new QPushButton("Add Product", this);
    QPushButton *boton_eliminar_producto = new QPushButton("Delete Product", this);
    QPushButton *boton_editar_producto = new QPushButton("Edit Product", this);
    layout_botones_productos->addWidget(boton_agregar_producto);
    layout_botones_productos->addWidget(boton_eliminar_producto);
    layout_botones_productos->addWidget(boton_editar_producto);
    layout_productos->addLayout(layout_botones_productos);

    // Conectar los botones de productos a sus funciones
    connect(boton_agregar_producto, &QPushButton::clicked, this, &MainWindow::agregar_producto);
    connect(boton_eliminar_producto, &QPushButton::clicked, this, &MainWindow::eliminar_producto);
    connect(boton_editar_producto, &QPushButton::clicked, this, &MainWindow::editar_producto);

    // Sección de Clientes
    QWidget *tab_clientes = new QWidget(this);
    QVBoxLayout *layout_clientes = new QVBoxLayout(tab_clientes);
    QLabel *etiqueta_clientes = new QLabel("Registered Customers", this);
    layout_clientes->addWidget(etiqueta_clientes);

    // Tabla de clientes
    tabla_clientes = new QTableWidget(this);
    tabla_clientes->setColumnCount(2);
    tabla_clientes->setHorizontalHeaderLabels({"Name", "Address"});
    tabla_clientes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    tabla_clientes->setShowGrid(false);
    layout_clientes->addWidget(tabla_clientes);

    // Crear un layout horizontal para los botones de clientes
    QHBoxLayout *layout_botones_clientes = new QHBoxLayout();
    QPushButton *boton_agregar_cliente = new QPushButton("Add Customer", this);
    QPushButton *boton_eliminar_cliente = new QPushButton("Delete Customer", this);
    QPushButton *boton_editar_cliente = new QPushButton("Edit Customer", this);
    layout_botones_clientes->addWidget(boton_agregar_cliente);
    layout_botones_clientes->addWidget(boton_eliminar_cliente);
    layout_botones_clientes->addWidget(boton_editar_cliente);
    layout_clientes->addLayout(layout_botones_clientes);

    // Conectar los botones de clientes a sus funciones
    connect(boton_agregar_cliente, &QPushButton::clicked, this, &MainWindow::agregar_cliente);
    connect(boton_eliminar_cliente, &QPushButton::clicked, this, &MainWindow::eliminar_cliente);
    connect(boton_editar_cliente, &QPushButton::clicked, this, &MainWindow::editar_cliente);

    // Agregar las pestañas en el orden correcto: Orders, Products, Customers
    tab_widget->addTab(tab_pedidos, "Orders");
    tab_widget->addTab(tab_productos, "Products");
    tab_widget->addTab(tab_clientes, "Customers");

    // Cargar datos automáticamente al iniciar
    cargar_datos_por_defecto();
}

// Método para afectar el inventario
void MainWindow::afectar_inventario()
{
    for (int i = 0; i < tabla_pedidos->rowCount(); ++i)
    {
        // Obtener el color de fondo de la primera celda de la fila
        QTableWidgetItem *item = tabla_pedidos->item(i, 0);
        if (item && item->background() == Qt::green) // Verificar si el fondo es verde
        {
            QString producto = tabla_pedidos->item(i, 2)->text(); // Obtener el nombre del producto
            for (int j = 0; j < tabla_productos->rowCount(); ++j)
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
void MainWindow::mostrarMenuContextual(const QPoint &pos)
{
    QTableWidgetItem *item = tabla_pedidos->itemAt(pos); // Obtener el ítem en la posición del clic
    if (item) // Verificar si hay un ítem válido
    {
        QMenu menu(this); // Crear un menú contextual
        QAction *accionRojo = menu.addAction("Red"); // Agregar opción "Red"
        QAction *accionVerde = menu.addAction("Green"); // Agregar opción "Green"

        // Conectar las acciones a las funciones correspondientes
        connect(accionRojo, &QAction::triggered, this, [this, item]() { cambiarColorFila(item->row(), Qt::red); });
        connect(accionVerde, &QAction::triggered, this, [this, item]() { cambiarColorFila(item->row(), Qt::green); });

        menu.exec(tabla_pedidos->viewport()->mapToGlobal(pos)); // Mostrar el menú en la posición del clic
    }
}

// Método para cambiar el color de fondo de una fila
void MainWindow::cambiarColorFila(int fila, const QColor &color)
{
    for (int col = 0; col < tabla_pedidos->columnCount(); ++col)
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
    QDialog dialog(this);
    dialog.setWindowTitle("Add Order");

    // Crear barras de búsqueda para cliente y producto
    QLineEdit *buscar_cliente = new QLineEdit(&dialog);
    buscar_cliente->setPlaceholderText(""); // Barra en blanco
    QLineEdit *buscar_producto = new QLineEdit(&dialog);
    buscar_producto->setPlaceholderText(""); // Barra en blanco

    // Crear un QCompleter para clientes
    QStringList lista_clientes;
    for (int i = 0; i < tabla_clientes->rowCount(); ++i) {
        lista_clientes << tabla_clientes->item(i, 0)->text(); // Obtener nombres de clientes
    }
    QCompleter *completer_clientes = new QCompleter(lista_clientes, &dialog);
    completer_clientes->setCaseSensitivity(Qt::CaseInsensitive); // No distinguir mayúsculas/minúsculas
    buscar_cliente->setCompleter(completer_clientes);

    // Crear un QCompleter para productos
    QStringList lista_productos;
    for (int i = 0; i < tabla_productos->rowCount(); ++i) {
        lista_productos << tabla_productos->item(i, 0)->text(); // Obtener nombres de productos
    }
    QCompleter *completer_productos = new QCompleter(lista_productos, &dialog);
    completer_productos->setCaseSensitivity(Qt::CaseInsensitive); // No distinguir mayúsculas/minúsculas
    buscar_producto->setCompleter(completer_productos);

    // Crear un layout para la ventana de diálogo
    QFormLayout form(&dialog);
    form.addRow("Customer:", buscar_cliente);
    form.addRow("Product:", buscar_producto);

    // Crear botones de aceptar y cancelar
    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&botones);

    // Conectar los botones a las acciones correspondientes
    connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    // Mostrar la ventana de diálogo y verificar si se aceptó
    if (dialog.exec() == QDialog::Accepted)
    {
        // Obtener los valores ingresados
        QString cliente = buscar_cliente->text();
        QString producto = buscar_producto->text();

        // Verificar que los campos no estén vacíos
        if (!cliente.isEmpty() && !producto.isEmpty())
        {
            // Buscar la dirección del cliente
            QString direccion = "";
            for (int i = 0; i < tabla_clientes->rowCount(); ++i)
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
            QTableWidgetItem *itemCliente = new QTableWidgetItem(cliente);
            itemCliente->setBackground(Qt::red); // Fondo rojo
            QTableWidgetItem *itemDireccion = new QTableWidgetItem(direccion);
            itemDireccion->setBackground(Qt::red); // Fondo rojo
            QTableWidgetItem *itemProducto = new QTableWidgetItem(producto);
            itemProducto->setBackground(Qt::red); // Fondo rojo

            // Asignar los ítems a la tabla
            tabla_pedidos->setItem(fila, 0, itemCliente);
            tabla_pedidos->setItem(fila, 1, itemDireccion);
            tabla_pedidos->setItem(fila, 2, itemProducto);
        }
    }
}

// Método para guardar datos en un archivo
void MainWindow::guardar_datos()
{
    QString ruta_archivo = QCoreApplication::applicationDirPath() + "/Data.txt"; // Ruta del archivo Data.txt
    QFile archivo(ruta_archivo); // Crear un objeto QFile con la ruta del archivo
    if (archivo.open(QIODevice::WriteOnly | QIODevice::Text)) // Abrir el archivo para escritura
    {
        QTextStream salida(&archivo); // Crear un flujo de texto para escribir en el archivo

        // Guardar la lista de clientes
        salida << "Customers\n"; // Escribir el encabezado de clientes
        for (int i = 0; i < tabla_clientes->rowCount(); ++i) // Recorrer la tabla de clientes
        {
            QString nombre = tabla_clientes->item(i, 0)->text();
            QString direccion = tabla_clientes->item(i, 1)->text();
            salida << nombre << " - " << direccion << "\n"; // Escribir cada cliente en el archivo
        }

        // Guardar la lista de productos
        salida << "Products\n"; // Escribir el encabezado de productos
        for (int i = 0; i < tabla_productos->rowCount(); ++i) // Recorrer la tabla de productos
        {
            QString nombre = tabla_productos->item(i, 0)->text();
            QString cantidad = tabla_productos->item(i, 1)->text();
            salida << nombre << " - " << cantidad << "\n"; // Escribir cada producto en el archivo
        }

        // Guardar la lista de pedidos
        salida << "Orders\n"; // Escribir el encabezado de pedidos
        for (int i = 0; i < tabla_pedidos->rowCount(); ++i) // Recorrer la tabla de pedidos
        {
            QString cliente = tabla_pedidos->item(i, 0)->text();
            QString direccion = tabla_pedidos->item(i, 1)->text();
            QString producto = tabla_pedidos->item(i, 2)->text();
            salida << cliente << " - " << direccion << " - " << producto << "\n"; // Escribir cada pedido en el archivo
        }

        archivo.close(); // Cerrar el archivo después de escribir
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
            QTextStream en(&archivo); // Crear un flujo de texto para leer del archivo
            QString linea; // Variable para almacenar cada línea leída
            QString seccion_actual; // Variable para identificar la sección actual

            // Leer cada línea del archivo
            while (!en.atEnd()) // Mientras no se llegue al final del archivo
            {
                linea = en.readLine(); // Leer una línea
                // Determinar la sección actual (clientes, productos o pedidos)
                if (linea.startsWith("Customers"))
                {
                    seccion_actual = "Customers"; // Actualizar la sección a clientes
                }
                else if (linea.startsWith("Products"))
                {
                    seccion_actual = "Products"; // Actualizar la sección a productos
                }
                else if (linea.startsWith("Orders"))
                {
                    seccion_actual = "Orders"; // Actualizar la sección a pedidos
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
        QTextStream en(&archivo); // Crear un flujo de texto para leer del archivo
        QString linea; // Variable para almacenar cada línea leída
        QString seccion_actual; // Variable para identificar la sección actual

        // Leer cada línea del archivo
        while (!en.atEnd())
        {
            linea = en.readLine(); // Leer una línea del archivo

            // Determinar la sección actual (clientes, productos o pedidos)
            if (linea.startsWith("Customers"))
            {
                seccion_actual = "Customers"; // Actualizar la sección a clientes
            }
            else if (linea.startsWith("Products"))
            {
                seccion_actual = "Products"; // Actualizar la sección a productos
            }
            else if (linea.startsWith("Orders"))
            {
                seccion_actual = "Orders"; // Actualizar la sección a pedidos
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
    QDialog dialog(this);
    dialog.setWindowTitle("Add Customer");

    QLineEdit *campo_nombre = new QLineEdit(&dialog);
    QLineEdit *campo_direccion = new QLineEdit(&dialog);

    QFormLayout form(&dialog);
    form.addRow("Name:", campo_nombre);
    form.addRow("Address:", campo_direccion);

    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&botones);

    connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
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

        QDialog dialog(this);
        dialog.setWindowTitle("Edit Customer");

        QLineEdit *campo_nombre = new QLineEdit(&dialog);
        campo_nombre->setText(nombre);
        QLineEdit *campo_direccion = new QLineEdit(&dialog);
        campo_direccion->setText(direccion);

        QFormLayout form(&dialog);
        form.addRow("Name:", campo_nombre);
        form.addRow("Address:", campo_direccion);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted)
        {
            tabla_clientes->setItem(fila, 0, new QTableWidgetItem(campo_nombre->text()));
            tabla_clientes->setItem(fila, 1, new QTableWidgetItem(campo_direccion->text()));
        }
    }
}

// Método para agregar un nuevo producto
void MainWindow::agregar_producto()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Add Product");

    QLineEdit *campo_nombre = new QLineEdit(&dialog);
    QLineEdit *campo_cantidad = new QLineEdit(&dialog);

    QFormLayout form(&dialog);
    form.addRow("Product Name:", campo_nombre);
    form.addRow("Quantity:", campo_cantidad);

    QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
    form.addRow(&botones);

    connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted)
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

        QDialog dialog(this);
        dialog.setWindowTitle("Edit Product");

        QLineEdit *campo_nombre = new QLineEdit(&dialog);
        campo_nombre->setText(nombre);
        QLineEdit *campo_cantidad = new QLineEdit(&dialog);
        campo_cantidad->setText(cantidad);

        QFormLayout form(&dialog);
        form.addRow("Product Name:", campo_nombre);
        form.addRow("Quantity:", campo_cantidad);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted)
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

        QDialog dialog(this);
        dialog.setWindowTitle("Edit Order");

        QLineEdit *buscar_cliente = new QLineEdit(&dialog);
        buscar_cliente->setText(cliente);
        buscar_cliente->setPlaceholderText("Search customer...");
        QLineEdit *buscar_producto = new QLineEdit(&dialog);
        buscar_producto->setText(producto);
        buscar_producto->setPlaceholderText("Search product...");

        QFormLayout form(&dialog);
        form.addRow("Customer:", buscar_cliente);
        form.addRow("Product:", buscar_producto);

        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(&botones);

        connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        if (dialog.exec() == QDialog::Accepted)
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
