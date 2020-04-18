#ifndef EXTRACT_WORDS_H__
#define EXTRACT_WORDS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<conio.h>  


	  /*
	  1.提取文本
	  2.判断文本是否有英文单词
	  3.提取单词（或去掉非英文单词）
	  4.全部转换为小写单词
	  5.按相同单词计数
	  6.展示单词和意思以及数量
	  */
	  
	  //优化
	  /*
	  1.给出选择
				1）翻译单词
				2）增加新单词以及释义
				3）翻译文本中的单词
	  2.给出的文本翻译按一定规则排序(首字母、单词长度)
	  */

//装载或存储经过处理的单词
typedef struct attr_word
{
	char word[20];
	int count;
	struct attr_word *next;
}WORD;


int extra_txt(char **buf, char *name);				//提取文本
int have_words(char **buffer, char ***Words);		//提取"单词"
WORD *countofwords(char **Words, int len);			//"单词"计数
int distinct_words(WORD **head);					//去重
void showmeans(WORD *head);							//显示意思。。
void GUI(void);
void translate_word(void);
void new_add(void);
void translate_text(void);
void sort_of_first(char ***Words, int len);

#endif