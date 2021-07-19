#include"parsing.cpp"

using namespace std;

stack< string > HS;//全局层次栈
stack< string > EXS;//全局表达式栈

//函数声明
string expressionStack(ofstream& out,vector<string> data);

//运算符表
string Opera[20] = { "++","--","!","~","*","/","%","+","-" ,"<<",">>","=","==","!=",">","<","<=",">=","&&","||"};

//全局变量
bool isconst = false;//const状态标志
bool isCT = false;//标记是否为字符
bool isST = false;//标记是否为字符串
int num_t = 0;//四元式分配t的次数
bool isarray = false;//标记数组

//通用函数
string intos()//完成标号的组合并返回，形成t0，t1等标号
{
    string intos  = "t" + to_string(num_t-1);
    return intos;
}

void printdata(vector<string> data)//输出一个vector
{
    for (unsigned int i = 0; i < data.size(); i++)
    {
        cout << data[i] << " ";
    }
    cout << endl;
}

void printtuple(vector<tuple<string, int, bool>> data)
{
    for (unsigned int i = 0; i < data.size(); i++)
    {
        cout << get<0>(data[i]) << " " << get<1>(data[i]) << " " << get<2>(data[i]) << endl;
    }
}

void getlinefromtxt(vector<string> &data,string filename)//读txt文件，并返回一行内容
{
    ifstream txt(filename);//读取token文件     "example.txt"
    string line;
    if (txt) // 有该文件
        while (getline(txt, line))
        {
            data.push_back(line);
        }
    else // 没有该文件
        cout << "no such file" << endl;
}

void Data_parsing(string data, vector<string> &pardata,int lenth=3)//解析文件读取的一行数据
{
    pardata.clear();
    int position;
    while(1)//按照空格分割字符串
    {
        position = data.find(' ');
        pardata.push_back(data.substr(0, position));//将分割的前一部分放入vector中
        data = data.substr(position + 1);//将字符串变成后一部分
        if (position == -1) break;
    }
    if(pardata.end()- pardata.begin() - lenth>0)
        pardata.erase(pardata.begin()+ lenth,pardata.end());
}

bool isdeclar(string name)//判断是否是声明关键字
{
    if (name != "int" && name != "bool" && name != "char" && name != "const" && name != "void")//加一个void
        return false;
    return true;
}

bool isoperator(string p)//判断是否是运算符
{
    for(int i=0;i<20;i++)//循环查表，查到返回true
        if (p == Opera[i])
            return true;
    return false;
}

void isFileExist(const char* filename)//判断输出文件是否存在，如果存在则删除重新新建，如果不存在，则直接新建
{
    fstream fs;
    fs.open(filename, ios::in);

    if (fs)//存在
    {
        remove(filename);//删除文件
    }
    //创建文件
    ofstream fout(filename);
    if (fout)
    {
        fout.close();
    }
}

void outfile(ofstream &out,string data1 = "_", string data2="_", string data3 = "_", string data4 = "_")//按行控制格式输出内容给txt
{
     out << data1 << " " << data2 << " " << data3 << " " << data4 << "\n";
     out.clear();
    // cout << data1 << " " << data2 << " " << data3 << " " << data4 << "\n";
}

bool haveParentheses(vector<string> data)//判断表达式是否含有括号
{
    for (unsigned int i = 0; i < data.size(); i++)
    {
        if (data[i] == "(")
            return true;
    }
    return false;
}

string expressionStack(ofstream& out,vector<string> data)//输入一个复杂表达式，通过压栈弹栈的方式生成四元式
{
    //printdata(data);
    if (data.size() == 3 && (isCT || isconst || isST))//当栈顶为等号时，直接赋值，直接处理，例如a=b或a='b'
    {
        string ct;
        if (isCT) ct = "\'" + data[2] + "\'", isCT = 0;//加上引号
        else if(isST) ct = "\"" + data[2] + "\"", isST = 0;//加上双引号
        else ct = data[2];
        outfile(out,HS.top(), ct, "_", data[0]);
        return "end";//直接结束
    }

    stack< string > s;//负责把栈反过来的数据正向弹出来
    vector<string> t;//负责把没有括号的表达式即简单表达式保存起来

    if (!haveParentheses(data))//判断是否有括号，没有括号直接处理
    {
        //~取反 ,单目运算符
        for (unsigned int i = 0; i < data.size(); i++)//循环的目的：找到 ~ 的位置
        {
            if (data[i] == "++" || data[i] == "--" || data[i] == "!" || data[i] == "~" || (i==0 && (data[i] == "+" || data[i] == "-")))
            {
                num_t++;//t的数量加一
                outfile(out,data[i], "0", data[i + 1], intos());//写入一个四元组
                data[i] = intos();//将表达式更改为新表达式，将a*b->t
                data.erase(data.begin() + i);//删除一个多余的元素
                string ret = expressionStack(out,data);//将新表达式再放入函数中处理，直到整个表达式没有单目运算符时返回
                return ret;//返回则直接跳出
            }
        }
        //先解决* / %运算，再解决+ -运算，按照符号优先级
        for (unsigned int i = 0; i < data.size(); i++)//循环的目的：找到* / %的位置，分成两个循环的目的：先处理完* / %
        {
            if (data[i] == "*" || data[i] == "/" || data[i] == "%")
            {
                num_t++;//t的数量加一
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//写入一个四元组
                data[i - 1] = intos();//将表达式更改为新表达式，将a*b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//删除两个多余的元素
                string ret = expressionStack(out,data);//将新表达式再放入函数中处理，直到整个表达式没有* / %时返回
                return ret;//返回则直接跳出
            }
        }
        //处理完优先级高的符号后处理+ -
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "+" || data[i] == "-")//循环找到第一个+，-
            {
                num_t++;
                outfile(out,data[i], data[i - 1], data[i + 1], intos());
                data[i - 1] = intos();
                data.erase(data.begin() + i, data.begin() + i + 2);//删除两个多余元素，a+b->t
                string ret = expressionStack(out,data);//把新的表达式递归继续处理，直到没有+ -
                return ret;//变成t后跳出
            }
        }
        //>> <<
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == ">>" || data[i] == "<<")
            {
                num_t++;//t的数量加一
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//写入一个四元组
                data[i - 1] = intos();//将表达式更改为新表达式，将a>>b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//删除两个多余的元素
                string ret = expressionStack(out,data);//将新表达式再放入函数中处理，直到整个表达式没有 <<  >> 时返回
                return ret;//返回则直接跳出
            }
        }
        //= == > < >= <= !=
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "=" || data[i] == "==" || data[i] == "<" || data[i] == ">" || data[i] == "<=" || data[i] == ">=" || data[i] == "!=")
            {
                if (data[i] == "=") outfile(out,data[i], data[i + 1], "_", data[i - 1]);//写入一个四元组
                else num_t++,outfile(out,data[i], data[i - 1], data[i + 1], intos());//写入一个四元组,//用到intos（），t的数量加一
                data[i - 1] = intos();//将表达式更改为新表达式，将a*b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//删除两个多余的元素
                string ret = expressionStack(out,data);//将新表达式再放入函数中处理，直到整个表达式没有上述符号时返回
                return ret;//返回则直接跳出
            }
        }
        //&&  ||
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "&&" || data[i] == "||")
            {
                num_t++;//t的数量加一
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//写入一个四元组
                data[i - 1] = intos();//将表达式更改为新表达式，将a||b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//删除两个多余的元素
                string ret = expressionStack(out,data);//将新表达式再放入函数中处理，直到整个表达式没有 && || 时返回
                return ret;//返回则直接跳出
            }
        }
    }
    else
    {
        //如果有括号，先去括号
        for (unsigned int i = 0; i <= data.size()+1; i++)
        {
            //cout << "!!!" << endl;
            if (i == data.size()+1 || ( i < data.size() && data[i] == ")"))//等于“）”时处理，或者循环到字符串尾部时处理
            {
                //cout << "!!!" << endl;
                i++;//往后移一位，防止压栈压入  “）”
                t.clear();//清空t组
                while (!EXS.empty() && EXS.top() != "(")//弹栈，弹到“（”
                {
                    t.push_back(EXS.top());
                    EXS.pop();
                }
                if(!EXS.empty()) EXS.pop();//弹出一个"("括号
                reverse(t.begin(), t.end());//倒置
                //printdata(t);
                string ret = expressionStack(out,t);//弹出部分的简单表达式递归处理
                //cout << "??????" << ret << " " << i << " " << data.size() << endl;
                EXS.push(ret);//处理结束，将符号t_n压栈
            }
            if (i < data.size())  EXS.push(data[i]);//不等于  “）”  则压栈,字符串结束不压栈
        }
    }
    return intos();
}

void outendl(ofstream &out,string str)
{
    vector<string> t;
    Data_parsing(str, t);
    if (t[2] != "while") out << endl;  //如果下一个四元式是while，for，if，则不输出换行
    HS.pop();
}

bool isC1(string str)//判断一个字符串是不是常数
{
    for (unsigned int i = 0; i < str.length(); i++)
    {
        if (str[i] < '0' || str[i] > '9') return false;
    }
    return true;
}

bool myfind(vector<tuple<string, int, bool>> asstable,string str,int &value,bool &use)
{
    for (unsigned int i = 0; i < asstable.size(); i++)
    {
        if (get<0>(asstable[i]) == str)
        {
            value = get<1>(asstable[i]);
            use = get<2>(asstable[i]);
            asstable[i] = make_tuple(get<0>(asstable[i]), get<1>(asstable[i]),true);
            return true;
        }
    }
    return false;
}

void outalltofile(ofstream &out, vector<string> data)
{
    for (unsigned int i = 0; i < data.size(); i++)
    {
        out << data[i] << endl;
    }
    out.close();
}

//代码优化
void CM()//Constant_merging，常数合并
{
    vector<string> linedata;
    vector<string> data;
    vector<tuple<string, int, bool>> asstable;
    const char* filename = "quaternion_CM.txt";//输出文件名
    ofstream out(filename);//文件输入变量，负责把四元式输入到输出文件中

    isFileExist(filename);//判断输出文件是否存在，并新建
    getlinefromtxt(linedata, "quaternion.txt");//按行将文本内容导入linedata中
    for (unsigned int linei = 0; linei < linedata.size(); linei++)
    {
        Data_parsing(linedata[linei], data,4);//解析该行数据
        //数据处理
        if (data[0] == "for" || data[0] == "while")//如果是循环则跳过循环
        {
            int fornum=1;//记录循环层次
            while (1)
            {
                linei++;
                vector<string> t;
                Data_parsing(linedata[linei], t, 4);//解析该行数据
                //cout << t[0] << endl;
                if (t[0] == "for" || t[0] == "while") fornum++;
                if (t[0] == "fe" || t[0] == "we") fornum--;
                if (!fornum) break;
            }
        }
        else if (data[0] == "=" && isC1(data[1]))//当该四元式为赋值语句时，并且赋值的数为常数时
        {
            tuple<string, int, bool> t(data[3], atoi(data[1].c_str()), false);
            asstable.push_back(t);
        }
        else if (isoperator(data[0]))
        {
            if (isC1(data[1]) && isC1(data[2]))
            {
                string result;
                if(data[0]=="+")
                    result = to_string(atoi(data[1].c_str()) + atoi(data[2].c_str()));
                else if(data[0] == "-")
                    result = to_string(atoi(data[1].c_str()) - atoi(data[2].c_str()));
                else if (data[0] == "*")
                    result = to_string(atoi(data[1].c_str()) * atoi(data[2].c_str()));
                else if (data[0] == "/")
                    result = to_string(atoi(data[1].c_str()) / atoi(data[2].c_str()));
                else if (data[0] == "%")
                    result = to_string(atoi(data[1].c_str()) % atoi(data[2].c_str()));

                linedata.erase(linedata.begin()+linei);
                int first = linedata[linei].find_first_of(data[3]);
                linedata[linei].replace(first, 2, result);
            }
            else if (isC1(data[1]) || isC1(data[2]))
            {
                int value;
                bool use;
                if (isC1(data[1]))
                { 
                    if (myfind(asstable, data[2], value,use))
                    {
                        string result;
                        if (data[0] == "+")
                            result = to_string(atoi(data[1].c_str()) + atoi(data[2].c_str()));
                        else if (data[0] == "-")
                            result = to_string(atoi(data[1].c_str()) - atoi(data[2].c_str()));
                        else if (data[0] == "*")
                            result = to_string(atoi(data[1].c_str()) * atoi(data[2].c_str()));
                        else if (data[0] == "/")
                            result = to_string(atoi(data[1].c_str()) / atoi(data[2].c_str()));
                        else if (data[0] == "%")
                            result = to_string(atoi(data[1].c_str()) % atoi(data[2].c_str()));

                        linedata.erase(linedata.begin() + linei);
                        int first = linedata[linei].find_first_of(data[3]);
                        linedata[linei].replace(first, 2, result);
                    }
                }
                else if(isC1(data[2]))
                {
                    if (myfind(asstable, data[1], value, use))
                    {
                        string result;
                        if (data[0] == "+")
                            result = to_string(atoi(data[1].c_str()) + atoi(data[2].c_str()));
                        else if (data[0] == "-")
                            result = to_string(atoi(data[1].c_str()) - atoi(data[2].c_str()));
                        else if (data[0] == "*")
                            result = to_string(atoi(data[1].c_str()) * atoi(data[2].c_str()));
                        else if (data[0] == "/")
                            result = to_string(atoi(data[1].c_str()) / atoi(data[2].c_str()));
                        else if (data[0] == "%")
                            result = to_string(atoi(data[1].c_str()) % atoi(data[2].c_str()));

                        linedata.erase(linedata.begin() + linei);
                        int first = linedata[linei].find_first_of(data[3]);
                        linedata[linei].replace(first, 2, result);
                    }
                }
            }
            else
            {
                int value1, value2;
                bool use1,use2;
                if (myfind(asstable, data[1], value1, use1) && myfind(asstable, data[2], value2, use2))
                {
                    string result;
                    if (data[0] == "+")
                        result = to_string(atoi(data[1].c_str()) + atoi(data[2].c_str()));
                    else if (data[0] == "-")
                        result = to_string(atoi(data[1].c_str()) - atoi(data[2].c_str()));
                    else if (data[0] == "*")
                        result = to_string(atoi(data[1].c_str()) * atoi(data[2].c_str()));
                    else if (data[0] == "/")
                        result = to_string(atoi(data[1].c_str()) / atoi(data[2].c_str()));
                    else if (data[0] == "%")
                        result = to_string(atoi(data[1].c_str()) % atoi(data[2].c_str()));

                    linedata.erase(linedata.begin() + linei);
                    int first = linedata[linei].find_first_of(data[3]);
                    linedata[linei].replace(first, 2, result);
                }
            }
        }
    }
    outalltofile(out, linedata);
    //printtuple(asstable);
}

void CES()//Common Expression Savings,公共表达式节省
{

}

void DIS()//Delete invalid statements,删除无用语句
{

}

//四元式生成
void Keywords(ofstream &out,string name)//得到一个关键字，判断当前状态并且压栈生成四元式
{
    if(name!="const")//const不作为状态，不压栈
        HS.push(name);
    else
        isconst = true;//单独表示const声明

    if (name == "while")//当关键字为while时直接输出一个（while _ _ _）,因为只有while包住了所有东西
        outfile(out,name), out << endl;
    if (name == "else") outfile(out,name);
}

void createQua()
{   
    vector<string> linedata;
    vector<string> data;//保存解析后的一行数据
    int par_left = 0;//记录左括号数目
    stack<bool> lar_par;//记录大括号层次以及状态
    vector<string> exep;//存储表达式
    vector<string> exep_C; //备份表达式
    string fK="(";//保存参数类型
    string fI="(";//保存参数名
    string left,midel;//存储左右两边表达式化简后的结果,以及中间界符
    int sem_n=0;//记录分号的数目，用于 “for循环”
    string fname;//保存函数名
    string usingfname;//调用函数名
    bool isf=false;//标记函数声明

    const char* filename = "quaternion.txt";//输出文件名
    ofstream out(filename);//文件输入变量，负责把四元式输入到输出文件中

    isFileExist(filename);//判断输出文件是否存在，并新建
    getlinefromtxt(linedata, "out_words.txt");//按行将文本内容导入linedata中
    //逐个解析数据
    for(unsigned int linei=0;linei< linedata.size(); linei++)
    {
        Data_parsing(linedata[linei],data);//解析该行数据
        //数据处理
        if (data[0]=="K") //如果当前为关键字，进入关键字处理函数
        {
            if (!HS.empty() && HS.top() == "f" && isdeclar(data[2]) && isf) fK= fK+data[2]+"|";//如果是函数声明，则保存函数声明的参数
            else Keywords(out,data[2]);

            if (data[2] == "else")//如果是else，判断其后是否有大括号
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);
                if (t[2] == "{") lar_par.push(true);
                else lar_par.push(false);
            }
        }
        else if (data[0] == "I")
        {
            vector<string> t;
            Data_parsing(linedata[linei + 1], t);
            if (t[2] == "(")
            {
                if (isdeclar(HS.top()))//该行 为函数声明，把"f"压栈，形成新的状态, , 并且保存函数名
                {
                    isf = true;
                    HS.push("f");
                    fname = data[2];
                }
                else //该处为函数调用，压栈
                {
                    usingfname = data[2];
                    HS.push("=f");
                    fI = "(";
                }
            }
            else if (t[2] == ".")
            {
                //isstruct = true;
                string p = t[2];
                Data_parsing(linedata[linei + 2], t);//获取点后一位的内容
                left = data[2] + p + t[2];//将a.b合并，并且保存
                exep.push_back(left);//压入exep
                linei += 2;//往后移两位跳过不需要的部分
            }
            else if (!HS.empty() && isdeclar(HS.top()))//如果当前状态栈栈顶为声明，标明现在为声明状态
            {
                vector<string> t;
                Data_parsing(linedata[linei+1], t);//获取后一行数据内容
                if (t[2] != "]")
                {
                    if (t[2] != "[")//不等于中括号时，为普通变量
                        outfile(out,HS.top(), data[2]);//生成四元式（类型 标识符 _ _)
                    else//否则为数组，字符组等
                    {
                        Data_parsing(linedata[linei + 2], t);//获取数组的长度，----------------声明一个数组
                        string myarray = "(" + data[2] + "|" + t[2] + ")";
                        outfile(out,HS.top(), myarray);//生成四元式（类型 标识符 长度 _)
                    }
                }
            }
            else if (!HS.empty() && HS.top() == "=" && isconst == false)//赋值状态，把后面的标识符存起来
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);//获取后一行数据内容
                if (t[2] != "[")//当标识符后不为[时，判定为普通变量
                    exep.push_back(data[2]);
                else
                {
                    Data_parsing(linedata[linei + 3], t);
                    if (t[2] == "]")
                    {
                        isarray = true;
                        left = data[2];
                        Data_parsing(linedata[linei + 2], t);//获取数组的长度，----------------保存一个数组到表达式中
                        midel = t[2];
                        //string myarray = "(" + data[2] + "|" + t[2] + ")";
                        //exep.push_back(myarray);
                    }
                    else left = data[2];
                }
            }
            else if (!HS.empty() && HS.top() == "[")
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);//获取后一行数据内容
                if (t[2] != "[")//当标识符后不为[时，判定为普通变量
                    exep.push_back(data[2]);
                else
                {
                    isarray = true;
                    left = data[2];
                    Data_parsing(linedata[linei + 2], t);//获取数组的长度，----------------保存一个数组到表达式中
                    midel = t[2];
                    //string myarray = "(" + data[2] + "|" + t[2] + ")";
                    //exep.push_back(myarray);
                }
            }
            else if (!HS.empty() && (HS.top() == "while" || sem_n == 1 || HS.top() == "if" || HS.top() == "elif" || HS.top() == "return"))//while状态，for的第一个；后，把后面的标识符存起来（while和for第一个分号后都是比较表达式）
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);//获取后一行数据内容
                if (t[2] != "[")//当标识符后不为[时，判定为普通变量
                    exep.push_back(data[2]);
                else
                {
                    Data_parsing(linedata[linei + 3], t);
                    if (t[2] == "]")
                    {
                        //cout << data[2] << "?????" << endl;
                        isarray = true;
                        left = data[2];
                        Data_parsing(linedata[linei + 2], t);//获取数组的长度，----------------保存一个数组到表达式中
                        midel = t[2];
                        //string myarray = "(" + data[2] + "|" + t[2] + ")";
                        //cout << myarray << endl;
                        //exep.push_back(myarray);
                    }
                    else left = data[2];
                }
            }
            else if (!HS.empty() && (HS.top() == "f" || HS.top() == "=f")) fI = fI + data[2] + "|";
        } 
        else if (data[0] == "CT" || data[0] == "C1" || data[0] == "ST")
        {
        if (HS.top() == "=f") fI = fI + data[2] + "|";

        if (data[0] == "CT") isCT = 1;
        if (data[0] == "ST") isST = 1;
        if (data[0] == "C1" && isarray)
        {
            isarray = false;
        }
        else
        {
            if (!HS.empty())
                if (HS.top() == "=" || HS.top() == "while" || sem_n == 1 || HS.top() == "if" || HS.top() == "elif" || HS.top() == "[" || HS.top() == "return")
                    if (data[0] == "CT")
                    {
                        isCT = 0;
                        exep.push_back("\'"+data[2]+ "\'");
                    }
                    else if(data[0] == "ST")
                    {
                        isST = 0;
                        exep.push_back("\"" + data[2] + "\"");
                    }
                    else
                        exep.push_back(data[2]);
        }
         }
        else if ((data[2] == "++" || data[2] == "--") && !(HS.top()=="while" || HS.top() == "=" || HS.top() == "if" || HS.top() == "elif" || sem_n==1))
        {
            vector<string> t;
            Data_parsing(linedata[linei + 1], t);
            if (t[0] != "I")
                if (linei > 1) Data_parsing(linedata[linei - 1], t);
            num_t++;
            outfile(out,data[2], "0", t[2], intos());
        }
        else if (isoperator(data[2]) || data[2] == "(" || data[2] == ")")
        {
            if (!HS.empty() && (HS.top() == "while" || HS.top() == "if" || HS.top() == "elif"))//根据括号判断压栈或者化简表达式  || HS.top()=="for"
            {
                if (data[2] == "(")
                    par_left++;
                if (data[2] == ")")
                    par_left--;
                if (!par_left)
                {
                    vector<string> t;
                    Data_parsing(linedata[linei + 1], t);
                    if (t[2] == "{") lar_par.push(true);
                    else lar_par.push(false);

                    if (HS.top() == "while" || HS.top() == "if" || HS.top() == "elif")
                    {
                        string ret;
                        if (exep.size() > 3)
                        {
                            ret=expressionStack(out,exep);
                        }  
                        else
                        {
                            ret = exep[1];
                            num_t++;//将单一判断数补为表达式
                            outfile(out,"!=", ret, "0", intos());
                            ret = intos();
                        }
                        exep.clear();
                        if (HS.top() == "while") outfile(out,"do", ret);
                        else outfile(out,HS.top(), ret);
                        HS.push("do");
                    }
                }
                else
                    if(HS.top() != "for") exep.push_back(data[2]);
            }
            else if (!HS.empty() && HS.top() == "for" && sem_n == 1)
            {
                exep.push_back(data[2]);
            }
            else if (!HS.empty() && data[2] == "=" && HS.top() != "=")
            {
                HS.push(data[2]);//栈顶变为“=”
                vector<string> t;
                Data_parsing(linedata[linei - 1], t);//获取前一行数据内容
                if (t[2] != "]")//当标识符后不为]时，判定为普通变量
                {
                    if (exep.size()==0)//两种情况，一种为普通标识符，一种为结构体变量，结构体变量已经提前存入exep，所以size不为0
                    {
                        exep.push_back(t[2]);
                        left = t[2];
                    }
                    //else exep.push_back(left),isstruct=false;
                } 
                else
                {
                    Data_parsing(linedata[linei - 3], t);
                    if (t[2] != "[")
                    {
                        string myarray = "(" + left + "|" + midel + ")";
                        left = myarray;
                        exep.push_back(myarray);
                    }
                    else
                    {
                        Data_parsing(linedata[linei - 2], t);//获取数组的长度，----------------保存一个数组到表达式中
                        vector<string> aname;
                        Data_parsing(linedata[linei - 4], aname);
                        string myarray = "(" + aname[2] + "|" + t[2] + ")";
                        left = myarray;
                        exep.push_back(myarray);
                    }
                }
                Data_parsing(linedata[linei + 2], t);//判断是否为带返回值函数调用
                exep.push_back(HS.top());//将“=”压入表达式栈
                //if (t[2] == "(") HS.pop(),exep.clear(), HS.push("=f"),fI="(",isret=true;//弹掉=，压入=f,清空表达式栈,函数参数表清空,标明需要返回值
                //else exep.push_back(HS.top());//将“=”压入表达式栈
            }
            else if (!HS.empty() && HS.top() == "=" && isconst == false) exep.push_back(data[2]);
            else if (!HS.empty() && HS.top() == "[") exep.push_back(data[2]);
            else if (!HS.empty() && data[2] == ")" && (HS.top() == "f" || HS.top() == "=f"))
            {
                if (HS.top() == "f")
                {
                    isf = false;
                    fK[fK.length() - 1] = ')';
                    fI[fI.length() - 1] = ')';
                    if(fI.length() > 1) outfile(out,"declarP", fname, fK, fI);
                    fK = "(";
                    fI = "(";
                }
                else
                {
                    fI[fI.length() - 1] = ')';
                    if (fI.length() > 1) outfile(out,"call", usingfname, "_", fI);
                    else outfile(out,"call", usingfname, "_", "_");
                    fI = "(";
                    HS.pop();
                    //outfile("f", fname + "_ret", "_", left);
                    if(HS.top()=="=" || HS.top() == "while" || HS.top() == "if" || HS.top() == "elif" || sem_n == 1 || HS.top() == "return")
                        exep.push_back(usingfname + "_ret");
                    //HS.pop();
                    /*if (isret)
                    {
                        isret = false;
                        outfile("f",fname+"_ret","_",left);
                        exep.push_back(fname+"_ret");
                        HS.pop();
                    }*/
                }
            }
            else if(!HS.empty() && HS.top() == "return") exep.push_back(data[2]);
        }
        else if (data[2] == ";")
        {
            if (!HS.empty() && HS.top() == "=")//= 在“；”后状态结束，弹出
            {
                expressionStack(out,exep);
                exep.clear();
                HS.pop();
            }

            if (!HS.empty() && HS.top() == "struct")
            {
                vector<string> t;
                Data_parsing(linedata[linei - 1], t);
                if (t[2] == "}")
                {
                    outfile(out,HS.top(), "end");
                    HS.pop();
                }
            }

            if (!HS.empty() && isdeclar(HS.top())) HS.pop();//声明在“；”后弹出
            if (isconst) isconst = 0;//const在“；”后改变状态
            if (!HS.empty() && HS.top() == "return")
            {
                string ret;
                if (exep.size() > 1)
                {
                    //printdata(exep);
                    ret = expressionStack(out,exep);
                }
                else if (exep.size() == 1)
                {
                    ret = exep[0];
                }
                else
                {
                    ret = "_";
                }
                exep.clear();
                outfile(out,HS.top(), ret);
                HS.pop();
            }

            if (!lar_par.empty() && !HS.empty() && !lar_par.top() && !isdeclar(HS.top()))//没有大括号的for，while，if，elif，else
            {
                if (HS.top() == "do") HS.pop();

                if (HS.top() == "while")
                {
                    lar_par.pop();
                    outfile(out,"we");
                    outendl(out,linedata[linei + 1]);
                }

                if (!HS.empty() && (HS.top() == "if" || HS.top() == "else" || HS.top() == "elif"))
                {
                    lar_par.pop();
                    if (HS.top() == "if") outfile(out,"ie", "0");
                    else if (HS.top() == "elif") outfile(out,"ie", "1");
                    else if (HS.top() == "else") outfile(out,"ie", "2");
                    
                    outendl(out,linedata[linei + 1]);
                }
            }

            if (!HS.empty() && HS.top() == "for") sem_n++;//记录分号的数目
            if (sem_n == 1) //第一个分号，输出（for _ _ _）
            {
                outfile(out,HS.top());
                out << endl;
            }
            if (sem_n == 2)//第二个分号，化简不等式
            {
                string ret = expressionStack(out,exep);
                exep.clear();
                outfile(out,"do", ret);
                HS.push("do");
                sem_n = 0;
            }
        }
        else if (data[2] == ",")
        {
            if (HS.top() == "=")
            {
                expressionStack(out,exep);
                exep.clear();
                HS.pop();
            }
        }
        else if (data[2] == "}" && !HS.empty())
        {
            if (!HS.empty() && HS.top() == "do") HS.pop();

            if (!HS.empty() && HS.top() == "f")
            {
                outfile(out,HS.top(), "end", fname);
                HS.pop();
            }
            else if (!HS.empty() && HS.top() == "for")
            {
                outfile(out,"fe");
                sem_n = 0;
                outendl(out,linedata[linei + 1]);
            }
            else if (!lar_par.empty() && !HS.empty() && HS.top() == "while" && lar_par.top())
            {
                lar_par.pop();
                outfile(out,"we");
                outendl(out,linedata[linei + 1]);
            }
            else if (!lar_par.empty() && lar_par.top() && !HS.empty() && (HS.top() == "if" || HS.top() == "else" || HS.top() == "elif"))
            {
                lar_par.pop();
                if (HS.top() == "if") outfile(out,"ie", "0");
                if (HS.top() == "elif") outfile(out,"ie", "1");
                if (HS.top() == "else") outfile(out,"ie", "2");

                outendl(out,linedata[linei + 1]);
            }
        }
        else if (data[2] == "{")
        {
            // 如果栈顶是"f"，那么肯定为函数
            if (HS.top()=="f")
            {
                string F = HS.top();
                HS.pop();
                outfile(out,F, fname, HS.top());
                out << endl;
                HS.pop();
                HS.push("f");
            }

            if (HS.top() == "struct")
            {
                vector<string> t;
                Data_parsing(linedata[linei - 1], t);
                outfile(out,HS.top(), t[2]);
            }

            if (HS.top() == "=")//for循环
            {
                expressionStack(out,exep);
                exep.clear();
                HS.pop();
            }
        }
        else if (data[2] == "[")
        {
            exep_C.swap(exep);
            exep.clear();
            vector<string> t;
            Data_parsing(linedata[linei + 2], t);//获取中括号后两位
            if (t[2] != "]") 
            {
                Data_parsing(linedata[linei - 1], t);
                left = t[2];//保存数组名
                HS.push(data[2]);//压“[”入状态栈
            }
        }
        else if (data[2] == "]")
        {
            isarray = false;
            if (HS.top() == "[")
            {
                midel = expressionStack(out,exep);
                exep.clear();
                HS.pop();
            }
            if (HS.top() == "=" || HS.top() == "while" || HS.top() == "if" || HS.top() == "elif" || sem_n == 1)
            {
                exep.swap(exep_C);
                string myarray = "(" + left + "|" + midel + ")";
                exep.push_back(myarray);
            }
        }
        else{}
    }
    out.close();
	return;
}

int Create_Quaternion()
{
    Parsing();
    cin.clear();
    cout.clear();
    createQua();//生成四元式
    //CM();//常值表达式节省
	return 0;
}