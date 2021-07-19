#include"parsing.cpp"

using namespace std;

stack< string > HS;//ȫ�ֲ��ջ
stack< string > EXS;//ȫ�ֱ��ʽջ

//��������
string expressionStack(ofstream& out,vector<string> data);

//�������
string Opera[20] = { "++","--","!","~","*","/","%","+","-" ,"<<",">>","=","==","!=",">","<","<=",">=","&&","||"};

//ȫ�ֱ���
bool isconst = false;//const״̬��־
bool isCT = false;//����Ƿ�Ϊ�ַ�
bool isST = false;//����Ƿ�Ϊ�ַ���
int num_t = 0;//��Ԫʽ����t�Ĵ���
bool isarray = false;//�������

//ͨ�ú���
string intos()//��ɱ�ŵ���ϲ����أ��γ�t0��t1�ȱ��
{
    string intos  = "t" + to_string(num_t-1);
    return intos;
}

void printdata(vector<string> data)//���һ��vector
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

void getlinefromtxt(vector<string> &data,string filename)//��txt�ļ���������һ������
{
    ifstream txt(filename);//��ȡtoken�ļ�     "example.txt"
    string line;
    if (txt) // �и��ļ�
        while (getline(txt, line))
        {
            data.push_back(line);
        }
    else // û�и��ļ�
        cout << "no such file" << endl;
}

void Data_parsing(string data, vector<string> &pardata,int lenth=3)//�����ļ���ȡ��һ������
{
    pardata.clear();
    int position;
    while(1)//���տո�ָ��ַ���
    {
        position = data.find(' ');
        pardata.push_back(data.substr(0, position));//���ָ��ǰһ���ַ���vector��
        data = data.substr(position + 1);//���ַ�����ɺ�һ����
        if (position == -1) break;
    }
    if(pardata.end()- pardata.begin() - lenth>0)
        pardata.erase(pardata.begin()+ lenth,pardata.end());
}

bool isdeclar(string name)//�ж��Ƿ��������ؼ���
{
    if (name != "int" && name != "bool" && name != "char" && name != "const" && name != "void")//��һ��void
        return false;
    return true;
}

bool isoperator(string p)//�ж��Ƿ��������
{
    for(int i=0;i<20;i++)//ѭ������鵽����true
        if (p == Opera[i])
            return true;
    return false;
}

void isFileExist(const char* filename)//�ж�����ļ��Ƿ���ڣ����������ɾ�������½�����������ڣ���ֱ���½�
{
    fstream fs;
    fs.open(filename, ios::in);

    if (fs)//����
    {
        remove(filename);//ɾ���ļ�
    }
    //�����ļ�
    ofstream fout(filename);
    if (fout)
    {
        fout.close();
    }
}

void outfile(ofstream &out,string data1 = "_", string data2="_", string data3 = "_", string data4 = "_")//���п��Ƹ�ʽ������ݸ�txt
{
     out << data1 << " " << data2 << " " << data3 << " " << data4 << "\n";
     out.clear();
    // cout << data1 << " " << data2 << " " << data3 << " " << data4 << "\n";
}

bool haveParentheses(vector<string> data)//�жϱ��ʽ�Ƿ�������
{
    for (unsigned int i = 0; i < data.size(); i++)
    {
        if (data[i] == "(")
            return true;
    }
    return false;
}

string expressionStack(ofstream& out,vector<string> data)//����һ�����ӱ��ʽ��ͨ��ѹջ��ջ�ķ�ʽ������Ԫʽ
{
    //printdata(data);
    if (data.size() == 3 && (isCT || isconst || isST))//��ջ��Ϊ�Ⱥ�ʱ��ֱ�Ӹ�ֵ��ֱ�Ӵ�������a=b��a='b'
    {
        string ct;
        if (isCT) ct = "\'" + data[2] + "\'", isCT = 0;//��������
        else if(isST) ct = "\"" + data[2] + "\"", isST = 0;//����˫����
        else ct = data[2];
        outfile(out,HS.top(), ct, "_", data[0]);
        return "end";//ֱ�ӽ���
    }

    stack< string > s;//�����ջ���������������򵯳���
    vector<string> t;//�����û�����ŵı��ʽ���򵥱��ʽ��������

    if (!haveParentheses(data))//�ж��Ƿ������ţ�û������ֱ�Ӵ���
    {
        //~ȡ�� ,��Ŀ�����
        for (unsigned int i = 0; i < data.size(); i++)//ѭ����Ŀ�ģ��ҵ� ~ ��λ��
        {
            if (data[i] == "++" || data[i] == "--" || data[i] == "!" || data[i] == "~" || (i==0 && (data[i] == "+" || data[i] == "-")))
            {
                num_t++;//t��������һ
                outfile(out,data[i], "0", data[i + 1], intos());//д��һ����Ԫ��
                data[i] = intos();//�����ʽ����Ϊ�±��ʽ����a*b->t
                data.erase(data.begin() + i);//ɾ��һ�������Ԫ��
                string ret = expressionStack(out,data);//���±��ʽ�ٷ��뺯���д���ֱ���������ʽû�е�Ŀ�����ʱ����
                return ret;//������ֱ������
            }
        }
        //�Ƚ��* / %���㣬�ٽ��+ -���㣬���շ������ȼ�
        for (unsigned int i = 0; i < data.size(); i++)//ѭ����Ŀ�ģ��ҵ�* / %��λ�ã��ֳ�����ѭ����Ŀ�ģ��ȴ�����* / %
        {
            if (data[i] == "*" || data[i] == "/" || data[i] == "%")
            {
                num_t++;//t��������һ
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//д��һ����Ԫ��
                data[i - 1] = intos();//�����ʽ����Ϊ�±��ʽ����a*b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//ɾ�����������Ԫ��
                string ret = expressionStack(out,data);//���±��ʽ�ٷ��뺯���д���ֱ���������ʽû��* / %ʱ����
                return ret;//������ֱ������
            }
        }
        //���������ȼ��ߵķ��ź���+ -
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "+" || data[i] == "-")//ѭ���ҵ���һ��+��-
            {
                num_t++;
                outfile(out,data[i], data[i - 1], data[i + 1], intos());
                data[i - 1] = intos();
                data.erase(data.begin() + i, data.begin() + i + 2);//ɾ����������Ԫ�أ�a+b->t
                string ret = expressionStack(out,data);//���µı��ʽ�ݹ��������ֱ��û��+ -
                return ret;//���t������
            }
        }
        //>> <<
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == ">>" || data[i] == "<<")
            {
                num_t++;//t��������һ
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//д��һ����Ԫ��
                data[i - 1] = intos();//�����ʽ����Ϊ�±��ʽ����a>>b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//ɾ�����������Ԫ��
                string ret = expressionStack(out,data);//���±��ʽ�ٷ��뺯���д���ֱ���������ʽû�� <<  >> ʱ����
                return ret;//������ֱ������
            }
        }
        //= == > < >= <= !=
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "=" || data[i] == "==" || data[i] == "<" || data[i] == ">" || data[i] == "<=" || data[i] == ">=" || data[i] == "!=")
            {
                if (data[i] == "=") outfile(out,data[i], data[i + 1], "_", data[i - 1]);//д��һ����Ԫ��
                else num_t++,outfile(out,data[i], data[i - 1], data[i + 1], intos());//д��һ����Ԫ��,//�õ�intos������t��������һ
                data[i - 1] = intos();//�����ʽ����Ϊ�±��ʽ����a*b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//ɾ�����������Ԫ��
                string ret = expressionStack(out,data);//���±��ʽ�ٷ��뺯���д���ֱ���������ʽû����������ʱ����
                return ret;//������ֱ������
            }
        }
        //&&  ||
        for (unsigned int i = 0; i < data.size(); i++)
        {
            if (data[i] == "&&" || data[i] == "||")
            {
                num_t++;//t��������һ
                outfile(out,data[i], data[i - 1], data[i + 1], intos());//д��һ����Ԫ��
                data[i - 1] = intos();//�����ʽ����Ϊ�±��ʽ����a||b->t
                data.erase(data.begin() + i, data.begin() + i + 2);//ɾ�����������Ԫ��
                string ret = expressionStack(out,data);//���±��ʽ�ٷ��뺯���д���ֱ���������ʽû�� && || ʱ����
                return ret;//������ֱ������
            }
        }
    }
    else
    {
        //��������ţ���ȥ����
        for (unsigned int i = 0; i <= data.size()+1; i++)
        {
            //cout << "!!!" << endl;
            if (i == data.size()+1 || ( i < data.size() && data[i] == ")"))//���ڡ�����ʱ��������ѭ�����ַ���β��ʱ����
            {
                //cout << "!!!" << endl;
                i++;//������һλ����ֹѹջѹ��  ������
                t.clear();//���t��
                while (!EXS.empty() && EXS.top() != "(")//��ջ������������
                {
                    t.push_back(EXS.top());
                    EXS.pop();
                }
                if(!EXS.empty()) EXS.pop();//����һ��"("����
                reverse(t.begin(), t.end());//����
                //printdata(t);
                string ret = expressionStack(out,t);//�������ֵļ򵥱��ʽ�ݹ鴦��
                //cout << "??????" << ret << " " << i << " " << data.size() << endl;
                EXS.push(ret);//���������������t_nѹջ
            }
            if (i < data.size())  EXS.push(data[i]);//������  ������  ��ѹջ,�ַ���������ѹջ
        }
    }
    return intos();
}

void outendl(ofstream &out,string str)
{
    vector<string> t;
    Data_parsing(str, t);
    if (t[2] != "while") out << endl;  //�����һ����Ԫʽ��while��for��if�����������
    HS.pop();
}

bool isC1(string str)//�ж�һ���ַ����ǲ��ǳ���
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

//�����Ż�
void CM()//Constant_merging�������ϲ�
{
    vector<string> linedata;
    vector<string> data;
    vector<tuple<string, int, bool>> asstable;
    const char* filename = "quaternion_CM.txt";//����ļ���
    ofstream out(filename);//�ļ�����������������Ԫʽ���뵽����ļ���

    isFileExist(filename);//�ж�����ļ��Ƿ���ڣ����½�
    getlinefromtxt(linedata, "quaternion.txt");//���н��ı����ݵ���linedata��
    for (unsigned int linei = 0; linei < linedata.size(); linei++)
    {
        Data_parsing(linedata[linei], data,4);//������������
        //���ݴ���
        if (data[0] == "for" || data[0] == "while")//�����ѭ��������ѭ��
        {
            int fornum=1;//��¼ѭ�����
            while (1)
            {
                linei++;
                vector<string> t;
                Data_parsing(linedata[linei], t, 4);//������������
                //cout << t[0] << endl;
                if (t[0] == "for" || t[0] == "while") fornum++;
                if (t[0] == "fe" || t[0] == "we") fornum--;
                if (!fornum) break;
            }
        }
        else if (data[0] == "=" && isC1(data[1]))//������ԪʽΪ��ֵ���ʱ�����Ҹ�ֵ����Ϊ����ʱ
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

void CES()//Common Expression Savings,�������ʽ��ʡ
{

}

void DIS()//Delete invalid statements,ɾ���������
{

}

//��Ԫʽ����
void Keywords(ofstream &out,string name)//�õ�һ���ؼ��֣��жϵ�ǰ״̬����ѹջ������Ԫʽ
{
    if(name!="const")//const����Ϊ״̬����ѹջ
        HS.push(name);
    else
        isconst = true;//������ʾconst����

    if (name == "while")//���ؼ���Ϊwhileʱֱ�����һ����while _ _ _��,��Ϊֻ��while��ס�����ж���
        outfile(out,name), out << endl;
    if (name == "else") outfile(out,name);
}

void createQua()
{   
    vector<string> linedata;
    vector<string> data;//����������һ������
    int par_left = 0;//��¼��������Ŀ
    stack<bool> lar_par;//��¼�����Ų���Լ�״̬
    vector<string> exep;//�洢���ʽ
    vector<string> exep_C; //���ݱ��ʽ
    string fK="(";//�����������
    string fI="(";//���������
    string left,midel;//�洢�������߱��ʽ�����Ľ��,�Լ��м���
    int sem_n=0;//��¼�ֺŵ���Ŀ������ ��forѭ����
    string fname;//���溯����
    string usingfname;//���ú�����
    bool isf=false;//��Ǻ�������

    const char* filename = "quaternion.txt";//����ļ���
    ofstream out(filename);//�ļ�����������������Ԫʽ���뵽����ļ���

    isFileExist(filename);//�ж�����ļ��Ƿ���ڣ����½�
    getlinefromtxt(linedata, "out_words.txt");//���н��ı����ݵ���linedata��
    //�����������
    for(unsigned int linei=0;linei< linedata.size(); linei++)
    {
        Data_parsing(linedata[linei],data);//������������
        //���ݴ���
        if (data[0]=="K") //�����ǰΪ�ؼ��֣�����ؼ��ִ�����
        {
            if (!HS.empty() && HS.top() == "f" && isdeclar(data[2]) && isf) fK= fK+data[2]+"|";//����Ǻ����������򱣴溯�������Ĳ���
            else Keywords(out,data[2]);

            if (data[2] == "else")//�����else���ж�����Ƿ��д�����
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
                if (isdeclar(HS.top()))//���� Ϊ������������"f"ѹջ���γ��µ�״̬, , ���ұ��溯����
                {
                    isf = true;
                    HS.push("f");
                    fname = data[2];
                }
                else //�ô�Ϊ�������ã�ѹջ
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
                Data_parsing(linedata[linei + 2], t);//��ȡ���һλ������
                left = data[2] + p + t[2];//��a.b�ϲ������ұ���
                exep.push_back(left);//ѹ��exep
                linei += 2;//��������λ��������Ҫ�Ĳ���
            }
            else if (!HS.empty() && isdeclar(HS.top()))//�����ǰ״̬ջջ��Ϊ��������������Ϊ����״̬
            {
                vector<string> t;
                Data_parsing(linedata[linei+1], t);//��ȡ��һ����������
                if (t[2] != "]")
                {
                    if (t[2] != "[")//������������ʱ��Ϊ��ͨ����
                        outfile(out,HS.top(), data[2]);//������Ԫʽ������ ��ʶ�� _ _)
                    else//����Ϊ���飬�ַ����
                    {
                        Data_parsing(linedata[linei + 2], t);//��ȡ����ĳ��ȣ�----------------����һ������
                        string myarray = "(" + data[2] + "|" + t[2] + ")";
                        outfile(out,HS.top(), myarray);//������Ԫʽ������ ��ʶ�� ���� _)
                    }
                }
            }
            else if (!HS.empty() && HS.top() == "=" && isconst == false)//��ֵ״̬���Ѻ���ı�ʶ��������
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);//��ȡ��һ����������
                if (t[2] != "[")//����ʶ����Ϊ[ʱ���ж�Ϊ��ͨ����
                    exep.push_back(data[2]);
                else
                {
                    Data_parsing(linedata[linei + 3], t);
                    if (t[2] == "]")
                    {
                        isarray = true;
                        left = data[2];
                        Data_parsing(linedata[linei + 2], t);//��ȡ����ĳ��ȣ�----------------����һ�����鵽���ʽ��
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
                Data_parsing(linedata[linei + 1], t);//��ȡ��һ����������
                if (t[2] != "[")//����ʶ����Ϊ[ʱ���ж�Ϊ��ͨ����
                    exep.push_back(data[2]);
                else
                {
                    isarray = true;
                    left = data[2];
                    Data_parsing(linedata[linei + 2], t);//��ȡ����ĳ��ȣ�----------------����һ�����鵽���ʽ��
                    midel = t[2];
                    //string myarray = "(" + data[2] + "|" + t[2] + ")";
                    //exep.push_back(myarray);
                }
            }
            else if (!HS.empty() && (HS.top() == "while" || sem_n == 1 || HS.top() == "if" || HS.top() == "elif" || HS.top() == "return"))//while״̬��for�ĵ�һ�����󣬰Ѻ���ı�ʶ����������while��for��һ���ֺź��ǱȽϱ��ʽ��
            {
                vector<string> t;
                Data_parsing(linedata[linei + 1], t);//��ȡ��һ����������
                if (t[2] != "[")//����ʶ����Ϊ[ʱ���ж�Ϊ��ͨ����
                    exep.push_back(data[2]);
                else
                {
                    Data_parsing(linedata[linei + 3], t);
                    if (t[2] == "]")
                    {
                        //cout << data[2] << "?????" << endl;
                        isarray = true;
                        left = data[2];
                        Data_parsing(linedata[linei + 2], t);//��ȡ����ĳ��ȣ�----------------����һ�����鵽���ʽ��
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
            if (!HS.empty() && (HS.top() == "while" || HS.top() == "if" || HS.top() == "elif"))//���������ж�ѹջ���߻�����ʽ  || HS.top()=="for"
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
                            num_t++;//����һ�ж�����Ϊ���ʽ
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
                HS.push(data[2]);//ջ����Ϊ��=��
                vector<string> t;
                Data_parsing(linedata[linei - 1], t);//��ȡǰһ����������
                if (t[2] != "]")//����ʶ����Ϊ]ʱ���ж�Ϊ��ͨ����
                {
                    if (exep.size()==0)//���������һ��Ϊ��ͨ��ʶ����һ��Ϊ�ṹ��������ṹ������Ѿ���ǰ����exep������size��Ϊ0
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
                        Data_parsing(linedata[linei - 2], t);//��ȡ����ĳ��ȣ�----------------����һ�����鵽���ʽ��
                        vector<string> aname;
                        Data_parsing(linedata[linei - 4], aname);
                        string myarray = "(" + aname[2] + "|" + t[2] + ")";
                        left = myarray;
                        exep.push_back(myarray);
                    }
                }
                Data_parsing(linedata[linei + 2], t);//�ж��Ƿ�Ϊ������ֵ��������
                exep.push_back(HS.top());//����=��ѹ����ʽջ
                //if (t[2] == "(") HS.pop(),exep.clear(), HS.push("=f"),fI="(",isret=true;//����=��ѹ��=f,��ձ��ʽջ,�������������,������Ҫ����ֵ
                //else exep.push_back(HS.top());//����=��ѹ����ʽջ
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
            if (!HS.empty() && HS.top() == "=")//= �ڡ�������״̬����������
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

            if (!HS.empty() && isdeclar(HS.top())) HS.pop();//�����ڡ������󵯳�
            if (isconst) isconst = 0;//const�ڡ�������ı�״̬
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

            if (!lar_par.empty() && !HS.empty() && !lar_par.top() && !isdeclar(HS.top()))//û�д����ŵ�for��while��if��elif��else
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

            if (!HS.empty() && HS.top() == "for") sem_n++;//��¼�ֺŵ���Ŀ
            if (sem_n == 1) //��һ���ֺţ������for _ _ _��
            {
                outfile(out,HS.top());
                out << endl;
            }
            if (sem_n == 2)//�ڶ����ֺţ����򲻵�ʽ
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
            // ���ջ����"f"����ô�϶�Ϊ����
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

            if (HS.top() == "=")//forѭ��
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
            Data_parsing(linedata[linei + 2], t);//��ȡ�����ź���λ
            if (t[2] != "]") 
            {
                Data_parsing(linedata[linei - 1], t);
                left = t[2];//����������
                HS.push(data[2]);//ѹ��[����״̬ջ
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
    createQua();//������Ԫʽ
    //CM();//��ֵ���ʽ��ʡ
	return 0;
}