#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <vector>
#include <limits>
#include <functional>

using namespace std;

struct DiaryEntry
{
    string subject;
    string date;
    string content;
    size_t passwordHash;
    string passwordHint;
};

const string diaryFileName = "MyDiary.txt";
const int maxLoginAttempts = 3;

size_t getHash(const string& password)
{
    return std::hash<string>{}(password);
}

bool validatePassword(const string &inputPassword, const size_t &storedPasswordHash)
{
    return getHash(inputPassword) == storedPasswordHash;
}

void showPasswordHint(const string &hint)
{
    cout << "Password hint: " << hint << endl;
}

vector<DiaryEntry> loadDiaryEntries()
{
    ifstream file(diaryFileName);
    vector<DiaryEntry> diary;
    if (!file)
    {
        return diary;
    }

    DiaryEntry entry;
    while (getline(file, entry.subject))
    {
        getline(file, entry.date);
        getline(file, entry.content);
        file >> entry.passwordHash;
        file.ignore();
        getline(file, entry.passwordHint);
        diary.push_back(entry);
    }

    file.close();
    return diary;
}

void saveDiaryEntries(const vector<DiaryEntry> &diary)
{
    ofstream file(diaryFileName);
    if (!file)
    {
        cerr << "Failed to open the diary file." << endl;
        return;
    }

    for (const DiaryEntry &entry : diary)
    {
        file << entry.subject << '\n'
             << entry.date << '\n'
             << entry.content << '\n'
             << entry.passwordHash << '\n'
             << entry.passwordHint << '\n';
    }

    file.close();
}

void addDiaryEntry(vector<DiaryEntry> &diary)
{
    DiaryEntry entry;

    cin.ignore();

    cout << "Enter the subject of the diary: ";
    getline(cin, entry.subject);

    time_t now = time(0);
    tm *currentDate = localtime(&now);
    char dateBuffer[80];
    strftime(dateBuffer, 80, "%Y-%m-%d %H:%M:%S", currentDate);
    entry.date = dateBuffer;

    cout << "Enter the content of the diary (press Enter on an empty line to finish):\n";
    string line;
    while (true)
    {
        getline(cin, line);
        if (line.empty())
        {
            break;
        }
        entry.content += line + '\n';
    }

    cout << "Do you want to protect this diary with a password? (y/n): ";
    char protectChoice;
    cin >> protectChoice;

    if (protectChoice == 'y' || protectChoice == 'Y')
    {
        string password, confirmPassword, hint;
        cout << "Enter a password: ";
        cin >> password;
        cout << "Confirm the password: ";
        cin >> confirmPassword;
        if (password != confirmPassword)
        {
            cout << "Password confirmation failed. Diary not protected." << endl;
        }
        else
        {
            entry.passwordHash = getHash(password);
            cin.ignore();
            cout << "Enter a password hint: ";
            getline(cin, hint);
            entry.passwordHint = hint;
        }
    }
    else
    {
        // ???? ????? ?? ????????? ???????, ???????????? ?????????? ????????
        entry.passwordHash = 0;
        entry.passwordHint = "";
    }

    diary.push_back(entry);
    saveDiaryEntries(diary);
}


void viewDiaryEntries(const vector<DiaryEntry> &diary)
{
    vector<DiaryEntry> loadedDiary = loadDiaryEntries();
    if (loadedDiary.empty())
    {
        cout << "No diary entries found." << endl;
        return;
    }

    cout << "Diary Entries:" << endl;
    for (size_t i = 0; i < loadedDiary.size(); ++i)
    {
        cout << i + 1 << ". " << loadedDiary[i].subject << " (" << loadedDiary[i].date << ")" << endl;
    }
    cout << "--------------------------" << endl;
    cout << "Enter the number of the diary entry to view: ";
    int entryNumber;
    cin >> entryNumber;

    if (entryNumber > 0 && entryNumber <= loadedDiary.size())
    {
        const DiaryEntry &entry = loadedDiary[entryNumber - 1];

        for (int attempt = 1; attempt <= maxLoginAttempts; ++attempt)
        {
            if (entry.passwordHash == 0)
            {
                cout << entry.content << endl;
                break;
            }
            else
            {
                cout << "This diary is password protected. Enter the password (attempt " << attempt << "): ";
                string inputPassword;
                cin >> inputPassword;

                if (validatePassword(inputPassword, entry.passwordHash))
                {
                    cout << entry.content << endl;
                    break;
                }
                else
                {
                    if (attempt < maxLoginAttempts)
                    {
                        showPasswordHint(entry.passwordHint);
                        cout << "Incorrect password. Try again." << endl;
                    }
                    else
                    {
                        cout << "Maximum login attempts reached. Returning to the main menu." << endl;
                        return;
                    }
                }
            }
        }
    }
    else
    {
        cout << "Invalid entry number." << endl;
    }
}

void deleteDiaryEntry(vector<DiaryEntry> &diary)
{
    if (diary.empty())
    {
        cout << "No diary entries to delete." << endl;
        return;
    }

    cout << "Diary Entries:" << endl;
    for (size_t i = 0; i < diary.size(); ++i)
    {
        cout << i + 1 << ". " << diary[i].subject << " (" << diary[i].date << ")" << endl;
    }

    cout << "Enter the number of the diary entry to delete: ";
    int entryNumber;
    cin >> entryNumber;

    if (entryNumber > 0 && entryNumber <= diary.size())
    {
        DiaryEntry &entry = diary[entryNumber - 1];

        if (entry.passwordHash != 0)
        {
            string inputPassword;
            cout << "This diary is password protected. Enter the password to delete: ";
            cin >> inputPassword;

            if (!validatePassword(inputPassword, entry.passwordHash))
            {
                showPasswordHint(entry.passwordHint);
                cout << "Password is incorrect. Deletion aborted." << endl;
                return;
            }
        }

        diary.erase(diary.begin() + entryNumber - 1);
        saveDiaryEntries(diary);
        cout << "Diary entry deleted successfully." << endl;
    }
    else
    {
        cout << "Invalid entry number." << endl;
    }
}

void searchDiaryEntries(const vector<DiaryEntry> &diary)
{
    cin.ignore();

    cout << "Enter the subject to search for: ";
    string searchTerm;
    getline(cin, searchTerm);

    bool found = false;

    for (const DiaryEntry &entry : diary)
    {
        if (entry.subject.find(searchTerm) != string::npos)
        {
            cout << "Subject: " << entry.subject << " (" << entry.date << ")" << endl;

            if (entry.passwordHash == 0)
            {
                cout << "Content: " << entry.content << endl;
            }
            else
            {
                cout << "This diary is password protected. To view its content, use the 'View diary entries' feature and enter the password." << endl;
            }

            cout << "-------------------" << endl;
            found = true;
        }
    }

    if (!found)
    {
        cout << "No diary entries found with the specified subject." << endl;
    }
}

int main()
{
    vector<DiaryEntry> diary = loadDiaryEntries();

    while (true)
    {
        cout << "--------------------------" << endl;
        cout << "Diary Program Menu:" << endl;
        cout << "1. Add new diary entry" << endl;
        cout << "2. View diary entries" << endl;
        cout << "3. Delete diary entry" << endl;
        cout << "4. Search diary entries" << endl;
        cout << "5. Quit" << endl;

        int choice;
        cout << "-----------------------------------" << endl;
        cout << "Enter your choice -> ";
        cin >> choice;
        cout << "-----------------------------------" << endl;

        switch (choice)
        {
        case 1:
            addDiaryEntry(diary);
            break;
        case 2:
            viewDiaryEntries(diary);
            break;
        case 3:
            deleteDiaryEntry(diary);
            break;
        case 4:
            searchDiaryEntries(diary);
            break;
        case 5:
            cout << "---- Exiting the diary program ----" << endl;
            saveDiaryEntries(diary);
            return 0;
        default:
            cout << "Invalid choice. Please enter a valid option (1-5)." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
    }
    return 0;
}

