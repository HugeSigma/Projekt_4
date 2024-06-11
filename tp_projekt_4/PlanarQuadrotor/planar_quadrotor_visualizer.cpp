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

    SDL_SetRenderDrawColor(gRenderer.get(), 0x80, 0x80, 0x80, 0xFF);

    SDL_Rect quadrotorKorpus = {
    quadrotorKorpus.x = q_x - 50, // srodek
    quadrotorKorpus.y = q_y - 20, // srodek
    quadrotorKorpus.w = 100, // szerokosc
    quadrotorKorpus.h = 40 // wysokosc
    };
    
    SDL_RenderFillRect(gRenderer.get(), &quadrotorKorpus);

    SDL_RenderPresent(gRenderer.get());
    
}