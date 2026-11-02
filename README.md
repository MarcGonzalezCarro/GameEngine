# SuperEngine

## 1. Descripción del proyecto

**SuperEngine** es un motor gráfico desarrollado en C++ que permite cargar, visualizar y gestionar geometrías 3D mediante un editor propio.

El motor utiliza las siguientes tecnologías:

- **SDL3:** gestión de ventanas y entrada.
- **OpenGL:** renderizado gráfico.
- **Dear ImGui:** interfaz gráfica del editor.
- **Assimp:** importación de modelos 3D.
- **DevIL:** carga y gestión de imágenes y texturas.
- **GLM:** operaciones matemáticas y transformaciones 3D.

El proyecto está organizado separando las funcionalidades propias del motor de las herramientas del editor, con el objetivo de mantener una arquitectura modular y facilitar futuras ampliaciones.

**Repositorio:** [[SuperEngine - GitHub](https://github.com/MarcGonzalezCarro/GameEngine)](https://github.com/MarcGonzalezCarro/GameEngine)

### Integrantes del equipo

- **Marc González:** [[GitHub](https://github.com/MarcGonzalezCarro)](https://github.com/MarcGonzalezCarro)
- **Irene Calatrava:** [[GitHub](https://github.com/irentren)](https://github.com/irentren)
- **Lluís Cazorla:** [[GitHub](https://github.com/LluisCazorla)](https://github.com/LluisCazorla)

---

## 2. Cómo utilizar el motor

### 2.1. Carga de modelos y texturas

Al iniciar el programa, el modelo `Baker_house` se carga automáticamente.

También es posible incorporar recursos mediante Drag & Drop:

- **Modelos FBX:** arrastrar un archivo `.fbx` desde cualquier ubicación del ordenador a la ventana del programa.
- **Texturas:** arrastrar archivos `.dds` o `.png` para aplicarlos a los GameObjects seleccionados.

Cada malla importada genera su correspondiente GameObject dentro de la jerarquía de la escena.

### 2.2. Controles de cámara

La cámara dispone de controles de navegación similares a los de Unity:

| Acción | Control |
|---|---|
| Activar movimiento y rotación de cámara | Mantener botón derecho del ratón |
| Mover hacia delante | W |
| Mover hacia atrás | S |
| Mover a la izquierda | A |
| Mover a la derecha | D |
| Aumentar velocidad | Mantener SHIFT |
| Zoom | Rueda del ratón |
| Orbitar alrededor del objeto | ALT + botón izquierdo |
| Enfocar geometría seleccionada | F |

### 2.3. Ventanas del editor

El editor dispone de varias ventanas accesibles desde el menú **View**, que permite mostrarlas u ocultarlas:

- **Scene:** muestra la escena 3D y los modelos cargados.
- **Console:** muestra información del funcionamiento del motor, la carga de modelos y texturas, la inicialización de librerías y los posibles errores.
- **Configuration:** permite consultar los FPS, configurar diferentes parámetros del motor y visualizar información del hardware y del software.
- **Hierarchy:** muestra todos los GameObjects de la escena y permite seleccionar, renombrar, duplicar y eliminar objetos.
- **Inspector:** muestra información de los componentes del GameObject seleccionado.

### 2.4. Inspector

El Inspector permite consultar los componentes del GameObject seleccionado:

- **Transform:** muestra la posición, rotación y escala del objeto. Los cambios realizados en el Transform se reflejan directamente en la escena.
- **Mesh:** muestra información de la geometría y permite visualizar sus normales.
- **Texture:** muestra información de la textura asignada, incluyendo su ruta y dimensiones.

### 2.5. Menú principal

El editor dispone de una barra de herramientas con las siguientes opciones:

- **File:** permite salir del programa.
- **View:** permite mostrar u ocultar las diferentes ventanas del editor.
- **Help:**
  - **Documentation:** abre la página del proyecto en GitHub.
  - **Report a bug:** abre la sección de incidencias del repositorio.
  - **Download latest:** abre la sección de releases.
  - **About:** muestra información del motor, su versión, los integrantes del equipo, las librerías utilizadas y la licencia.

También se dispone de un menú para crear y cargar geometrías básicas.

---

## 3. Funcionalidades extra

Además de las funcionalidades solicitadas, SuperEngine incorpora las siguientes características adicionales:

### 3.1. Transform conectado a la escena

El componente Transform está conectado directamente con el sistema de transformaciones de los objetos.

Cuando se modifica la posición, rotación o escala de un GameObject, el cambio se refleja directamente en la geometría renderizada.

Esto permite visualizar las modificaciones en tiempo real desde el editor.

### 3.2. Gestión de la jerarquía de GameObjects

Los GameObjects pueden organizarse y gestionarse desde la ventana Hierarchy.

Es posible:

- Crear relaciones entre objetos mediante una estructura padre-hijo.
- Renombrar GameObjects.
- Duplicar GameObjects.
- Eliminar GameObjects.

### 3.3. Creación de Empty GameObjects

Desde la ventana Hierarchy se pueden crear Empty GameObjects.

Estos objetos se crean inicialmente únicamente con un componente Transform funcional, permitiendo utilizarlos como elementos organizativos o como base para futuras funcionalidades.


---

## 4. Librerías utilizadas

| Librería | Funcionalidad |
|---|---|
| **SDL3** | Gestión de ventanas, eventos y entrada. |
| **OpenGL** | Renderizado gráfico. |
| **GLM** | Operaciones matemáticas y transformaciones 3D. |
| **Dear ImGui** | Interfaz gráfica del editor. |
| **Assimp** | Importación de modelos 3D. |
| **DevIL** | Carga y gestión de imágenes y texturas. |

---

## 5. Licencia

Este proyecto se distribuye bajo la licencia **MIT**.

Para consultar las condiciones de uso y distribución, véase el archivo `LICENCE.md` incluido en el repositorio.

Las licencias de las librerías de terceros utilizadas en el proyecto se encuentran en la carpeta `Licences/`.