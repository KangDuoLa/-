#include "extract_words.h"

//装载本地词库的结构体
struct dict
{
	char *key;
	char *content;
};

//去掉字符串结尾的换行、空格、回车和转换小写
void format_string(char *str, int num)
{
	int i;
	
	for(i = strlen(str) - 1; i >= 0; i--)
	{
		if(str[i] != '\n' && str[i] != ' ' && str[i] != '\r')
		{
			str[i + 1] = '\0';
			break;
		}
	}

	//大写转小写
	if(num == 0)
	{
		while(*str != '\0')
		{
			if(*str >= 'A' && *str <= 'Z')
			{
				*str += 32;
			}
			str++;
		}
	}
}

void GUI(void)
{
	int num = 0;
	system("cls");
	printf("********************Welcome to Word translation system!********************\n");
	printf("*Please choose the number:                                                *\n");
	printf("*		1.Translate the one word                                  *\n");
	printf("*		2.Add the new word and translation                        *\n");
	printf("*		3.Translate the text                                      *\n");
	printf("*		4.Exit the system                                         *\n");
	printf("***************************************************************************\n");
	scanf("%d", &num);
	switch(num)
	{
		case 1 : translate_word();
			break;
		case 2 : new_add();
			break;
		case 3 : translate_text();
			break;
		case 4 : printf("See you next time!\n"); 
				 exit(0);
		default: printf("The number is wrong!\n");
				 getch();
	}
}

void translate_word(void)
{
	struct dict *p = NULL;
	char key[20] = {0};
	char content[128] = {0};
	int len = open_dict(&p, "newdict.txt");	
	
	while(1)
	{
		printf("Please input a word. If you puts \"exit\" then will be back.\n");
		memset(key, 0, sizeof(key));
		memset(content, 0, sizeof(content));
		scanf("%s", key);
		format_string(key, 1);									//去掉字符串结尾的空格和回车
		if(strcmp(key, "exit") == 0)
		{
			system("cls");
			return ;
		}
		
		if(search_dict(p, len, key, content))					//根据用户输入，在字典中检索
		{
			printf("%s\n", content);
		} 
		else
		{
			printf("Sorry not found.\n");
		}
	}
}


void new_add(void)
{
	struct dict *p = NULL;
	FILE *fp = NULL;
	char word[20] = {0};
	char key[30] = {"#"};
	char trans[128] = {0};
	char content[150] = {"Trans:"};
	int len = open_dict(&p, "newdict.txt");	
	
	while(1)
	{
		printf("Please input the \"new word\". If you input the \"exit\" will be back.\n");
		memset(word, 0, sizeof(word));
		memset(key + 1, 0, sizeof(key));
		memset(trans, 0, sizeof(trans));
		memset(content + 6, 0, sizeof(content));
		scanf("%s", word);
		format_string(word, 1);
		
		if(strcmp(word, "exit") == 0)
		{
			system("cls");
			return ;
		}
		
		if(search_dict(p, len, word, trans) == 1)
		{
			printf("%s\n", trans);
			printf("Have been existed the word, please change or exit.\n");
			continue;
		}
		else
		{
			fp = fopen("newdict.txt", "a");	
			printf("Please input the translation:\n");
			
			scanf("%s", word);
			strcat(key, word);
			strcat(content, trans);
			
			fseek(fp, 0, SEEK_END);
			fprintf(fp, "%s\n", key);
			fprintf(fp, "%s\n", content);
			fclose(fp);
		}
	}
}

void translate_text(void)
{
	char filename[32] = {0};
	printf("Please input the filename:");
	scanf("%s", filename);
	
	char *buf = NULL;
	char **words = NULL;                             //用于存储提取的单词
	WORD *Words = NULL;								 //（链表）装载或存储经过处理的单词
	int len = 0;
	int i = 0;
	
	if(extra_txt(&buf, filename) < 0)
	{
		printf("func extra_txt() err. Maybe your filename is wrong!\n");
		getch();
		return ;
	}

	if((len = have_words(&buf, &words)) < 0)
	{
		printf("func have_words() err.\n");
		return ;
	}
	
	sort_of_first(&words, len);
	
	if((Words = countofwords(words, len)) == NULL)
	{
		printf("func countofwords() err.\n");
		return ;
	}
	
	for(; i < 5; i++)
	{
		distinct_words(&Words);
	}
	
	showmeans(Words);
	printf("\n\nTranslate Successfuly!\n");
	getch();
}

//提取文本
int extra_txt(char **buf, char *name)
{
	FILE *fp = NULL;
	int lenth = 0;
	
	if((fp = fopen(name, "r")) == NULL)
	{
		printf("fopen() err.\n");
		return -1;
	}
	
	//获取长度
	fseek(fp, 0, SEEK_END);
	lenth = ftell(fp);
	fseek(fp, 0, SEEK_SET);     //注意移动回去

	*buf = malloc(lenth + 1);
	memset(*buf, 0, lenth + 1);

	fread(*buf, lenth, 1, fp);

	return 0;
}


//提取"单词"
int have_words(char **buffer, char ***Words)
{
	FILE *fp = NULL;
	char *buf = *buffer;
	char **words = NULL;
	char tbuf[20] = {0};
	char *tmp = malloc(2 * strlen(buf));
	char *Temp = tmp;
	int i = 0;
	int len = 0;
	int count = 0;
	memset(tmp, 0, 2 * strlen(buf));
	
	while(*buf != '\0')
	{

		if(*buf >= 'a' && *buf <= 'z' || *buf >= 'A' && *buf <=  'Z')//是字母
		{
			while(1)
			{
				*tmp = *buf;
				if(!(*buf >= 'a' && *buf <= 'z' || *buf >= 'A' && *buf <=  'Z'))//不是字母
				{
					*tmp = '\n';
					tmp++;
					count++;
					break;
				}
				buf++;
				tmp++;
				count++;
			}
		}
		buf++;
	}

	tmp = Temp;
	if(count == 0)
	{
		printf("The text have no words!\n");
		return -1;
	}
	tmp[count] = '\0';
		
	fp = fopen("Temp.txt", "w");
	fwrite(Temp, count, 1, fp);	
	fclose(fp);
	
	fp = fopen("Temp.txt", "r");
	
	words = malloc(4);							//指针4个字节
	i = 0;
	
	while(!feof(fp))							//文件没到末尾
	{
		memset(tbuf, 0, sizeof(tbuf));
		fgets(tbuf, sizeof(tbuf), fp);			//读取一行
		len = strlen(tbuf);
		if(len > 0)
		{
			words[i] = malloc(len);
			memset(words[i], 0, len);
			format_string(tbuf, 0);
			strcpy(words[i], tbuf);
		}
		i++;
		words = realloc(words, (i + 1) * 4);    //注意(i + 1)
		memset(words + i, 0, 4);				//注意(words + i)
	}
	fclose(fp);
	free(tmp);
	*Words = words;
	return i - 1;
}


//按首字母排序
void sort_of_first(char ***Words, int len)
{
	if(Words == NULL)
	{
		printf("Words is NULL.\n");
		return ;
	}
	
	int i = 0, j = 0;
	char **words = *Words;
	char *tmp = NULL;
	
	for(; i < len; i++)
	{
		for(j = i; j < len; j++)
		{
			if(words[i][0] > words[j][0])
			{
				tmp = words[i];
				words[i] = words[j];
				words[j] = tmp;
			}
		}
	}

	return ;
}

//打印
void print_word(WORD *head)
{
	WORD *tmp = head;
	for(tmp = tmp->next; tmp != NULL; tmp = tmp->next)
	{
		printf("%s  %d\n", tmp->word, tmp->count);
	}
}


//"单词"计数
WORD *countofwords(char **Words, int len)
{
	if(Words == NULL)
	{
		printf("Words is NULL.\n");
		return NULL;
	}
	
	char **words = Words;
	char tmp[20] = {0};
	int num = 0;
	int i = 0, j = 0;
	WORD *head = malloc(sizeof(WORD));
	WORD *p = head;

	//计数
	while(i != len)
	{
		WORD *temp = malloc(sizeof(WORD));
		p->next = temp;							//末位插入
		strcpy(tmp, words[i]);					//用作对比
		strcpy(temp->word, words[i]);			//保存
		while(j != len)							//循环比对
		{
			if(strcmp(tmp, words[j]) == 0)
			{
				num++;
			}
			j++;
		}
		temp->count = num;
		p = temp;
		p->next = NULL;
		num = 0;
		j = 0;
		i++;
	}
	
	return head;
}


//去重
int distinct_words(WORD **head)
{
	if(head == NULL)
	{
		printf("head is NULL.\n");
		return -1;
	}
	
	int flag = 0;
	WORD *tmp = (*head)->next;
	WORD *cur = tmp;
	WORD *pcur = *head;

	while(tmp != NULL)
	{
		while(cur != NULL)
		{
			if(strcmp(tmp->word, cur->word) == 0)
			{
				flag++;	
				if(flag > 1)
				{
					pcur->next = cur->next;				//删除
					if(pcur->next == NULL)
					{
						free(cur);
						flag = 1;
						break;
					}
					free(cur);
					cur = pcur->next;
					flag = 1;
				}
			}		
			pcur = pcur->next;
			cur = cur->next;
		}

		flag = 0;
		pcur = tmp;										//注意移动回去
		tmp = tmp->next;
		cur = tmp;
	}

	return 0;
}

//打开字典文件，并读取文件内容
int open_dict(struct dict **p, const char *name)
{
	FILE *fp = fopen(name, "r");
	*p = (struct dict *)malloc(sizeof(struct dict));
	char buf[2048] = {0};
	int len = 0;
	int i = 0;
	memset(*p, 0, sizeof(struct dict));
	
	while(!feof(fp))
	{
		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), fp);
		len = strlen(buf);
		if(len > 0)
		{
			(*p)[i].key = (char *)malloc(len);
			memset((*p)[i].key, 0, len);
			format_string(buf, 1);
			strcpy((*p)[i].key, &buf[1]);
		}

		memset(buf, 0, sizeof(buf));
		fgets(buf, sizeof(buf), fp);
		len = strlen(buf);
		if(len > 0)
		{
			(*p)[i].content = (char *)malloc(len);
			memset((*p)[i].content, 0, len);
			strcpy((*p)[i].content, &buf[6]);
		}
		
		i++;
		*p = (struct dict *)realloc(*p, (1 + i) * sizeof(struct dict));
		memset(*p + i, 0, sizeof(struct dict));
	}
	fclose(fp);

	return i;
}

//根据关键字key,在字典中查找内容
int search_dict(const struct dict *p, int size, const char *key, char *content)
{
	int i = 0;
	for(i = 0; i < size; i++)//遍历字典
	{
		if((p[i].key == NULL) || (p[i].content == NULL))
		{
			continue;
		}

		if(strcmp(p[i].key, key) == 0)
		{
			strcpy(content, p[i].content);
			return 1;
		}
	}
	return 0;
}

void free_memory(struct dict *p, int size, WORD *head)
{
	int i = 0;
	WORD *tmp = head;
	for(i = 0; i < size; i++)//循环释放key与content成员内存
	{
		if(p[i].key)
		{
			free(p[i].key);
		}
		
		if(p[i].content)
		{
			free(p[i].content);
		}
	}
	free(p);
	
	for(; head != NULL; head = head->next)
	{
		free(tmp);
		tmp = head;
	}
}


void showmeans(WORD *head)
{
	if(head == NULL)
	{
		printf("head is NULL.\n");
		return ;
	}
	
	int len = 0;
	struct dict *p = NULL;
	char content[512] = {0};
	FILE *fpk = NULL;
	FILE *fpu = NULL;
	WORD *tmp = head->next;
	fpk = fopen("know_words.txt", "a");	
	fpu = fopen("unknow_words.txt", "a");	
	
	len = open_dict(&p, "newdict.txt");
	
	for(; tmp != NULL; tmp = tmp->next)
	{
		if(search_dict(p, len, tmp->word, content) == 0)
		{
			printf("%s is not in dict.\n", tmp->word);
			fprintf(fpu, "%s \n", tmp->word);
		}
		else
		{
			printf("%s ------- %s -------%d\n", tmp->word, content, tmp->count);
			fprintf(fpk, "%s ------- %s -------%d\n", tmp->word, content, tmp->count);
		}
	}

	fclose(fpk);	
	fclose(fpu);	
	free_memory(p, len, head);
}

