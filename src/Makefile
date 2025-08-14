CXX = g++
CFLAGS = -Wall -Wextra -std=c++11
LIB_GDAL = -lgdal
LIB_BOOST = -lboost_filesystem

raster:
	$(CXX) $(CFLAGS) raster.cpp -o raster $(LIB_GDAL) $(LIB_BOOST)

rgb:
	$(CXX) $(CFLAGS) rgb.cpp -o rgb $(LIB_GDAL)

clean:
	rm -v rgb
	rm -v raster
