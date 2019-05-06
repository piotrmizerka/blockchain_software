#pragma once

struct Block
{
	int id;
	long long time;
};

bool operator < (const Block &P, const Block &Q);

class FileParser
{
	/// functional variables
	TransactionHypergraph transactionHypergraph2Consider;
	Graph usersGraph2Consider;

	/// inner variables
	vector <int> addresses;
	set <int> addresses2Consider;
	set <int> addresses2ConsiderSmallFiles;
	map <int, int> addressesIdsHypergraph;
	set <Block> blocks;
	set <int> blocksIds2Consider;
	map <int, Transaction> transactions2Consider;
	set <int> transactionsIds2Consider;

public:
	/// functionalities
	// generating transaction hypergraph
	void generateTransactionHypergraph2Consider();
	// generating users graph
	void generateUsersGraph2Consider();
	// other functionalities
	void parseAll();
	void parseMyTransactions();
	void parseMyTransactionsInputs();
	void parseMyTransactionsOutputs();
	void parseSmallAll();
	void parseSmallTransactions();
	void parseSmallIn();
	void parseSmallOut();
	void generateMyFiles();
	void parseAddresses();
	void parseBlockhash();
	void parseTx();
	void parseTxin();
	void parseTxout();

	/// gets, sets and destructor
	void setAddresses2Consider(set<int> addresses);
	void setTransactions2Consider(map<int, Transaction> transactions);
	set<int> getAddresses2Consider();
	map<int, Transaction> getTransactions2Consider();
	~FileParser();
};



