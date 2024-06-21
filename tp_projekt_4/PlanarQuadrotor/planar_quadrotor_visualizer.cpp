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
    
    int Dron_L = -80;
    int Dron_P = 80;  
    int Dron_G = -10; 
    int Dron_D = 10;  

    int x1 = Dron_L; //koordynaty x, y na plaszycznie
    int x2 = Dron_P;  
    int y1 = Dron_G;
    int y2 = Dron_D;

    SDL_Point quadrotor_rogi[4] = {
        {x1, y2}, //lewy dolny
        {x2, y2}, //prawy dolny
        {x2, y1}, //prawy gorny
        {x1, y1} //lewy gorny
    };

    for (int i = 0; i < 4; ++i) {
        float x_buffor = quadrotor_rogi[i].x * cos(q_theta) - quadrotor_rogi[i].y * sin(q_theta);
        float y_buffor = quadrotor_rogi[i].x * sin(q_theta) + quadrotor_rogi[i].y * cos(q_theta);
        quadrotor_rogi[i].x = static_cast<int>(x_buffor) + srodek_x;
        quadrotor_rogi[i].y = static_cast<int>(y_buffor) + srodek_y;
    }

    Sint16 wsp_x[4] = { quadrotor_rogi[0].x,quadrotor_rogi[1].x, quadrotor_rogi[2].x, quadrotor_rogi[3].x };
    Sint16 wsp_y[4] = { quadrotor_rogi[0].y,quadrotor_rogi[1].y, quadrotor_rogi[2].y, quadrotor_rogi[3].y };

    filledPolygonColor(gRenderer.get(),wsp_x,wsp_y,4, 0xFFAAB220); //kolor ciala aabbggrr
    SDL_SetRenderDrawColor(gRenderer.get(), 0x80, 0x80, 0x80, 0xFF); //obrys kolor rgba
    SDL_RenderDrawLines(gRenderer.get(), quadrotor_rogi, 4);
    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x, quadrotor_rogi[3].y, quadrotor_rogi[0].x, quadrotor_rogi[0].y);

    //rogi:
    // 3 - lewy gorny; 2 - prawy gorny

    SDL_SetRenderDrawColor(gRenderer.get(), 0x80, 0x80, 0x80, 0xFF); //kolor smigiel i drazka

    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x, quadrotor_rogi[3].y - 20, quadrotor_rogi[3].x, quadrotor_rogi[3].y + 1); //lewe smiglo drazek
    SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x, quadrotor_rogi[2].y - 20, quadrotor_rogi[2].x, quadrotor_rogi[2].y + 1); //prawe smiglo drazek

    if(animacja % 2 == 0){
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y + 0, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y - 40);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y - 40, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y + 0);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y + 1, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y - 39);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y - 39, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y + 1);

        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y - 40, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y + 0);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y + 0, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y - 40);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y - 39, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y + 1);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y + 1, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y - 39);
    }

    if (animacja % 2 == 1){
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y - 20, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y - 20);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x - 20, quadrotor_rogi[3].y - 21, quadrotor_rogi[3].x + 20, quadrotor_rogi[3].y - 21);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x, quadrotor_rogi[3].y, quadrotor_rogi[3].x, quadrotor_rogi[3].y - 40);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[3].x, quadrotor_rogi[3].y - 1, quadrotor_rogi[3].x, quadrotor_rogi[3].y - 41);

        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y - 20, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y - 20);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x - 20, quadrotor_rogi[2].y - 21, quadrotor_rogi[2].x + 20, quadrotor_rogi[2].y - 21);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x, quadrotor_rogi[2].y, quadrotor_rogi[2].x, quadrotor_rogi[2].y - 40);
        SDL_RenderDrawLine(gRenderer.get(), quadrotor_rogi[2].x, quadrotor_rogi[2].y - 1, quadrotor_rogi[2].x, quadrotor_rogi[2].y - 41);
    }


    SDL_RenderPresent(gRenderer.get());
    animacja++;
}