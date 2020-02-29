#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mpi.h>
#include <string>
#include <vector>
///
/// http://wili.cc/blog/mandelbrot-mpi.html
///
#define MCW MPI_COMM_WORLD
using namespace std;

const int Master = 0;

struct Color {
    int red;
    int green;
    int blue;
};

struct MandelbrotConfig {
    int width;
    int height;
    double xComplexMin;
    double xComplexMax;
    double yComplexMin;
    double yComplexMax;
    double maxIterations;
    Color colorOne;
    Color colorTwo;
    string outputFileName;
};

MandelbrotConfig readConfig(string configFile){
    ifstream fin;
    MandelbrotConfig config;

    fin.open(configFile);
    if (fin){
        fin >> config.width >> config.height;
        fin >> config.xComplexMin >> config.xComplexMax;
        fin >> config.yComplexMin >> config.yComplexMax;
        fin >> config.maxIterations;
        fin >> config.colorOne.red >> config.colorOne.green >> config.colorOne.blue;
        fin >> config.colorTwo.red >> config.colorTwo.green >> config.colorTwo.blue;
        fin >> config.outputFileName;
        fin.close();

    }
    if(!fin){
        cout << "Could not open File";
    }
    return config;
}
Color getPixelColor(Color color1, Color color2, double iterations, double maxIterations) {
    Color colorReturn;
    double myColorR = static_cast<double>((color2.red - color1.red) / maxIterations);
    double myColorG = static_cast<double>((color2.green - color1.green) / maxIterations);
    double myColorB = static_cast<double>((color2.blue - color1.blue) / maxIterations);
    colorReturn.red = (color1.red + (iterations * myColorR));
    colorReturn.green = (color1.green + (iterations * myColorG));
    colorReturn.blue = (color1.blue + (iterations * myColorB));
    return colorReturn;
}

void writeFile(MandelbrotConfig config, std::vector<std::vector<Color> > array){
  ofstream fout;
  fout.open(config.outputFileName);
  fout << "P3" << endl;
  fout << config.width << " " << config.height << endl;
  fout << "255" << endl;
  for(int i = 0; i < config.height; i++){
    for(int j = 0; j < config.width; j++){
      Color Pixel = array[i][j];
      fout << Pixel.red << " " << Pixel.green << " " << Pixel.blue << " ";
    }
    fout << endl;
  }
  fout.close();
}

void drawMandelbrot(MandelbrotConfig config, int rank, int &worldsize){
    double pixelWidth = ((config.xComplexMax-config.xComplexMin)/config.width);
    double pixelHeight = ((config.yComplexMax-config.yComplexMin)/config.height);
    std::vector<std::vector<Color> > pixels(config.height, std::vector<Color> (config.width));
    std::vector<Color> sub_pixels;

  int *displace;
  displace = (int *)calloc(worldsize,sizeof(int));
  int *dim_list;
  dim_list = (int *)calloc(worldsize,sizeof(int));
  int j = 0;
  for (int i=0; i<worldsize; i++) {
    displace[i] = j;
    cout << "displace[" << i << "] = " << displace[i] << endl;
    j += config.width;
    dim_list[i] = config.width;
  }


  MPI_Scatterv(&(pixels), sendcounts, displs, MPI_INT, &sub_pixels,
              100, MPI_INT, 0, MCW);
    for(int i = 0; i < config.height; i++) {
    int _i = i * (rank + 1);

    for (int j = 0; j < config.width; j++) {
      double x0 = config.xComplexMin + j * pixelWidth;
      double y0 = config.yComplexMin + _i * pixelHeight;
      double x = 0;
      double y = 0;
      double iteration = 0;
      while ((x * x) + (y * y) < (2 * 2) && iteration < config.maxIterations) {
        double xtemp = (x * x) - (y * y) + x0;
        y = 2 * x * y + y0;
        x = xtemp;
        iteration = iteration + 1;
      }
      Color Pixel = getPixelColor(config.colorOne, config.colorTwo, iteration,
                                  config.maxIterations);
      pixels[i][j] = Pixel;
    }
  }
    if (!rank) {
    MPI_Gatherv(&sub_pixels,config.width,MPI_INT,&pixels,&dim_list[rank],&displace[rank],MPI_INT,0,MPI_COMM_WORLD);
  }


    writeFile(config, pixels);
}

int main(int argc, char *argv[]) {
  string configFile;
  MandelbrotConfig config;
  int rank , size;
  int  data;
  MPI_Init (&argc , &argv);
  MPI_Comm_rank(MCW , &rank);
  MPI_Comm_size(MCW , &size);
  if(rank == 0) {
    // Read in config file location from user
    cout << "Mandelbrot Config File: ";
    cin >> configFile;
    // Read config file contents into MandelbrotConfig struct instance
    config = readConfig(configFile);
    // Compute and write specified mandelbrot image to PPM file
    // drawMandelbrot(config);
  }



  drawMandelbrot(config, rank, size);
  MPI_Finalize();
  return 0;
}
