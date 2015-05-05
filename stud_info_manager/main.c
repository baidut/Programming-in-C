/*
 * Student Information Manager
 * https://baidut.github.io/ | Released under MIT license
 * Copyright (c) 2015 Zhenqiang.Ying <yingzhenqiang@gmail.com>
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX      100
#define NAME_LEN 10

typedef struct {
    char name[NAME_LEN];
    unsigned int score[3];
    unsigned int sum;
} Stu_t;

Stu_t   stu[MAX];
int     num;

Stu_t   input_stu(void);                    // 通过提示引导用户正确地输入一条学生记录，并返回这条记录
void    output_stu(int i);                  // 根据数组索引打印出对应的学生信息
int     find_stu(void);                     // 通过提示引导用户查找记录，如果找到则显示查找到的记录，并返回数组索引，否则返回-1

void    read_stu(const char *filename);     // 根据文件名读入文件中的学生数据
void    write_stu(const char *filename);    // 将学生数据写入到指定的文件中

int     confirm(const char *words);         // 请求用户确认，输入y或Y返回1，输入n或N返回0，其他情况提示用户重新输入
void    safe_flush(FILE *fp);               // safe_flush(stdin)清理多余的输入，防止影响下次获取输入

int main(void) {
    const char *filename = "stud_info.txt";

    read_stu(filename); // 导入数据
    printf("欢迎使用学生成绩管理系统\n"
           "-当前已有%d条记录,本程序限制%d条记录\n",
           num, MAX);

    for (;;) {
    char choice;
    int i, j;

        printf( "-菜单:\t"
                "1.查找\t"
                "2.追加\t"
                "3.删除\t"
                "4.修改\t"
                "5.排序\t"
                "6.显示\t"
                "7.保存\t"
                "0.退出\n"
                "输入序号:");

        choice = getchar();
        safe_flush(stdin);

        switch (choice) {
        case '1': // 查找
            find_stu();
            break;
        case '2': // 追加
            if (num < MAX) {
                stu[num] = input_stu();
                output_stu(num);
                num++;
                puts("追加成功!");
            }
            else puts("记录已满，无法追加！");
            break;
        case '3': // 删除
            i = find_stu();
            if (-1 != i) {
                if (confirm("确认删除？")) {
                    while (i++ != num) {
                        stu[i-1] = stu[i];
                    }
                    num--;
                    puts("删除成功!");
                }
            }
            break;
        case '4': // 修改
            i = find_stu();
            if (-1 != i) {
                stu[i] = input_stu();
                output_stu(i);
                puts("修改成功！");
            }
            break;
        case '5': // 按照总分排序
            for (i = 0; i < num-1; i++)     // 冒泡排序中已经排好的
                for (j=0; j < num-1-i; j++) // 遍历没排好的
                    if ( stu[j].sum < stu[j+1].sum ) {
                        Stu_t t = stu[j];
                        stu[j] = stu[j+1];
                        stu[j+1] = t;
                    }
            puts("排序成功！");
            /* fall through */     // 完成排序后自动显示
        case '6': // 显示
            for (i = 0; i < num; i++) {
                printf("%4d:\t", i+1);
                output_stu(i);
            }
            break;
        case '7': // 保存
            write_stu(filename);
            break;
        case '0': // 退出
            if (confirm("推出前保存？（Y/N）")) {
                write_stu(filename);
            }
            exit(0);
        default:
            puts("没有该序号！请重新输入");
            break;
        }
    } // END for

    return 0;
}

void safe_flush(FILE *fp){
    int ch;
    while ( ch = fgetc(fp), ch != EOF && ch != '\n');
}

int confirm(const char *words) {
    char choice;
    puts(words);
    for (;;) {
        choice = getchar();
        safe_flush(stdin);

        switch (choice) {
        default:
            puts("输入非法，重新输入！");
            continue;
        case 'Y':
        case 'y':
            puts("已确认！");
            return 1;
        case 'N':
        case 'n':
            puts("已取消！");
            return 0;
        }
    }
}

Stu_t input_stu(void) {
    Stu_t ret;
    puts("依次输入姓名、语文、数学、外语成绩，空格分隔，成绩要求是整数:");
    while ( 4 != scanf("%s%d%d%d", ret.name,
                                  &ret.score[0],
                                  &ret.score[1],
                                  &ret.score[2])) {
        safe_flush(stdin);
        puts("输入有误，请重新输入!");
    }
    safe_flush(stdin);
    ret.sum = ret.score[0] + ret.score[1] + ret.score[2];
    return ret;
}

void output_stu(int i) {
    if (i < 0 || i > MAX) return;
    printf("%s\t语:%d\t数:%d\t外:%d\t总分：%d\n",
                                stu[i].name,
                                stu[i].score[0],
                                stu[i].score[1],
                                stu[i].score[2],
                                stu[i].sum);
}

int find_stu(void) {
    char name[NAME_LEN];
    int i;
    printf("输入姓名（限%d个字符）：", NAME_LEN);
    scanf("%s", name);
    safe_flush(stdin);
    for (i = 0; i < num; i++) {
        if (0 == strcmp(stu[i].name, name)) {
            output_stu(i);
            return i;
        }
    }
    puts("没有找到记录!");
    return -1;
}

void read_stu(const char *filename) {
    FILE *fp = fopen(filename, "rt");
    if (NULL != fp) {
        fread(&num, sizeof(int), 1, fp);
        fread(stu, sizeof(Stu_t), num, fp);
    }
    fclose(fp);
}

void write_stu(const char* filename) {
    FILE *fp = fopen(filename, "wt");
    if (NULL != fp) {
        fwrite(&num, sizeof(int), 1, fp);
        fwrite(stu, sizeof(Stu_t), num, fp);
        fclose(fp);
        puts("保存成功!");
    }
    else puts("打开文件失败！");
}
