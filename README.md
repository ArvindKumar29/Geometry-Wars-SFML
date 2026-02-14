# 🚀 Geometry Wars Clone (C++ & SFML)

A fast-paced, 2D arcade survival shooter built entirely from scratch using **C++** and the **SFML (Simple and Fast Multimedia Library)**. This game features a custom-built Entity-Component-System (ECS) architecture to handle rendering, physics, and game logic efficiently.

## 🎮 Gameplay Overview
Survive as long as you can against endless waves of geometric enemies! Aim, shoot, and dodge. When large enemies are destroyed, they shatter into smaller pieces in a 360-degree blast, requiring quick reflexes to survive.

## ✨ Key Features
* **Custom ECS Engine:** Built a robust Entity-Component-System from the ground up for memory-efficient game loop management.
* **Math-Driven Mechanics:** Uses Trigonometry (Sine/Cosine) to calculate perfect 360-degree vector blasts for splitting enemies.
* **Collision Detection:** Custom mathematical bounding-box and radius-based collision for bullets, player, and enemies.
* **Memory Management:** Implemented `Lifespan` components to safely destroy off-screen entities and avoid memory leaks.
* **Data-Driven Design:** Game variables (window size, fonts, player speed) are dynamically loaded from a `Config.txt` file.

## 🕹️ Controls
* **W, A, S, D:** Move the Player
* **Mouse Cursor:** Aim
* **Left Mouse Click:** Shoot Bullets
* **Esc:** Pause/Quit Game

## 🛠️ How to Build & Run (For Developers)
If you want to compile the source code yourself:
1. Clone this repository:
   ```bash
   git clone [https://github.com/YOUR_USERNAME/Geometry-Wars-SFML.git](https://github.com/YOUR_USERNAME/Geometry-Wars-SFML.git)
2. Open the .slnx or .vcxproj file in Visual Studio.
3. Ensure you have SFML 2.5+ configured in your Visual Studio project settings (Include Directories, Library Directories, and Linker Inputs).
4. Set the build mode to Release (x64) for optimal performance.
5. Build and Run!

(Note: Make sure Config.txt and arial.ttf are in the same directory as your compiled .exe)

## 📂 Assets
Font: Arial
Graphics: Procedurally generated SFML shapes (sf::CircleShape).

## 👨‍💻 Author
Developed by **_ARVIND KUMAR_** as a journey into C++ Game Engine Architecture.
