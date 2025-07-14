#include "Renderer.h"
#include "Engine.h" // Necesario para Engine::Get() y sus métodos

// Inicialización de los datos estáticos del renderizador
RendererData Renderer::s_Data;

// Inicialización de variables estáticas para el control del tiempo y el ratón
float Renderer::s_DeltaTime = 0.0f;
float Renderer::s_LastFrame = 0.0f;

bool Renderer::s_FirstMouse = true;
float Renderer::s_LastX;
float Renderer::s_LastY;

// Constructor por defecto
Renderer::Renderer() = default;

// Destructor
Renderer::~Renderer()
{
    // Asegura que los recursos se liberen al destruir el renderizador
    Shutdown();
}

// Inicialización del subsistema de renderizado
void Renderer::Init()
{
    // Carga las funciones de OpenGL usando GLAD
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return; // Salir si GLAD no se inicializa
    }

    // Configura las variables iniciales del renderizador
    SetVariables();
    // Carga los shaders
    LoadShaders();
    // Configura los búferes de vértices e índices
    SetupBuffers();
    // Establece los callbacks de GLFW para eventos de ventana y entrada
    SetCallbacks();
}

// Configura las variables de datos iniciales del renderizador
void Renderer::SetVariables()
{
    // Crea nuevas instancias de los componentes de la escena y la cámara
    s_Data.m_Scene = new Scene();
    s_Data.m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f)); // Cámara en posición inicial
    s_Data.m_Cube = new Cube("Cube"); // Crea un objeto cubo
    s_Data.m_ClearColor = new glm::vec3(0.0f, 0.1f, 0.2f); // Color de fondo inicial

    // Añade el cubo a la escena
    s_Data.m_Scene->AddCube(std::shared_ptr<Cube>(s_Data.m_Cube));
}

// Devuelve una referencia a la estructura de datos del renderizador
RendererData& Renderer::GetData()
{
    return s_Data;
}

// Carga los shaders desde archivos
void Renderer::LoadShaders()
{
    s_Data.m_Shader = new Shader(ENGINE_RESOURCES_PATH"shaders/vertex.glsl", ENGINE_RESOURCES_PATH"shaders/fragment.glsl");
}

// Configura los búferes de vértices, índices y el FrameBuffer
void Renderer::SetupBuffers()
{
    // Crea instancias de VertexArray, VertexBuffer e IndexBuffer
    s_Data.m_VAO = new VertexArray();
    s_Data.m_VBO = new VertexBuffer();
    s_Data.m_IBO = new IndexBuffer();

    s_Data.m_VAO->Bind(); // Vincula el VAO

    // Establece los datos de vértices e índices para el cubo
    s_Data.m_VBO->SetData(sizeof(float) * Cube::GetVertices().size(), Cube::GetVertices().data());
    s_Data.m_IBO->SetData(sizeof(unsigned int) * Cube::GetIndices().size(), Cube::GetIndices().data());

    // Configura los atributos de los vértices (posiciones y colores)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), reinterpret_cast<void*>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Desvincula los búferes después de la configuración
    VertexArray::Unbind();
    VertexBuffer::Unbind();
    IndexBuffer::Unbind();

    // Configura el FrameBuffer
    s_Data.m_FBO = new FrameBuffer();
    // Obtiene el tamaño actual de la ventana para adjuntar la textura al FBO
    WindowSize windowSize = Engine::Get()->GetWindow()->GetSize();
    s_Data.m_FBO->AttachTexture(windowSize.Width, windowSize.Height);
    FrameBuffer::Unbind(); // Desvincula el FBO
}

// Establece los callbacks de GLFW para eventos de ventana y entrada
void Renderer::SetCallbacks()
{
    // Callback para redimensionamiento de ventana: reconfigura los búferes
    glfwSetWindowSizeCallback(Engine::Get()->GetWindow()->GetNativeWindow(), [](GLFWwindow* window, int width, int height)
    {
        SetupBuffers(); // Vuelve a configurar los búferes (incluyendo FBO) al cambiar el tamaño de la ventana
    });
    // Callback para redimensionamiento del framebuffer: ajusta el viewport de OpenGL
    glfwSetFramebufferSizeCallback(Engine::Get()->GetWindow()->GetNativeWindow(), [](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height); // Ajusta el viewport de OpenGL
    });
    // Callback para el scroll del ratón: procesa el zoom de la cámara
    glfwSetScrollCallback(Engine::Get()->GetWindow()->GetNativeWindow(), [](GLFWwindow* window, double xOffset, double yOffset)
    {
        s_Data.m_Camera->ProcessMouseScroll(static_cast<float>(yOffset));
    });
}

// Función principal de renderizado (dibujo de la escena)
void Renderer::Render() {
    // Calcula el delta time para movimientos dependientes del tiempo
    auto currentFrame = static_cast<float>(glfwGetTime());
    s_DeltaTime = currentFrame - s_LastFrame;
    s_LastFrame = currentFrame;

    // Vincula el FrameBuffer para que todo el dibujo se realice en él
    s_Data.m_FBO->Bind();

    // ELIMINADO: glfwPollEvents(); // Esto debe ser llamado una sola vez en el bucle principal de la aplicación (Editor::Run())
    ProcessInput(Engine::Get()->GetWindow()->GetNativeWindow()); // Procesa la entrada del ratón y teclado

    glEnable(GL_DEPTH_TEST); // Habilita la prueba de profundidad para objetos 3D

    // Establece el color de fondo y limpia los búferes de color y profundidad
    glClearColor(s_Data.m_ClearColor->x, s_Data.m_ClearColor->y, s_Data.m_ClearColor->z, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    s_Data.m_Shader->Use(); // Activa el shader para el dibujo

    s_Data.m_Cube->Draw(); // Dibuja el cubo (puede ser una llamada a glDrawElements/Arrays interna)

    // Obtiene las matrices de modelo, vista y proyección
    const glm::mat4& model = *s_Data.m_Cube->GetModelMatrix();
    const glm::mat4& view = s_Data.m_Camera->GetViewMatrix();

    // Calcula la matriz de proyección (perspectiva)
    WindowSize size = Engine::Get()->GetWindow()->GetSize();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(size.Width) / static_cast<float>(size.Height), 0.1f, 100.0f);

    // Pasa las matrices y el color al shader como uniformes
    glUniformMatrix4fv(s_Data.m_Shader->GetUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(s_Data.m_Shader->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(s_Data.m_Shader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform3fv(s_Data.m_Shader->GetUniformLocation("color"), 1, glm::value_ptr(*s_Data.m_Cube->GetShaderColor()));

    // Restablece la matriz del modelo del cubo (si es necesario, esto puede variar según la lógica del motor)
    *s_Data.m_Cube->GetModelMatrix() = glm::mat4(1.0f);

    s_Data.m_VAO->Bind(); // Vincula el VAO antes de dibujar

    // Dibuja los elementos indexados (el cubo)
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr);

    // Desvincula el FrameBuffer y vuelve al framebuffer por defecto (la pantalla)
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // ELIMINADO: glfwSwapBuffers(Engine::Get()->GetWindow()->GetNativeWindow()); // Esto debe ser llamado una sola vez en el bucle principal de la aplicación (Editor::Run())
}

// Procesa la entrada del usuario (ratón y teclado) para controlar la cámara
void Renderer::ProcessInput(GLFWwindow *window)
{
    if(Input::IsMouseButtonPressed(ButtonRight)) // Si el botón derecho del ratón está presionado
    {
        auto mousePos = Input::GetMousePosition(); // Obtiene la posición actual del ratón

        if(s_FirstMouse) // Si es la primera vez que se detecta el ratón
        {
            s_LastX = mousePos.x; // Guarda la posición inicial del ratón
            s_LastY = mousePos.y;
            s_FirstMouse = false; // Ya no es la primera vez
        }

        // Calcula el desplazamiento del ratón
        float xOffset = mousePos.x - s_LastX;
        float yOffset = s_LastY - mousePos.y; // Invertido porque las coordenadas Y de la pantalla aumentan hacia abajo

        // Actualiza la última posición del ratón
        s_LastX = mousePos.x;
        s_LastY = mousePos.y;

        // Procesa el movimiento del ratón para la cámara
        s_Data.m_Camera->ProcessMouseMovement(xOffset, yOffset);

        // Deshabilita el cursor de GLFW para una experiencia de cámara en primera persona
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else // Si el botón derecho del ratón no está presionado
    {
        // Habilita el cursor de GLFW
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    // Procesa la entrada del teclado para mover la cámara
    if (Input::IsKeyPressed(W))
        s_Data.m_Camera->ProcessKeyboard(FORWARD, s_DeltaTime);
    if (Input::IsKeyPressed(S))
        s_Data.m_Camera->ProcessKeyboard(BACKWARD, s_DeltaTime);
    if (Input::IsKeyPressed(A))
        s_Data.m_Camera->ProcessKeyboard(LEFT, s_DeltaTime);
    if (Input::IsKeyPressed(D))
        s_Data.m_Camera->ProcessKeyboard(RIGHT, s_DeltaTime);
    if (Input::IsKeyPressed(Q))
        s_Data.m_Camera->ProcessKeyboard(DOWN, s_DeltaTime);
    if (Input::IsKeyPressed(E))
        s_Data.m_Camera->ProcessKeyboard(UP, s_DeltaTime);
}

// Apagado y liberación de recursos del renderizador
void Renderer::Shutdown()
{
    // Libera la memoria de los búferes y shaders
    s_Data.m_VAO->Shutdown();
    s_Data.m_VBO->Shutdown();
    s_Data.m_FBO->Shutdown();
    s_Data.m_IBO->Shutdown();
    s_Data.m_Shader->Shutdown();

    // ¡IMPORTANTE! Liberar la memoria asignada con 'new' para los punteros en RendererData
    // Esto es crucial para evitar fugas de memoria.
    delete s_Data.m_Scene;
    s_Data.m_Scene = nullptr;
    delete s_Data.m_Camera;
    s_Data.m_Camera = nullptr;
    delete s_Data.m_Cube;
    s_Data.m_Cube = nullptr;
    delete s_Data.m_ClearColor;
    s_Data.m_ClearColor = nullptr;
}
