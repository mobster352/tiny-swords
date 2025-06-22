#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "constants.h"

/*
* Parameters:
* int xIndex: the horizontal index of the sprite sheet
* int yIndex: the vertical index of the sprite sheet
*
* Returns:
* Vector2: the position of the sprite as a Vector2
*/
Vector2 findSprite(int xIndex, int yIndex){
	return (Vector2){xIndex * SPRITE_SIZE, yIndex * SPRITE_SIZE};
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Hello Raylib");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	SetTargetFPS(10); //the sprites were made to be 10 fps / 100ms



	// Assuming you have a texture called 'atlas' loaded
	Texture atlas = LoadTexture("Warrior_Blue.png");
	Rectangle sourceRec = { 0, 0, 192, 192 }; // Example: First sprite in the sheet
	Rectangle destRec = { 100, 100, 192, 192 }; // Example: Draw at (100, 100)
	Vector2 origin = { 0.0f, 0.0f }; // Optional origin point

	int xIndex = 0;
	int yIndex = 0;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		// Draw the sprite
		Vector2 sprite = findSprite(xIndex, yIndex);
		sourceRec.x = sprite.x;
		sourceRec.y = sprite.y;
		DrawTexturePro(atlas, sourceRec, destRec, origin, 0.0f, WHITE);
		xIndex++;

		if(xIndex > 6){
			xIndex = 0;
			yIndex++;
			if(yIndex > 8)
				yIndex = 0;
		}
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(atlas);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
