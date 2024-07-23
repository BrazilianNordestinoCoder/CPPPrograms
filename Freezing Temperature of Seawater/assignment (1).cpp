/*********************************************************
Author: Pedro Felix Fernandes
Date Created: September 08, 2023
Description:
___________________________________________________
Assignment 1 - Civil Engineering Case Study – Freezing Temperature of Seawater
___________________________________________________
Civil Engineering Case Study – Freezing Temperature of Seawater:
The program reads a data file with data of different concentrations of salinity
and Freezing temperatures. It calculates the model parameters following the data provided.
After that, the code asks for a salinity value from the user and calculates the freezing temperature
***********************************************************/

// Include Libraries
#include <iostream>
#include <fstream> // for reading of data files 
#include <string>

//use standard namespace
using namespace std;

// Main Function
int main() {
	//Inputs
	double m = 0, b = 0;
	int optionverif = 0;
	int option = 0;
	// Output
	cout << " Welcome to the calculator of the Freezing Temperature of Seawater, please choose an option:" << endl
		<< "1 - Calculate the linear regression model using the data stored in a file." << endl
		<< "2 - Calculate a freezing temperature given the water salinity using the linear model of option 1" << endl
		<< "3 - exit the program" << endl;
	//Option selection Input
	cin >> option;

	// Infinite loop while option is different from 3
	while (option != 3) {

		// Option 1 selected - Calculate the Model Parameters
		if (option == 1) {

			//Creation of input file stream
			ifstream inFile;
			//Inputs
			string filename;
			double x, y;

			// User inform name of the file and stores it on filename
			cout << "Please inform the name of the file, with its extension, that have the data to calculate the linear model" << endl;
			cin >> filename;

			// Open the file informed by the user
			inFile.open(filename , ios::in);

			// If opening of the fail fails, close the program
			if (inFile.fail()) {

				cerr << "Failed to open input file";
				exit(-1);

			}
			// Creation of the sums to calculate the Model Parameters
			double sumx(0), sumy(0), sumxx(0), sumxy(0);
			double count = 0;

			// Creation of variable to store the headers of the life
			string header;
			getline(inFile, header);

			// Read the information of the file while it doesn't reach the end of the file
			while(!inFile.eof()){
				// Assign the first value to x and the second to y
				inFile >> x >> y;
				// Sum the values of x
				sumx += x;
				// Sum the values of y
				sumy += y;
				// Sum the values of x*x
				sumxx += (x*x);
				// Sum the values of x*y
				sumxy += (x*y);
				// Increment counter - Define how many lines of values were on the data file
				count++;
			}
			// Close file
			inFile.close();

			// Create inputs to store values
			double m1,m2,b1,b2;

			//Calculate the upper side and lower side of m formula
			m1 = (sumx * sumy) - (count * sumxy);
			m2 = (sumx * sumx) - (count * sumxx);
			//Find the value of model parameter m
			m = m1 / m2;
			//Calculate the upper and bottom side of the b formula
			b1 = (sumx * sumxy) - (sumxx * sumy);
			b2 = (sumx * sumx) - (count * sumxx);
			// Find the value of the model parameter b
			b = b1 / b2;

			// Inform values of the model parameters to user
			cout << "Model parameter m is " << m << ". Model parameter b is " << b << endl<< endl;

			// Prompt the user to select another option of the menu
			cout << "Please select one of the options of the menu to move forward "<< endl
				<< "1 - Calculate the linear regression model using the data stored in the file." << endl
				<< "2 - Calculate a freezing temperature given the water salinity using the linear model of option 1" << endl
				<< "3 - exit the program" << endl;

			// Declare a variable to verify if option 1 was selected before option 2
			optionverif++;



		}

		// Option 2 selected - Calculate the Freezing temperature with the model parameter m and b, with the salinity informed by the user
		else if (option == 2) {

			// Verification if the option 1 was selected before
			if (optionverif != 1) {
				cout << "ERROR, value of the model parameters not found, please select 1 to calculate them." << endl;
			}
			// If option 1 was selected before the program continues
			else if (optionverif == 1) {
				// Input
				float sal;
				// Prompt the user for salinity value
				cout << "Please inform a salinity value to calculate the freezing temperature " << endl;
				// Store salinity value
				cin >> sal;
				
				// Create vulnerable to store Freezing Temperature
				float Ftemp;

				Ftemp = (m * sal) + b;

				// Inform the user the Freezing temperature of the informed salinity and model parameters of m and b
				cout << "Freezing temperature is " << Ftemp << endl << endl;

				// Prompt the user for the following step
				cout << "Please select one of the options of the menu to move forward " << endl
					<< "1 - Calculate the linear regression model using the data stored in the file." << endl
					<< "2 - Calculate a freezing temperature given the water salinity using the linear model of option 1" << endl
					<< "3 - exit the program" << endl;
			}

		}
		// If the user selects another option that is not 1 or 2, prompts the user for the another option selection
		else cout << "Invalid menu option, please choose an option:" << endl
			<< "1 - Calculate the linear regression model using the data stored in the file." << endl
			<< "2 - Calculate a freezing temperature given the water salinity using the linear model of option 1" << endl
			<< "3 - exit the program" << endl;
		cin >> option;



	}
	// If user selects option 3, display message and exit the program
	cout << "Calculation ended, Program exited";

	return 0;
}