#pragma once

class UsersGraphCreator
{
   

public:

	/// functionalities
	// contracting addresses
	void contractAddresses(string relativePath = "");
	
	/// inner methods
	// neighbors list
	void createNeighborsList();
	// other methods
	void contractAddressesBFS();
	void BFS();

	
};