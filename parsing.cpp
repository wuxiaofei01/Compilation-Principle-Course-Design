#include "get_word.h"

const int maxn = 1000000;
pair<string, int> Token[maxn]; //存放TOKEN串
string str[maxn];
int row[maxn];
int pos_token = 0;    //当前TOKEN串读到第几个位置了
vector<string> error; //存放第一次扫描可以得到的错误信息

#define INT \
    pair<string, int> { "K", 1 }
#define VOID \
    pair<string, int> { "K", 2 }
#define BREAK \
    pair<string, int> { "K", 3 }
#define FLOAT \
    pair<string, int> { "K", 4 }
#define WHILE \
    pair<string, int> { "K", 5 }
#define DO \
    pair<string, int> { "K", 6 }
#define STRUCT \
    pair<string, int> { "K", 7 }
#define CONST \
    pair<string, int> { "K", 8 }
#define CASE \
    pair<string, int> { "K", 9 }
#define FOR \
    pair<string, int> { "K", 10 }
#define RETURN \
    pair<string, int> { "K", 11 }
#define IF \
    pair<string, int> { "K", 12 }
#define DEFALUT \
    pair<string, int> { "K", 13 }
#define ELIF \
    pair<string, int> { "K", 14 }
#define CHAR \
    pair<string, int> { "K", 15 }

#define ELSE \
    pair<string, int> { "K", 17 }
#define BOOL \
    pair<string, int> { "K", 18 }

void Const_declaration(); //常量声明
void Identifier();//标识符
void Variable_declaration();//变量声明
void Const_Define();//常量定义
void Value_Function();//有返回值的函数定义
void Integer();//数字
void Char();//字符
void PROCESS();//进程
void Variable_Define();//变量定义
void Main();//主函数
void Compound_Sentence();//复合语句
void Parameters_Table();//参数表
void Statement_List();//语句列
int Statement();//语句
void Conditional_Statements();//条件语句
void Expression();//表达式
void Condition();//条件
void Loop_Statement();//循环语句
void Assignment_Statement();//赋值语句
void While_Statement();;//while 语句
void For_Statement();//for语句
void Return_Function();//返回语句
int Function_parameter();//函数的参数表
void None_Define(int cnt);//检查标识符未定义
void Check_parsing();


void Const_declaration() //结束后的位置
{
    while (Token[pos_token] == CONST)
    {
        pos_token++;
        //再进入常量定义
        Const_Define();
    }
}
void Identifier()
{
    if (Token[pos_token].first == "I")
    {
        pos_token++;
        return;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "缺少标识符\n";
        exit(0);
    }
}

void Types()
{
    if (Token[pos_token] == INT || Token[pos_token] == CHAR)
    {
        pos_token++;
        return;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "缺少类型标识符\n";
        exit(0);
    }
}

void Unsigned_Integer()
{
    if (Token[pos_token].first == "C1" && str[pos_token] != "0" || (Token_C1[str[pos_token]]))
    {
        pos_token++;
        return;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "无符号整数错误";
        exit(0);
    }
}

void Variable_declaration()
{
    while ((Token[pos_token] == INT || Token[pos_token] == CHAR) && Token[pos_token + 1] != pair<string, int>{"K", 16} && (Token[pos_token + 2] != pair<string, int>{"P", 3}))
    {
        Variable_Define();
    }
}

void Variable_Define()
{
    Types();
    Identifier();

    if (Token[pos_token] == pair<string, int>{"P", 17}) //判断数组
    {
        pos_token++;
        Unsigned_Integer();
        if (Token[pos_token] == pair<string, int>{"P", 18})
        {
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "缺少 \"]\" 符号\n";
            exit(0);
        }
    }

    if (Token[pos_token] == pair<string, int>{"P", 11})
    {
        pos_token++;
        Expression();
    }

    while (Token[pos_token] == pair<string, int>{"P", 12}) // ，间隔符号
    {
        pos_token++;
        Identifier();
        if (Token[pos_token] == pair<string, int>{"P", 11})
        {
            pos_token++;
            Expression();
        }
        if (Token[pos_token] == pair<string, int>{"P", 17}) //判断数组
        {
            pos_token++;
            Unsigned_Integer();
            if (Token[pos_token] == pair<string, int>{"P", 18})
            {
                pos_token++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "缺少 \"]\" 符号\n";
                exit(0);
            }
        }
    }
    if (Token[pos_token] == pair<string, int>{"P", 13})
        pos_token++;
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "缺少 ; ";
        exit(0);
    }
}
void Const_Define()
{
    bool flag = 0; //0 int 1 char
    if (Token[pos_token] == INT || Token[pos_token] == CHAR)
    {
        pos_token++;
        Identifier();
        string const_variable = str[pos_token - 1];
        Token_C1[const_variable] = 1;
        if (Token[pos_token] == pair<string, int>{"P", 11})
        {
            pos_token++;
            if (Token[pos_token - 3] == INT)
            {
                flag = 0;
                Integer();
            }
            else if (Token[pos_token - 3] == CHAR)
            {
                flag = 1;
                Char();
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "常量定义错误\n";
                exit(0);
            }
            while (Token[pos_token] == pair<string, int>{"P", 12})
            {
                pos_token++;
                Identifier();

                if (Token[pos_token] == pair<string, int>{"P", 11})
                {
                    pos_token++;
                    if (flag == 0)
                    {
                        Integer();
                    }
                    else if (flag == 1)
                    {
                        Char();
                    }
                    else
                    {
                        printf("第 %d 行", row[pos_token]);
                        cout << "常量定义错误\n";
                        exit(0);
                    }
                }
            }
            if (Token[pos_token] != pair<string, int>{"P", 13})
            {
                printf("第 %d 行", row[pos_token]);
                cout << "缺少 \";\" 符号\n";
                exit(0);
            }
            pos_token++;
        }
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "常量定义错误\n";
        exit(0);
    }
}

void Integer()
{
    if (Token[pos_token].first == "C1")
    {
        pos_token++;
        return;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "整数错误\n";
        exit(0);
    }
}

void Char()
{
    if (Token[pos_token].first == "CT")
    {
        pos_token++;
        return;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "字符常量错误\n";
        exit(0);
    }
}
void Compound_Sentence()
{
    if (Token[pos_token] == CONST)
    {
        Const_declaration();
    }
    if (Token[pos_token] == INT || Token[pos_token] == CHAR)
    {
        Variable_Define();
    }
    Statement_List();
}

void Expression()
{
    if(str[pos_token] == "-")pos_token++;
    if(str[pos_token - 1] == "=" && Token[pos_token] == pair<string,int>{"P",13})
    {
        printf("第 %d 行", row[pos_token]);
        cout << "缺少表达式\n";
        exit(0);
    }
    if(Token[pos_token] == pair<string,int>{"P",13})return;
    int sum1 = 0 , sum2 = 0;
    int pos = 0;
    for (pos_token; Token[pos_token] != pair<string, int>{"P", 11} && Token[pos_token] != pair<string, int>{"P", 12} && Token[pos_token] != pair<string, int>{"P", 15} && Token[pos_token] != pair<string, int>{"P", 16} && Token[pos_token] != pair<string, int>{"P", 13} &&Token[pos_token]!=pair<string, int>{"]", 18} && Token[pos_token].first != "K"; pos_token++)
    //一直遍历 直到 遍历到 < > == !=
    {

        if(str[pos_token] == "++" || str[pos_token] == "--")
        {
            
        }
        string function_num = str[pos_token];

        if (Token_F[function_num])
        {
            pos_token++;
            if (Token[pos_token] == pair<string, int>{"P", 3})
            {
                pos_token++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "函数缺少 (\n";
                exit(0);
            }

            int n = Function_parameter();
            if (n != Token_F_parameter[function_num])
            {
                printf("第 %d 行", row[pos_token]);
                cout << "函数参数数目错误\n";
                exit(0);
            }
            if (Token[pos_token] != pair<string, int>{"P", 4})
            {
                printf("第 %d 行", row[pos_token]);
                cout << "函数缺少 )\n";
                exit(0);
            }
            pos++;
            continue;
        }

        if(sum1 < 0|| sum2 < 0)
        {
                return ;
        }

        if (Token[pos_token] == pair<string, int>{"P", 3}  ) //左括号(
        {
            sum1++;
            continue;
        }
        if (Token[pos_token] == pair<string, int>{"P", 4} ) //右括号)
        {
            sum1--;
            continue;
        }
        if(Token[pos_token] == pair<string , int >{"P" , 17})
        {
            sum2++;
            pos = 0;
            continue;
        }
        if(Token[pos_token] == pair<string,int>{"P",18})
        {
            sum2--;
            pos = 1;
            continue;
        }
        if (pos % 2 == 0)
        {
            if (Token[pos_token].first == "C1" || Token[pos_token].first == "I" ||Token[pos_token].first == "CT" || Token[pos_token].first == "ST")
            {
                pos++; //此处应该是 算术部分
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "算术表达式错误";
                exit(0);
            }
        }
        else if (pos % 2 == 1)
        {
            pair<string, int> temp = Token[pos_token];
            if (temp == pair<string, int>{"P", 1} || temp == pair<string, int>{"P", 2} || temp == pair<string, int>{"P", 5} || temp == pair<string, int>{"P", 6} || temp == pair<string, int>{"P", 7} || temp == pair<string, int>{"P", 8} || temp == pair<string, int>{"P", 9} || temp == pair<string, int>{"P", 10} || temp == pair<string, int>{"P", 21} || temp == pair<string, int>{"P", 22} || temp == pair<string, int>{"P", 23} || temp == pair<string, int>{"P", 24} || temp == pair<string, int>{"P", 25} || temp == pair<string, int>{"P", 26} || temp == pair<string, int>{"P", 27})
            {
                pos++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "算术表达式错误";
                exit(0);
            }
        }
    }

    if (pos % 2 == 0)
    {
        printf("第 %d 行", row[pos_token]);
        cout << "算法表达式错误\n";
        exit(0);
    }
    if (pos == 0)
    {
        printf("第 %d 行", row[pos_token]);
        cout << "缺少表达式\n";
        exit(0);
    }
}

void Condition()
{
    Expression();
}
void Conditional_Statements(int &pos)
{
    if (Token[pos_token] == IF || Token[pos_token] == ELIF)
    {
        if (Token[pos_token] == IF)
            pos++;
        if (Token[pos_token] == ELIF || Token[pos_token] == ELSE)
        {
            if (pos == 0)
            {
                printf("第 %d 行", row[pos_token]);
                cout << "缺少if";
                exit(0);
            }
            pos--;
        }
        pos_token++;

        if (Token[pos_token] == pair<string, int>{"P", 3}) // (
        {
            pos_token++;
        }

        Condition();

        if (Token[pos_token] == pair<string, int>{"P", 4}) // )
            pos_token++;
    }
    else if (Token[pos_token] == ELSE)
    {
        pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 15})
            pos_token++;
        Statement();
    }
}

void Assignment_Statement()
{
    if (Token[pos_token].first == "I" || Token[pos_token].first == "C1" || Token[pos_token].first == "CT")
    {
        pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 11}) // =
        {
            pos_token++;
            if (Token_F[str[pos_token]]) //赋值语句是用的函数
            {
                string num_function = str[pos_token];
                pos_token++;
                if (Token[pos_token] == pair<string, int>{"P", 3})
                    pos_token++;
                else
                {
                    cout << "缺少(\n";
                    exit(0);
                }
                int n = Function_parameter(); //得到实际上使用了几个参数
                if (n != Token_F_parameter[num_function] || Token_F[num_function] != 1)
                {
                    printf("第 %d 行", row[pos_token]);
                    cout << "函数参数传递数量不相同 或者 返回值错误\n";
                    cout << num_function << endl;
                    printf("函数实际传递了 %d 个参数， 应该为 %d 个\n", n, Token_F_parameter[num_function]);
                    exit(0);
                }
                if (Token[pos_token] == pair<string, int>{"P", 4})
                {
                    pos_token++;
                }
            }
            else
            {
                Expression();

                if (Token[pos_token] == pair<string, int>{"P", 12})
                {
                    pos_token++;
                    Assignment_Statement();
                }
            }
        }
        else if (Token[pos_token] == pair<string, int>{"P", 17}) //[
        {
            Expression();
            if (Token[pos_token] == pair<string, int>{"P", 11})
            {
                pos_token++;
                Expression();
            }
            else
            {
                cout << "=";
                exit(0);
            }
        }
    }
}

void While_Statement()
{
    if (Token[pos_token] == WHILE)
    {
        pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 3})
            pos_token++;

        Condition();

        if (Token[pos_token] == pair<string, int>{"P", 4})
            pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 15})
            pos_token++ ;
        Statement();

        if (Token[pos_token] == pair<string, int>{"P", 16})
            pos_token++ ;

        Statement();
    }
}

void For_Statement()
{
    if (Token[pos_token] == FOR)
    {
        pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 3})
            pos_token++;
        //第一个冒号
        //没有 声明 赋值

        if (Token[pos_token] != pair<string, int>{"P", 13})
        {
            if (Token[pos_token] == INT || Token[pos_token] == CHAR)
            {
                Variable_declaration();
            }
            else if (Token[pos_token].first == "I")
            {
                Assignment_Statement();
            }
        }

        if (Token[pos_token] == pair<string, int>{"P", 13}) //冒号
            pos_token++;

        if (Token[pos_token] != pair<string, int>{"P", 13})
        {
            Condition();
        }

        if (Token[pos_token] == pair<string, int>{"P", 13}) //冒号
            pos_token++;

        Assignment_Statement(); //赋值语句

        //if 结束
        if (Token[pos_token] == pair<string, int>{"P", 4})
            pos_token++;

        if (Token[pos_token] == pair<string, int>{"P", 15})
            pos_token++;

        Statement();

        if (Token[pos_token] == pair<string, int>{"P", 16})
            pos_token++;

    }
}
void Loop_Statement() //对于while而言
{
    if (Token[pos_token] == WHILE)
    {
        While_Statement();
    }
    else if (Token[pos_token] == FOR)
    {
        For_Statement();
    }
}

int Function_parameter()
{
    int res = 0;
    if(Token[pos_token] == pair<string,int>{"P",4})return 0;
    if (Token[pos_token].first == "I" || Token[pos_token].first == "C1" || Token[pos_token].first == "ST" || Token[pos_token].first == "CT")
    {
        pos_token++;
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << " 函数参数只允许传递 字符 ， 字符串 ， 标识符 ， 和 整数\n";
        exit(0);
    }
    res++;
    if (Token[pos_token] == pair<string, int>{"P", 12})
    {
        pos_token++;
        res += Function_parameter();
    }
    return res;
}

int pos = 0;

int Statement()
{
    int cnt = 0;
    while (Token[pos_token] == IF || Token[pos_token] == ELIF || Token[pos_token] == ELSE) //进入条件语句
    {
        Conditional_Statements(pos);
        if (Token[pos_token] == pair<string, int>{"P", 15})
        {
            pos_token++;
        }
        Statement();
        if (Token[pos_token] == pair<string, int>{"P", 16})
        {
            pos_token++;
        }
        cnt++;
    }
    while (Token[pos_token] == WHILE || Token[pos_token] == FOR)
    {
        Loop_Statement();
        cnt++;
    }

    while ((Token[pos_token] == INT || Token[pos_token] == CHAR) && Token[pos_token + 1].first != "K" && Token_F[str[pos_token + 1]] == 0)
    {
        Variable_declaration();
        cnt++;
    }
    while (Token[pos_token].first == "I" || Token[pos_token].first == "C1" || Token[pos_token].first == "CT")
    {
        cnt++;
        string temp = str[pos_token];
        if (Token_F[temp])
        {
            pos_token++;
            if (Token[pos_token] == pair<string, int>{"P", 3})
            {
                pos_token++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "函数使用错误\n";
                exit(0);
            }

            int res = Function_parameter();
            if (res != Token_F_parameter[temp])
            {
                printf("第 %d 行", row[pos_token]);
                cout << "参数错误\n";
                exit(0);
            }

            if (Token[pos_token] == pair<string, int>{"P", 4})
            {
                pos_token++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "函数使用错误\n";
                exit(0);
            }

            if (Token[pos_token] == pair<string, int>{"P", 13})
            {
                pos_token++;
            }
            else
            {
                printf("第 %d 行", row[pos_token]);
                cout << "缺少 ;";
                exit(0);
            }
            break;
        }
        Assignment_Statement();
        if (Token[pos_token] == pair<string, int>{"P", 13})
        {
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "缺少 ;";
            exit(0);
        }
    }
    if (Token[pos_token] == RETURN)
    {
        Return_Function();
        cnt++;
    }
    return cnt;
}
void Statement_List()
{
    int temp = 1;
    while (Token[pos_token] != pair<string, int>{"P", 16} && temp != 0)
    {
        temp = Statement(); //可以多次重复使用语句函数
    }
}

void Main()
{
    if (Token[pos_token] == pair<string, int>{"P", 3})
    {
        pos_token++;
        if (Token[pos_token] == pair<string, int>{"P", 4})
        {
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "主函数错误\n";
            exit(0);
        }

        if (Token[pos_token] == pair<string, int>{"P", 15})
        {
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "主函数错误\n";
            exit(0);
        }
        Compound_Sentence();

        if (Token[pos_token] == pair<string, int>{"P", 16})
        {
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "主函数错误\n";
            exit(0);
        }
    }
    else
    {
        printf("第 %d 行", row[pos_token]);
        cout << "主函数错误\n";
        exit(0);
    }
}
int num_function_paremeter = 0;
void Parameters_Table()
{
    num_function_paremeter++;
    Types();
    Identifier();
    if (Token[pos_token] == pair<string, int>{"P", 12})
    {
        pos_token++;
        Parameters_Table();
    }
    else
        return;
}
void Return_Function()
{
    if (Token[pos_token] == RETURN)
    {
        pos_token++;
        Expression();
        if (Token[pos_token] == pair<string, int>{"P", 13})
        {
            pos_token++;
            return;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << "函数返回错误\n";
            exit(0);
        }
    }
}
void Function()
{
    while ((Token[pos_token] == VOID || Token[pos_token] == CHAR || Token[pos_token] == BOOL || Token[pos_token] == INT) && Token[pos_token + 1].first == "I" && Token[pos_token + 2] == pair<string, int>{"P", 3} && str[pos_token + 1] != "main")
    {
        pos_token++;
        string function_name;
        if (Token[pos_token].first == "I")
        {
            function_name = str[pos_token];
            pos_token++;
        }
        else
        {
            printf("第 %d 行", row[pos_token]);
            cout << " 函数声明错误\n";
            exit(0);
        }

        //    (
        if (Token[pos_token] == pair<string, int>{"P", 3})
        {
            pos_token++;
        }

        if (Token[pos_token] != pair<string,int>{"P",4})
            Parameters_Table();
        Token_F_parameter[function_name] = num_function_paremeter;
        num_function_paremeter = 0;


        if (Token[pos_token] == pair<string, int>{"P", 4})
        {
            pos_token++;
        }
        else 
        {
            printf("第 %d 行", row[pos_token]);
            cout << "函数缺少 )\n";
            exit(0);
        }
        if (Token[pos_token] == pair<string, int>{"P", 15})
            pos_token++ ;

        Compound_Sentence();

        if (Token[pos_token] == pair<string, int>{"P", 16})
            pos_token++ ;
    }
}

void PROCESS()
{
    while (!(Token[pos_token] == INT && str[pos_token + 1] == "main"))
    {
        if (Token[pos_token] == CONST)
        {
            Const_declaration();
        }
        if ((Token[pos_token] == CHAR || Token[pos_token] == BOOL || Token[pos_token] == INT) && str[pos_token + 1] != "main" && Token[pos_token + 2] != pair<string, int>{"P", 3})
        {
            Variable_declaration();
        }
        if ((Token[pos_token] == VOID || Token[pos_token] == CHAR || Token[pos_token] == BOOL || Token[pos_token] == INT) && Token[pos_token + 1].first == "I" && Token[pos_token + 2] == pair<string, int>{"P", 3} && str[pos_token + 1] != "main")
        {
            Function();
        }
    }
    if (Token[pos_token] == INT && str[pos_token + 1] == "main")
    {
        pos_token += 2;
        Main();
    }
}

void None_Define(int cnt)
{
    for(int i = 0 ; i < cnt ; i ++)
    {
        if(Token[i].first == "I")
        {
            string name = str[i];
            int flag = 0;
            for(int j = 0 ; j <= i ; j ++)
            {
                if(str[j] == name && ( str[j - 1] == "int" || str[j-1] == "void" || str[j-1] == "bool" || str[j-1] == "," || str[j-1] == "char"))
                {
                    flag = 1;
                }
            }
            if(flag == 0)
            {
                printf("第 %d 行", row[i]);
                cout <<"存在未定义\n";
                exit(0);
            }
        }
    }

}
int Parsing()
{
    ios::sync_with_stdio(false);

    get_words();

    cin.clear();
    cout.clear();

    freopen("CON", "w", stdout);
    freopen("out_words.txt", "r", stdin);
    int cnt = 0 , cnt_l = 0 , cnt_r = 0;
    while (cin >> Token[cnt].first >> Token[cnt].second >> str[cnt] >> row[cnt++ + 1])
    {
        if(Token[cnt - 1] == pair<string,int>{"P",15})cnt_l++;
        if(Token[cnt - 1] == pair<string,int>{"P",16})cnt_r++;
        if ((Token[cnt - 3] == INT || Token[cnt - 3] == CHAR) && Token[cnt - 2].first == "I" && Token[cnt - 1] == pair<string, int>{"P", 3})
        {
            Token_F[str[cnt - 2]] = 1; //此处为函数 有返回值
        }
        else if ((Token[cnt - 3] == VOID) && Token[cnt - 2].first == "I" && Token[cnt - 1] == pair<string, int>{"P", 3})
        {
            Token_F[str[cnt - 2]] = 2;
        }

    }
    None_Define(cnt);
    if(cnt_r != cnt_l)
    {
        cout <<"{ }不匹配 \n";
        exit(0);
    }
    PROCESS();

    cout << "YES\n";
    fclose(stdin);
}