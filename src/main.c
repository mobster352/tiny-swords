#include "raylib.h"
#include "raymath.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include "constants.h"

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

void* concatVector2ToString(const char *string, Vector2 value){
	char buffer[32];
	// Format the vector as (x, y)
    snprintf(buffer, sizeof(buffer), "(%d, %d)", (int)value.x, (int)value.y);
    // Calculate required size for new string
        //Calculate required size for new string
        size_t new_string_len = strlen(string) + strlen(buffer) + 1; //+1 for null terminator
        //Allocate memory for new string
        char *result = (char *)malloc(new_string_len);
        if(result == NULL){
                perror("Failed to allocate memory");
                return NULL;
        }
        //Copy the first string
        strcpy(result, string);
        strcat(result, buffer);
        return result;
}

void drawTextWithVector2(char *text, Vector2 value, int posX, int posY, int fontSize, Color color){
	void *newText = concatVector2ToString(text, value);
	DrawText(newText,posX,posY,fontSize,color);
	free(newText);
}

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

void playAnimationLoop(Texture atlas, Rectangle sourceRec, Rectangle destRec, Vector2 origin, int animation, int *xIndex){
	Vector2 sprite = findSprite(*xIndex, animation);
	sourceRec.x = sprite.x;
	sourceRec.y = sprite.y;
	DrawTexturePro(atlas, sourceRec, destRec, origin, 0.0f, WHITE);
	(*xIndex)++;
	if(*xIndex > ANIMATION_STEPS)
		*xIndex = 0;
}

void playAnimationOnce(Texture atlas, Rectangle sourceRec, Rectangle destRec, Vector2 origin, int *animation, int *xIndex, bool *isOnceAnimationPlaying){
	Vector2 sprite = findSprite(*xIndex, *animation);
	sourceRec.x = sprite.x;
	sourceRec.y = sprite.y;
	DrawTexturePro(atlas, sourceRec, destRec, origin, 0.0f, WHITE);
	(*xIndex)++;
	if(*xIndex > ANIMATION_STEPS){
		*xIndex = 0;
		*animation = IDLE_ANIMATION;
		*isOnceAnimationPlaying = false;
	}
}

void changeAnimation(int *xIndex, int *animation, int newAnimation){
	*xIndex = 0;
	*animation = newAnimation;
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
	Texture playerAtlas = LoadTexture("Warrior_Blue.png");
	Rectangle sourceRec = { 0, 0, 192, 192 }; // Example: First sprite in the sheet
	Rectangle destRec = { 100, 100, 192, 192 }; // Example: Draw at (100, 100)
	Vector2 origin = { 0.0f, 0.0f }; // Optional origin point

	Texture groundAtlas = LoadTexture("Tilemap_Flat.png");
	Rectangle groundSourceRec = { 0, 0, 192, 192 }; // Example: First sprite in the sheet
	Rectangle groundDestRec = { 0, 0, 192, 192 }; // Example: Draw at (100, 100)

	int xIndex = 0;
	int yIndex = 0;
	int animation = IDLE_ANIMATION;
	bool isOnceAnimationPlaying = false;
	bool hValue = false;
	Vector2 unitVector = Vector2Zero();

	Player player = {
		{
			(Vector2){GetScreenWidth()/2-COLLIDER_WIDTH_SIZING,GetScreenHeight()/2-COLLIDER_HEIGHT_SIZING}
		},
		{
			player.transform2D.position.x + COLLIDER_WIDTH_SIZING/SPRITE_DIVISOR,
			player.transform2D.position.y + COLLIDER_HEIGHT_SIZING/SPRITE_DIVISOR,
			SPRITE_SIZE/4,
			SPRITE_SIZE/2.5f
		}
	};
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		drawTextWithVector2("Pos: ", GetMousePosition(), 1100, 780, 20, WHITE);

		for(int i=0; i<7; i++){
			for(int j=0; j<5; j++){
				Rectangle dest = {192 * i, 192 * j, 192, 192};
				DrawTexturePro(groundAtlas, groundSourceRec, dest, origin, 0, WHITE);
			}
		}

		// Draw the sprite
		// Vector2 sprite = findSprite(xIndex, yIndex);
		// sourceRec.x = sprite.x;
		// sourceRec.y = sprite.y;
		// DrawTexturePro(atlas, sourceRec, destRec, origin, 0.0f, WHITE);
		// xIndex++;

		// if(xIndex > 6){
		// 	xIndex = 0;
		// 	yIndex++;
		// 	if(yIndex > 8)
		// 		yIndex = 0;
		// }

		if(IsKeyDown(KEY_D) && !isOnceAnimationPlaying){
			if(animation != WALK_ANIMATION)
				changeAnimation(&xIndex, &animation, WALK_ANIMATION);
			if(hValue){
				sourceRec.width = -sourceRec.width;
				hValue = false;
			}
			unitVector = (Vector2){1,0};
			player.transform2D.position = Vector2Add(player.transform2D.position, Vector2Scale(unitVector, PLAYER_SPEED * GetFrameTime()));
			player.collider.x = player.transform2D.position.x + COLLIDER_WIDTH_SIZING/SPRITE_DIVISOR;
			player.collider.y = player.transform2D.position.y + COLLIDER_HEIGHT_SIZING/SPRITE_DIVISOR;
		}
		if(IsKeyDown(KEY_A) && !isOnceAnimationPlaying){
			if(animation != WALK_ANIMATION)
				changeAnimation(&xIndex, &animation, WALK_ANIMATION);
			if(!hValue){
				sourceRec.width = -sourceRec.width;
				hValue = true;
			}
			unitVector = (Vector2){-1,0};
			player.transform2D.position = Vector2Add(player.transform2D.position, Vector2Scale(unitVector, PLAYER_SPEED * GetFrameTime()));
			player.collider.x = player.transform2D.position.x + COLLIDER_WIDTH_SIZING/SPRITE_DIVISOR;
			player.collider.y = player.transform2D.position.y + COLLIDER_HEIGHT_SIZING/SPRITE_DIVISOR;
		}
		if(IsKeyDown(KEY_W) && !isOnceAnimationPlaying){
			if(animation != WALK_ANIMATION)
				changeAnimation(&xIndex, &animation, WALK_ANIMATION);
			unitVector = (Vector2){0,-1};
			player.transform2D.position = Vector2Add(player.transform2D.position, Vector2Scale(unitVector, PLAYER_SPEED * GetFrameTime()));
			player.collider.x = player.transform2D.position.x + COLLIDER_WIDTH_SIZING/SPRITE_DIVISOR;
			player.collider.y = player.transform2D.position.y + COLLIDER_HEIGHT_SIZING/SPRITE_DIVISOR;
		}
		if(IsKeyDown(KEY_S) && !isOnceAnimationPlaying){
			if(animation != WALK_ANIMATION)
				changeAnimation(&xIndex, &animation, WALK_ANIMATION);
			unitVector = (Vector2){0,1};
			player.transform2D.position = Vector2Add(player.transform2D.position, Vector2Scale(unitVector, PLAYER_SPEED * GetFrameTime()));
			player.collider.x = player.transform2D.position.x + COLLIDER_WIDTH_SIZING/SPRITE_DIVISOR;
			player.collider.y = player.transform2D.position.y + COLLIDER_HEIGHT_SIZING/SPRITE_DIVISOR;
		}

		destRec = (Rectangle){ player.transform2D.position.x, player.transform2D.position.y, SPRITE_SIZE/SPRITE_DIVISOR, SPRITE_SIZE/SPRITE_DIVISOR };

		//Idle Animation
		if(isLooping[animation] && !isOnceAnimationPlaying)
			playAnimationLoop(playerAtlas, sourceRec, destRec, origin, animation, &xIndex);
		else
			playAnimationOnce(playerAtlas, sourceRec, destRec, origin, &animation, &xIndex, &isOnceAnimationPlaying);

		if(IsKeyReleased(KEY_D) || IsKeyReleased(KEY_A) || IsKeyReleased(KEY_W) || IsKeyReleased(KEY_S)){
			changeAnimation(&xIndex, &animation, IDLE_ANIMATION);
		}
		if(IsKeyPressed(KEY_SPACE)){
			if(Vector2Equals(unitVector, (Vector2){1,0}) || Vector2Equals(unitVector, (Vector2){-1,0}))
				changeAnimation(&xIndex, &animation, HOOK_ANIMATION);
			else if(Vector2Equals(unitVector, (Vector2){0,-1}))
				changeAnimation(&xIndex, &animation, UP_SLASH_RIGHT_ANIMATION);
			else if(Vector2Equals(unitVector, (Vector2){0,1}))
				changeAnimation(&xIndex, &animation, DOWN_SLASH_RIGHT_ANIMATION);
			isOnceAnimationPlaying = true;
		}

		//debug collider
		DrawRectangleLines(player.collider.x, player.collider.y, player.collider.width/SPRITE_DIVISOR, player.collider.height/SPRITE_DIVISOR, RED);
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up
	UnloadTexture(playerAtlas);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
