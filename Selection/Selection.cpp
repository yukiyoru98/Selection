#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int Selection(vector<int>&, int, int, int);
int MedianOfMedians(vector<int>&, int, int);
void Swap(vector<int>&, int, int);
int Partition(vector<int>&, int, int);
void InsertionSort(vector<int>&, int, int);

int main() {

	//===========Read Input File===========
	ifstream ReadFile("input.txt", ios::in);
	//check if file exists
	if (!ReadFile.is_open()) {
		cerr << "Input File not found.";
		exit(1);
	}

	int n = 0; // input size
	int rank = 0; // the rank of the element to search for
	vector<int> vec(1); //store the keys

	ReadFile >> n; // read first line(numbers to sort)
	ReadFile >> rank; // read second line(rank to search for)

	for (int i = 1; i <= n; i++) { // read the numbers and save them into vec
		int x;
		ReadFile >> x;
		vec.push_back(x);
	}

	//===========Do Selection===========
	int ans = Selection(vec, 1, n, rank);

	//===========Write Output File===========
	ofstream WriteFile("output.txt", ios::out | ios::trunc);
	if (!WriteFile.is_open()) {
		cerr << "Output File not found.";
		exit(1);
	}

	WriteFile << "Size of array: " << n << "\n";
	WriteFile << "The element of rank " << rank << " is: " << vec[ans] << "\n";

	return 0;
}

int Selection(vector<int>& vec, int start, int end, int select) {
	int size = end - start + 1; //size of the array
	
	//if fewer than 5 elements
	if (size <= 5) { 
		InsertionSort(vec, start, end); //do insertion sort
		return select;
	}

	//if more than 5 elements
	//find median of medians as pivot, and partition the array 
	int pivot = MedianOfMedians(vec, start, end);
	Swap(vec, end, pivot); // put the pivot to the end of array
	pivot = Partition(vec, start, end); //do partition to get the index of the pivot

	if (pivot == select) { //pivot is the index we are looking for
		return pivot;
	}
	else if (pivot < select) { //the index to find is in the larger section
		Selection(vec, pivot + 1, end, select);
	}
	else { //the index to find is in the smaller section
		Selection(vec, start, pivot - 1, select);
	}

}

int MedianOfMedians(vector<int>& vec, int start, int end) {

	int groups = (end - start + 1) / 5; //divide the array into groups of 5
	if ((end - start + 1) % 5 != 0) { //if cannot be divided evenly(i.e. the last group has fewer than 5 elements)
		groups++; //manually add 1 group
	}

	int i;
	//Use Selection to find the medians in each group and place it in the front of array
	for (i = 0; i < groups; i++) {
		int sub_start = start + 5 * i; //the start index of the group
		int sub_end = ((i+1) == groups) ? end : sub_start + 4; //the end index of the group
		int med = (sub_start + sub_end) / 2; //the median's index of the group
		med = Selection(vec, sub_start, sub_end, med);  // select median
		Swap(vec, med, start + i); //place median at the front of array
	}

	// last median's position is start + i - 1
	// now the medians positions are from start to start + i - 1
	// Use Selection to find the median of medians
	return Selection(vec, start, start + i - 1, (start + start + i - 1) / 2);
}

void Swap(vector<int> &vec, int a, int b) { // swap two of the elements in a vector
	int tmp = vec[a];
	vec[a] = vec[b];
	vec[b] = tmp;
}

int Partition(vector<int> &vec, int start, int end) { // the first element is chosen as pivot

	//i: the index where the last element smaller than pivot is
	int i = start - 1; //-1 since all the other elements might be larger the pivot

	for (int j = start; j < end; j++) { //compare all the other elements with pivot
		if (vec[j] <= vec[end]) {//if smaller
			i++;//increment i, meaning that one more element smaller than pivot is found
			Swap(vec, i, j);//swap it with the ith element
		}
	}
	Swap(vec, i + 1, end); //insert pivot between the smaller part and the larger part

	return i + 1; //return the pivot's position
}

void InsertionSort(vector<int> &vec, int start, int end) {

	for (int k = start + 1; k <= end; k++) {//select index of key(starting from the second element)
		int key = vec[k]; //record key
		int cmp = k - 1; //compare with all elements before key
		while (cmp >= start && vec[cmp] > key) { //if not out of range and element is larger than key
			vec[cmp + 1] = vec[cmp]; //move this element backwards
			cmp--; //continue comparing with the previous element
		}
		/*the loop would be broken in either of the following cases:
		 (1)out-of-range problem: key is smaller than all the other elements before it
		 ->insert key into the front
		 (2)the element compared is smaller->insert key into the position behind that element
		 */
		vec[cmp + 1] = key;
	}
}