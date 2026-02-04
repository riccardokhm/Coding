// Read and copy the content of a file.
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

using namespace std;
int main(int argc, char const *argv[])
{
    char ch;
    char copy_mode = 0;

    ofstream output_file;
    ifstream input_file;

    if (argc != 4)
    {
        cerr << "Usage: " << argv[0] << " <file_to_copy> <output_file> <copy_mode>" << endl;
        return 1;
    }
   
    if (strcmp(argv[3], "binary") == 0)
        copy_mode = 'b';
    else if (strcmp(argv[3], "textual") == 0)
        copy_mode = 't';
    else {
        cerr << "Unknown copy mode: " << argv[3] << endl;
        return 1;
    }
    
    switch (copy_mode)
    {
    case 'b':
        input_file.open(argv[1], ios::in | ios::binary);
        output_file.open(argv[2], ios::out | ios::binary);

        if (!input_file) {
            cerr << "Error opening input file: " << argv[1] << endl;
            return 1;
        }
        if (!output_file) {
            cerr << "Error opening output file: " << argv[2] << endl;
            return 1;
        }

        while (input_file.get(ch))
            output_file.put(ch);

        break;
    
    case 't':
        input_file.open(argv[1]);
        output_file.open(argv[2]);

        if (!input_file) {
            cerr << "Error opening input file: " << argv[1] << endl;
            return 1;
        }
        if (!output_file) {
            cerr << "Error opening output file: " << argv[2] << endl;
            return 1;
        }

        while (input_file.get(ch)) {
            cout << ch;
            output_file.put(ch);
        }

        break;
    }

    input_file.close();
    output_file.close();

    return 0;

    
}