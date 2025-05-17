//
// Created by agmui on 12/10/24.
//
#include <cstdio>
#include <cstdlib>

#ifdef SFML_WINDOW
//#include <SDL.h>
#include <SFML/Graphics.hpp>
#endif
#define FPS_CAP 240 //144
#define SCREEN_TICKS_PER_FRAME (1000 / FPS_CAP)  // 16 ms per frame

#include "RayTracer.h"

void setup() {}

int main(int argc, char** argv) {
  //Need at least two arguments (obj input and png output)
  if (argc < 3) {
    printf("Usage %s input.obj output.png\n", argv[0]);
    exit(0);
  }
  RayTracer rayTracer = RayTracer();
//    rayTracer.run(argv[1], argv[2], false);

  //      char obj[] = "../resources/tests/vector_visual.obj";

  //      char obj[] = "../resources/part1/test.obj";

  //  char obj[] = "../resources/part2/test1.obj" ;
  //  char obj[] = "../resources/part2/test2.obj" ;
  //  char obj[] = "../resources/part2/test3.obj" ;
  //  char obj[] = "../resources/part2/test4.obj" ;
  //  char obj[] = "../resources/part2/test5.obj" ;

  //  char obj[] = "../resources/part3/test1.obj" ;
  //  char obj[] = "../resources/part3/test2.obj" ;

  //  char obj[] = "../resources/part4/test1.obj" ;
  //  char obj[] = "../resources/part4/test2.obj" ;

  //  char obj[] = "../resources/part5/test1.obj" ;
  //  char obj[] = "../resources/part5/test2.obj" ;

//          char obj[] = "../resources/scenes/blue_sphere.obj";
//  char obj[] = "../resources/scenes/cornell_box.obj";
//        char obj[] = "../resources/scenes/spheres.obj";

//    char obj[] = "../resources/scenes/bigger/bunny-scene.obj" ;
      char obj[] = "../resources/scenes/bigger/happy-scene.obj" ;

  char outPNG[] = "out.png";

  rayTracer.run(obj, outPNG, false);

#ifdef SFML_WINDOW
  //==
  // create a frame buffer for RESxRES
  Buffer<Color> buffer = Buffer<Color>(RES, RES);

  Loader loader(obj);
  std::shared_ptr<Scene> scene = loader.load(obj);
  //creates a camera object
  Camera camera = loader.loadCamera();
  //    loader.~Loader();  //TODO:

  std::shared_ptr<Material> emptyMat = std::make_shared<Material>(
      "na", "na", Vector3(1.3f, 1.3f, 1.3f), Vector3(1, 1, 1), Vector3(1, 1, 1),
      0, 0, 0.8, 0, 0, 0);

  scene->addPrimitive(std::make_shared<Torus>(
      camera.getPos() + 7.0f * camera.getGaze(), 2, 1, emptyMat, 0, 0, 0));

//  scene->addPrimitive(
//      std::make_shared<AABB>(300, 200, -250, 250, 250, -100, emptyMat));

  Median s = Median();
  BVH bvh(s, scene);
  bvh.buildTree(camera, scene);

  //==

  auto window = sf::RenderWindow(sf::VideoMode({RES, RES}), "FPS: 0",
                                 sf::Style::Titlebar);
  window.setFramerateLimit(FPS_CAP);

  window.setPosition({100, 100});

  window.setMouseCursorGrabbed(true);
  window.setMouseCursorVisible(false);
  sf::Vector2i windowCenter(window.getSize().x / 2, window.getSize().y / 2);
  sf::Mouse::setPosition(windowCenter,
                         window);  // Set the mouse to the center initially

  float moveSpeed = 0.5f;  // camera move speed
  // Camera rotation angles
  float pitch = 0.0f;        // Up-down angle
  float yaw = 0.0f;          // Left-right angle
  float sensitivity = 0.1f;  // Sensitivity

  sf::Clock clock = sf::Clock();
  sf::Time previousTime = clock.getElapsedTime();
  sf::Time currentTime;
  float fps = 0;

  uint8_t pixels[RES * RES * 4];
  sf::Texture texture({RES, RES});
  sf::Sprite sprite(texture);

  Vector3 offset = VEC_0;

  while (window.isOpen()) {
    while (const std::optional event = window.pollEvent()) {
      if (event->is<sf::Event::Closed>() ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape) ||
          sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Q)) {
        window.close();
      } else if (event->is<sf::Event::MouseWheelScrolled>()) {
        auto mouseWheel = event->getIf<sf::Event::MouseWheelScrolled>();
        static float x = 0;
        x += 0.1f * mouseWheel->delta;
        x = std::max(x, 0.5f);
        moveSpeed = 3 * x * x;
      }

      //       else if (event->is<sf::Event::MouseMoved>()) {
      //        auto mouseCord = event->getIf<sf::Event::MouseMoved>();
      //        int cord[2] = {mouseCord->position.x - RES / 2,
      //                       mouseCord->position.y - RES / 2};
      //        printf("%d, %d\n", cord[0], cord[1]);
      //      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
      offset += -Vector3(camera.getW()[0], 0, camera.getW()[2]) * moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
      offset += Vector3(camera.getW()[0], 0, camera.getW()[2]) * moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
      offset += -camera.getU() * moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
      offset += camera.getU() * moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
      offset += camera.getUpNorm() * moveSpeed;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift))
      offset += -camera.getUpNorm() * moveSpeed;

    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2i delta = mousePos - windowCenter;
    yaw += (float)delta.x * sensitivity;
    pitch -= (float)delta.y * sensitivity;

    // Clamp the pitch to prevent flipping
    yaw = (float)fmod(yaw, 360);
    pitch = std::clamp(pitch, -179.9f, 179.9f);
    float y = yaw * (M_PIf / 180);
    float p = pitch * (M_PIf / 180);
    //polar to cartesian transform
    //TODO: initial camera look at point is wrong
    auto camFront =
        Vector3(cos(p) * cos(y), sin(p), sin(y) * cos(p)).normalize();
    //    printf("%f, %f : %f, %f, %f\n", yaw, pitch, camFront[0], camFront[1],
    //           camFront[2]);
    sf::Mouse::setPosition(windowCenter, window);  // reset mouse

    camera.move(offset, camFront + camera.getPos() + offset);
    offset = VEC_0;

    //    auto t = scene->getPrimitiveList().back();
    //    t->turnX(M_PI_4f / 16.0f);
    //    t->turnY(M_PI_4f / 128.0f);
    //    t->turnZ(M_PI_4f / 32.0f);

    rayTracer.render(buffer, camera, scene, bvh);
    for (int i = 0; i < RES; ++i) {
      for (int j = 0; j < RES; ++j) {
        Color c = buffer.at(j, i);
        pixels[(i * RES + j) * 4 + 0] = c[0];
        pixels[(i * RES + j) * 4 + 1] = c[1];
        pixels[(i * RES + j) * 4 + 2] = c[2];
        pixels[(i * RES + j) * 4 + 3] = 255;
      }
    }
    texture.update(pixels);

    window.clear();
    window.draw(sprite);
    window.display();

    // calculate fps
    currentTime = clock.getElapsedTime();
    fps = 1.0f / (currentTime.asSeconds() -
                  previousTime.asSeconds());  // the asSeconds returns a float
    char buff[11];
    snprintf(buff, 11, "FPS: %3.2f", fps);
    window.setTitle(buff);
    previousTime = currentTime;
  }
  //     */

  /*
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    printf(
        "SDL could not be initialized!\n"
        "SDL_Error: %s\n",
        SDL_GetError());
    return 0;
  }

#if defined linux && SDL_VERSION_ATLEAST(2, 0, 8)
  // Disable compositor bypass
  if (!SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0")) {
    printf("SDL can not disable compositor bypass!\n");
    return 0;
  }
#endif

  // Create window
  SDL_Window* window =
      SDL_CreateWindow("Basic C SDL project", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, RES, RES, SDL_WINDOW_SHOWN);
  if (!window) {
    printf(
        "Window could not be created!\n"
        "SDL_Error: %s\n",
        SDL_GetError());
  }
  // Create renderer
  SDL_Renderer* renderer =
      SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf(
        "Renderer could not be created!\n"
        "SDL_Error: %s\n",
        SDL_GetError());
  }
  // Event loop exit flag
  bool quit = false;

  float sensitivity = 0.00001f;
  float pitch = 0, yaw = 0;

  SDL_SetRelativeMouseMode(SDL_TRUE);  // hides mouse
  //  SDL_WarpMouseInWindow(window, RES / 2, RES / 2);

  Uint32 newtime = SDL_GetTicks();
  Uint32 oldtime = 0;

  // ===

  // ===
  // Event loop
  while (!quit) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
      // User requests quit
      if (e.type == SDL_QUIT ||
          (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)) {
        quit = true;
      } else {

        //== spin ===
        if (e.type == SDL_MOUSEMOTION) {
          int deltaX = e.motion.xrel;  // Relative motion in X
          int deltaY = e.motion.yrel;  // Relative motion in Y

          // Use deltaX and deltaY to adjust your camera's yaw and pitch
          yaw -= sensitivity * deltaX;
          pitch -= sensitivity * deltaY;
          Vector3 front;
          front[0] = -cos(yaw) * cos(pitch);
          front[1] = sin(pitch);
          front[2] = sin(yaw) * cos(pitch);
          front.normalize();
          printf("front_0: %f, front_1: %f\n", front[0], front[1]);
          camera.move(VEC_0, camera.getPos() + camera.getGaze() + front);
        }
        //    camera.move(VEC_0, {float(e.motion.x - RES) / 100000, 0, 0});

        //    auto t = scene->getPrimitiveList().back();
        //    t->turnX(M_PI_4f / 16.0f);
        //    t->turnY(M_PI_4f / 128.0f);
        //    t->turnZ(M_PI_4f / 32.0f);

        rayTracer.render(buffer, camera, scene, bvh);

        //=========

        // draw buffer
        for (int y = 0; y < RES; ++y) {
          for (int x = 0; x < RES; ++x) {
            Color& c = buffer.at(x, y);
            SDL_SetRenderDrawColor(renderer, c[0], c[1], c[2], 0);
            SDL_RenderDrawPoint(renderer, x, y);
          }
        }

        SDL_RenderPresent(renderer);  // Update screen

        Uint32 msec = newtime - oldtime;
        oldtime = newtime;
        newtime = SDL_GetTicks();
        if (msec > 0) {
          double fps = 1000.0 / (double)msec;
          char buff[19];
          snprintf(buff, 19, "FPS: %3.2f ms: %i", fps, msec);
          SDL_SetWindowTitle(window, buff);
          if (msec < SCREEN_TICKS_PER_FRAME)
            SDL_Delay(SCREEN_TICKS_PER_FRAME - msec);
        }
      }
    }
  }

  // Destroy renderer, window, and Quit SDL
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
  //  */

#endif  // SFML_WINDOW
}
