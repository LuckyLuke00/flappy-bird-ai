<div align="center">

# Flappy Bird AI: Teaching an AI to Fly

[![Raylib](https://img.shields.io/badge/Raylib-4.2.0-1864ab?style=flat-square&labelColor=212529)](https://www.raylib.com/)
[![C++](https://img.shields.io/badge/C++-17%2B-1864ab?style=flat-square&logo=C%2B%2B&logoColor=white&labelColor=212529)](https://isocpp.org/)
[![Ko-fi](https://img.shields.io/badge/Ko--fi-Support-1864ab?style=flat-square&logo=Ko-fi&logoColor=white&labelColor=212529)](https://ko-fi.com/luckyluke)
[![MIT License](https://img.shields.io/badge/License-MIT-1864ab?style=flat-square&logo=open-source-initiative&logoColor=white&labelColor=212529)](https://opensource.org/licenses/MIT)

This project is a Flappy bird game where the computer learns to play it by itself using a genetic algorithm. The game was implemented in C++ using the Raylib library. The final result is that the bird never hits any pipes. The genetic algorithm tries to find the optimal difference between the center of the pipe gap and the bird's y position to start flapping.

</div>

## Design/Implementation

### The genetic algorithm works as follows:
1. Initialize a random population of birds with random values.
2. For each generation:
    - Evaluate the fitness of each strategy by measuring the distance traveled.
    - Keep the best bird.
    - Apply crossover and mutation to generate new birds.
3. Repeat the process until the bird no longer hits any pipes.

## How to Use

*The program works for both 64-bit and 32-bit.*

1. Clone the repository.
2. Open the solution with Visual Studio 2022.
3. Build and run the project.

**Warning:** Occasionally, when launching the project, Raylib can take a while to initialize. The program will appear to be stuck, but it isn't. Just wait for a little and the game will start.

## Conclusion/Future Work

This project demonstrates the potential of genetic algorithms for solving problems that involve finding an optimal strategy. In future work, it would be interesting to explore the possibility of combining a genetic algorithm with a neural network for training the agent.

#### *References*

- [*Build a Genetic Algorithm From Scratch In C++ 🧬 💻 - by Top Shelf Technology*](https://youtu.be/SWi-4IHFf1c)

- [*How AIs learn part 2 || Coded example - by Code Bullet*](https://youtu.be/BOZfhUcNiqk)
