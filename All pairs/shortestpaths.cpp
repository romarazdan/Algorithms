/*******************************************************************************
 * Name        : shortestpaths.cpp
 * Author      : Gregory Baumann and Roma Razdan
 * Date        : 12/07/20
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System.
 ******************************************************************************/

#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <sstream>
#include <limits>
#include <iomanip>

using namespace std;

long** pathLen;
long** interVert;
long long INF = 4294967296;
int num_vertices;
queue<long> reverse;

void findpath(int start, int end){
	if(interVert[start][end] != INF){
		findpath(start,interVert[start][end]);
		reverse.push(interVert[start][end]);
		findpath(interVert[start][end],end);
	}
}

void printPaths(){
	char start,current, end;
	long distance;
	for(int i = 0; i < num_vertices; i++){
		start = 'A' + i;
		for(int j = 0; j < num_vertices; j++){
			end = 'A' + j;
			cout << start << " -> " << end << ", ";
			if(pathLen[i][j] == INF){
				cout << "distance: infinity, path: none";
			}else if(i == j){
				cout << "distance: 0, path: " << start;
			}else{
				distance = pathLen[i][j];
				cout << "distance: " << distance << ", path: ";
				cout << start;
				findpath(i,j);
				while(!reverse.empty()){
					current = 'A' + reverse.front();
					cout << " -> " << current;
					reverse.pop();
				}
				cout << " -> " << end;
			}
			cout << endl;
		}
	}
}

long** Floyd(long** const disMatrix){

	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			pathLen[i][j] = disMatrix[i][j];
		}
	}

	for(int i = 0; i < num_vertices; i++) {
		for(int j = 0; j < num_vertices; j++) {
			for(int k = 0; k < num_vertices; k++){
				if(pathLen[j][k] > pathLen[j][i] + pathLen[i][k]){
					pathLen[j][k] = pathLen[j][i] + pathLen[i][k];
					interVert[j][k] = i;
				}
			}
		}
	}
	return pathLen;
}


/**
* Displays the matrix on the screen formatted as a table.
*/

int len(long int num){
	int length = 0;
	while(num%10 != 0){
		num -= 1;
	}
	if(num == 0){
		return 1;
	}
	while(num != 0){
		length++;
		num = num / 10;
	}
	return length;
}
void display_table(long** const matrix, const string &label, const bool use_letters = false){
	//Prints the distance, intermediate tables.
	cout << label << endl;
	long max_val = 0;

	for(int i = 0; i < num_vertices; i++){
		for(int j = 0; j < num_vertices; j++){
			long cell = matrix[i][j];
			if(cell < INF && cell > max_val) {
				max_val = matrix[i][j];
			}
		}
	}
	int max_cell_width = use_letters ? len(max_val) :
			len(max(static_cast<long>(num_vertices), max_val));
	cout << ' ';
	for(int j = 0; j < num_vertices; j++){
		cout<< setw(max_cell_width + 1) << static_cast<char>(j + 'A');
	}
	cout << endl;
	for(int i = 0; i < num_vertices; i++){
		cout << static_cast<char>(i + 'A');
		for(int j = 0; j < num_vertices; j++){
			cout<< " " << setw(max_cell_width);
			if(matrix[i][j] == INF){
				cout << "-";
			} else if(use_letters){
				cout << static_cast<char>(matrix[i][j] + 'A');
			} else {
				cout << matrix[i][j];
			}

		}
		cout << endl;
	}
	cout << endl;
}

int main(int argc, char *argv[]) {
	int numVerticies, lineNum;
	lineNum = 1;
	istringstream iss;
	long** disMatrix;
	if(argc != 2){
		cerr << "Usage: ./shortestpaths <filename>" << endl;
		return 0;
	}
	ifstream txtfile(argv[1]);
	string line;
	if(txtfile.fail()){
		cerr << "Error: Cannot open file '" << argv[1] <<  "'." << endl;
		return 0;
	}
	disMatrix = new long*[numVerticies];
	while (getline(txtfile, line)){
		if(lineNum == 1){
			iss.str(line);
			if(!(iss >> numVerticies) || numVerticies < 1 || numVerticies > 26){
				cerr << "Error: Invalid number of vertices '" << line << "' on line 1." << endl;
				return 0;
			}
			iss.clear();
			pathLen = new long*[numVerticies];
			disMatrix = new long*[numVerticies];
			interVert = new long*[numVerticies];
			for(int i = 0; i < numVerticies; i++){
				disMatrix[i] = new long[numVerticies];
				pathLen[i] = new long[numVerticies];
				interVert[i] = new long[numVerticies];
			}
			for(int i = 0; i < numVerticies; i++){
				for(int j = 0; j < numVerticies; j++){
					if(i == j){
						disMatrix[i][j] = 0;
					}else{
						disMatrix[i][j] = INF;
					}
					interVert[i][j] = INF;
				}
			}
		}else{
			string word,char1,char2,num;
			int len;
			queue<string> in_queue;
			stringstream ss(line);
			char maxVert = numVerticies + 64;
			while (ss >> word){
				in_queue.push(word);
			}
			ss.clear();
			if(in_queue.size() != 3){
				cerr << "Error: Invalid edge data '" << line <<"' on line "<< lineNum << "."<< endl;
				return 0;
			}
			char1 = in_queue.front();
			in_queue.pop();
			if(char1[0] < 65 || char1[0] > numVerticies + 64){
				cerr << "Error: Starting vertex '" << char1 << "' on line " << lineNum << " is not among valid values A-" << maxVert << "." << endl;
				return 0;
			}
			char2 = in_queue.front();
			in_queue.pop();
			if(char2[0] < 65 || char2[0] > numVerticies + 64){
				cerr << "Error: Ending vertex '" << char2 << "' on line " << lineNum << " is not among valid values A-" << maxVert << "." << endl;
				return 0;
			}
			num = in_queue.front();
			in_queue.pop();
			ss.str(num);
			if(!(ss >> len) || len <= 0){
				cerr << "Error: Invalid edge weight '"<< num << "' on line " << lineNum << "." << endl;
				return 0;
			}
			ss.clear();
			disMatrix[char1[0] - 65][char2[0] - 65] = len;
		}
		lineNum++;
	}
	txtfile.close();
	num_vertices = numVerticies;
	display_table(disMatrix, "Distance matrix:");
	display_table(Floyd(disMatrix), "Path lengths:");
	display_table(interVert, "Intermediate vertices:", true);
	printPaths();
	for(int i = 0; i < numVerticies; i++){
		delete [] disMatrix[i];
		delete [] pathLen[i];
		delete [] interVert[i];
	}
	delete [] disMatrix;
	delete [] pathLen;
	delete [] interVert;
	return 0;
}

