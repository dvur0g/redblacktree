#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <math.h>
#include <windows.h>
#include <conio.h>


HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
const int colorDefault = 15, colorRed = 12;

void gotoxy(int x, int y) 
{
	COORD coord;	
  	coord.X = x;
  	coord.Y = y;
  	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int menu(std::string text) 
{
    const int upkey = 119, downkey = 115, enterkey = 13, esckey = 27,
            uparrow = 72, downarrow = 80, leftarrow = 75, rightarrow = 77, bias = 48;

	int i = 0, count = 0, type = 0, y = 0, x = 32;
	std::string arrow = "<---", empty = "    ";
	
    while(text[i]) 
    {
		if(text[i] == '\n')
			count++;
		i++;
	}

	gotoxy(0, 0); //system("cls");
	std::cout << text << "\n" << count + 1 << ". Quit (esc)";
	
	gotoxy(x, type);
	std::cout << arrow;

	while(true) 
    {
		int a = getch();
		if(a > bias && a <= bias + count) 
        {
			type = a - bias - 1;
			break;
		} 
        else if((a == downarrow || a == downkey) && type < count - 1) 
        {
			gotoxy(x, type);
			std::cout << empty;
			gotoxy(x, ++type);
			std::cout << arrow;
		} 
        else if((a == uparrow || a == upkey) && type > 0) 
        {
			gotoxy(x, type);
			std::cout << empty;
			gotoxy(x, --type);
			std::cout << arrow;
		} 
        else if(a == enterkey)
			break;
		else if(a == esckey)
			return count;
	}
	system("cls");
	return type;
}

int input(char* str, int min, int max) 
{
	int result = min - 1;
	while(result <= min || result >= max) 
    {
		system("cls");
		std::cout << str << " (" << min << ", " << max << "): ";
		std::cin >> result;
	}	
	return result;
}

struct node 
{
    bool red;
    int data;
    struct node *link[2];
};

class redblacktree 
{
    node *root;
    int length;

    bool isRed(node *node)
    {
        return node != NULL && node -> red == true;
    }

    node *rotateOnce(node *root, int dir)
    {
        node *save = root -> link[!dir];
        
        root -> link[!dir] = save -> link[dir];
        save -> link[dir] = root;
        
        root -> red = 1;
        save -> red = 0;
        
        return save;
    }

    node *rotateTwice(node *root, int dir)
    {
        root -> link[!dir] = rotateOnce(root -> link[!dir], !dir);
        return rotateOnce(root, dir);
    }
    
    node *makeNode(int data)
    {
        node *p = new node();
       
        p -> data = data;
        p -> red = true;
        p -> link[0] = NULL;
        p -> link[1] = NULL;
        
        return p;
    }

    void inOrder(node *p)
    {
        if (p == NULL)
            return;

        inOrder(p -> link[0]);
        std::cout << p -> data << " ";
        inOrder(p -> link[1]);
    }
   
    void preOrder(node *p)
    {
        if(p == NULL)
            return;

        std::cout << p -> data << " ";
        preOrder(p -> link[0]);
        preOrder(p -> link[1]);
    }

    void revOrder(node *p)
    {
        if(p == NULL)
            return;
               
        revOrder(p -> link[0]);
        revOrder(p -> link[1]);
        std::cout << p -> data << " ";
    }

    void print(const std::string& line, const node* p, bool right)
    {
        if(p == NULL)
            return;
            
        std::cout << line;

        std::cout << (right ? rightLine : leftLine);
        if(p -> red)
            SetConsoleTextAttribute(hConsole, colorRed);
        std::cout << p -> data << "\n";
        SetConsoleTextAttribute(hConsole, colorDefault);

        print(line + (right ? continueLine : "    "), p -> link[1], true);
        print(line + (right ? continueLine : "    "), p -> link[0], false);
    }

    std::string leftLine = "";
    std::string rightLine = "";
    std::string continueLine = "";

    public:
        void fillLines()
        {
            rightLine.push_back(char(195));//├
            leftLine.push_back(char(192));//└
            continueLine.push_back(char(179));//│

            for(int i = 0; i < 2; ++i)
            {
                leftLine.push_back(char(196));//--
                rightLine.push_back(char(196));//--
                continueLine.push_back(' ');
            }
        }

        void add(int data)
        {
            if(root == NULL) 
                root = makeNode(data);
            
            node head = {0};
            node *g, *t, *p, *q;
            int dir = 0, last;
    
            t = &head;
            g = p = NULL;
            q = t -> link[1] = root;
    
            while(true) 
            {
                if(q == NULL) {
                    p -> link[dir] = q = makeNode(data);
                    ++length;
                }
                else if(isRed(q -> link[0]) && isRed(q -> link[1])) 
                {
                    q -> red = true;
                    q -> link[0] -> red = false;
                    q -> link[1] -> red = false;
                }
                
                if(isRed(q) && isRed(p)) 
                {
                    int dir2 = t -> link[1] == g;
        
                    if(q == p -> link[last])
                        t -> link[dir2] = rotateOnce(g, !last);
                    else
                        t -> link[dir2] = rotateTwice(g, !last);
                }
        
                if (q -> data == data)
                    break;
        
                last = dir;
                dir = q -> data < data;
        
                if(g != NULL)
                    t = g;
                
                g = p, p = q;
                q = q -> link[dir];
            }
            root = head.link[1];
            root -> red = 0;
        }

        void del(int data)
        {
            if(root == NULL) 
                return;

            node head = {0};
            node *q, *p, *g, *f = NULL;
            int dir = 1;

            q = &head;
            g = p = NULL;
            q -> link[1] = root;
            
            while(q -> link[dir] != NULL) 
            {
                int last = dir;
    
                g = p, p = q;
                q = q -> link[dir];
                dir = q -> data < data;
                
                if(q -> data == data)
                    f = q;
                
                if (!isRed(q) && !isRed(q -> link[dir])) 
                {
                    if(isRed(q -> link[!dir]))
                        p = p -> link[last] = rotateOnce(q, dir);
                    else if(!isRed(q -> link[!dir])) 
                    {
                        node *s = p -> link[!last];

                        if (s != NULL) 
                        {
                            if(!isRed(s -> link[!last]) && !isRed(s -> link[last])) 
                            {
                                p -> red = false;
                                s -> red = true;
                                q -> red = true;
                            }
                            else 
                            {
                                int dir2 = g -> link[1] == p;
                        
                                if(isRed(s -> link[last]))
                                    g -> link[dir2] = rotateTwice(p, last);
                                else if(isRed(s -> link[!last]))
                                    g -> link[dir2] = rotateOnce(p, last);
                    
                                q -> red = g -> link[dir2] -> red = true;
                                g -> link[dir2] -> link[0] -> red = false;
                                g -> link[dir2] -> link[1] -> red = false;
                            }
                        }
                    }
                }
            }
                
            if(f != NULL) 
            {
                f -> data = q -> data;
                p -> link[p -> link[1] == q] = q -> link[q -> link[0] == NULL];
                delete q;
            }
            root = head.link[1];
            if(root != NULL)
                root -> red = 0;
        }

        node* find(int key)
        {
            node *p = root;
            while(p != NULL && p -> data != key)
            {
                if(key < p -> data)
                    p = p -> link[0];
                else
                    p = p -> link[1];
            }
            return p;
        }

        void printInOrder()
        {
            inOrder(root);
        }

        void printPreOrder(node *p)
        {
            preOrder(root);
        }

        void printRevOrder()
        {
            revOrder(root);
        }       

        void print()
        {
            print("", root, false);    
        }
};

int main()
{
    setlocale(0, "C");
    srand(time(NULL));

    redblacktree *tree = new redblacktree();
    tree -> fillLines();
    
    int t;
    while(true)
    {
        system("cls");
        t = menu("1. add()\n2. del()\n3. find()\n4. printAsTree()\n5. printAsArray()\n6. addManyNumbers()\n");

        if(t == 0)
            tree -> add(input("input number to add", 0, 1000));
        else if(t == 1)
            tree -> del(input("input number to delete", 0, 1000));
        else if(t == 2)
        {
            node *p = tree -> find(input("input number to find", 0, 1000));
            if(p != NULL)
                std::cout << p -> data;
            else
                std::cout << "Not found";
            getch();
        }
        else if(t == 3)
        {
            system("cls");
            std::cout << "Tree:\n";
            tree -> print();
            getch();
        }
        else if(t == 4)
        {
            tree -> printInOrder();
            getch();
        }
        else if(t == 5)
        {
            //рандомно запонить дерево
            int amount = input("How many numbers to add? ", 0, 100);
            for(int i = 0; i < amount; i++)
                tree -> add(rand()%1000);
        }
        else
            break;
    }
    std::cout << "\n";

    return 0;
}