CXX = clang++
CXXFLAGS = -Weverything -Wextra -std=c++17 -Wno-c++98-compat -Wno-unsafe-buffer-usage

raster:
	$(CXX) $(CXXFLAGS) src/raster.cpp -o raster -lgdal -lboost_filesystem

rgb:
	$(CXX) $(CXXFLAGS) src/rgb.cpp -o rgb -lgdal

to_byte:
	$(CXX) $(CXXFLAGS) src/to_byte.cpp -o to_byte -lgdal

numero_digital:
	$(CXX) $(CXXFLAGS) src/numero_digital.cpp -o numero_digital -lgdal

.PHONY = format
format:
	clang-format --style=Microsoft -i src/*.cpp
