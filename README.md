# Solar System Simulation

This project is a 3D simulation of the solar system, developed using C++, Qt, and OpenGL. It's designed as a learning project to explore 3D graphics, simulations, and object-oriented programming in C++.

## Features

*   **3D Visualization:** The solar system is rendered in a 3D space, with planets and the sun represented as spheres.
*   **Orbital Representation:** Planets are positioned roughly according to their real orbital distances from the Sun.
*   **Camera Controls:** You can rotate the camera around the solar system to view it from different angles.
*   **Zoom Functionality:** The user can zoom in and out to get a closer look at individual planets or a broader view of the system.
*   **Planetary Rotation:** Planets rotate around the sun.
*   **Satellite Orbits:** Some planets have satellites orbiting them.
*   **Realistic Lighting and Shadows:** The sun acts as a light source, and planets cast shadows based on their position relative to the sun.
*   **Adjustable Animation Speed:**  A slider allows you to control the speed of the planets' orbital motion.
*   **Saturn Rings:** A simple representation of Saturn's rings.

## Controls

*   **Mouse Drag:**
    *   **Left Click and Drag:** Rotate the camera around the solar system.
    *   **Middle Click and Drag:** Move the camera across the scene.
*   **Mouse Wheel:** Zoom in and out.
*   **Slider:** Adjusts the animation speed of the planets.

## Technologies Used

*   **C++:** Programming language.
*   **Qt:** Cross-platform application development framework for UI and other functionalities.
*   **OpenGL:** Graphics API for rendering 2D and 3D vector graphics.
*   **CMake:** Cross-platform build system.

## How to Build

1.  **Prerequisites:**
    *   CMake (3.16 or higher)
    *   Qt 6 (with `Core`, `Gui`, `OpenGL`, `OpenGLWidgets`, `Widgets` modules)
    *   A C++ compiler that supports C++17 or higher (e.g., GCC, Clang, MSVC)
2.  **Clone the repository:**

    ```bash
    git clone [repository_url]
    cd SolarSystem
    ```
3.  **Create a build directory:**

    ```bash
    mkdir build
    cd build
    ```
4.  **Run CMake:**

    ```bash
    cmake ..
    ```
5.  **Build the project:**

    ```bash
    cmake --build .
    ```
6.  **Run the application:**

    ```bash
     ./SolarSystem  # Linux/macOS
     .\SolarSystem.exe # Windows
    ```

## Project Structure
```bash
SolarSystem/

├── CMakeLists.txt # Файл сборки CMake
├── SolarSystem/ #Папка с файлами проекта
├── CMakeLists.txt:13 # Дополнительный CMake file (может быть не нужен)
├── Header Files/ # Папка с заголовочными файлами
│ ├── celestialbody.h # Заголовочный файл базового класса CelestialBody
│ ├── glwidget.h # Заголовочный файл класса GLWidget (OpenGL виджет)
│ ├── mainwindow.h # Заголовочный файл класса MainWindow
│ ├── planet.h # Заголовочный файл класса Planet
│ ├── satellite.h # Заголовочный файл класса Satellite
│ └── sun.h # Заголовочный файл класса Sun
├── Resources/ # Папка с ресурсами
│ ├── resources.qrc # Файл ресурсов Qt
│ └── / # Папка с шейдерами
│ ├── fragmentshader.fsh # Файл фрагментного шейдера OpenGL
│ └── vertexshader.vsh # Файл вершинного шейдера OpenGL
└── Source Files/ # Папка с исходными файлами
├── celestialbody.cpp # Исходный файл базового класса CelestialBody
├── glwidget.cpp # Исходный файл класса GLWidget
├── main.cpp # Главный файл приложения
├── mainwindow.cpp # Исходный файл класса MainWindow
├── planet.cpp # Исходный файл класса Planet
├── satellite.cpp # Исходный файл класса Satellite
└── sun.cpp # Исходный файл класса Sun
├── .gitignore # Файл игнорирования Git
└── README.md # Файл README проекта# Файл README проекта
```

## Future Improvements

*   More accurate planetary orbits and speeds
*   More detailed rendering of planets (textures, more complex geometry)
*   Add different celestial bodies (asteroids, comets)
*   Add adjustable light source
*   User interface for controlling parameters
*   Optimization for performance



## Author
# Kotolevskij Vladislav
