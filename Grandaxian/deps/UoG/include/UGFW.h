/**
*	@author	Jamie Stewart
*	@date	12/01/2016
*	@brief	This is the main entry point for access to the functionality provided by the 
*			Framework. This class needs to be included in your main.cpp
*			in order to get a simple 2d sprite game up and running.
**/

/**
* DLL Export Define
* - This is a define for building the framework as a DLL or as a static library
* - The syntax and meaning of this will be covered in class when we are covering
*   building a math library
**/
#ifdef UGFW_DLL_BUILD
#define UGFWAPI __declspec(UGFWAPI)
#elif UGFW_DLL
#define UGFWAPI __declspec(dllimport)
#else
#define UGFWAPI 
#endif

namespace UG
{


#pragma region SColor structure to describe a colour
	/**
	* @brief A Structure to describe a colour value as an unsigned int, and as individual 8 bit components (char values)
	**/
	struct SColour
	{
		/// @brief Default constructor for a SColour object
		SColour() {}
		/// @brief Constructor for an SColour obejct that constructs the SColour object from an unsigned integer value
		/// @param a_iColor the colour that the SColour will be created to specify
		SColour(unsigned int a_iColour) { argb.colour = a_iColour; }
		/// @brief Constructor to create an SColor object from four char values RGBA
		/// @param r unsigned char to represent the red component of an SColour object
		/// @param g unsigned char to represent the red component of an SColour object
		/// @param b unsigned char to represent the red component of an SColour object
		/// @param a unsigned char to represent the red component of an SColour object
		SColour(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
		{
			argb.colours.r = r;
			argb.colours.g = g;
			argb.colours.b = b;
			argb.colours.a = a;
		}
		/// and unnamed union that causes SColours member variables to use the same area of memory.
		/// this means that @param colour and @param colours occupy the same space, a change to one is 
		/// reflected in the other.
		union
		{
			unsigned int colour;
			struct SColours
			{
				unsigned char a, r, g, b;
			} colours;
		}argb;

	};
#pragma endregion

	//////////////////////////////////////////////////////////////////////////
	/// @brief Call this function to initialise the framework 
	/// @param a_iWidth the width in pixels that we want the screen to display for the width of the window
	/// @param a_height the width in pixels that we want the screen to display for the height of the window
	/// @param a_bFullscreen should this application run in a fullscreen mode.
	/// @param a_pWIndowTitle the title that we want to be displayed in the windows title bar
	//////////////////////////////////////////////////////////////////////////
	UGFWAPI int			Create(int a_iWidth, int a_iHeight, bool a_bFullscreen = false, const char* a_pWindowTitle = nullptr, int a_xPosition = 0, int a_yPosition = 0);
	//////////////////////////////////////////////////////////////////////////
	/// @brief This function should be called just prior to exiting your program
	///	it will unload all the components of the Framework that have been loaded
	//////////////////////////////////////////////////////////////////////////
	UGFWAPI void			Dispose();
	//////////////////////////////////////////////////////////////////////////
	/// @brief This function is to be called to update the current frame. 
	/// @return a boolean value indicating that the framework updated successfully
	//////////////////////////////////////////////////////////////////////////
	UGFWAPI bool			Process();
	
	//////////////////////////////////////////////////////////////////////////
	/// @brief This function is to be called to close the application
	///
	//////////////////////////////////////////////////////////////////////////
	UGFWAPI void			Close();

#pragma region Line Drawing functionality

	/// @brief Function for drawing a simple line
	/// @param a_iStartX the starting X Position for the Line
	/// @param a_iStartY the starting Y Position for the line
	/// @param a_iEndX the end X Position for the line
	/// @param a_iEndY the end Y Position for the line
	UGFWAPI void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY);
	/// @brief Function for drawing a line with a specific uniform colour
	/// @param a_iStartX the starting X Position for the Line
	/// @param a_iStartY the starting Y Position for the line
	/// @param a_iEndX the end X Position for the line
	/// @param a_iEndY the end Y Position for the line
	/// @param a_sColour a Vec4 representing the color for the line segment
	UGFWAPI void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sColour);
	/// @brief Function for drawing a line that starts as one colour and finishes as another colour.
	/// @param a_iStartX the starting X Position for the Line
	/// @param a_iStartY the starting Y Position for the line
	/// @param a_iEndX the end X Position for the line
	/// @param a_iEndY the end Y Position for the line
	/// @param a_sStartColour a Vec4 denoting the colour of the starting point of the line segment 
	/// @param a_sEndColour a Vec4 denoting the colour of the end point of the line segment 
	UGFWAPI void			DrawLine(int a_iStartX, int a_iStartY, int a_iEndX, int a_iEndY, SColour a_sStartColour, SColour a_sEndColour);

#pragma endregion
#pragma region Input handling functionality
	/// @brief Function returns weather a key is currently being pressed
	/// @param a_uiKey the unique identifier for the key, these are kept in an enum referenced from GLFW
	/// @return boolean value to represent wheather a key is held down or not
	UGFWAPI bool			IsKeyDown(unsigned int a_uiKey);
	/// @brief Function to get the position of the mouse, relative to the display window. Value is returned via function parameters passed in as reference arguments
	///			the returned values are given in pixels from the windows origin.
	/// @param a_uiMousePosX the reference argument that is to be given the value for the mouse cursors X position
	/// @param a_uiMousePosY the reference argument that is to be given the value for the mouse cursors Y position
	UGFWAPI void			GetMousePos(double& a_uiMousePosX, double& a_uiMousePosY);
	/// @brief Function to test if a mouse button is being pressed. 
	/// @param a_iMouseButton and integer identifier to identify the button to test for
	/// @return boolean value indicating if button is currently being triggered
	UGFWAPI bool			GetMouseButtonDown(int a_iMouseButton);
	/// @brief Function to Test to see if the mouse button has been released
	/// @param a_iMoustButton an integer identifier to identify the button to test for release
	/// @return boolean value to indicate weather button has just been released.
	UGFWAPI bool			GetMouseButtonReleased(int a_iMouseButton);
#pragma endregion
#pragma region Sprite creation and mainpulation functionality
	/// Function to create a Sprite
	/// @param a_pTextureName - the name of the texture that the sprite image resides in
	/// @param a_fWidth - the width that the sprite object is to be draw on the screen, this width is in pixels
	/// @param a_fHeight - the height that the sprite object is to be draw on the screen, this height is in pixels
	/// @param a_bDrawFromCenter - weather or not the sprite is to be drawn from the center position, defaults to true
	/// @param a_v4color - the colour tint that the sprite is to be given, defaults to white.
	/// @return the ID for the sprite, this is an unsigned integer.
	///
	UGFWAPI int			CreateSprite(const char* a_pTextureName, float a_fWidth, float a_fHeight, bool a_bDrawFromCenter = true, SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));
	/// Function to create a Sprite
	/// @param a_pTextureName - the name of the texture that the sprite image resides in
	/// @param a_fv2Size - the width and height of the sprite in Pixels as a float vector
	/// @param a_fv2Origin - The origin position for the sprite matrix, this dictates where the sprite will be drawn from (Top Left, Top Right etc)
	/// @param a_v4UVCoords - The UV Coordinates for the sprite as a float 4, components  1 & 2 correspond to origin point, components 3 & 4 correspond to end point.
	/// @param a_v4color - the colour tint that the sprite is to be given, defaults to white.
	/// @return the ID for the sprite, this is an unsigned integer.
	///
	UGFWAPI int			CreateSprite(const char* a_pTextureName, const float* a_fv2Size, const float* a_fv2Origin, const float* a_fv4UVCoords = nullptr, SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));
	/// Function to Duplicate an existing Sprite
	/// @param a_uiSpriteID This is ID of the sprite that is to be duplicated
	/// @return Returns a new sprite ID.
	UGFWAPI  int			DuplicateSprite(int a_uiSpriteID);
	/// This function is used to free up the memory associated with the Sprite ID passed in. After this function is called the sprite can no longer be drawn as it ceases to exist
	/// @param a_uiSpriteID the ID of the sprite to be destroyed.
	UGFWAPI void			DestroySprite(int a_uiSpriteID);

	/// This function is used to set the layer that the sprite will be rendered on, lower layers are rendered first
	/// @param a_iSpriteID the Unique ID that is used for the sprite
	/// @param a_uiLayer the layer the the sprite is to be moved to
	UGFWAPI void			SetSpriteLayer(int a_iSpriteID, const unsigned int& a_uiLayer);
	/// This function is used to Get the layer that the sprite is currently rendered on, lower layers are rendered first
	/// @param a_iSpriteID the Unique ID that is used for the sprite
	/// @param a_uiLayer an unsigned int reference that will be returned with the layer that the sprite is currently on 
	UGFWAPI void			GetSpriteLayer(int a_iSpriteID, unsigned int& a_uiLayer);
	/// This funciton moves a Sprite
	/// @param a_uiSpriteID the ID of the sprite to be moved
	/// @param a_fXPos the X co-ordinate that the sprite is to be moved to in screenspace
	/// @param a_fYPos the Y co-ordinate that the sprite is to be moved to in screenspace
	UGFWAPI void			MoveSprite( int a_uiSpriteID, float a_fXPos, float a_fYPos);
	/// This funciton moves a Sprite
	/// @param a_uiSpriteID the ID of the sprite to be moved
	/// @param a_fVec a float vector that contains two values (X & Y) to relocate the sprite to
	UGFWAPI void			MoveSprite( int a_uiSpriteID, const float* a_fVec);
	/// @brief This function gets the x y post of the Sprite
	/// @param a_uiSpriteID the unique identifier for the sprite
	/// @param a_fXPos the X co-ordinate that the sprite is at
	/// @param a_fYPos the Y co-ordinate that the sprite is at
	UGFWAPI void			GetSpritePosition( int a_uiSpriteID, float& a_xPos, float& a_YPos);
	/// @brief This function sets the 4x4 matrix for the sprite
	/// @param a_uiSpriteID the unique identifier for the sprite
	/// @param a_fm4 a float pointer indicating the start of a float array containing 16 values representing a 4 x 4 matrix
	UGFWAPI void			SetSpriteMatrix( int a_uiSpriteID, const float* a_fm4);
	/// @brief This function gets the 4x4 matrix for the sprite
	/// @param a_uiSpriteID the unique identifier for the sprite
	/// @param a_fm4 a float pointer indicating the start of a float array that is to be used to hold the 16 float values representing a 4 x 4 matrix
	UGFWAPI void			GetSpriteMatrix( int a_uiSpriteID, float* a_fm4);
	/// @brief Function to instruct a sprite to draw the screen
	/// @param a_uiSpriteID the unique identifier for the Sprite
	UGFWAPI void			DrawSprite( int a_uiSpriteID);
	/// @brief Function to stop a sprite from drawing to the screen
	/// @param a_uiSpriteID the unique identifier for the Sprite
	UGFWAPI void			StopDrawingSprite(int a_uiSpriteID);
	/// @brief Function to rotate a sprite by a_fRotation number of degrees, positive rotation is in a clockwise direction
	/// @param a_uiSpriteID the unique ID for the sprite
	/// @param a_fRotation the amount in degrees to rotate the sprite by, this value is non-cumulative E.G passing in a rotation value of 1 Degree every frame will not incrementally rotate the sprite each frame.
	UGFWAPI void			RotateSprite( int a_uiSpriteID, float a_fRotation);
	/// @brief This function sets the sprites UV coordinates
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_minUCoord this represents the starting point for the UV coordinates on the U Texture axis
	/// @param a_minVCoord this represents the starting point for the UV coordinates on the V Texture axis
	/// @param a_maxUCoord this represents the end point for the UV coordinates on the U Texture axis
	/// @param a_maxVCoord this represents the end point for the UV coordinates on the V Texture axis
	UGFWAPI void			SetSpriteUVCoordinates( int a_uiSpriteID, float a_minUCoord, float a_minVCoord, float a_maxUCoord, float a_maxVCoord);
	/// @brief This function sets the sprites UV coordinates
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_v4UVVoordinates a float vector4 representing the minimum and maximum UV Coordinates for the sprite, the X and Y components of the Vec4 correspond to the minimum UV coordinates
	///			the Z & W components correspond to the maximum UV Coordinates
	UGFWAPI void			SetSpriteUVCoordinates( int a_uiSpriteID, const float* a_fUVVec4);
	/// @brief This functions retrieves the Sprites current UV coordinates
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_minUCoord this represents the starting point for the UV coordinates on the U Texture axis
	/// @param a_minVCoord this represents the starting point for the UV coordinates on the V Texture axis
	/// @param a_maxUCoord this represents the end point for the UV coordinates on the U Texture axis
	/// @param a_maxVCoord this represents the end point for the UV coordinates on the V Texture axis
	UGFWAPI void			GetSpriteUVCoordinates( int a_uiSpriteID, float& a_minUCoord, float& a_minVCoord, float& a_maxUCoord, float& a_maxVCoord);
	/// @brief This functions retrieves the Sprites current UV coordinates
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_v4UVVoordinates a float vector4 representing the minimum and maximum UV Coordinates for the sprite, the X and Y components of the Vec4 correspond to the minimum UV coordinates
	///			the Z & W components correspond to the maximum UV Coordinates
	UGFWAPI void			GetSpriteUVCoordinates( int a_uiSpriteID, float* a_fUVVec4);
	/// @brief Used to set the colour for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_sColour the colour that the sprite is to be tinted with, this colour is passed in a vec4 with xyzw values mapping to rgba values
	UGFWAPI void			SetSpriteColour( int a_uiSpriteID, const SColour& a_sColour);
	/// @brief Used to get the current colour for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_sColour the colour that the sprite is tinted with, this colour is accessed via a vec4 with xyzw values mapping to rgba values
	UGFWAPI void			GetSpriteColour( int a_uiSpriteID, SColour& a_sColour);
	/// @brief Used to set the discard colour for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_sColour the colour that the sprite is to be tinted with, this colour is passed in a vec4 with xyzw values mapping to rgba values
	UGFWAPI void			SetSpriteDiscardColour(int a_uiSpriteID, const SColour& a_sColour);
	/// @brief Used to get the discard colour for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_sColour the colour that the sprite is tinted with, this colour is accessed via a vec4 with xyzw values mapping to rgba values
	UGFWAPI void			GetSpriteDiscardColour(int a_uiSpriteID, SColour& a_sColour);
	/// @brief Used to set the current scale for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions that the sprite is to be scaled by
	UGFWAPI void			SetSpriteScale( int a_uiSpriteID, const float& a_fSpriteWidth, const float& a_fSpriteHeight);
	/// @brief Used to get the current scale for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_v2Scale a Vec2 representing the scale in the X and Y Directions that the sprite is to currently scaled by
	UGFWAPI void			GetSpriteScale( int a_uiSpriteID, float& a_fSpriteWidth, float& a_fSpriteHeight);
	/// @brief Used to set the current blend mode for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_uiSourceFactor the blend factor for the source image
	/// @param a_uiDestinationFactor the blend mode that is to be used by the destination image.
	UGFWAPI void			SetSpriteBlendMode( int a_uiSpriteID, const unsigned int& a_uiSourceFactor, const unsigned int& a_uiDestinationFactor);
	/// @brief Used to get the current blend mode for the sprite
	/// @param a_uiSpriteID the unique identifier for the given sprite
	/// @param a_uiSourceFactor the current blend factor for the source image
	/// @param a_uiDestinationFactor the current blend mode that is used by the destination image.
	UGFWAPI void			GetSpriteBlendMode( int a_uiSpriteID, unsigned int& a_uiSourceFactor, unsigned int& a_uiDestinationFactor);

	UGFWAPI void			SetSpriteTexture(int a_iSpriteID, const char* a_pSpriteTextureName);
	UGFWAPI void			GetSpriteTexture(int a_iSpriteID, char* a_pSpriteTextureName);

#pragma endregion
#pragma region Font Rendering functionality
	/// @brief This function is used to display text output to the screen.  This text is renedered as series of simple sprites who's texture is an area of a font character sheet.
	/// @param a_v4Position this is the location that the origin of the String is to be drawn to the screen, this is a Vec4 to account for the underlying Mat4 calculations that take place in 
	///			the deeper parts of the font renderering functionality
	/// @param a_v4Color the color that the text is to be rendered in
	/// @param a_pTextToDisplay the text that is to be written to the screen. This is to be passed through as a pointer to a char array.
	UGFWAPI void			DrawString(const char* a_pText, int a_iXPos, int a_iYPos, float fSize = 1.0f, SColour a_sColour = SColour(0xFF, 0xFF, 0xFF, 0xFF));

	UGFWAPI void			AddFont(const char* a_pFontName);
	UGFWAPI void			SetFont(const char* a_pFontName);
	UGFWAPI void			RemoveFont(const char* a_pFontName);

#pragma endregion

	UGFWAPI void			ClearScreen();
	UGFWAPI void			SetBackgroundColor(SColour a_bgColor);

	UGFWAPI unsigned int	LoadTexture(const char* a_TextureName);
	UGFWAPI void			GetTextureSize(const char* a_TextureName, unsigned int& a_width, unsigned int& a_height);
	//////////////////////////////////////////////////////////////////////////
	/// @brief GetDeltaTime() a function to get the amount of time that has passed since the last time the CPU was polled
	///	@return the amount of time that has passed in seconds since the CPU was last polled.
	//////////////////////////////////////////////////////////////////////////
	UGFWAPI float			GetDeltaTime();


	//Functionality that we need in our base application
	UGFWAPI void			GetScreenSize(int& a_iWidth, int& a_iHeight);

	UGFWAPI void			SetScreenSize(const int& a_iWidth, const int& a_iHeight);

}