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

#include <iostream>
#include <string>
#include <cstdio>
#include <map>
#include <unordered_map>
#include <bitset>
#include "../quartoNetworkVisual/olcPGEX_Network.h"
#include "quartoGame.h"

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



class CustomServer : public olc::net::server_interface<CustomMsgTypes>
{
private:
	std::map<uint32_t, std::shared_ptr<olc::net::connection<CustomMsgTypes>>> clients;
	std::unordered_map<uint32_t, sPlayerDescription> mapPlayers;
public:
	std::unordered_map<uint32_t, std::vector<uint32_t>> createdGames;  // Maps game ID to vector of client IDs
	std::unordered_map<uint32_t, QuartoGame> quartoGames;
	std::vector <uint32_t> gamesBuffer;
	std::vector <std::string> playersBuffer;
	int maxMessages = 20; // Assuming each message is 10 pixels high

	CustomServer(uint16_t nPort) : olc::net::server_interface<CustomMsgTypes>(nPort)
	{

	}

	bool pushGame(uint32_t gameID) {
		// Check if the game already exists in the createdGames map
		if (createdGames.find(gameID) != createdGames.end()) {
			std::cout << "Game " << gameID << " already exists in createdGames\n";
			return false; // Game already exists
		}

		// Check if max messages limit is reached
		if (gamesBuffer.size() >= maxMessages) {
			std::cout << "Cannot add game " << gameID << ": max messages limit reached.\n";
			return false; // Cannot add more games
		}

		// Add the new game ID to the gamesBuffer
		gamesBuffer.push_back(gameID);
		std::cout << "Game " << gameID << " added to buffer. Current buffer size: " << gamesBuffer.size() << std::endl;

		// Initialize the new game in createdGames with an empty vector
		createdGames[gameID] = std::vector<uint32_t>(); // Initializes the game with an empty player list
		
		return true; // Successfully added the game
	}

	bool players() {
		// Check if the map is not empty
		if (clients.empty()) {
			std::cout << "No connected players." << std::endl;
			return false;
		}

		// Iterate through the map and print all player IDs
		std::cout << "Connected Players: " << std::endl;
		for (const auto& [id, connection] : clients) {  // C++17 structured binding
			std::cout << "Player ID: " << clients[id]->GetID()  << std::endl;
		}
		for (const auto& client : clients) {
			std::cout << "Player ID: " << client.first << " Connection ID: " << client.second->GetID() << std::endl;
		}
		return true;
	}

	// Function to send a message to a specific client
	void SendMessageToClient(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, const olc::net::message<CustomMsgTypes>& msg) {
		if (client && client->IsConnected()) {
			client->Send(msg); // Send the message to the specific client
		}
	}
	
	// Find a specific client by ID
	std::shared_ptr<olc::net::connection<CustomMsgTypes>> GetClientByID(uint32_t id) {
		for (auto& client : m_deqConnections) {
			if (client->GetID() == id) {
				return client;
			}
		}
		return nullptr;
	}

	// Send message to a client by ID
	void SendMessageToClientByID(uint32_t clientID, olc::net::message<CustomMsgTypes>& msg) {
		auto client = GetClientByID(clientID);
		if (client) {
			SendMessageToClient(client, msg);
		}
		else {
			std::cout << "Client with ID " << clientID << " not found.\n";
		}
	}
	
	olc::net::message<CustomMsgTypes> updateBuffer(std::vector <std::uint32_t> buff) {
		// Create message and send buffer of games to all players
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::UpdateBuffer;
		
		// Send game details to all clients
		for (const auto& game : buff) {
			msg << std::to_string(game);
		}
		msg << int(buff.size());
		return msg;
	}

	bool resolveGame(uint32_t idOfGame) {
		quartoGames[idOfGame].isOver = true;
		return quartoGames[idOfGame].isOver;
	}

protected:

	virtual bool OnClientConnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		// Add client to the map when connected
		clients[client->GetID()] = client;

		// Send acceptance message to the client
		olc::net::message<CustomMsgTypes> msg;
		msg.header.id = CustomMsgTypes::ServerAccept;
		client->Send(msg);
		std::cout << "Registered Clients:\n";

		return true;
	}

	// Called when a client appears to have disconnected
	virtual void OnClientDisconnect(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client)
	{
		// Remove client from the server when disconnected
		clients.erase(client->GetID());
		std::cout << "Removing client [" << client->GetID() << "]\n";
	}

	// Called when a message arrives
	virtual void OnMessage(std::shared_ptr<olc::net::connection<CustomMsgTypes>> client, olc::net::message<CustomMsgTypes>& msg)
	{
		players();
		MessageAllClients(updateBuffer(gamesBuffer));

		switch (msg.header.id)
		{
		case CustomMsgTypes::ServerPing:
		{
			std::cout << "[" << client->GetID() << "]: Server Ping\n";
			// Simply bounce message back to client
			client->Send(msg);
		}
		break;

		case CustomMsgTypes::MessageAll:
		{
			std::cout << "[" << client->GetID() << "]: Message All\n";
			std::string user_text;
			msg >> user_text;
			// Construct a new message and send it to all clients
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ServerMessage;
			msg << client->GetID();
			msg << user_text;
			MessageAllClients(msg, client);
		}
		break;

		case CustomMsgTypes::CreateGame:
		{
			uint32_t creatorID = client->GetID();
			std::cout << "[" << creatorID << "]: Creating Game\n";

			// Use creatorID as the new game ID
			uint32_t newGameID = creatorID;  // Or generate a unique game ID based on your logic

			if (pushGame(newGameID)) {  // Only pass the new game ID
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::GameCreated;

				olc::net::message<CustomMsgTypes> msg2;
				msg2.header.id = CustomMsgTypes::GameCreatedByMe;

				// Store the host ID in createdGames
				createdGames[newGameID].push_back(creatorID); // Store the host ID
				
				// Send game details to all clients
				for (int i = 0; i < gamesBuffer.size(); i++) {
					msg << gamesBuffer[i];
				}
				msg << int(gamesBuffer.size());  // Send the count of games
				msg << newGameID;  // Send the new game ID

				// push new instance of quartoGames into map


				// Send message to all clients
				MessageAllClients(msg, client);

				// Send confirmation to the game creator
				msg2 << creatorID;  // Send the creator's ID as confirmation
				client->Send(msg2);
			}
			else {
				std::string str = "Game not created.";
				olc::net::message<CustomMsgTypes> msg;
				msg.header.id = CustomMsgTypes::GameNotCreated;
				msg << str;
				client->Send(msg);
			}
		}
		break;

		case CustomMsgTypes::JoinGame:
		{
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::GameJoined;
			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::GameJoinedByPlayer;
			olc::net::message<CustomMsgTypes> msg3;
			msg3.header.id = CustomMsgTypes::UpdateBuffer;

			if (!gamesBuffer.empty()) {
				// Ensure you get the right creatorID for the game to join
				uint32_t creatorID = gamesBuffer.front();  // Assuming this holds the host's ID
				uint32_t player2 = client->GetID();
				std::cout << "Player " << player2 << " is trying to join game hosted by " << creatorID << std::endl;

				if (createdGames.find(creatorID) == createdGames.end()) {
					std::cout << "Game ID " << creatorID << " does not exist in createdGames." << std::endl;
					break; // Handle accordingly
				} 

				// Notify the joining player
				msg << creatorID;
				client->Send(msg);

				// Add the player to the game
				createdGames[creatorID].push_back(player2);


				// Notify the host player
				msg2 << player2;
				SendMessageToClientByID(creatorID, msg2);
				QuartoGame q(creatorID);
				q.player1 = creatorID;
				q.player2 = player2;
				quartoGames[creatorID] = q;


				std::cout << "Current Clients:" << std::endl;
				for (const auto& client : clients) {
					std::cout << "Client ID: " << client.first << std::endl;
				}
				gamesBuffer.erase(gamesBuffer.begin());  // Remove the game from the buffer
				// send new buffer to all players
				msg3 << int(gamesBuffer.size());
				for (int i = 0; i < gamesBuffer.size(); i++) {
					msg3 << gamesBuffer[i];
				}
				MessageAllClients(msg3, client);

			}
			else {
				std::string txt = "No games to join";
				msg.header.id = CustomMsgTypes::GameNotJoined;
				msg << txt;
				client->Send(msg);
			}
		}
		break;


		case CustomMsgTypes::PlayerRegistration:
		{
			sPlayerDescription desc;
			msg >> desc;
			desc.nUniqueID = client->GetID();
			mapPlayers.insert_or_assign(desc.nUniqueID, desc);
			
			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::ClientAssignID;
			msg << desc.nUniqueID;
			MessageClient(client, msg);

			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::Game_AddPlayer;
			msg2 << desc;
			MessageAllClients(msg2);

			for (const auto& player : mapPlayers) {
				olc::net::message<CustomMsgTypes> msg3;
				msg3.header.id = CustomMsgTypes::Game_AddPlayer;
				msg3 << player.second;
				MessageClient(client, msg3);
			}
		}
		break;

		case CustomMsgTypes::Game_one:
		{
			uint32_t idLocal;
			uint32_t idRemote;
			int tileLocal;

			msg >> idLocal;
			msg >> tileLocal;

			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::Game_three;

			msg << tileLocal;

			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::Game_four;

			
			if (client->GetID() == idLocal) {
				idRemote = quartoGames[idLocal].player2;
			}
			else {
				idRemote = quartoGames[idLocal].player1;
			}

			quartoGames[idLocal].tiles[tileLocal] = -1;

			msg2 << tileLocal;

			SendMessageToClientByID(idRemote, msg);
			//MessageClient(client->GetID(), msg2);
			client->Send(msg2);
			std::cout <<  "game one: Player " << idRemote << " sending " << tileLocal << std::endl;
			
			// Show all the tiles on the board (rotated 180 degrees)
			for (int row = 3; row >= 0; row--) {
				for (int col = 3; col >= 0; col--) {
					int mirroredIndex = row * 4 + col;
					std::cout << quartoGames[idLocal].tiles[mirroredIndex] << " ";
				}
				std::cout << std::endl;
			}
			std::cout << std::endl;
		}
		break;

		case CustomMsgTypes::Game_three:
		{
			uint32_t idLocal;
			uint32_t idRemote;
			int tileLocal;
			int place;
			msg >> idLocal;
			msg >> tileLocal;
			msg >> place;
			if (client->GetID() == idLocal) {
				idRemote = quartoGames[idLocal].player2;
			}
			else {
				idRemote = quartoGames[idLocal].player1;
			}

			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::Game_one;
			msg << tileLocal;
			msg << place;

			olc::net::message<CustomMsgTypes> msg2;
			msg2.header.id = CustomMsgTypes::Game_two;
			msg2 << tileLocal;
			msg2 << place;

			quartoGames[idLocal].tiles[tileLocal] = place;

			SendMessageToClientByID(idRemote, msg);
			client->Send(msg2);
			std::cout << "game three: Player " << idRemote << " placing " << tileLocal << " on place " << place << std::endl;
		}
		break;
		case CustomMsgTypes::Game_quarto:
		{
			uint32_t idLocal;
			uint32_t idRemote;
			msg >> idLocal;
			if (client->GetID() == idLocal) {
				idRemote = quartoGames[idLocal].player2;
			}
			else {
				idRemote = quartoGames[idLocal].player1;
			}

			olc::net::message<CustomMsgTypes> msg;
			msg.header.id = CustomMsgTypes::Game_over;
			
			bool over = resolveGame(quartoGames[idLocal].gameID);
			msg << over;

			SendMessageToClientByID(idRemote, msg);
			client->Send(msg);
			std::cout << "game Quarto: " << quartoGames[idLocal].gameID << std::endl;

			// Remove all occurrences of gameId
			gamesBuffer.erase(std::remove(gamesBuffer.begin(), gamesBuffer.end(), quartoGames[idLocal].gameID), gamesBuffer.end());
			createdGames.erase(quartoGames[idLocal].gameID);

			// Print remaining elements in the buffer
			for (const auto& game : gamesBuffer) {
				std::cout << game << " ";
			}
			std::cout << std::endl;

		}
		break;
		}		
	}
};

int main()
{
	CustomServer server(60000);
	server.Start();

	while (1)
	{
		server.Update(-1, true);
	}



	return 0;
}