#pragma once

#include <afxtempl.h>
#include <iostream> 
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
using namespace std ;

#define  WORD_WELL_KNOWN_THREHOLD   3
#define  Z_LEXICON_VERSION_KEY      _T("0_Z_Lexicon_Version")

typedef struct _WORD_INFO
{
    wstring    word;   // 单词
    ULONG      times;  // 历史第几次遇到
    ULONG      count;  // 本次处理文本中出现的次数
    bool operator<(const _WORD_INFO & pinfo) const
    {
        return (times==pinfo.times) ? (count>pinfo.count) : (times<pinfo.times);
    }
    bool operator>(const _WORD_INFO & pinfo) const
    {
        return (times==pinfo.times) ? (count<pinfo.count) : (times>pinfo.times);
    }
}WORD_INFO;

class CTextSpliter
{
public:
    CTextSpliter(void);
    ~CTextSpliter(void);


//////////////////////////////////////////////////////////////////////////
// 新文本分词统计处理
public:
    void spliter_split_process(CString filepath, CString split_result_file)
    {
        spliter_word_split(filepath);
        //UnitTest_word_split();return;
        spliter_output_result(split_result_file);
    }
public:
    map<wstring,ULONG>   WordsList;
private:
    void spliter_word_split(CString filepath);
    void spliter_output_result(CString split_result_file);
    CString spliter_load_file_to_process(CString filepath);
private:
    void UnitTest_word_split();


////////////////////////////////////////////////////////////////////////////////////////
// 生词提取 更新熟词库
public:
    void knower_new_words_process(CString know_words_file,CString new_words_file, map<wstring,ULONG> & words)
    {
        knower_load_known_words(know_words_file);
        //UnitText_load_know_words(); return;

        knower_extract_new_words(words);
        //UnitText_extract_new_words(); return;
        knower_output_new_words(new_words_file);
        knower_write_know_words_file(know_words_file);
    }
private:
    map<wstring,ULONG>   KnownWordsList;
    vector<WORD_INFO>   NewWords;
private:
    void knower_load_known_words(CString filepath);
    void knower_extract_new_words(map<wstring,ULONG> & words);
    void knower_output_new_words(CString new_words_file);
    void knower_write_know_words_file(CString know_words_file);
private:
    void UnitText_load_know_words();
    void UnitText_extract_new_words();
};
