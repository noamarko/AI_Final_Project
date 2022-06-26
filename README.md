# **2D War Rooms**
A game I built as a final project in an artificial intelligence course.  
The are 2 types of teams - Cyan team and Olive teams.  
Each team has two soldiers starting in a random room on the map (The soldiers from the same team begin in the same room).  
There are 2 health points on the map marked in light green and 2 ammo points marked in brown.  
The black shapes in the rooms are shelters.  
The black dots are shots fired by the soldiers.  
The black dots that explode are grenades thrown by the soldiers.  

Each soldier begin with 100 health points, 20 bullets and 5 grenades.

# **Game rules**
1) Search for health points if soldier does not have enough  
2) Search for ammo if soldier does not have enough  
3) When the opponent is in the same room:  
	3.1) Search for cover while attacking.  
	3.2) Attacking rules:  
		3.2.1) Throw grenade when the soldier or opponent soldier is behind a shelter  
		3.2.2) Otherwise Shoot  
4) Move to random room

# **Start Game Instructions**
To see the legend of the soldiers -> Click the left mouse button.  
Explanation of the details:   
	H = Health  
	B = Current bullets ammo  
	G = Current grenades ammo  
	R = Current room number  

To start the game -> Click the Right mouse button and press start.
