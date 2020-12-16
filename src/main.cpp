#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <sstream>

using namespace std;

struct {
	vector<unsigned char> bytes;
} typedef IP_Entry;

struct {
	vector<unsigned char> prefix;
	int prefix_length;
	int AS;
} typedef DB_Entry;

struct {
	IP_Entry ip;
	DB_Entry db;
} typedef Lookup_Entry;

vector<IP_Entry> ip_list;
vector<DB_Entry> db;
vector<Lookup_Entry> lookup_entries;

// Split strings on decimal point and convert
// each byte to an integer. This makes it easier
// to perform bitwise comparison on parts of the
// address later on.
vector<unsigned char> __to_array(string addr) {
	vector<unsigned char> array;
	stringstream ss(addr);
	string token;
	while (std::getline(ss, token, '.')) {
		array.push_back(stoi(token.c_str()));
	}
	return array;
}

// Undoes __to_array()
// Convert each byte of ip to string and concat with decimals
string __to_string(vector<unsigned char> ip) {
	ostringstream stream;
	for(int i = 0; i < ip.size()-1; i++) {
		stream << static_cast<unsigned int>(ip.at(i)) << ".";
	}
	stream << static_cast<unsigned int>(ip.back());
	return stream.str();
}

// Turn ip byte array into integer by shifting
// each byte by the inverse of its index (largest index
// represents the rightmost term in the address).
int __to_int(vector<unsigned char> ip) {
	int result = 0;
	int size = ip.size();
	for(int i = 0; i < size; i++) {
		int temp = ip.at((size-1)-i) << 8*i;
		result += temp;
	}
	return result;
}

// Load IPs from specified file
void load_ips(string filename) {
	ifstream infile;
	infile.open(filename);

	while (infile) {
		string ip;
		infile >> ip;
		// Last line of file is blank
		if (ip.length() == 0) {
			continue;
		}
		IP_Entry entry;
		entry.bytes = __to_array(ip);
		ip_list.push_back(entry);
	}
	infile.close();
}

// Load DB from specified file
void load_db(string filename) {
	ifstream infile;
	infile.open(filename);

	while (infile) {
		string prefix;
		infile >> prefix;
		// Last line of file is blank
		if (prefix.length() == 0) {
			continue;
		}
		DB_Entry entry;
		infile >> entry.prefix_length;
		infile >> entry.AS;
		entry.prefix = __to_array(prefix);
		db.push_back(entry);
	}
	infile.close();
}

// Check if ip in question matches the prefix at
// the given DB index
int __check_entry(int index, IP_Entry ip) {
	DB_Entry entry = db.at(index);
	int addr = __to_int(ip.bytes);
	int prefix = __to_int(entry.prefix);
	int shift = 32 - entry.prefix_length;
	addr >>= shift, prefix >>= shift;
	int res = !(addr ^ prefix);
	return res;
}

void __lookup(IP_Entry ip) {
	Lookup_Entry entry;
	int curr_prefix_length = 0;
	for (int i = 0; i < db.size(); i++) {
		if (!__check_entry(i, ip)) {
			continue;
		}
		// Prefix matched
		int prefix_length = db.at(i).prefix_length;
		if (prefix_length > curr_prefix_length) {
			curr_prefix_length = prefix_length;
			entry.db = db.at(i);
			entry.ip = ip;
		}
	}
	lookup_entries.push_back(entry);
}

// Find AS values for all ips in ip_list
void do_lookup() {
	for_each(ip_list.begin(), ip_list.end(), __lookup);
}

// Write lookup_entries array to output.txt
void write_output() {
	ofstream outfile;
	outfile.open("output.txt", ofstream::out | ofstream::trunc);
	for(int i = 0; i < lookup_entries.size(); i++) {
		Lookup_Entry entry = lookup_entries.at(i);
		outfile
		<< __to_string(entry.db.prefix) << '/'
		<< entry.db.prefix_length << ' '
		<< entry.db.AS << ' '
		<< __to_string(entry.ip.bytes) << endl;
	}
	outfile.close();
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		cout << "Wrong number of arguments" << endl;
		return 1;
	}
	load_db(argv[1]);
	load_ips(argv[2]);
	do_lookup();
	write_output();
	return 0;
}

