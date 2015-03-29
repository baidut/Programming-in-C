#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX      100
#define NAME_LEN 10

typedef struct{
	char name[NAME_LEN];
	unsigned int score[3];
	unsigned int sum;
}Stu_t;

Stu_t 	stu[100];
int     num;

Stu_t 	input_stu(void);    // ���������ѧ����Ϣ
void 	output_stu(int i);  // ��������������ʾ��Ӧ��ѧ����Ϣ
int 	find_stu(void);     // ���Ҳ���ʾ���ݣ�������������

void    read_stu(const char* filename);
void    write_stu(const char* filename);

int     confirm(const char* words);

void safe_flush(FILE *fp) {
    int ch;
    while( (ch = fgetc(fp)) != EOF && ch != '\n' );
}

int main(void) {
    const char* filename = "stud_info.txt";
    read_stu(filename);// ��ȡ����
    printf("��ӭʹ��ѧ���ɼ�����ϵͳ\n"
           "-��ǰ����%d����¼,����������%d����¼\n"
           ,num,MAX);
    // ���˵�
    for(;;){
    char choice;
    int i,j;

        printf( "-�˵�:\t"
                "1.����\t"
                "2.׷��\t"
                "3.ɾ��\t"
                "4.�޸�\t"
                "5.����\t"
                "6.��ʾ\t"
                "7.����\t"
                "0.�˳�\n"
                "�������:");

        choice = getchar();
        safe_flush(stdin);

        switch(choice){
            case '1': // ����
                find_stu();
                break;
            case '2': // ׷��
                if(num<MAX){
                    stu[num] = input_stu();
                    output_stu(num);
                    num++;
                    puts("׷�ӳɹ�!");
                }
                else puts("��¼�������޷�׷�ӣ�");
                break;
            case '3': // ɾ��
                i = find_stu();
                if(-1!=i){
                    if( confirm("ȷ��ɾ����") ){
                        while(i++ != num){
                            stu[i-1] = stu[i];
                        }
                        num--;
                        puts("ɾ���ɹ�!");
                    }
                }
                break;
            case '4': // �޸�
                i = find_stu();
                if(-1!=i){
                    stu[i] = input_stu();
                    output_stu(i);
                    puts("�޸ĳɹ���");
                }
                break;
            case '5': // �����ܷ�����
                for(i = 0;i < num-1;i++) // ð�������Ѿ��źõ�
                    for(j = 0;j < num-1-i;j++) // ����û�źõ�
                        if( stu[j].sum < stu[j+1].sum ) {
                            Stu_t t = stu[j];
                            stu[j] = stu[j+1];
                            stu[j+1] = t;
                        }
                puts("����ɹ���");
                /* fall through */     // ���������Զ���ʾ
            case '6': // ��ʾ
                for(i = 0;i < num;i++){
                    printf("%4d:\t",i+1);
                    output_stu(i);
                }
                break;
            case '7':
                write_stu(filename);
                break;
            case '0':
                if( confirm("�Ƴ�ǰ���棿��Y/N��") ){
                    write_stu(filename);
                }
                exit(0);
            default:
                puts("û�и���ţ�����������");
                break;
        }
    } // END FOR

	return 0;
}

int confirm(const char* words){
    char choice;
    puts(words);
    for(;;){
        choice = getchar();
        safe_flush(stdin);

        switch(choice){
            default:
                puts("����Ƿ����������룡");
                continue;
            case 'Y':
            case 'y':
                puts("��ȷ�ϣ�");
                return 1;
            case 'N':
            case 'n':
                puts("��ȡ����");
                return 0;
        }
    }
}

Stu_t input_stu(void) {
	Stu_t ret;
	puts("�����������������ġ���ѧ������ɼ����ո�ָ����ɼ�Ҫ��������:");
	while( 4 != scanf("%s%d%d%d",ret.name,
								 &ret.score[0],
			                     &ret.score[1],
			                     &ret.score[2] )){
        safe_flush(stdin);
        puts("������������������!");
    }
    safe_flush(stdin);// ����س�����Ⱦ
    ret.sum = ret.score[0] + ret.score[1] + ret.score[2];
	return ret;
}

void output_stu(int i) {
	if(i < 0 || i > MAX) return;
	printf("%s\t��:%d\t��:%d\t��:%d\t�ܷ֣�%d\n",
								stu[i].name,
								stu[i].score[0],
								stu[i].score[1],
								stu[i].score[2],
								stu[i].sum);
}

int find_stu(void) {
    char name[NAME_LEN];
    int i;
	printf("������������%d���ַ�����",NAME_LEN);
	scanf("%s",name);
	safe_flush(stdin);
	for(i = 0;i < num;i++){
		if(0==strcmp(stu[i].name,name)){
			output_stu(i);
			return i;
		}
	}
	puts("û���ҵ���¼!");
	return -1;
}
void read_stu(const char* filename){
    FILE* fp = fopen(filename,"rt");
    if(NULL != fp) {
        fread( &num,sizeof(int),1,fp);
        fread( stu,sizeof(Stu_t),num,fp);
    }
    fclose(fp);
}

void write_stu(const char* filename) {
    FILE* fp = fopen(filename,"wt");
    if(NULL != fp) {
        fwrite( &num,sizeof(int),1,fp);
        fwrite( stu,sizeof(Stu_t),num,fp);
        fclose(fp);
        puts("����ɹ�!");
    }
    else puts("���ļ�ʧ�ܣ�");
}
