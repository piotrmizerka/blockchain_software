#pragma once

class TransactionHypergraphTest
{
	/// inner variables
	TransactionHypergraph testTransactionHypergraph;
	string pathx = "C:\\Users\\Administrator\\Desktop\\bitcoin\\bitcoin_analysis\\tests\\TransactionHypergraph\\";

public:
	/// functionalities
	// cluster
	void clusterTest(bool smallTests = true, bool bigTests = false);
	void clusterGenerateBigTest(int addressesNumber = 1000, int transactionsNumber = 100);
	// createUsersGraph
	void createUsersGraphTest(bool smallTests = true, bool bigTests = false);
	void createUsersGraphGenerateBigTest(int addressesNumber = 1000, int transactionsNumber = 100);
	// all
	void testAll(bool smallTests = true, bool bigTests = false);
	void generateBigTestAll(int addressesNumber = 1000, int transactionsNumber = 100);

	/// inner methods
	void generateBigTestIn(string relativePath, int addressesNumber = 1000, int transactionsNumber = 100);
	void loadTestIn(string relativePath);
	void clusterVerify(bool smallTests = true, int smallId = 0);
	void createUsersGraphVerify(bool smallTests = true, int smallId = 0);/*heuristic verification by out-degrees and weight sum*/
};