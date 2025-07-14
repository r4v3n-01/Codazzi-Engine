#include "Engine.h"

// --- INCLUSIONES NECESARIAS ---
// Incluye la cabecera de Renderer para las llamadas a Renderer::Init() y Renderer::Shutdown().
#include "Renderer.h"
// Incluye la cabecera de Window para la creación de la instancia de Window.
#include "Window.h"
// Incluye iostream para mensajes de depuración si se desea, aunque no se usará LOG_INFO/ERROR aquí.
#include <iostream>

// Inicialización del puntero estático a la instancia del motor.
// Esto es parte del patrón Singleton.
Engine* Engine::s_Instance = nullptr;

// --- CONSTRUCTOR ---
Engine::Engine()
{
    // Asigna la instancia actual al puntero estático, haciendo de esta la única instancia del motor.
    s_Instance = this;

    // Crea una nueva instancia de la ventana dinámicamente.
    // Esta ventana será gestionada por el motor.
    m_Window = new Window();

    // Inicializa el subsistema de renderizado.
    // Se asume que Renderer::Init() configura OpenGL y otros aspectos de renderizado.
    Renderer::Init();
}

// --- DESTRUCTOR ---
Engine::~Engine()
{
    // Llama al método Shutdown() para asegurar que todos los recursos sean liberados
    // cuando la instancia del motor sea destruida.
    Shutdown();
}

// --- MÉTODO RUN (ESTÁTICO) ---
// Este método es el bucle principal del motor, según la declaración en Engine.h.
// Actualmente, solo llama a Renderer::Render().
// Si el motor tiene una lógica de actualización interna (física, IA, etc.),
// debería ir aquí o en un método Update() llamado desde aquí.
void Engine::Run()
{
    // Llama a la función de renderizado del subsistema Renderer.
    // Esto es lo que dibuja la escena 3D.
    Renderer::Render();
}

// --- MÉTODO RENDER (NO ESTÁTICO) ---
// Esta es la implementación de la función Engine::Render() que faltaba y causaba el error de enlazado.
// Su propósito es encapsular la lógica de renderizado del motor.
// Actualmente, no hace nada, pero su existencia es crucial para la compilación.
void Engine::Render()
{
    // Aquí iría la lógica específica de renderizado del motor,
    // como preparar la escena, actualizar cámaras, etc.,
    // antes de que Renderer::Render() haga el trabajo final.
    // Por ahora, lo dejamos vacío para solucionar el error de enlazado.
}

// --- MÉTODO SHUTDOWN (ESTÁTICO) ---
// Este método se encarga de liberar todos los recursos asignados por el motor.
// Como es estático, no puede acceder directamente a m_Window.
// Debe acceder a m_Window a través de la instancia Singleton (Engine::Get()).
void Engine::Shutdown()
{
    // Apaga el subsistema de renderizado, liberando sus recursos (ej. VBOs, Shaders, Texturas).
    Renderer::Shutdown();

    // Libera la memoria asignada dinámicamente para la ventana.
    // Accede a m_Window a través de la instancia Singleton.
    // Es crucial llamar a 'delete' para evitar fugas de memoria.
    if (Engine::Get() && Engine::Get()->m_Window) { // Verifica que la instancia y el puntero no sean nullptr.
        delete Engine::Get()->m_Window;
        Engine::Get()->m_Window = nullptr; // Establece el puntero a nullptr después de liberar la memoria.
    }
}


