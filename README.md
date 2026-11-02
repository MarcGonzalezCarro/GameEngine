# \# SuperEngine

# 

# \## 1. Descripción del proyecto

# 

# SuperEngine es un motor gráfico desarrollado en C++ que permite cargar, visualizar y gestionar geometrías 3D mediante un editor propio.

# 

# El motor utiliza SDL3 para la gestión de ventanas y entrada, OpenGL para el renderizado, Dear ImGui para la interfaz del editor, Assimp para la importación de modelos 3D y DevIL para la carga de imágenes.

# 

# El proyecto está organizado separando las funcionalidades propias del motor de las herramientas del editor, con el objetivo de mantener una arquitectura modular y facilitar futuras ampliaciones.

# 

# \*\*Repositorio:\*\* https://github.com/MarcGonzalezCarro/GameEngine

# 

# \### Integrantes del equipo

# 

# \- Marc González — https://github.com/MarcGonzalezCarro

# \- Irene Calatrava — https://github.com/irentren

# \- Lluís Cazorla — https://github.com/LluisCazorla

# 

# \---

# 

# \## 2. Cómo utilizar el motor

# 

# \### 2.1. Carga de modelos y texturas

# 

# Al iniciar el programa, el modelo `Baker\_house` se carga automáticamente.

# 

# También es posible incorporar recursos mediante Drag \& Drop:

# 

# \- \*\*Modelos FBX:\*\* arrastrar un archivo `.fbx` desde cualquier ubicación del ordenador a la ventana del programa.

# \- \*\*Texturas:\*\* arrastrar archivos `.dds` o `.png` para aplicarlos a los GameObjects seleccionados.

# 

# Cada malla importada genera su correspondiente GameObject dentro de la jerarquía de la escena.

# 

# \### 2.2. Controles de cámara

# 

# La cámara dispone de controles de navegación similares a los de Unity:

# 

# | Acción | Control |

# |---|---|

# | Activar movimiento y rotación de cámara | Mantener botón derecho del ratón |

# | Mover hacia delante | W |

# | Mover hacia atrás | S |

# | Mover a la izquierda | A |

# | Mover a la derecha | D |

# | Aumentar velocidad | Mantener SHIFT |

# | Zoom | Rueda del ratón |

# | Orbitar alrededor del objeto | ALT + botón izquierdo |

# | Enfocar geometría seleccionada | F |

# 

# \### 2.3. Ventanas del editor

# 

# El editor dispone de varias ventanas accesibles desde el menú \*\*View\*\*:

# 

# \- \*\*Scene:\*\* muestra la escena 3D y los modelos cargados.

# \- \*\*Console:\*\* muestra información del funcionamiento del motor, carga de modelos y texturas, inicialización de librerías y posibles errores.

# \- \*\*Configuration:\*\* permite consultar los FPS, configurar diferentes parámetros del motor y visualizar información del hardware y del software.

# \- \*\*Hierarchy:\*\* muestra todos los GameObjects de la escena y permite seleccionar uno.

# \- \*\*Inspector:\*\* muestra información de los componentes del GameObject seleccionado.

# 

# \### 2.4. Inspector

# 

# El Inspector permite consultar los componentes del GameObject seleccionado:

# 

# \- \*\*Transform:\*\* muestra la posición, rotación y escala.

# \- \*\*Mesh:\*\* muestra información de la geometría y permite visualizar sus normales, si está habilitada esa funcionalidad.

# \- \*\*Texture:\*\* muestra información de la textura asignada, incluyendo su ruta y dimensiones.

# 

# \### 2.5. Menú principal

# 

# \- \*\*File:\*\* permite salir del programa.

# \- \*\*View:\*\* permite mostrar u ocultar las ventanas del editor.

# \- \*\*Help:\*\*

# &#x20; - Documentation: abre la página del proyecto en GitHub.

# &#x20; - Report a bug: abre la sección de incidencias.

# &#x20; - Download latest: abre la sección de releases.

# &#x20; - About: muestra información del motor, versión, integrantes, librerías utilizadas y licencia.

# 

# También se dispone de un menú para crear y cargar geometrías básicas.

# 

# \---

# 

# \## 3. Funcionalidades extra

# 

# Además de las funcionalidades solicitadas, el motor incorpora o contempla las siguientes características adicionales:

# 

# \### Transform conectado a la escena

# 

# El componente Transform está conectado directamente con el sistema de transformaciones de los objetos. Cuando se modifica la posición, rotación o escala de un GameObject, el cambio se refleja en la geometría renderizada.

# 

# Esto permite visualizar los cambios de transformación directamente en la escena.

# 

# \### Jerarquía de GameObjects

# 

# Los GameObjects pueden organizarse mediante una estructura padre-hijo. Así como ser renombrados, duplicados y borrados





# \### Normales Custom

# 

# En configuración se pueden modificar los visuales de las normales tanto color, como tamaño.





# \### Empty GameObject

# 

# Se pueden crear empty gameobjects desde el botón de aparece en la hierarchy. Solo se crean con un transform funcional.



# \---

# 

# \## 4. Comentarios adicionales para el profesor

# 

# \- Los controles de la cámara solamente funcionan cuando la ventana de Scene tiene el foco (\*focus\*). Es decir, es necesario tener seleccionada esa ventana para que las entradas de teclado y ratón controlen la cámara. Esto evita que la cámara interfiera con la interacción con otras ventanas del editor, como Hierarchy, Inspector o Configuration.

# \- Al hacer doble click en un gameObject aparece una pequeña caja donde poder escribir. Esta un pelin pegada con la sección del nombre del gameobject así que tienes que tener un poco de precisión.

# \- Para duplicar o eliminar gameObject hay dos botones en la parte más alta del inspector.

# \---

# 

# \## 5. Librerías utilizadas

# 

# \- \*\*SDL3:\*\* gestión de ventanas, eventos y entrada.

# \- \*\*OpenGL:\*\* renderizado gráfico.

# \- \*\*GLM:\*\* operaciones matemáticas y transformaciones 3D.

# \- \*\*Dear ImGui:\*\* interfaz gráfica del editor.

# \- \*\*Assimp:\*\* importación de modelos 3D.

# \- \*\*DevIL:\*\* carga y gestión de imágenes y texturas.

# 

# \## 6. Licencia

# 

# Este proyecto se distribuye bajo la licencia MIT.

# 

# Consultar el archivo `LICENSE` incluido en el repositorio para conocer las condiciones de uso y distribución.

