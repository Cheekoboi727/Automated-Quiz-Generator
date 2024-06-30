#include <iostream>                         // input output
#include <iomanip>                          // for precision of average to 2 decimal places
#include <limits>                           // to remove all characters from input buffer after error handling
#include <cstdlib>                          // for srand and rand functions
#include <time.h>                           // for time related operations, specifically srand(time(0))
#include <random>                           // To generate Random numbers
#include <ctime>                            // also for time related functions
#include <fstream>                          // used to acces external files, for reading from and writing into
#include <sstream>                          // used for string stream operations, specifically to read data from csv files
#include <algorithm>                        // for find() function looking for tracked quiz
#include <hpdf.h>                           // external library to export PDF

using namespace std;

class Node                                  // Node to store each question of a quiz
{
    public:
    string question;
    string *options;
    int correctIndex;
    Node *next;    

    Node()
    {
        question = nullptr;
        options = nullptr;
        correctIndex = 0;
        next = nullptr;
    }

    Node(string val,string op1, string op2, string op3, string op4,int cor)
    {
        question = val;
        options = new string[4];
        options[0] = op1;
        options[1] = op2;
        options[2] = op3;
        options[3] = op4;
        correctIndex = cor;
        next = NULL;
    }
};

class SimpleQueue                               // Static Queue of Node type objects
{
    Node **arr;
    int front;
    int rear;
    int size;
    int count;

    public:
    SimpleQueue(int Size)
    {
        arr = new Node*[Size];            
        front = rear = -1;
        size = Size;
        count = 0;
    }
    void Enqueue(Node* n)
    {
        if (!isFull())
        {
            rear = (rear+1)%size;
            count++;
            arr[rear] = n;          
        }
        else
            cout<<"\nQueue is Full.";    
    }
    Node* Dequeue()
    {
        if (!isEmpty())
        {
            front = (front +1)%size;
            count--;
            return arr[front];  
        } 
        else
            cout<<"\nQueue is Empty.";
    }
    void PrintQueue()                   
    {
        if (!isEmpty())
        {
            cout<<endl;
            for (int i = front;i<=rear;i++)
            {
                cout<<arr[i]<<"  ";
            }
        }
        else
            cout<<"\nQueue is empty.";
    }
    bool isFull()
    {
        if (rear == front && count != 0)             
            return true;
        return false;
    }
    bool isEmpty()
    {
        if (rear == front && count == 0)               
            return true;
        return false;
    }

    bool isPresent(Node* val)
    {
        for (int i=front+1 ; i< rear; i++)
        {
            if(arr[i]->question == val->question)
                return true;
        }
        return false;
    }

    int Length()
    {
        return count;
    }

};


class LinkedList                                // Node type Linked List
{
    Node* head;

    public:
    LinkedList()
    {
        head = NULL;
    }
    void Append(string val,string op1, string op2, string op3, string op4,int cor)
    {
        Node *n1 = new Node(val, op1, op2,op3,op4,cor);
        if (head == NULL)
        {
            head = n1;
        }
        else
        {
            Node *temp = head;
            while (temp->next != NULL)
            {
                temp = temp->next;
            }
            temp->next = n1;
        }
    }
    
    Node* ValueatIndex(int index)
    {
        Node *temp = head;
        int count = 0;
        while (temp->next != NULL && count<index)
        {
            temp = temp->next;
            count++;
        }
        if (count == index)
        {
            return temp;
        }
    }
    void DeleteatIndex(int index)
    {
        Node *temp = head;
        int count = 0;
        if (index == 0)
        {
            Node* del = head;
            head= head->next;
            delete del;
            del = NULL;
        }
        else
        {
            while (temp->next != NULL && count<index-1)
            {
                temp = temp->next;
                count++;
            }
            if (count == index-1)
            {
                Node *del = temp->next;
                temp->next = temp->next->next;
                delete del;
                del = NULL;
            }
            else
            {
                cout<<"\nIndex out of range.";
            }
        }
    }    
  
   
    bool isEmpty()
    {
        if (head == NULL)
            return true;
        return false;
    }


};



LinkedList QuizFromCSV(string filename)                     // makes a quiz
{
    LinkedList quizList;                                    
    string fullpath = "QuizFiles\\"+filename;               // relative path of file

    ifstream file(fullpath);                                // file is accessed for input stream

    if (!file.is_open())                                    // if file is not opened
    {
        std::cerr << "Error opening file: " << filename << std::endl;       // error display message
        return quizList;
    }

    string line;                                            // string for data from csv, for each line
    while (getline(file, line))                             // while loop runs as long as there are lines to access in .csv
    {
        std::stringstream ss(line);
        std::string question, option1, option2, option3, option4;
        int correctOption;

        if (getline(ss, question, ',') && getline(ss, option1, ',') && getline(ss, option2, ',') && getline(ss, option3, ',') && getline(ss, option4, ',') && (ss >> correctOption)) 
        {
            quizList.Append(question, option1, option2, option3, option4, correctOption);       // string from ss to variable, till a comma ","
        } 
        else 
        {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();                                   // close the file after access is not required

    return quizList;                                // return the quiz
}

struct User                         // user structure for data storing for each user.
{
    string name;                    // name
    string QuizSub;                 // quiz subject
    char Diff;                      // difficulty
    int score;                      // score in that quiz
    int count;                      // number of times quiz taken

    User(string n, string Qname, char Dif, int s, int c)
        : name(n), QuizSub(Qname), Diff(Dif), score(s), count(c) {}
};

class QuizManager 
{
    private:
    vector<User> users;             // vector of user type, with all users from .csv file

    public:
    void loadUsers()                // 
    {
        ifstream file("UserData\\users.csv");   
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string name, QuizName, difficulty, scoreStr, countStr;

            if (getline(ss, name, ',') && getline(ss, QuizName, ',') && getline(ss, difficulty, ',') && getline(ss, scoreStr, ',') && getline(ss, countStr)) {
                try 
                {
                    int score = stoi(scoreStr);         // string from .csv converted to integer
                    int count = stoi(countStr);
                    users.emplace_back(name, QuizName, difficulty[0], score, count);
                } 
                catch (const exception& e)              // catches exception
                {
                    cerr << "Error parsing line: " << line << " - " << e.what() << endl;    // cout the name of exception
                }
            }
        }

        file.close();
    }

    void saveUsers() const 
    {
        ofstream file("UserData\\users.csv");               // file is accessed for output

        for (const auto& user : users)                      // iterates over users vector
        {
            file << user.name << "," << user.QuizSub << "," << user.Diff << "," << user.score << "," << user.count << "\n";
        }                                                   // saves string stream in the file

        file.close();
    }

    void addUser(string name, string quizName, char dif, int score) 
    {
    // Check if the user already exists
        for (auto& user : users) 
        {
            if (user.name == name && user.QuizSub == quizName && user.Diff == dif) 
            {
            // User already exists, update the score and return
                user.score += score;
                return;
            }
        }

    // User doesn't exist, add a new user with count initialized to 0
        users.emplace_back(name, quizName, dif, score, 0);
    }

    bool userExists(string name, string quizName, char dif) const 
    {
        for (const auto& user : users)      // if user name, quiz and diff exist already, then true
        {
            if (user.name == name && user.QuizSub == quizName && user.Diff == dif) 
            {
                return true;
            }
        }
        return false;
    }

  
  
  
    void updateScore(string name, string QuizName, int newScore, char dif) 
    {
        for (auto& user : users) 
        {
            if (user.name == name && user.QuizSub == QuizName && user.Diff == dif)  //name, sub and diff are matched, score and count are incremented
            {
                user.score += newScore;
                user.count++; 
                return;
            }
        }
        cerr << "User not found for update: " << name << ", " << QuizName << ", " << dif << endl;  // if user not found, and not returned yet.
    }
    const vector<User>& getUsers() const 
    {
        return users;                           // get function
    }
};
string Subject(int x)               // function to convert number of subject into subject
{
    if (x==1)
        return "Geography";
    else if (x==2)
        return "Computer Science";
    else if (x==3)
        return "Science";
    else
        return "Philosophy";
}

char Difficulty(int x)              // convert number of diff to difficulty
{
    if (x==1)
        return 'E';
    else if (x==2)
        return 'M';
    else
        return 'H';
}

string Diff(int x)          // returns difficulty string
{
    if (x==1)
    {
        return "Easy";
    }
    else if (x==2)
    {
        return "Med";
    }
    else
    {
        return "Hard";
    }
}

SimpleQueue QuizMaker(LinkedList l,int count)               // Takes a Linked List with all questions, and number of questions for quiz
{
    SimpleQueue q1(count);                                  // makes a queue with size required for quiz
    while (q1.Length() != count)                            // until queue not full
    {
        int randomNumber = rand() % 20;                     // random question selected
        if (!q1.isPresent(l.ValueatIndex(randomNumber)))    // checked if it is already in queue      
            q1.Enqueue(l.ValueatIndex(randomNumber));       // if not, then added
    }
    return q1;
}

int QuizTaker(SimpleQueue q1)                               // takes queue with quiz
{
    int count1 = 0, score = 0;                              // count counts question number, score counts correct answers
    char ans;
    while (q1.Length() != 0)                                // runs until questions are end
    {
        count1++;
        Node *n = q1.Dequeue();                             // node with question retrieved
        cout<<count1<<") "<<n->question<<endl               // question and options are shown
        <<"a. "<<n->options[0]<<endl
        <<"b. "<<n->options[1]<<endl
        <<"c. "<<n->options[2]<<endl
        <<"d. "<<n->options[3]<<endl;
        cout<<"Your answer(a,b,c,d)\n";
        while (true)                                        // exception handling for incorrect input
        {
            if (cin>>ans)
            {
                if (ans == 'a' || ans == 'b' || ans == 'c' || ans == 'd')
                {
                    break;
                }
                else
                {
                    cout<<"Please give correct input\n";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }     
        cout<<endl;
        if (n->correctIndex == 1)                   // matching the correct index with user answer
        {
            if (ans == 'a')
            score++;
        }
        else if (n->correctIndex == 2)
        {
            if (ans == 'b')
            score++;
        }
        else if (n->correctIndex == 3)
        {
            if (ans == 'c')
            score++;
        }
        else if (n->correctIndex == 4)
        {
            if (ans == 'd')
            score++;
        }

    }
    return score;                           // returns user score
}

void AddQuestionToCSV(string filename) 
{
    ofstream file;                                      // access file in write mode
    string fullpath = "QuizFiles\\"+filename;           // relative file path
    file.open(fullpath, ios_base::app);                 // Open the file in append mode

    if (!file.is_open())                                // if file not opening
    {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string Question, op1, op2, op3, op4;
    int correctOption;

    beg:
                                                        
    cout << "\nPlease give the Question:\n";           // User input for the new question
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    getline(cin, Question);

    cout << "\nPlease give the options one by one:\n";
    getline(cin, op1);
    getline(cin, op2);
    getline(cin, op3);
    getline(cin, op4);

                                                        
    cout << "\nPlease enter the correct option (1-4):\n";  // User input for the correct option
    cin >> correctOption;

                                                        
    if (correctOption < 1 || correctOption > 4)                 // Validate correctOption to be within the range 1-4
    {
        std::cerr << "Invalid correct option. Please enter a number between 1 and 4.\n";
        goto beg;
    }                                                           // Write the new question data to the file

    
    
    file << Question << "," << op1 << "," << op2 << "," << op3 << "," << op4 << "," << correctOption << "\n";
    file.close();
}

void ExportQuizToPDF(string subject, SimpleQueue& quizQueue, string diff ) 
{
    HPDF_Doc pdf;                                               // a pdf doc variable is declared
    HPDF_Page page;                                             // a new page variable is declared

    pdf = HPDF_New(NULL, NULL);                                 // a doc is created
    if (!pdf)                                                   // if not created, then error raised
    {
        cout << "Error: Cannot create PDF document." << endl;
        return;
    }

    page = HPDF_AddPage(pdf);                                   // Create a new page in the PDF
    HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);     // page size and properties
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);      // Set font and size
    HPDF_Page_SetFontAndSize(page, font, 12);                   // That font is added to the page

    int questionNumber = 1;                                     // Add quiz questions to the PDF
    Node* currentQuestion;                                      // Node to store the question

    while (!quizQueue.isEmpty())                                // while queue is not empty, keep retrieving the questions
    {
        currentQuestion = quizQueue.Dequeue();
        if (questionNumber == 8 || questionNumber == 15)        // for every 8th question, new page is added
        {
            page = HPDF_AddPage(pdf);
            HPDF_Page_SetSize(page, HPDF_PAGE_SIZE_A4, HPDF_PAGE_PORTRAIT);
            HPDF_Page_SetFontAndSize(page, font, 12);
        }
        if (questionNumber > 7 && questionNumber < 15)                                 // for question above 7, text position is reset at top of the page
        {
            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, 50, 750 - ((questionNumber-1)-7) * 100);
        }
        else if (questionNumber > 14)                                 // for question above 7, text position is reset at top of the page
        {
            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, 50, 750 - ((questionNumber-1)-14) * 100);
        }
        else
        {
            HPDF_Page_BeginText(page);
            HPDF_Page_MoveTextPos(page, 50, 750 - (questionNumber - 1) * 100);
        }

        stringstream questionText;                              
        questionText << questionNumber << ") " << currentQuestion->question;        //question is retrieved
        HPDF_Page_ShowText(page, questionText.str().c_str());
        HPDF_Page_EndText(page);

        if (questionNumber > 7 && questionNumber < 15)                                         // add options to PDF
        {
            for (int i = 0; i < 4; ++i) 
            {
                stringstream optionText;
                char optionLetter = 'a' + i;
                optionText << optionLetter << ". " << currentQuestion->options[i];

                HPDF_Page_BeginText(page);
                HPDF_Page_MoveTextPos(page, 70, 750 - ((questionNumber-1)-7) * 100 - (i + 1) * 20);
                HPDF_Page_ShowText(page, optionText.str().c_str());
                HPDF_Page_EndText(page);
            } 
        }

        else if (questionNumber > 14)                                         // add options to PDF
        {
            for (int i = 0; i < 4; ++i) 
            {
                stringstream optionText;
                char optionLetter = 'a' + i;
                optionText << optionLetter << ". " << currentQuestion->options[i];

                HPDF_Page_BeginText(page);
                HPDF_Page_MoveTextPos(page, 70, 750 - ((questionNumber-1)-14) * 100 - (i + 1) * 20);
                HPDF_Page_ShowText(page, optionText.str().c_str());
                HPDF_Page_EndText(page);
            } 
        }

        else                                                            
        {
            for (int i = 0; i < 4; ++i) 
            {
                stringstream optionText;
                char optionLetter = 'a' + i;
                optionText << optionLetter << ". " << currentQuestion->options[i];

                HPDF_Page_BeginText(page);
                HPDF_Page_MoveTextPos(page, 70, 750 - (questionNumber - 1) * 100 - (i + 1) * 20);
                HPDF_Page_ShowText(page, optionText.str().c_str());
                HPDF_Page_EndText(page);
            }
        }
        ++questionNumber;
    }

    string filename = subject + diff + "_quiz.pdf";                     // filename is set for the PDF file
    string fullpath = "ExportedQuizzes\\"+filename;                     // relative file path
    HPDF_SaveToFile(pdf, fullpath.c_str());                             // file is saved

    HPDF_Free(pdf);                                                     // file is removed from memory

    cout << "Quiz exported to " << filename << " successfully." << endl;
}

int ActualLength (int l)                                    // translates the user option into actual number of questions
{
    if (l==1)
        return 5;
    else if (l==2)
        return 10;
    else   
        return 20;
}

int main()
{
    QuizManager quizManager;
    quizManager.loadUsers();
    string name;
    int score;

    srand(time(0));
    
    cout<<"Welcome!"<<endl;
    cout<<"What is your name?\n";
    getline(cin,name);
    start:
    cout<<endl<<endl<<endl<<endl;
    cout<<"What do you want to do?"<<endl;
    cout<<"1. List of Quizzes"<<endl
        <<"2. Take a Quiz"<<endl
        <<"3. Add Questions to an Existing Quiz"<<endl
        <<"4. Observe My Progress"<<endl
        <<"5. Generate PDF"<<endl
        <<"(Give a value from 1 to 5)"<<endl;
    int check;    
    while (true)
    {
        if (cin>>check)
        {
            if (check >=1 && check <=5)
            {
                break;
            }
            else
            {
                cout<<"Please give a value between 1 and 5.";
            }
        }
        else
        {
            cout<<"You gave a wrong input. Please try again.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    } 
  
    if (check==1)
    {
        cout<<endl<<endl<<endl<<endl;
        cout<<"\nThe quizzes are:\n1. Geography\n2. Computer Science\n3. Science\n4. Philosophy";
        cout<<endl<<"Press 1 to go back or 0 to quit the program.\n";
        int temp1;
        cin>>temp1;
        if (temp1 == 1)
        {
            cout<<endl;
            goto start;
        }
    }
    else if (check==2)
    {
        quiz:
        cout<<endl<<endl<<endl<<endl;
        LinkedList l;
        SimpleQueue q1(10);
        cout<<endl<<"Which quiz do you want to take?\n";
        cout<<"1. Geography\n2. Computer Science\n3. Science\n4. Philosophy";
        cout<<endl<<"(Give value from 1 to 4, Press 0 to go back)\n";
        int check2;
        while (true)
        {
            if (cin>>check2)
            {
                if (check2 >=1 && check2 <=4)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 1 and 4.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        if (check2 == 0)
            goto start;
 
        cout<<"\nWhat Difficulty do you want to do?"<<endl
            <<"1. Easy\n2. Medium\n3. Hard"<<endl;
        int check3;
        while (true)
        {
            if (cin>>check3)
            {
                if (check3 >=1 && check3 <=3)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 1 and 3.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }    
        }
        if (!quizManager.userExists(name, Subject(check2),Difficulty(check3))) 
        {
            quizManager.addUser(name,Subject(check2),Difficulty(check3), 0);
        } 
        cout<<endl<<endl<<endl;  
        if (check2 == 1)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 3)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;    
            }      

        }
        
        if (check2 == 2)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("ComputerEasy.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;    
            }      
            else if(check3 == 2)
            {
                l = QuizFromCSV("ComputerMed.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;    
            }      
            else if(check3 == 3)
            {
                l = QuizFromCSV("ComputerHard.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;    
            }      

        }

        if (check2 == 3)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("ScienceEasy.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("ScienceMed.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 3)
            {
                l = QuizFromCSV("ScienceHard.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            

        }

        if (check2 == 4)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("PhilosophyEasy.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("PhilosophyMed.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }

            else if(check3 == 3)
            {
                l = QuizFromCSV("PhilosophyHard.csv");
                q1 = QuizMaker(l,10);
                score = QuizTaker(q1);

                cout<<endl;
            }          

        } 
        int userScore = score;  
        quizManager.updateScore(name,Subject(check2), userScore, Difficulty(check3));  
        quizManager.saveUsers();  
        cout<<"\nPress 1 to retake a quiz, 2 to go back, or 0 to quit the program.";
        int temp0;
        while (true)
        {
            if (cin>>temp0)
            {
                if (temp0 >=0 && temp0 <=2)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 0 and 2.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }      
        
        if (temp0 == 1)
            goto quiz;
        else if (temp0 == 2)
            goto start;           
    }
    else if (check==3)
    {
        cout<<endl<<endl<<endl<<endl;
        cout<<"Which Quiz do you want to add a question to?\n";
        cout<<"1. Geography\n2. Computer Science\n3. Science\n4. Philosophy\n";
        int check2, check3;
        while (true)
        {
            if (cin>>check2)
            {
                if (check2 >=1 && check2 <=4)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 1 and 4.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        cout<<"What is the difficulty level: (1 for Easy; 2 for Medium; 3 for Hard) \n";
        cin>>check3;
        
        if (check2 == 1 && check3 == 1)
        {
            AddQuestionToCSV("Geography.csv");
        }
        if (check2 == 1 && check3 == 2)
        {
            AddQuestionToCSV("Geography.csv");
        }
        if (check2 == 1 && check3 == 3)
        {
            AddQuestionToCSV("Geography.csv");
        }
        if (check2 == 2 && check3 == 1)
        {
            AddQuestionToCSV("ComputerEasy.csv");
        }
        if (check2 == 2 && check3 == 2)
        {
            AddQuestionToCSV("ComputerMed.csv");
        }
        if (check2 == 2 && check3 == 3)
        {
            AddQuestionToCSV("ComputerHard.csv");
        }
        if (check2 == 3 && check3 == 1)
        {
            AddQuestionToCSV("ScienceEasy.csv");
        }
        if (check2 == 3 && check3 == 2)
        {
            AddQuestionToCSV("ScienceMed.csv");
        }
        if (check2 == 3 && check3 == 3 )
        {
            AddQuestionToCSV("ScienceHard.csv");
        }
        if (check2 == 4 && check3 == 1)
        {
            AddQuestionToCSV("PhilosophyEasy.csv");
        }
        if (check2 == 4 && check3 == 2)
        {
            AddQuestionToCSV("PhilosophyMed.csv");
        }
        if (check2 == 4 && check3 == 3)
        {
            AddQuestionToCSV("PhilosophyHard.csv");
        }      
        cout<<"\nQuestion added succesfully."
            <<"\nPress 1 to go back, or 2 to terminate.\n";
        int temp1;
        cin>>temp1;                  
        if (temp1==1)
        {
            cout<<endl;
            goto start;
        }

    }
    else if (check==4)
    {
        cout<<endl<<endl<<endl<<endl;
        int countt = 0;

// Find the user
        for (const auto& user : quizManager.getUsers())         //iterates over the users
        {
            if (user.name == name) 
            {
                if (countt == 0)
                {
                    cout << "\nAverage scores for " << name << ":\n";   // prints the name of user when it first comes
                }
                countt++;
                vector<pair<string, char>> trackedQuizzes;              // a pair vector for (Subject,Difficulty)
                pair<string, char> quiz(user.QuizSub, user.Diff);       // picks subject and difficulty of iterating user
                auto it = find(trackedQuizzes.begin(), trackedQuizzes.end(), quiz);     // looks for the quiz in vector
                if (it == trackedQuizzes.end())                         // if not found (That is why at end)
                {
                    trackedQuizzes.push_back(quiz);                     // push the new quiz
                    int totalScore = 0;                                 // initialises score and count at zero for first quiz
                    int quizCount = 0;

                    for (const auto& quizUser : quizManager.getUsers())     // reiterates on users again, finding all quizzes taken by the same user, and calculating average
                    {
                        if (quizUser.name == name && quizUser.QuizSub == user.QuizSub && quizUser.Diff == user.Diff) 
                        {
                            totalScore += quizUser.score;               // if quiz found, we increment the score and the count
                            quizCount += quizUser.count;
                        }
                    }

                    if (quizCount > 0)                                  // if quiz are taken, we calculate averages
                    {
                        double averageScore = static_cast<double>(totalScore) / quizCount;
                        cout << "Quiz: " << user.QuizSub << ", Difficulty: " << user.Diff << ", Average Score: " << fixed << setprecision(2) << averageScore << "\n";
                    }
                }
            }
        }

        cout<<"\n\nPress 1 to go back, or 0 to terminate the program.\n";
        int ter;
        while (true)
        {
            if (cin>>ter)
            {
                if (ter == 0 || ter == 1)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 1 and 4.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }
        if (ter == 1)
            goto start;

    }

    else if (check == 5)
    {
        cout<<endl<<endl<<endl<<endl;
        cout << "Which Quiz do you want to export to PDF?" << endl;
        cout << "1. Geography\n2. Computer Science\n3. Science\n4. Philosophy\n";
        int quizChoice;
        while (true) {
            if (cin >> quizChoice) 
            {
                if (quizChoice >= 1 && quizChoice <= 4) 
                {
                    break;
                } 
                else 
                {
                    cout << "Please give a value between 1 and 4." << endl;
                }
            } 
            else 
            {
                cout << "You gave a wrong input. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        string subject = Subject(quizChoice);
        int difficulty;
        cout << "Enter the difficulty level (1. Easy, 2. Medium, 3. Hard): ";
        while (true) {
            if (cin >> difficulty) 
            {
                if (difficulty >= 1 && difficulty <= 3) 
                {
                    break;
                } 
                else 
                {
                    cout << "Please give a value between 1 and 3." << endl;
                }
            } 
            else 
            {
                cout << "You gave a wrong input. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        cout<<"\nWhat Should be the length of the Quiz?"<<endl
            <<"1. Short\n2. Medium\n3. Long"<<endl;
        int length;
        while (true)
        {
            if (cin>>length)
            {
                if (length >=1 && length <=3)
                {
                    break;
                }
                else
                {
                    cout<<"Please give a value between 1 and 3.";
                }
            }
            else
            {
                cout<<"You gave a wrong input. Please try again.\n";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }    
        }
        LinkedList quizList;
        if (quizChoice==1 && difficulty>=1 && difficulty<=3)
        {
            quizList = QuizFromCSV("Geography.csv");
        }
        else if (quizChoice==2 && difficulty==1)
        {
            quizList = QuizFromCSV("ScienceEasy.csv");
        }
        else if (quizChoice==2 && difficulty==2)
        {
            quizList = QuizFromCSV("ScienceMed.csv");
        }
        else if (quizChoice==2 && difficulty==3)
        {
            quizList = QuizFromCSV("ScienceHard.csv");
        }
        else if (quizChoice==3 && difficulty==1)
        {
            quizList = QuizFromCSV("ComputerEasy.csv");
        }
        else if (quizChoice==3 && difficulty==2)
        {
            quizList = QuizFromCSV("ComputerMed.csv");
        }
        else if (quizChoice==3 && difficulty==3)
        {
            quizList = QuizFromCSV("ComputerHard.csv");
        }
        else if (quizChoice==4 && difficulty==1)
        {
            quizList = QuizFromCSV("PhilosophyEasy.csv");
        }
        else if (quizChoice==4 && difficulty==2)
        {
            quizList = QuizFromCSV("PhilosophyMed.csv");
        }
        else if (quizChoice==4 && difficulty==3)
        {
            quizList = QuizFromCSV("PhilosophyHard.csv");
        }
        SimpleQueue quizQueue = QuizMaker(quizList,ActualLength(length));

        // Export quiz to PDF
        ExportQuizToPDF(subject, quizQueue,Diff(difficulty));

        cout << "\nPress 1 to go back or 0 to quit the program." << endl;
        int temp;
        while (true) {
            if (cin >> temp) 
            {
                if (temp == 1 || temp == 0) 
                {
                    break;
                } 
                else 
                {
                    cout << "Please give a value of 0 or 1." << endl;
                }
            } 
            else 
            {
                cout << "You gave a wrong input. Please try again." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        }

        if (temp == 1) 
        {
            cout << endl;
            goto start;
        }
    
    }
    end:
    cout<<"\nThe program is terminated.";
}

