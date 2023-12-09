# Business-diary

Certainly! Here's a textual description of the code in English:

The provided C++ code is a simple diary program that allows users to add, view, delete, and search diary entries. The diary entries can be protected by a password, and the program uses basic file input/output to store and retrieve entries.

1. *Data Structure:*
   - The `DiaryEntry` struct represents an individual diary entry and includes fields for subject, date, content, password hash, and a password hint.

2. **Constants:*
   - `diaryFileName`: The constant string represents the filename used to store diary entries.
   - `maxLoginAttempts`: The maximum number of login attempts allowed for password-protected entries.

3. **Password Hashing:*
   - The `getHash` function uses the standard C++ `std::hash` to generate a hash for a given password.

4. **Password Validation:*
   - The `validatePassword` function compares a user-input password with a stored password hash to validate the password.

5. **Loading Diary Entries:*
   - The `loadDiaryEntries` function reads diary entries from the specified file (`MyDiary.txt`) and populates a vector of `DiaryEntry` structs.

6. **Saving Diary Entries:*
   - The `saveDiaryEntries` function writes the vector of `DiaryEntry` structs back to the file, updating the diary.

7. **Adding Diary Entry:*
   - The `addDiaryEntry` function allows the user to input a new diary entry, including subject, content, and an optional password.

8. **Viewing Diary Entries:*
   - The `viewDiaryEntries` function displays a list of available diary entries and allows the user to view the content of a selected entry, prompting for a password if necessary.

9. **Deleting Diary Entry:*
   - The `deleteDiaryEntry` function enables the user to delete a selected diary entry, prompting for a password if the entry is password-protected.

10. **Searching Diary Entries:*
    - The `searchDiaryEntries` function lets the user search for diary entries based on a specified subject.

11. **Main Program Loop:*
    - The `main` function serves as the entry point for the program. It initializes the diary, presents a menu to the user, and processes user choices until the user decides to exit.

The program emphasizes simplicity and user interaction, providing basic functionality for maintaining a password-protected diary.



![image](https://github.com/MaksDrap/Business-diary/assets/132902478/061d18cd-fd91-4062-a074-562bd03c0c11)
