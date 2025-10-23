#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>  
#define MAX_STR_LEN 100000
#define FILE_NAME "hall_records.dat"
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"
typedef struct {
    char name[MAX_STR_LEN];
    char phone[MAX_STR_LEN];  
    char email[MAX_STR_LEN];
} Contact;
typedef struct {
    float total_payable;
    float total_paid;
    float total_due;
    float total_others;
} Payment;
typedef struct StudentNode {
    char id[MAX_STR_LEN];  
    Contact student;
    Contact father;
    Contact mother;
    Contact guardian;
    Payment payment;
    struct StudentNode* next;
} StudentNode;
StudentNode* head = NULL;
int studentCount = 0;
clock_t start_time, end_time;
double cpu_time_used;
void startTimeMeasurement();
void stopTimeMeasurement(const char* operation);
void displayHallInfo();
void addNewStudent();
void displayPaymentStatus();
void searchStudent();
void updateStudentInfo();
void deleteStudentInfo();
void saveToFile();
void loadFromFile();
void displayAllStudents();
void clearInputBuffer();
StudentNode* findStudentByID(char* id);
StudentNode* findStudentByNameOrContact(char* searchTerm);
void displayStudentDetails(StudentNode* student);
void displayMenu();
void freeAllStudents();
void startTimeMeasurement() {
    start_time = clock();
}
void stopTimeMeasurement(const char* operation) {
    end_time = clock();
    cpu_time_used = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;
    printf("%sOperation '%s' completed in %f seconds%s\n", COLOR_CYAN, operation, cpu_time_used, COLOR_RESET);
}
int main() {
    int choice;
    #ifdef _WIN32
    system("color");
    #endif
    startTimeMeasurement();
    loadFromFile();
    stopTimeMeasurement("Loading records");
    do {
        displayMenu();
        printf("%sEnter your choice (1-10): %s", COLOR_CYAN, COLOR_RESET);
        if (scanf("%d", &choice) != 1) {
            printf("%sInvalid input. Please enter a number.%s\n", COLOR_RED, COLOR_RESET);
            clearInputBuffer();
            continue;
        }
        clearInputBuffer();
        startTimeMeasurement(); 
        switch (choice) {
            case 1:
                displayHallInfo();
                stopTimeMeasurement("Display hall info");
                break;
            case 2:
                addNewStudent();
                stopTimeMeasurement("Add new student");
                break;
            case 3:
                displayPaymentStatus();
                stopTimeMeasurement("Display payment status");
                break;
            case 4:
                searchStudent();
                stopTimeMeasurement("Search student");
                break;
            case 5:
                updateStudentInfo();
                stopTimeMeasurement("Update student info");
                break;
            case 6:
                deleteStudentInfo();
                stopTimeMeasurement("Delete student info");
                break;
            case 7:
                saveToFile();
                stopTimeMeasurement("Save to file");
                break;
            case 8:
                loadFromFile();
                stopTimeMeasurement("Load from file");
                break;
            case 9:
                displayAllStudents();
                stopTimeMeasurement("Display all students");
                break;
            case 10:
                printf("%sSaving records before exit...%s\n", COLOR_YELLOW, COLOR_RESET);
                startTimeMeasurement();
                saveToFile();
                stopTimeMeasurement("Save before exit");
                printf("%sThank you for using Hall Management System!%s\n", COLOR_GREEN, COLOR_RESET);
                startTimeMeasurement();
                freeAllStudents();
                stopTimeMeasurement("Free memory");
                break;
            default:
                printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
                stopTimeMeasurement("Process invalid choice");
        }
        if (choice != 10) {
            printf("\n%sPress Enter to continue...%s", COLOR_CYAN, COLOR_RESET);
            getchar();
        } 
    } while (choice != 10);
    return 0;
}
void freeAllStudents() {
    StudentNode* current = head;
    StudentNode* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
    studentCount = 0;
}
void displayMenu() {
    system("cls || clear"); 
    printf("\n%s===== HALL MANAGEMENT SYSTEM =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s1. Hall Information and Rules%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s2. Add New Student%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s3. Payment Status%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s4. Search Student%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s5. Update Student Information%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%s6. Delete Student Information%s\n", COLOR_RED, COLOR_RESET);
    printf("%s7. Save Records to File%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s8. Load Records from File%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%s9. Display All Students%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%s10. Exit%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s===================================%s\n", COLOR_MAGENTA, COLOR_RESET);
}
void displayHallInfo() {
    printf("\n%s===== HALL INFORMATION AND RULES =====%s\n", COLOR_YELLOW, COLOR_RESET);
    printf("%s1. The hall is open 24/7 for residents.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s2. Visitors are allowed from 10:00 AM to 8:00 PM only.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s3. Maintain silence in study areas.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s4. Monthly fees must be paid by the 10th of each month.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s5. Any damage to hall property will be charged accordingly.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s6. Cooking is not allowed in rooms.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s7. Keep your rooms and common areas clean.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s8. Report any maintenance issues to the hall office.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s9. Follow all university guidelines.%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%s10. For any assistance, contact the hall provost.%s\n", COLOR_CYAN, COLOR_RESET);
}
void getStringInput(char* dest, const char* prompt) {
    printf("%s%s%s", COLOR_CYAN, prompt, COLOR_RESET);
    fgets(dest, MAX_STR_LEN, stdin);
    dest[strcspn(dest, "\n")] = 0;
}
void getContactInput(Contact* contact, const char* relationshipType) {
    char prompt[MAX_STR_LEN];
    sprintf(prompt, "%s Name: ", relationshipType);
    getStringInput(contact->name, prompt);
    sprintf(prompt, "%s Phone: ", relationshipType);  
    getStringInput(contact->phone, prompt);
    sprintf(prompt, "%s Email: ", relationshipType);
    getStringInput(contact->email, prompt);
}
void addNewStudent() {
    StudentNode* newStudent = (StudentNode*)malloc(sizeof(StudentNode));
    if (newStudent == NULL) {
        printf("%sMemory allocation failed!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    memset(newStudent, 0, sizeof(StudentNode));
    printf("\n%s===== ADD NEW STUDENT =====%s\n", COLOR_GREEN, COLOR_RESET);
    getStringInput(newStudent->id, "Student ID: ");
    if (findStudentByID(newStudent->id) != NULL) {
        printf("%sA student with this ID already exists!%s\n", COLOR_RED, COLOR_RESET);
        free(newStudent);
        return;
    }
    getContactInput(&newStudent->student, "Student");
    getContactInput(&newStudent->father, "Father's");
    getContactInput(&newStudent->mother, "Mother's");
    getContactInput(&newStudent->guardian, "Local Guardian's");
    printf("%sTotal Payable Amount: %s", COLOR_CYAN, COLOR_RESET);
    scanf("%f", &newStudent->payment.total_payable);
    clearInputBuffer();
    printf("%sTotal Paid Amount: %s", COLOR_CYAN, COLOR_RESET);
    scanf("%f", &newStudent->payment.total_paid);
    clearInputBuffer();
    newStudent->payment.total_due = newStudent->payment.total_payable - newStudent->payment.total_paid;
    printf("%sTotal Other Charges: %s", COLOR_CYAN, COLOR_RESET);
    scanf("%f", &newStudent->payment.total_others);
    clearInputBuffer();
    newStudent->next = head;
    head = newStudent;
    studentCount++;
    printf("\n%sStudent added successfully!%s\n", COLOR_GREEN, COLOR_RESET);
}
void displayPaymentStatus() {
    char searchTerm[MAX_STR_LEN];
    printf("\n%s===== PAYMENT STATUS =====%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%sEnter student name, ID, or contact number: %s", COLOR_CYAN, COLOR_RESET);
    fgets(searchTerm, MAX_STR_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0;  
    clock_t search_start = clock(); 
    StudentNode* student = findStudentByNameOrContact(searchTerm);
    clock_t search_end = clock(); 
    double search_time = ((double) (search_end - search_start)) / CLOCKS_PER_SEC;
    if (student != NULL) {
        printf("\n%sPayment Status for %s:%s\n", COLOR_YELLOW, student->student.name, COLOR_RESET);
        printf("%s------------------------%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%sTotal Payable: TK%.2f%s\n", COLOR_GREEN, student->payment.total_payable, COLOR_RESET);
        printf("%sTotal Paid   : TK%.2f%s\n", COLOR_GREEN, student->payment.total_paid, COLOR_RESET);
        printf("%sTotal Due    : TK%.2f%s\n", COLOR_RED, student->payment.total_due, COLOR_RESET);
        printf("%sOther Charges: TK%.2f%s\n", COLOR_MAGENTA, student->payment.total_others, COLOR_RESET);
        printf("%s------------------------%s\n", COLOR_YELLOW, COLOR_RESET);
        printf("%sFinal Balance: TK%.2f%s\n", COLOR_BLUE, student->payment.total_due + student->payment.total_others, COLOR_RESET);
        printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET);
    } else {
        printf("%sStudent not found!%s\n", COLOR_RED, COLOR_RESET);
        printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET);
    }
}
void searchStudent() {
    char searchTerm[MAX_STR_LEN];
    printf("\n%s===== SEARCH STUDENT =====%s\n", COLOR_CYAN, COLOR_RESET);
    printf("%sEnter student name, ID, or contact number: %s", COLOR_CYAN, COLOR_RESET);
    fgets(searchTerm, MAX_STR_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; 
    clock_t search_start = clock(); 
    StudentNode* student = findStudentByNameOrContact(searchTerm);
    clock_t search_end = clock(); 
    double search_time = ((double) (search_end - search_start)) / CLOCKS_PER_SEC;
    if (student != NULL) {
        displayStudentDetails(student);
        printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET);
    } else {
        printf("%sStudent not found!%s\n", COLOR_RED, COLOR_RESET);
        printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET);
    }
}
void updateStudentInfo() {
    char searchTerm[MAX_STR_LEN];
    int choice;
    printf("\n%s===== UPDATE STUDENT INFORMATION =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%sEnter student name, ID, or contact number: %s", COLOR_CYAN, COLOR_RESET);
    fgets(searchTerm, MAX_STR_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; 
    clock_t search_start = clock();
    StudentNode* student = findStudentByNameOrContact(searchTerm);
    clock_t search_end = clock(); 
    double search_time = ((double) (search_end - search_start)) / CLOCKS_PER_SEC;
    printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET);
    if (student != NULL) {
        do {
            printf("\n%sStudent Found: %s%s\n", COLOR_GREEN, student->student.name, COLOR_RESET);
            printf("%sWhat information would you like to update?%s\n", COLOR_CYAN, COLOR_RESET);
            printf("%s1. Contact Information%s\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s2. Parent/Guardian Information%s\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s3. Payment Information%s\n", COLOR_YELLOW, COLOR_RESET);
            printf("%s4. Back to Main Menu%s\n", COLOR_YELLOW, COLOR_RESET);
            printf("%sEnter your choice (1-4): %s", COLOR_CYAN, COLOR_RESET);
            if (scanf("%d", &choice) != 1) {
                printf("%sInvalid input. Please enter a number.%s\n", COLOR_RED, COLOR_RESET);
                clearInputBuffer();
                continue;
            }
            clearInputBuffer();
            
            clock_t operation_start = clock(); 
            switch (choice) {
                case 1:
                    getStringInput(student->student.phone, "Phone: ");
                    getStringInput(student->student.email, "Email: ");
                    printf("%sContact information updated successfully!%s\n", COLOR_GREEN, COLOR_RESET);
                    break;   
                case 2:
                    printf("\n%sUpdate Father's Information:%s\n", COLOR_BLUE, COLOR_RESET);
                    getStringInput(student->father.phone, "Father's Phone: ");
                    getStringInput(student->father.email, "Father's Email: ");
                    
                    printf("\n%sUpdate Mother's Information:%s\n", COLOR_MAGENTA, COLOR_RESET);
                    getStringInput(student->mother.phone, "Mother's Phone: ");
                    getStringInput(student->mother.email, "Mother's Email: ");
                    printf("\n%sUpdate Guardian's Information:%s\n", COLOR_CYAN, COLOR_RESET);
                    getStringInput(student->guardian.phone, "Guardian's Phone: ");
                    getStringInput(student->guardian.email, "Guardian's Email: ");
                    printf("%sParent/Guardian information updated successfully!%s\n", COLOR_GREEN, COLOR_RESET);
                    break;
                case 3:
                    printf("%sTotal Payable Amount: %s", COLOR_CYAN, COLOR_RESET);
                    scanf("%f", &student->payment.total_payable);
                    clearInputBuffer();
                    printf("%sTotal Paid Amount: %s", COLOR_CYAN, COLOR_RESET);
                    scanf("%f", &student->payment.total_paid);
                    clearInputBuffer();
                    student->payment.total_due = student->payment.total_payable - student->payment.total_paid;
                    printf("%sTotal Other Charges: %s", COLOR_CYAN, COLOR_RESET);
                    scanf("%f", &student->payment.total_others);
                    clearInputBuffer();
                    printf("%sPayment information updated successfully!%s\n", COLOR_GREEN, COLOR_RESET);
                    break;
                case 4:
                    printf("%sReturning to main menu...%s\n", COLOR_YELLOW, COLOR_RESET);
                    break;
                default:
                    printf("%sInvalid choice. Please try again.%s\n", COLOR_RED, COLOR_RESET);
            }
            if (choice >= 1 && choice <= 3) {
                clock_t operation_end = clock(); // End timing for update operation
                double operation_time = ((double) (operation_end - operation_start)) / CLOCKS_PER_SEC;
                printf("%sUpdate operation completed in %f seconds%s\n", COLOR_CYAN, operation_time, COLOR_RESET);
            }
            
        } while (choice != 4);
    } else {
        printf("%sStudent not found!%s\n", COLOR_RED, COLOR_RESET);
    }
}
void deleteStudentInfo() {
    char searchTerm[MAX_STR_LEN];
    char confirm;
    printf("\n%s===== DELETE STUDENT INFORMATION =====%s\n", COLOR_RED, COLOR_RESET);
    printf("%sEnter student name, ID, or contact number: %s", COLOR_CYAN, COLOR_RESET);
    fgets(searchTerm, MAX_STR_LEN, stdin);
    searchTerm[strcspn(searchTerm, "\n")] = 0; 
    clock_t search_start = clock(); 
    StudentNode* student = findStudentByNameOrContact(searchTerm);
    clock_t search_end = clock(); 
    double search_time = ((double) (search_end - search_start)) / CLOCKS_PER_SEC;
    printf("%sSearch completed in %f seconds%s\n", COLOR_CYAN, search_time, COLOR_RESET); 
    if (student != NULL) {
        displayStudentDetails(student);
        printf("\n%sAre you sure you want to delete this student's record? (y/n): %s", COLOR_RED, COLOR_RESET);
        scanf("%c", &confirm);
        clearInputBuffer();
        if (tolower(confirm) == 'y') {
            clock_t delete_start = clock(); 
            if (student == head) {
                head = head->next;
                free(student);
            } else {
                StudentNode* prev = head;
                while (prev != NULL && prev->next != student) {
                    prev = prev->next;
                }
                if (prev != NULL) {
                    prev->next = student->next;
                    free(student);
                }
            }
            studentCount--;
            clock_t delete_end = clock(); 
            double delete_time = ((double) (delete_end - delete_start)) / CLOCKS_PER_SEC;
            printf("%sStudent record deleted successfully!%s\n", COLOR_GREEN, COLOR_RESET);
            printf("%sDelete operation completed in %f seconds%s\n", COLOR_CYAN, delete_time, COLOR_RESET);
        } else {
            printf("%sDeletion cancelled.%s\n", COLOR_YELLOW, COLOR_RESET);
        }
    } else {
        printf("%sStudent not found!%s\n", COLOR_RED, COLOR_RESET);
    }
}
void saveToFile() {
    clock_t io_start = clock(); 
    FILE *file = fopen(FILE_NAME, "wb"); 
    if (file == NULL) {
        printf("%sError opening file for writing!%s\n", COLOR_RED, COLOR_RESET);
        return;
    }
    fwrite(&studentCount, sizeof(int), 1, file);
    StudentNode* current = head;
    while (current != NULL) {
        fwrite(current->id, sizeof(current->id), 1, file);
        fwrite(&current->student, sizeof(Contact), 1, file);
        fwrite(&current->father, sizeof(Contact), 1, file);
        fwrite(&current->mother, sizeof(Contact), 1, file);
        fwrite(&current->guardian, sizeof(Contact), 1, file);
        fwrite(&current->payment, sizeof(Payment), 1, file);
        current = current->next;
    }
    fclose(file);
    clock_t io_end = clock(); 
    double io_time = ((double) (io_end - io_start)) / CLOCKS_PER_SEC;
    printf("%sRecords saved successfully to %s!%s\n", COLOR_GREEN, FILE_NAME, COLOR_RESET);
    printf("%sFile operation completed in %f seconds%s\n", COLOR_CYAN, io_time, COLOR_RESET);
}
void loadFromFile() {
    clock_t io_start = clock(); 
    FILE *file = fopen(FILE_NAME, "rb");
    if (file == NULL) {
        printf("%sNo existing records found or error opening file!%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    freeAllStudents();
    fread(&studentCount, sizeof(int), 1, file);
    for (int i = 0; i < studentCount; i++) {
        StudentNode* newStudent = (StudentNode*)malloc(sizeof(StudentNode));
        if (newStudent == NULL) {
            printf("%sMemory allocation failed during loading!%s\n", COLOR_RED, COLOR_RESET);
            fclose(file);
            return;
        }
        newStudent->next = NULL;
        fread(newStudent->id, sizeof(newStudent->id), 1, file);
        fread(&newStudent->student, sizeof(Contact), 1, file);
        fread(&newStudent->father, sizeof(Contact), 1, file);
        fread(&newStudent->mother, sizeof(Contact), 1, file);
        fread(&newStudent->guardian, sizeof(Contact), 1, file);
        fread(&newStudent->payment, sizeof(Payment), 1, file);
        newStudent->next = head;
        head = newStudent;
    }
    fclose(file);
    clock_t io_end = clock();
    double io_time = ((double) (io_end - io_start)) / CLOCKS_PER_SEC;
    printf("%sRecords loaded successfully from %s!%s\n", COLOR_GREEN, FILE_NAME, COLOR_RESET);
    printf("%sTotal number of students loaded: %d%s\n", COLOR_BLUE, studentCount, COLOR_RESET);
    printf("%sFile operation completed in %f seconds%s\n", COLOR_CYAN, io_time, COLOR_RESET);
}
void displayAllStudents() {
    if (head == NULL) {
        printf("%sNo students in the system!%s\n", COLOR_YELLOW, COLOR_RESET);
        return;
    }
    printf("\n%s===== ALL STUDENTS =====%s\n", COLOR_BLUE, COLOR_RESET);
    printf("%sTotal students: %d%s\n\n", COLOR_BLUE, studentCount, COLOR_RESET);
    clock_t display_start = clock(); 
    StudentNode* current = head;
    int count = 1;
    while (current != NULL) {
        const char* rowColor = (count % 2 == 0) ? COLOR_CYAN : COLOR_GREEN;
        printf("%s%d. %s (ID: %s)%s\n", rowColor, count++, current->student.name, current->id, COLOR_RESET);
        current = current->next;
    }
    clock_t display_end = clock(); 
    double display_time = ((double) (display_end - display_start)) / CLOCKS_PER_SEC;
    printf("%sDisplay operation completed in %f seconds%s\n", COLOR_CYAN, display_time, COLOR_RESET);
    int choice;
    printf("\n%sEnter student number to view details (0 to return): %s", COLOR_CYAN, COLOR_RESET);
    scanf("%d", &choice);
    clearInputBuffer();
    if (choice > 0 && choice <= studentCount) {
        current = head;
        for (int i = 1; i < choice && current != NULL; i++) {
            current = current->next;
        }
        if (current != NULL) {
            clock_t detail_start = clock(); 
            displayStudentDetails(current);
            clock_t detail_end = clock();
            double detail_time = ((double) (detail_end - detail_start)) / CLOCKS_PER_SEC;
            printf("%sDetail display completed in %f seconds%s\n", COLOR_CYAN, detail_time, COLOR_RESET);
        }
    } else if (choice != 0) {
        printf("%sInvalid choice!%s\n", COLOR_RED, COLOR_RESET);
    }
}
void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}
StudentNode* findStudentByID(char* id) {
    StudentNode* current = head;
    while (current != NULL) {
        if (strcmp(current->id, id) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; 
}
StudentNode* findStudentByNameOrContact(char* searchTerm) {
    StudentNode* current = head;

    while (current != NULL) {
        if (strcmp(current->student.name, searchTerm) == 0 || 
            strcmp(current->student.phone, searchTerm) == 0 || 
            strcmp(current->id, searchTerm) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL; 
}
void displayStudentDetails(StudentNode* student) {
    printf("\n%s===== STUDENT DETAILS =====%s\n", COLOR_MAGENTA, COLOR_RESET);
    printf("%sName: %s%s\n", COLOR_YELLOW, student->student.name, COLOR_RESET);
    printf("%sID: %s%s\n", COLOR_YELLOW, student->id, COLOR_RESET);
    printf("%sPhone: %s%s\n", COLOR_YELLOW, student->student.phone, COLOR_RESET);
    printf("%sEmail: %s%s\n", COLOR_YELLOW, student->student.email, COLOR_RESET);  
    printf("\n%sFather's Name: %s%s\n", COLOR_BLUE, student->father.name, COLOR_RESET);
    printf("%sFather's Phone: %s%s\n", COLOR_BLUE, student->father.phone, COLOR_RESET);
    printf("%sFather's Email: %s%s\n", COLOR_BLUE, student->father.email, COLOR_RESET);
    printf("\n%sMother's Name: %s%s\n", COLOR_MAGENTA, student->mother.name, COLOR_RESET);
    printf("%sMother's Phone: %s%s\n", COLOR_MAGENTA, student->mother.phone, COLOR_RESET);
    printf("%sMother's Email: %s%s\n", COLOR_MAGENTA, student->mother.email, COLOR_RESET);
    printf("\n%sGuardian's Name: %s%s\n", COLOR_CYAN, student->guardian.name, COLOR_RESET);
    printf("%sGuardian's Phone: %s%s\n", COLOR_CYAN, student->guardian.phone, COLOR_RESET);
    printf("%sGuardian's Email: %s%s\n", COLOR_CYAN, student->guardian.email, COLOR_RESET);
    printf("\n%sPayment Information:%s\n", COLOR_GREEN, COLOR_RESET);
    printf("%sTotal Payable: TK%.2f%s\n", COLOR_GREEN, student->payment.total_payable, COLOR_RESET);
    printf("%sTotal Paid: TK%.2f%s\n", COLOR_GREEN, student->payment.total_paid, COLOR_RESET);
    printf("%sTotal Due: TK%.2f%s\n", COLOR_RED, student->payment.total_due, COLOR_RESET);
    printf("%sOther Charges: TK%.2f%s\n", COLOR_MAGENTA, student->payment.total_others, COLOR_RESET);
    printf("%sFinal Balance: TK%.2f%s\n", COLOR_BLUE, student->payment.total_due + student->payment.total_others, COLOR_RESET);
}