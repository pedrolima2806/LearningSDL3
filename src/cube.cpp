#include <cmath>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>

constexpr float PI = 3.14159265358979323846f;

void setPerspective(float fovYDeg, float aspect, float zNear, float zFar) {
    float f = 1.0f / std::tan((fovYDeg * 0.5f) * PI / 180.0f);

    float m[16] = {
        f / aspect, 0.0f, 0.0f,  0.0f,
        0.0f,       f,    0.0f,  0.0f,
        0.0f,       0.0f, (zFar + zNear) / (zNear - zFar), -1.0f,
        0.0f,       0.0f, (2.0f * zFar * zNear) / (zNear - zFar), 0.0f
    };

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(m);
}

void drawCube() {
    glBegin(GL_QUADS);

    // Frente
    glColor3f(1.0f, 0.2f, 0.2f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);

    // Trás
    glColor3f(0.2f, 1.0f, 0.2f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);

    // Topo
    glColor3f(0.2f, 0.4f, 1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);

    // Base
    glColor3f(1.0f, 1.0f, 0.2f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);

    // Direita
    glColor3f(1.0f, 0.2f, 1.0f);
    glVertex3f( 1.0f, -1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f, -1.0f);
    glVertex3f( 1.0f,  1.0f,  1.0f);
    glVertex3f( 1.0f, -1.0f,  1.0f);

    // Esquerda
    glColor3f(0.2f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f,  1.0f);
    glVertex3f(-1.0f,  1.0f, -1.0f);

    glEnd();
}

int main() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "Erro no SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        "SDL3 OpenGL Cube",
        800,
        600,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        std::cerr << "Erro ao criar janela: " << SDL_GetError() << '\n';
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
        std::cerr << "Erro ao criar contexto OpenGL: " << SDL_GetError() << '\n';
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    if (!SDL_GL_MakeCurrent(window, glContext)) {
        std::cerr << "Erro ao ativar contexto OpenGL: " << SDL_GetError() << '\n';
        SDL_GL_DestroyContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);
    glEnable(GL_DEPTH_TEST);

    bool running = true;
    SDL_Event event;
    float angle = 0.0f;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE) {
                running = false;
            }
        }

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        if (h == 0) h = 1;

        glViewport(0, 0, w, h);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        setPerspective(60.0f, static_cast<float>(w) / static_cast<float>(h), 1.0f, 100.0f);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, -6.0f);
        glRotatef(angle, 1.0f, 1.0f, 0.0f);

        drawCube();

        SDL_GL_SwapWindow(window);
        angle += 0.6f;
    }

    SDL_GL_DestroyContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}