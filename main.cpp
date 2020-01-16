//Nguyen Thanh Trung
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include <algorithm>

using namespace std;

#define fname_length 13
#define limited_memory 8000000000 //8GB

char max_string[] = {char(127), char(127), char(127), char(127), char(127), char(127), char(127), char(127)};

struct node {  
    string element; 
    int index; 
}; 

void mySwap(node &x, node &y) { 
    node temp = x; 
    x = y; 
    y = temp; 
} 

class MinHeap {
private:
	vector<node> h_arr; 
    int h_size;
public:
	MinHeap(vector<node> a, int size) {
		h_size = size;
		h_arr = a;
		int i = (h_size - 1)/2;
		while (i >= 0) { 
			heapify(i); 
			i--; 
		} 
	}

	void heapify(int i) {
		int l = getLeft(i); 
		int r = getRight(i); 
		int smallest = i; 
		if (l < h_size && h_arr[l].element.compare(h_arr[i].element) < 0) 
			smallest = l; 
		if (r < h_size && h_arr[r].element.compare(h_arr[smallest].element) < 0) 
			smallest = r; 
		if (smallest != i) { 
			mySwap(h_arr[i], h_arr[smallest]); 
			heapify(smallest); 
		} 
	}

	int getLeft(int i) {
		return 2*i+1;
	}

	int getRight(int i) {
		return 2*i+2;
	}

	node getMin() {
		return h_arr[0];
	}

	void replaceRoot(node x) {
		h_arr[0] = x;
		heapify(0);
	}
};

void merge(string arr[], int l, int m, int r) { 
    int i, j, k; 
    int n1 = m - l + 1; 
    int n2 = r - m; 
  
    string *L = new string[n1];
	string *R = new string[n2];
  
    for(i = 0; i < n1; i++) 
        L[i] = arr[l + i]; 
    for(j = 0; j < n2; j++) 
        R[j] = arr[m + 1 + j]; 
  
    i = 0; 
    j = 0; 
    k = l; 
    while (i < n1 && j < n2) { 
		if (L[i].compare(R[j]) <= 0) 
            arr[k++] = L[i++]; 
        else
            arr[k++] = R[j++]; 
    } 
  
    while (i < n1) 
        arr[k++] = L[i++]; 
  
    while(j < n2) 
        arr[k++] = R[j++]; 
} 

void mergeSort(string arr[], int l, int r) { 
    if (l < r) { 
        int m = l + (r - l) / 2; 
        mergeSort(arr, l, m); 
        mergeSort(arr, m + 1, r); 
        merge(arr, l, m, r); 
    } 
} 

void createInitialRuns(char *input_file, long mem_limit, long &num_ways) { 
	if (mem_limit > limited_memory) mem_limit = limited_memory;
	ifstream f_in;
	f_in.open(input_file);

	// Determine file size
	streampos begin = f_in.tellg();
	f_in.seekg(0, ios::end);
	streampos end = f_in.tellg();
	size_t size = end-begin;
	if (size <= mem_limit)
		num_ways = 1;
	else
		num_ways = (size/mem_limit) + 5;
	f_in.clear();
	f_in.seekg(0, ios::beg);

	ofstream *f_out = new ofstream[num_ways];
    char fileName[fname_length]; 
    for (long i = 0; i < num_ways; i++) { 
        itoa(i, fileName, 10); 
		f_out[i].open(fileName);
    } 
  
    bool more_input = true; 
    int next_output_file = 0; 
  
	vector<string> arr;
	if (num_ways > 1) {
		while (more_input) { 
			size_t countByte = 0;
			while (countByte < mem_limit) {
				if (f_in.eof()) { 
					more_input = false; 
					break; 
				} 
				if (more_input) {
					string temp;
					getline(f_in, temp);
					arr.push_back(temp);
					countByte += temp.size();
				}
			}
			mergeSort(&arr[0], 0, arr.size() - 1); 
			for (int j = 0; j < arr.size(); j++) {
				if (arr[j].size())
					f_out[next_output_file]<<arr[j]<<"\n";
			}
			arr.clear();
			vector<string> temp;
			arr.swap(temp);
			next_output_file++; 
		} 
	}
	else if (num_ways == 1) {
		while (true) { 
			if (f_in.eof()) { 
				break; 
			} 
			string temp;
			getline(f_in, temp);
			arr.push_back(temp);
		}
		mergeSort(&arr[0], 0, arr.size() - 1); 
		for (int j = 0; j < arr.size(); j++) {
			if (arr[j].size())
				f_out[next_output_file]<<arr[j]<<"\n";
		}
		arr.clear();
		vector<string> temp;
		arr.swap(temp);
	}

    for (long i = 0; i < num_ways; i++)
		f_out[i].close(); 
	f_in.close();
} 

bool isEmptyFile(ifstream &file) {
	return file.peek() == ifstream::traits_type::eof();
}

bool isFileExists(char *name) {
    ifstream f(name);
    bool rs = f.good();
	f.close();
	return rs;
}

void mergeFiles(char *output_file, int num_ways) { 
	ifstream *f_in = new ifstream[num_ways];
    for (long i = 0; i < num_ways; i++) { 
        char fileName[fname_length]; 
		itoa(i, fileName, 10);
		f_in[i].open(fileName);
    } 
	ofstream f_out(output_file);

	vector<node> arr;
	long i;
    for (i = 0; i < num_ways; i++) { 
		node temp;
		if (!isEmptyFile(f_in[i])) {
			getline(f_in[i], temp.element);
			temp.index = i;
			arr.push_back(temp);
		}
    } 

	MinHeap hp(arr, arr.size());
    int count = 0; 
  
    while (count != i) { 
		node root = hp.getMin();
		if (root.element.size() && root.element.compare(max_string) != 0)
			f_out<<root.element<<"\n";

		if(f_in[root.index].eof()) {
			root.element = max_string;
            count++; 
		}
		else 
			getline(f_in[root.index], root.element);
        hp.replaceRoot(root); 
    } 
  
	char fileName[fname_length]; 
	for (long i = 0; i < num_ways; i++) {
		f_in[i].close(); 
		itoa(i, fileName, 10); 
		remove(fileName);
	}
	f_out.close(); 
} 

void externalSort(char* input_file,  char *output_file, long mem_limit) { 
	long num_ways;
    createInitialRuns(input_file, mem_limit, num_ways); 
	mergeFiles(output_file, num_ways); 
} 

char *genString() {
	char *s = new char[1000];
	int len = 1 + rand() % 1000, value = 0;
	for (int i = 0; i < len; i++) {
		value = rand() % 9;
		s[i] = value + '0';
	}
	s[len] = '\0';
	return s;
}

int isValidMem(char *mem) {
	int len = strlen(mem);
	if (len > 10) return false;
	if (mem[0] == '-') return false;
	for (int i = 0; i < len; i++) {
		if (mem[i] < '0' || mem[i] > '9') return false;
	}
	int rs = 0;
	for (int i = 0; i < len; i++) {
		rs = rs*10 + (mem[i] - '0');
	}
	if (rs > limited_memory) return false;
	return rs;
}

int main() { 
	char mem_limit[100]; 
    char input_file[100];
    char output_file[100]; 

	printf("Input file name: ");
	cin>>input_file;
	while (!isFileExists(input_file)) {
		printf("File does not exist!\nInput file name: ");
		cin>>input_file;
	}

	printf("Output file name: ");
	cin>>output_file;

	printf("Memory limit (bytes): ");
	cin>>mem_limit;
	int mem;
	while (!(mem = isValidMem(mem_limit))) {
		printf("Memory limit (bytes): ");
		cin>>mem_limit;
	}
  
	// Generate text file
	/*
    ofstream f_out(input_file);
  
    for (int i = 0; i < 100000; i++) 
		f_out<<genString()<<'\n';

	f_out.close();
	*/

	externalSort(input_file, output_file, mem); 
    return 0; 
} 