// sb ops 'a' nhi true to wo dekhna
// csv files main add krnay aur questions.
// add new question wala part csv k liye

#include <iostream>
#include <iomanip>
#include <limits>
#include <cstdlib>
#include <time.h>
#include <random>
#include <ctime>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <windows.h>

using namespace std;

class Node
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

class SimpleQueue
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
            arr[rear] = n;          //first increments rear, and then adds new value
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
    void PrintQueue()                   //Print whole queue for testing purpose
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
        if (rear == front && count != 0)             //Same as Stack.
            return true;
        return false;
    }
    bool isEmpty()
    {
        if (rear == front && count == 0)                //In empty queue, front and rear point to same empty cell.
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

class Stack                  
{
    Node **arr;
    int top;
    int size;

    public:
    Stack(int Size)
    {
        arr = new Node*[Size];        //initializes an array of size "size"  
        top = -1;                   //Top initializes at -1
        size = Size;                
    }
    void Push(Node* n)
    {
        if (!isFull())
            arr[++top] = n;        //increments top and adds new value at top  
        else
            cout<<"\nStack is Full.";
    }
    void Pop()
    {
        if (!isEmpty())               //Deletes top most element, does not return anything. Sir taught Pop as both
            top--;                    //return top element and delete. But official Stacks do not do that.
        else
            cout<<"\nStack is Empty.";
                                        
    }                                   
    Node* Top()
    {
        if (!isEmpty())
            return arr[top];            //Returns top most element
        else
            cout<<"\nStack is Empty.";
    }
    void printStack()               //To print whole stack for testing purpose
    {
        if (!isEmpty())
        {
            cout<<endl;
            for(int i=0;i<=top;i++)
            {
                cout<<arr[i]<<"  ";
            }
        }
        else
            cout<<"\nStack is Empty.";    
    }
    bool isFull()
    {
        if (top == size-1)          //As top (index) is one less than size
            return true;
        return false;
    }
    bool isEmpty()
    {
        if (top == -1)              //As in empty stack, top = -1
            return true;
        return false;
    }

};


class LinkedList
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

    void InsertHead(Node *ptr)
    {

    }
};



LinkedList QuizFromCSV(string filename) 
{
    LinkedList quizList;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return quizList;
    }

    std::string line;
    while (getline(file, line)) {
        std::stringstream ss(line);
        std::string question, option1, option2, option3, option4;
        int correctOption;

        if (getline(ss, question, ',') && getline(ss, option1, ',') && getline(ss, option2, ',') && getline(ss, option3, ',') && getline(ss, option4, ',') && (ss >> correctOption)) {
            quizList.Append(question, option1, option2, option3, option4, correctOption);
        } else {
            std::cerr << "Error parsing line: " << line << std::endl;
        }
    }

    file.close();

    return quizList;
}

struct User {
    string name;
    string QuizSub;
    char Diff;
    int score;
    int count; // New count variable

    User(string n, string Qname, char Dif, int s, int c)
        : name(n), QuizSub(Qname), Diff(Dif), score(s), count(c) {}
};

class QuizManager {
private:
    vector<User> users;

public:
    void loadUsers() {
        ifstream file("users.csv");
        string line;

        while (getline(file, line)) {
            stringstream ss(line);
            string name, QuizName, difficulty, scoreStr, countStr;

            if (getline(ss, name, ',') && getline(ss, QuizName, ',') && getline(ss, difficulty, ',') && getline(ss, scoreStr, ',') && getline(ss, countStr)) {
                try {
                    int score = stoi(scoreStr);
                    int count = stoi(countStr);
                    users.emplace_back(name, QuizName, difficulty[0], score, count);
                } catch (const exception& e) {
                    cerr << "Error parsing line: " << line << " - " << e.what() << endl;
                }
            }
        }

        file.close();
    }

    void saveUsers() const {
        ofstream file("users.csv");

        for (const auto& user : users) {
            file << user.name << "," << user.QuizSub << "," << user.Diff << "," << user.score << "," << user.count << "\n";
        }

        file.close();
    }

    void addUser(const string& name, string quizName, char dif, int score) 
    {
    // Check if the user already exists
        for (auto& user : users) {
            if (user.name == name && user.QuizSub == quizName && user.Diff == dif) {
            // User already exists, update the score and return
                user.score += score;
                return;
            }
        }

    // User doesn't exist, add a new user with count initialized to 1
        users.emplace_back(name, quizName, dif, score, 0);
    }

    bool userExists(const string& name, string quizName, char dif) const {
        for (const auto& user : users) {
            if (user.name == name && user.QuizSub == quizName && user.Diff == dif) {
                return true;
            }
        }
        return false;
    }

    void updateScore(const string& name, const string& QuizName, int newScore, char dif) {
        for (auto& user : users) {
            if (user.name == name && user.QuizSub == QuizName && user.Diff == dif) {
                user.score += newScore;
                user.count++; // Increment the count
                return;
            }
        }
        cerr << "User not found for update: " << name << ", " << QuizName << ", " << dif << endl;
    }
    const vector<User>& getUsers() const 
    {
        return users;
    }
};
string Subject(int x)
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

char Difficulty(int x)
{
    if (x==1)
        return 'E';
    else if (x==2)
        return 'M';
    else
        return 'H';
}

SimpleQueue QuizMaker(LinkedList l)
{
    SimpleQueue q1(10);
    while (q1.Length() != 10)
    {
        int randomNumber = rand() % 15;
        if (!q1.isPresent(l.ValueatIndex(randomNumber)))
        q1.Enqueue(l.ValueatIndex(randomNumber));
    }
    return q1;
}

int QuizTaker(SimpleQueue q1)
{
    int count1 = 0, score = 0;
    char ans;
    while (q1.Length() != 0)
    {
        count1++;
        Node *n = q1.Dequeue();
        cout<<count1<<") "<<n->question<<endl
        <<"a. "<<n->options[0]<<endl
        <<"b. "<<n->options[1]<<endl
        <<"c. "<<n->options[2]<<endl
        <<"d. "<<n->options[3]<<endl;
        cout<<"Your answer(a,b,c,d)\n";
        cin>>ans;
        cout<<endl;
        if (n->correctIndex == 1)
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
}

void AddQuestionToCSV(string filename) {
    std::ofstream file;
    file.open(filename, std::ios_base::app); // Open the file in append mode

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string Question, op1, op2, op3, op4;
    int correctOption;

    beg:
    // User input for the new question
    std::cout << "\nPlease give the Question:\n";
    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, Question);

    std::cout << "\nPlease give the options one by one:\n";
    std::getline(std::cin, op1);
    std::getline(std::cin, op2);
    std::getline(std::cin, op3);
    std::getline(std::cin, op4);

    // User input for the correct option
    std::cout << "\nPlease enter the correct option (1-4):\n";
    std::cin >> correctOption;

    // Validate correctOption to be within the range 1-4
    if (correctOption < 1 || correctOption > 4) {
        std::cerr << "Invalid correct option. Please enter a number between 1 and 4.\n";
        goto beg;
    }

    // Write the new question data to the file
    file << Question << "," << op1 << "," << op2 << "," << op3 << "," << op4 << "," << correctOption << "\n";


    file.close();
}

int main()
{
    QuizManager quizManager;
    quizManager.loadUsers();
    string name;
    int score;

    srand(time(0));
    
    cout<<"Welcome!"<<endl;
    cout<<"\nWhat is your name?\n";
    getline(cin,name);
    start:
    cout<<"What do you want to do?"<<endl;
    cout<<"1. List of Quizzes"<<endl
        <<"2. Take a Quiz"<<endl
        <<"3. Add Questions to an Existing Quiz"<<endl
        <<"4. Observe My Progress"<<endl
        <<"(Give a value from 1 to 4)"<<endl;
    int check;    
    while (true)
    {
        if (cin>>check)
        {
            if (check >=0 && check <=4)
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
  
    if (check==1)
    {
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
        if (check2 == 1)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 3)
            {
                l = QuizFromCSV("Geography.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;    
            }      

        }
        
        if (check2 == 2)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("ComputerEasy.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;    
            }      
            else if(check3 == 2)
            {
                l = QuizFromCSV("ComputerMed.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;    
            }      
            else if(check3 == 3)
            {
                l = QuizFromCSV("ComputerHard.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;    
            }      

        }

        if (check2 == 3)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("ScienceEasy.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("ScienceMed.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 3)
            {
                l = QuizFromCSV("ScienceHard.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            

        }

        if (check2 == 4)
        {
            if (check3 == 1)
            {
                l = QuizFromCSV("PhilosophyEasy.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }
            else if(check3 == 2)
            {
                l = QuizFromCSV("PhilosophyMed.csv");
                q1 = QuizMaker(l);
                score = QuizTaker(q1);

                cout<<endl;
            }

            else if(check3 == 3)
            {
                l = QuizFromCSV("PhilosophyHard.csv");
                q1 = QuizMaker(l);
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
        // if (temp0 == 0)
        //     goto end;
        if (temp0 == 1)
            goto quiz;
        else if (temp0 == 2)
            goto start;           
    }
    else if (check==3)
    {
        cout<<"Which Quiz do you want to add a question to?\n";
        cout<<"1. Geography\n2. Computer Science\n3. Science\n4. Philosophy\n";
        int check2, check3;
        while (true)
        {
            if (cin>>check2)
            {
                if (check2 >=0 && check2 <=4)
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

        cout<<"What is the difficulty level: \n";
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
        else
        {
            //goto end;
        }
    }
    else if (check==4)
    {
        int countt = 0;

// Find the user
        for (const auto& user : quizManager.getUsers()) 
        {
            if (user.name == name) 
            {
                if (countt == 0)
        // User found, calculate average score for each quiz
                    std::cout << "\nAverage scores for " << name << ":\n";
                countt++;
        // Track quizzes to avoid duplicate entries
                vector<pair<string, char>> trackedQuizzes;

                pair<string, char> quiz(user.QuizSub, user.Diff);
                auto it = find(trackedQuizzes.begin(), trackedQuizzes.end(), quiz);
        // Check if the user has taken this quiz
                if (it == trackedQuizzes.end()) 
                {
                    trackedQuizzes.push_back(quiz);

            // Calculate average score for the quiz
                    int totalScore = 0;
                    int quizCount = 0;

                    for (const auto& quizUser : quizManager.getUsers()) 
                    {
                        if (quizUser.name == name && quizUser.QuizSub == user.QuizSub && quizUser.Diff == user.Diff) 
                        {
                            totalScore += quizUser.score;
                            quizCount += quizUser.count;
                        }
                    }

                    if (quizCount > 0) 
                    {
                        double averageScore = static_cast<double>(totalScore) / quizCount;
                        cout << "Quiz: " << user.QuizSub << ", Difficulty: " << user.Diff << ", Average Score: " << fixed << setprecision(2) << averageScore << "\n";
                    }
                }
            }
        }

    }
    end:
    cout<<"\nThe program is terminated.";
}

