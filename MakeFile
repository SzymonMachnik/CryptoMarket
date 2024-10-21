all:
	g++ -o main.exe main.cpp memory/memory.cpp crypto/crypto.cpp handleUser/handleUser.cpp -LC:/CryptoScraper/curlLib/lib -IC:/CryptoScraper/curlLib/include -lcurl