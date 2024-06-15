#include "planar_quadrotor_visualizer.h"

PlanarQuadrotorVisualizer::PlanarQuadrotorVisualizer(PlanarQuadrotor *quadrotor_ptr): quadrotor_ptr(quadrotor_ptr) {}

/**
 * TODO: Improve visualizetion
 * 1. Transform coordinates from quadrotor frame to image frame so the circle is in the middle of the screen - zrobione 7.06
 * 2. Use more shapes to represent quadrotor (e.x. try replicate http://underactuated.mit.edu/acrobot.html#section3 or do something prettier)
 * 3. Animate proppelers (extra points)
 */
void PlanarQuadrotorVisualizer::render(std::shared_ptr<SDL_Renderer> &gRenderer) {
    Eigen::VectorXf state = quadrotor_ptr->GetState();
    float q_x, q_y, q_theta;

    /* x, y, theta coordinates */
    q_x = state[0];
    q_y = state[1];
    q_theta = state[2];

    SDL_Rect quadrotorKorpus = {
    quadrotorKorpus.x = q_x - 50, // srodek
    quadrotorKorpus.y = q_y - 20, // srodek
    quadrotorKorpus.w = 100, // szerokosc
    quadrotorKorpus.h = 40 // wysokosc
    };
    
    int Dron_L = quadrotorKorpus.x;
    int Dron_P = quadrotorKorpus.x + quadrotorKorpus.w;
    int Dron_G = quadrotorKorpus.y;
    int Dron_D = quadrotorKorpus.y + quadrotorKorpus.h;

    int x1 = Dron_L; //koordynaty x, y na plaszycznie
    int x2 = Dron_P;  
    int y1 = Dron_G;
    int y2 = Dron_D;

    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderFillRect(gRenderer.get(), &quadrotorKorpus);

    SDL_SetRenderDrawColor(gRenderer.get(), 0x80, 0x80, 0x80, 0xFF);

    SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 + 20, x1 + 20, y1 - 20);
    SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 20, x1 + 20, y1 + 20);
    SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 + 21, x1 + 20, y1 - 19);
    SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 19, x1 + 20, y1 + 21);

    SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 20, x2 + 20, y1 + 20);
    SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 + 20, x2 + 20, y1 - 20);
    SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 19, x2 + 20, y1 + 21);
    SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 + 21, x2 + 20, y1 - 19);

    SDL_RenderPresent(gRenderer.get());
}