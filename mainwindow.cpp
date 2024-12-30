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

// Inicializar la clase base QMainWindow con el widget padre
MainWindow::MainWindow(QWidget *ptr_prin) : QMainWindow(ptr_prin)
{
    // Configuración de la ventana principal
    setWindowTitle("Stock Manager"); // Establecer el título de la ventana
    resize(600, 400); // Dimensiones por defecto
    setWindowState(Qt::WindowMaximized); // Maximizar la ventana al inicio

    // Crear el widget central y el layout principal
    QWidget *widget_central = new QWidget(this); // Crear un widget central
    setCentralWidget(widget_central); // Establecer el widget central en la ventana
    QVBoxLayout *layout_vertical = new QVBoxLayout(widget_central); // Crear un layout vertical

    // Establecer el color de fondo del menú
    menuBar()->setStyleSheet("QMenuBar { background-color: #f0f0f0; }");

    // Crear una barra de menú
    QMenuBar *barra_menu = this->menuBar(); // Obtener la barra de menú de la ventana

    // Crear el menú "Archivo"
    QMenu *menu_archivo = barra_menu->addMenu("File"); // Agregar menú "Archivo"

    // Crear acciones para abrir y guardar
    QAction *accion_abrir = new QAction("Open File", this);
    QAction *accion_guardar = new QAction("Save File", this);

    // Conectar las acciones a las funciones correspondientes
    connect(accion_abrir, &QAction::triggered, this, &MainWindow::cargar_datos);
    connect(accion_guardar, &QAction::triggered, this, &MainWindow::guardar_datos);

    // Agregar las acciones al menú "Archivo"
    menu_archivo->addAction(accion_abrir);
    menu_archivo->addAction(accion_guardar);

    // Crear un layout horizontal para los botones y la barra de búsqueda
    QHBoxLayout *layout_horizontal = new QHBoxLayout(); // Crear un layout horizontal
    layout_vertical->addLayout(layout_horizontal); // Agregar el layout horizontal al layout principal

    // Crear una barra de búsqueda
    QLineEdit *barra_busqueda = new QLineEdit(this); // Crear un campo de texto para búsqueda
    barra_busqueda->setPlaceholderText("Search..."); // Establecer un texto de sugerencia
    layout_horizontal->addWidget(barra_busqueda); // Agregar la barra de búsqueda al layout horizontal
    // Conectar la barra de búsqueda a la función de filtrado
    connect(barra_busqueda, &QLineEdit::textChanged, this, &MainWindow::filtrar_elementos); // Conectar el cambio de texto al filtro

    // Crear un QTabWidget para diferentes secciones (Pedidos, Clientes, Productos)
    QTabWidget *tab_widget = new QTabWidget(this); // Crear un widget de pestañas
    layout_vertical->addWidget(tab_widget); // Agregar el widget de pestañas al layout principal

    // Sección de Pedidos
    QWidget *tab_pedidos = new QWidget(this); // Crear una pestaña para pedidos
    QVBoxLayout *layout_pedidos = new QVBoxLayout(tab_pedidos); // Crear un layout vertical para pedidos
    QLabel *etiqueta_pedidos = new QLabel("Current Orders", this); // Crear una etiqueta para la sección de pedidos
    layout_pedidos->addWidget(etiqueta_pedidos); // Agregar la etiqueta al layout de pedidos

    // Tabla de pedidos
    tabla_pedidos = new QTableWidget(this); // Crear una tabla para mostrar pedidos
    tabla_pedidos->setColumnCount(3); // Tres columnas: Cliente, Dirección y Producto
    tabla_pedidos->setHorizontalHeaderLabels({"Customer", "Address", "Product"}); // Establecer los nombres de las columnas
    tabla_pedidos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_pedidos->setShowGrid(false); // Ocultar las líneas de la tabla
    layout_pedidos->addWidget(tabla_pedidos); // Agregar la tabla al layout de pedidos

    // Crear un layout horizontal para los botones de pedidos
    QHBoxLayout *layout_botones_pedidos = new QHBoxLayout(); // Crear un layout horizontal para botones
    QPushButton *boton_agregar_pedido = new QPushButton("Add Order", this); // Crear botón para agregar pedido
    QPushButton *boton_eliminar_pedido = new QPushButton("Delete Order", this); // Crear botón para eliminar pedido
    QPushButton *boton_editar_pedido = new QPushButton("Edit Order", this); // Crear botón para editar pedido
    layout_botones_pedidos->addWidget(boton_agregar_pedido); // Agregar botón de agregar al layout
    layout_botones_pedidos->addWidget(boton_eliminar_pedido); // Agregar botón de eliminar al layout
    layout_botones_pedidos->addWidget(boton_editar_pedido); // Agregar botón de editar al layout
    layout_pedidos->addLayout(layout_botones_pedidos); // Agregar el layout de botones al layout de pedidos

    // Conectar los botones de pedidos a sus funciones
    connect(boton_agregar_pedido, &QPushButton::clicked, this, &MainWindow::agregar_pedido); // Conectar agregar pedido
    connect(boton_eliminar_pedido, &QPushButton::clicked, this, &MainWindow::eliminar_pedido); // Conectar eliminar pedido
    connect(boton_editar_pedido, &QPushButton::clicked, this, &MainWindow::editar_pedido); // Conectar editar pedido

    // Sección de Productos
    QWidget *tab_productos = new QWidget(this); // Crear una pestaña para productos
    QVBoxLayout *layout_productos = new QVBoxLayout(tab_productos); // Crear un layout vertical para productos
    QLabel *etiqueta_productos = new QLabel("Available Products", this); // Crear una etiqueta para la sección de productos
    layout_productos->addWidget(etiqueta_productos); // Agregar la etiqueta al layout de productos

    // Tabla de productos
    tabla_productos = new QTableWidget(this); // Crear una tabla para mostrar productos
    tabla_productos->setColumnCount(2); // Dos columnas: Nombre y Cantidad
    tabla_productos->setHorizontalHeaderLabels({"Product", "Quantity"}); // Establecer los nombres de las columnas
    tabla_productos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_productos->setShowGrid(false); // Ocultar las líneas de la tabla
    layout_productos->addWidget(tabla_productos); // Agregar la tabla al layout de productos

    // Crear un layout horizontal para los botones de productos
    QHBoxLayout *layout_botones_productos = new QHBoxLayout(); // Crear un layout horizontal para botones
    QPushButton *boton_agregar_producto = new QPushButton("Add Product", this); // Crear botón para agregar producto
    QPushButton *boton_eliminar_producto = new QPushButton("Delete Product", this); // Crear botón para eliminar producto
    QPushButton *boton_editar_producto = new QPushButton("Edit Product", this); // Crear botón para editar producto
    layout_botones_productos->addWidget(boton_agregar_producto); // Agregar botón de agregar al layout
    layout_botones_productos->addWidget(boton_eliminar_producto); // Agregar botón de eliminar al layout
    layout_botones_productos->addWidget(boton_editar_producto); // Agregar botón de editar al layout
    layout_productos->addLayout(layout_botones_productos); // Agregar el layout de botones al layout de productos

    // Conectar los botones de productos a sus funciones
    connect(boton_agregar_producto, &QPushButton::clicked, this, &MainWindow::agregar_producto); // Conectar agregar producto
    connect(boton_eliminar_producto, &QPushButton::clicked, this, &MainWindow::eliminar_producto); // Conectar eliminar producto
    connect(boton_editar_producto, &QPushButton::clicked, this, &MainWindow::editar_producto); // Conectar editar producto

    // Sección de Clientes
    QWidget *tab_clientes = new QWidget(this); // Crear una pestaña para clientes
    QVBoxLayout *layout_clientes = new QVBoxLayout(tab_clientes); // Crear un layout vertical para clientes
    QLabel *etiqueta_clientes = new QLabel("Registered Customers", this); // Crear una etiqueta para la sección de clientes
    layout_clientes->addWidget(etiqueta_clientes); // Agregar la etiqueta al layout de clientes

    // Tabla de clientes
    tabla_clientes = new QTableWidget(this); // Crear una tabla para mostrar clientes
    tabla_clientes->setColumnCount(2); // Dos columnas: Nombre y Dirección
    tabla_clientes->setHorizontalHeaderLabels({"Name", "Address"}); // Establecer los nombres de las columnas
    tabla_clientes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); // Ajustar el tamaño de las columnas
    tabla_clientes->setShowGrid(false); // Ocultar las líneas de la tabla
    layout_clientes->addWidget(tabla_clientes); // Agregar la tabla al layout de clientes

    // Crear un layout horizontal para los botones de clientes
    QHBoxLayout *layout_botones_clientes = new QHBoxLayout(); // Crear un layout horizontal para botones
    QPushButton *boton_agregar_cliente = new QPushButton("Add Customer", this); // Crear botón para agregar cliente
    QPushButton *boton_eliminar_cliente = new QPushButton("Delete Customer", this); // Crear botón para eliminar cliente
    QPushButton *boton_editar_cliente = new QPushButton("Edit Customer", this); // Crear botón para editar cliente
    layout_botones_clientes->addWidget(boton_agregar_cliente); // Agregar botón de agregar al layout
    layout_botones_clientes->addWidget(boton_eliminar_cliente); // Agregar botón de eliminar al layout
    layout_botones_clientes->addWidget(boton_editar_cliente); // Agregar botón de editar al layout
    layout_clientes->addLayout(layout_botones_clientes); // Agregar el layout de botones al layout de clientes

    // Conectar los botones de clientes a sus funciones
    connect(boton_agregar_cliente, &QPushButton::clicked, this, &MainWindow::agregar_cliente); // Conectar agregar cliente
    connect(boton_eliminar_cliente, &QPushButton::clicked, this, &MainWindow::eliminar_cliente); // Conectar eliminar cliente
    connect(boton_editar_cliente, &QPushButton::clicked, this, &MainWindow::editar_cliente); // Conectar editar cliente

    // Agregar las pestañas en el orden correcto: Orders, Products, Customers
    tab_widget->addTab(tab_pedidos, "Orders"); // Agregar pestaña de pedidos al widget de pestañas
    tab_widget->addTab(tab_productos, "Products"); // Agregar pestaña de productos al widget de pestañas
    tab_widget->addTab(tab_clientes, "Customers"); // Agregar pestaña de clientes al widget de pestañas

    // Cargar datos automáticamente al iniciar
    cargar_datos_por_defecto();
}

// Método para agregar un nuevo cliente
void MainWindow::agregar_cliente()
{
    // Crear una ventana de diálogo personalizada
    QDialog dialog(this);
    dialog.setWindowTitle("Add Customer");

    // Crear campos de entrada para nombre y dirección
    QLineEdit *campo_nombre = new QLineEdit(&dialog);
    QLineEdit *campo_direccion = new QLineEdit(&dialog);

    // Crear un layout para la ventana de diálogo
    QFormLayout form(&dialog);
    form.addRow("Name:", campo_nombre);
    form.addRow("Address:", campo_direccion);

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
        QString nombre = campo_nombre->text();
        QString direccion = campo_direccion->text();

        // Verificar que los campos no estén vacíos
        if (!nombre.isEmpty() && !direccion.isEmpty())
        {
            // Agregar una nueva fila a la tabla de clientes
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
    int fila = tabla_clientes->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        tabla_clientes->removeRow(fila); // Eliminar la fila seleccionada
    }
}

// Método para editar un cliente seleccionado
void MainWindow::editar_cliente()
{
    int fila = tabla_clientes->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        // Obtener los valores actuales
        QString nombre = tabla_clientes->item(fila, 0)->text();
        QString direccion = tabla_clientes->item(fila, 1)->text();

        // Crear una ventana de diálogo personalizada
        QDialog dialog(this);
        dialog.setWindowTitle("Edit Customer");

        // Crear campos de entrada para nombre y dirección
        QLineEdit *campo_nombre = new QLineEdit(&dialog);
        campo_nombre->setText(nombre);
        QLineEdit *campo_direccion = new QLineEdit(&dialog);
        campo_direccion->setText(direccion);

        // Crear un layout para la ventana de diálogo
        QFormLayout form(&dialog);
        form.addRow("Name:", campo_nombre);
        form.addRow("Address:", campo_direccion);

        // Crear botones de aceptar y cancelar
        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(&botones);

        // Conectar los botones a las acciones correspondientes
        connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        // Mostrar la ventana de diálogo y verificar si se aceptó
        if (dialog.exec() == QDialog::Accepted)
        {
            // Actualizar los valores en la tabla
            tabla_clientes->setItem(fila, 0, new QTableWidgetItem(campo_nombre->text()));
            tabla_clientes->setItem(fila, 1, new QTableWidgetItem(campo_direccion->text()));
        }
    }
}

// Método para agregar un nuevo producto
void MainWindow::agregar_producto()
{
    // Crear una ventana de diálogo personalizada
    QDialog dialog(this);
    dialog.setWindowTitle("Add Product");

    // Crear campos de entrada para nombre y cantidad
    QLineEdit *campo_nombre = new QLineEdit(&dialog);
    QLineEdit *campo_cantidad = new QLineEdit(&dialog);

    // Crear un layout para la ventana de diálogo
    QFormLayout form(&dialog);
    form.addRow("Product Name:", campo_nombre);
    form.addRow("Quantity:", campo_cantidad);

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
        QString nombre = campo_nombre->text();
        QString cantidad = campo_cantidad->text();

        // Verificar que los campos no estén vacíos
        if (!nombre.isEmpty() && !cantidad.isEmpty())
        {
            // Agregar una nueva fila a la tabla de productos
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
    int fila = tabla_productos->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        tabla_productos->removeRow(fila); // Eliminar la fila seleccionada
    }
}

// Método para editar un producto seleccionado
void MainWindow::editar_producto()
{
    int fila = tabla_productos->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        // Obtener los valores actuales
        QString nombre = tabla_productos->item(fila, 0)->text();
        QString cantidad = tabla_productos->item(fila, 1)->text();

        // Crear una ventana de diálogo personalizada
        QDialog dialog(this);
        dialog.setWindowTitle("Edit Product");

        // Crear campos de entrada para nombre y cantidad
        QLineEdit *campo_nombre = new QLineEdit(&dialog);
        campo_nombre->setText(nombre);
        QLineEdit *campo_cantidad = new QLineEdit(&dialog);
        campo_cantidad->setText(cantidad);

        // Crear un layout para la ventana de diálogo
        QFormLayout form(&dialog);
        form.addRow("Product Name:", campo_nombre);
        form.addRow("Quantity:", campo_cantidad);

        // Crear botones de aceptar y cancelar
        QDialogButtonBox botones(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dialog);
        form.addRow(&botones);

        // Conectar los botones a las acciones correspondientes
        connect(&botones, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        connect(&botones, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        // Mostrar la ventana de diálogo y verificar si se aceptó
        if (dialog.exec() == QDialog::Accepted)
        {
            // Actualizar los valores en la tabla
            tabla_productos->setItem(fila, 0, new QTableWidgetItem(campo_nombre->text()));
            tabla_productos->setItem(fila, 1, new QTableWidgetItem(campo_cantidad->text()));
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
            tabla_pedidos->setItem(fila, 0, new QTableWidgetItem(cliente));
            tabla_pedidos->setItem(fila, 1, new QTableWidgetItem(direccion));
            tabla_pedidos->setItem(fila, 2, new QTableWidgetItem(producto));
        }
    }
}

// Método para eliminar un pedido seleccionado
void MainWindow::eliminar_pedido()
{
    int fila = tabla_pedidos->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        tabla_pedidos->removeRow(fila); // Eliminar la fila seleccionada
    }
}

// Método para editar un pedido seleccionado
void MainWindow::editar_pedido()
{
    int fila = tabla_pedidos->currentRow(); // Obtener la fila seleccionada
    if (fila >= 0) // Verificar si hay una fila seleccionada
    {
        // Obtener los valores actuales
        QString cliente = tabla_pedidos->item(fila, 0)->text();
        QString direccion = tabla_pedidos->item(fila, 1)->text();
        QString producto = tabla_pedidos->item(fila, 2)->text();

        // Crear una ventana de diálogo personalizada
        QDialog dialog(this);
        dialog.setWindowTitle("Edit Order");

        // Crear barras de búsqueda para cliente y producto
        QLineEdit *buscar_cliente = new QLineEdit(&dialog);
        buscar_cliente->setText(cliente);
        buscar_cliente->setPlaceholderText("Search customer...");
        QLineEdit *buscar_producto = new QLineEdit(&dialog);
        buscar_producto->setText(producto);
        buscar_producto->setPlaceholderText("Search product...");

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

// Método para cargar datos desde el archivo Data.txt
void MainWindow::cargar_datos_por_defecto()
{
    // Construir la ruta del archivo Data.txt en el directorio del ejecutable
    QString ruta_archivo = QCoreApplication::applicationDirPath() + "/Data.txt";

    // Crear un objeto QFile con la ruta del archivo
    QFile archivo(ruta_archivo);

    // Verificar si el archivo existe y abrirlo en modo lectura
    if (archivo.exists() && archivo.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // Crear un flujo de texto para leer del archivo
        QTextStream en(&archivo);

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
                    }
                }
            }
        }
        archivo.close(); // Cerrar el archivo después de leer
    }
}

// Método para guardar datos en un archivo
void MainWindow::guardar_datos()
{
    QString nombre_archivo = QFileDialog::getSaveFileName(this, "Save File As", QCoreApplication::applicationDirPath(), "Text Files (*.txt)"); // Abrir diálogo para guardar archivo
    if (!nombre_archivo.isEmpty()) // Verificar que se haya elegido un nombre de archivo
    {
        QFile archivo(nombre_archivo); // Crear un objeto QFile con el nombre elegido
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
                        }
                    }
                }
            }
            archivo.close(); // Cerrar el archivo después de leer
        }
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
