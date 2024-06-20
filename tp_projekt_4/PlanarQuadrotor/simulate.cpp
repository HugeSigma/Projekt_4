/**
 * SDL window creation adapted from https://github.com/isJuhn/DoublePendulum
*/
#include "simulate.h"
#include "matplot/matplot.h"
#include "SDL.h"

namespace mp = matplot;

const int base_freq = 44100;
const int chunk_size = 1024;
const int wzmocnienie = 1;

//dzwiek
void dzwiek(Uint8* bufor, double freq_dzwieku, double theta)
{
    double time;
    double current_value;

    for (int i = 0; i < chunk_size; i++)
    {
        time = i / base_freq;
        current_value = wzmocnienie * sin(time* 2 * M_PI * freq_dzwieku);

        bufor[i] = (Uint8)(current_value + wzmocnienie * 2 + theta * 2 * wzmocnienie); //theta
    }
}

Eigen::MatrixXf LQR(PlanarQuadrotor &quadrotor, float dt) {
    /* Calculate LQR gain matrix */
    Eigen::MatrixXf Eye = Eigen::MatrixXf::Identity(6, 6);
    Eigen::MatrixXf A = Eigen::MatrixXf::Zero(6, 6);
    Eigen::MatrixXf A_discrete = Eigen::MatrixXf::Zero(6, 6);
    Eigen::MatrixXf B(6, 2);
    Eigen::MatrixXf B_discrete(6, 2);
    Eigen::MatrixXf Q = Eigen::MatrixXf::Identity(6, 6);
    Eigen::MatrixXf R = Eigen::MatrixXf::Identity(2, 2);
    Eigen::MatrixXf K = Eigen::MatrixXf::Zero(6, 6);
    Eigen::Vector2f input = quadrotor.GravityCompInput();

    Q.diagonal() << 0.001, 0.001, 100, 0.001, 0.001, 2 / 2 / M_PI;
    R.row(0) << 50, 5;
    R.row(1) << 5, 50;

    std::tie(A, B) = quadrotor.Linearize();
    A_discrete = Eye + dt * A;
    B_discrete = dt * B;
    
    return LQR(A_discrete, B_discrete, Q, R);
}

void control(PlanarQuadrotor &quadrotor, const Eigen::MatrixXf &K) {
    Eigen::Vector2f input = quadrotor.GravityCompInput();
    quadrotor.SetInput(input - K * quadrotor.GetControlState());
}

int main(int argc, char* args[])
{
    std::shared_ptr<SDL_Window> gWindow = nullptr;
    std::shared_ptr<SDL_Renderer> gRenderer = nullptr;
    const int SCREEN_WIDTH = 1280;
    const int SCREEN_HEIGHT = 720;

    int mid_width = SCREEN_WIDTH / 2;
    int mid_height = SCREEN_HEIGHT / 2;
    
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        std::cerr << "Blad inicjalizacji SDL: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_AudioSpec specyfikacja;
    specyfikacja.freq = 54100;
    specyfikacja.format = AUDIO_U8;
    specyfikacja.channels = 2;
    specyfikacja.samples = chunk_size;
    specyfikacja.callback = NULL;
    specyfikacja.userdata = NULL;

    double freq_dzwieku = 100;

    SDL_AudioDeviceID urzadzenie_dzwiekowe = SDL_OpenAudioDevice(NULL, 0, &specyfikacja, NULL, 0);
    if (urzadzenie_dzwiekowe == 0) {
        std::cerr << "Blad urzadzenia audio: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    /**
     * TODO: Extend simulation
     * 1. Set goal state of the mouse when clicking left mouse button (transform the coordinates to the quadrotor world! see visualizer TODO list)
     *    [x, y, 0, 0, 0, 0] - zrobione 7.06
     * 2. Update PlanarQuadrotor from simulation when goal is changed - zrobione 7.06
    */

    Eigen::VectorXf initial_state = Eigen::VectorXf::Zero(6);

    initial_state[0] = mid_width;
    initial_state[1] = mid_height;

    PlanarQuadrotor quadrotor(initial_state);
    PlanarQuadrotorVisualizer quadrotor_visualizer(&quadrotor);
    /**
     * Goal pose for the quadrotor
     * [x, y, theta, x_dot, y_dot, theta_dot]
     * For implemented LQR controller, it has to be [x, y, 0, 0, 0, 0]
    */
    Eigen::VectorXf goal_state = Eigen::VectorXf::Zero(6);
    goal_state << mid_width, mid_height, 0, 0, 0, 0;
    quadrotor.SetGoal(goal_state);

    /* Timestep for the simulation */
    const float dt = 0.1; //odpowiada za czas; 0.005 - ostatecznie; 0.1 - szybkie tempo do test�w
    quadrotor.DoUpdateState(dt);

    Eigen::MatrixXf K = LQR(quadrotor, dt);
    Eigen::Vector2f input = Eigen::Vector2f::Zero(2);

    /**
     * TODO: Plot x, y, theta over time
     * 1. Update x, y, theta history vectors to store trajectory of the quadrotor - zrobione 7.06
     * 2. Plot trajectory using matplot++ when key 'p' is clicked  - zrobione 8.06
    */
    std::vector<float> x_history;
    std::vector<float> y_history;
    std::vector<float> theta_history;
    std::vector<float> time;

    if (init(gWindow, gRenderer, SCREEN_WIDTH, SCREEN_HEIGHT) >= 0)
    {
        SDL_Event e;
        bool quit = false;
        float delay;
        float current_time = 0;
        int x, y;
        const int max_time = 500;


        Eigen::VectorXf state = Eigen::VectorXf::Zero(6);

        while (!quit)
        {
            //events
            while (SDL_PollEvent(&e) != 0)
            {
                if (e.type == SDL_QUIT)
                {
                    quit = true;
                }
                else if (e.type == SDL_MOUSEMOTION)
                {
                    SDL_GetMouseState(&x, &y);
                    std::cout << "Mouse position: (" << x << ", " << y << ")" << std::endl;                    
                }
                else if (e.type == SDL_MOUSEBUTTONDOWN)
                {
                    SDL_GetMouseState(&x, &y);
                    goal_state << x, y, 0, 0, 0, 0;
                    quadrotor.SetGoal(goal_state);
                    quadrotor.DoUpdateState(dt);
                }
                else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_p)
                {
                    mp::tiledlayout(3, 1);
                    auto w1 = mp::nexttile();
                    mp::plot(w1, time, x_history);
                    mp::ylabel(w1, "x");
                    mp::xlabel(w1, "t");

                    auto w2 = mp::nexttile();
                    mp::plot(w2, time, y_history);
                    mp::ylabel(w2, "y");
                    mp::xlabel(w2, "t");

                    auto w3 = mp::nexttile();
                    mp::plot(w3, time, theta_history);
                    mp::ylabel(w3, "theta");
                    mp::xlabel(w3, "t");

                    mp::show();
                }
                
            }

            SDL_Delay((int) dt * 1000);

            SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer.get());

            /* Quadrotor rendering step */
            quadrotor_visualizer.render(gRenderer);

            SDL_RenderPresent(gRenderer.get());

            /* Simulate quadrotor forward in time */
            control(quadrotor, K);
            quadrotor.Update(dt);

            //dodanie pozycji drona do wektor�w
            x_history.push_back(quadrotor.GetState()[0]);
            y_history.push_back(quadrotor.GetState()[1]);
            theta_history.push_back(quadrotor.GetState()[2]);
            time.push_back(current_time);

            if (time.back() >= max_time)
            {
                x_history.erase(x_history.begin());
                y_history.erase(y_history.begin());
                theta_history.erase(theta_history.begin());
                time.erase(time.begin());
            }
            current_time += dt;

            Uint8* bufor_dzwieku = new Uint8[specyfikacja.samples];
            dzwiek(bufor_dzwieku, freq_dzwieku, quadrotor.GetState()[2]);
            SDL_QueueAudio(urzadzenie_dzwiekowe, bufor_dzwieku, specyfikacja.samples);
            SDL_PauseAudioDevice(urzadzenie_dzwiekowe, 0);
            delete[] bufor_dzwieku;
        }
    }
    SDL_CloseAudioDevice(urzadzenie_dzwiekowe);
    SDL_Quit();
    return 0;
}

int init(std::shared_ptr<SDL_Window>& gWindow, std::shared_ptr<SDL_Renderer>& gRenderer, const int SCREEN_WIDTH, const int SCREEN_HEIGHT)
{
    if (SDL_Init(SDL_INIT_VIDEO) >= 0)
    {
        SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
        gWindow = std::shared_ptr<SDL_Window>(SDL_CreateWindow("Planar Quadrotor", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN), SDL_DestroyWindow);
        gRenderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(gWindow.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);
        SDL_SetRenderDrawColor(gRenderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
    }
    else
    {
        std::cout << "SDL_ERROR: " << SDL_GetError() << std::endl;
        return -1;
    }
    return 0;
}
