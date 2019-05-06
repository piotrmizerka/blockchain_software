#include "stdafx.h"

void FileParser::setAddresses2Consider(set<int> addresses)
{
	addresses2Consider = addresses;
}

void FileParser::setTransactions2Consider(map<int, Transaction> transactions)
{
	transactions2Consider = transactions;
}

set<int> FileParser::getAddresses2Consider()
{
	return addresses2Consider;
}

map<int, Transaction> FileParser::getTransactions2Consider()
{
	return transactions2Consider;
}

FileParser::~FileParser()
{
}

void FileParser::generateUsersGraph2Consider()
{
	generateTransactionHypergraph2Consider();
	transactionHypergraph2Consider.createUsersGraph();
	usersGraph2Consider = transactionHypergraph2Consider.getUsersGraph();
}

void FileParser::generateTransactionHypergraph2Consider()
{
	vector <int> addressesx;
	vector <Transaction> transactionsx;
	Transaction transaction2Add;
	FOREACH(addressx, addresses2Consider)addressesx.push_back(*addressx);
	FOREACH(transactionx, transactions2Consider)
	{
		transaction2Add = transactionx->second;
		transaction2Add.id = transactionx->first;
		transactionsx.push_back(transaction2Add);
	}
	transactionHypergraph2Consider = TransactionHypergraph(addressesx, transactionsx);
}

void FileParser::parseAddresses()
{
	int id;
	string temp;
	cout << "\naddresses.txt\n\n";
	ifstream read;
	read.open( "F:\\bitcoin\\blockchain_new\\addresses.txt" );
	for (int i = 0; i<10; i++)
	{
		for (int j = 0; j<2; j++)
		{
			read >> temp;
			cout << temp << " ";
		}
		cout << endl;
	}
	read.close();

	int it = 0;
	addresses.clear();
	read.open("F:\\bitcoin\\blockchain_new\\addresses.txt");
	read.seekg(40);
	int length = read.tellg();
	cout << length << endl;
	getline(read, temp); cout << temp << endl;
	read >> id; cout << id<<endl;
	read.close();
}

void FileParser::parseBlockhash()
{
	ifstream read;
	string bhash,txs;
	blocks.clear();
	Block block;
	read.open("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\bh.dat");
	int it = 0;
	while (!read.eof())
	{
		read >> block.id >> bhash >> block.time >> txs;
		blocks.insert(block);
	}
	read.close();
}

void FileParser::parseTx()
{
	int txId, blockId, inputsNo, outputsNo, tempxx;
	FILE * read, *write;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\tx.dat", "r+");
	write = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\my_transactions_info\\my_transactions.dat", "w+");
	int it = 0;

	cout << "Generating txs:\n";

	while (!feof(read))
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &blockId);
		fscanf(read, "%i", &inputsNo);
		fscanf(read, "%i", &outputsNo);
		if (it % 10000000 == 0)
		{
			cout << it<<" "<<txId << " " << blockId << " " << inputsNo << " " << outputsNo << endl;
		}
		fprintf(write, "%i %i\n", txId, blockId);
		it++;
	}
	fclose(read);
	fclose(write);
}

void FileParser::parseTxin()
{
	int txId, inputId, addrId, inputSeq, prevTxId, prevOutputSeq;
	long long value;
	FILE * read, *write;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\txin.dat", "r+");
	write = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\my_transactions_info\\my_transactions_inputs.dat", "w+");
	int it = 0;

	cout << "Generating ins:\n";

	while (!feof(read))
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &inputSeq);
		fscanf(read, "%i", &prevTxId);
		fscanf(read, "%i", &prevOutputSeq);
		fscanf(read, "%i", &addrId);
		fscanf(read, "%lld", &value);
		if (it % 10000000 == 0)
		{
			cout << it << " " << txId << " " << inputSeq << " " << addrId << " " << value << endl;
		}
		fprintf(write, "%i %i\n", txId, addrId);
		it++;
	}
	fclose(read);
	fclose(write);
}

void FileParser::parseTxout()
{
	int txId, outputId, addrId;
	long long value;
	FILE * read, *write;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\txout.dat", "r+");
	write = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\my_transactions_info\\my_transactions_outputs.dat", "w+");
	int it = 0;

	cout << "Generating outs:\n";

	while (!feof(read))
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &outputId);
		fscanf(read, "%i", &addrId);
		fscanf(read, "%lld", &value);
		if (it % 10000000 == 0)
		{
			cout << it << " " << txId << " " << outputId << " " << addrId << " " << value << endl;
		}
		fprintf(write, "%i %i %I64d\n", txId, addrId, value);
		it++;
	}
	fclose(read);
	fclose(write);
}

void FileParser::parseMyTransactions()
{
	int txId, blockId;
	transactions2Consider.clear();
	transactionsIds2Consider.clear();
	addresses2Consider.clear();
	FILE * read;
	read = fopen("F:\\bitcoin\\blockchain_new\\my_transactions.txt", "r+");
	int it = 0;
	cout << "Parsing my_transactions.txt:\n";
	while (!feof(read) && it<100000)
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &blockId);
		if (it % 1000000 == 0)
		{
			cout << it << " " << txId << " " << blockId << endl;
		}
		if (blocksIds2Consider.find(blockId)!=blocksIds2Consider.end())
		{
			transactionsIds2Consider.insert(txId);
		}
		it++;
	}
	fclose(read);
}

void FileParser::parseMyTransactionsInputs()
{
	int txId, addrId;
	FILE * read;
	read = fopen("F:\\bitcoin\\blockchain_new\\my_transactions_inputs.txt", "r+");
	int it = 0;
	cout << "Parsing my_transactions_inputs.txt:\n";
	while (!feof(read) && it<100000)
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &addrId);
		if (it % 100000 == 0)
		{
			cout << it << " " << txId << " " << addrId << endl;
		}
		if (transactionsIds2Consider.find(txId) != transactionsIds2Consider.end())
		{
			transactions2Consider[txId].in.push_back(addrId);
			if (addresses2Consider.find(addrId) == addresses2Consider.end())
			{
				addresses2Consider.insert(addrId);
			}
		}
		it++;
	}
	fclose(read);
}

void FileParser::parseMyTransactionsOutputs()
{
	int txId, addrId;
	long long value;
	FILE * read;
	read = fopen("F:\\bitcoin\\blockchain_new\\my_transactions_outputs.txt", "r+");
	int it = 0;
	cout << "Parsing my_transactions_outputs.txt:\n";
	while (!feof(read) && it < 100000)
	{
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &addrId);
		fscanf(read, "%lld", &value);
		if (it % 100000 == 0)
		{
			cout << it << " " << txId << " " << addrId << " " << value << endl;
		}
		if (transactionsIds2Consider.find(txId) != transactionsIds2Consider.end())
		{
			transactions2Consider[txId].out.push_back(addrId);
			transactions2Consider[txId].amountsReceived[addrId].push_back(double(value));
			if (addresses2Consider.find(addrId) == addresses2Consider.end())
			{
				addresses2Consider.insert(addrId);
			}
		}
		it++;
	}
	fclose(read);
}

void FileParser::parseSmallAll()
{
	parseSmallTransactions();
	parseSmallIn();
	parseSmallOut();
	addresses2Consider = addresses2ConsiderSmallFiles;
}

void FileParser::parseSmallTransactions()
{
	int txId, blockId,currentTransactionId;
	transactions2Consider.clear();
	transactionsIds2Consider.clear();
	addresses2Consider.clear();
	addresses2ConsiderSmallFiles.clear();
	FILE * read;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\small_files\\transactions.dat", "r+");
	cout << "Parsing small transactions:\n";
	int it = 0;
	while (!feof(read))
	{
		if (it % 1000000 == 0)cout << it << endl;
		it++;
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &blockId);
		transactionsIds2Consider.insert(txId);
	}
	fclose(read);
}

void FileParser::parseSmallIn()
{
	int txId, addrId,currentAddressSmallFiles;
	FILE * read;
	vector<int>::iterator it;
	addressesIdsHypergraph.clear();
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\small_files\\in.dat", "r+");
	cout << "Parsing small in:\n";
	int itx = 0;
	while (!feof(read))
	{
		if (itx % 1000000 == 0)cout << itx << endl;
		itx++;
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &addrId);
		if (transactionsIds2Consider.find(txId) != transactionsIds2Consider.end())
		{
			if (addresses2Consider.find(addrId) == addresses2Consider.end())
			{
				currentAddressSmallFiles = addresses2Consider.size() + 1;
				addressesIdsHypergraph.insert(make_pair(addrId, currentAddressSmallFiles));
				addresses2Consider.insert(addrId);
				addresses2ConsiderSmallFiles.insert(currentAddressSmallFiles);
			}
			currentAddressSmallFiles = addressesIdsHypergraph[addrId];
			it = find(transactions2Consider[txId].in.begin(), transactions2Consider[txId].in.end(), currentAddressSmallFiles);
			if (it == transactions2Consider[txId].in.end())transactions2Consider[txId].in.push_back(currentAddressSmallFiles);
		}
	}
	fclose(read);
}

void FileParser::parseSmallOut()
{
	int txId, addrId, currentAddressSmallFiles;
	long long value;
	FILE * read;
	vector<int>::iterator it;
	read = fopen("C:\\Users\\Administrator\\Desktop\\bitcoin\\pliki_wegrow_2018_luty\\small_files\\out.dat", "r+");
	cout << "Parsing small out:\n";
	int itx = 0;
	while (!feof(read))
	{
		if (itx % 1000000 == 0)cout << itx << endl;
		itx++;
		fscanf(read, "%i", &txId);
		fscanf(read, "%i", &addrId);
		fscanf(read, "%lld", &value);
		if (transactionsIds2Consider.find(txId) != transactionsIds2Consider.end())
		{
			if (addresses2Consider.find(addrId) == addresses2Consider.end())
			{
				currentAddressSmallFiles = addresses2Consider.size() + 1;
				addressesIdsHypergraph.insert(make_pair(addrId, currentAddressSmallFiles));
				addresses2Consider.insert(addrId);
				addresses2ConsiderSmallFiles.insert(currentAddressSmallFiles);
			}
			currentAddressSmallFiles = addressesIdsHypergraph[addrId];
			it = find(transactions2Consider[txId].out.begin(), transactions2Consider[txId].out.end(), currentAddressSmallFiles);
			if (it == transactions2Consider[txId].out.end())transactions2Consider[txId].out.push_back(currentAddressSmallFiles);
			transactions2Consider[txId].amountsReceived[currentAddressSmallFiles].push_back(double(value));
		}
	}
	fclose(read);
}

void FileParser::generateMyFiles()
{
	//parseTx();
	parseTxin();
	//parseTxout();
}

void FileParser::parseAll()
{
	parseMyTransactions();
	parseMyTransactionsInputs();
	parseMyTransactionsOutputs();
	cout << "adresses2Consider.size() = " << addresses2Consider.size() << endl;
	cout << "transactions2Consider.size() = " << transactions2Consider.size() << endl;
	cout << "transactionsIds2Consider.size() = " << transactionsIds2Consider.size() << endl;
}

bool operator<(const Block & P, const Block & Q)
{
	return P.id<Q.id;
}
