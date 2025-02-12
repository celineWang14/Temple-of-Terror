# **Temple of Terror 🎮**  
*A C++ Object-Oriented Adventure Game*

## **Overview 🏰**  
**Temple of Terror** is a text-based adventure game written in C++. The game follows an object-oriented programming (OOP) approach, utilizing classes and inheritance to manage game elements like characters, objects, and game logic.

## **Features ✨**  
- 🏹 **Dynamic Character System** – Players interact with different characters and objects.  
- 🔑 **Interactive Gameplay** – Solve puzzles and navigate through the temple to escape.  
- ⚔️ **Combat Mechanics** – Encounter enemies and use strategy to survive.  
- 🏆 **Progression System** – Gain items and unlock new areas.  

## **Project Structure 📂**  
```
Temple-of-Terror/
│── Actor.cpp          # Defines character behaviors
│── Actor.h            
│── Game.cpp           # Core game logic
│── Game.h            
│── main.cpp           # Entry point of the game
│── object.cpp         # Handles game objects (items, weapons, etc.)
│── object.h           
│── utilities.cpp      # Utility functions (randomization, input handling, etc.)
│── utilities.h        
│── report.docx        # Project documentation
│── README.md          # Project documentation
```

## **Technologies Used 🛠️**  
- **Language:** C++  
- **Programming Paradigm:** Object-Oriented Programming (OOP)  
- **Concepts:** Encapsulation, Polymorphism, Inheritance, Abstraction  

## **How to Compile & Run 🚀**  
1. **Clone the repository**  
   ```bash
   git clone https://github.com/NannanWang/Temple-of-Terror.git
   ```
2. **Navigate to the project folder**  
   ```bash
   cd Temple-of-Terror
   ```
3. **Compile the game using g++**  
   ```bash
   g++ -o Temple main.cpp Game.cpp Actor.cpp object.cpp utilities.cpp
   ```
4. **Run the game**  
   ```bash
   ./Temple
   ```

## **Future Improvements 🚧**  
- 🎨 Add a graphical interface (SFML or SDL for visual rendering).  
- 🧠 Improve AI for enemy behaviors.  
- 📜 Expand the storyline and add more interactive elements.  

## **Contributors 👾**  
- **Nannan Wang** ([@NannanWang](https://github.com/NannanWang))  
