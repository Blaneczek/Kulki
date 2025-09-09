# Getting Started
<sub>Unreal Engine version 5.5.4</sub>

Clone the repository or download the ZIP, generate Visual Studio files, and compile using the IDE.

# KULKI
Kulki is a simple game in which the player's task is to eat weaker enemies and avoid stronger ones.

# Movement
Movement is controlled with the mouse. When you click the right button, the player's ball follows the cursor. 

<img src="https://github.com/user-attachments/assets/54b08d13-6441-4c58-aef9-00946a86485b" width="800">

# Enemies
Enemies are spawned at the beginning of the game. The further away they are from the player, the higher their attribute values. These values can be set using a float curve where the X axis is the distance from the player and the Y axis is the enemy's Strength/Speed. 
- Yellow ball: adds Speed if we eat it, subtracts it if it eats us
- Red ball: adds Strength if we eat it, subtracts it if it eats us
- Purple ball: subtracts Strength and Speed if we eat it or it eats us

When one of our attributes drops to 0, we lose and start over.

<img src="https://github.com/user-attachments/assets/68e998d7-bc25-4a8e-a3fd-e947d63d9f8c" width="800">

<br>Information about enemy spawns can be set in DataAsset: for each color, we select distance ranges (e.g., 5 yellow balls will spawn between 1000 and 1500 from the player).

<img src="https://github.com/user-attachments/assets/f58c2307-3a02-495b-a8d7-ecef96c072a4" width="800">

<br>The Strength and Speed values of opponents scale with the selected difficulty level. The multiplier is set in the same DataAsset, and the difficulty level is set after clicking Start Game in the menu.

<img src="https://github.com/user-attachments/assets/ced9ea94-7b2b-4d66-9a10-fa363cd3932d" width="800">
<img src="https://github.com/user-attachments/assets/b0ea5828-7dde-454d-bdd9-212976563b1e" width="800">

