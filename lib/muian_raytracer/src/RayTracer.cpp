//
// Created by agmui on 12/10/24.
//

#include "RayTracer.h"

int RES = 100;

void RayTracer::printASCII(Buffer<Color> buffer, const int print_height,
                           const int print_width) {
  static auto startTime = std::chrono::system_clock::now();

  auto endTime = std::chrono::system_clock::now();
  std::chrono::duration<double, std::milli> delta = endTime - startTime;
  startTime = endTime;
  //  char printBuff[(print_width+1)*print_height];
  //  char *ptr = printBuff;
  constexpr double FPS_MAX = 1000.0 / 60.0;
  constexpr char pallet[] = ".,-~:;=!*#$@";
  for (int y = 0; y < RES; y += RES / print_height) {
    for (int x = 0; x < RES; x += RES / print_width) {
      Color c = buffer.at(x, y);
      const Vector3 maxBright = Vector3(255, 255, 255);
      Vector3 help = Vector3(c[0], c[1], c[2]);
      int ascii_intensity =
          (int)(help.squaredLength() *
                ((float)(strlen(pallet) - 1) / maxBright.squaredLength()));
      putc(pallet[ascii_intensity], stdout);
      //      *ptr = select[ascii_intensity];
      //      ptr++;
    }
    putc('\n', stdout);
    //    *ptr='\n';
    //    ptr++;
  }
  printf("\033[H");  // reset to top left
                     //  puts(printBuff);
  fflush(stdout);

  //frame limiting
  if (delta.count() < FPS_MAX) {
    usleep(static_cast<__useconds_t>((FPS_MAX - delta.count()) * 1000));
  }
}

inline void multiThreadTrace(const Camera& camera, int i, BVH& bvh,
                             const std::shared_ptr<Scene>& scene,
                             Buffer<Vector3>& intensityBuffer, float& maxIntensity){
  for (int y = RES*i/NUM_THREADS; y < RES*(i+1)/NUM_THREADS; y++) {
    for (int x = 0; x < RES; x++) {
      const Ray& r =
          camera.getRay(x, y);  //TODO: maybe pass cam into raygen interface
      //Convert vectors to RGB colors for testing results
      Vector3 intensity = r.getDirection(); //VEC_0;

      hitpoint hp;
      //      scene->trace(hp, r);
      scene->trace(hp, r, MAX_RAY_DIST, bvh);

#ifdef OBJ_DEBUG
      if (x == 49 && y == 36) {
        draw = true;
        ObjViewer::get().addRay(r.getOrigin(), r.getDirection(),
                                (hp.t == -1) ? 1000 : hp.t);  // initial ray
      }
#endif

      if (hp.t != -1)
        intensity = scene->shade(hp, r, bvh);
      intensityBuffer.at(x, y) = intensity;
      maxIntensity = std::max(maxIntensity, intensity.squaredLength()); //TODO: not thread safe
      //      maxIntensity = std::max({maxIntensity, intensity[0], intensity[1], intensity[2]});
    }
#if defined(RELEASE) && NUM_THREADS==1 && !defined(SFML_WINDOW)
//    if (y % 10 == 0)
//      printf("%2.0f%\n", 100 * (float)y / RES);
#endif
  }
}

#ifdef OBJ_DEBUG
//int cord[2] = {0, 0};
bool draw = false;
#endif
void RayTracer::render(Buffer<Color>& buffer, const Camera& camera,
                       const std::shared_ptr<Scene>& scene, BVH& bvh) {

//  bvh.printDotfile();

  Buffer<Vector3> intensityBuffer = Buffer<Vector3>(RES, RES);
  float maxIntensity = 0;
#if NUM_THREADS!=1
  std::thread theadArr[NUM_THREADS];
  for (int i = 0; i < NUM_THREADS; ++i)
    theadArr[i] = std::thread(multiThreadTrace,camera,i,std::ref(bvh),scene,std::ref(intensityBuffer), std::ref(maxIntensity));
  for (auto & t : theadArr)
    t.join();
#else
  multiThreadTrace(camera, 0, bvh, scene, intensityBuffer, maxIntensity);
#endif
  for (int y = 0; y < RES; ++y) {
    for (int x = 0; x < RES; ++x) {
      //             Color c = Color(((hp.normal)[0] * 255.0f), ((hp.normal)[1] * 255.0f),
      //                      ((hp.normal)[2] * 255.0f));

      buffer.at(x, y) =
          VEC2COLOR(intensityBuffer.at(x, y) / sqrt(maxIntensity));

      //      buffer.at(x, y) = VEC2COLOR(intensityBuffer.at(x,y) / maxIntensity);
    }
  }
}

void RayTracer::run(char* filename, char* outputPNG, bool ascii) {

  // create a frame buffer for RESxRES
  Buffer<Color> buffer = Buffer<Color>(RES, RES);

  Loader loader(filename);
  std::shared_ptr<Scene> scene = loader.load(filename);
  //creates a camera object
  Camera camera = loader.loadCamera();
  //    loader.~Loader();  //TODO:

  std::shared_ptr<Material> emptyMat = std::make_shared<Material>(
      "na", "na", Vector3(1.3f, 1.3f, 1.3f), Vector3(1, 1, 1), Vector3(1, 1, 1),
      0, 0, 0.9, 0, 0, 0);


  if (ascii)
    scene->addPrimitive(std::make_shared<Torus>(
        camera.getPos() + 7.0f * camera.getGaze(), 2, 1, emptyMat, 0, 0, 0));

//  scene->addPrimitive(
//      std::make_shared<AABB>(300, 200, -250, 250, 250, -100, emptyMat));

  uint64_t startTime = TIMENOW();
  Median s = Median();
//  SAH s = SAH();
  BVH bvh(s, scene);
  bvh.buildTree(camera, scene);
//  bvh.printDotfile();
  printf("num nodes in bvh: %d\n", bvh.numNodes());
  printf("bvh node size: %lu\n", sizeof(BVHnode));
  printf("aabb: %lu\n", sizeof(AABB));
  printf("build tree: %lu us\n", TIMENOW() - startTime);
  do {
    if (ascii) {
      auto t = scene->getPrimitiveList().back();
      t->turnX(M_PI_4f / 16.0f);
      t->turnY(M_PI_4f / 128.0f);
      t->turnZ(M_PI_4f / 32.0f);
    }

    startTime = TIMENOW();
    render(buffer, camera, scene, bvh);
    auto rez = TIMENOW() - startTime;
    printf("render: %lu us | %lu sec\n", rez, rez / 1000000);

    if (ascii)
      printASCII(buffer);
  } while (ascii);

  //Write output buffer to file
  simplePNG_write(outputPNG, buffer.getWidth(), buffer.getHeight(),
                  (unsigned char*)&buffer.at(0, 0));
}
