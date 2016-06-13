#include "StdAfx.h"
#include "TextSpliter.h"

CTextSpliter::CTextSpliter(void)
{
}

CTextSpliter::~CTextSpliter(void)
{
}


//////////////////////////////////////////////////////////////////////////
// ������һЩͨ�÷���

static std::string ws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}

static std::wstring s2ws(const std::string& s)
{
    setlocale(LC_ALL, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

static wstring CString2string(CString strMfc){return strMfc.GetBuffer(0);}
static CString string2CString(wstring strStl){return strStl.c_str();}

static bool cmp_pair_value(pair<wstring,ULONG> a, pair<wstring,ULONG> b){return (a.second == b.second)? (a.first.compare(b.first)<=0) : (a.second < b.second);;}
static bool cmp_pair_value_r(pair<wstring,ULONG> a, pair<wstring,ULONG> b){return (a.second == b.second)? (a.first.compare(b.first)<=0) : (a.second > b.second);}

static std::vector<std::wstring> string_split_to_vector(const  std::wstring& s, const std::wstring& delim)  
{  
    std::vector<std::wstring> elems;  
    size_t pos = 0;  
    size_t len = s.length();  
    size_t delim_len = delim.length();  
    if (delim_len == 0) return elems;  
    while (pos < len)  
    {  
        int find_pos = s.find(delim, pos);  
        if (find_pos < 0)  
        {  
            elems.push_back(s.substr(pos, len - pos));  
            break;  
        }  
        elems.push_back(s.substr(pos, find_pos - pos));  
        pos = find_pos + delim_len;  
    }  
    return elems;  
}  

static void spliter_insert_into_map(map<wstring,ULONG> & m, wstring key)
{
    map<wstring,ULONG>::iterator itr;
    ULONG val = 0;
    itr = m.find(key);
    if(itr != m.end())
        itr->second++;
    else
        m.insert(pair<wstring,ULONG>(key,val+1));
}


static CString read_file_to_string( CString filepath ) 
{
    CFile fFile(filepath,CFile::modeRead);  

    // ��Ϊ��Windows�³��õ��ı��������ΪANSI  
    // ���������ڵ���ѡ���ʱ��ѡ����Unicode����  
    // ������Ҫʹ��CStringA���͵ı�����  
    // CStringA������⣺http://baike.baidu.com/view/998109.htm  
    CStringA strFile ;  

    fFile.Read(strFile.GetBufferSetLength(fFile.GetLength()),fFile.GetLength());  
    fFile.Close();  
    strFile.ReleaseBuffer();  

    // �򻷾�ΪUnicdoe��ԭ��CStringA������ֱ����ʾ������ؼ���  
    // ����ʹ��CStringת��ΪUnicdoe�ַ�  
    return CString(strFile);
}


static void spliter_write_vector_to_file(wstring file, vector<wstring> & v)
{
    ofstream fout;
    fout.open(file.c_str(),ios.trunc);
    vector<wstring>::iterator itr;
    for (itr=v.begin();itr!=v.end();itr++)
    {
        string key = ws2s(*itr);
        fout<<key<<"\r\n";
    }
    fout.flush();
    fout.close();
}
static void spliter_write_vector_to_file(wstring file, vector<WORD_INFO> & v)
{
    ofstream fout;
    fout.open(file.c_str(),ios.trunc);
    vector<WORD_INFO>::iterator itr;
    ULONG times = -1;
    for (itr=v.begin();itr!=v.end();itr++)
    {
        string key = ws2s(itr->word);
        ULONG count = itr->count;
        if(itr->times != times)
        {
            times = itr->times;
            fout<<"//,//////////////////////////////////////////////////////////////////////////"<<"\r\n";
            fout<<"//,��"<<times+1<<"��������"<<"\r\n";
        }

        fout<<count<<" , "<<key<<"\r\n";
    }
    fout.flush();
    fout.close();
}
static void pop_all_element_in_map(map<wstring,ULONG> & m)
{
    CString word;
    ULONG count;
    map<wstring,ULONG>::iterator itr;
    for (itr=m.begin();itr!=m.end();++itr)
    {
        word = string2CString(itr->first);
        count = itr->second;
        CString strMsg;
        strMsg.Format(_T("%s = %d"),word,count);
        AfxMessageBox(strMsg);
        cout<<word<<":"<<count<<endl;
    }
}

static void pop_all_element_in_vector(vector<wstring> & v)
{
    CString word;
    vector<wstring>::iterator itr;
    for (itr=v.begin();itr!=v.end();++itr)
    {
        word = string2CString(*itr);
        AfxMessageBox(word);
        cout<<word<<endl;
    }
}
static void pop_all_element_in_vector(vector<WORD_INFO> & v)
{
    CString word;
    ULONG count;
    vector<WORD_INFO>::iterator itr;
    for (itr=v.begin();itr!=v.end();++itr)
    {
        word = string2CString(itr->word);
        count = itr->count;
        CString strMsg;
        strMsg.Format(_T("%s, count=%d, times"),word,count,itr->times);
        AfxMessageBox(strMsg);
        cout<<word<<":"<<count<<endl;
    }
}

//////////////////////////////////////////////////////////////////////////
// �������ı�  ���зִ�ͳ��

// ��ȡ���ı�
CString CTextSpliter::spliter_load_file_to_process(CString filepath)
{
    return read_file_to_string(filepath);
}
// �ִ�ͳ��
void CTextSpliter::spliter_word_split(CString filepath)
{
    CString strTxt = spliter_load_file_to_process(filepath);
    strTxt = strTxt.MakeLower();

    CString word;
    ULONG count =0;
    for(ULONG i=0;i<strTxt.GetLength();i++)
    {

        if(strTxt[i] >= _T('a') && strTxt[i] <= _T('z'))
            word += strTxt[i];
        else if(word.GetLength() != 0)
        {
            spliter_insert_into_map(WordsList,CString2string(word));
            word = _T("");
            count = 0;
        }
    }
    if(word.GetLength() != 0)
        spliter_insert_into_map(WordsList,CString2string(word));
}
// ���ִʽ��������ļ���
void CTextSpliter::spliter_output_result(CString split_result_file)
{
    wstring file = CString2string(split_result_file);
    vector<pair<wstring,ULONG>>  arrWords;
    map<wstring,ULONG>::iterator itr;
    for (itr=WordsList.begin();itr!=WordsList.end();itr++)
        arrWords.push_back(*itr);
    sort(arrWords.begin(),arrWords.end(),cmp_pair_value_r);

    ofstream fout;
    fout.open(file.c_str(),ios.trunc);
    vector<pair<wstring,ULONG>>::iterator itr_vec;
    for (itr_vec=arrWords.begin();itr_vec!=arrWords.end();itr_vec++)
    {
        string key = ws2s(itr_vec->first);
        ULONG count = itr_vec->second;
        fout<<count<<" , "<<key<<"\r\n";
    }
    fout.flush();
    fout.close();

}
void CTextSpliter::UnitTest_word_split()
{
    pop_all_element_in_map(WordsList);
}



////////////////////////////////////////////////////////////////////////////////////////
// ��ȡ���� ������ʿ�

// ������ʿ�
void CTextSpliter::knower_load_known_words(CString filepath)
{
    CString know_words_text = read_file_to_string(filepath);

    //know_words_text = know_words_text.MakeLower(); // �汾���ֶ��д�д��ĸ ������ʿ������д�Сд
    know_words_text.Replace(_T("\r\n"),_T("\n"));
    know_words_text.Replace(_T(" "),_T(""));

    wstring strSTL_know_words_text = CString2string(know_words_text);
    vector<wstring> know_words = string_split_to_vector(strSTL_know_words_text,_T("\n"));

    for (int i=0;i<know_words.size();i++)
    {
        wstring line = know_words[i];
        vector<wstring> know_word_info = string_split_to_vector(line,_T(","));
        KnownWordsList.insert(pair<wstring,ULONG>(know_word_info[0],_wtoi(know_word_info[1].c_str())));
    }
}

// ����ʿ���бȽ���ȡ�´�  ������ʿ�
void CTextSpliter::knower_extract_new_words(map<wstring,ULONG> & words)
{
    bool IsThereFirstMetWord = false;
    // ��ȡ�´�
    map<wstring,ULONG>::iterator words_itr;
    for (words_itr=words.begin();words_itr!=words.end();words_itr++)
    {
        WORD_INFO wordInfo;
        wordInfo.word = words_itr->first;
        wordInfo.count = words_itr->second;
        wordInfo.times = 0;
        map<wstring,ULONG>::iterator knowwords_itr;
        knowwords_itr = KnownWordsList.find(words_itr->first);
        if(knowwords_itr == KnownWordsList.end())
        {
            NewWords.push_back(wordInfo);
            IsThereFirstMetWord = true;
        }
        else if(knowwords_itr->second <= WORD_WELL_KNOWN_THREHOLD) // ����̫���
        {
            wordInfo.times = knowwords_itr->second;
            NewWords.push_back(wordInfo);
        }
    }
    sort(NewWords.begin(),NewWords.end());

    // ����е�һ���������´�(˵�������ı�) ���д�׷��һ�� �´�׷�ӵ��ʿ� ���д���������+1
    if(IsThereFirstMetWord)
    {
        map<wstring,ULONG>::iterator words_itr;
        for (words_itr=words.begin();words_itr!=words.end();words_itr++)
            spliter_insert_into_map(KnownWordsList, words_itr->first);
        spliter_insert_into_map(KnownWordsList, Z_LEXICON_VERSION_KEY);// ���ڴʿ����Ӱ汾��
    }
}

// ����ȡ���´�д�뵽�ļ���
void CTextSpliter::knower_output_new_words(CString new_words_file)
{
    spliter_write_vector_to_file(CString2string(new_words_file),NewWords);
}

// �����µ���ʿ�д�ص���ʿ��ļ���
void CTextSpliter::knower_write_know_words_file(CString known_words_file)
{
    wstring file = CString2string(known_words_file);
    vector<pair<wstring,ULONG>>  arrWords;
    map<wstring,ULONG>::iterator itr;
    itr=KnownWordsList.begin();
    ULONG curLexiconVersion = 1;
    if(itr->first.compare(Z_LEXICON_VERSION_KEY)==0)
    {
        curLexiconVersion = itr->second;
        itr++;
    }
    for (;itr!=KnownWordsList.end();itr++)
        arrWords.push_back(*itr);
    sort(arrWords.begin(),arrWords.end(),cmp_pair_value);

    string key;
    ofstream fout;
    fout.open(file.c_str(),ios.trunc);
    key = ws2s(Z_LEXICON_VERSION_KEY);
    fout<<key<<" , "<<curLexiconVersion<<"\r\n";

    vector<pair<wstring,ULONG>>::iterator itr_vec;
    string align_space_max = "          ";
    for (itr_vec=arrWords.begin();itr_vec!=arrWords.end();itr_vec++)
    {
        key = ws2s(itr_vec->first);
        ULONG count = itr_vec->second;
        string align_space = (align_space_max.length()>key.length()) ? align_space_max.substr(0,align_space_max.length()-key.length()) : "";
        fout<<key<<align_space<<" , "<<count<<"\r\n";
    }
    fout.flush();
    fout.close();

}


void CTextSpliter::UnitText_load_know_words()
{
    pop_all_element_in_map(KnownWordsList);
}


void CTextSpliter::UnitText_extract_new_words()
{
    pop_all_element_in_vector(NewWords);
}


