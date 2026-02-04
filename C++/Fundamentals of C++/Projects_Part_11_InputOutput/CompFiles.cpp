// Program to compare two different files and returning true if they are
// equals, otherwise false

#include <iostream>
#include <fstream>

using namespace std;

int main (int argc, char const *argv[]){

    ifstream file1, file2;
    int i, bufferCount;
    unsigned char buf1[1024], buf2[1024];

    if (argc != 3)
    {
        cout << "File usage: <fileToCompare1> <fileToCompare2>";
        return 1;
    }

    cout << "Starting comparing the two files provided..." << endl;

    file1.open(argv[1], ios::in | ios::binary);
    if (!file1)
    {
        cout << "Error while opening file " << argv[1];
        return 1;
    }

    file2.open(argv[2], ios::in | ios::binary);
    if (!file2)
    {
        cout << "Error while opening file " << argv[2];
        return 1;
    }

    cout << "Comparing the two files..." << endl;

    do
    {
       file1.read((char *) buf1, sizeof buf1);
       file2.read((char *)buf2, sizeof buf2);

       if (file1.gcount() != file2.gcount()){
        cout << "Files have different size and so they are not equivalent..." << endl;
        cout << "Closing the stream";
        file1.close();
        file2.close();
        return 0;
       }

       for ( i = 0; i < file1.gcount(); i++)
       {
            if (buf1[i] != buf2[2])
            {
                cout << "Files are different in content at byte "<< bufferCount * 1024 + i << endl;
                cout << "Closing the stream";
                file1.close();
                file2.close();

                return 1;
            }  
       }

       bufferCount++;
       
    } while (!file1.eof() && !file2.eof());
    
    cout << "Files are equal..." << endl;

    file1.close();
    file2.close();
    
    return 0;
}