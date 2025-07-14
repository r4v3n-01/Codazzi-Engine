#include "Window.h" // Incluye la declaración de la clase Window

// Incluye glad para cargar las funciones de OpenGL
#include <glad/glad.h>
// Incluye GLFW para la gestión de ventanas y eventos
#include <GLFW/glfw3.h>
// Incluye string para el manejo de cadenas de texto (título de la ventana)
#include <string>
// Incluye iostream para mensajes de error y depuración
#include <iostream>
// Incluye Renderer.h si Window necesita alguna información del renderizador
// (aunque en este archivo no parece ser directamente usada, se mantiene por si acaso)
#include <Renderer.h>
// Incluye stb_image para cargar iconos de ventana
#define STB_IMAGE_IMPLEMENTATION // ¡IMPORTANTE! Definir esto solo una vez en todo el proyecto
#include <stb_image.h>

// Constructor por defecto, llama a Init()
Window::Window()
{
    Init();
}

// Constructor con título y dimensiones, inicializa m_Data y llama a Init()
Window::Window(const std::string& title, const int width, const int height)
{
    m_Data = WindowData{title, width, height};
    Init();
}

// Destructor, llama a Shutdown() para liberar recursos
Window::~Window()
{
    Shutdown();
}

// Inicializa la ventana de GLFW y el contexto de OpenGL
void Window::Init()
{
    // Intenta inicializar GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return; // Sale si falla la inicialización de GLFW
    }

    std::cout << "GLFW Initialized" << std::endl;

    // Configura hints para la creación de la ventana de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // Versión mayor de OpenGL
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); // Versión menor de OpenGL
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Perfil Core de OpenGL
    glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE); // Habilita el escalado a monitores de alta DPI

    // Configuraciones específicas para Linux (X11)
#if defined(_WIN32)
	// Add Windows-specific configuration if needed
#elif defined(__linux__)
    glfwWindowHintString(GLFW_X11_CLASS_NAME, "ferx"); // Nombre de clase X11
    glfwWindowHintString(GLFW_X11_INSTANCE_NAME, "Ferx"); // Nombre de instancia X11
#endif

    // Crea la ventana de GLFW
    m_Window = glfwCreateWindow(m_Data.Size.Width, m_Data.Size.Height, m_Data.Title.c_str(), nullptr, nullptr);
    if (!m_Window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // Termina GLFW si la ventana no se crea
        return;
    }

    // Obtiene y guarda la escala de contenido de la ventana
    GetScale();

    // Establece el icono de la ventana
    SetWindowIcon();

    // Establece el contexto de OpenGL como el actual para esta ventana
    glfwMakeContextCurrent(m_Window);

    // Habilita VSync (sincronización vertical).
    // Esto limita la tasa de fotogramas a la tasa de refresco del monitor (ej. 60 FPS)
    // y elimina el "tearing" (desgarro de pantalla).
    glfwSwapInterval(1); // 1 para VSync activado, 0 para desactivado
}

// Métodos estáticos Create() que devuelven un objeto Window por valor.
// NOTA DE PULIDO: Considerar devolver unique_ptr si la propiedad es única
// o deshabilitar la copia si no se maneja correctamente.
Window Window::Create()
{
    return Window{};
}

Window Window::Create(const std::string& title, int width, int height)
{
    return Window{title, width, height};
}

// Devuelve el puntero nativo de GLFW a la ventana
GLFWwindow* Window::GetNativeWindow() const
{
    return m_Window;
}

// Devuelve el título de la ventana
const std::string& Window::GetTitle() const
{
    return m_Data.Title;
}

// Obtiene y devuelve el tamaño actual de la ventana
WindowSize Window::GetSize()
{
    glfwGetWindowSize(m_Window, &m_Data.Size.Width, &m_Data.Size.Height);
    return m_Data.Size;
}

// Obtiene y devuelve la escala de contenido de la ventana
WindowScale Window::GetScale()
{
    glfwGetWindowContentScale(m_Window, &m_Data.Scale.X, &m_Data.Scale.Y);
    return m_Data.Scale;
}

// Establece el icono de la ventana
// NOTA DE PULIDO: Este método no debería ser 'const' si modifica el estado de m_Window
// o el recurso externo de la ventana de GLFW.
void Window::SetWindowIcon() const
{
    int width, height, channels;
    // Ruta al archivo del icono (ENGINE_RESOURCES_PATH debe estar definido)
    const char* iconPath = ENGINE_RESOURCES_PATH"icons/icon.png";

    // Carga los píxeles de la imagen del icono, forzando 4 canales (RGBA)
    unsigned char* pixels = stbi_load(iconPath, &width, &height, &channels, 4);
    if (!pixels) {
        std::cerr << "Failed to load icon: " << iconPath << std::endl;
        return; // Sale si falla la carga del icono
    }

    // Crea una estructura GLFWimage y asigna los datos del icono
    GLFWimage images[1];
    images[0].width = width;
    images[0].height = height;
    images[0].pixels = pixels;

    // Establece el icono de la ventana de GLFW
    glfwSetWindowIcon(m_Window, 1, images);

    // Libera la memoria de los píxeles cargados por stb_image
    stbi_image_free(pixels);
}

// Apaga la ventana de GLFW
// NOTA DE PULIDO: Este método no debería ser 'const' ya que destruye la ventana
// y debería poner m_Window a nullptr para evitar punteros colgantes.
void Window::Shutdown() const
{
    if (m_Window) { // Verifica que el puntero no sea nullptr antes de destruir
        glfwDestroyWindow(m_Window);
        // m_Window = nullptr; // No se puede asignar a nullptr si el método es const
    }
}
