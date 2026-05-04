#include <iostream>
#include <string>
#include <fstream>

using namespace std;

struct Stack {
    string arr[100];
    int top;
};

void initStack(Stack &s) {
    s.top = -1;
}

bool isEmpty(Stack &s) {
    return s.top == -1;
}

bool isFull(Stack &s) {
    return s.top == 99;
}

void push(Stack &s, string value) {
    if (!isFull(s)) {
        s.top++;
        s.arr[s.top] = value;
    }
}

string pop(Stack &s) {
    if (!isEmpty(s)) {
        string value = s.arr[s.top];
        s.top--;
        return value;
    }
    return "";
}

void clearStack(Stack &s) {
    s.top = -1;
}

int main() {
    string text = "";
    string lastSavedFile = "";  // Store the last filename used for saving
    Stack undoStack;
    Stack redoStack;

    initStack(undoStack);
    initStack(redoStack);

    int choice;
    string input;
    string searchWord;
    int deleteCount;
    int position;
    string filename;

    push(undoStack, text);
    cout<<"Welcome to Text Writing and Editor Program"<<endl;

    do {
        cout << "\nMENU" << endl;
        cout << "----" << endl;
        cout << "1. Insert text at cursor" << endl;
        cout << "2. Delete N chars at cursor" << endl;
        cout << "3. Clear the whole text" << endl;
        cout << "4. Search" << endl;
        cout << "5. Undo" << endl;
        cout << "6. Redo" << endl;
        cout << "7. Save to directory as a .txt file" << endl;
        cout << "8. Save (overwrite the current)" << endl;
        cout << "9. Load from file" << endl;
        cout << "10. Move cursor" << endl;
        cout << "11. Show Text (with status of the text)" << endl;
        cout << "12. Exit" << endl;
        cout << "Enter choice (1-12): ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input. Enter number 1-12." << endl;
            continue;
        }

        if (choice < 1 || choice > 12) {
            cout << "Invalid choice. Enter number 1-12." << endl;
            continue;
        }

        cin.ignore();

        if (choice == 1) {
            cout << "Enter text to insert at cursor: ";
            getline(cin, input);

            if (input.empty()) {
                cout << "Cannot insert empty text." << endl;
                continue;
            }

            push(undoStack, text);
            clearStack(redoStack);

            int cursorPos = text.length();
            text = text + input;
            cout << "Text inserted successfully at position " << cursorPos << "." << endl;
        }
        else if (choice == 2) {
            if (text.empty()) {
                cout << "Nothing to delete. Document is empty." << endl;
                continue;
            }

            cout << "Enter number of characters to delete from cursor (end): ";
            cin >> deleteCount;
            cin.ignore();

            if (deleteCount <= 0) {
                cout << "Invalid count." << endl;
                continue;
            }

            if (deleteCount > (int)text.length()) {
                deleteCount = text.length();
                cout << "Only " << deleteCount << " characters available to delete." << endl;
            }

            push(undoStack, text);
            clearStack(redoStack);

            string deletedText = text.substr(text.length() - deleteCount);
            text = text.substr(0, text.length() - deleteCount);
            cout << "Deleted " << deleteCount << " character(s): \"" << deletedText << "\"" << endl;
        }
        else if (choice == 3) {
            if (text.empty()) {
                cout << "Document is already empty." << endl;
                continue;
            }

            push(undoStack, text);
            clearStack(redoStack);
            text = "";
            cout << "All text cleared successfully." << endl;
        }
        else if (choice == 4) {
            if (text.empty()) {
                cout << "Document is empty. Nothing to search." << endl;
                continue;
            }

            cout << "Enter word or text to search: ";
            getline(cin, searchWord);

            if (searchWord.empty()) {
                cout << "Cannot search for empty text." << endl;
                continue;
            }

            int foundCount = 0;
            int foundPositions[100];

            for (int i = 0; i <= (int)(text.length() - searchWord.length()); i++) {
                bool match = true;
                for (int j = 0; j < (int)searchWord.length(); j++) {
                    if (text[i + j] != searchWord[j]) {
                        match = false;
                        break;
                    }
                }
                if (match) {
                    foundPositions[foundCount] = i;
                    foundCount++;
                }
            }

            if (foundCount == 0) {
                cout << "Search word not found: \"" << searchWord << "\"" << endl;
            } else {
                cout << "Found " << foundCount << " occurrence(s) of \"" << searchWord << "\" at positions: ";
                for (int i = 0; i < foundCount; i++) {
                    cout << foundPositions[i];
                    if (i < foundCount - 1) cout << ", ";
                }
                cout << endl;
            }
        }
        else if (choice == 5) {
            if (isEmpty(undoStack) || undoStack.top == 0) {
                cout << "Nothing to undo. Already at initial state." << endl;
            } else {
                push(redoStack, text);
                text = pop(undoStack);
                cout << "Undo successful. Previous version restored." << endl;
            }
        }
        else if (choice == 6) {
            if (isEmpty(redoStack)) {
                cout << "Nothing to redo. No undone actions available." << endl;
            } else {
                push(undoStack, text);
                text = pop(redoStack);
                cout << "Redo successful. Version restored." << endl;
            }
        }
        else if (choice == 7) {
            cout << "Enter filename to save (without extension): ";
            getline(cin, filename);

            string fullFilename = filename + ".txt";
            ofstream outFile(fullFilename.c_str());

            if (outFile) {
                outFile << text;
                outFile.close();
                lastSavedFile = fullFilename;  // Store the filename for overwrite option
                cout << "Document saved successfully to " << fullFilename << endl;
            } else {
                cout << "Error: Could not create file." << endl;
            }
        }
        else if (choice == 8) {
            if (lastSavedFile.empty()) {
                cout << "No previous file to overwrite. Please use option 7 first to save a file." << endl;
            } else {
                ofstream outFile(lastSavedFile.c_str());
                if (outFile) {
                    outFile << text;
                    outFile.close();
                    cout << "Document overwritten successfully to " << lastSavedFile << endl;
                } else {
                    cout << "Error: Could not overwrite file." << endl;
                }
            }
        }
        else if (choice == 9) {
            cout << "Enter filename to load (without extension): ";
            getline(cin, filename);

            string fullFilename = filename + ".txt";
            ifstream inFile(fullFilename.c_str());

            if (inFile) {
                push(undoStack, text);
                clearStack(redoStack);

                string loadedText = "";
                string line;
                while (getline(inFile, line)) {
                    if (!loadedText.empty()) loadedText += "\n";
                    loadedText += line;
                }
                text = loadedText;
                inFile.close();
                cout << "Document loaded successfully from " << fullFilename << " (" << text.length() << " characters)" << endl;
            } else {
                cout << "Error: Could not open file. File may not exist." << endl;
            }
        }
        else if (choice == 10) {
            cout << "Current cursor is at end of document (position " << text.length() << ")." << endl;
            cout << "Enter new cursor position (0 to " << text.length() << "): ";
            cin >> position;
            cin.ignore();

            if (position < 0 || position > (int)text.length()) {
                cout << "Invalid position. Cursor remains at end." << endl;
            } else {
                cout << "Cursor moved from " << text.length() << " to position " << position << endl;
            }
        }
        else if (choice == 11) {
            cout << "\n==================================================" << endl;
            cout << "TEXT CONTENT" << endl;
            cout << "==================================================" << endl;
            if (text.empty()) {
                cout << "[Document is empty]" << endl;
            } else {
                cout << text << endl;
            }
            cout << "==================================================" << endl;
            cout << "STATUS:" << endl;
            cout << "Total characters: " << text.length() << endl;
            cout << "Total words: ";

            if (text.empty()) {
                cout << "0" << endl;
            } else {
                int wordCount = 1;
                for (int i = 0; i < (int)text.length(); i++) {
                    if (text[i] == ' ') {
                        wordCount++;
                    }
                }
                cout << wordCount << endl;
            }
            cout << "Undo stack size: " << (undoStack.top + 1) << endl;
            cout << "Redo stack size: " << (redoStack.top + 1) << endl;
            cout << "==================================================" << endl;
        }
        else if (choice == 12) {
            cout << "Exiting editor. Goodbye!" << endl;
        }

    } while (choice != 12);

    return 0;
}