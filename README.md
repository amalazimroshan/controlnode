# ControlNode

A geospatial traffic management system that uses OpenStreetMap data to analyze intersections and control traffic signals. Features a web interface for real-time configuration.

## Quick Start

### Prerequisites
- C++ Compiler & CMake
- libcurl
- Python 3.x (for Python implementation)
- Web browser

### Installation

```bash
# Clone repository
git clone https://github.com/amalazimroshan/controlnode.git
cd controlnode

# Build C++ implementation
mkdir build && cd build
cmake ..
make

# Install Python dependencies (if using Python implementation)
pip install osmnx networkx matplotlib
```

### Running the System

#### C++ Version
1. Start the server:
```bash
./controlnode [latitude] [longitude]  # Coordinates optional
```

2. Access web interface:
- Open `http://localhost:8080`
- Configure traffic signals, timing, and road conditions

#### Python Version
- Open Jupyter notebook in [Google Colab](https://colab.research.google.com/github/amalazimroshan/controlnode/blob/main/get_bearings_from_nearest_node_osmnx.ipynb)
- Or run locally using Jupyter

## Features
- Real-time traffic signal management
- OpenStreetMap data integration
- Web-based configuration interface
- Support for both Python and C++ implementations
