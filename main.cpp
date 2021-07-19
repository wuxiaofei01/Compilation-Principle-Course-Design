#include"quaternion.cpp"

using namespace std;
#pragma warning(disable:4996)

typedef struct quat {	//每条四元式
	char Q[4][30];

	quat* next;
	quat* before;
} quat;

typedef struct block {	//每块
	block* before;
	block* next;
	quat* q_head;
	quat* q_tail;
}block;


int count_while[20] = { 0 };
int count_for[20] = { 0 };
block* head;
block* temp_b;
char biaoshifu[40][20] = { 0 };
char function_names[20][20] = { 0 };
int fun_name_num = 0;
int if_num = 0;
char digui[20][20] = { 0 };
int digui_num = 0;

block* init_block()
{
	block* temp = (block*)malloc(sizeof(block));
	block* cur = head;

	while (cur->next != NULL)
	{
		cur = cur->next;
	}
	temp->before = cur;
	cur->next = temp;
	temp->next = NULL;

	quat* temp_q = (quat*)malloc(sizeof(quat));
	temp->q_head = temp_q;
	temp->q_head->before = temp->q_head;
	temp->q_head->next = NULL;

	return temp;
}

quat* init_quat(block* cur_block)
{
	quat* temp = (quat*)malloc(sizeof(quat));
	quat* cur = cur_block->q_head;

	while (cur->next != NULL)
	{
		cur = cur->next;
	}
	cur_block->q_tail = temp;
	temp->before = cur;
	cur->next = temp;
	temp->next = NULL;

	return temp;
}

void delete_quat(block* cur_b, quat* cur)
{

	if (cur == cur_b->q_head)
	{
		if (cur->next != NULL)
		{
			cur_b->q_head = cur->next;
			cur_b->q_head->before = cur_b->q_head;
		}
		else
		{
			if (cur_b->next != NULL)
			{
				cur_b->before->next = cur_b->next;
				cur_b->next->before = cur_b->before;
			}
			else
			{
				cur_b->before->next = NULL;
			}
		}
	}

	if (cur->next != NULL)
	{
		cur->next->before = cur->before;

		cur->before->next = cur->next;
	}
	else
	{
		cur_b->q_tail = cur->before;
		cur->before->next = NULL;
	}
	free(cur);
}

void delete_block(block* cur)
{
	if (cur == head)
	{
		head = head->next;
		head->before = head;
	}

	if (cur->next != NULL)
	{
		cur->next->before = cur->before;
		cur->before->next = cur->next;
	}
	else
	{
		cur->before->next = NULL;
	}
	free(cur);
}

void read_quat()	//读取文件形成链表
{
	FILE* fp;
	fp = fopen("quaternion.txt", "r");
	char cur[30] = { 0 };	//临时存放单词

	block* cur_b;
	cur_b = head;

	quat* cur_q;
	cur_q = cur_b->q_head;

	quat* cur_q_tail;
	cur_q_tail = cur_q;

	while (1)
	{
		char c;
		for (int j = 0; j < 4; j++)	//一个四元式
		{

			for (int i = 0; i < 30; i++)
			{
				c = fgetc(fp);
				if (c == ' ')
				{
					strcpy(cur_q->Q[j], cur);
					memset(cur, '\0', sizeof(cur));
					break;
				}
				else if (c != '\n')
				{
					cur[i] = c;
				}

				if (c == '\n')	//四元式结束
				{
					strcpy(cur_q->Q[j], cur);

					memset(cur, '\0', sizeof(cur));
					break;
				}
			}
		}

		if (c == '\n')
		{
			c = fgetc(fp);
			if (c == '\n')
			{
				init_block();
				cur_b->q_tail = cur_q;
				cur_b = cur_b->next;
				cur_q = cur_b->q_head;

			}
			else if (feof(fp))
				break;
			else
			{
				fseek(fp, -1, SEEK_CUR);
				init_quat(cur_b);

				cur_q = cur_q->next;
			}
			continue;
		}
	}
	if (strcmp(cur_q->Q[0], cur_q->Q[1]) == 0)
	{
		delete_quat(cur_b, cur_q);
	}

	fclose(fp);
}

void data_seg(char biaoshifu[][20])
{
	FILE* fp;
	fp = fopen("result.txt", "w");
	fprintf(fp, "DSEG SEGMENT\n");
	printf("DSEG SEGMENT\n");

	int i = 0;
	while (strcmp(biaoshifu[i], ""))
	{
		if (biaoshifu[i][0] >= '0' && biaoshifu[i][0] <= '9')	//是数组
		{
			int j = 0;
			char num[30] = { 0 };
			while (biaoshifu[i][j] >= '0' && biaoshifu[i][j] <= '9')
			{
				num[j] = biaoshifu[i][j];
				j++;
			}
			int k = 0;
			char name[30] = { 0 };
			while (biaoshifu[i][j] != '\0')
			{
				name[k] = biaoshifu[i][j];
				j++;
				k++;
			}

			fprintf(fp, "%s DW %s DUP(0)\n", name, num);
			printf("%s DW %s DUP(0)\n", name, num);
			i++;
		}
		else
		{
			fprintf(fp, "%s DW 0\n", biaoshifu[i]);
			printf("%s DW 0\n", biaoshifu[i]);
			i++;
		}

	}
	fprintf(fp, "DSEG ENDS\n\n");
	printf("DSEG ENDS\n\n");
	fprintf(fp, "SSEG SEGMENT STACK\nSKTOP DW 300 DUP(0)\nSSEG ENDS\n\n");
	printf("SSEG SEGMENT STACK\nSKTOP DW 300 DUP(0)\nSSEG ENDS\n\n");
	fclose(fp);
}

void write_list(block* head)
{
	FILE* fp;
	fp = fopen("linklist.txt", "w");

	block* temp_b;
	temp_b = head;

	quat* temp_q;
	temp_q = temp_b->q_head;
	while (temp_b != NULL)
	{

		temp_q = temp_b->q_head;
		while (temp_q != NULL)
		{
			//printf("%s %s %s %s\n", temp_q->Q[0], temp_q->Q[1], temp_q->Q[2], temp_q->Q[3]);
			fprintf(fp, "%s %s %s %s\n", temp_q->Q[0], temp_q->Q[1], temp_q->Q[2], temp_q->Q[3]);

			temp_q = temp_q->next;

		}
		//printf("\n");
		fprintf(fp, "\n");
		temp_b = temp_b->next;
	}
	fclose(fp);
}

void find_functions()
{
	block* temp_b1 = temp_b;
	quat* temp_q1 = temp_b1->q_head;

	block* temp_b2 = temp_b;			//函数头
	quat* temp_q2 = temp_b1->q_head;

	block* temp_b3 = temp_b;		//函数尾
	quat* temp_q3 = temp_b1->q_head;

	char temp_arr[5][20] = { 0 };
	int k = 0;


	while (temp_b1 != NULL)		//找到一个函数头
	{
		temp_q1 = temp_b1->q_head;
		while (temp_q1 != NULL)
		{

			if (strcmp(temp_q1->Q[0], "f") == 0 && strcmp(temp_q1->Q[1], "end") != 0)
			{
				temp_b2 = temp_b1;
				temp_q2 = temp_q1;

				init_quat(temp_b1);

				strcpy(temp_b1->q_tail->Q[0], "int");				//函数类型待定

				strcpy(temp_b1->q_tail->Q[1], temp_q1->Q[1]);
				strcat(temp_b1->q_tail->Q[1], "_ret");
				strcpy(temp_b1->q_tail->Q[2], "_");
				strcpy(temp_b1->q_tail->Q[3], "_");		//函数名_ret为函数返回值

				break;
			}
			else if (strcmp(temp_q1->Q[0], "declarP") == 0)
			{

				temp_b2 = temp_b1;
				temp_q2 = temp_q1;

				init_quat(temp_b1);

				strcpy(temp_b1->q_tail->Q[0], "int");				//函数类型待定

				strcpy(temp_b1->q_tail->Q[1], temp_q1->Q[1]);
				strcat(temp_b1->q_tail->Q[1], "_ret");
				strcpy(temp_b1->q_tail->Q[2], "_");
				strcpy(temp_b1->q_tail->Q[3], "_");		//函数名_ret为函数返回值


				int i = 1;
				char c;
				c = temp_q1->Q[3][1];

				while (c != ')')
				{
					int j = 0;

					while (c != '|' && c != ')')
					{
						temp_arr[k][j] = c;
						i++;
						j++;
						c = temp_q1->Q[3][i];
						if (c == '|' || c == ')')
						{
							break;
						}

					}

					init_quat(temp_b1);
					strcpy(temp_b1->q_tail->Q[0], "int");
					strcpy(temp_b1->q_tail->Q[1], temp_arr[k]);

					strcat(temp_b1->q_tail->Q[1], temp_q1->Q[1]);
					strcpy(temp_b1->q_tail->Q[2], "_");
					strcpy(temp_b1->q_tail->Q[3], "_");

					k++;

					if (c == ')')
					{
						break;
					}

					i++;
					c = temp_q1->Q[3][i];
				}

				char temp[30] = { 0 };
				temp[0] = '(';
				strcat(temp, temp_arr[0]);
				strcat(temp, temp_q1->Q[1]);
				for (int n = 1; n < k; n++)
				{
					strcat(temp, "|");
					strcat(temp, temp_arr[n]);
					strcat(temp, temp_q1->Q[1]);
				}
				strcat(temp, ")");
				strcpy(temp_q1->Q[3], temp);

				break;

			}
			temp_q1 = temp_q1->next;
		}

		if (temp_q1 == NULL)
		{
			temp_b1 = temp_b1->next;
			temp_b = temp_b1;
			continue;
		}
		else if (strcmp(temp_q1->Q[0], "f") == 0 && strcmp(temp_q1->Q[1], "end") != 0)
			break;
		else if (strcmp(temp_q1->Q[0], "declarP") == 0)
			break;
	}

	temp_b = temp_b1;
	if (temp_b == NULL)
	{
		return;
	}

	temp_b3 = temp_b2->next;
	temp_q3 = temp_b3->q_head;;
	while (temp_b3 != NULL)				//找到该函数结尾
	{
		temp_q3 = temp_b3->q_head;
		while (temp_q3 != NULL)
		{
			if (strcmp(temp_q3->Q[0], "f") == 0 && strcmp(temp_q3->Q[1], "end") == 0)
			{
				break;
			}
			temp_q3 = temp_q3->next;
		}

		if (temp_q3 == NULL)
		{
			temp_b3 = temp_b3->next;
			temp_b = temp_b3;
			continue;
		}
		else if (strcmp(temp_q3->Q[0], "f") == 0 && strcmp(temp_q3->Q[1], "end") == 0)
			break;
	}

	while (temp_q1 != temp_q3 && temp_b1 != temp_b3->next)
	{
		if (temp_q1 != temp_q2)
			temp_q1 = temp_b1->q_head;
		if (temp_q1 == temp_q3)
			break;
		while (temp_q1 != NULL)
		{
			for (int i = 0; i < k; i++)
			{
				for (int j = 0; j < 4; j++)
				{

					if (strcmp(temp_q1->Q[j], temp_arr[i]) == 0)
					{
						strcat(temp_q1->Q[j], temp_q2->Q[1]);
					}
				}
			}
			temp_q1 = temp_q1->next;
			if (temp_q1 == temp_q3)
				break;
		}
		if (temp_q1 == temp_q3)
			break;
		if (temp_q1 == NULL)
			temp_b1 = temp_b1->next;
	}

	/*链接*/

	block* new_bhead = (block*)malloc(sizeof(block));
	block* new_btail = (block*)malloc(sizeof(block));

	new_bhead->before = new_bhead;
	new_bhead->q_head = temp_q2;
	new_bhead->q_tail = temp_b2->q_tail;

	new_btail->q_head = temp_b3->q_head;
	new_btail->q_tail = temp_q3;
	temp_b3->q_head = temp_q3->next;
	new_btail->next = head;
	head->before = new_btail;


	if (temp_b3 != temp_b2->next)
	{
		new_bhead->next = temp_b2->next;
		new_btail->before = temp_b3->before;
		temp_b3->before->next = new_btail;
	}
	else
	{
		new_bhead->next = new_btail;
		new_btail->before = new_bhead;
	}

	temp_b2->next = temp_b3;
	temp_b3->before = temp_b2;
	temp_b2->q_tail = temp_q2->before;


	if (temp_q2 != temp_b2->q_head)
	{
		temp_q2->before->next = NULL;
		temp_q2->before = temp_q2;
	}
	else
	{
		temp_b2->before->next = temp_b3;
		temp_b3->before = temp_b2->before;
	}

	if (temp_q3->next != NULL)
	{
		temp_b3->q_head = temp_q3->next;
		temp_q3->next->before = temp_q3->next;
		temp_q3->next = NULL;
	}
	else
	{
		temp_b3->before->next = NULL;
		temp_b = NULL;
	}

	head = new_bhead;

}

void code_seg(block* head)
{
	FILE* fp;
	fp = fopen("result.txt", "a");
	fprintf(fp, "CSEG SEGMENT\nASSUME CS:CSEG,DS:DSEG,SS:SSEG\n\n");
	printf("CSEG SEGMENT\nASSUME CS:CSEG,DS:DSEG,SS:SSEG\n\n");
	int mark = 0;	//段计数

	block* fend_b = head;
	quat* fend_q = fend_b->q_head;
	while (fend_b->next != NULL)
	{
		fend_q = fend_b->q_head;
		while (fend_q != NULL)
		{
			fend_q = fend_q->next;
		}
		if (fend_q == NULL)
			fend_b = fend_b->next;
		if (fend_b->next == NULL)
			break;
	}
	fend_q = fend_b->q_tail;


	while (fend_b != head)
	{
		fend_q = fend_b->q_head;
		while (fend_q != NULL)
		{
			if (strcmp(fend_q->Q[0], "f") == 0 && strcmp(fend_q->Q[1], "end") == 0)
				break;
			fend_q = fend_q->next;
		}
		if (fend_q == NULL)
		{
			fend_b = fend_b->before;
			continue;
		}
		if (strcmp(fend_q->Q[0], "f") == 0 && strcmp(fend_q->Q[1], "end") == 0)
			break;
	}

	if (fend_b->next != NULL && fend_q == fend_b->q_tail)
	{
		fend_b = fend_b->next;
		fend_q = fend_b->q_head;
	}


	block* cur_block = head;
	quat* cur_quat = cur_block->q_head;

	while (cur_block != NULL)
	{
		if (cur_quat == fend_q && strcmp(fend_q->Q[1], "end") != 0)
		{
			fprintf(fp, "\nSTART:MOV AX,DSEG\nMOV DS,AX\nMOV AX,SSEG\nMOV SS,AX\n\n");
			printf("\nSTART:MOV AX,DSEG\nMOV DS,AX\nMOV AX,SSEG\nMOV SS,AX\n\n");
		}
		else if (cur_quat == fend_q && strcmp(fend_q->Q[1], "end") == 0)
		{
			if (strcmp(fend_q->before->Q[0], "return") != 0)
			{
				fprintf(fp, "RET\n");
				printf("RET\n");
			}
			fprintf(fp, "%s ENDP\n", cur_quat->Q[2]);
			printf("%s ENDP\n", cur_quat->Q[2]);
			fprintf(fp, "\nSTART:MOV AX,DSEG\nMOV DS,AX\nMOV AX,SSEG\nMOV SS,AX\n\n");
			printf("\nSTART:MOV AX,DSEG\nMOV DS,AX\nMOV AX,SSEG\nMOV SS,AX\n\n");
			fprintf(fp, "CALL main\n");
			printf("CALL main\n");
			break;
		}
		if (strcmp(cur_quat->Q[0], "=") == 0)
		{
			if (cur_quat->next != NULL && strcmp(cur_quat->next->Q[0], "for") == 0)
			{
				int temp = atoi(cur_quat->Q[1]);
				memset(cur_quat->Q[1], '\0', sizeof(cur_quat->Q[1]));
				itoa(temp - 1, cur_quat->Q[1], 10);
			}

			if (cur_quat->Q[1][0] == 't' && cur_quat->Q[1][1] >= '0' && cur_quat->Q[1][2] <= '9')	//若Q[1]为临时变量
			{
				if (cur_quat->Q[3][0] != '(')	//不是数组
				{
					fprintf(fp, "MOV %s,AX\n", cur_quat->Q[3]);
					printf("MOV %s,AX\n", cur_quat->Q[3]);
				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[3][j] != '|')
					{
						name[j - 1] = cur_quat->Q[1][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[3][j] != ')')
					{
						num[k] = cur_quat->Q[1][j];
						k++;
						j++;
					}

					if (strcmp(num, cur_quat->before->Q[3]) != 0)
					{
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV %s[SI],AX\n", num, name);
						printf("MOV SI,%s\nADD SI,SI\nMOV %s[SI],AX\n", num, name);
					}
					else
					{
						fprintf(fp, "MOV SI,AX\nADD SI,SI\nMOV %s[SI],AX\n", name);
						printf("MOV SI,AX\nADD SI,SI\nMOV %s[SI],AX\n", name);

					}

				}
			}
			else
			{
				if (cur_quat->Q[1][0] != '(')
				{
					if (cur_quat->Q[3][0] != '(')	//不是数组
					{
						fprintf(fp, "MOV AX,%s\nMOV %s,AX\n", cur_quat->Q[1], cur_quat->Q[3]);
						printf("MOV AX,%s\nMOV %s,AX\n", cur_quat->Q[1], cur_quat->Q[3]);
					}
					else
					{
						char name[30] = { 0 };
						char num[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[3][j] != '|')
						{
							name[j - 1] = cur_quat->Q[3][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[3][j] != ')')
						{
							num[k] = cur_quat->Q[3][j];
							k++;
							j++;
						}

						if (strcmp(num, cur_quat->before->Q[3]) != 0)
						{
							fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV %s[SI],AX\n", cur_quat->Q[1], num, name);
							printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV %s[SI],AX\n", cur_quat->Q[1], num, name);
						}
						else
						{
							fprintf(fp, "MOV BX,%s\nMOV SI,AX\nADD SI,SI\nMOV %s[SI],BX\n", cur_quat->Q[1], name);
							printf("MOV BX,%s\nMOV SI,AX\nADD SI,SI\nMOV %s[SI],BX\n", cur_quat->Q[1], name);
						}

					}

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[3][j] != ')' && cur_quat->Q[1][j] != '|')
					{
						name[j - 1] = cur_quat->Q[1][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[1][j] != ')')
					{
						num[k] = cur_quat->Q[1][j];
						k++;
						j++;
					}

					if (cur_quat->Q[3][0] != '(')	//不是数组
					{
						if (strcmp(cur_quat->before->Q[3], num) != 0)
						{
							fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV %s,AX\n", num, name, cur_quat->Q[3]);
							printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV %s,AX\n", num, name, cur_quat->Q[3]);
						}
						else
						{
							fprintf(fp, "MOV SI,AX\nADD SI,SI\nMOV BX,%s[SI]\nMOV %s,BX\n", name, cur_quat->Q[3]);
							printf("MOV SI,AX\nADD SI,SI\nMOV BX,%s[SI]\nMOV %s,BX\n", name, cur_quat->Q[3]);
						}
					}
					else   //都是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[3][j] != ')' && cur_quat->Q[3][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[3][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[3][j] != ')')
						{
							num1[k] = cur_quat->Q[3][j];
							k++;
							j++;
						}

						if (strcmp(num, cur_quat->before->Q[3]) != 0)
						{
							fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name);
							printf("MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name);
						}
						else
						{
							fprintf(fp, "MOV SI,AX\nADD SI,SI\nMOV BX,%s[SI]\n", name);
							printf("MOV SI,AX\nADD SI,SI\nMOV BX,%s[SI]\n", name);
						}

						if (strcmp(num1, cur_quat->before->Q[3]) != 0)
						{
							fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV %s[SI],BX\n", num1, name1);
							printf("MOV SI,%s\nADD SI,SI\nMOV %s[SI],BX\n", num1, name1);
						}
						else
						{
							fprintf(fp, "MOV SI,AX\nADD SI,SI\nMOV %s[SI],BX\n", name1);
							printf("MOV SI,AX\nADD SI,SI\nMOV %s[SI],BX\n", name1);
						}
					}

				}

			}

		}
		else if (strcmp(cur_quat->Q[0], ">") == 0 || strcmp(cur_quat->Q[0], ">=") == 0 || strcmp(cur_quat->Q[0], "<") == 0 || strcmp(cur_quat->Q[0], "<=") == 0 || strcmp(cur_quat->Q[0], "==") == 0)
		{
			int mark_tt = mark;
			if (strcmp(cur_quat->next->Q[0], "if") == 0 || strcmp(cur_quat->next->Q[0], "elif") == 0)
			{
				int mark_t = mark;
				block* bt = cur_block->next;
				quat* qt = bt->q_head;
				while (bt != NULL)
				{
					qt = bt->q_head;
					while (qt != NULL)
					{
						if (strcmp(qt->Q[0], "else") == 0 || strcmp(qt->Q[0], "elif") == 0 || strcmp(qt->Q[0], "if") == 0)
							break;
						else
						{
							qt = qt->next;
						}
					}

					if (qt == NULL)
					{
						mark_t++;
						bt = bt->next;
					}
					else
						break;
				}
				if (qt->Q[0] != NULL && strcmp(qt->Q[0], "if") != 0)
				{
					if (bt != NULL)
						mark = mark_t;
				}


				if (cur_quat->Q[1][0] != '(')
				{
					if (cur_quat->Q[2][0] != '(')	//不是数组
					{
						fprintf(fp, "MOV AX,%s\nMOV BX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
						printf("MOV AX,%s\nMOV BX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
					}
					else
					{
						char name[30] = { 0 };
						char num[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[3][j] != ')' && cur_quat->Q[2][j] != '|')
						{
							name[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						if (cur_quat->Q[3][j] == ')')
						{
							num[k] = cur_quat->Q[1][j];
							k++;
							j++;
						}
						else
						{
							while (cur_quat->Q[2][j] != ')')
							{
								num[k] = cur_quat->Q[2][j];
								k++;
								j++;
							}
						}

						fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", cur_quat->Q[1], num, name);
						printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", cur_quat->Q[1], num, name);
					}

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[1][j] != '|')
					{
						name[j - 1] = cur_quat->Q[1][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[1][j] != ')')
					{
						num[k] = cur_quat->Q[1][j];
						k++;
						j++;
					}
					if (cur_quat->Q[2][0] != '(')	//不是数组
					{
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\n", num, name, cur_quat->Q[2]);
						printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\n", num, name, cur_quat->Q[2]);

					}
					else   //都是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[2][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[2][j] != ')')
						{
							num1[k] = cur_quat->Q[2][j];
							k++;
							j++;
						}
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name, num1, name1);
						printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name, num1, name1);
					}
				}

				if (strcmp(cur_quat->Q[0], "<") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJGE block%d\n", mark + 1);
					printf("CMP AX,BX\nJGE block%d\n", mark + 1);
				}
				else if (strcmp(cur_quat->Q[0], "<=") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJG block%d\n", mark + 1);
					printf("CMP AX,BX\nJG block%d\n", mark + 1);
				}
				else if (strcmp(cur_quat->Q[0], ">") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJLE block%d\n", mark + 1);
					printf("CMP AX,BX\nJLE block%d\n", mark + 1);
				}
				else if (strcmp(cur_quat->Q[0], ">=") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJL block%d\n", mark + 1);
					printf("CMP AX,BX\nJL block%d\n", mark + 1);
				}
				else if (strcmp(cur_quat->Q[0], "==") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJNE block%d\n", mark + 1);
					printf("CMP AX,BX\nJNE block%d\n", mark + 1);
				}
				else if (strcmp(cur_quat->Q[0], "!=") == 0)
				{
					fprintf(fp, "CMP AX,BX\nJE block%d\n", mark + 1);
					printf("CMP AX,BX\nJE block%d\n", mark + 1);
				}
				cur_quat = cur_quat->next;
			}
			mark = mark_tt;
		}
		else if (strcmp(cur_quat->Q[0], "else") == 0)
		{
			cur_quat = cur_quat->next;
			continue;
		}
		else if (strcmp(cur_quat->Q[0], "ie") == 0)
		{
			int mark_tt = mark;
			int mark_t = mark;
			block* bt = cur_block->next;
			quat* qt = cur_quat;
			while (bt != NULL)
			{
				qt = bt->q_head;
				while (qt != NULL)
				{
					if (strcmp(qt->Q[0], "if") == 0 || strcmp(qt->Q[0], "else") == 0)
						break;
					qt = qt->next;
				}
				if (qt == NULL)
				{
					mark_t++;
					bt = bt->next;
					continue;
				}
				if (strcmp(qt->Q[0], "if") == 0 || strcmp(qt->Q[0], "else") == 0)
					break;
			}
			if (bt != NULL && strcmp(qt->Q[0], "else") == 0)
			{
				//mark = mark_t + 1;
				fprintf(fp, "JMP block%d\n", mark_t + 2);
				printf("JMP block%d\n", mark_t + 2);
				mark = mark_tt;
			}

			if (cur_quat->next == NULL)
			{
				mark++;
				fprintf(fp, "block%d:", mark);
				printf("block%d:", mark);
				cur_block = cur_block->next;

				cur_quat = cur_block->q_head;
				continue;
			}
			else
				cur_quat = cur_quat->next;
		}
		else if (strcmp(cur_quat->Q[0], "for") == 0 || strcmp(cur_quat->Q[0], "while") == 0)
		{
			if (strcmp(cur_quat->Q[0], "for") == 0)
			{
				block* b = cur_block->next;
				quat* q = b->q_head;
				if (q->Q[2][0] <= '0' || q->Q[2][0] >= '9')
				{
					fprintf(fp, "MOV BX,%s\nDEC BX\nMOV %s,BX\n", q->Q[2], q->Q[2]);
					printf("MOV BX,%s\nDEC BX\nMOV %s,BX\n", q->Q[2], q->Q[2]);
				}
				else
				{
					int temp = atoi(q->Q[2]);
					memset(q->Q[2], '0', sizeof(q->Q[2]));
					itoa(temp, q->Q[2], 10);
				}
			}

			mark++;
			
			if (strcmp(cur_quat->Q[0], "while") == 0)
			{
				int i = 0;
				for (i = 0; i < 20; i++)
				{
					if (count_while[i] == 0)
						break;
				}
				count_while[i] = mark;
			}
			else if (strcmp(cur_quat->Q[0], "for") == 0)
			{
				int i = 0;
				for (i = 0; i < 20; i++)
				{
					if (count_for[i] == 0)
						break;
				}
				count_for[i] = mark;
			}
			fprintf(fp, "block%d:", mark);
			printf("block%d:", mark);
			cur_block = cur_block->next;
			cur_quat = cur_block->q_head;

			if (cur_quat->Q[1][0] != '(')
			{
				if (cur_quat->Q[2][0] != '(')	//不是数组
				{
					fprintf(fp, "MOV AX,%s\nMOV BX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
					printf("MOV AX,%s\nMOV BX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", cur_quat->Q[1], num, name);
					printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", cur_quat->Q[1], num, name);
				}

			}
			else
			{
				char name[30] = { 0 };
				char num[20] = { 0 };
				int j = 1;
				while (cur_quat->Q[1][j] != '|')
				{
					name[j - 1] = cur_quat->Q[1][j];
					j++;
				}
				j++;
				int k = 0;
				while (cur_quat->Q[1][j] != ')')
				{
					num[k] = cur_quat->Q[1][j];
					k++;
					j++;
				}
				if (cur_quat->Q[2][0] != '(')	//不是数组
				{
					fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\n", num, name, cur_quat->Q[2]);
					printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\n", num, name, cur_quat->Q[2]);
				}
				else   //都是数组
				{
					char name1[30] = { 0 };
					char num1[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name1[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num1[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name, num1, name1);
					printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\n", num, name, num1, name1);
				}
			}

			if (strcmp(cur_quat->Q[0], "<") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJGE block%d\n", mark + 1);
				printf("CMP AX,BX\nJGE block%d\n", mark + 1);
			}
			else if (strcmp(cur_quat->Q[0], "<=") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJG block%d\n", mark + 1);
				printf("CMP AX,BX\nJG block%d\n", mark + 1);
			}
			else if (strcmp(cur_quat->Q[0], ">") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJLE block%d\n", mark + 1);
				printf("CMP AX,BX\nJLE block%d\n", mark + 1);
			}
			else if (strcmp(cur_quat->Q[0], ">=") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJL block%d\n", mark + 1);
				printf("CMP AX,BX\nJL block%d\n", mark + 1);
			}
			else if (strcmp(cur_quat->Q[0], "==") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJNE block%d\n", mark + 1);
				printf("CMP AX,BX\nJNE block%d\n", mark + 1);
			}
			else if (strcmp(cur_quat->Q[0], "!=") == 0)
			{
				fprintf(fp, "CMP AX,BX\nJE block%d\n", mark + 1);
				printf("CMP AX,BX\nJE block%d\n", mark + 1);
			}

		}
		else if (strcmp(cur_quat->Q[0], "fe") == 0 || strcmp(cur_quat->Q[0], "we") == 0)
		{

			if (mark > 0)
			{
				if (strcmp(cur_quat->Q[0], "fe") == 0)
				{

					int i = 19;
					for (i; i >= 0; i--)
					{
						
						if (count_for[i] != 0)
							break;
					}
					fprintf(fp, "JMP block%d\n", count_for[i]);
					printf("JMP block%d\n", count_for[i]);
					count_for[i] = 0;
				}
				else if (strcmp(cur_quat->Q[0], "we") == 0)
				{
					
					int i = 19;
					for (i; i >= 0; i--)
					{
						if (count_while[i] != 0)
							break;
					}
					fprintf(fp, "JMP block%d\n", count_while[i]);
					printf("JMP block%d\n", count_while[i]);
					count_while[i] = 0;
				}
			}
		}
		else if (strcmp(cur_quat->Q[0], "+") == 0)
		{
			int i = 0;
			if (cur_quat->Q[1][0] >= '0' && cur_quat->Q[1][0] <= '9')
			{
				if (cur_quat->Q[2][0] != '(')	//都不是数组
				{
					fprintf(fp, "MOV AX,%s\nADD AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
					printf("MOV AX,%s\nADD AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", cur_quat->Q[1], num, name);
					printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", cur_quat->Q[1], num, name);
				}
			}
			else if (cur_quat->Q[2][0] == 't' && cur_quat->Q[2][1] >= '0' && cur_quat->Q[2][1] <= '9')
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							fprintf(fp, "ADD AX,%s\n", cur_quat->Q[1]);
							printf("ADD AX,%s\n", cur_quat->Q[1]);
							break;

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							fprintf(fp, "MOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num, name);
							printf("MOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num, name);
							break;
						}

					}
					i++;
				}

			}
			else
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							if (cur_quat->Q[2][0] != '(')	//都不是数组
							{
								fprintf(fp, "MOV AX,%s\nADD AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
								printf("MOV AX,%s\nADD AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
								break;

							}
							else
							{
								char name[30] = { 0 };
								char num[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", cur_quat->Q[1], num, name);
								printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", cur_quat->Q[1], num, name);
								break;
							}

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							if (cur_quat->Q[2][0] != '(')	//不是数组
							{
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nADD AX,%s\n", num, name, cur_quat->Q[2]);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nADD AX,%s\n", num, name, cur_quat->Q[2]);
								break;
							}
							else   //都是数组
							{
								char name1[30] = { 0 };
								char num1[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name1[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num1[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num, name, num1, name1);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num, name, num1, name1);
								break;
							}
						}

					}
					i++;
				}
				if (strcmp(biaoshifu[i], "") == 0)
				{
					if (cur_quat->Q[2][0] != '(')
					{
						fprintf(fp, "ADD AX,%s\n", cur_quat->Q[2]);
						printf("ADD AX,%s\n", cur_quat->Q[2]);
					}
					else   //是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[2][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[2][j] != ')')
						{
							num1[k] = cur_quat->Q[2][j];
							k++;
							j++;
						}
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num1, name1);
						printf("MOV SI,%s\nADD SI,SI\nADD AX,%s[SI]\n", num1, name1);
					}
				}

			}


		}
		else if (strcmp(cur_quat->Q[0], "-") == 0)
		{
			int i = 0;
			if (cur_quat->Q[1][0] >= '0' && cur_quat->Q[1][0] <= '9')
			{
				if (cur_quat->Q[2][0] != '(')	//都不是数组
				{
					fprintf(fp, "MOV AX,%s\nSUB AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
					printf("MOV AX,%s\nSUB AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", cur_quat->Q[1], num, name);
					printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", cur_quat->Q[1], num, name);
				}
			}
			else if (cur_quat->Q[2][0] == 't' && cur_quat->Q[2][1] >= '0' && cur_quat->Q[2][1] <= '9')
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							fprintf(fp, "SUB AX,%s\n", cur_quat->Q[1]);
							printf("SUB AX,%s\n", cur_quat->Q[1]);
							break;

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							fprintf(fp, "MOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num, name);
							printf("MOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num, name);
							break;
						}

					}
					i++;
				}

			}
			else
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							if (cur_quat->Q[2][0] != '(')	//都不是数组
							{
								fprintf(fp, "MOV AX,%s\nSUB AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
								printf("MOV AX,%s\nSUB AX,%s\n", cur_quat->Q[1], cur_quat->Q[2]);
								break;

							}
							else
							{
								char name[30] = { 0 };
								char num[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", cur_quat->Q[1], num, name);
								printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", cur_quat->Q[1], num, name);
								break;
							}

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							if (cur_quat->Q[2][0] != '(')	//不是数组
							{
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nSUB AX,%s\n", num, name, cur_quat->Q[2]);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nSUB AX,%s\n", num, name, cur_quat->Q[2]);
								break;
							}
							else   //都是数组
							{
								char name1[30] = { 0 };
								char num1[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name1[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num1[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num, name, num1, name1);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num, name, num1, name1);
								break;
							}
						}

					}
					i++;
				}
				if (strcmp(biaoshifu[i], "") == 0)
				{
					if (cur_quat->Q[2][0] != '(')
					{
						fprintf(fp, "SUB AX,%s\n", cur_quat->Q[2]);
						printf("SUB AX,%s\n", cur_quat->Q[2]);
					}
					else   //是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[2][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[2][j] != ')')
						{
							num1[k] = cur_quat->Q[2][j];
							k++;
							j++;
						}
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num1, name1);
						printf("MOV SI,%s\nADD SI,SI\nSUB AX,%s[SI]\n", num1, name1);
					}
				}
			}

		}
		else if (strcmp(cur_quat->Q[0], "*") == 0)
		{
			int i = 0;
			if (cur_quat->Q[1][0] >= '0' && cur_quat->Q[1][0] <= '9')
			{
				if (cur_quat->Q[2][0] != '(')	//都不是数组
				{
					fprintf(fp, "MOV AX,%s\nIMUL %s\n", cur_quat->Q[1], cur_quat->Q[2]);
					printf("MOV AX,%s\nIMUL %s\n", cur_quat->Q[1], cur_quat->Q[2]);

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nIMUL %s[SI]\n", cur_quat->Q[1], num, name);
					printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nIMUL %s[SI]\n", cur_quat->Q[1], num, name);
				}
			}
			else if (cur_quat->Q[2][0] == 't' && cur_quat->Q[2][1] >= '0' && cur_quat->Q[2][1] <= '9')
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							fprintf(fp, "MOV BX,AX\nMOV AX,%s\nIMUL BL\n", cur_quat->Q[1]);
							printf("MOV BX,AX\nMOV AX,%s\nIMUL BL\n", cur_quat->Q[1]);
							break;
						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							fprintf(fp, "MOV AX,BX\nMOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nIMUL BL\n", num, name);
							printf("MOV AX,BX\nMOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nIMUL BL\n", num, name);
							break;
						}

					}
					i++;
				}

			}
			else
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							if (cur_quat->Q[2][0] != '(')	//都不是数组
							{
								fprintf(fp, "MOV AX,%s\nMOV BX,%s\nIMUL BL\n", cur_quat->Q[1], cur_quat->Q[2]);
								printf("MOV AX,%s\nMOV BX,%s\nIMUL BL\n", cur_quat->Q[1], cur_quat->Q[2]);
								break;

							}
							else
							{
								char name[30] = { 0 };
								char num[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", cur_quat->Q[1], num, name);
								printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", cur_quat->Q[1], num, name);
								break;
							}

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							if (cur_quat->Q[2][0] != '(')	//不是数组
							{
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\nIMUL BL\n", num, name, cur_quat->Q[2]);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\nIMUL BL\n", num, name, cur_quat->Q[2]);
								break;
							}
							else   //都是数组
							{
								char name1[30] = { 0 };
								char num1[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name1[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num1[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", num, name, num1, name1);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", num, name, num1, name1);
								break;
							}
						}

					}
					i++;
				}
				if (strcmp(biaoshifu[i], "") == 0)
				{
					if (cur_quat->Q[2][0] != '(')
					{
						fprintf(fp, "MOV BX,%s\nIMUL BL\n", cur_quat->Q[2]);
						printf("MOV BX,%s\nIMUL BL\n", cur_quat->Q[2]);
					}
					else   //是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[2][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[2][j] != ')')
						{
							num1[k] = cur_quat->Q[2][j];
							k++;
							j++;
						}
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", num1, name1);
						printf("MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIMUL BL\n", num1, name1);
					}
				}
			}

		}
		else if (strcmp(cur_quat->Q[0], "/") == 0)
		{
			int i = 0;
			if (cur_quat->Q[1][0] >= '0' && cur_quat->Q[1][0] <= '9')
			{
				if (cur_quat->Q[2][0] != '(')	//都不是数组
				{
					fprintf(fp, "MOV AX,%s\nIDIV %s\n", cur_quat->Q[1], cur_quat->Q[2]);
					printf("MOV AX,%s\nIDIV %s\n", cur_quat->Q[1], cur_quat->Q[2]);

				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[2][j] != '|')
					{
						name[j - 1] = cur_quat->Q[2][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[2][j] != ')')
					{
						num[k] = cur_quat->Q[2][j];
						k++;
						j++;
					}
					fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nIDIV %s[SI]\n", cur_quat->Q[1], num, name);
					printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nIDIV %s[SI]\n", cur_quat->Q[1], num, name);
				}
			}
			else if (cur_quat->Q[2][0] == 't' && cur_quat->Q[2][1] >= '0' && cur_quat->Q[2][1] <= '9')
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							fprintf(fp, "MOV BX,AX\nMOV AX,%s\nIDIV BL\n", cur_quat->Q[1]);
							printf("MOV BX,AX\nMOV AX,%s\nIDIV BL\n", cur_quat->Q[1]);
							break;
						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							fprintf(fp, "MOV AX,BX\nMOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nIDIV BL\n", num, name);
							printf("MOV AX,BX\nMOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nIDIV BL\n", num, name);
							break;
						}

					}
					i++;
				}

			}
			else
			{
				while (strcmp(biaoshifu[i], ""))
				{
					if (strcmp(biaoshifu[i], cur_quat->Q[1]) == 0)
					{
						if (cur_quat->Q[1][0] != '(')
						{
							if (cur_quat->Q[2][0] != '(')	//都不是数组
							{
								fprintf(fp, "MOV AX,%s\nMOV BX,%s\nIDIV BL\n", cur_quat->Q[1], cur_quat->Q[2]);
								printf("MOV AX,%s\nMOV BX,%s\nIDIV BL\n", cur_quat->Q[1], cur_quat->Q[2]);
								break;

							}
							else
							{
								char name[30] = { 0 };
								char num[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", cur_quat->Q[1], num, name);
								printf("MOV AX,%s\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", cur_quat->Q[1], num, name);
								break;
							}

						}
						else
						{
							char name[30] = { 0 };
							char num[20] = { 0 };
							int j = 1;
							while (cur_quat->Q[1][j] != '|')
							{
								name[j - 1] = cur_quat->Q[1][j];
								j++;
							}
							j++;
							int k = 0;
							while (cur_quat->Q[1][j] != ')')
							{
								num[k] = cur_quat->Q[1][j];
								k++;
								j++;
							}

							if (cur_quat->Q[2][0] != '(')	//不是数组
							{
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\nIDIV BL\n", num, name, cur_quat->Q[2]);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV BX,%s\nIDIV BL\n", num, name, cur_quat->Q[2]);
								break;
							}
							else   //都是数组
							{
								char name1[30] = { 0 };
								char num1[20] = { 0 };
								int j = 1;
								while (cur_quat->Q[2][j] != '|')
								{
									name1[j - 1] = cur_quat->Q[2][j];
									j++;
								}
								j++;
								int k = 0;
								while (cur_quat->Q[2][j] != ')')
								{
									num1[k] = cur_quat->Q[2][j];
									k++;
									j++;
								}
								fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", num, name, num1, name1);
								printf("MOV SI,%s\nADD SI,SI\nMOV AX,%s[SI]\nMOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", num, name, num1, name1);
								break;
							}
						}

					}
					i++;
				}
				if (strcmp(biaoshifu[i], "") == 0)
				{
					if (cur_quat->Q[2][0] != '(')
					{
						fprintf(fp, "MOV BX,%s\nIDIV BL\n", cur_quat->Q[2]);
						printf("MOV BX,%s\nIDIV BL\n", cur_quat->Q[2]);
					}
					else   //是数组
					{
						char name1[30] = { 0 };
						char num1[20] = { 0 };
						int j = 1;
						while (cur_quat->Q[2][j] != '|')
						{
							name1[j - 1] = cur_quat->Q[2][j];
							j++;
						}
						j++;
						int k = 0;
						while (cur_quat->Q[2][j] != ')')
						{
							num1[k] = cur_quat->Q[2][j];
							k++;
							j++;
						}
						fprintf(fp, "MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", num1, name1);
						printf("MOV SI,%s\nADD SI,SI\nMOV BX,%s[SI]\nIDIV BL\n", num1, name1);
					}
				}

			}

		}
		else if (strcmp(cur_quat->Q[0], "++") == 0)
		{
			if (cur_quat->Q[1][0] != '(')
			{
				fprintf(fp, "INC %s\n", cur_quat->Q[1]);
				printf("INC %s\n", cur_quat->Q[1]);
			}
			else
			{
				char name1[30] = { 0 };
				char num1[20] = { 0 };
				int j = 1;
				while (cur_quat->Q[1][j] != '|')
				{
					name1[j - 1] = cur_quat->Q[1][j];
					j++;
				}
				j++;
				int k = 0;
				while (cur_quat->Q[1][j] != ')')
				{
					num1[k] = cur_quat->Q[1][j];
					k++;
					j++;
				}
				fprintf(fp, "MOV SI,%s\nADD SI,SI\nINC %s[SI]\n", num1, name1);
				printf("MOV SI,%s\nADD SI,SI\nINC %s[SI]\n", num1, name1);
			}
		}
		else if (strcmp(cur_quat->Q[0], "--") == 0)
		{
			if (cur_quat->Q[1][0] != '(')
			{
				fprintf(fp, "DEC %s\n", cur_quat->Q[1]);
				printf("DEC %s\n", cur_quat->Q[1]);
			}
			else
			{
				char name1[30] = { 0 };
				char num1[20] = { 0 };
				int j = 1;
				while (cur_quat->Q[1][j] != '|')
				{
					name1[j - 1] = cur_quat->Q[1][j];
					j++;
				}
				j++;
				int k = 0;
				while (cur_quat->Q[1][j] != ')')
				{
					num1[k] = cur_quat->Q[1][j];
					k++;
					j++;
				}
				fprintf(fp, "MOV SI,%s\nADD SI,SI\nDEC %s[SI]\n", num1, name1);
				printf("MOV SI,%s\nADD SI,SI\nDEC %s[SI]\n", num1, name1);
			}
		}
		else if (strcmp(cur_quat->Q[0], "f") == 0)	//函数
		{
			if (strcmp(cur_quat->Q[1], "end") != 0)
			{
				fprintf(fp, "%s PROC NEAR\n", cur_quat->Q[1]);
				printf("%s PROC NEAR\n", cur_quat->Q[1]);
			}
			else
			{
				if (strcmp(cur_quat->before->Q[0], "return") != 0)
				{
					fprintf(fp, "RET\n");
					printf("RET\n");
				}
				fprintf(fp, "%s ENDP\n\n", cur_quat->Q[2]);
				printf("%s ENDP\n\n", cur_quat->Q[2]);
			}

			if (strcmp(cur_quat->Q[1], "end") != 0)
			{
				block* temp_endb = cur_block;
				quat* temp_endq = cur_quat;
				while (temp_endb != NULL)
				{

					temp_endq = temp_endb->q_head;
					while (temp_endq != NULL)
					{
						if (strcmp(temp_endq->Q[1], "end") == 0)
						{
							break;
						}
						temp_endq = temp_endq->next;
					}
					if (temp_endq == NULL)
					{
						temp_endb = temp_endb->next;
						continue;
					}
					if (strcmp(temp_endq->Q[1], "end") == 0)
						break;
				}

				block* tempb_digui = cur_block;
				quat* tempq_digui = cur_quat;

				while (tempb_digui != temp_endb->next)
				{
					tempq_digui = tempb_digui->q_head;
					while (tempq_digui != NULL)
					{
						if (strcmp(tempq_digui->Q[0], "call") == 0 && strcmp(tempq_digui->Q[1], cur_quat->Q[1]) == 0)
						{
							break;
						}
						tempq_digui = tempq_digui->next;
					}
					if (tempq_digui == NULL)
					{
						tempb_digui = tempb_digui->next;
						continue;
					}

					if (strcmp(tempq_digui->Q[0], "call") == 0 && strcmp(tempq_digui->Q[1], cur_quat->Q[1]) == 0)
					{
						break;
					}

				}

				if (tempq_digui != NULL && strcmp(tempq_digui->Q[0], "call") == 0 && strcmp(tempq_digui->Q[1], cur_quat->Q[1]) == 0)
				{
					strcpy(digui[digui_num], cur_quat->Q[1]);
					digui_num++;

					block* temp_b = head;
					quat* temp_q = temp_b->q_head;
					while (temp_b != NULL)
					{
						temp_q = temp_b->q_head;
						while (temp_q != NULL)
						{
							if (strcmp(temp_q->Q[0], "declarP") == 0 && strcmp(temp_q->Q[1], cur_quat->Q[1]) == 0)
							{
								break;
							}
							temp_q = temp_q->next;
						}
						if (temp_q == NULL)
						{
							temp_b = temp_b->next;
							continue;
						}

						if (strcmp(temp_q->Q[0], "declarP") == 0 && strcmp(temp_q->Q[1], cur_quat->Q[1]) == 0)
						{
							break;
						}
					}

					char a[5][20] = { 0 };
					int k = 0;
					int i = 1;
					char c;
					c = temp_q->Q[3][1];
					while (c != ')')
					{
						int j = 0;

						while (c != '|' && c != ')')
						{
							a[k][j] = c;
							i++;
							j++;
							c = temp_q->Q[3][i];
							if (c == '|' || c == ')')
							{
								break;
							}

						}
						k++;

						if (c == ')')
						{
							break;
						}

						i++;
						c = temp_q->Q[3][i];
					}

					for (int n = 0; n < k; n++)
					{
						fprintf(fp, "PUSH %s\n", a[n]);
						printf("PUSH %s\n", a[n]);
					}
				}
			}

		}
		else if (strcmp(cur_quat->Q[0], "return") == 0)
		{
			block* temp_endb = cur_block;
			quat* temp_endq = cur_quat;
			while (temp_endb != NULL)
			{
				temp_endq = temp_endb->q_head;
				while (temp_endq != NULL)
				{
					if (strcmp(temp_endq->Q[1], "end") == 0)
					{
						break;
					}
					temp_endq = temp_endq->next;
				}
				if (temp_endq == NULL)
				{
					temp_endb = temp_endb->next;
					continue;
				}
				if (strcmp(temp_endq->Q[1], "end") == 0)
					break;
			}
			char ret[30] = { 0 };
			strcpy(ret, temp_endq->Q[2]);
			strcat(ret, "_ret");
			if (cur_quat->Q[1][0] == 't' && cur_quat->Q[1][1] >= '0' && cur_quat->Q[1][1] <= '9')
			{
				fprintf(fp, "MOV %s,AX\n", ret);
				printf("MOV %s,AX\n", ret);
			}
			else if (strcmp(cur_quat->Q[1], "_") == 0)
			{
				fprintf(fp, "MOV CX,0\nMOV %s,CX\n", ret);
				printf("MOV CX,0\nMOV %s,CX\n", ret);
			}
			else
			{
				fprintf(fp, "MOV CX,%s\nMOV %s,CX\n", cur_quat->Q[1], ret);
				printf("MOV CX,%s\nMOV %s,CX\n", cur_quat->Q[1], ret);
			}

			for (int i = 0; i < digui_num; i++)
			{
				if (strcmp(temp_endq->Q[2], digui[i]) == 0)
				{
					block* temp_b = head;
					quat* temp_q = temp_b->q_head;
					while (temp_b != NULL)
					{
						temp_q = temp_b->q_head;
						while (temp_q != NULL)
						{
							if (strcmp(temp_q->Q[0], "declarP") == 0 && strcmp(temp_q->Q[1], temp_endq->Q[2]) == 0)
							{
								break;
							}
							temp_q = temp_q->next;
						}
						if (temp_q == NULL)
						{
							temp_b = temp_b->next;
							continue;
						}

						if (strcmp(temp_q->Q[0], "declarP") == 0 && strcmp(temp_q->Q[1], temp_endq->Q[2]) == 0)
						{
							break;
						}
					}

					char a[5][20] = { 0 };
					int k = 0;
					int i = 1;
					char c;
					c = temp_q->Q[3][1];
					while (c != ')')
					{
						int j = 0;

						while (c != '|' && c != ')')
						{
							a[k][j] = c;
							i++;
							j++;
							c = temp_q->Q[3][i];
							if (c == '|' || c == ')')
							{
								break;
							}

						}
						k++;

						if (c == ')')
						{
							break;
						}

						i++;
						c = temp_q->Q[3][i];
					}
					if (strcmp(cur_quat->next->Q[1], "end") != 0)
					{
						for (int j = k; j > 0; j--)
						{
							fprintf(fp, "POP %s\n", a[j - 1]);
							printf("POP %s\n", a[j - 1]);
						}
					}
				}
			}
			fprintf(fp, "RET\n");
			printf("RET\n");
		}
		else if (strcmp(cur_quat->Q[0], "call") == 0)
		{
			if (strcmp(cur_quat->Q[3], "_") == 0)
			{
				fprintf(fp, "CALL %s\n", cur_quat->Q[1]);
				printf("CALL %s\n", cur_quat->Q[1]);
			}
			else
			{

				block* temp_bsta = head;
				quat* temp_qsta = temp_bsta->q_head;

				char a[5][20] = { 0 };	//参数
				char b[5][20] = { 0 };	//传值

				while (temp_bsta != NULL)
				{
					temp_qsta = temp_bsta->q_head;
					while (temp_qsta != NULL)
					{

						if (strcmp(temp_qsta->Q[0], "declarP") == 0 && strcmp(temp_qsta->Q[1], cur_quat->Q[1]) == 0)
						{
							break;
						}
						temp_qsta = temp_qsta->next;
					}
					if (temp_qsta == NULL)
					{
						temp_bsta = temp_bsta->next;
						continue;
					}

					if (strcmp(temp_qsta->Q[0], "declarP") == 0 && strcmp(temp_qsta->Q[1], cur_quat->Q[1]) == 0)
					{
						break;
					}
				}

				int k = 0;
				int i = 1;
				char c;
				c = temp_qsta->Q[3][1];
				while (c != ')')
				{
					int j = 0;

					while (c != '|' && c != ')')
					{
						a[k][j] = c;
						i++;
						j++;
						c = temp_qsta->Q[3][i];
						if (c == '|' || c == ')')
						{
							break;
						}

					}
					k++;

					if (c == ')')
					{
						break;
					}

					i++;
					c = temp_qsta->Q[3][i];
				}

				int k2 = 0;
				int i2 = 1;
				char c2;
				c2 = cur_quat->Q[3][1];

				while (c2 != ')')
				{
					int j2 = 0;

					while (c2 != '|' && c2 != ')')
					{
						b[k2][j2] = c2;
						i2++;
						j2++;
						c2 = cur_quat->Q[3][i2];
						if (c2 == '|' || c2 == ')')
						{
							break;
						}

					}
					k2++;

					if (c2 == ')')
					{
						break;
					}

					i2++;
					c2 = cur_quat->Q[3][i2];
				}
				for (int i = 0; i < k2; i++)
				{
					fprintf(fp, "MOV DX,%s\nMOV %s,DX\n", b[i], a[i]);
					printf("MOV DX,%s\nMOV %s,DX\n", b[i], a[i]);
				}
				fprintf(fp, "CALL %s\n", cur_quat->Q[1]);
				printf("CALL %s\n", cur_quat->Q[1]);

				for (int i = 0; i < digui_num; i++)
				{
					if (strcmp(digui[i], cur_quat->Q[1]) == 0)
					{
						block* b = cur_block;
						quat* q = cur_quat;
						while (b != NULL)
						{
							q = b->q_head;
							while (q != NULL)
							{
								if (strcmp(q->Q[0], "f") == 0 && strcmp(q->Q[1], "end") == 0)
									break;
								q = q->next;
							}
							if (q == NULL)
							{
								b = b->next;
								continue;
							}
							if (strcmp(q->Q[0], "f") == 0 && strcmp(q->Q[1], "end") == 0)
								break;
						}
						if (strcmp(q->Q[2], cur_quat->Q[1]) == 0)
						{
							for (int n = k; n > 0; n--)
							{
								fprintf(fp, "POP %s\n", a[n - 1]);
								printf("POP %s\n", a[n - 1]);
							}
						}

					}
				}
			}
		}

		if (cur_quat->next != NULL)
		{
			cur_quat = cur_quat->next;
		}
		else
		{
			mark++;
			fprintf(fp, "block%d:", mark);
			printf("block%d:", mark);
			if (cur_block->next != NULL)
			{
				cur_block = cur_block->next;
				cur_quat = cur_block->q_head;
			}
			else
			{
				fprintf(fp, "CALL main\n");
				printf("CALL main\n");
				break;
			}
		}

	}
	fprintf(fp, "\nMOV AH,4CH\nINT 21H\nCSEG ENDS\nEND START\n");
	printf("\nMOV AH,4CH\nINT 21H\nCSEG ENDS\nEND START\n");
	fclose(fp);
}

void code_top(block* head)
{
	block* cur_block = head;
	quat* cur_quat = cur_block->q_head;
	quat* temp_quat = cur_quat;

	/*去除声明语句,存进标识符数组*/
	int arr_mark = 0;
	int i = 0;
	while (cur_block != NULL)
	{
		cur_quat = cur_block->q_head;
		while (cur_quat != NULL)
		{
			if (strcmp(cur_quat->Q[0], "int") == 0 || strcmp(cur_quat->Q[0], "char") == 0)
			{

				if (cur_quat->Q[1][0] != '(')	//不是数组
				{
					if (i > 0)
					{
						int j = 0;
						for (j ; j < i; j++)
						{
							if (strcmp(biaoshifu[j], cur_quat->Q[1]) == 0)
								break;
						}
						if (strcmp(biaoshifu[j], cur_quat->Q[1]) == 0)
						{
							int block_mark = 1;
							arr_mark++;
							char t1[5] = { 0 };
							char t2[20] = { 0 };
							itoa(arr_mark, t1, 10);
							strcpy(t2, cur_quat->Q[1]);
							strcat(t2, t1);
							quat* q1 = cur_quat->next;
							while (q1 != NULL)
							{
								if (strcmp(q1->Q[1], cur_quat->Q[1]) == 0)
								{
									memset(q1->Q[1], '\0', sizeof(q1->Q[1]));
									strcpy(q1->Q[1], t2);
								}
								else if (strcmp(q1->Q[2], cur_quat->Q[1]) == 0)
								{
									memset(q1->Q[2], '\0', sizeof(q1->Q[2]));
									strcpy(q1->Q[2], t2);
								}
								else if (strcmp(q1->Q[3], cur_quat->Q[1]) == 0)
								{
									memset(q1->Q[3], '\0', sizeof(q1->Q[3]));
									strcpy(q1->Q[3], t2);
								}

								if (q1->Q[1][0]=='(')
								{
									char name[30] = { 0 };
									char num[20] = { 0 };
									int j = 1;
									while (q1->Q[1][j] != '|')
									{
										name[j - 1] = q1->Q[1][j];
										j++;
									}
									j++;
									int k = 0;
									while (q1->Q[1][j] != ')')
									{
										num[k] = q1->Q[1][j];
										k++;
										j++;
									}
									if (strcmp(num, cur_quat->Q[1]) == 0)
									{
										strcat(num, t1);
										char newch[30] = { 0 };
										strcat(newch, "(");
										strcat(newch, name);
										strcat(newch, "|");
										strcat(newch, num);
										strcat(newch, ")");
										memset(q1->Q[1], '\0', sizeof(q1->Q[1]));
										memcpy(q1->Q[1], newch,30);
									}
								}
								else if (q1->Q[2][0] == '(')
								{

									char name[30] = { 0 };
									char num[20] = { 0 };
									int j = 1;
									while (q1->Q[2][j] != '|')
									{
										name[j - 1] = q1->Q[2][j];
										j++;
									}
									j++;
									int k = 0;
									while (q1->Q[2][j] != ')')
									{
										num[k] = q1->Q[2][j];
										k++;
										j++;
									}
									if (strcmp(num, cur_quat->Q[1]) == 0)
									{
										strcat(num, t1);
										char newch[30] = { 0 };
										strcat(newch, "(");
										strcat(newch, name);
										strcat(newch, "|");
										strcat(newch, num);
										strcat(newch, ")");
										memset(q1->Q[2], '\0', sizeof(q1->Q[2]));
										memcpy(q1->Q[2], newch,30);
									}
								}
								else if (q1->Q[3][0] == '(')
								{
									char name[30] = { 0 };
									char num[20] = { 0 };
									int j = 1;
									while (q1->Q[3][j] != '|')
									{
										name[j - 1] = q1->Q[3][j];
										j++;
									}
									j++;
									int k = 0;
									while (q1->Q[3][j] != ')')
									{
										num[k] = q1->Q[3][j];
										k++;
										j++;
									}
									if (strcmp(num, cur_quat->Q[1]) == 0)
									{
										strcat(num, t1);
										char newch[30] = { 0 };
										strcat(newch, "(");
										strcat(newch, name);
										strcat(newch, "|");
										strcat(newch, num);
										strcat(newch, ")");
										memset(q1->Q[3], '\0', sizeof(q1->Q[3]));
										memcpy(q1->Q[3], newch, 30);
									}
								}
								q1 = q1->next;
							}
							block* b1 = cur_block->next;
							if (b1 != NULL)
							{
								q1 = b1->q_head;
								while (b1 != NULL)
								{
									q1 = b1->q_head;
									while (q1 != NULL)
									{

										if (strcmp(q1->Q[1], cur_quat->Q[1]) == 0)
										{
											memset(q1->Q[1], '\0', sizeof(q1->Q[1]));
											strcpy(q1->Q[1], t2);
										}
										else if (strcmp(q1->Q[2], cur_quat->Q[1]) == 0)
										{
											memset(q1->Q[2], '\0', sizeof(q1->Q[2]));
											strcpy(q1->Q[2], t2);
										}
										else if (strcmp(q1->Q[3], cur_quat->Q[1]) == 0)
										{
											memset(q1->Q[3], '\0', sizeof(q1->Q[3]));
											strcpy(q1->Q[3], t2);
										}

										if (q1->Q[1][0] == '(')
										{
											char name[30] = { 0 };
											char num[20] = { 0 };
											int j = 1;
											while (q1->Q[1][j] != '|')
											{
												name[j - 1] = q1->Q[1][j];
												j++;
											}
											j++;
											int k = 0;
											while (q1->Q[1][j] != ')')
											{
												num[k] = q1->Q[1][j];
												k++;
												j++;
											}
											if (strcmp(num, cur_quat->Q[1]) == 0)
											{
												strcat(num, t1);
												char newch[30] = { 0 };
												strcat(newch, "(");
												strcat(newch, name);
												strcat(newch, "|");
												strcat(newch, num);
												strcat(newch, ")");
												memset(q1->Q[1], '\0', sizeof(q1->Q[1]));
												memcpy(q1->Q[1], newch, 30);
											}
										}
										else if (q1->Q[2][0] == '(')
										{

											char name[30] = { 0 };
											char num[20] = { 0 };
											int j = 1;
											while (q1->Q[2][j] != '|')
											{
												name[j - 1] = q1->Q[2][j];
												j++;
											}
											j++;
											int k = 0;
											while (q1->Q[2][j] != ')')
											{
												num[k] = q1->Q[2][j];
												k++;
												j++;
											}
											if (strcmp(num, cur_quat->Q[1]) == 0)
											{
												strcat(num, t1);
												char newch[30] = { 0 };
												strcat(newch, "(");
												strcat(newch, name);
												strcat(newch, "|");
												strcat(newch, num);
												strcat(newch, ")");
												memset(q1->Q[2], '\0', sizeof(q1->Q[2]));
												memcpy(q1->Q[2], newch, 30);
											}
										}
										else if (q1->Q[3][0] == '(')
										{

											char name[30] = { 0 };
											char num[20] = { 0 };
											int j = 1;
											while (q1->Q[3][j] != '|')
											{
												name[j - 1] = q1->Q[3][j];
												j++;
											}
											j++;
											int k = 0;
											while (q1->Q[3][j] != ')')
											{
												num[k] = q1->Q[3][j];
												k++;
												j++;
											}
											if (strcmp(num, cur_quat->Q[1]) == 0)
											{
												strcat(num, t1);
												char newch[30] = { 0 };
												strcat(newch, "(");
												strcat(newch, name);
												strcat(newch, "|");
												strcat(newch, num);
												strcat(newch, ")");
												memset(q1->Q[3], '\0', sizeof(q1->Q[3]));
												memcpy(q1->Q[3], newch, 30);
											}
										}

										if (strcmp(q1->Q[0], "for") == 0 || strcmp(q1->Q[0], "while") == 0 || strcmp(q1->Q[0], "if") == 0)
										{
											block_mark++;
										}
										else if (strcmp(q1->Q[0], "fe") == 0 || strcmp(q1->Q[0], "we") == 0 || strcmp(q1->Q[0], "ie") == 0 || (strcmp(q1->Q[0], "f") == 0 && strcmp(q1->Q[1], "end") == 0))
										{
											block_mark--;
										}
										q1 = q1->next;
										if (block_mark == 0)
											break;
									}
									if (q1 == NULL)
									{
										b1 = b1->next;
										continue;
									}

									if (block_mark == 0)
										break;
								}
							}
							memset(cur_quat->Q[1], '\0', sizeof(cur_quat->Q[1]));
							strcpy(cur_quat->Q[1], t2);
						}
						
					}

					temp_quat = cur_quat;

					strcpy(biaoshifu[i], cur_quat->Q[1]);
					cur_quat = cur_quat->next;

					delete_quat(cur_block, temp_quat);
					i++;
				}
				else
				{
					char name[30] = { 0 };
					char num[20] = { 0 };
					int j = 1;
					while (cur_quat->Q[1][j] != '|')
					{
						name[j - 1] = cur_quat->Q[1][j];
						j++;
					}
					j++;
					int k = 0;
					while (cur_quat->Q[1][j] != ')')
					{
						num[k] = cur_quat->Q[1][j];

						k++;
						j++;
					}

					if (num[0] >= '0' && num[0] <= '9')	//数组长度为定值
					{

					}
					else	//数组长度为变量
					{

						block* temp_b = head;
						quat* temp_q = temp_b->q_head;
						while (temp_b != NULL)
						{
							temp_q = temp_b->q_head;
							while (temp_q != NULL)
							{
								if (strcmp(temp_q->Q[3], num) == 0)
								{
									break;
								}

								temp_q = temp_q->next;
							}
							if (temp_q == NULL)
							{
								temp_b = temp_b->next;
								continue;
							}
							if (strcmp(temp_q->Q[3], num) == 0)
								break;


						}
						memset(num, '\0', sizeof(num));
						strcpy(num, temp_q->Q[1]);
					}

					temp_quat = cur_quat;
					strcat(num, name);
					strcpy(biaoshifu[i], num);
					cur_quat = cur_quat->next;
					delete_quat(cur_block, temp_quat);

					i++;
				}

			}
			else if (strcmp(cur_quat->Q[0], "do") == 0)
			{
				temp_quat = cur_quat;
				cur_quat = cur_quat->next;
				delete_quat(cur_block, temp_quat);
			}
			else
				cur_quat = cur_quat->next;
		}

		if (cur_block->next == NULL)
			break;
		else
			cur_block = cur_block->next;
	}

	data_seg(biaoshifu);
	code_seg(head);
}

void code()
{
	head = (block*)malloc(sizeof(block));
	head->before = head;
	head->next = NULL;

	quat* temp_q = (quat*)malloc(sizeof(quat));
	head->q_head = temp_q;
	head->q_head->before = head->q_head;
	head->q_head->next = NULL;

	temp_b = head;

	read_quat();

	while (temp_b != NULL)
	{
		find_functions();
	}

	write_list(head);

	code_top(head);
}

int main()
{
    Create_Quaternion();
	code();
	return 0;
}