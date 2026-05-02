#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <limits>
#include <algorithm>
#include <cctype>

using namespace std;

// Stores one edit action for undo/redo
struct Change {
    enum Kind { ADD, REMOVE };
    Kind kind;
    string data;
    int pos;
};

class SimpleEditor {
private:
    string content;
    int cursor;
    vector<Change> undoList;
    vector<Change> redoList;

    int addCount;
    int removeCount;
    int undoCount;
    int redoCount;

    static bool isSpace(char c) {
        return isspace(static_cast<unsigned char>(c));
    }

    void keepCursorValid() {
        if (cursor < 0) cursor = 0;
        if (cursor > (int)content.length()) cursor = (int)content.length();
    }

    void addAt(int pos, const string& text) {
        if (pos < 0) pos = 0;
        if (pos > (int)content.length()) pos = (int)content.length();

        content.insert(pos, text);
        cursor = pos + (int)text.length();
        keepCursorValid();
    }

    string removeAt(int pos, int len) {
        if (pos < 0) pos = 0;
        if (pos >= (int)content.length()) return "";
        if (len <= 0) return "";

        if (pos + len > (int)content.length()) {
            len = (int)content.length() - pos;
        }

        string removed = content.substr(pos, len);
        content.erase(pos, len);

        if (cursor > pos) {
            cursor = max(pos, cursor - len);
        }

        keepCursorValid();
        return removed;
    }

    void addHistory(const Change& ch) {
        undoList.push_back(ch);
        redoList.clear();
    }

    bool findWord(int pos, int& start, int& end) const {
        if (content.empty()) return false;

        if (pos < 0) pos = 0;
        if (pos >= (int)content.length()) pos = (int)content.length() - 1;

        while (pos < (int)content.length() && isSpace(content[pos])) {
            ++pos;
        }

        if (pos >= (int)content.length()) return false;

        start = pos;
        while (start > 0 && !isSpace(content[start - 1])) {
            --start;
        }

        end = pos;
        while (end < (int)content.length() && !isSpace(content[end])) {
            ++end;
        }

        return true;
    }

public:
    SimpleEditor()
        : cursor(0), addCount(0), removeCount(0), undoCount(0), redoCount(0) {}

    void show() const {
        cout << "\n========== DOCUMENT ==========\n";
        if (content.empty()) {
            cout << "[Empty document]\n";
        } else {
            for (size_t i = 0; i < content.length(); ++i) {
                if (i == (size_t)cursor) cout << '|';
                cout << content[i];
            }
            if (cursor == (int)content.length()) cout << '|';
            cout << '\n';
        }

        cout << "==============================\n";
        cout << "Chars: " << content.length()
             << " | Words: " << countWords()
             << " | Cursor: " << cursor << "\n";
        cout << "Stats - Adds: " << addCount
             << " | Removes: " << removeCount
             << " | Undos: " << undoCount
             << " | Redos: " << redoCount << "\n\n";
    }

    int countWords() const {
        if (content.empty()) return 0;

        istringstream iss(content);
        string word;
        int count = 0;

        while (iss >> word) {
            ++count;
        }

        return count;
    }

    void insertText(const string& text) {
        if (text.empty()) {
            cout << "Cannot insert empty text.\n";
            return;
        }

        Change ch{Change::ADD, text, cursor};
        addHistory(ch);

        addAt(cursor, text);
        ++addCount;

        cout << "Inserted \"" << text << "\" at position " << ch.pos << "\n";
    }

    void deleteChars(int n) {
        if (n <= 0) {
            cout << "Invalid delete count.\n";
            return;
        }

        if (cursor >= (int)content.length()) {
            cout << "Nothing to delete at cursor.\n";
            return;
        }

        int oldPos = cursor;
        string removed = removeAt(oldPos, n);

        if (removed.empty()) {
            cout << "Nothing deleted.\n";
            return;
        }

        Change ch{Change::REMOVE, removed, oldPos};
        addHistory(ch);

        ++removeCount;
        cout << "Deleted " << removed.length() << " character(s): \"" << removed << "\"\n";
    }

    void deleteRange(int pos, int len) {
        if (content.empty()) {
            cout << "Document is empty.\n";
            return;
        }

        if (pos < 0 || pos >= (int)content.length()) {
            cout << "Invalid position.\n";
            return;
        }

        if (len <= 0) {
            cout << "Length must be positive.\n";
            return;
        }

        int oldPos = pos;
        string removed = removeAt(oldPos, len);

        if (removed.empty()) {
            cout << "Nothing deleted.\n";
            return;
        }

        Change ch{Change::REMOVE, removed, oldPos};
        addHistory(ch);

        ++removeCount;
        cout << "Deleted range [" << oldPos << ", "
             << (oldPos + (int)removed.length() - 1) << "]: \"" << removed << "\"\n";
    }

    void deleteWord() {
        if (content.empty()) {
            cout << "No word to delete.\n";
            return;
        }

        int start, end;
        if (!findWord(cursor, start, end)) {
            cout << "No word found at or after cursor.\n";
            return;
        }

        string removed = removeAt(start, end - start);
        if (removed.empty()) {
            cout << "Nothing deleted.\n";
            return;
        }

        Change ch{Change::REMOVE, removed, start};
        addHistory(ch);

        ++removeCount;
        cout << "Deleted word: \"" << removed << "\"\n";
    }

    void searchText(const string& word) const {
        if (word.empty()) {
            cout << "Search text cannot be empty.\n";
            return;
        }

        vector<int> found;
        size_t pos = 0;

        while ((pos = content.find(word, pos)) != string::npos) {
            found.push_back((int)pos);
            pos += word.length();
        }

        if (found.empty()) {
            cout << "Text \"" << word << "\" not found.\n";
        } else {
            cout << "Found " << found.size() << " occurrence(s) at: ";
            for (size_t i = 0; i < found.size(); ++i) {
                cout << found[i];
                if (i + 1 < found.size()) cout << ", ";
            }
            cout << "\n";
        }
    }

    void undo() {
        if (undoList.empty()) {
            cout << "Nothing to undo.\n";
            return;
        }

        Change last = undoList.back();
        undoList.pop_back();

        if (last.kind == Change::ADD) {
            removeAt(last.pos, (int)last.data.length());
            redoList.push_back(last);
            cout << "Undo: removed \"" << last.data << "\"\n";
        } else {
            addAt(last.pos, last.data);
            redoList.push_back(last);
            cout << "Undo: restored \"" << last.data << "\"\n";
        }

        ++undoCount;
    }

    void redo() {
        if (redoList.empty()) {
            cout << "Nothing to redo.\n";
            return;
        }

        Change last = redoList.back();
        redoList.pop_back();

        if (last.kind == Change::ADD) {
            addAt(last.pos, last.data);
            undoList.push_back(last);
            cout << "Redo: inserted \"" << last.data << "\"\n";
        } else {
            removeAt(last.pos, (int)last.data.length());
            undoList.push_back(last);
            cout << "Redo: deleted \"" << last.data << "\"\n";
        }

        ++redoCount;
    }

    void saveFile(const string& fileName) const {
        if (fileName.empty()) {
            cout << "Filename cannot be empty.\n";
            return;
        }

        ofstream file(fileName);
        if (!file) {
            cout << "Error: cannot open file for writing.\n";
            return;
        }

        file << content;
        cout << "Document saved to " << fileName << "\n";
    }

    void loadFile(const string& fileName) {
        if (fileName.empty()) {
            cout << "Filename cannot be empty.\n";
            return;
        }

        ifstream file(fileName);
        if (!file) {
            cout << "Error: cannot open file for reading.\n";
            return;
        }

        string loaded((istreambuf_iterator<char>(file)),
                      istreambuf_iterator<char>());

        content = loaded;
        cursor = (int)content.length();
        undoList.clear();
        redoList.clear();
        addCount = 0;
        removeCount = 0;
        undoCount = 0;
        redoCount = 0;

        cout << "Loaded from " << fileName << " (" << content.length() << " chars)\n";
        keepCursorValid();
    }

    void appendFile(const string& fileName) const {
        if (fileName.empty()) {
            cout << "Filename cannot be empty.\n";
            return;
        }

        ofstream file(fileName, ios::app);
        if (!file) {
            cout << "Error: cannot open file for appending.\n";
            return;
        }

        file << content;
        cout << "Document appended to " << fileName << "\n";
    }

    void setCursor(int newPos) {
        cursor = newPos;
        keepCursorValid();
        cout << "Cursor moved to position " << cursor << "\n";
    }

    void showStats() const {
        cout << "\n--- Editor Statistics ---\n";
        cout << "Total insert operations: " << addCount << "\n";
        cout << "Total delete operations: " << removeCount << "\n";
        cout << "Total undo operations:   " << undoCount << "\n";
        cout << "Total redo operations:   " << redoCount << "\n";
        cout << "Undo stack size:         " << undoList.size() << "\n";
        cout << "Redo stack size:         " << redoList.size() << "\n";
    }

    string getContent() const {
        return content;
    }
};

void clearInput() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int main() {
    SimpleEditor editor;
    int choice;
    string input;
    int pos, len, count;

    cout << "==== TEXT EDITOR SIMULATION ====\n";
    cout << "Supports undo/redo, cursor movement, word/range delete, statistics.\n";

    do {
        editor.show();

        cout << "MENU:\n"
             << "1. Insert text at cursor\n"
             << "2. Delete N chars at cursor\n"
             << "3. Delete range (position, length)\n"
             << "4. Delete word at cursor\n"
             << "5. Search\n"
             << "6. Undo\n"
             << "7. Redo\n"
             << "8. Save to file\n"
             << "9. Load from file\n"
             << "10. Append to file\n"
             << "11. Move cursor\n"
             << "12. Show statistics\n"
             << "13. Exit\n"
             << "Choice: ";

        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            clearInput();
            cout << "Invalid input. Enter a number.\n";
            continue;
        }

        if (choice < 1 || choice > 13) {
            cout << "Invalid choice (1-13).\n";
            clearInput();
            continue;
        }

        clearInput();

        switch (choice) {
            case 1:
                cout << "Text to insert: ";
                getline(cin, input);
                editor.insertText(input);
                break;

            case 2:
                cout << "Number of chars to delete: ";
                cin >> count;
                if (cin.fail()) {
                    cin.clear();
                    clearInput();
                    cout << "Invalid number.\n";
                } else {
                    editor.deleteChars(count);
                    clearInput();
                }
                break;

            case 3:
                cout << "Start position: ";
                cin >> pos;
                cout << "Length: ";
                cin >> len;
                if (cin.fail()) {
                    cin.clear();
                    clearInput();
                    cout << "Invalid numbers.\n";
                } else {
                    editor.deleteRange(pos, len);
                    clearInput();
                }
                break;

            case 4:
                editor.deleteWord();
                break;

            case 5:
                cout << "Search for: ";
                getline(cin, input);
                editor.searchText(input);
                break;

            case 6:
                editor.undo();
                break;

            case 7:
                editor.redo();
                break;

            case 8:
                cout << "Filename: ";
                getline(cin, input);
                editor.saveFile(input);
                break;

            case 9:
                cout << "Filename: ";
                getline(cin, input);
                editor.loadFile(input);
                break;

            case 10:
                cout << "Filename: ";
                getline(cin, input);
                editor.appendFile(input);
                break;

            case 11:
                cout << "New cursor position (0.." << editor.getContent().length() << "): ";
                cin >> pos;
                if (cin.fail()) {
                    cin.clear();
                    clearInput();
                    cout << "Invalid position.\n";
                } else {
                    editor.setCursor(pos);
                    clearInput();
                }
                break;

            case 12:
                editor.showStats();
                break;

            case 13:
                cout << "Exiting editor. Goodbye!\n";
                break;
        }
    } while (choice != 13);

    return 0;
}
