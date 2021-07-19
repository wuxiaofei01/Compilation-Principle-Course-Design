#include <bits/stdc++.h>
using namespace std;
int row_words = 0;         //��ǰ token���ǵڼ���
map<string, int> Token_K;  //�ؼ���
map<string, int> Token_P;  //���
map<string, int> Token_I;  //��ʶ��
map<string, int> Token_C1; //����
map<string, int> Token_C2; //��ʵ����
map<string, int> Token_CT; //�ַ�����
map<string, int> Token_ST; //�ַ�������
map<string, int> Token_F;  //����
map<string, int> Token_F_parameter;//�����Ĳ�����
string s;

void init_Token_K()
{
    Token_K.insert(pair<string, int>{"int", 1});
    Token_K.insert(pair<string, int>{"void", 2});
    Token_K.insert(pair<string, int>{"break", 3});
    Token_K.insert(pair<string, int>{"float", 4});
    Token_K.insert(pair<string, int>{"while", 5});
    Token_K.insert(pair<string, int>{"do", 6});
    Token_K.insert(pair<string, int>{"struct", 7});
    Token_K.insert(pair<string, int>{"const", 8});
    Token_K.insert(pair<string, int>{"case", 9});
    Token_K.insert(pair<string, int>{"for", 10});
    Token_K.insert(pair<string, int>{"return", 11});
    Token_K.insert(pair<string, int>{"if", 12});
    Token_K.insert(pair<string, int>{"defalut", 13});
    Token_K.insert(pair<string, int>{"elif", 14});
    Token_K.insert(pair<string, int>{"char", 15});
    Token_K.insert(pair<string, int>{"else", 17});
    Token_K.insert(pair<string, int>{"bool", 18});
}
void init_Token_P()
{
    Token_P.insert(pair<string, int>{"-", 1});
    Token_P.insert(pair<string, int>{"/", 2});
    Token_P.insert(pair<string, int>{"(", 3});
    Token_P.insert(pair<string, int>{")", 4});
    Token_P.insert(pair<string, int>{"==", 5});
    Token_P.insert(pair<string, int>{"<=", 6});
    Token_P.insert(pair<string, int>{"<", 7});
    Token_P.insert(pair<string, int>{"+", 8});
    Token_P.insert(pair<string, int>{"*", 9});
    Token_P.insert(pair<string, int>{">", 10});
    Token_P.insert(pair<string, int>{"=", 11});
    Token_P.insert(pair<string, int>{",", 12});
    Token_P.insert(pair<string, int>{";", 13});
    Token_P.insert(pair<string, int>{"++", 14});
    Token_P.insert(pair<string, int>{"{", 15});
    Token_P.insert(pair<string, int>{"}", 16});
    Token_P.insert(pair<string, int>{"[", 17});
    Token_P.insert(pair<string, int>{"]", 18});
    Token_P.insert(pair<string, int>{"\'", 19});
    Token_P.insert(pair<string, int>{"\"", 20});
    Token_P.insert(pair<string, int>{">=", 21});
    Token_P.insert(pair<string, int>{">>", 22});
    Token_P.insert(pair<string, int>{"<<", 23});
    Token_P.insert(pair<string, int>{"%", 24});
    Token_P.insert(pair<string, int>{"!=", 25});
    Token_P.insert(pair<string, int>{"&&", 26});
    Token_P.insert(pair<string, int>{"||", 27});
}
void init()
{
    init_Token_K();
    init_Token_P();
}
// map<string, int> Token_K;//�ؼ���
// map<string, int> Token_P;//���
// map<string, int> Token_I;//��ʶ��
// map<string, int> Token_C1;//����
// map<string, int> Token_C2;//��ʵ����
// map<string, int> Token_CT;//�ַ�����
// map<string, int> Token_ST;//�ַ�������
void Token_out(string cha, int p, string str)
{
    cout << cha << " " << p << " " << str << " " << row_words << endl;
}

void Fun_K(int &pos)
{
    if (!isalpha(s[pos]))
        return;
    string temp;
    for (int i = pos; i <= s.size(); i++)
    {
        if (!isalpha(s[i])) //������ĸ ��ô ���� �ָ���
        {
            if (Token_K[temp] != 0)
            {
                pos = i;
                Token_out("K", Token_K[temp], temp);
            }
            return;
        }
        temp += s[i];
    }
}

void Fun_I(int &pos)
{
    if (!isalpha(s[pos]) && s[pos] != '_')
        return;
    string temp;
    for (int i = pos; i <= s.size(); i++)
    {
        if (!isalnum(s[i]) && s[i] != '_') //��ʶ�� ֻ���� ������ĸ �»���
        {
            if (!Token_I[temp]) //������
                //Token_I.insert(pair<string , int >{temp , Token_I.size() + 1 });
                Token_I[temp] = Token_I.size() + 1;
            Token_out("I", Token_I.size(), temp);
            pos = i;
            return;
        }
        temp += s[i];
    }
}

void Fun_C1(int &pos)
{
    string temp;
    for (int i = pos; i <= s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            if (s[i] == '.' || s[i] == 'e')
                return;             //Ӧ����ʵ��
            else if (isalpha(s[i])) //��ʶ���������
            {
                cout << "�ʷ�����";
                exit(0);
            }
            else
            {
                if (!Token_C1[temp])
                    //Token_C1.insert(pair<string,int>{temp , Token_C1.size() + 1});
                    Token_C1[temp] = Token_C1.size() + 1;
                Token_out("C1", Token_C1.size(), temp);
                pos = i;
                return;
            }
        }
        temp += s[i];
    }
}

void Fun_C2(int &pos)
{
    if (pos == s.size() || s[pos] == ' ' || !isdigit(s[pos]))
        return;
    string temp;
    int flag1 = 0, flag2 = 0, flag3 = 0, flag4 = 0; //��¼ . e - +
    for (int i = pos; i <= s.size(); i++)
    {
        if (s[i] == '+' || s[i] == '-')
            if (s[i - 1] != 'e') //��Ҫ�˳� ���� Ҳ��Ҫ�ж��ǲ��ǺϷ��ķ��� ���� 0.5
            {
                if (flag1 == 1)
                {
                    if (!Token_C2[to_string(atof(temp.c_str()))])
                        //Token_C2.insert(pair<string,int>{to_string(atof(temp.c_str())),Token_C2.size() + 1});
                        Token_C2[to_string(atof(temp.c_str()))] = Token_C2.size() + 1;

                    Token_out("C2", Token_C2.size(), temp);
                    pos = i;
                }
                return;
            }
        if (!isdigit(s[i]) && s[i] != '.' && s[i] != 'e' && s[i] != '-' && s[i] != '+')
        {
            if (flag1 <= 1 && flag2 <= 1 && flag3 <= 1 && flag4 <= 1)
            {
                if (!Token_C2[to_string(atof(temp.c_str()))])
                    //Token_C2.insert(pair<string,int>{to_string(atof(temp.c_str())),Token_C2.size() + 1});
                    Token_C2[to_string(atof(temp.c_str()))] = Token_C2.size() + 1;
                Token_out("C2", Token_C2.size(), temp);
                pos = i;
                return;
            }
            else
            {
                cout << "�ʷ�����";
                exit(0);
            }
        }
        if (s[i] == '.')
            flag1++;
        if (s[i] == 'e')
            flag2++;
        if (s[i] == '-')
            flag3++;
        if (s[i] == '+')
            flag4++;
        temp += s[i];
    }
}
void Fun_CT(int &pos)
{
    if (s[pos] != '\'')
        return;
    string temp;
    for (int i = pos + 1; i <= s.size(); i++)
    {
        if (s[i] == '\'')
        {
            if (Token_CT[temp])
                //Token_CT.insert(pair<string,int>{temp,Token_CT.size() + 1});
                Token_CT[temp] = Token_CT.size() + 1;
            Token_out("CT", Token_CT.size(), temp);
            i++;
            pos = i;
            return;
        }
        temp += s[i];
    }
}
void Fun_ST(int &pos)
{
    if (s[pos] != '\"')
        return;
    string temp;
    for (int i = pos + 1; i <= s.size(); i++)
    {
        if (s[i] == '\"')
        {
            if (Token_ST[temp])
                //Token_ST.insert(pair<string,int>{temp,Token_ST.size() + 1});
                Token_ST[temp] = Token_ST.size() + 1;
            Token_out("ST", Token_ST.size(), temp);
            i++;
            pos = i;
            return;
        }
        temp += s[i];
    }
}
void Fun_P(int &pos)
{
    string temp;
    for (int i = pos; i <= s.size(); i++)
    {
        if (Token_P[temp])
        {
            string temp1 = temp;
            temp1 += s[i];
            if (Token_P[temp1])
            {
                Token_out("P", Token_P[temp1], temp1);
                pos = i + 1;
                return;
            }

            Token_out("P", Token_P[temp], temp);
            pos = i;
            return;
        }
        temp += s[i];
    }
}
void Check(int i)
{
    if (!isdigit(s[i]))
        return;
    string temp;
    for (int i = i; i <= s.size(); i++)
    {
        if (!isdigit(s[i]))
        {
            if (s[i] == '.' || s[i] == 'e')
                return;             //Ӧ����ʵ��
            else if (isalpha(s[i])) //��ʶ���������
            {
                cout << "�ʷ�����";
                exit(0);
            }
            else
                return;
        }
        temp += s[i];
    }
}

int get_words()
{
    freopen("test.txt", "r", stdin);
    freopen("out_words.txt", "w", stdout);
    init();
    while (getline(cin, s))
    {
        row_words++;
        int i = 0;
        Check(i);
        for (int i = 0; i < s.size();)
        {
            if (s[i] == ' ')
            {
                i++;
                continue;
            }
            if (isalpha(s[i])) //�����һ���ַ��� ��ĸ ��ô ��ֻ���� �ؼ��� ���� ��ʶ��
            {
                Fun_K(i);
                Fun_I(i);
            }
            else if (isdigit(s[i])) //����� ���� ��ô�� �� ��������ʵ��
            {
                Fun_C1(i);
                Fun_C2(i);
            }
            else if (s[i] == '\'') //�ַ�����
            {
                Fun_CT(i);
            }
            else if (s[i] == '\"') //�ַ�������
            {
                Fun_ST(i);
            }
            else
            {
                Fun_P(i);
            }
        }
    }
    fclose(stdin);
    fclose(stdout);

    return 0;
}