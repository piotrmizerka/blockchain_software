#pragma once

class AddressesContractorTest
{
	/// inner variables
	AddressesContractor testAddressesContractor;
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\AddressesContractor\\";
	bool testValue;

public:

	bool forestCheckx;

	/// functionalities
	// createTransactionAddressesParallel
	void createTransactionAddressesParallelGenerateBigTest(int addressesNumber = 360000, 
		int edgesNumber = 1000000, int filesNumber = 100, string relativePath = "createTransactionAddressesParallel"); // TODO
	// loadTransactionsAddressesParallel
	void loadTransactionsAddressesParallelTest(int transactionsNumber = 300000, int addressesNumber = 360000, int filesNumber = 100);
	// loadNeighborsListParallel
	void loadNeighborsListParallelTest(int transactionsNumber = 300000, int addressesNumber = 360000, int filesNumber = 100);
	// loadEdgesParallel
	void loadEdgesParallelTest(int transactionsNumber = 300000, int addressesNumber = 360000, int filesNumber = 100);
	// contractAddressesParsllel
	void contractAddressesParallelTest(int transactionsNumber = 300000, int addressesNumber = 360000, int filesNumber = 100);
	// labelConnectedComponentsParallel
	void labelConnectedComponentsParallelGenerateBigTest(int addressesNumber = 10000,
		int edgesNumber = 20000, int filesNumber = 100);
	void labelConnectedComponentsParallelTest(int transactionsNumber = 700000, int addressesNumber = 10000, int filesNumber = 100);
	// createUsersGraphParallel
	void createUsersGraphParallelGenerateBigTest(int transactionsNumber = 30000, int addressesNumber = 10000, int filesNumber = 100);
	void createUsersGraphParallelTest(int transactionsNumber = 30000, int addressesNumber = 10000, int filesNumber = 100); // checks number of unique pair edges and their sizes

	/// gets
	bool getTestValue();
};