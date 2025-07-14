#include "Editor.h" // Incluye la declaración de la clase Editor

// --- INCLUSIONES NECESARIAS ---
// Estas inclusiones son cruciales para que el código compile,
// ya que Editor.cpp utiliza clases y funciones de estos módulos.
// Se asume que las rutas de inclusión de CMake ya apuntan a las carpetas correctas (ej. engine/core).
// Por lo tanto, los includes deben ser directos al nombre del archivo dentro de esas carpetas.
#include "Engine.h"    // Asumiendo que Engine.h está en una ruta de inclusión como 'engine/core'
#include "GUI.h"            // Asumiendo que GUI.h está en una ruta de inclusión como 'editor/include'
#include "Window.h"    // Asumiendo que Window.h está en una ruta de inclusión como 'engine/core'
#include <GLFW/glfw3.h>     // Necesario para glfwWindowShouldClose, glfwSwapBuffers, glfwPollEvents
#include "Renderer.h" // Asumiendo que Renderer.h está en una ruta de inclusión como 'engine/rendering'
#include <memory>           // Necesario para std::unique_ptr y std::make_unique

// --- INICIALIZACIÓN DE MIEMBROS ESTÁTICOS ---
// Estos punteros estáticos ahora son std::unique_ptr y se inicializan a nullptr.
// std::unique_ptr se encarga automáticamente de la gestión de memoria.
std::unique_ptr<Editor> Editor::s_Instance = nullptr;
std::unique_ptr<Engine> Editor::s_Engine = nullptr;

// --- CONSTRUCTOR ---
Editor::Editor()
{
    // El constructor ahora solo registra un mensaje de log.
    // La asignación de s_Instance se maneja en Init() para el patrón Singleton.
}

// --- DESTRUCTOR ---
Editor::~Editor()
{
    // Llama a Shutdown() para asegurar la liberación de recursos.
    Shutdown();
}

// --- OBTENER INSTANCIA DEL EDITOR (SINGLETON) ---
Editor* Editor::GetInstance()
{
    // Devuelve el puntero crudo (raw pointer) de la instancia de Editor.
    // Es seguro usar .get() aquí porque no transferimos la propiedad.
    return s_Instance.get();
}

// --- OBTENER INSTANCIA DEL MOTOR ---
Engine* Editor::GetEngine()
{
    // Devuelve el puntero crudo (raw pointer) de la instancia de Engine.
    // Es seguro usar .get() aquí porque no transferimos la propiedad.
    return s_Engine.get();
}

// --- INICIALIZACIÓN DEL EDITOR ---
void Editor::Init()
{
    // Implementación del patrón Singleton para Editor:
    // Asegura que solo se cree una instancia de Editor si aún no existe.
    if (s_Instance == nullptr) {
        s_Instance = std::make_unique<Editor>(); // Crea la única instancia de Editor dinámicamente.
    }

    // Crea una nueva instancia del motor usando std::make_unique.
    // La inicialización del Engine se asume que ocurre en su constructor.
    s_Engine = std::make_unique<Engine>();

    // Inicializa la GUI.
    // GUI::Init() devuelve 'void', por lo que no se puede usar en una condición 'if (!...)'.
    // Se pasa la ventana nativa de GLFW obtenida del motor para que la GUI se adjunte a ella.
    GUI::Init(s_Engine->GetWindow()->GetNativeWindow());

    // Carga las configuraciones guardadas de la GUI (ej. desde imgui.ini).
    GUI::LoadConfigs();

    // Inicia el bucle principal del editor.
    Run();
}

// --- BUCLE PRINCIPAL DEL EDITOR ---
void Editor::Run()
{
    // Cachea el puntero a la ventana para evitar llamadas repetitivas a s_Engine->GetWindow()
    // dentro del bucle principal.
    Window* editorWindow = s_Engine->GetWindow();
    if (!editorWindow) {
        // En un entorno real, aquí se debería manejar este error críticamente.
        return; // Sale si la ventana no es válida.
    }

    // Bucle principal del editor. Se ejecuta continuamente mientras la ventana de GLFW no reciba una señal de cierre.
    // Se usa glfwWindowShouldClose directamente ya que Window::ShouldClose() no está declarado.
    while (!glfwWindowShouldClose(editorWindow->GetNativeWindow())) {
        // Ejecuta la parte de actualización de la GUI (ej. manejo de eventos de ImGui, preparación de nuevos frames).
        GUI::Run();

        // Llama al método estático Run() del Engine.
        // Asumimos que Engine::Run() es el que se encarga de llamar a Renderer::Render()
        // para dibujar la escena del motor.
        Engine::Run(); // Llamada al método estático Run() del Engine.

        // La GUI renderiza sus propios elementos (paneles, botones, etc.)
        // y luego incrusta el contenido del FrameBuffer de la escena 3D
        // (obtenido de Renderer::GetData().m_FBO) dentro de uno de sus paneles (ej. ShowScene).
        GUI::Render(*Renderer::GetData().m_FBO); // Asegúrate de que Renderer::GetData().m_FBO es accesible y válido.

        // Intercambia los buffers de la ventana para mostrar el frame renderizado en pantalla.
        // Se usa glfwSwapBuffers directamente ya que Window::SwapBuffers() no está declarado.
        glfwSwapBuffers(editorWindow->GetNativeWindow());

        // Procesa los eventos de entrada (teclado, ratón) y del sistema operativo.
        // Se usa glfwPollEvents directamente ya que Window::PollEvents() no está declarado.
        glfwPollEvents();
    }
}

// --- APAGADO DEL EDITOR ---
void Editor::Shutdown()
{
    // Asegura que la GUI se apague primero. Es importante que la GUI libere sus recursos
    // mientras el contexto de la ventana y el motor aún están activos.
    if (s_Instance && s_Engine) { // Solo apagar si las instancias existen
        GUI::Shutdown();

        // Apaga el motor, liberando todos sus recursos (OpenGL, etc.).
        s_Engine->Shutdown();

        // Con std::unique_ptr, no necesitamos llamar a 'delete'.
        // Los destructores de s_Engine y s_Instance se llamarán automáticamente
        // cuando los unique_ptr salgan del ámbito o cuando el programa finalice.
        // Simplemente establecemos los punteros a nullptr para claridad y para indicar
        // que los objetos ya no son válidos, aunque unique_ptr ya se encarga de la liberación.
        s_Engine = nullptr;
        s_Instance = nullptr;
    } else {
        // En un entorno real, aquí se podría registrar una advertencia.
    }
}
