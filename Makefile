CXX = g++
CFLAGS = -Wall -Wextra -std=c++11

raster:
	$(CXX) $(CFLAGS) src/raster.cpp -o raster -lgdal -lboost_filesystem

rgb:
	$(CXX) $(CFLAGS) src/rgb.cpp -o rgb -lgdal

numero_digital:
	$(CXX) $(CFLAGS) src/numero_digital.cpp -o numero_digital -lgdal -lboost_filesystem
