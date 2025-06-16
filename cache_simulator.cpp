#include <iostream>
#include <vector>
#include <iomanip>
#include <unordered_map>
#include <sstream>
using namespace std;

struct CacheBlock {
    bool valid;
    int tag;
    int dataAddress;
};

int hexToDec(const string &hex) {
    return stoi(hex, nullptr, 16);
}

void printBinary(int num, int bits = 8) {
    for (int i = bits - 1; i >= 0; i--)
        cout << ((num >> i) & 1);
}

int main() {
    int cacheSize, blockSize;
    cout << "Enter cache size (in bytes): ";
    cin >> cacheSize;
    cout << "Enter block size (in bytes): ";
    cin >> blockSize;

    int numBlocks = cacheSize / blockSize;
    vector<CacheBlock> cache(numBlocks, {false, -1, -1});

    int n;
    cout << "Enter number of memory accesses: ";
    cin >> n;

    vector<pair<char, string>> accesses(n);
    cout << "Enter accesses in format <R/W> <hex_address>:\n";
    for (int i = 0; i < n; i++) {
        cin >> accesses[i].first >> accesses[i].second;
    }

    int hits = 0, misses = 0;

    cout << "\n=== Access Log ===\n";
    for (auto &[op, addrStr] : accesses) {
        int address = hexToDec(addrStr);
        int blockNumber = (address / blockSize) % numBlocks;
        int tag = address / (blockSize * numBlocks);

        CacheBlock &block = cache[blockNumber];

        bool isHit = (block.valid && block.tag == tag);
        cout << op << " " << addrStr << " → " << (isHit ? "HIT" : "MISS");

        if (isHit) {
            hits++;
        } else {
            misses++;
            block.valid = true;
            block.tag = tag;
            block.dataAddress = address;
        }

        cout << " (Index: " << blockNumber << ", Tag: " << tag << ")\n";
    }

    cout << "\n=== Final Cache State ===\n";
    cout << "Index\tValid\tTag\tAddress\n";
    for (int i = 0; i < numBlocks; i++) {
        cout << i << "\t" << cache[i].valid << "\t";
        if (cache[i].valid)
            cout << cache[i].tag << "\t0x" << hex << cache[i].dataAddress << dec;
        else
            cout << "-\t-";
        cout << "\n";
    }

    cout << "\nTotal Accesses: " << n << "\n";
    cout << "Cache Hits: " << hits << "\n";
    cout << "Cache Misses: " << misses << "\n";
    cout << "Hit Rate: " << fixed << setprecision(2) << (hits * 100.0 / n) << "%\n";

    return 0;
}


