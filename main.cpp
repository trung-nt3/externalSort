//Nguyen Thanh Trung
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <queue>

using namespace std;

#define fname_length 1001
char fileName[fname_length]; 

#define limited_memory 8000000000 //8GB
#define rint register int

struct node {  
    string element; 
    int index; 
}; 

class myComparator { 
public: 
    int operator() (const node& node1, const node& node2) { 
        return (node1.element.compare(node2.element) > 0);
    } 
}; 

void mySwap(node &x, node &y) { 
    node temp = x; 
    x = y; 
    y = temp; 
} 

template<class Iter>
void mergeSort(Iter first, Iter last) { 
    if (last - first > 1) { 
        Iter middle = first + (last - first) / 2;
        mergeSort(first, middle);
        mergeSort(middle, last);
  		inplace_merge(first, middle, last);
	} 
} 

void my_itoa(int num, char *arr, int &cnt) {
 	if (num <= 0 && cnt > 0) return;
 	if (num == 0 && cnt == 0) {
  		arr[cnt] = '0';
  		arr[cnt+1] = '\0';
  		return;
 	}
 	if (num > 0) {
  		my_itoa(num/10, arr, cnt);
  		arr[cnt] = num%10 + '0';
  		++cnt;
  		arr[cnt] = '\0';
 	}
}

void createInitialRuns(string input_file, string output_file, unsigned long long mem_limit, int &num_ways) { 
 	if (mem_limit > limited_memory) mem_limit = limited_memory;
 	ifstream f_in;
 	f_in.open(input_file);

 	//Determine file size
 	streampos begin = f_in.tellg();
 	f_in.seekg(0, ios::end);
 	streampos end = f_in.tellg();
 	unsigned long long size = end-begin;
 	if (size <= mem_limit)
  		num_ways = 1;
 	else
  		num_ways = (size/mem_limit) + 1;
 	f_in.clear();
 	f_in.seekg(0, ios::beg);

 	vector<string> arr;
 	if (num_ways > 1) {
  		ofstream *f_out = new ofstream[num_ways];
  		for (rint i = 0; i < num_ways; i++) { 
   			int cnt = 0;
   			my_itoa(i, fileName, cnt);
   			f_out[i].open(fileName);
  		} 
  
  		bool more_input = true; 
  		int next_output_file = 0; 
  
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
     				std::string().swap(temp);
    			}
   			}
   			mergeSort(arr.begin(), arr.end()); 
   			f_out[next_output_file].clear();
   			for (rint j = 0; j < arr.size(); j++) {
    			if (arr[j].size()) {
     				f_out[next_output_file]<<arr[j]<<"\n";
    			}
			}
   			arr.clear();
   			vector<string> temp;
   			arr.swap(temp);
   			next_output_file++; 
  		}	 
  		for (rint i = 0; i < num_ways; i++)
   			f_out[i].close(); 
 	}
 	else if (num_ways == 1) {
  		while (true) { 
   			if (f_in.eof()) { 
    			break; 
   			}	 
   			string temp;
   			getline(f_in, temp);
   			arr.push_back(temp);
   			std::string().swap(temp);
  		}
  		mergeSort(arr.begin(), arr.end()); 
  		ofstream f_out(output_file);
  		f_out.clear();
  		for (rint j = 0; j < arr.size(); j++) {
   			if (arr[j].size()) {
    			f_out<<arr[j]<<"\n";
   			}
  		}
  		arr.clear();
  		vector<string> temp;
  		arr.swap(temp);
  		f_out.close();
 	}
 	f_in.close();
} 

bool isEmptyFile(ifstream &file) {
 	return file.peek() == ifstream::traits_type::eof();
}

bool isFileExists(string name) {
    ifstream f(name);
    bool rs = f.good();
 	f.close();
 	return rs;
}

void mergeFiles(string output_file, int num_ways) { 
 	if (num_ways == 1) return;
 	ifstream *f_in = new ifstream[num_ways];
    for (rint i = 0; i < num_ways; i++) { 
  		int cnt = 0;
  		my_itoa(i, fileName, cnt);
  		f_in[i].open(fileName);
    } 
 	ofstream f_out(output_file);
 	f_out.clear();

 	priority_queue<node, vector<node>, myComparator> min_heap; 
  
    for (rint i = 0; i < num_ways; i++) { 
  		node temp;
	  	if (!isEmptyFile(f_in[i])) {
   			getline(f_in[i], temp.element);
   			temp.index = i;
   			min_heap.push(temp);
  		}
    } 

    int count = 0; 
  
 	while (count != num_ways) { 
  		node root = min_heap.top();
  		min_heap.pop();
  		if (root.element.size())
   			f_out<<root.element<<"\n";

  		if(f_in[root.index].eof()) {
            ++count; 
   			continue;
  		}
  		else 
   			getline(f_in[root.index], root.element);
   			min_heap.push(root);
    } 

 	for (rint i = 0; i < num_ways; i++) {
  		f_in[i].close(); 
  		int cnt = 0;
  		my_itoa(i, fileName, cnt); 
  		remove(fileName);
 	}
 	f_out.close(); 
} 

void externalSort(string input_file, string output_file, unsigned long long mem_limit) { 
 	int num_ways;
    createInitialRuns(input_file, output_file, mem_limit, num_ways); 
 	mergeFiles(output_file, num_ways); 
} 

unsigned long long isValidMem(string mem) {
 	int len = mem.size();
 	for (rint i = 0; i < len; i++) {
  		if (mem[i] < '0' || mem[i] > '9') {
   			printf("Invalid value!\n");
   			return false;
  		}
 	}
 	if (len > 10) {
  		printf("Too large!\n");
  		return false;
 	}
 	unsigned long long rs = 0;
 	for (rint i = 0; i < len; i++) {
  		rs = rs*10 + (mem[i] - '0');
 	}
 	if (rs > limited_memory) {
 		printf("Too large!\n");
 		return false;
	}
 	return rs;
}

bool isValidOutputFile(string output_file) {
 	int len = output_file.size();
 	for (rint i = 0; i < len; i++) {
  		if (output_file[i] == '/' || output_file[i] == '\\' || output_file[i] == ':' ||
   			output_file[i] == '*' || output_file[i] == '?' || output_file[i] == '"' || 
   			output_file[i] == '<' || output_file[i] == '>' || output_file[i] == '|') {
   				printf("A file name can't contain: \\ / : * ? %c < > | or special character!\n", 34);
   				return false;
  		}
 	}
 	return true;
}

void genString(char *arr) {
 	int len = 1 + rand() % 1000, value = 0;
 	for (rint i = 0; i < len; i++) {
  		value = 1 + rand() % 9;
  		arr[i] = value + '0';
 	}
 	arr[len] = '\0';
}

int main() { 
 	std::ios::sync_with_stdio(false);

 	string mem_limit; 
    string input_file;
    string output_file; 

 	printf("Input file name: ");
 	cin>>input_file;
 	while (!isFileExists(input_file)) {
  		printf("File does not exist!\nInput file name: ");
  		cin>>input_file;
 	}

 	printf("Output file name: ");
 	cin>>output_file;
 	while (!isValidOutputFile(output_file)) {
  		printf("Output file name: ");
  		cin>>output_file;
 	}

 	printf("Memory limit (bytes): ");
 	cin>>mem_limit;
 	unsigned long long mem;
 	while (!(mem = isValidMem(mem_limit))) {
  		printf("Memory limit (bytes): ");
  		cin>>mem_limit;
 	}
 
 	/*
 	// Generate text file
    ofstream f_out(input_file);
    for (int i = 0; i < 100000; i++) {
  		f_out.clear();
  		genString(fileName);
  		f_out<<fileName<<'\n';
 	}
 	f_out.close();
 	*/

 	externalSort(input_file, output_file, mem); 
 	printf("Done!!!\n");
    return 0; 
}
