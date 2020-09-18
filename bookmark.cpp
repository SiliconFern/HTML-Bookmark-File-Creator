#include <iostream>
#include <cstdio>
#include <string>
using namespace std;


// Show the function help:
void show_help(string strProgName)
{
	cout << "USAGE:  " << strProgName << " [-d] web_address name" << endl;
	cout << "        " << strProgName << " -h|v" << endl;
	cout << endl;
	cout << "           -h   Show usage/help." << endl;
	cout << "           -v   Show the version number." << endl;
	cout << "           -d   Place the bookmark on the desktop, instead of the" << endl;
	cout << "                current directory." << endl;
	cout << "  web_address   Is just the URL, like https://example.io" << endl;
	cout << "         name   The name of the bookmark file." << endl;
	cout << endl;
	cout << "The program creates a bookmark file, by default in the curren directory." << endl;
	cout << endl;
	cout << "The file is a very simple, stripped HTML file with" << endl;
	cout << "<!DOCTYPE Internet-bookmark> to define the purpose of the document. It uses the" << endl;
	cout << "deprecated extension .htm instead of .html just as a way to indicate at the" << endl;
	cout << "first sight the purpose of the file, since almost every other HTML files has" << endl;
	cout << "the four letter extension." << endl;
	cout << endl;
	cout << "At the moment, nothing in the function controls the way the file is written, so" << endl;
	cout << "it could be system dependent: new lines as CR,NL, ANSI OR UTF-16 file, etcetera." << endl;
	cout << "And it is needed to create a way for the program to Check if the entered URL is" << endl;
	cout << "a valid one, as well as the name." << endl;
}

// Extract the path and program name form the global variable _pgmptr.
// Return  -2:  The global variable _pgmptr holds not the full qualified path to the program.
//         -1:  Coudn't retrieve the path, neither the file name.
//          0:  No errors in the execution, no bookmark created.
//          1:  Bookmark succesfully created.
int get_prog_pathname(string &strProgPath, string &strProgName)
{
	string strPgmPtr = string(_pgmptr);
	string::size_type split = strPgmPtr.find_last_of("/\\");
	string::size_type strip = strPgmPtr.find_last_of(".");
	string temp = strPgmPtr.substr(0,strip);
	strProgPath = strPgmPtr.substr(0,split);
	strProgName = temp.substr(split+1,strip);

	return 1;
}

// Get the USERPROFILE path.
// Return  -1:  Can't retrive the path.
//          0:  Success.
int get_user_path(string &strUserPath)
{
	char *buffer;
	buffer = getenv("USERPROFILE");
	if(buffer == NULL) return -1;
	else { strUserPath = string(buffer); return 0; }
}


// MAIN.CPP DEFINES THE ENTRY POINT OF THE PROGRAM.
// Return:  -2:  Failed to create the bookmark file.
//          -1:  No bookmark created because a file with the same file name already exists.
//           0:  Execution succesful, but no bookmark was created.
//           1:  Bookmark file created succesfully.
int main(int argc, char const *argv[])
{
	string strProgVersion("0.1");
	string strProgName;
	string strProgPath;
	string strDesktopPath;
	string strUrl;
	string strBookmarkName;

	if(get_prog_pathname(strProgPath, strProgName) != 1) strProgName = "this_program_name";

	// Check the parameters:	
	if(argc < 3 || argc > 4)
	{
		if(argc == 2)
		{
			if(string(argv[1]) == "-h" || string(argv[1]) == "--help") { show_help(strProgName); return 0; }
			if(string(argv[1]) == "-v" || string(argv[1]) == "--ver") { cout << "Program version: " << strProgVersion << endl; return 0; }
			if(string(argv[1]).at(0) == '-') { cout << "Incorrect parameter." << endl; }
		}
		else cout << "  Incorrect number of parameters." << endl;

		cout << "  To get help about this program, type: " << strProgName << " -h" << endl;
		return 0;
	}

	// SHOULD BE A GREAT IDEA TO PLACE HERE SOME CODE TO CHECK IF THE URL
	// HAS A VALID STRUCTURE.
	// ...
	// .

	strUrl = string(argv[argc-2]);

	// If the bookmark is to be created on the desktop, prepare the path:
	if(string(argv[1]) == "-d")
	{
		get_user_path(strBookmarkName);
		strBookmarkName.append("\\Desktop\\");
	}
	// Complete the file name string:
	strBookmarkName.append(string(argv[argc-1]));
	strBookmarkName.append(".htm");


	// Create the bookmark file:
	FILE *pFile;
	// Try to open a file with the selected name to avoid overwriting it if it does exist:
	pFile = fopen(strBookmarkName.c_str(), "r");
	if(pFile != NULL)
	{
		fclose(pFile);
		cout << "  The selected file name already exists." << endl;
		cout << "  Please select other name for your bookmark." << endl;
		cout << "  Bookmark file not created." << endl;
		return -1;
	}
	// Open the file:
	pFile = fopen(strBookmarkName.c_str(), "w");
	if(pFile == NULL) { perror("Error creating the bookmark file"); return -2; }
	// Write the file:
	fprintf(pFile, "<!DOCTYPE Internet-bookmark>\n");
	fprintf(pFile, "<META HTTP-EQUIV=\"REFRESH\" CONTENT=\"0; URL=");
	fprintf(pFile, "%s\">\n", strUrl.c_str());
	// Close the file:
	fclose(pFile);

	return 1;
}