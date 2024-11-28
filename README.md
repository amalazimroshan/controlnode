# ControlNode

## Overview

ControlNode is a geospatial routing project that uses OpenStreetMap data to analyze road intersections and calculate road directions.

## Implementations

I have implemented it using Python Jupyter notebook and C++

- **C++ Project**: Fetches road intersection data from Overpass API and calculates road directions
- **Python Jupyter Notebook**: Uses OSMnx library to retrieve road intersection bearings

## Python Notebook

[![Open In Colab](https://colab.research.google.com/assets/colab-badge.svg)](https://colab.research.google.com/github/amalazimroshan/controlnode/blob/main/get_bearings_from_nearest_node_osmnx.ipynb)

### Features

- Retrieves road intersection data using OSMnx
- Calculates bearings of roads at a given intersection
- Visualizes road network and intersection details

## C++ Project

### Functionality

- Interacts with Overpass API
- Processes road intersection geographical data
- Computes road directions and bearings

## Prerequisites

- C++ Compiler
- Python 3.x
- OSMnx Library
- Overpass API access
- libcurl

## Installation

```bash
# Clone the repository
git clone https://github.com/amalazimroshan/controlnode.git
cd controlnode

mkdir build
cd build
cmake ..
make
# Install Python dependencies
pip install osmnx networkx matplotlib
```

## Usage

1. Configure API credentials
2. Run C++ executable to fetch intersection data
3. Use Jupyter notebook for visualization and analysis

<!-- ## License

[Specify your license]

## Contributors

[List contributors] -->
