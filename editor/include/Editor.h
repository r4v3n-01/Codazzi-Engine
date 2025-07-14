#pragma once

#define GLFW_INCLUDE_NONE
#include "Engine.h"
#include "GUI.h"
#include <memory> // Necesario para std::unique_ptr

class Editor
{
public:
    // Constructor y Destructor
    Editor();
    ~Editor();

    // Métodos estáticos para el ciclo de vida del editor
    static void Init();
    static void Run();
    static void Shutdown();

    // Métodos estáticos para obtener las instancias (Singleton-like)
    // Ahora devuelven punteros crudos (raw pointers) para acceder a los objetos
    // gestionados por std::unique_ptr, sin transferir la propiedad.
    static Editor* GetInstance();
    static Engine* GetEngine();

private:
    // Miembros estáticos que ahora usan std::unique_ptr para una gestión
    // de memoria segura y automática. Esto elimina la necesidad de llamar
    // a 'delete' manualmente en el Shutdown().
    static std::unique_ptr<Editor> s_Instance;
    static std::unique_ptr<Engine> s_Engine;
};
