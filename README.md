# Codazzi Engine 🏔️
Un motor gráfico 3D hecho en C++ y OpenGL, con el objetivo de facilitar la creación de juegos en 3D.

# Descripción
Codazzi Engine es un proyecto personal que hago sin experiencia. Usando C++ y la API gráfica de OpenGL, este proyecto se tomará el reto de construir un motor funcional, y fácil de entender.

Este motor comenzó como un fork de Ferx Engine, un proyecto educativo que proporcionó una base sólida. El objetivo actual es refactorizar, expandir y añadir nuevas funcionalidades sobre esa base para crear una herramienta más robusta y completa.

# Estado Actual
Actualmente, el motor cuenta con las siguientes características básicas:

- Renderizado de una escena simple con un cubo de prueba.

- Una interfaz de usuario para depuración y control creada con ImGui.

- Controles de cámara básicos (Paneo, Zoom y Órbita).

- Capacidad para modificar propiedades en tiempo real, como colores de objetos y del fondo.

# Hoja de Ruta (Roadmap) 🗺️
Los próximos pasos planeados para el desarrollo del motor son:

- Corrección de Bugs: Solucionar los problemas de interacción entre la cámara y la interfaz.

- Arquitectura de Componentes: Implementar un sistema de GameObjects y Components similar al de Unity.

- Carga de Modelos: Añadir la capacidad de cargar y renderizar modelos 3D desde archivos (formato .obj para empezar).

- Sistema de Materiales: Crear un sistema básico para manejar texturas y shaders.

- Integración de Scripting: Incorporar Lua para manejar la lógica de los objetos del juego.

# Cómo Construir el Proyecto
Sigue estos pasos para clonar, preparar y construir el motor gráfico:

### Clonar el Repositorio
Para clonar el repositorio de Codazzi Engine, utiliza el siguiente comando:

```lenguaje
git clone https://github.com/r4v3n-01/Codazzi-Engine.git
```

### Inicializar Submódulos

Después de clonar el repositorio, es crucial actualizar los submódulos para obtener todas las dependencias necesarias:

```lenguaje
git submodule update --init
```

¡Eso es todo! Ahora puedes construir y ejecutar el proyecto. Para hacerlo, navega al directorio scripts y utiliza el comando make o el archivo BuildProject.sh.

## Comandos de Construcción
El proyecto ofrece diferentes comandos para construir y gestionar el motor, tanto a través de Makefile como de un script de shell.

### Usando Makefile
Aquí tienes una lista de los comandos make disponibles:

```make```: Construye el proyecto y lo ejecuta (por defecto: modo debug). make build-debug: Construye el proyecto en modo de depuración (debug).

```make build-release```: Construye el proyecto en modo de lanzamiento (release).

```make run-debug```: Ejecuta el binario construido en modo de depuración.

```make run-release```: Ejecuta el binario construido en modo de lanzamiento.

```make clean-debug```: Limpia los archivos de construcción generados en modo de depuración.

```make clean-release```: Limpia los archivos de construcción generados en modo de lanzamiento.

```make clean-debug-all```: Elimina completamente el directorio de construcción de depuración.

```make clean-release-all```: Elimina completamente el directorio de construcción de lanzamiento.


### Usando BuildProject.sh (Script de Shell)
Aquí tienes una lista de los comandos disponibles con BuildProject.sh:

```./BuildProject.sh all```: Construye el proyecto y lo ejecuta (por defecto: modo debug).

```./BuildProject.sh build-debug```: Construye el proyecto en modo de depuración (debug).

```./BuildProject.sh build-release```: Construye el proyecto en modo de lanzamiento (release).

```./BuildProject.sh run-debug```: Ejecuta el binario construido en modo de depuración.

```./BuildProject.sh run-release```: Ejecuta el binario construido en modo de lanzamiento.

```./BuildProject.sh clean-debug```: Limpia los archivos de construcción generados en modo de depuración.

```./BuildProject.sh clean-release```: Limpia los archivos de construcción generados en modo de lanzamiento.

```./BuildProject.sh clean-debug-all```: Elimina completamente el directorio de construcción de depuración.

```./BuildProject.sh clean-release-all```: Elimina completamente el directorio de construcción de lanzamiento.

# Dependencias
Para poder construir y ejecutar el proyecto, necesitarás tener instaladas las siguientes dependencias en tu sistema:

``git`` ``cmake`` ``make`` ``ninja``

# Agradecimientos
Este proyecto no sería posible sin el increíble trabajo inicial de los creadores del Ferx Engine. Su base educativa ha sido fundamental para acelerar el desarrollo y el aprendizaje.

# Licencia
Este proyecto está distribuido bajo la Licencia MIT. Puedes ver el archivo LICENSE para más detalles.
