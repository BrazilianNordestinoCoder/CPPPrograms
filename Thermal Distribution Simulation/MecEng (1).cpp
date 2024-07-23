/**********************************************************
Author: Pedro Felix Fernandes
Date Created : November 10, 2023
Description:
___________________________________________________
Assignment 2 - Mechanical Engineering Case Study – Thermal Distribution Simulation
___________________________________________________
Mechanical Engineering Case Study – Thermal Distribution Simulation:
The program simulates the temperature distribution in a thin metal plate with constant (isothermal) temperatures
on each side associated with the thermal stimulation, using a 2D grid with unknown dimensions (number of rows and columns).
1) Create the dynamic 2D grid.
2) Set initial side temperature values for the grid.
3) Stimulate the grid at a specific point.
4) Calculate the thermal distribution based on the specific stimulation and initial conditions.
5) Print the thermal grid on the screen.
6) Exit the program.
***************************************************/

// Including Libraries and Header Files
#include <iostream>
#include <cmath>
#include <iomanip>
#include "Header.h"
// defining standard namespace
using namespace std;

// functions that will be used on the program
double** create2DGrid(int rows, int cols);
void setsideTemperature(double** grid, int rows, int cols);
void stimulateGrid(double** grid, int rows, int cols);
void displayGrid(double** grid,int rows, int cols);
void simulateThermalDissipation(double** grid, int rows, int cols, double tolerance);
void deleteGrid(double** grid, int rows);

// main function
int main() {
	// global variables
	double toler = 0;
	int input = 0;
	int row = 0;
	int col = 0;
	double **arr = nullptr;

	// menu options
	cout << "Select one of the options below to simulate the thermal distribution in a thin metal plate with constant(isothermal)" << endl;
	cout << "1) Create the dynamic 2D grid." << endl;
	cout << "2) Set initial side temperature values for the grid." << endl;
	cout << "3) Stimulate the grid at a specific point." << endl;
	cout << "4) Calculate the thermal distribution based on the specific stimulation and initial conditions." << endl;
	cout << "5) Print the thermal grid on the screen." << endl;
	cout << "6) Exit the program." << endl;

	while (true) {
		cout << "Select one of the options of the main menu above " << endl;

		while (!(cin >> input)) { // validation of the input
			cin.clear(); //error flag clearing
			cin.ignore(numeric_limits<streamsize>::max(), '\n');// discard the invalid input
		}
		switch (input) {
			// Option 1 - Create a grid
		case 1:
			cout << "Inform the number of rows: " << endl;
			cin >> row;
			cout << "Inform the number of columns: " << endl;
			cin >> col;
			
			arr=create2DGrid(row, col); // call function to create the 2D grid using input values

			cout << "Grid created" << endl; // Creation confirmation
			break;
		//Option 2 - Set initial side temperature values for the grid.
		case 2:
			if (arr == nullptr) { // validation if option 1 was made before
				cout << "The creation of the grid is needed for this step" << endl;
			}
			else {
				setsideTemperature(arr, row, col); // call function to set temperature on the sides
				cout << "Temperature setted" << endl;
			}
			break;
		// Option 3 - Stimulate the grid at a specific point.
		case 3:
			if (arr == nullptr) { // validation if option 1 was made before
				cout << "The creation of the grid is needed for this step" << endl;
			}
			else {
				stimulateGrid(arr, row, col); // Call function to stimulate the Grid
				displayGrid(arr, row, col); // Call function to print the grid
			}
			break;
		// Option 4 - Calculate the thermal distribution based on the specific stimulation and initial conditions.
		case 4:
			if (arr == nullptr) { // validation if option 1 was made before
				cout << "The creation of the grid is needed for this step" << endl;
			}
			else {
				cout << "Enter the simulation tolerance (a positive value)"; // simulation tolerance (treshold)
				while (!(cin >> toler)){
					cout << "Invalid input, enter a number:" << endl;
					cin.clear(); // clear error flag
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
				}
				simulateThermalDissipation(arr, row, col, toler);//Call function to make the Thermal Dissipation of the grid
				displayGrid(arr, row, col); // call the function to print the grip

				// Store the resulting thermal distribution data on an image file.
				const char* filename = "Outputimage.bmp";
				uint8_t** image = new uint8_t * [row];
				for (int i = 0; i < row; ++i) {
					image[i] = new uint8_t[col];
					for (int j = 0; j < col; ++j) {
						image[i][j] = static_cast<uint8_t>(arr[i][j]);
					}
				}
				writeBitmap(filename, image, col, row);// Call function to write bit map of image
				cout << "Thermal distribution saved as an image." << endl;

				// Delete dynamic array of image
				for (int i = 0; i < row; ++i) {
					delete[] image[i];
				}
				delete[] image;
			}
			break;
			// Option 5 - Print the thermal grid on the screen.
		case 5:
			if (arr == nullptr) { // validation if option 1 was made before
				cout << "The creation of the grid is needed for this step" << endl;
			}
			else {
				displayGrid(arr, row, col); // Call fucntion to print grid
			}
			break;
			//Option 6 - Exit the program
		case 6:
			cout << "exited the program" << endl;
			deleteGrid(arr, row); // Call function to delete dynamic grid
			return 0;
		default:
			cout << "Invalid option, try again" << endl;
			break;

		}
	}
}

void simulateThermalDissipation(double** grid, int rows, int cols, double tolerance) // function to simulate thermal dissipation
{
	bool thermEquilibrium = false; // since the thermal equilibrium wasn't reached, we define as false
	do { // Thermal distribution calculation:
			double maxchange = 0;
			for (int i = 1; i < rows - 1; i++) {
			for (int j = 1; j < cols - 1; j++) {
				double temp = (grid[i][j + 1]
					+ grid[i][j - 1]
					+ grid[i + 1][j]
					+ grid[i - 1][j])/4;
				double chng = abs(grid[i][j] - temp);
				if (chng > maxchange) {
					maxchange = chng;
				}
				grid[i][j] = temp;
			}
		}

		if (maxchange <= tolerance) {
			thermEquilibrium = true; // When the thermal distribution is reached, the value is true
		}
		// Ensure that temperatures are within a valid range (0 to 255) for image conversion
		for (int i = 0; i < rows; i++) {
			for (int j = 0; j < cols; j++) {
				if (grid[i][j] < 0) {
					grid[i][j] = 0;
				}
				else if (grid[i][j] > 255) {
					grid[i][j] = 255;
				}
			}
		}
	} while (!thermEquilibrium);
}

// Print the Grid
void displayGrid(double** grid, int rows, int cols) {
	cout << fixed << setprecision(2);
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cout << grid[i][j] << "\t";
		}
		cout << endl;
	}
}

// Stimulate the Grid
void stimulateGrid(double** grid, int rows, int cols) {
	int a, b;
	double temp;
	do { // Ask for the coordinates and value of temperature of the stimulation
		cout << "Please inform the row of the grid where do you want to make the stimulation" << endl;
		cin >> a;
		cout << "Please inform the column of the grid where do you want to make the stimulation" << endl;
		cin >> b;
		cout << "Please inform the temperature value of the stimulation" << endl;
		cin >> temp;

		// Invalid Option verifier
		if (b<1 || b>cols || a<1 || a>rows) {
			cout << "Invalid option, please select the values again" << endl;
		}
	} while (b<1 || b>cols || a<1 || a>rows);
		grid[a][b] = temp;
}

// Set the temepratures of the sides of the grid
void setsideTemperature(double** grid, int rows, int cols) {
	//getting the temperatures from the user
	double temp[4];
	cout << "Please inform the initial temperature of the top of the grid" << endl;
	cin >> temp[0];
	cout << "Please inform the initial temperature of the bottom of the grid" << endl;
	cin >> temp[1];
	cout << "Please inform the initial temperature of the left of the grid" << endl;
	cin >> temp[2];
	cout << "Please inform the initial temperature of the right of the grid" << endl;
	cin >> temp[3];

	// insert values on the top
	for (int i = 0; i < cols; i++) {
		grid[0][i] = temp[0];
	}
	// insert the values on the bottom
	for (int i = 0; i < cols; i++) {
		grid[rows-1][i] = temp[1];
	}
	// insert the values on the left
	for (int i = 1; i < rows - 1; i++) {
		grid[i][0] = temp[2];
	}
	// insert the values on the right
	for (int i = 1; i < rows - 1; i++) {
		grid[i][cols-1] = temp[3];
	}

}

// Create 2D grid
double** create2DGrid( int rows, int cols) {
	double** grid = new double* [rows];
	for (int i = 0; i < rows; i++) {
		grid[i] = new double [cols];
	}

	// define all values of the grid to 0, after creating it
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			grid[i][j] = 0;
		}
	}
	return grid;
}

// Delete the dynamic grid - memory deallocation
void deleteGrid(double** grid, int rows) {
	for (int i = 0; i < rows; ++i) {
		delete[] grid[i];
	}
	delete[] grid;

}