#include "planar_quadrotor_visualizer.h"
#include <cmath>

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

    int srodek_x = static_cast<int>(q_x);
    int srodek_y = static_cast<int>(q_y);

    static int animacja = 0;
    if (animacja == 100) {
        animacja = 0;
    }
    
    int Dron_L = -50;
    int Dron_P = 50;  
    int Dron_G = -20; 
    int Dron_D = 20;  

    int x11 = Dron_L; //koordynaty x, y na plaszycznie
    int x21 = Dron_P;  
    int y11 = Dron_G;
    int y21 = Dron_D;

    SDL_Point quadrotor_rogi[4] = {
        {x11, y21}, //lewy dolny
        {x21, y21}, //prawy dolny
        {x21, y11}, //prawy gorny
        {x11, y11} //lewy gorny
    };

    for (int i = 0; i < 4; ++i) {
        float x_buffor = quadrotor_rogi[i].x * cos(q_theta) - quadrotor_rogi[i].y * sin(q_theta);
        float y_buffor = quadrotor_rogi[i].x * sin(q_theta) + quadrotor_rogi[i].y * cos(q_theta);
        quadrotor_rogi[i].x = static_cast<int>(x_buffor) + srodek_x;
        quadrotor_rogi[i].y = static_cast<int>(y_buffor) + srodek_y;
    }

    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0x00, 0x00, 0xFF);

    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[0].x, quadrotor_rogi[0].y, quadrotor_rogi[1].x, quadrotor_rogi[1].y);
    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[1].x, quadrotor_rogi[1].y, quadrotor_rogi[2].x, quadrotor_rogi[2].y);
    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x, quadrotor_rogi[2].y, quadrotor_rogi[3].x, quadrotor_rogi[3].y);
    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x, quadrotor_rogi[3].y, quadrotor_rogi[0].x, quadrotor_rogi[0].y);

    SDL_Rect quadrotorKorpus = {
    quadrotorKorpus.x = q_x - 50, // srodek
    quadrotorKorpus.y = q_y - 20, // srodek
    quadrotorKorpus.w = 100, // szerokosc
    quadrotorKorpus.h = 40 // wysokosc
    };

    int Dron_L2 = quadrotorKorpus.x;
    int Dron_P2 = quadrotorKorpus.x + quadrotorKorpus.w;
    int Dron_G2 = quadrotorKorpus.y;
    int Dron_D2 = quadrotorKorpus.y + quadrotorKorpus.h;

    int x1 = Dron_L2; //koordynaty x, y na plaszycznie
    int x2 = Dron_P2;
    int y1 = Dron_G2;
    int y2 = Dron_D2;

    SDL_SetRenderDrawBlendMode(gRenderer.get(), SDL_BLENDMODE_BLEND);

    SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0x00, 0x00, 0x00);

    SDL_RenderFillRect(gRenderer.get(), &quadrotorKorpus);


    SDL_SetRenderDrawColor(gRenderer.get(), 0x80, 0x80, 0x80, 0xFF);

    SDL_RenderDrawLine(gRenderer.get(), x1, y1, x1, y1 - 20);
    SDL_RenderDrawLine(gRenderer.get(), x2, y1, x2, y1 - 20);
    if(animacja % 2 == 0){
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 + 0, x1 + 20, y1 - 40);
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 40, x1 + 20, y1 + 0);
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 + 1, x1 + 20, y1 - 39);
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 39, x1 + 20, y1 + 1);

        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 40, x2 + 20, y1 + 0);
        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 + 0, x2 + 20, y1 - 40);
        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 39, x2 + 20, y1 + 1);
        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 + 1, x2 + 20, y1 - 39); 
    }

    if (animacja % 2 == 1){
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 20, x1 + 20, y1 - 20);
        SDL_RenderDrawLine(gRenderer.get(), x1 - 20, y1 - 21, x1 + 20, y1 - 21);
        SDL_RenderDrawLine(gRenderer.get(), x1, y1, x1, y1 - 40);
        SDL_RenderDrawLine(gRenderer.get(), x1, y1 - 1, x1, y1 - 41);

        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 20, x2 + 20, y1 - 20);
        SDL_RenderDrawLine(gRenderer.get(), x2 - 20, y1 - 21, x2 + 20, y1 - 21);
        SDL_RenderDrawLine(gRenderer.get(), x2, y1, x2, y1 - 40);
        SDL_RenderDrawLine(gRenderer.get(), x2, y1 - 1, x2, y1 - 41);
    }


    SDL_RenderPresent(gRenderer.get());
    animacja++;
}