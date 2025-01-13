/*
	MMO Client/Server Framework using ASIO
	"Happy Birthday Mrs Javidx9!" - javidx9

	Videos:
	Part #1: https://youtu.be/2hNdkYInj4g
	Part #2: https://youtu.be/UbjxGvrDrbw

	License (OLC-3)
	~~~~~~~~~~~~~~~

	Copyright 2018 - 2020 OneLoneCoder.com

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions or derivations of source code must retain the above
	copyright notice, this list of conditions and the following disclaimer.

	2. Redistributions or derivative works in binary form must reproduce
	the above copyright notice. This list of conditions and the following
	disclaimer must be reproduced in the documentation and/or other
	materials provided with the distribution.

	3. Neither the name of the copyright holder nor the names of its
	contributors may be used to endorse or promote products derived
	from this software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
	"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
	LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
	A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
	HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
	SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
	LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
	DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
	THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
	(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

	Links
	~~~~~
	YouTube:	https://www.youtube.com/javidx9
	Discord:	https://discord.gg/WhwHUMV
	Twitter:	https://www.twitter.com/javidx9
	Twitch:		https://www.twitch.tv/javidx9
	GitHub:		https://www.github.com/onelonecoder
	Homepage:	https://www.onelonecoder.com

	Author
	~~~~~~
	David Barr, aka javidx9, ©OneLoneCoder 2019, 2020

*/

#ifdef DEBUG
bool debug = true;
#endif // DEBUG

#define _SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING
#define OLC_PGE_APPLICATION
#define _CRT_SECURE_NO_WARNINGS
//#define OLC_PGEX_GRAPHICS3D

#include <sstream>
#include <iostream>
#include <chrono>
#include <strstream>
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstring> // Include this for strncpy_s
#include <vector>
#include <unordered_map>
#include <bitset>
#include "olcPixelGameEngine.h"
#include "olcPGEX_Graphics3D.h"
#include "olcPGEX_Network.h"
#include "../QuartoServer/quartoGame.h"
//#include "../quartoNetworkVisual/MMO_Common.h"
#include "olcPGEX_TransformedView.h"

// Enum to identify each button
enum class ButtonID {
	BUTTON_PING,
	BUTTON_CREATE,
	BUTTON_SEND,
	BUTTON_JOIN,
	BUTTON_QUARTO
};
// Enum to identify messages
enum class CustomMsgTypes : uint32_t
{
	ServerAccept,
	ServerDeny,
	ServerPing,
	MessageAll,
	ServerMessage,
	CreateGame,
	GameCreated,
	GameCreatedByMe,
	GameNotCreated,
	GameJoined,
	GameJoinedByPlayer,
	JoinGame,
	GameNotJoined,
	UpdateBuffer,
	PlayerRegistration,
	ClientAssignID,
	Game_AddPlayer,
	Game_one,
	Game_two,
	Game_three,
	Game_four,
	Game_quarto,
	Game_over
};

// Function to convert an integer to a 4-bit binary string
std::string to4BitBinary(int number)
{
	if (number < 0 || number > 15)
	{
		throw std::out_of_range("Number out of range for 4-bit representation");
	}

	// Use bitset to convert the number to binary and then extract the string
	std::bitset<4> binary(number);
	return binary.to_string();
};

class CustomClient : public olc::net::client_interface<CustomMsgTypes>
{
public:
	std::vector <std::string> gamesBuffer;
	
	void PingServer()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerPing;
		// Caution with this...
		std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
		msg << timeNow;
		Send(msg);
	}

	void MessageAll(std::string txt)
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::MessageAll;
		msg << txt;
		Send(msg);
	}

	void CreateGame()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::CreateGame;
		Send(msg);
		std::cout << "Creating Game" << std::endl;
	}

	void IterateBuffer(std::vector<std::string> buffer) {
		for (int b = 0; b < buffer.size(); b++) {

		}
	}

	void JoinGame()
	{
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::JoinGame;
		Send(msg);
		std::cout << "Joining Game: " << std::endl;
	}

	void ChooseTile(uint32_t id, int num) {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::Game_one;
		msg << num;
		msg << id;
		Send(msg);
	}

	void PlaceTile(uint32_t id, int num, int place) {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::Game_three;
		msg << place;
		msg << num;
		msg << id;
		Send(msg);
	}

	void Quarto(uint32_t id) {
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::Game_quarto;
		msg << id;
		Send(msg);
	}
};

class CustomClientGUI : public olc::PixelGameEngine
{
private:
	CustomClient c;

	bool inLobby, inGame, start;
	std::string textInput;
	int textLength = 0;
	std::vector <std::string> messageBuffer;
	std::vector <uint32_t> gamesBuffer;
	float fRotationX, fRotationY, fRotationYOffSet; // Rotation angle in radians
	olc::vf2d vPrevMousePos; // Previous mouse position
	olc::GFX3D::mat4x4 matTrans, matRotX, matRotY, matRot;

	struct Button {
		olc::vf2d pos;
		olc::vf2d size;
		ButtonID id;
		std::string text;
	};

	std::unordered_map<uint32_t, sPlayerDescription> mapPlayers;
	uint32_t nPlayerID = 0;
	std::string name = "";

	std::vector<Button> buttons;
	olc::vf2d buttonSize = { 70, 30 };
	Button buttonQuarto = { { 650, 120 }, { buttonSize.x, buttonSize.y }, ButtonID::BUTTON_QUARTO, "Quarto!" };

	bool IsMouseInButton(const Button& button) {
		// Get mouse position
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();

		// Check if mouse position is within button boundaries
		return mouseX >= button.pos.x && mouseX <= button.pos.x + button.size.x &&
			mouseY >= button.pos.y && mouseY <= button.pos.y + button.size.y;
	}

	void drawBoardPlan() {
		float xFloat = ScreenWidth() - 140;
		float yFloat = ScreenHeight() - 140;
		float tFloat = 25;

		for (int i = 0; i < 16; i++) {
			int x = i % 4;
			int y = i / 4;
			// Rotate the board 180 degrees
			int rotatedX = 3 - x;
			int rotatedY = 3 - y;

			int mouseX = GetMouseX();
			int mouseY = GetMouseY();

			if (pBoard[i].empty) {
				DrawStringDecal({ (xFloat + (rotatedX * tFloat) + (tFloat / 3)), (yFloat + (rotatedY * tFloat) + (tFloat / 3)) }, std::to_string(pBoard[i].sTile));
				DrawRectDecal({ (xFloat + (rotatedX * tFloat)), (yFloat + (rotatedY * tFloat)) }, { tFloat, tFloat }, olc::YELLOW);
			}
			else {
				DrawStringDecal({ (xFloat + (rotatedX * tFloat) + (tFloat / 3)), (yFloat + (rotatedY * tFloat) + (tFloat / 3)) }, std::to_string(pBoard[i].sTile), olc::RED);
				DrawRectDecal({ (xFloat + (rotatedX * tFloat)), (yFloat + (rotatedY * tFloat)) }, { tFloat, tFloat }, olc::RED);
			}
		}
	}

	int choosePossition() {
		float xFloat = ScreenWidth() - 140;
		float yFloat = ScreenHeight() - 140;
		float tFloat = 25;
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();

		int pos;
		while (!GetMouse(0).bReleased) {
			for (int i = 0; i < 16; i++) {
				int y = i % 4;
				int x = i / 4;
				if (pBoard[i].empty) {
					DrawStringDecal({ (xFloat + (x * tFloat) + (tFloat / 3)), (yFloat + (y * tFloat) + (tFloat / 3)) }, std::to_string(i));

					if (mouseX >= xFloat && mouseX <= xFloat + tFloat
						&& mouseY >= yFloat && mouseY <= yFloat + tFloat) {
						DrawRectDecal({ (xFloat + (x * tFloat)), (yFloat + (y * tFloat)) }, { tFloat, tFloat }, olc::BLUE);
						pos = i;
					}
					else {
						DrawRectDecal({ (xFloat + (x * tFloat)), (yFloat + (y * tFloat)) }, { tFloat, tFloat }, olc::YELLOW);
					}

				}
				else {
					DrawStringDecal({ (xFloat + (x * tFloat) + (tFloat / 3)), (yFloat + (y * tFloat) + (tFloat / 3)) }, std::to_string(i), olc::RED);
					DrawRectDecal({ (xFloat + (x * tFloat)), (yFloat + (y * tFloat)) }, { tFloat, tFloat }, olc::RED);
				}
			}
		}
		return pos;
	}

	bool IsMouseOverTile(int tile, float mouseWorldX, float mouseWorldY, float fTileSize) {
		// Calculate the row and column of the tile
		int row = tile / 8;  // 8 columns
		int col = tile % 8;
		float fGridStartX = -3.5f;  // Horizontal start position for the grid
		float fGridStartY = 4.5f;   // Vertical start position for the grid (move up on the screen)
		
		// Get the position of the tile
		float fXoffset = fGridStartX + col * fTileSize;
		float fYoffset = fGridStartY - row * fTileSize;

		// Calculate tile boundaries
		float left = fXoffset - fTileSize / 2;
		float right = fXoffset + fTileSize / 2;
		float top = fYoffset - fTileSize / 2;
		float bottom = fYoffset + fTileSize / 2;

		// Check if mouse position falls within boundaries
		return mouseWorldX >= left && mouseWorldX <= right &&
			mouseWorldY >= top && mouseWorldY <= bottom;
	}

	bool mouseInMap(int pos) {
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();

		// Calculate original row and column from the position
		int row = pos / 4;  // Row number (0-3)
		int col = pos % 4;  // Column number (0-3)

		// Perform 180-degree mirroring
		int mirroredRow = 3 - row;
		int mirroredCol = 3 - col;

		// Calculate tile position based on mirrored coordinates
		int xOffsetTiles = ScreenWidth() - 140;
		int yOffsetTiles = ScreenHeight() - 140;
		int xPos = 25 * mirroredCol + xOffsetTiles;
		int yPos = 25 * mirroredRow + yOffsetTiles;

		// Check if mouse is within the tile bounds
		if (mouseX >= xPos && mouseX <= xPos + 25
			&& mouseY >= yPos && mouseY <= yPos + 25) {
			return true;
		}
		return false;
	}

	bool IsMouseInTile(int sTile) {
		// Get mouse position
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();
		int row = sTile / 8;  // Row number (2 rows)
		int col = sTile % 8;  // Column number (8 columns)
		int xOffsetTiles = ScreenWidth() / 8;
		int yOffsetTiles = ScreenHeight() / 8;
		int xPos = tileSize * col + xOffsetTiles;
		int yPos = tileSize * row + yOffsetTiles;

		if (mouseX >= xPos && mouseX <= xPos + tileSize
			&& mouseY >= yPos && mouseY <= yPos + tileSize) {
			return true;
		}
		else {
			return false;
		}
	}

	void DrawButton(const Button& button, bool mouseOver) {
		// Determine button color based on mouse state
		olc::Pixel buttonColor = mouseOver ? olc::GREY : olc::DARK_GREY;

		// Draw button rectangle
		FillRect(button.pos, button.size, buttonColor);

		// Draw button text
		DrawString(button.pos + olc::vf2d(10, 10), button.text, mouseOver ? olc::BLACK : olc::WHITE);
	}

	void OnButtonClick(const Button& button) {
		// Handle button click action
		// For example, you can print a message
		//std::cout << "Button clicked at position: " << button.text << std::endl;
		switch (button.id) {
		case ButtonID::BUTTON_PING:
			c.PingServer();
			break;
		case ButtonID::BUTTON_CREATE:
			c.CreateGame();
			break;
		case ButtonID::BUTTON_SEND:
			if (!empty(textInput)) {
				pushMessage(textInput);
				c.MessageAll(textInput);
				textInput = "";
			}
			break;
		case ButtonID::BUTTON_JOIN:
			c.JoinGame();
			break;
		case ButtonID::BUTTON_QUARTO:
			c.Quarto(q->gameID);
			
		default:
			break;
		}
	}
	
	// Lobby chat
	void handleChat() {
		// Handle keyboard input for text entry
		if (GetKey(olc::Key::ESCAPE).bPressed) {
			// Handle pressing Enter key
			std::cout << "Exiting game" << std::endl;
			
			// handle ending
		}
		else if (GetKey(olc::Key::ENTER).bPressed) {
			// Handle pressing Enter key
			if (!empty(textInput)) {
				pushMessage(textInput);
				c.MessageAll(textInput);
				textInput = "";
			}
		}
		else if (GetKey(olc::Key::SPACE).bPressed) {
			// Handle pressing Space key
			textInput += " ";
		}
		else if (GetKey(olc::Key::BACK).bPressed) {
			// Handle pressing BackSpace key
			if (!empty(textInput)) {
				textInput.pop_back();
			}
		}
		else {
			// Handle typing characters
			for (int i = 0; i < 256; i++) {
				if (GetKey(static_cast<olc::Key>(i)).bPressed) {
					if (i >= 0 && i <= 35)
					{
						textInput += table[i - 1];
					}
				}
			}
		}
		// Draw the text content within the text input rectangle
		if (!empty(textInput)) {
			DrawString(input_rectPos + olc::vf2d(4, 4), textInput, olc::WHITE);
		}
		// Draw messages within the message rectangle
		for (int i = 0; i < messageBuffer.size(); i++) {
			DrawString(rectPos.x + 5, rectPos.y + (10 * i), messageBuffer[i]);
		}

		// Draw games ID within the right rectangle
		for (int j = 0; j < gamesBuffer.size(); j++) {
			DrawString(games_rectPos.x + 5, games_rectPos.y + (10 * j), std::to_string(gamesBuffer[j]));
		}
	}

	// Lobby buttons
	void handleButtons() {
		// Iterate over buttons
		for (auto& button : buttons) {
			// Check if the mouse cursor is over the button
			bool mouseOverButton = IsMouseInButton(button);

			// Draw the button
			DrawButton(button, mouseOverButton);

			// Handle button click
			if (mouseOverButton && GetMouse(0).bReleased) {
				// Button clicked, perform action
				OnButtonClick(button);
			}
		}
	}

	// hover over free tiles
	void handleTileSpaces() {
		// Iterate over tiles
		for (auto& tile : pBoard) {
			if (IsMouseInTile(tile.sTile)) {
				std::string tileStr = std::to_string(tile.sTile);
				DrawString(120, 120, tileStr);
			}
		}
	}

	bool showButton(int situation) {
		bool show = true;
		if (situation % 2 == 0) {
			// Draw QUARTO! button
			show = IsMouseInButton(buttonQuarto);
			DrawButton(buttonQuarto, show);

			// Handle button click
			if (show && GetMouse(0).bReleased) {
				// Button clicked, perform action
				OnButtonClick(buttonQuarto);
			}
		}
		return true;
	}

	// each players turn - 4 situations
	int turn(int situation) {
		std::string printableString;
		int tilePos = 0;
		float xFloat = ScreenWidth() - 140;
		float yFloat = ScreenHeight() - 140;
		float tFloat = 25;
		int mouseX = GetMouseX();
		int mouseY = GetMouseY();
		int col, row;
		bool mouseOverButton;
		switch (situation) {
		case 0:
			// Choose tile for oponent
			DrawString(50, 45, "choose tile for oponent or call quarto!");

			for (int i = 0; i < 16; i++) {
				if (IsMouseInTile(i) && q->tiles[i] == 0 ) {
					int row = i / 8;  // Row number (2 rows)
					int col = i % 8;  // Column number (8 columns)
					int xOffsetTiles = ScreenWidth() / 8;
					int yOffsetTiles = ScreenHeight() / 8;
					float xFloat = tileSize * col + xOffsetTiles;
					float yFloat = tileSize * row + yOffsetTiles;
					float tFloat = tileSize;

					DrawRectDecal({ (xFloat), (yFloat) }, { tFloat, tFloat }, olc::YELLOW);
					if (GetMouse(0).bReleased) {
						c.ChooseTile(q->gameID, i);
					}
				}
			}
			break;

		case 1:
			// Waiting for chosen tile
			DrawString(50, 45, "wait for oponent to choose tile");
			break;

		case 2:
			// Placing tile on board
			printableString = "Place tile ";
			printableString += std::to_string(q->activeTile);
			printableString += " or call quarto!";
			DrawString(50, 45, printableString);
			tilePos = 1;

			for (int i = 0; i < 16; i++) {
				row = i / 4;
				col = i % 4;

				// Apply 180-degree mirroring to correct tile placement
				int mirroredRow = 3 - row;
				int mirroredCol = 3 - col;
				int mirroredIndex = mirroredRow * 4 + mirroredCol;

				// Check if the mouse is over the mirrored tile
				if (mouseInMap(mirroredIndex) && pBoard[mirroredIndex].empty) {
					// Highlight the tile to show it's selectable
					DrawRectDecal({ (xFloat + (col * tFloat)), (yFloat + (row * tFloat)) }, { tFloat, tFloat }, olc::BLUE);

					// Handle placing the tile
					if (GetMouse(0).bReleased) {
						c.PlaceTile(q->gameID, q->activeTile, mirroredIndex);
						pBoard[mirroredIndex].empty = false;
						pBoard[mirroredIndex].sTile = q->activeTile;
						q->activeTile = -1;
					}
				}
				tilePos++;
			}
			break;

		case 3:
			// Waiting for placing tile
			DrawString(50, 45, "waiting for oponent to place tile");
			break;
		}
		return situation;
	}

	// Push string into messageBuffer
	void pushMessage(std::string txt) {
		messageBuffer.push_back(txt);
		if (messageBuffer.size() > maxMessages) {
			messageBuffer.erase(messageBuffer.begin());
		}
	}

	// Push game into local Buffer
	bool pushGame(uint32_t gameID) {
		// Check if the game already exists
		for (const auto& existingGameID : gamesBuffer) {
			if (existingGameID == gameID) {
				std::cout << "Game " << gameID << " already exists in buffer\n";
				return false; // Game already exists
			}
		}

		// Check if max messages limit is reached
		if (gamesBuffer.size() >= maxMessages) {
			pushMessage("Too many games created, join one!");
			return false; // Cannot add more games
		}

		// Add the new game ID to the buffer
		gamesBuffer.push_back(gameID);
		pushMessage("Game " + std::to_string(gameID) + " created\n");

		return true; // Successfully added the game
	}

public:
	olc::Sprite* spr_logo = nullptr;
	olc::Decal* dec_logo = nullptr;
	std::vector<olc::Sprite*> spr_tiles;
	std::vector<olc::Decal*> dec_tiles;
	olc::Sprite* spr_cursor = nullptr;
	olc::Decal* dec_cursor = nullptr;
	// Define the position and size of the rectangle
	int buttonSpace = 20;
	//olc::vf2d buttonSize = { 70, 30 };  // Defined a lot above
	olc::vf2d rectPos = { 30.0f, 60.0f };
	olc::vf2d rectSize = { 300.0f, 200.0f };
	olc::vf2d tileRectPos = { 400.0f, 90.0f };
	olc::vf2d tileRectSize = { 50.0f, 50.0f };
	olc::vf2d buttonPos = { rectPos.x + rectSize.x + buttonSpace, rectPos.y };
	olc::vf2d input_rectPos = { rectPos.x, rectPos.y+rectSize.y+2 };
	olc::vf2d input_rectSize = { rectSize.x, 14 };
	olc::vf2d games_rectPos = { rectPos.x + rectSize.x + buttonSize.x + (buttonSpace * 2), rectPos.y }; //251
	olc::vf2d games_rectSize = { rectSize.x, rectSize.y };
	olc::GFX3D::mesh board;
	std::unordered_map<int, olc::GFX3D::mesh> tileMap;
	olc::GFX3D::mesh tile1000;
	olc::GFX3D::mesh tile1001;
	olc::GFX3D::mesh tile1010;
	olc::GFX3D::mesh tile1011;
	olc::GFX3D::mesh tile1100;
	olc::GFX3D::mesh tile1101;
	olc::GFX3D::mesh tile1110;
	olc::GFX3D::mesh tile1111;
	olc::GFX3D::PipeLine renderer;

	olc::GFX3D::vec3d vUp = { 0, -1,  0 };
	olc::GFX3D::vec3d vEye = { 0,  1, -5 }; // 0, 3, -5
	olc::GFX3D::vec3d vLookDir = { 0,  0,  1 };

	float fTheta = 0;
	// iterate over tile1111 objects in the playing board
	int nStartX = 0;
	int nEndX = boardWidth;
	int nStartY = 0;
	int nEndY = boardLength;
	int situation = 0; // who is on turn in game

	struct sCell {
		bool empty = false;
		int sTile = 0;
	};
	
	int boardWidth = 4;
	int boardLength = 4;
	int tileIterator = boardWidth * boardLength;
	std::vector<sCell> pBoard;
	bool what;
	int tileSize = 60;

	olc::Sprite* boardTex, * boardTex2;
	
	// create instance of quarto game
	QuartoGame* q;
	
	// Calculate the maximum number of messages that can fit in the rectangle
	int maxMessages = (int)(rectSize.y / 10); // Assuming each message is 10 pixels high

	std::string justMessage = "";
	const char table[37] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

	void initBoard(std::vector<sCell>& pBoard) {
		
		for (int x = 0; x < boardWidth; x++) {
			for (int y = 0; y < boardLength; y++) {
				sCell cell;
				cell.empty = true;
				//cell.sTile = y * boardWidth + x;
				cell.sTile = 0;
				pBoard.push_back(cell);
			}
		}
	}

	// Method to initialize tiles
	void initTileImages() {
		for (int i = 0; i <= 15; i++) {
			std::string strTileNumber = to4BitBinary(i); // Convert to a unique identifier
			std::string strTileName = strTileNumber + ".png";
			spr_tiles.push_back(new olc::Sprite(strTileName));
			dec_tiles.push_back(new olc::Decal(spr_tiles.back()));
		}
	}

	CustomClientGUI() : c()
	{
		sAppName = "Quarto Client GUI";
	}
	
	void hoverOverTile() {
		olc::vf2d vCurrentMousePos = GetMousePos();
	}

	void CameraControls(float fElapsedTime)
	{
		if (GetKey(olc::Key::UP).bHeld) vEye.y -= 8.f * GetElapsedTime();
		if (GetKey(olc::Key::DOWN).bHeld) vEye.y += 8.f * GetElapsedTime();
		if (GetKey(olc::Key::RIGHT).bHeld) vEye.x += 8.f * GetElapsedTime();
		if (GetKey(olc::Key::LEFT).bHeld) vEye.x -= 8.f * GetElapsedTime();

		// Get the current mouse position
		olc::vf2d vCurrentMousePos = GetMousePos();

		// Calculate the mouse movement (difference in position)
		olc::vf2d vMouseDelta = vCurrentMousePos - vPrevMousePos;

		// Update previous mouse position
		vPrevMousePos = vCurrentMousePos;

		// Zoom in/out with mouse wheel
		if (GetMouseWheel() != 0) {
			if (GetMouseWheel() > 0) {
				vEye.z = vEye.z + (GetMouseWheel() / 480.0);
			}
			else if (GetMouseWheel() < 0) {
				vEye.z = vEye.z + (GetMouseWheel() / 480.0);
			}

			if (vEye.z < -7.5) {
				vEye.z = -7.5;
			}
			if (vEye.z > -3.75) {
				vEye.z = -3.75;
			}
		}

		// Rotate the view if the right mouse button is held down
		if (GetMouse(olc::Mouse::RIGHT).bHeld)
		{
			// Calculate rotation based on horizontal mouse movement
			float fRotationSpeed = 0.3f; // Adjust rotation speed as needed
			fRotationX += vMouseDelta.y * fRotationSpeed * fElapsedTime;
			if (fRotationX >= 0.12) {
				fRotationX = 0.12;
			}
			else if (fRotationX <= -1.2) {
				fRotationX = -1.2;
			}
			float maxTilt = 0.8;
			fRotationY += vMouseDelta.x * fRotationSpeed * fElapsedTime;
			if (fRotationY >= maxTilt) {
				fRotationY = maxTilt;
			}
			else if (fRotationY <= -maxTilt) {
				fRotationY = -maxTilt;
			}
		}
	}

	bool OnUserCreate() override
	{
		spr_logo = new olc::Sprite("pge2_logo.png");
		dec_logo = new olc::Decal(spr_logo);

		initTileImages();

		// Define button positions and sizes
		buttons.push_back({ {buttonPos.x, buttonPos.y}, {buttonSize.x, buttonSize.y}, ButtonID::BUTTON_PING, "Ping!" });
		buttons.push_back({ {buttonPos.x, buttonPos.y + buttonSize.y + buttonSpace}, {buttonSize.x, buttonSize.y}, ButtonID::BUTTON_CREATE, "Create\nGame" });
		buttons.push_back({ {buttonPos.x, buttonPos.y + ((buttonSize.y + buttonSpace) * 2)}, {buttonSize.x, buttonSize.y}, ButtonID::BUTTON_JOIN, "Join" });
		buttons.push_back({ {buttonPos.x, buttonPos.y + ((buttonSize.y + buttonSpace) * 3)}, {buttonSize.x, buttonSize.y}, ButtonID::BUTTON_SEND, "Send" });

		// Initialize text input variables
		textInput = "";
		inLobby = false;
		inGame = false;
		start = true;

		board.LoadFromObjectFile("board.obj");

		std::string tileFileName = "";
		// load tiles from object files
		for (int t = 8; t < 16; t++) {
			tileFileName = to4BitBinary(t) + ".obj";
			tileMap[t].LoadFromObjectFile(tileFileName);
		}

		olc::GFX3D::ConfigureDisplay();

		renderer.SetProjection(100.0f, (float)ScreenHeight() / (float)ScreenWidth(), 0.1f, 1000.0f, 0.0f, 0.0f, ScreenWidth(), ScreenHeight());

		fRotationX = 0.0f;
		fRotationYOffSet = -1.6f;
		fRotationYOffSet = 0.0f;
		fRotationY = 0.0f + fRotationYOffSet;
		vPrevMousePos = GetMousePos();

		initBoard(pBoard);
		
		c.Connect("127.0.0.1", 60000);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::DARK_BLUE);

		// OLC logo for copyright
		DrawDecal({ 0.0f, 0.0f }, dec_logo, { 0.25f, 0.25f });
		
		// Initial start of player before lobby
		if (start) {
			start = false;
			inLobby = true;
		}
		
		// Player in lobby
		if (inLobby) {
			name = std::to_string(mapPlayers[nPlayerID].nUniqueID);
			DrawString(50, 45, name);
			DrawRectDecal(rectPos, rectSize);
			DrawRectDecal(input_rectPos, input_rectSize);
			DrawRectDecal(games_rectPos, games_rectSize);

			// Players objects
			mapPlayers[nPlayerID];

			// Handle buttons
			handleButtons();

			// Keyboard input
			handleChat();
			
			// Get the mouse's current position
			olc::vi2d mousePos = GetMousePos();
		}
		
		// Games starts
		if(inGame){
			if (q->start) {

				// Draw the game graphics and run the game logic
				if (!q->isOver) {
					CameraControls(fElapsedTime);
					// clear both window and depth buffer
					Clear(olc::DARK_BLUE);
					olc::GFX3D::ClearDepth();

					drawBoardPlan();

					// Show quartoButton dependable on situation in game
					showButton(situation);

					// prepare view transform
					olc::GFX3D::vec3d vLookTarget = olc::GFX3D::Math::Vec_Add(vEye, vLookDir);
					renderer.SetCamera(vEye, vLookTarget, vUp);

					// prepare world transform and render board object
					//olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MakeRotationZ(fRotationY);
					matRotX = olc::GFX3D::Math::Mat_MakeRotationX(fRotationX);
					matRotY = olc::GFX3D::Math::Mat_MakeRotationY(fRotationY);
					//matRot = olc::GFX3D::Math::Mat_MultiplyMatrix(matRotX, matRotY);
					olc::GFX3D::mat4x4 matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(matRotX, matRotY);
					renderer.SetTransform(matWorld);

					// Set the color as the material color before rendering
					olc::Sprite* sandTexture = new olc::Sprite("desert_04.png");
					olc::Sprite* woodTexture = new olc::Sprite("brown_wood_05.png");

					// Set material color to a wood-like brown
					//olc::Pixel woodColor = olc::Pixel(139, 69, 19);  // Dark wood brown (SaddleBrown)

					// Set the color as the material color before rendering
					//renderer.SetMaterialColor(woodColor);
					//renderer.SetTexture(sandTexture);

					//renderer.Render(board.tris, olc::GFX3D::RENDER_TEXTURED);
					renderer.Render(board.tris, olc::GFX3D::RENDER_FLAT);
					
					turn(situation);
					float tFloat = tileSize;
					float scaleX = tileSize / 1000.0f;
					float scaleY = tileSize / 1000.0f;
					float scale = std::min(scaleX, scaleY);

					for (int q = 0; q < 16; q++) {
						int tilePos = abs(q - 15);
						float mouseScreenX = GetMouseX(); // Screen-space mouse X
						float mouseScreenY = GetMouseY(); // Screen-space mouse Y

						// check if board space is not empty render the tile 
						if (!pBoard[tilePos].empty) {
							// use some local var's to easily tinker with the location of the objects
							float fTileSize = 1.0f;
							float fXoffset = -1.5f;
							float fYoffset = -1.5f;
							// Calculate position in 4x4 grid based on sTile
							int gridX = tilePos % 4;  // Column index in 4x4 grid
							int gridY = tilePos / 4;  // Row index in 4x4 grid

							// Calculate the world position using fTileSize and offsets
							float worldPosX = gridX * fTileSize + fXoffset;
							float worldPosY = gridY * fTileSize + fYoffset;

							// Apply the calculated position to the transformation matrix
							matTrans = olc::GFX3D::Math::Mat_MakeTranslation(worldPosX, 0.0f, worldPosY);
							//matTrans = olc::GFX3D::Math::Mat_MakeTranslation(pBoard[tile].sTile * fTileSize + fXoffset, 0.0f, pBoard[tile].sTile * fTileSize + fYoffset);
							matRotX = olc::GFX3D::Math::Mat_MakeRotationX(fRotationX);
							matRotY = olc::GFX3D::Math::Mat_MakeRotationY(fRotationY);
							matRot = olc::GFX3D::Math::Mat_MultiplyMatrix(matRotX, matRotY);
							matWorld = olc::GFX3D::Math::Mat_MultiplyMatrix(matTrans, matRot);
							renderer.SetTransform(matTrans);
							renderer.SetTransform(matWorld);

							//std::cout << "render this: " << pBoard[y * boardWidth + x].sTile << std::endl;

							if (pBoard[tilePos].sTile <= 7) {
								//renderer.Render(tileMap[tile + 8].tris, olc::GFX3D::RENDER_FLAT);
								renderer.Render(tileMap[pBoard[tilePos].sTile + 8].tris, olc::GFX3D::RENDER_FLAT);
							}
							else {
								renderer.SetTexture(woodTexture);
								renderer.Render(tileMap[pBoard[tilePos].sTile].tris, olc::GFX3D::RENDER_TEXTURED);
							}


							//renderer.Render(tile1111.tris, olc::GFX3D::RENDER_WIRE);
						}
					}
					for (int o = 0; o < 16; o++) {
						if (q->tiles[o] == 0) {
							// Rendeer flat grid for choosing tiles
							int row = o / 8;  // Row number (2 rows)
							int col = o % 8;  // Column number (8 columns)
							int xOffsetTiles = ScreenWidth() / 8;
							int yOffsetTiles = ScreenHeight() / 8;
							float xFloat = tileSize * col + xOffsetTiles;
							float yFloat = tileSize * row + yOffsetTiles;

							DrawDecal({ (xFloat), (yFloat) }, dec_tiles[o], { scale, scale });
							DrawString(tileSize * col + xOffsetTiles, tileSize * row + yOffsetTiles, std::to_string(o), olc::WHITE);
						}
					}

				}
			}
			else {
				DrawString(50, 50, "Waiting for other player ...");
			}
		}
		
		// Handle incoming messages from the server
		if (c.IsConnected())
		{
			if (!c.Incoming().empty())
			{
				auto msg = c.Incoming().pop_front().msg;

				switch (msg.header.id)
				{
				case CustomMsgTypes::ServerAccept:
				{
					uint32_t id;
					std::string txt = "Server Accepted Connection\n";
					std::cout << txt << std::endl;
					pushMessage(txt);
					//msg >> id;
					//std::cout << id << " - - - - " << std::endl;
					//mapPlayers[id].nUniqueID = id;
					olc::net::message<CustomMsgTypes> msg;
					msg.header.id = CustomMsgTypes::PlayerRegistration;
					msg << mapPlayers[nPlayerID];
					c.Send(msg);
				}
				break;

				case CustomMsgTypes::ServerPing:
				{
					// Server has responded to a ping request
					std::chrono::system_clock::time_point timeNow = std::chrono::system_clock::now();
					std::chrono::system_clock::time_point timeThen = std::chrono::system_clock::now(); // Initialize with default value
					msg >> timeThen; // Extract the time from the message
					std::ostringstream oss;
					oss << "Ping: " << std::chrono::duration<double>(timeNow - timeThen).count() << "\n";
					std::string pingString = oss.str();
					std::cout << pingString;
					pushMessage(pingString);
				}
				break;

				case CustomMsgTypes::ServerMessage:
				{
					std::string stringMessage, userText, ID;
					uint32_t clientID;
					//extract data from message
					//msg >> gamesBuffer;
					msg >> userText;
					msg >> clientID;
					ID = std::to_string(clientID);
					stringMessage = ID + ": " + userText;

					pushMessage(stringMessage);
					//std::cout << stringMessage << "\n";
				}
				break;

				case CustomMsgTypes::GameCreated:
				{
					uint32_t clientID;
					int gamesInt;

					msg >> clientID;  // Reading client ID
					msg >> gamesInt;  // Reading the number of games

					std::cout << "Games created by client " << clientID << ": " << gamesInt << std::endl;
					
					uint32_t tempID;  // Use uint32_t for tempID
					for (int b = 0; b < gamesInt; b++) {
						
						msg >> tempID;

						// Print the game ID being processed
						std::cout << "Processing Game ID: " << tempID << std::endl;

						// Push the game ID to the local games buffer
						if (pushGame(tempID)) {
							std::cout << "Game " << tempID << " added to the local buffer." << std::endl;
						}
						else {
							std::cout << "Failed to add Game " << tempID << " to the local buffer. It may already exist." << std::endl;
						}
					}
				}
				break;


				case CustomMsgTypes::GameCreatedByMe:
				{
					uint32_t game;
					msg >> game;
					std::cout << "You host game " << game << std::endl;
					pushGame(game);
					q = new QuartoGame(game);
					inLobby = false;
					inGame = true;
					start = false;
					situation = 0;
				}
				break;

				case CustomMsgTypes::GameNotCreated:
				{
					std::string str;
					msg >> str;
					pushMessage(str);  // Ensure pushMessage is defined to handle std::string
				}
				break;

				case CustomMsgTypes::GameJoined:
				{
					uint32_t clientID;
					msg >> clientID;

					std::string stringMessage = "You joined game " + std::to_string(clientID);
					std::cout << stringMessage << std::endl;
					pushMessage(stringMessage);
					q = new QuartoGame(clientID);
					q->player2 = clientID;
					q->start = true;
					inLobby = false;
					inGame = true;
					situation = 1;
				}
				break;

				case CustomMsgTypes::GameJoinedByPlayer:
				{
					uint32_t clientID;
					msg >> clientID;

					std::cout << "Player " << clientID << " joined your game." << std::endl;
					q->player2 = clientID;
					q->start = true;

				}
				break;

				case CustomMsgTypes::GameNotJoined:
				{
					std::string txt;
					msg >> txt;
					std::cout << txt << std::endl;
					pushMessage(txt);
				}
				break;

				case CustomMsgTypes::UpdateBuffer:
				{
					uint32_t gameID;
					int gamesInt;
					msg >> gamesInt;
					gamesBuffer.clear();  // Clear buffer regardless of count

					for (int b = 0; b < gamesInt; b++) {
						msg >> gameID;
						pushGame(gameID);  // Ensure the correct type is pushed
					}
				}
				break;


				case CustomMsgTypes::ClientAssignID:
				{					
					uint32_t tempID;
					msg >> tempID;
					mapPlayers.insert_or_assign(mapPlayers[nPlayerID].nUniqueID, mapPlayers[nPlayerID]);
					nPlayerID = tempID;
					//std::cout << "moje id je " << mapPlayers[nPlayerID].nUniqueID << std::endl;
				}
				break;
				
				case CustomMsgTypes::Game_AddPlayer:
				{
					sPlayerDescription desc;
					msg >> desc;
					mapPlayers.insert_or_assign(desc.nUniqueID, desc);

					if (desc.nUniqueID == nPlayerID)
					{
						std::cout << "tak ted nevim" << std::endl;
					}
				}
				break;
				
				// get something
				case CustomMsgTypes::Game_one:
				{
					int tile, place;
					msg >> place;
					msg >> tile;
					q->tiles[tile] = place;
					pBoard[place].sTile = tile;
					pBoard[place].empty = false;
					situation = 1;
				}
				break;

				case CustomMsgTypes::Game_two:
				{
					int tile, place;
					msg >> place;
					msg >> tile;
					q->tiles[tile] = place;
					pBoard[place].sTile = tile;
					pBoard[place].empty = false;
					situation = 0;
				}
				break;

				case CustomMsgTypes::Game_three:
				{
					int recievedTile;
					msg >> q->activeTile;
					std::cout << "Active Tile is " << q->activeTile << std::endl;
					q->tiles[q->activeTile] = -1;
					//pBoard[q->activeTile].sTile = -1;
					//pBoard[q->activeTile].empty = false;
					situation = 2;
				}
				break;

				case CustomMsgTypes::Game_four:
				{					
					msg >> q->activeTile;
					q->activeTile = q->activeTile;
					q->tiles[q->activeTile] = -1;
					//pBoard[q->activeTile].sTile = -1;
					//pBoard[q->activeTile].empty = false;
					situation = 3;
				}
				break;
				
				case CustomMsgTypes::Game_over:
				{
					msg >> q->isOver;

					q->start = false;
					inLobby = true;
					inGame = false;
				}
				break;
				}
			}
		}
		else
		{
			inGame = false;
			inLobby = true;
			std::string down = "Server Down\n";
			std::cout << down;
			pushMessage(down);
			return false;
		}

		return true;
	}
};

int main()
{
	CustomClientGUI clientGUI;
	if (clientGUI.Construct(800, 600, 1, 1))
		clientGUI.Start();
	else
		std::cerr << "Failed to create the client GUI\n";

	return 0;
}
