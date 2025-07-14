#include "Scene.h" // Incluye la declaración de la clase Scene
#include <iostream> // Se incluye por si se desea añadir mensajes de depuración en el destructor

// Constructor por defecto de Scene.
// Se ha eliminado la implementación explícita aquí porque en Scene.h está declarado como '=default;'.
// El compilador generará automáticamente el cuerpo por defecto.
// Scene::Scene()
// {
//     // Opcional: std::cout << "Scene constructor called." << std::endl;
// }

// Destructor explícito de Scene.
// Este destructor es necesario para que el enlazador encuentre su implementación
// cuando se llama a 'delete' en un puntero a Scene.
// Los std::shared_ptr dentro del vector 'cubes' se desreferenciarán automáticamente
// cuando el vector 'cubes' se destruya, liberando la memoria de los Cube si son los últimos propietarios.
Scene::~Scene()
{
    // Opcional: std::cout << "Scene destructor called." << std::endl;
    // No es necesario liberar los cubos aquí, ya que std::shared_ptr los gestiona.
}

// Añade un cubo a la escena.
void Scene::AddCube(const std::shared_ptr<Cube>& cube)
{
    cubes.push_back(cube);
}

// Busca un cubo en la escena por su nombre.
std::shared_ptr<Cube> Scene::GetCubeByName(const std::string& name)
{
    for (const auto& cube : cubes)
    {
        if(cube->name == name)
        {
            return cube;
        }
    }
    return nullptr; // Devuelve nullptr si no se encuentra el cubo
}

// Obtiene una referencia constante al vector de cubos en la escena.
const std::vector<std::shared_ptr<Cube>>& Scene::GetCubes() const
{
    return cubes;
}

